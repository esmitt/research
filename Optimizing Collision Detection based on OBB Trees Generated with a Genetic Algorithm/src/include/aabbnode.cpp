#include "aabbnode.h"

AABBNode::AABBNode()
{
 nt = 0;
 t = NULL;
 left = right = NULL;
}

AABBNode::AABBNode(OFF *obj)
{
 BB.init(obj);
 nt = obj->nf;
 t = new int[nt];
 for(int i=0; i<nt; i++)
  t[i] = i;
 left = right = NULL;
}

AABBNode::AABBNode(OFF *obj, int n, int *tri)
{
 BB.init(obj, n, tri);
 nt = n;
 t = tri;
 left = right = NULL;
}


AABBNode::~AABBNode()
{
 if (t) delete [] t;
 if (left) delete left;
 if (right) delete right;
}

int AABBNode::size()
{
 return BB.size() + sizeof(left) + sizeof(right) + sizeof(nt) + sizeof(t) +
  (left?left->size():0) + (right?right->size():0) + (t?(nt*sizeof(int)):0);
}

inline bool AABBNode::readchild(FILE *f, pAABBNode &n)
{
 char child;
 int r = fread(&child, 1, 1, f);
 assertbytesread(r, 1)
 if (child=='\1')
 {
  pAABBNode aux = new AABBNode();
  n = aux;
  //n = new AABBNode();
  return n->read(f);
 }
 else if (child!='\0') return false;
 return true;
}

bool AABBNode::read(FILE *f)
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

void AABBNode::write(FILE *f)
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

void AABBNode::render()
{
#ifndef _NOT_OPENGL_
 if (nt) BB.render();
 if(left) left->render();
 if(right) right->render();
#endif
}


