#pragma warning(disable:4786)

#include <map>

using namespace std;

#include "off.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "defs.h"
#include "defs.cpp"

OFF::OFF()
{
 face = NULL;
 normal = NULL;
 vertex = NULL;
 nf = nv = 0;
}

OFF::OFF(char *fname)
{
 face = NULL;
 normal = NULL;
 vertex = NULL;
 nf = nv = 0;
 load(fname);
}

OFF::~OFF()
{
 release();
}

unsigned int OFF::mem()
{
 return sizeof(f3 *)*3+sizeof(int)*2+sizeof(float)*6*nv+sizeof(int)*3*nf;
}

bool OFF::load(char *fname)
{
 FILE *f=fopen(fname,"rt");
 int i;
 if (!f) return false;
 char magic[80];
 fscanf(f,"%s", magic);
 if (strcmpi(magic,"OFF"))
 {
  fclose(f);
  return false;
 }
 fscanf(f,"%d %d %*d", &nv, &nf);
 face = new i3[nf];
 vertex = new f3[nv];
 normal = new f3[nv];

 //center[0] = center[1] = center[2] = 0.f;

 max[0] = max[1] = max[2] = -1E9f;
 min[0] = min[1] = min[2] = 1E9f;

 for(i=0; i<nv; i++)
 {
  normal[i][0] = normal[i][1] = normal[i][2] = 0.f;
  fscanf(f, "%f %f %f", vertex[i], vertex[i]+1, vertex[i]+2);
  //center[0] += vertex[i][0];
  //center[1] += vertex[i][1];
  //center[2] += vertex[i][2];
  if (max[0] < vertex[i][0]) max[0] = vertex[i][0];
  if (max[1] < vertex[i][1]) max[1] = vertex[i][1];
  if (max[2] < vertex[i][2]) max[2] = vertex[i][2];
  if (min[0] > vertex[i][0]) min[0] = vertex[i][0];
  if (min[1] > vertex[i][1]) min[1] = vertex[i][1];
  if (min[2] > vertex[i][2]) min[2] = vertex[i][2];
 }

 size[0] = max[0] - min[0];
 size[1] = max[1] - min[1];
 size[2] = max[2] - min[2];

 //center[0] /= (float)nv;
 //center[1] /= (float)nv;
 //center[2] /= (float)nv;

 center[0] = (max[0] + min[0]) / 2.f;
 center[1] = (max[1] + min[1]) / 2.f;
 center[2] = (max[2] + min[2]) / 2.f;

 dist2center = 0.f;
 for(i=0; i<nv; i++)
 {
  f3 v = {vertex[i][0] - center[0], vertex[i][1] - center[1], vertex[i][2] - center[2]};
  float d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  if (d > dist2center) dist2center = d;
 }

 for(i=0; i<nf; i++)
 {
  fscanf(f,"%*d %d %d %d", face[i], face[i]+1, face[i]+2);
  float *p1 = vertex[face[i][0]];
  float *p2 = vertex[face[i][1]];
  float *p3 = vertex[face[i][2]];
  f3 v1, v2, n;
  subf3(v1, p2, p1);
  subf3(v2, p3, p2);
  cross(n, v1, v2);
  addf3(normal[face[i][0]], normal[face[i][0]], n);
  addf3(normal[face[i][1]], normal[face[i][1]], n);
  addf3(normal[face[i][2]], normal[face[i][2]], n);
 }
 fclose(f);

 for(i=0; i<nv; i++)
  normalize(normal[i]);

 return true;
}

void OFF::release()
{
 if (face) delete [] face;
 if (normal) delete [] normal;
 if (vertex) delete [] vertex;
}

void OFF::renderWire(int i)
{
#ifndef _NOT_OPENGL_
 {
  glBegin(GL_LINE_LOOP);
   glVertex3fv(vertex[face[i][0]]);
   glVertex3fv(vertex[face[i][1]]);
   glVertex3fv(vertex[face[i][2]]);
  glEnd();
 }
#endif
}

void OFF::renderWire()
{
#ifndef _NOT_OPENGL_
 for(int i=0; i<nf; i++)
 {
  glBegin(GL_LINE_LOOP);
   glVertex3fv(vertex[face[i][0]]);
   glVertex3fv(vertex[face[i][1]]);
   glVertex3fv(vertex[face[i][2]]);
  glEnd();
 }
#endif
}

void OFF::render()
{
#ifndef _NOT_OPENGL_
 for(int i=0; i<nf; i++)
 {
  glBegin(GL_TRIANGLES);
  
   glNormal3fv(normal[face[i][0]]);
   glVertex3fv(vertex[face[i][0]]);
   glNormal3fv(normal[face[i][1]]);
   glVertex3fv(vertex[face[i][1]]);
   glNormal3fv(normal[face[i][2]]);
   glVertex3fv(vertex[face[i][2]]);
  glEnd();
 }
#endif
}

void OFF::renderColor(int *f, int nf, float r, float g, float b)
{
#ifndef _NOT_OPENGL_
 glColor3f(r, g, b);
 for(int i=0; i<nf; i++)
 {
//  glBegin(GL_LINE_LOOP);
  glBegin(GL_TRIANGLES);
   glNormal3fv(normal[face[f[i]][0]]);
   glVertex3fv(vertex[face[f[i]][0]]);
   glNormal3fv(normal[face[f[i]][1]]);
   glVertex3fv(vertex[face[f[i]][1]]);
   glNormal3fv(normal[face[f[i]][2]]);
   glVertex3fv(vertex[face[f[i]][2]]);
  glEnd();
 }
#endif
}

void OFF::render(int *f, int nf)
{
#ifndef _NOT_OPENGL_
 for(int i=0; i<nf; i++)
 {
  glBegin(GL_LINE_LOOP);
   //glNormal3fv(normal[face[f[i]][0]]]);
   //glNormal3fv(normal[face[f[i]][0]]]);
   glVertex3fv(vertex[face[f[i]][0]]);
   glVertex3fv(vertex[face[f[i]][1]]);
   glVertex3fv(vertex[face[f[i]][2]]);
  glEnd();
 }
#endif
}

void OFF::savePolys(char *fname, int *f, int nf)
{
 map <int,int> vert;
 map <int,int> vertinv;
 int nv=0, i;

 for(i=0; i<nf; i++)
 {
  for(int j=0; j<3; j++)
  {
   int v = face[f[i]][j];
   int vmap = vert[v];
   if (!vmap)
   {
// 7 3 8 10 11 12 29 27 30 31 32 33 34 35 36 37 38 39 40 41 42
    vert[v] = ++nv;
    vertinv[nv-1] = v;
   }
  }
 }

 nv = vert.size();

 FILE *F = fopen(fname, "wt");
 fprintf(F,"OFF\n%d %d 45\n", nv, nf);
 map <int,int>::iterator it = vertinv.begin();
 for(i=0; i<nv; i++)
 {
  pair<int,int> p = *it;
  int index = p.second; // first
  fprintf(F, "%f %f %f\n", vertex[index][0], vertex[index][1], vertex[index][2]);
  it++;
 }

 for(i=0; i<nf; i++)
 {
  int *v = face[f[i]];
  fprintf(F, "3 %d %d %d\n", vert[v[0]]-1, vert[v[1]]-1, vert[v[2]]-1);
 }

 fclose(F);
}

//mine
///////////////////
//REDUCIR TIME!!!
///////////////////
/*
OBB_struct& OFF::getOrientation()
{
	return m_boundingBox;
}

void OFF::setOrientation(CPoint3fv axisX, CPoint3fv axisY, CPoint3fv axisZ)
{
	m_boundingBox.m_orientationX = axisX;
	m_boundingBox.m_orientationY = axisY;
	m_boundingBox.m_orientationZ = axisZ;
}

void OFF::calculateVolume()
{
	float u1, u2, u3;
	float l1, l2, l3;
	float r1, r2, r3;
	u1 = u2 = u3 = FLT_MIN;
	l1 = l2 = l3 = FLT_MAX;

	//for(int i = 0;  i < m_nvertex; i++)
	for(int i = 0;  i < nv; i++)
	{
		r1 = m_boundingBox.getProjection(vertex[i], AXIS_X);
		if(r1 > u1)
			u1 = r1;
		if(r1 < l1)
			l1 = r1;

		r2 = m_boundingBox.getProjection(vertex[i],AXIS_Y);
		if(r2 > u2)
			u2 = r2;
		if(r2 < l2)
			l2 = r2;

		r3 = m_boundingBox.getProjection(vertex[i],AXIS_Z);
		if(r3 > u3)
			u3 = r3;
		if(r3 < l3)
			l3 = r3; 
	}
	
	m_boundingBox.m_minimumXt = l1;
	m_boundingBox.m_minimumYt = l2;
	m_boundingBox.m_minimumZt = l3;
	m_boundingBox.m_maximumXt = u1;
	m_boundingBox.m_maximumYt = u2;
	m_boundingBox.m_maximumZt = u3;

	m_boundingBox.m_deltaX = u1 - l1;
	m_boundingBox.m_deltaY = u2 - l2;
	m_boundingBox.m_deltaZ = u3 - l3;
	
	m_boundingBox.m_center = (m_boundingBox.m_orientationX*(l1 + u1)) + (m_boundingBox.m_orientationY*(l2 + u2)) + (m_boundingBox.m_orientationZ*(l3 + u3));
	m_boundingBox.m_center /= 2.f;
}
*/