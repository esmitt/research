#include ".\Point3fv.h"

#include <math.h>
////////////////////////////////////////////////////////////
// CPoint3fv
////////////////////////////////////////////////////////////

/// \brief Constructor que asigna 0 a x,y,z.
CPoint3fv::CPoint3fv()
{
	m_point[POS_X] = 0.f;
	m_point[POS_Y] = 0.f;
	m_point[POS_Z] = 0.f;
}

/// \brief Constructor que asigna los parametros a x,y,z.
/// @param x valor a asignar a m_x
/// @param y valor a asignar a m_y
/// @param z valor a asignar a m_z
CPoint3fv::CPoint3fv(float x, float y, float z)
{
	m_point[POS_X] = x;
	m_point[POS_Y] = y;
	m_point[POS_Z] = z;
}

/// \brief Asigna los parametros a x,y,z.
/// @param x valor a asignar a m_x
/// @param y valor a asignar a m_y
/// @param z valor a asignar a m_z
void CPoint3fv::set(float x, float y, float z)
{
	m_point[POS_X] = x;
	m_point[POS_Y] = y;
	m_point[POS_Z] = z;
}

/// \brief Sobrecarga del operador *.
/// @param f valor flotante a multiplicar
/// @return un punto3f con el resultado de p3f*f
CPoint3fv CPoint3fv::operator * (float f)
{
	return CPoint3fv(m_point[POS_X]*f, m_point[POS_Y]*f, m_point[POS_Z]*f);
}

/// \brief Sobrecarga del operador *.
/// @param p3f punto a multiplicar
/// @return un punto3f con el resultado de this*p3f
CPoint3fv CPoint3fv::operator * (CPoint3fv& p3fv)
{
	return CPoint3fv(m_point[POS_X]*p3fv.getX(), m_point[POS_Y]*p3fv.getY(), m_point[POS_Z]*p3fv.getZ());
}

/// \brief Sobrecarga del operador +.
/// @param p3f_1 punto a multiplicar
/// @param p3f_2 punto a multiplicar
/// @return punto3f con el resultado de p3f_1*p3f_2
CPoint3fv operator + (CPoint3fv& p3fv_1, CPoint3fv& p3fv_2)
{
	return CPoint3fv(p3fv_1.getX() + p3fv_2.getX(),
					 p3fv_1.getY() + p3fv_2.getY(), 
					 p3fv_1.getZ() + p3fv_2.getZ());
}

/// \brief Sobrecarga del operador -.
/// @param p3f_1 punto como minuendo
/// @param p3f_2 punto como sustraendo
/// @return punto3f con el resultado de p3f_1-p3f_2
CPoint3fv operator - (CPoint3fv& p3fv_1, CPoint3fv& p3fv_2)
{
	return CPoint3fv(p3fv_1.getX() - p3fv_2.getX(), p3fv_1.getY() - p3fv_2.getY(), p3fv_1.getZ() - p3fv_2.getZ());
}

/// \brief Calculo de la distancia euclideana entre 2 puntos.
/// @param p3f_1 punto 1 del calculo
/// @param p3f_2 punto 2 del calculo
/// @return valor de la disancia entre p3f_1 y p3f_2
float distanceEuclidian(CPoint3fv& p3fv_1, CPoint3fv& p3fv_2)
{
	float x,y,z;
	x = (p3fv_1.getX() - p3fv_2.getX());
	x *= x;
	y = (p3fv_1.getY() - p3fv_2.getY());
	y *= y;
	z = (p3fv_1.getZ() - p3fv_2.getZ());
	z *= z;
	return ((float)sqrt(x + y + z));
}

void calculateNormal(CPoint3fv& normal, CPoint3fv& p3fv_1, CPoint3fv& p3fv_2, CPoint3fv& p3fv_3)
{
	//calculateNormal(normal, (p3fv_1 - p3fv_2), (p3fv_2 - p3fv_3));
	//calculateNormal(normal, (p3fv_2 - p3fv_1), (p3fv_3 - p3fv_1));
	calculateNormal(normal, (p3fv_2 - p3fv_1), (p3fv_3 - p3fv_1));
	normal.normalize();
}

void calculateNormal(Vector3fv& normal, Vector3fv& v1, Vector3fv& v2)
{
	//cross product
	normal.getX() = v1.getY() * v2.getZ() - v1.getZ() * v2.getY();
	normal.getY() = v1.getZ() * v2.getX() - v1.getX() * v2.getZ();
	normal.getZ() = v1.getX() * v2.getY() - v1.getY() * v2.getX();
}

CPoint3fv CPoint3fv::operator = (CPoint3fv& p3fv)
{
	m_point[POS_X] = p3fv.getX();
	m_point[POS_Y] = p3fv.getY();
	m_point[POS_Z] = p3fv.getZ();
	return *this;
}

CPoint3fv CPoint3fv::operator += (CPoint3fv& p3fv)
{
	m_point[POS_X] += p3fv.getX();
	m_point[POS_Y] += p3fv.getY();
	m_point[POS_Z] += p3fv.getZ();
	return *this;
}

CPoint3fv CPoint3fv::operator /= (float f)
{
	m_point[POS_X] /= (f == 0.f)?1.f:f;
	m_point[POS_Y] /= (f == 0.f)?1.f:f;
	m_point[POS_Z] /= (f == 0.f)?1.f:f;
	return *this;
}

CPoint3fv CPoint3fv::operator *= (float f)
{
	m_point[POS_X] *= f;
	m_point[POS_Y] *= f;
	m_point[POS_Z] *= f;
	return *this;
}

double dotProduct(CPoint3fv& p3fv_1, CPoint3fv& p3fv_2)
{
	return  p3fv_1.getX() * p3fv_2.getX() + 
			p3fv_1.getY() * p3fv_2.getY() + 
			p3fv_1.getZ() * p3fv_2.getZ();
}

void crossProduct(CPoint3fv& result, CPoint3fv& p3fv_1, CPoint3fv& p3fv_2)
{
	result.getX() = p3fv_1.getY() * p3fv_2.getZ() - p3fv_1.getZ() * p3fv_2.getY();
	result.getY() = p3fv_1.getZ() * p3fv_2.getX() - p3fv_1.getX() * p3fv_2.getZ();
	result.getZ() = p3fv_1.getX() * p3fv_2.getY() - p3fv_1.getY() * p3fv_2.getX();
}

///
/// Normaliza el vector
///
void CPoint3fv::normalize()
{
	double norma = norm();
	if(norma != 0.0f)
	{
		m_point[POS_X] /= (float)norma;
		m_point[POS_Y] /= (float)norma;
		m_point[POS_Z] /= (float)norma;
	}
	else
	{
		m_point[POS_X] = 0.f;
		m_point[POS_Y] = 0.f;
		m_point[POS_Z] = 0.f;
	}
}

double CPoint3fv::norm()
{
	return sqrt(getX()*getX() + getY()*getY() + getZ()*getZ());
}
