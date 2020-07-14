#ifndef __OCT_NODE_H__
#define __OCT_NODE_H__

#include <stdio.h>

#include "defs.h"
#include "off.h"

class OctTreeNode;
typedef OctTreeNode *pOctTreeNode;

class OctTreeNode
{
public:
 OctTreeNode *child[8];
 OctTreeNode();
 ~OctTreeNode();
 OctTreeNode(OFF *obj);
 OctTreeNode(OFF *obj, int n, int *tri);
 OctTreeNode(f3 ori, f3 sizes);

 void init(OFF *obj);
 void init(OFF *obj, int n, int *tri);

 int nt;
 int *t;

 f3 ori;
 f3 sizes;

 int size();
 void write(FILE *f);
 bool read(FILE *f);

 inline bool isLeaf() {return t!=NULL;}
 bool anyVertexInside(OFF *obj, int t);
 bool vertexInside(f3 v);

 inline bool readchild(FILE *f, pOctTreeNode &n);

 void render();
};

#endif