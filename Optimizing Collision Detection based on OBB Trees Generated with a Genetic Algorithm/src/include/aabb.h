#ifndef __AABB_H__
#define __AABB_H__

#include <stdio.h>
#include "defs.h"
#include "off.h"

class AABB
{
public:
 f3 min;
 f3 max;

 AABB(f3 min, f3 max);
 AABB();
 AABB(OFF *obj);
 AABB(OFF *obj, int n, int *tri);

 void init(f3 min, f3 max);
 void init(OFF *obj);
 void init(OFF *obj, int n, int *tri);

 int operator = (AABB &otro);
 void getSize(f3 s);
 void getCenter(f3 s);
 bool inside(f3 v);
 bool inside(OFF *obj, i3 t);
 int countVertex(OFF *obj, i3 t);

 void write(FILE *f);
 bool read(FILE *f);
 int  size();

 void split(int axis, AABB *l, AABB *r, float t=0.5f);

 void render();
};

#endif __AABB_H__