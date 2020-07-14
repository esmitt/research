#include "octreenode.h"
#include "AABB.h"

OctTreeNode::OctTreeNode()
{
 nt = 0;
 t = NULL;
 for(int i=0; i<8; i++) child[i] = NULL;
}

OctTreeNode::OctTreeNode(f3 ori, f3 sizes)
{
 copyf3(this->ori, ori);
 copyf3(this->sizes, sizes);
 nt = 0;
 t = NULL;
 for(int i=0; i<8; i++) child[i] = NULL;
}

OctTreeNode::~OctTreeNode()
{
 if (t) delete [] t;
 for(int i=0; i<8; i++)
  if (child[i]) delete child[i];
}

OctTreeNode::OctTreeNode(OFF *obj)
{
 init(obj);
}

OctTreeNode::OctTreeNode(OFF *obj, int n, int *tri)
{
 init(obj, n, tri);
}

void OctTreeNode::init(OFF *obj)
{
 AABB BB(obj);
 nt = obj->nf;
 t = new int[nt];
 for(int i=0; i<nt; i++)
  t[i] = i;
 copyf3(ori, BB.min);
 BB.getSize(sizes);
 for(i=0; i<8; i++) child[i] = NULL;
}

void OctTreeNode::init(OFF *obj, int n, int *tri)
{
 AABB BB(obj, n, tri);
 nt = n;
 t = tri;
 copyf3(ori, BB.min);
 BB.getSize(sizes);
 for(int i=0; i<8; i++) child[i] = NULL;
}


int OctTreeNode::size()
{
 int ac = 2*3*sizeof(float) + sizeof(nt) + sizeof(t) + 8*sizeof(OctTreeNode *);
 for(int i=0; i<8; i++)
  if (child[i]) ac += child[i]->size();
 if (t) ac += nt * sizeof(int);
 return ac;
}

void OctTreeNode::write(FILE *f)
{
 fwrite(ori, sizeof(float), 3, f); // ori
 fwrite(sizes, sizeof(float), 3, f); // sizes

 fwrite(&nt, sizeof(int), 1, f);
 if (nt)
  fwrite(t, sizeof(int), nt, f);

 for(int i=0; i<8; i++)
 {
  if (child[i])
  {
   fwrite("\1", 1, 1, f);
   child[i]->write(f);
  }
  else
   fwrite("\0", 1, 1, f);
 }
}

bool OctTreeNode::read(FILE *f)
{
 int r;
 r = fread(ori, sizeof(float), 3, f);
 assertbytesread(r, 3);
 r = fread(sizes, sizeof(float), 3, f);
 assertbytesread(r, 3);

 r = fread(&nt, sizeof(int), 1, f);
 assertbytesread(r, 1)

 if (nt)
 {
  t = new int [nt];
  r = fread(t, sizeof(int), nt, f);
  assertbytesread(r, nt)
 }

 for(int i=0; i<8; i++)
 {
  bool r = readchild(f, child[i]);
  if (!r) return false;
 }

 return true;
}

inline bool OctTreeNode::readchild(FILE *f, pOctTreeNode &n)
{
 char childExists;
 int r = fread(&childExists, 1, 1, f);
 assertbytesread(r, 1)
 if (childExists=='\1')
 {
  pOctTreeNode aux = new OctTreeNode();
  n = aux;
  return n->read(f);
 }
 else if (childExists!='\0') return false;
 return true;
}

void OctTreeNode::render()
{
#ifndef _NOT_OPENGL_
 if (nt)
 {
  f3 v2;
  addf3(v2, ori, sizes);
  AABB bb(ori, v2);
  bb.render();
 }
 else
 {
  for(int i=0; i<8; i++)
   if (child[i]) child[i]->render();
 }
#endif
}

bool OctTreeNode::anyVertexInside(OFF *obj, int t)
{
 return vertexInside(obj->vertex[obj->face[t][0]]) ||
        vertexInside(obj->vertex[obj->face[t][1]]) ||        
        vertexInside(obj->vertex[obj->face[t][2]]);
}

bool OctTreeNode::vertexInside(f3 v)
{
 return ori[0] <= v[0] && v[0] <= ori[0]+sizes[0] &&
        ori[1] <= v[1] && v[1] <= ori[1]+sizes[1] &&
        ori[2] <= v[2] && v[2] <= ori[2]+sizes[2];
}
