#ifndef __AABB_TREE_H__
#define __AABB_TREE_H__

#include "aabbnode.h"


class AABBTree
{
 void buildTree(OFF *obj, AABBNode *root);
 int clasifyTriangle(OFF *obj, int t, AABB *BB);

public:
 AABBNode *root;
 AABBTree(OFF *obj);
 AABBTree();
 ~AABBTree();
 void release();
 int size();
 void write(FILE *f);
 void write(char *fname);
 bool read(FILE *f);
 bool read(char *fname);
 void render();
 int getWorst();
 int getAverage();
 int getTotal();
 int countLeaves();

 void countLeaves(AABBNode *root, int &c);
 void getTotal(AABBNode *root, int &c);
 void getWorst(AABBNode *root, int &c);
};

#endif