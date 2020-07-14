#pragma once

#include "Point3fv.h"
#include "utils.h"
#include "Defs.h"

struct OBB_struct
{
	CPoint3fv m_center;
	float m_deltaX;
	float m_deltaY;
	float m_deltaZ;
	float m_minimumXt;
	float m_minimumYt;
	float m_minimumZt;
	float m_maximumXt;
	float m_maximumYt;
	float m_maximumZt;
	Vector3fv m_orientationX;
	Vector3fv m_orientationY;
	Vector3fv m_orientationZ;
	
	OBB_struct()
	{
		m_orientationX.set(0, 1 ,0);
		m_orientationY.set(1, 0, 0);
		m_orientationZ.set(0, 0, 1);
	}
	~OBB_struct(){}
	
	//float getProjection(float point[3] , int axis);
	float getProjection(f3 point, int axis);

	float getVolume();
	//void draw();
};