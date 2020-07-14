#ifndef __OBB_NODE_H__
#define __OBB_NODE_H__

#include "obb.h"
#include <stdio.h>

class OBBNode;
typedef OBBNode *pOBBNode;

class OBBNode
{
public:
 OBBNode *left;
 OBBNode *right;
 OBB BB;
 OBBNode();
 ~OBBNode();
 OBBNode(OFF *obj);
 OBBNode(OFF *obj, int n, int *tri);

 int size();

 int nt;
 int *t;

 float r, g, b;  // ojo: en la version definitiva no deberia existir!

 void write(FILE *f);
 bool read(FILE *f);
 bool readchild(FILE *f, pOBBNode &n);
 void render();
 void render(int l);
 void renderColors(OFF *obj);
 void assignColor();
 inline bool isLeaf() {return t!=NULL;}
 double GetVolumeValue();
};

#endif