#ifndef POINT3FV
#define POINT3FV

#define POS_X 0
#define POS_Y 1
#define POS_Z 2

//Clase que maneja un punto 3D como un arreglo de flotantes
class CPoint3fv
{
public:
	float m_point[3];
	
	CPoint3fv();
	CPoint3fv(float x, float y, float z);
	
	//metodos de acceso
	inline float* getArray(){return m_point;}
	inline float& getX(){return m_point[POS_X];}
	inline float& getY(){return m_point[POS_Y];}
	inline float& getZ(){return m_point[POS_Z];}

	void set(float x, float y, float z);
	CPoint3fv operator * (float f);
	CPoint3fv operator * (CPoint3fv& p3fv);
	
	friend CPoint3fv operator + (CPoint3fv& p3fv_1, CPoint3fv& p3fv_2);	
	friend CPoint3fv operator - (CPoint3fv& p3fv_1, CPoint3fv& p3fv_2);
	
	friend float distanceEuclidian(CPoint3fv& p3fv_1, CPoint3fv& p3fv_2);
	friend void calculateNormal(CPoint3fv& normal, CPoint3fv& p3fv_1, CPoint3fv& p3fv_2, CPoint3fv& p3fv_3);
	friend void calculateNormal(CPoint3fv& normal, CPoint3fv& v1, CPoint3fv& v2);

	float& operator[] (int pos){return m_point[pos];}
	CPoint3fv operator - (){return CPoint3fv(-m_point[POS_X], -m_point[POS_Y], -m_point[POS_Z]);}
	CPoint3fv operator = (CPoint3fv& p3fv);
	CPoint3fv operator += (CPoint3fv& p3fv);
	CPoint3fv operator /= (float f);
	CPoint3fv operator *= (float f);

	friend double dotProduct(CPoint3fv& p3fv_1, float x, float y, float z);
	friend void crossProduct(CPoint3fv& result, CPoint3fv& p3fv_1, CPoint3fv& p3fv_2);

	void normalize();
	double norm();
};
typedef CPoint3fv Vector3fv;		// Vector de 3 flotantes

#endif