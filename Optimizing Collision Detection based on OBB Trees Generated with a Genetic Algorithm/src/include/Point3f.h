#pragma once

class CPoint3f
{
public:
	float m_x;						// Posición en x
	float m_y;						// Posición en y
	float m_z;						// Posición en z
	
	CPoint3f();
	CPoint3f(float x, float y, float z);
	
	void set(float x, float y, float z);
	CPoint3f operator * (float f);
	CPoint3f operator * (const CPoint3f& p3f);

	friend CPoint3f operator + (const CPoint3f& p3f_1, const CPoint3f& p3f_2);
	friend CPoint3f operator - (const CPoint3f& p3f_1, const CPoint3f& p3f_2);
	
	friend float distanceEuclidia(const CPoint3f& p3f_1, const CPoint3f& p3f_2);
	friend void calculateNormal(CPoint3f& normal, const CPoint3f& p3f_1, const CPoint3f& p3f_2, const CPoint3f& p3f_3);
	friend void calculateNormal(CPoint3f& normal, const CPoint3f& v1, const CPoint3f& v2);

	CPoint3f operator - (){return CPoint3f(-m_x, -m_y, -m_z);}
	CPoint3f operator = (const CPoint3f& p3f);
	CPoint3f operator += (const CPoint3f& p3f);
	CPoint3f operator /= (float f);
	CPoint3f operator *= (float f);

	friend double dotProduct(const CPoint3f& p3f_1, const CPoint3f& p3f_2);
	friend void crossProduct(CPoint3f& result, const CPoint3f& p3f_1, const CPoint3f& p3f_2);
	double norm();
};
typedef CPoint3f Vector3f;		// Vector de 3 flotantes
