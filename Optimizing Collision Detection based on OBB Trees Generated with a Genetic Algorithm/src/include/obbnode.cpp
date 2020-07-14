#include "OBBNode.h"

OBBNode::OBBNode()
{
 nt = 0;
 t = NULL;
 left = right = NULL;
}

OBBNode::OBBNode(OFF *obj)
{
 BB.init(obj);
 nt = obj->nf;
 t = new int[nt];
 for(int i=0; i<nt; i++)
  t[i] = i;
 left = right = NULL;
}

OBBNode::OBBNode(OFF *obj, int n, int *tri)
{
 BB.init(obj, n, tri);
 nt = n;
 t = tri;
 left = right = NULL;
}


OBBNode::~OBBNode()
{
 if (t) delete [] t;
 if (left) delete left;
 if (right) delete right;
}

int OBBNode::size()
{
 return BB.size() + sizeof(left) + sizeof(right) + sizeof(nt) + sizeof(t) +
  (left?left->size():0) + (right?right->size():0) + (t?(nt*sizeof(int)):0);
}

void OBBNode::assignColor()
{
 if (nt)
 {
  r = (float)rand()/(float)RAND_MAX;
  g = (float)rand()/(float)RAND_MAX;
  b = (float)rand()/(float)RAND_MAX;
 }
 if (left) left->assignColor();
 if (right) right->assignColor();

}

inline bool OBBNode::readchild(FILE *f, pOBBNode &n)
{
 char child;
 int r = fread(&child, 1, 1, f);
 assertbytesread(r, 1)
 if (child=='\1')
 {
  pOBBNode aux = new OBBNode();
  n = aux;
  //n = new AABBNode();
  return n->read(f);
 }
 else if (child!='\0') return false;
 return true;
}

bool OBBNode::read(FILE *f)
{
 int r;
 if (!BB.read(f)) return false;
 r = fread(&nt, sizeof(int), 1, f);
 assertbytesread(r, 1)
 if (nt)
 {
  t = new int [nt];
  r = fread(t, sizeof(int), nt, f);
  assertbytesread(r, nt)
 }
 if (!readchild(f, left)) return false;
 return readchild(f, right);
}

void OBBNode::write(FILE *f)
{
 BB.write(f);
 fwrite(&nt, sizeof(int), 1, f);
 if (nt)
  fwrite(t, sizeof(int), nt, f);

 if (left)
 {
  fwrite("\1", 1, 1, f); // a continuacion viene el arbol izq!
  left->write(f);
 }
 else
  fwrite("\0", 1, 1, f);

 if (right)
 {
  fwrite("\1", 1, 1, f); // a continuacion viene el arbol der!
  right->write(f);
 }
 else
  fwrite("\0", 1, 1, f);
}

void OBBNode::render()
{
#ifndef _NOT_OPENGL_
if (nt) BB.render();
 if(left) left->render();
 if(right) right->render();
#endif
}

void OBBNode::render(int l)
{
	if(l==0)BB.render();
	else{
if (nt) BB.render();

if(left) left->render(l-1);
 if(right) right->render(l-1);
	}
}

void OBBNode::renderColors(OFF *obj)
{
#ifndef _NOT_OPENGL_
 if (nt>0)
 {
  // render de los triangulos:
  obj->renderColor(t, nt, r, g, b);
  //obj->savePolys("polys.off", t, nt);
 }
 if (left) left->renderColors(obj);
 if (right) right->renderColors(obj);
#endif
}

double OBBNode::GetVolumeValue()
{
	if (isLeaf())
		return BB.GetVolumeValue();
	else
	{
		return (right)?right->GetVolumeValue():0 + (left)?left->GetVolumeValue():0;
	}
}
