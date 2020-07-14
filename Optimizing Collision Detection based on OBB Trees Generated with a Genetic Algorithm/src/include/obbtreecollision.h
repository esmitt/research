#ifndef __OBB_TREE_COLLISION_H__
#define __OBB_TREE_COLLISION_H__

#include "collisiondetection.h"
#include "obbtree.h"

class OBBTreeCollision:public CollisionDetection
{
 OBBTree *T1, *T2;
 OFF *off1, *off2;
 float *M1, *M2;  // matrices de transformacion para los dos objetos
public:
 bool Detect();
 OBBTreeCollision(OBBTree *T1, OBBTree *T2, OFF *off1, OFF *off2, float *M1, float *M2);
 ~OBBTreeCollision();
};

#endif