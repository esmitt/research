#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "defs.h"

class Quaternion
{
 public:
	float x,y,z,w;
	Quaternion(){x=y=z=0.0f;w=1.0f;};
	//Quaternion(vector3 axis,float angle); axis-angle
	//Quaternion(float _x,float _y,float _z,float _w){x=_x;y=_y;z=_z;w=_w;};
};

Quaternion operator*(Quaternion a,Quaternion b);
Quaternion RotationArc(f3 v0,f3 v1);

#endif