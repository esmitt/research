#include <list>
#include <string.h>
#include "aabbtree.h"

using namespace std;

AABBTree::AABBTree()
{
 root = NULL;
}

AABBTree::AABBTree(OFF *obj)
{
 root = new AABBNode(obj);
 root->nt = obj->nf;
 root->t = new int[root->nt];
 for(int i=0; i<root->nt; i++)
  root->t[i] = i;
 buildTree(obj, root);
}

AABBTree::~AABBTree()
{
 release();
}

void AABBTree::buildTree(OFF *obj, AABBNode *root)
{
 if (root)
 {
  int n = root->nt;
  int *t = root->t;
  f3 bbsize, bbcenter;
  root->BB.getSize(bbsize);
  root->BB.getCenter(bbcenter);
  
  // buscar el eje mayor:
  int axis[3];

  if (bbsize[0] > bbsize[1])
  {
   if (bbsize[2] > bbsize[0])
   {
    axis[0] = 2;
    axis[1] = 0;
    axis[2] = 1;
   }
   else
   {
    axis[0] = 0;
    if (bbsize[1] > bbsize[2])
    {
     axis[1] = 1;
     axis[2] = 2;
    }
    else
    {
     axis[1] = 2;
     axis[2] = 1;
    }
   }
  }
  else
  {
   if (bbsize[2] > bbsize[1])
   {
    axis[0] = 2;
    axis[1] = 1;
    axis[2] = 0;
   }
   else
   {
    axis[0] = 1;
    if (bbsize[2] > bbsize[0])
    {
     axis[1] = 2;
     axis[2] = 0;
    }
    else
    {
     axis[1] = 0;
     axis[2] = 2;
    }
   }
  }
  // ejes ordenados en axis = axis!
  list <int> left, right;
  AABB *leftBB=NULL, *rightBB=NULL;
  leftBB = new AABB();
  rightBB = new AABB();

  int c1, c2, c3;

#define maxintento 5

  float intentos[maxintento] = {0.5f, 0.25f, 0.75f, 0.1f, 0.9f};

  for(int axisNow = 0; axisNow<3; axisNow++)
  {   
   for(int intento = 0; intento < maxintento; intento++)
   {
    c1 = c2 = c3 = 0;
    
    left.clear();
    right.clear();
    
    root->BB.split(axis[axisNow], leftBB, rightBB, intentos[intento]);
    
    for(int i=0; i<n; i++)
    {
     int cl = leftBB->countVertex(obj, obj->face[t[i]]);//(obj, t[i], &root->BB);
     
     switch(cl)
     {
     case 3:   // izq
      left.push_back(t[i]);c1++;
      break;
     case 0:    // der
      right.push_back(t[i]);c2++;
      break;
     default:    // compartido
      left.push_back(t[i]);
      right.push_back(t[i]);c3++;
      break;
     }
    }
    
    if (c1 && c2)  // la division por este eje esta bien!
     break;
   }
   if (c1 && c2) break;
  }
  // ya se repartieron los triangulos

  if (axisNow==3)
  { // ya probamos todos los ejes==>ninguno sirve!
   if (root->nt==98) obj->savePolys("polys.off", root->t, root->nt);

   return;
  }
  
  int ln = left.size(), rn = right.size();
  AABBNode *leftChild = NULL, *rightChild = NULL;
  
  if (ln==0) 
  {
   delete leftBB;
   delete rightBB;
   root->left = root->right = NULL;
   return;
  }
  else
  {
   int *tl = new int[ln];
   list <int>::iterator it = left.begin();
   int i=0;
   while(it!=left.end())
   {
    tl[i++] = *it++;
   }
   leftChild = new AABBNode(obj, ln, tl);
  }
  
  if (rn==0)
  {
   delete leftBB;
   delete rightBB;
   root->left = root->right = NULL;
   if (leftChild) delete leftChild;
   return;
  }
  else
  {
   int *tr = new int[rn];
   list <int>::iterator it = right.begin();
   int i=0;
   while(it!=right.end())
   {
    tr[i++] = *it++;
   }
   rightChild = new AABBNode(obj, rn, tr);
  }
  
  root->left = leftChild;
  root->right = rightChild;
  
  delete [] root->t;
  root->t = NULL;
  root->nt = 0;
  
  if (root->left) buildTree(obj, leftChild);
  if (root->right) buildTree(obj, rightChild);
  }
}

int AABBTree::clasifyTriangle(OFF *obj, int t, AABB *BB)
{
 int l=0, r=0;
 for(int i=0; i<3; i++)
  if (BB->inside(obj->vertex[obj->face[t][i]]))
   l++;
  else
   r++;
 
  return l==0 ? 1 :       // 1 si todos a la derecha
         r==0 ? -1 :      // -1 todos a la izq
         0;               // 0 compartido!
}

int AABBTree::size()
{
 return sizeof(root) + (root?root->size():0);
}

void AABBTree::write(FILE *f)
{
 fwrite("AABBTREE", 1, 8, f);
 if (root) root->write(f);
}

void AABBTree::write(char *fname)
{
 FILE *f = fopen(fname, "wb");
 write(f);
 fclose(f);
}

bool AABBTree::read(FILE *f)
{
 char magic[9];
 magic[8] = '\0';
 int r = fread(magic, 1, 8, f);
 assertbytesread(r,8)
 if (strcmp(magic,"AABBTREE")) return false;
 
 release();

 root = new AABBNode();
 return root->read(f);
}

bool AABBTree::read(char *fname)
{
 FILE *f = fopen(fname, "rb");
 if (!f) return false;
 if (!read(f))
 {
  fclose(f);
  return false;
 }
 fclose(f);
 return true;
}

void AABBTree::release()
{
 if (root) delete root;
 root = NULL;
}

void AABBTree::render()
{
 #ifndef _NOT_OPENGL_
 if (root) root->render();
#endif
}

int AABBTree::getWorst()
{
 int c = 0;
 getWorst(root, c);
 return c;
}

int AABBTree::getAverage()
{
 return getTotal() / countLeaves();
}

int AABBTree::getTotal()
{
 int c = 0;
 getTotal(root, c);
 return c;
}

int AABBTree::countLeaves()
{
 int c = 0;
 countLeaves(root, c);
 return c;
}

void AABBTree::countLeaves(AABBNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c++;
   return;
  }
  countLeaves(root->left, c);
  countLeaves(root->right, c);
 }
}

void AABBTree::getTotal(AABBNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c+=root->nt;
   return;
  }
  getTotal(root->left, c);
  getTotal(root->right, c);
 }
}

void AABBTree::getWorst(AABBNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c = max(c, root->nt);
   return;
  }
  getWorst(root->left, c);
  getWorst(root->right, c);
 }
}

