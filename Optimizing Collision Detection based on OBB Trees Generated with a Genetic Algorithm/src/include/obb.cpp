#include "OBB.h"
#include "mathutil.h"
#include "autovector.h"
#include "tickscounter.h"

#pragma warning(disable:4786)
#include <map>



using namespace std;

/*
OBB::OBB(f3 min, f3 max)
{
 copyf3(this->max, max);
 copyf3(this->min, min);
}
*/

OBB::OBB()
{
 ori[0] = ori[1] = ori[2] = 
 sizes[0] = sizes[1] = sizes[2] = 0.f;
}

OBB::OBB(OFF *obj)
{
 init(obj);
 /*
 max[0] = max[1] = max[2] = -1E9;
 min[0] = min[1] = min[2] =  1E9;
 for(int i=0; i<obj->nv; i++)
 {
  if (max[0] < obj->vertex[i][0]) max[0] = obj->vertex[i][0];
  if (max[1] < obj->vertex[i][1]) max[1] = obj->vertex[i][1];
  if (max[2] < obj->vertex[i][2]) max[2] = obj->vertex[i][2];
  if (min[0] > obj->vertex[i][0]) min[0] = obj->vertex[i][0];
  if (min[1] > obj->vertex[i][1]) min[1] = obj->vertex[i][1];
  if (min[2] > obj->vertex[i][2]) min[2] = obj->vertex[i][2];
 }
 */
}

OBB::OBB(OFF *obj, int n, int *tri)
{
 init(obj, n, tri);
}

void OBB::init(f3 min, f3 max)
{
// copyf3(this->max, max);
// copyf3(this->min, min);
}

void OBB::init(OFF *obj)
{
 int *t = new int[obj->nf];
 for(int i=0; i<obj->nf; i++)
  t[i] = i;
 init(obj, obj->nf, t);
 delete [] t;

}

inline float project(f3 v, f3 D)
{
 // proyecta el punto v sobre la recta D
 return dot(v, D)/dot(D, D);
}

inline float project(f3 v, f3 D, f3 c)
{
 // proyecta el punto v sobre la recta con vd D que pasa por el pto c
 return (dot(v, D)-dot(c,D))/dot(D, D);
}


inline void project(f3 *vl, f3 D, float &m, float &M)
{
 // proyecta los 8 vertices de vl sobre la recta D y deja los bordes en m y M
 m = 1E9f; M = -1E9f;
 for(int i=0; i<8; i++)
 {
  float t = project(vl[i], D);
  if (t > M) M = t;
  if (t < m) m = t;
 }
} 


#include <set>
#include <iostream>
#include "Genetic.h"
#define V1 0
#define V2 1
#define V3 2
//HERE
//HERE
//HERE
//HERE
void OBB::init(OFF *obj, int n, int *tri)
{
	//CTicksCounter timem;
	//timem.Start();
	f3 *c=new f3[n];            // c: arreglo de centroides de los triang.
 float *A=new float[n];      // A: area de cada triangulo
 float AH=0.f;               // AH: area total
 int i;
 f3 CH={0.f,0.f,0.f};        // CH: centroide ponderado
 f3 *puntos = obj->vertex;
 i3 *T = obj->face;
 set<f3*> setVertex;		//conjunto de vertices
 for(i=0; i<n; i++)
 {
  c[i][0]=(obj->vertex[obj->face[tri[i]][0]][0]+obj->vertex[obj->face[tri[i]][1]][0]+obj->vertex[obj->face[tri[i]][2]][0])/3.f;
  c[i][1]=(obj->vertex[obj->face[tri[i]][0]][1]+obj->vertex[obj->face[tri[i]][1]][1]+obj->vertex[obj->face[tri[i]][2]][1])/3.f;
  c[i][2]=(obj->vertex[obj->face[tri[i]][0]][2]+obj->vertex[obj->face[tri[i]][1]][2]+obj->vertex[obj->face[tri[i]][2]][2])/3.f;
  setVertex.insert(&obj->vertex[obj->face[tri[i]][V1]]);
  setVertex.insert(&obj->vertex[obj->face[tri[i]][V2]]);
  setVertex.insert(&obj->vertex[obj->face[tri[i]][V3]]);
  A[i] = area(obj->vertex[obj->face[tri[i]][0]], obj->vertex[obj->face[tri[i]][1]], obj->vertex[obj->face[tri[i]][2]]);
  AH+=A[i];
  CH[0]+=A[i]*c[i][0];
  CH[1]+=A[i]*c[i][1];
  CH[2]+=A[i]*c[i][2];
 }
 CH[0]/=AH;
 CH[1]/=AH;
 CH[2]/=AH;
 
 //el conjunto de triangulos
 set<f3*>::iterator itVertex = setVertex.begin();
 /*
 cout << setVertex.size() <<std::endl;
 while(itVertex != setVertex.end())
 {
	 std::cout << (*itVertex)[0][0] << " " << (*itVertex)[0][1] << " " << (*itVertex)[0][2] << std::endl;
	itVertex++;
 }
 */
  //lamar al genetico


 f33 C={{0.f,0.f,0.f},{0.f,0.f,0.f},{0.f,0.f,0.f}};  // C: matriz de covarianza

 if(USE_GENETIC)
 {
	CGenetic genetic;
	//genetic.createPopulation();
	
	// take a time
	//CTicksCounter timeg;
	//timeg.Start();
	genetic.runAlgorithm(setVertex);
	//cout << timeg.Stop() << endl;
	axis[0][0]=genetic.off.getOrientation().m_orientationX.getX();
	axis[0][1]=genetic.off.getOrientation().m_orientationX.getY();
	axis[0][2]=genetic.off.getOrientation().m_orientationX.getZ();
	axis[1][0]=genetic.off.getOrientation().m_orientationY.getX();
	axis[1][1]=genetic.off.getOrientation().m_orientationY.getY();
	axis[1][2]=genetic.off.getOrientation().m_orientationY.getZ();
	axis[2][0]=genetic.off.getOrientation().m_orientationZ.getX();
	axis[2][1]=genetic.off.getOrientation().m_orientationZ.getY();
	axis[2][2]=genetic.off.getOrientation().m_orientationZ.getZ();
	setVertex.clear();
 }
 else
 {
	 for(int j=0; j<3; j++)
	 {
	  for(int k=0; k<3; k++)
	  {
	   for (i=0; i<n; i++)
	   {
		float *p=puntos[T[tri[i]][0]];
		float *q=puntos[T[tri[i]][1]];
		float *r=puntos[T[tri[i]][2]];
		C[j][k]+=(9.f*c[i][j]*c[i][k]+p[j]*p[k]+q[j]*q[k]+r[j]*r[k])*A[i]/(12.f*AH)-CH[j]*CH[k];
		if (fabs(C[j][k])>1000.f)
		 j=j;
	   }
	  }
	 }

	 delete [] c;
	 delete [] A;

	 float d[4];
	 float * CE[4];
	 CE[0]=new float[4];
	 CE[1]=new float[4];
	 CE[2]=new float[4];
	 CE[3]=new float[4];
	 
	 memcpy(&CE[1][1],C[0],sizeof(float)*3);
	 memcpy(&CE[2][1],C[1],sizeof(float)*3);
	 memcpy(&CE[3][1],C[2],sizeof(float)*3);

	 float *AA[4];
	 AA[0]=new float[4];
	 AA[1]=new float[4];
	 AA[2]=new float[4];
	 AA[3]=new float[4];
	 int rot;
	 jacobi(CE, 3, d, AA, &rot);       // calcula los autovectores y los almacena en A

	 axis[0][0]=AA[1][1];
	 axis[0][1]=AA[2][1];
	 axis[0][2]=AA[3][1];
	 axis[1][0]=AA[1][2];
	 axis[1][1]=AA[2][2];
	 axis[1][2]=AA[3][2];
	 axis[2][0]=AA[1][3];
	 axis[2][1]=AA[2][3];
	 axis[2][2]=AA[3][3];

	  // los ejes estan en gerg!

	 // Ojo: falta destruir CE y AA
	 for (i=0; i<4; i++) 
	 {
	  delete [] AA[i];
	  delete [] CE[i];
	 }

 }

 f3 center = {0.f, 0.f, 0.f};

 map <int,int> puntos_sin_repetir;

 for(i=0; i<n; i++)
 {
  for(int k=0; k<3; k++)
  {
   puntos_sin_repetir[T[tri[i]][k]] = 1;
  }
 }

 map <int, int>::iterator it = puntos_sin_repetir.begin();
 int nv = 0;
 while(it!=puntos_sin_repetir.end())
 {  
  pair <int, int> p = *it;
  for(int j=0; j<3; j++)
   center[j] += puntos[p.first][j];
  it++;
  nv++;
 }

 /*
 for(i=0; i<n; i++)
 {
  for(int j=0; j<3; j++)      // coord
   for(int k=0; k<3; k++)     // vertice
   {
    center[j] += puntos[T[tri[i]][k]][j];
   }
 }
 */

 center[0]/=(float)nv;
 center[1]/=(float)nv;
 center[2]/=(float)nv;

 float tMin[3] = {1E6,1E6,1E6}, tMax[3] = {-1E6,-1E6,-1E6};

 it = puntos_sin_repetir.begin();
 f3 zero = {0.f, 0.f, 0.f};
 while(it!=puntos_sin_repetir.end())
 {  
  float t;
  pair <int, int> p = *it;
  for(int j=0; j<3; j++)
  {
   t = project(puntos[p.first], axis[j], zero);

   // DEbUG
   
   f3 punto_en_eje;
   floatbyf3(punto_en_eje, t, axis[j]);
   f3 v2;
   subf3(v2, punto_en_eje, puntos[p.first]);
   normalize(v2);
   normalize(punto_en_eje);
   float res = dot(v2, punto_en_eje);
   
   // eNDDEbug

   if (t > tMax[j]) tMax[j] = t;
   if (t < tMin[j]) tMin[j] = t;
  }
   //center[j] += puntos[p.first][j];
  it++;
 }

 //f3 axis_dist[3]; // los ejes escalados a la mitad!

 //f3 cent;
// cent

 ori[0] = ori[1] = ori[2] = 0.f;

 for(i=0; i<3; i++)
 {
  f3 Mx, Mn;
  floatbyf3(Mx, tMax[i], axis[i]);
  //addf3(Mx, Mx, center);

  floatbyf3(Mn, tMin[i], axis[i]);
  //addf3(Mn, Mn, center);


  sizes[i] = distf3(Mx, Mn);
  
  addf3(ori, ori, Mn);

  //floatbyf3(axis_dist[i], sizes[i]/2.f, axis[i]);

  // NOTA: EL CeNTRO ESTA MaL CaLCuLADO. CaLCULAR DE OTRA FORMa!
  //cent[i] = 
 }

 f3 cent;
 f3 ax1,ax2,ax3;
 floatbyf3(ax1, sizes[0], axis[0]);
 floatbyf3(ax2, sizes[1], axis[1]);
 floatbyf3(ax3, sizes[2], axis[2]);
 addf3(cent, ori, ax1);
 addf3(cent, cent, ax2);
 addf3(cent, cent, ax3);

// subf3(ori, center, axis_dist[0]); 
// subf3(ori, ori, axis_dist[1]); 
// subf3(ori, ori, axis_dist[2]); 

 // DEBUG:
 int yes=0, no=0;
 it = puntos_sin_repetir.begin();
 while(it!=puntos_sin_repetir.end())
 {
  pair <int, int> p = *it;
  if (inside(puntos[p.first]))
   yes++; 
  else 
   no++;
  it++;
 }
/*
 for(i=0; i<n; i++)
 {
   for(int k=0; k<3; k++)     // vertice
   {
    if (inside(puntos[T[tri[i]][k]]))
     yes++; else no++;
   }
 }
 */
 // END DEBUG
 //cout << timem.Stop() << endl;
}

int OBB::operator = (OBB &otro)
{
 copyf3(sizes, otro.sizes);
 copyf3(ori, otro.ori);
 for(int i=0; i<3; i++)
  copyf3(axis[i], otro.axis[i]);
 return 1;
}

void OBB::getSize(f3 s)
{
 copyf3(s, sizes);
}

void OBB::getCenter(f3 s)
{
 f3 a,b,c;
 floatbyf3(a, sizes[0], axis[0]);
 floatbyf3(b, sizes[1], axis[1]);
 floatbyf3(c, sizes[2], axis[2]);
 addf3(s, ori, a);
 addf3(s, s, b);
 addf3(s, s, c);
}

bool OBB::inside(OFF *obj, i3 t)
{
 return inside(obj->vertex[t[0]]) && inside(obj->vertex[t[1]]) && inside(obj->vertex[t[2]]);
}

int OBB::countVertex(OFF *obj, i3 t)
{
 int r = 0;
 for(int i=0; i<3; i++)
  if (inside(obj->vertex[t[i]])) r++;
 return r;
}

inline void pointInLine(f3 r, f3 c, f3 d, float t)
// retorna en r evaluar la recta p = c + d*t. Se asume d normalizado
{
 floatbyf3(r, t, d);
 addf3(r, r, c);
}

bool OBB::inside(f3 v)
{
 float t;

 for(int i=0; i<3; i++)
 {
  f3 aux,aux2;
  pointInLine(aux, ori, axis[i], sizes[i]);

  float t2 = project(aux, axis[i], ori);
  float d = distf3(aux, ori);

  t = project(v, axis[i], ori);

  pointInLine(aux2, ori, axis[i], t);

  if (t<0.f || t>sizes[i]) return false;
 }

 return true;
 /*
 return v[0] <= max[0] && v[0] >= min[0] &&
        v[1] <= max[1] && v[1] >= min[1] &&
        v[2] <= max[2] && v[2] >= min[2];*/
}

void OBB::write(FILE *f)
{
 fwrite(ori, sizeof(float), 3, f);
 fwrite(sizes, sizeof(float), 3, f);
 fwrite(axis[0], sizeof(float), 3, f);
 fwrite(axis[1], sizeof(float), 3, f);
 fwrite(axis[2], sizeof(float), 3, f);
}

bool OBB::read(FILE *f)
{
 int r;
 r=fread(ori, sizeof(float), 3, f);
 assertbytesread(r,3)
 r=fread(sizes, sizeof(float), 3, f);
 assertbytesread(r,3)
 r=fread(axis[0], sizeof(float), 3, f);
 assertbytesread(r,3)
 r=fread(axis[1], sizeof(float), 3, f);
 assertbytesread(r,3)
 r=fread(axis[2], sizeof(float), 3, f);
 assertbytesread(r,3)
 return true;
}

void OBB::split(int axis, OBB *l, OBB *r, float t)
{
 f3 neworig;
 floatbyf3(neworig, t*sizes[axis], this->axis[axis]);
 addf3(neworig, ori, neworig);
 *l = *this;
 *r = *this;

 l->sizes[axis] *= t;
 r->sizes[axis] *= 1.f - t;
 f3 desp;
 floatbyf3(desp, l->sizes[axis], this->axis[axis]);
 addf3(r->ori, ori, desp);
 /*
 float c = min[axis] + (max[axis]-min[axis])*t;
 *l = *this;
 *r = *this;

 l->max[axis] = c;
 r->min[axis] = c;
 */
}

int OBB::size()
{
 return sizeof(*this);
}

void OBB::render()
{
#ifndef _NOT_OPENGL_

 f3 v[8];
 findVertex(v);

 glBegin(GL_LINE_LOOP);
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);
  glVertex3fv(v[3]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3fv(v[4]);
  glVertex3fv(v[5]);
  glVertex3fv(v[6]);
  glVertex3fv(v[7]);
 glEnd();

 glBegin(GL_LINES);
  glVertex3fv(v[0]);
  glVertex3fv(v[4]);
  glVertex3fv(v[1]);
  glVertex3fv(v[5]);
  glVertex3fv(v[2]);
  glVertex3fv(v[6]);
  glVertex3fv(v[3]);
  glVertex3fv(v[7]);
  // OJO: FALTA!
 glEnd();
 /*
for(int k = 0;k < 8; k++)
	 cout << v[k][0] <<" " << v[k][1]<<" " << v[k][2] << endl;
*/
#endif
}

double OBB::GetVolumeValue()
{
	return sizes[0]*sizes[1]*sizes[2];
}