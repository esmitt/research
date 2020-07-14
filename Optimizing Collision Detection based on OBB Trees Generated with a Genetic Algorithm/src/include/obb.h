#ifndef __OBB_H__
#define __OBB_H__

#include <stdio.h>
#include "defs.h"
#include "off.h"
static const int USE_GENETIC = true;
static int global = 1;
class OBB
{
public:
 f3 ori;        // origen del OBB
 f3 sizes;      // tama#os de cada lado
 f3 axis[3];    // ejes del OBB

 //OBB(f3 min, f3 max);
 OBB();
 OBB(OFF *obj);
 OBB(OFF *obj, int n, int *tri);

 void init(f3 min, f3 max);
 void init(OFF *obj);
 void init(OFF *obj, int n, int *tri);

 int operator = (OBB &otro);
 void getSize(f3 s);
 void getCenter(f3 s);
 bool inside(f3 v);
 bool inside(OFF *obj, i3 t);
 int countVertex(OFF *obj, i3 t);

 void write(FILE *f);
 bool read(FILE *f);
 int  size();

 void split(int axis, OBB *l, OBB *r, float t=0.5f);
 inline void findVertex(f3 *v);

 void render();
 //mine
 double GetVolumeValue();
};

inline void OBB::findVertex(f3 *v)
{
 f3 d1, d2, d3;

 floatbyf3(d1, sizes[0], axis[0]); // buscar los 
 floatbyf3(d2, sizes[1], axis[1]); // ejes escalados
 floatbyf3(d3, sizes[2], axis[2]);

 copyf3(v[0], ori);
 addf3(v[1], ori, d1);
 addf3(v[2], v[1], d2);
 addf3(v[3], ori, d2);
 addf3(v[4], v[0], d3);
 addf3(v[5], v[4], d1);
 addf3(v[6], v[5], d2);
 addf3(v[7], v[4], d2);
}

#endif __AABB_H__