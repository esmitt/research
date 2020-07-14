#include "MyQuaternion.h"
#include "utils.h"

// http://skal.planet-d.net/demo/matrixfaq.htm
void MyQuaternion::setQuaternion(float x, float y, float z, float w)
{
	//float angle = (w/2.f)*0.017453292519943f;
	float angle = w*0.008726203218641f;
	m_vector.set(x,y,z);
	m_vector.normalize();
	float sin_a = sin(angle);
	float cos_a = cos(angle);
	m_vector.set(m_vector.getX() * sin_a, m_vector.getY() * sin_a, m_vector.getZ() * sin_a);
	m_angle = cos_a;
}


CPoint3fv MyQuaternion::getPointRotated(CPoint3fv point)
{
	CPoint3fv r;
	float matrix[3][3];

	//build a quaternion
	float xx, xy, xz, xw, yy, yz, yw, zz, zw;
	xx = m_vector.getX()*m_vector.getX();
	xy = m_vector.getX()*m_vector.getY();
	xz = m_vector.getX()*m_vector.getZ();
	xw = m_vector.getX()*m_angle;

	yy = m_vector.getY()*m_vector.getY();
	yz = m_vector.getY()*m_vector.getZ();
	yw = m_vector.getY()*m_angle;

	zz = m_vector.getZ()*m_vector.getZ();
	zw = m_vector.getZ()*m_angle;

	matrix[0][0] = 1 - 2*(yy + zz);
	matrix[0][1] =	  2*(xy - zw);
	matrix[0][2] =	  2*(xz + yw);

	matrix[1][0] =     2*(xy + zw);
	matrix[1][1] = 1 - 2*(xx + zz);
	matrix[1][2] =	  2*(yz - xw);

	matrix[2][0] =     2*(xz - yw);
	matrix[2][1] =     2*(yz + xw);
	matrix[2][2] = 1 - 2*(xx + yy);
	//matrix is the rotation matrix, now multipliy --> matrix x vector
	r.set(	matrix[0][0]*point.getX() + matrix[0][1]*point.getY() + matrix[0][2]*point.getZ(),
			matrix[1][0]*point.getX() + matrix[1][1]*point.getY() + matrix[1][2]*point.getZ(),
			matrix[2][0]*point.getX() + matrix[2][1]*point.getY() + matrix[2][2]*point.getZ());
	return r;
}

CPoint3fv MyQuaternion::getPointRotated(int orientation)
{
	CPoint3fv r;
	if (orientation == AXIS_X)
	{
		//position [0][0], [1][0] & [2][0]
		r.set(1.f - 2.f*((m_vector.getY()*m_vector.getY()) + (m_vector.getZ()*m_vector.getZ())),
		2.f*((m_vector.getX()*m_vector.getY()) + (m_vector.getZ()*m_angle)),
		2.f*((m_vector.getX()*m_vector.getZ()) - (m_vector.getY()*m_angle)));
	}
	else if (orientation == AXIS_Y)
	{
		//position [0][1], [1][1] & [2][1]
		r.set(2.f*((m_vector.getX()*m_vector.getY()) - (m_vector.getZ()*m_angle)),
			1.f - 2.f*((m_vector.getX()*m_vector.getX()) + (m_vector.getZ()*m_vector.getZ())),
			2.f*((m_vector.getY()*m_vector.getZ()) + (m_vector.getX()*m_angle)));
	}
	else if (orientation == AXIS_Z)
	{
		//position [0][2], [1][2] & [2][2]
		r.set(2.f*((m_vector.getX()*m_vector.getZ()) + (m_vector.getY()*m_angle)),
			2.f*((m_vector.getY()*m_vector.getZ()) - (m_vector.getX()*m_angle)),
			1.f - 2.f*((m_vector.getX()*m_vector.getX()) + (m_vector.getY()*m_vector.getY())));
	}
	/*
	float matrix[3][3];

	//build a quaternion
	float xx, xy, xz, xw, yy, yz, yw, zz, zw;
	xx = m_vector.getX()*m_vector.getX();
	xy = m_vector.getX()*m_vector.getY();
	xz = m_vector.getX()*m_vector.getZ();
	xw = m_vector.getX()*m_angle;

	yy = m_vector.getY()*m_vector.getY();
	yz = m_vector.getY()*m_vector.getZ();
	yw = m_vector.getY()*m_angle;

	zz = m_vector.getZ()*m_vector.getZ();
	zw = m_vector.getZ()*m_angle;

	matrix[0][0] = 1 - 2*(yy + zz);
	matrix[0][1] =	  2*(xy - zw);
	matrix[0][2] =	  2*(xz + yw);

	matrix[1][0] =     2*(xy + zw);
	matrix[1][1] = 1 - 2*(xx + zz);
	matrix[1][2] =	  2*(yz - xw);

	matrix[2][0] =     2*(xz - yw);
	matrix[2][1] =     2*(yz + xw);
	matrix[2][2] = 1 - 2*(xx + yy);
	//matrix is the rotation matrix, now multipliy --> matrix x vector
	r.set(	matrix[0][0]*point.getX() + matrix[0][1]*point.getY() + matrix[0][2]*point.getZ(),
			matrix[1][0]*point.getX() + matrix[1][1]*point.getY() + matrix[1][2]*point.getZ(),
			matrix[2][0]*point.getX() + matrix[2][1]*point.getY() + matrix[2][2]*point.getZ());
	*/
	return r;
}