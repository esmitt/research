#include "quaternion.h"

Quaternion operator*(Quaternion a,Quaternion b) 
{
	Quaternion c;
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z; 
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y; 
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x; 
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w; 
	return c;
}

//
//        RotationArc()
//
//  This is the function from Games Programming Gems
//  that we are using in the VirtualTrackBall function
//
float viewangle;

Quaternion RotationArc(f3 v0,f3 v1)
{
  Quaternion q;
  // v0.normalize(); 
  // v1.normalize();  // If vector is already unit length then why do it again?
  f3 c;
  cross(c, v0, v1);
  //vector3 c = CrossProduct(v0,v1);
  float d = dot(v0, v1);
  //float   d = DotProduct(v0,v1);
  float   s = (float)sqrt((1+d)*2);
  q.x = c[0] / s;
  q.y = c[1] / s;
  q.z = c[2] / s;
  q.w = s /2.0f;
  return q;
}
