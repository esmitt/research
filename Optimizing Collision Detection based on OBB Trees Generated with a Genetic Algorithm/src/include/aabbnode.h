#ifndef __AABB_NODE_H__
#define __AABB_NODE_H__

#include "aabb.h"
#include <stdio.h>

class AABBNode;
typedef AABBNode *pAABBNode;

class AABBNode
{
public:
 AABBNode *left;
 AABBNode *right;
 AABB BB;
 AABBNode();
 ~AABBNode();
 AABBNode(OFF *obj);
 AABBNode(OFF *obj, int n, int *tri);

 int size();

 int nt;
 int *t;

 void write(FILE *f);
 bool read(FILE *f);
 bool readchild(FILE *f, pAABBNode &n);
 void render();
 inline bool isLeaf() {return t!=NULL;}
};

#endif