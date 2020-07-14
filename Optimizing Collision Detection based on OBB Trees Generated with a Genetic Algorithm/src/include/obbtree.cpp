#include <list>
#include <string.h>
#include "obbtree.h"

using namespace std;

OBBTree::OBBTree()
{
 root = NULL;
}
#include "tickscounter.h"
#include <iostream>
using namespace std;
OBBTree::OBBTree(OFF *obj)
{
 //CTicksCounter mtime;
 //mtime.Start();
 root = new OBBNode(obj);
 buildTree(obj, root);
 //cout << mtime.Stop() << endl;
}

OBBTree::~OBBTree()
{
 release();
}

void OBBTree::buildTree(OFF *obj, OBBNode *root)
{
	int axisNow;
	if (root)
 {
  int n = root->nt;
  int *t = root->t;
  f3 bbsize;//, bbcenter;
  root->BB.getSize(bbsize);
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
  OBB *leftBB=NULL, *rightBB=NULL;
  leftBB = new OBB();
  rightBB = new OBB();

  int c1, c2, c3;

  for(axisNow = 0; axisNow < 3; axisNow++)
  {
   c1 = c2 = c3 = 0;

   left.clear();
   right.clear();

   root->BB.split(axis[axisNow], leftBB, rightBB, 0.5f);
   for(int i=0; i<n; i++)
   {
    int cl = leftBB->countVertex(obj, obj->face[t[i]]);//(obj, t[i], &root->BB);
    
    switch(cl)
    {
    case 3:   // izq
    case 2:
     left.push_back(t[i]);c1++;
     break;
    case 0:    // der
    case 1:
     right.push_back(t[i]);c2++;
     break;
    default:    // compartido
     // no deberia entrar aqui!
   //  left.push_back(t[i]);
   //  right.push_back(t[i]);c3++;
     left.push_back(t[i]); c1++; // lo mandamos a juro al lado izq
     //right.push_back(t[i]);c3++;
     break;
    }
   }
   
   if (c1 && c2)  // la division por este eje esta bien!
    break;
  }
  if (axisNow==3)
  {
   // esta es una hoja!
   return;
  }

  int ln = left.size(), rn = right.size();
  OBBNode *leftChild = NULL, *rightChild = NULL;

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
   leftChild = new OBBNode(obj, ln, tl);
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
   rightChild = new OBBNode(obj, rn, tr);
  }

  root->left = leftChild;
  root->right = rightChild;
  
  delete [] root->t;
  root->t = NULL;
  root->nt = 0;
  
  if (root->left) buildTree(obj, leftChild);
  if (root->right) buildTree(obj, rightChild);
 }

 /*
  int n = root->nt;
  int *t = root->t;
  f3 bbsize, bbcenter;
  root->BB.getSize(bbsize);
  root->BB.getCenter(bbcenter);
  
  // buscar el eje mayor:
 // ejes ordenados en axis = axis!
  list <int> left, right;
  OBB *leftBB=NULL, *rightBB=NULL;
  leftBB = new OBB();
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
  */
}

int OBBTree::clasifyTriangle(OFF *obj, int t, OBB *BB)
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

int OBBTree::size()
{
 return sizeof(root) + (root?root->size():0);
}

void OBBTree::write(FILE *f)
{
 fwrite("OBBTREE", 1, 8, f);
 if (root) root->write(f);
}

void OBBTree::write(char *fname)
{
 FILE *f = fopen(fname, "wb");
 write(f);
 fclose(f);
}

bool OBBTree::read(FILE *f)
{
 char magic[9];
 magic[8] = '\0';
 int r = fread(magic, 1, 8, f);
 assertbytesread(r,8)
 if (strcmp(magic,"OBBTREE")) return false;
 
 release();

 root = new OBBNode();
 return root->read(f);
}

bool OBBTree::read(char *fname)
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

void OBBTree::release()
{
 if (root) delete root;
 root = NULL;
}

void OBBTree::render()
{
 #ifndef _NOT_OPENGL_
 if (root) root->render();
#endif
}

void OBBTree::render(int l)
{
 #ifndef _NOT_OPENGL_
 if (root) root->render(l);
#endif
}


void OBBTree::renderColors(OFF *obj)
{
 #ifndef _NOT_OPENGL_
 if (root) root->renderColors(obj);
#endif
}

void OBBTree::assignColors()
{
 if (root)
  root->assignColor();
}

int OBBTree::getWorst()
{
 int c = 0;
 getWorst(root, c);
 return c;
}

int OBBTree::getAverage()
{
 return getTotal() / countLeaves();
}

int OBBTree::getTotal()
{
 int c = 0;
 getTotal(root, c);
 return c;
}

int OBBTree::countLeaves()
{
 int c = 0;
 countLeaves(root, c);
 return c;
}

void OBBTree::countLeaves(OBBNode *root, int &c)
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

int OBBTree::GetPathLenght()
{
 int c = 0;
 GetPathLenght(root, c, 0);
 return c;
}

void OBBTree::GetPathLenght(OBBNode *root, int &c, int level)
{
	if (root)
	 {
	  if (root->isLeaf())
	  {
	   c += ((level*(level+1))>>1);
	   return;
	  }
	  GetPathLenght(root->left, c, level+1);
	  GetPathLenght(root->right, c, level+1);
	 }
}

int OBBTree::DepthAverage()
{
	return DepthAverage(root, 0);
}

int OBBTree::DepthAverage(OBBNode *root, int level)
{
	if(root->isLeaf())
	{
		return level;
	}
	else
		return ((root->left)?DepthAverage(root->left, level+1):0) + ((root->right)?DepthAverage(root->right, level+1):0);
}

double OBBTree::StandardDeviation(double average)
{
	return StandardDeviation(root, 0, average);
}

double OBBTree::StandardDeviation(OBBNode *root, int level, double average)
{
	if(root->isLeaf())
	{
		return (((double)level - average)*((double)level - average));
	}
	else
		return ((root->left)?StandardDeviation(root->left, level+1, average):0) + ((root->right)?StandardDeviation(root->right, level+1, average):0);
}

void OBBTree::getTotal(OBBNode *root, int &c)
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

void OBBTree::getWorst(OBBNode *root, int &c)
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

double OBBTree::GetVolumeValue()
{
	return root->GetVolumeValue();
}
