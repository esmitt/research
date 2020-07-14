#include "obb_struct.h"

/*
void OBB::draw()
{
	glLineWidth(3.f);
	CPoint3fv p1,p2,p3,p4,p5,p6;
	CPoint3fv r1, r2, r3, r4;
	Vector3fv v1, v2, v3;
	Vector3fv bb1, bb2, bb3, bb4, bb5, bb6, bb7, bb8;
	
	p1 = m_center + m_orientationX * m_minimumXt;
	p2 = m_center + m_orientationY * m_minimumYt;
	p3 = m_center + m_orientationZ * m_minimumZt;
	
	p4 = m_center + m_orientationX * m_maximumXt;
	p5 = m_center + m_orientationY * m_maximumYt;
	p6 = m_center + m_orientationZ * m_maximumZt;

	v1 = p1 - m_center;
	v2 = p5 - m_center;
	v3 = p6 - m_center;
	r1 = v1 + v2 + v3;
	
	v1 = p1 - m_center;
	v2 = p2 - m_center;
	v3 = p6 - m_center;
	r2 = v1 + v2 + v3;

	glBegin(GL_LINES);
		glVertex3fv(r1.getArray());
		glVertex3fv(r2.getArray());
	glEnd();
	///////////////////
	bb1 = r1;
	bb2 = r2;
	///////////////////

	v1 = p4 - m_center;
	v2 = p2 - m_center;
	v3 = p6 - m_center;
	r1 = v1 + v2 + v3;
	
	v1 = p4 - m_center;
	v2 = p5 - m_center;
	v3 = p6 - m_center;
	r2 = v1 + v2 + v3;
	
	glBegin(GL_LINES);
		glVertex3fv(r1.getArray());
		glVertex3fv(r2.getArray());
	glEnd();
	///////////////////
	bb3 = r1;
	bb4 = r2;
	///////////////////
	
	glBegin(GL_LINES);
		glVertex3fv(bb1.getArray());
		glVertex3fv(bb4.getArray());
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(bb2.getArray());
		glVertex3fv(bb3.getArray());
	glEnd();
	
	///////////////// PARTE DE ATRAS
	v1 = p1 - m_center;
	v2 = p5 - m_center;
	v3 = p3 - m_center;
	r1 = v1 + v2 + v3;
	
	v1 = p1 - m_center;
	v2 = p2 - m_center;
	v3 = p3 - m_center;
	r2 = v1 + v2 + v3;

	glBegin(GL_LINES);
		glVertex3fv(r1.getArray());
		glVertex3fv(r2.getArray());
	glEnd();
	///////////////////
	bb5 = r1;
	bb6 = r2;
	///////////////////

	v1 = p4 - m_center;
	v2 = p2 - m_center;
	v3 = p3 - m_center;
	r1 = v1 + v2 + v3;
	
	v1 = p4 - m_center;
	v2 = p5 - m_center;
	v3 = p3 - m_center;
	r2 = v1 + v2 + v3;
	
	glBegin(GL_LINES);
		glVertex3fv(r1.getArray());
		glVertex3fv(r2.getArray());
	glEnd();
	///////////////////
	bb7 = r1;
	bb8 = r2;
	///////////////////
	glBegin(GL_LINES);
		glVertex3fv(bb5.getArray());
		glVertex3fv(bb8.getArray());
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(bb6.getArray());
		glVertex3fv(bb7.getArray());
	glEnd();

	glBegin(GL_LINES);
		glVertex3fv(bb1.getArray());
		glVertex3fv(bb5.getArray());
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(bb4.getArray());
		glVertex3fv(bb8.getArray());
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(bb2.getArray());
		glVertex3fv(bb6.getArray());
	glEnd();
	glBegin(GL_LINES);
		glVertex3fv(bb3.getArray());
		glVertex3fv(bb7.getArray());
	glEnd();
	
	
	glColor3f(0,1,0);
	glPointSize(4.f);
	glBegin(GL_POINTS);
		glVertex3fv(p1.getArray());
		glVertex3fv(p2.getArray());
		glVertex3fv(p3.getArray());
		glVertex3fv(p4.getArray());
		glVertex3fv(p5.getArray());
		glVertex3fv(p6.getArray());
	glEnd();
	
}
*/
/*
double dotProduct(CPoint3fv& p3fv_1, float p3fv_2[3])
{
	return  p3fv_1.getX() * p3fv_2[0] + 
			p3fv_1.getY() * p3fv_2[1] + 
			p3fv_1.getZ() * p3fv_2[2];
}
*/
double dotProduct(CPoint3fv& p3fv_1, float x, float y, float z)
{
	return  p3fv_1.getX() * x + 
			p3fv_1.getY() * y + 
			p3fv_1.getZ() * z;
}

float OBB_struct::getProjection(f3 point, int axis)
{
	if(axis == AXIS_X)
		return dotProduct(m_orientationX, point[0], point[1], point[2]);
	if(axis == AXIS_Y)
		return dotProduct(m_orientationY, point[0], point[1], point[2]);
	if(axis == AXIS_Z)
		return dotProduct(m_orientationZ, point[0], point[1], point[2]);
	return 0;
}

float OBB_struct::getVolume()
{
	return (m_deltaX * m_deltaY * m_deltaZ);
}