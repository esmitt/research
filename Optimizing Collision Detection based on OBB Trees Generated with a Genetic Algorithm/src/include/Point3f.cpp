#include ".\Point3f.h"
#include <math.h>
////////////////////////////////////////////////////////////
// CPoint3f
////////////////////////////////////////////////////////////

/// \brief Constructor que asigna 0 a x,y,z.
CPoint3f::CPoint3f()
{
	m_x = 0.f;
	m_y = 0.f;
	m_z = 0.f;
}

/// \brief Constructor que asigna los parametros a x,y,z.
/// @param x valor a asignar a m_x
/// @param y valor a asignar a m_y
/// @param z valor a asignar a m_z
CPoint3f::CPoint3f(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/// \brief Asigna los parametros a x,y,z.
/// @param x valor a asignar a m_x
/// @param y valor a asignar a m_y
/// @param z valor a asignar a m_z
void CPoint3f::set(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/// \brief Sobrecarga del operador *.
/// @param f valor flotante a multiplicar
/// @return un punto3f con el resultado de p3f*f
CPoint3f CPoint3f::operator * (float f)
{
	return CPoint3f(m_x*f, m_y*f, m_z*f);
}

/// \brief Sobrecarga del operador *.
/// @param p3f punto a multiplicar
/// @return un punto3f con el resultado de this*p3f
CPoint3f CPoint3f::operator * (const CPoint3f& p3f)
{
	return CPoint3f(m_x*p3f.m_x, m_y*p3f.m_y, m_z*p3f.m_z);
}

/// \brief Sobrecarga del operador +.
/// @param p3f_1 punto a multiplicar
/// @param p3f_2 punto a multiplicar
/// @return punto3f con el resultado de p3f_1*p3f_2
CPoint3f operator + (const CPoint3f& p3f_1, const CPoint3f& p3f_2)
{
	return CPoint3f(p3f_1.m_x + p3f_2.m_x, p3f_1.m_y + p3f_2.m_y, p3f_1.m_z + p3f_2.m_z);
}

/// \brief Sobrecarga del operador -.
/// @param p3f_1 punto como minuendo
/// @param p3f_2 punto como sustraendo
/// @return punto3f con el resultado de p3f_1-p3f_2
CPoint3f operator - (const CPoint3f& p3f_1, const CPoint3f& p3f_2)
{
	return CPoint3f(p3f_1.m_x - p3f_2.m_x, p3f_1.m_y - p3f_2.m_y, p3f_1.m_z - p3f_2.m_z);
}

/// \brief Calculo de la distancia euclideana entre 2 puntos.
/// @param p3f_1 punto 1 del calculo
/// @param p3f_2 punto 2 del calculo
/// @return valor de la disancia entre p3f_1 y p3f_2
float distanceEuclidia(const CPoint3f& p3f_1, const CPoint3f& p3f_2)
{
	float x,y,z;
	x = (p3f_1.m_x - p3f_2.m_x);
	x *= x;
	y = (p3f_1.m_y - p3f_2.m_y);
	y *= y;
	z = (p3f_1.m_z - p3f_2.m_z);
	z *= z;
	return ((float)sqrt(x + y + z));
}

void calculateNormal(CPoint3f& normal, const CPoint3f& p3f_1, const CPoint3f& p3f_2, const CPoint3f& p3f_3)
{
	Vector3f v1, v2;
	v1 = (p3f_1 - p3f_2);
	v2 = (p3f_2 - p3f_3);
	normal.m_x = v1.m_y * v2.m_z - v1.m_z * v2.m_y;
	normal.m_y = v1.m_z * v2.m_x - v1.m_x * v2.m_z;
	normal.m_z = v1.m_x * v2.m_y - v1.m_y * v2.m_x;
	double norma = normal.norm();
	if(norma == 0.0) norma = 1.0;
	normal /= ((float)norma);
}

void calculateNormal(Vector3f& normal, const Vector3f& v1, const Vector3f& v2)
{
	normal.m_x = v1.m_y * v2.m_z - v1.m_z * v2.m_y;
	normal.m_y = v1.m_z * v2.m_x - v1.m_x * v2.m_z;
	normal.m_z = v1.m_x * v2.m_y - v1.m_y * v2.m_x;
	double norma = normal.norm();
	if(norma == 0.0) norma = 1.0;
	normal /= ((float)norma);
}


CPoint3f CPoint3f::operator = (const CPoint3f& p3f)
{
	m_x = p3f.m_x;
	m_y = p3f.m_y;
	m_z = p3f.m_z;
	return *this;
}

CPoint3f CPoint3f::operator += (const CPoint3f& p3f)
{
	m_x += p3f.m_x;
	m_y += p3f.m_y;
	m_z += p3f.m_z;
	return *this;
}

CPoint3f CPoint3f::operator /= (float f)
{
	m_x /= (f == 0.f)?1.f:f;
	m_y /= (f == 0.f)?1.f:f;
	m_z /= (f == 0.f)?1.f:f;
	return *this;
}

CPoint3f CPoint3f::operator *= (float f)
{
	m_x *= f;
	m_y *= f;
	m_z *= f;
	return *this;
}

double dotProduct(const CPoint3f& p3f_1, const CPoint3f& p3f_2)
{
	return p3f_1.m_x * p3f_2.m_x + 
				 p3f_1.m_y * p3f_2.m_y + 
				 p3f_1.m_z * p3f_2.m_z;
}

void crossProduct(CPoint3f& result, const CPoint3f& p3f_1, const CPoint3f& p3f_2)
{
	result.m_x = p3f_1.m_y * p3f_2.m_z - p3f_1.m_z * p3f_2.m_y;
	result.m_y = p3f_1.m_z * p3f_2.m_x - p3f_1.m_x * p3f_2.m_z;
	result.m_z = p3f_1.m_x * p3f_2.m_y - p3f_1.m_y * p3f_2.m_x;
}

double CPoint3f::norm()
{
	return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}
