#ifndef __SOFTROTATION_H__
#define __SOFTROTATION_H__

#include "Quaternion.h"

void planelineintersection(f3 n,float d,f3 p1,f3 p2, f3 r);
void MakeMatrix4x4(float *m,const f3 v,const Quaternion &q);
void MouseDir(int x,int y, f3 r);
Quaternion VirtualTrackBall(f3 cop,f3 cor,f3 dir1,f3 dir2);

#endif