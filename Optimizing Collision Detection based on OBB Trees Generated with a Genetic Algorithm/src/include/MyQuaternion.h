#pragma once

#include "Point3fv.h"
#include <math.h>
///
/// Struct for Quaternion
///
struct MyQuaternion
{
	CPoint3fv m_vector;
	float m_angle;

	MyQuaternion(){}
	~MyQuaternion(){}
	
	void setQuaternion(float x, float y, float z, float w);
	CPoint3fv getPointRotated(CPoint3fv point);
	CPoint3fv getPointRotated(int orientation);
};