#include <list>
#include <string.h>
#include "octree.h"

using namespace std;

OctTree::OctTree()
{
 root = NULL;
}

OctTree::OctTree(OFF *obj)
{
 root = new OctTreeNode(obj);
 root->nt = obj->nf;
 root->t = new int[root->nt];
 for(int i=0; i<root->nt; i++)
  root->t[i] = i;
 buildTree(obj, root);
}

OctTree::~OctTree()
{
 release();
}

void OctTree::buildTree(OFF *obj, OctTreeNode *root)
{
 if (root)
 {
  int n = root->nt;
  int *t = root->t;

  if (n==1) return;
  
  list <int> triangles[8];

  f3 bbsize;
  f3 ori;
  floatbyf3(bbsize, 0.5f, root->sizes);

  root->child[0] = new OctTreeNode(root->ori, bbsize);

  copyf3(ori, root->ori); ori[0]+=bbsize[0];
  root->child[1] = new OctTreeNode(ori, bbsize);

  ori[1]+=bbsize[1];
  root->child[2] = new OctTreeNode(ori, bbsize);

  ori[0]-=bbsize[0];
  root->child[3] = new OctTreeNode(ori, bbsize);

  copyf3(ori, root->ori); ori[2] += bbsize[2];
  root->child[4] = new OctTreeNode(ori, bbsize);

  ori[0]+=bbsize[0];
  root->child[5] = new OctTreeNode(ori, bbsize);

  ori[1]+=bbsize[1];
  root->child[6] = new OctTreeNode(ori, bbsize);

  ori[0]-=bbsize[0];
  root->child[7] = new OctTreeNode(ori, bbsize);

  int c[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for(int i=0; i<n; i++)
  {
   for(int j=0; j<8; j++)
   {
    if (root->child[j]->anyVertexInside(obj, t[i]))
    {
     triangles[j].push_back(t[i]);
     c[j]++;
    }
   }
  }

  bool hasChilds = false;

  for(i=0; i<8; i++)
  {
   if (c[i]==0 || c[i]==n)
   {
    delete root->child[i];
    root->child[i] = NULL;
   }
   else
   {
    hasChilds = true;
    int *T = new int[c[i]];
    int j=0;
    list <int>::iterator it = triangles[i].begin();
    while(it!=triangles[i].end())
    {
     T[j++]=*it++;
    }
    triangles[i].clear();
    root->child[i]->init(obj, c[i], T);
    buildTree(obj, root->child[i]);
    //delete [] T;
   }
  }

  if (hasChilds)
  {
   delete root->t;
   root->t = NULL;
   root->nt = 0;
  }
 }
}

/*
void OctTree::buildTree(OFF *obj, AABBNode *root)
{
 if (root)
 {
  f3 bbsize, bbcenter;
  root->BB.getSize(bbsize);
  root->BB.getCenter(bbcenter);
  
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
*/
int OctTree::size()
{
 return sizeof(root) + (root?root->size():0);
}

void OctTree::write(FILE *f)
{
 fwrite("OCTTREE", 1, 7, f);
 if (root) root->write(f);
}

void OctTree::write(char *fname)
{
 FILE *f = fopen(fname, "wb");
 write(f);
 fclose(f);
}

bool OctTree::read(FILE *f)
{
 char magic[8];
 magic[7] = '\0';
 int r = fread(magic, 1, 7, f);
 assertbytesread(r,7)
 if (strcmp(magic,"OCTTREE")) return false;
 
 release();

 root = new OctTreeNode();
 return root->read(f);
}

bool OctTree::read(char *fname)
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

void OctTree::release()
{
 if (root) delete root;
 root = NULL;
}

void OctTree::render()
{
 #ifndef _NOT_OPENGL_
 if (root) root->render();
#endif
}

int OctTree::getWorst()
{
 int c = 0;
 getWorst(root, c);
 return c;
}

int OctTree::getAverage()
{
 return getTotal() / countLeaves();
}

int OctTree::getTotal()
{
 int c = 0;
 getTotal(root, c);
 return c;
}

int OctTree::countLeaves()
{
 int c = 0;
 countLeaves(root, c);
 return c;
}

void OctTree::countLeaves(OctTreeNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c++;
   return;
  }
  for(int i=0; i<8; i++)
   countLeaves(root->child[i], c);
 }
}

void OctTree::getTotal(OctTreeNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c+=root->nt;
   return;
  }
  for(int i=0; i<8; i++)
   getTotal(root->child[i], c);
 }
}

void OctTree::getWorst(OctTreeNode *root, int &c)
{
 if (root)
 {
  if (root->isLeaf())
  {
   c = max(c, root->nt);
   return;
  }
  for(int i=0; i<8; i++)
   getWorst(root->child[i], c);
 }
}

