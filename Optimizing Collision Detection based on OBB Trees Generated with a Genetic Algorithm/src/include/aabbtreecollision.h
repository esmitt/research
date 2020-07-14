#ifndef __AABB_TREE_COLLISION_H__
#define __AABB_TREE_COLLISION_H__

#include "collisiondetection.h"
#include "aabbtree.h"

class AABBTreeCollision:public CollisionDetection
{
 AABBTree *T1, *T2;
 OFF *off1, *off2;
 float *M1, *M2;  // matrices de transformacion para los dos objetos
public:
 bool Detect();
 AABBTreeCollision(AABBTree *T1, AABBTree *T2, OFF *off1, OFF *off2, float *M1, float *M2);
 ~AABBTreeCollision();
};

#endif