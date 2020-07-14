#ifndef __OBB_TREE_H__
#define __OBB_TREE_H__

#include "obbnode.h"
//#include "off.h"

class OBBTree
{
 void buildTree(OFF *obj, OBBNode *root);
 int clasifyTriangle(OFF *obj, int t, OBB *BB);

public:
 OBBNode *root;
 OBBTree(OFF *obj);
 OBBTree();
 ~OBBTree();
 void release();
 int size();
 void write(FILE *f);
 void write(char *fname);
 bool read(FILE *f);
 bool read(char *fname);
 double GetVolumeValue();

 void render();
 void render(int l);
 void renderColors(OFF *obj);

 void assignColors();
 
 int getWorst();
 int getAverage();
 int getTotal();
 int countLeaves();
 int GetPathLenght();
 int DepthAverage();
 int DepthAverage(OBBNode *root, int level);
 double StandardDeviation(double average);
 double StandardDeviation(OBBNode *root, int level, double average);

 void countLeaves(OBBNode *root, int &c);
 void GetPathLenght(OBBNode *root, int &c, int level);
 void getTotal(OBBNode *root, int &c);
 void getWorst(OBBNode *root, int &c);
};

#endif