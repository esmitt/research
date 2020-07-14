#ifndef __OCT_TREE_H__
#define __OCT_TREE_H__

#include "octreenode.h"


class OctTree
{
 void buildTree(OFF *obj, OctTreeNode *root);

public:
 OctTreeNode *root;
 OctTree();
 OctTree(OFF *obj);
 ~OctTree();
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

 void countLeaves(OctTreeNode *root, int &c);
 void getTotal(OctTreeNode *root, int &c);
 void getWorst(OctTreeNode *root, int &c);

 /*
 int clasifyTriangle(OFF *obj, int t, AABB *BB);
 */
};

#endif