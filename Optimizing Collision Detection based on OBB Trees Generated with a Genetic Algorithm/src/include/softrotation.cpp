#include <windows.h>
#include <.\gl\gl.h>
#include "softrotation.h"

void planelineintersection(f3 n,float d,f3 p1,f3 p2, f3 r)
{
  // returns the point where the line p1-p2 intersects the plane n&d
  f3 dif;
  subf3(dif, p2 , p1);
  float dn= dot(n,dif);
  float t = -(d+dot(n,p1) )/dn;
  floatbyf3(r,t,dif);
  addf3(r,r,p1);
  //return p1 + (dif*t);
}

void MakeMatrix4x4(float *m,const f3 v,const Quaternion &q) 
{
	// a lean function for filling m[16] with
	// a 4x4 transformation matrix based on 
	// translation v and rotation q
	// This routine would likely be used by an opengl
	// programmer calling glmultmatrix()
	m[0] = 1-2*(q.y*q.y+q.z*q.z);
	m[1] = 2*(q.x*q.y+q.w*q.z)  ;
	m[2] = 2*(q.x*q.z-q.w*q.y)  ;
	m[3] = 0            ;
	m[4] = 2*(q.x*q.y-q.w*q.z)  ;	
	m[5] = 1-2*(q.x*q.x+q.z*q.z);
	m[6] = 2*(q.y*q.z+q.w*q.x)  ;
	m[7] = 0    ;   
	m[8] = 2*(q.x*q.z+q.w*q.y)  ; 
	m[9] = 2*(q.y*q.z-q.w*q.x)  ; 
	m[10]= 1-2*(q.x*q.x+q.y*q.y);
	m[11]= 0    ; 
	m[12] = v[0];
	m[13] = v[1] ;
	m[14] = v[2] ;
	m[15] = 1.0f;
}

void MouseDir(int x,int y, f3 r){
  // Converts x and y in screen coords releative to opengl window
  // To a vector pointing into the world
  //assert(viewangle >0.0f);     // Note this must be Perspective
  GLint vp[4];
  // just use the current viewport!
  glGetIntegerv(GL_VIEWPORT,vp);
  x-=vp[0];
  y-=vp[1];
  // cull x,y with respect to the viewport
  float  spread = (float)tan(45.f/2*3.14/180);
  r[0] = spread * (x-vp[2]/2.0f) /(vp[3]/2.0f);
  r[1] = spread * (y-vp[3]/2.0f) /(vp[3]/2.0f);
  r[2] = -1.0f;      
}

Quaternion VirtualTrackBall(f3 cop,f3 cor,f3 dir1,f3 dir2) 
{
	// Implement track ball functionality to spin stuf on the screen
	//  cop   center of projection
	//  cor   center of rotation
	//  dir1  old mouse direction 
	//  dir2  new mouse direction
	// pretend there is a sphere around cor.  Then find the points
	// where dir1 and dir2 intersect that sphere.  Find the
	// rotation that takes the first point to the second.
	// If the line "dir" doesn't hit the sphere then take the closest
	// point on the sphere to that line.
	float m;
	// compute plane 
	f3 nrml;
 subf3(nrml, cor, cop);
	float fudgefactor = 1.0f/(mod(nrml) * 0.25f); // since trackball proportional to distance from cop
	normalize(nrml);
	float dist = -dot(nrml,cor);
 f3 u;
 f3 tmp;
 addf3(tmp,cop,dir1);
	planelineintersection(nrml,dist,cop,tmp,u);
 subf3(u,u,cor);
 floatbyf3(u,fudgefactor,u);
	m= mod(u);
	if(m>1) 
    {
  floatbyf3(u,1.f/m,u);
       // u=u*1.0f/m;
    }
	else 
    {
  f3 t1;
  floatbyf3(t1, (float)sqrt(1-m*m), nrml);
  subf3(u,u,t1);
	}
 f3 v,t1;
 addf3(t1,cop,dir2);
	planelineintersection(nrml,dist,cop,t1,v);
 subf3(v,v,cor);
 floatbyf3(v,fudgefactor,v);
	m= mod(v);
	if(m>1) 
    {
        floatbyf3(v,1.f/m,v);
    }
	else 
    {
  f3 t1;
  floatbyf3(t1,(float)sqrt(1-m*m),nrml);
  subf3(v,v,t1);
	//	v=v - (nrml * (float)sqrt(1-m*m));
	}
	return RotationArc(u,v);
}
