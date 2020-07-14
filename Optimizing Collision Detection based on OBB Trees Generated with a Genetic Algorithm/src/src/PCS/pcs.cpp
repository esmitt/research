/*
#pragma warning(disable:4786)

#include <map>
#include <list>

using namespace std;
*/

#include <stdio.h>
#include <windows.h>
#include <.\gl\gl.h>
#include <.\gl\glut.h>
//#define GL_GLEXT_PROTOTYPES
//#include "..\..\include\glext.h"
//#include "..\..\include\wglext.h"
#include <math.h>
#include "..\..\include\defs.h"
#include "..\..\include\off.h"

#include "extgl.h"

typedef float f3[3];

int Width=500, Height=500;
bool viewOFFs = false;

f3 pan={0.f, 0.f, -0.f};

OFF obj1, obj2;

f3 pos = {0.f, 0.f, 0.f};
// zero = {

bool *PCS1, *PCS2;

unsigned int * query1, * query2;

void Init(void) 
{
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)Width/Height, 1.0f, 700.0f);
		glMatrixMode(GL_MODELVIEW);

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);

  float ambient[4] = {.2f, .2f, .2f, 1.f};
  float diffuse[4] = {.4f, .4f, .4f, 1.f};
  float specular[4] = {1.f, 1.f, 1.f, 1.f};
  float lightpos[4] = {0.f, 0.f, 10.f, 1.f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  glPolygonOffset(1.f, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // cargar la extension...

  //glParam();

	/*
 glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) Width, 0.0, (GLfloat) Height);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);								
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	*/
}

static bool QueryExtension(char *extName)
{
 char *p = (char *) glGetString(GL_EXTENSIONS);
 char *end = p + strlen(p);
 while (p < end) {
  unsigned int n = strcspn(p, " ");
  if ((strlen(extName)==n) && (strncmp(extName,p,n)==0)) {
   return true;
  }
  p += (n + 1);
 }
 return false;
}

void OnSize(int width, int height)
{
	Width=width;
	Height=height;
 glViewport(0,0,Width, Height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)Width/Height, 1.0f, 700.0f);
		glMatrixMode(GL_MODELVIEW);
/*	glViewport(0,0,Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) Width, 0.0, (GLfloat) Height);*/
}

void reshape(int w, int h)
{
	Width=w;
	Height=h;


  glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)w/h, 1.0f, 700.0f);
		glMatrixMode(GL_MODELVIEW);
}


class Quaternion
{
 public:
	float x,y,z,w;
	Quaternion(){x=y=z=0.0f;w=1.0f;};
};

Quaternion operator*(Quaternion a,Quaternion b) 
{
	Quaternion c;
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z; 
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y; 
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x; 
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w; 
	return c;
}

//
//        RotationArc()
//
//  This is the function from Games Programming Gems
//  that we are using in the VirtualTrackBall function
//
Quaternion RotationArc(f3 v0,f3 v1)
{
  Quaternion q;
  // v0.normalize(); 
  // v1.normalize();  // If vector is already unit length then why do it again?
  f3 c;
  cross(c, v0, v1);
  //vector3 c = CrossProduct(v0,v1);
  float d = dot(v0, v1);
  //float   d = DotProduct(v0,v1);
  float   s = (float)sqrt((1+d)*2);
  q.x = c[0] / s;
  q.y = c[1] / s;
  q.z = c[2] / s;
  q.w = s /2.0f;
  return q;
}

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

f3	position={0.f, 0.f, -50.f};    // of the object
Quaternion orientation; // of the object!!
float viewangle = 45.0f;

int mousex=0;
int mousey=0;
int mousexold=0;
int mouseyold=0;

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
  float  spread = (float)tan(viewangle/2*3.14/180);
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

void mousepassivemotion(int x,int y){
	mousexold=mousex;
	mouseyold=mousey;
	mousex=x;
	mousey = Height-y;
}

void Draw(void);

bool clickOnPanel = false;

void mouseactivemotion(int x,int y){
	mousepassivemotion(x,y); // update mouse x&y
 if (!clickOnPanel)
 {
  f3 mdir_old, mdir_new;
  MouseDir(mousexold,mouseyold,mdir_old);
  MouseDir(mousex,mousey,mdir_new);
  f3 zero={0.f, 0.f, 0.f};
  Quaternion q = VirtualTrackBall(zero,position,mdir_old,mdir_new);
  orientation = q*orientation;
 }
 Draw();
}

struct Material
{
 float Amb[4];
 float Diff[4];
 float Spec[4];
 float Exp;
 inline void set(void);
};

void Material::set()
{
 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diff);
 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spec);
 glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, Exp);
}

Material gold={
 {.24725f, .1995f, .0745f, 1.f},
 {.75164f, .60648f, .22648f, 1.f},
 {.628281f, .555802f, .366065f, 1.f},
 51.2f};
 
 Material chrome={
  {.25f, .25f, .25f, 1.f},
  {.4f, .4f, .4f, 1.f},
  {.774587f, .774587f, .774587f, 1.f},
  76.8f
 };

 Material polished_silver={
  {.23125f, .23125f, .23125f, 1.f},
  {.2775f, .2775f, .2775f, 1.f},
  {.773911f, .773911f, .773911f, 1.f},
  89.6f
 };

 Material pewter={
  {.10588f, .058824f, .113725f, 1.f},
  {.427451f, .470588f, .541176f, 1.f},
  {.3333f, .3333f, .521569f, 1.f},
  51.2f
 };

 Material perl={
  {.25f, .20725f, .20725f, 1.f},
  {1.f, 0.829f, 0.829f, 1.f},
  {0.296648f, 0.296648f, 0.296648f, 1.f},
  11.264f
 };

f3 zero = {0.f, 0.f, 0.f};

int pasada(OFF *o1, OFF *o2, bool *PCS1, bool *PCS2, f3 pos1, f3 pos2, unsigned * q2, f3 c1, f3 c2)
{
  
 /*
      asumo que ya esta limpio el zbuffer y que DepthFunc = GL_LESS y DepthMask = true

      Al final:

  Actualiza PCS2 y retorna el # triangulos en el PCS2

 */

 //glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
 //glDepthMask(GL_TRUE);
 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // ahora hay que hacer otra pasada
 
 glPushMatrix();
 glTranslatef(pos1[0], pos1[1], pos1[2]);    
 glColor3fv(c1);
 for(int i=0; i<o1->nf; i++)
 {
  if (PCS1[i])
   o1->renderFace(i);
 } 
 glPopMatrix();
 
 glDepthFunc(GL_GEQUAL);
 glDepthMask(GL_FALSE);
 glPushMatrix();
 glColor3fv(c2);
 glTranslatef(pos2[0], pos2[1], pos2[2]);    
 for(i=0; i<o2->nf; i++)
 {
  if (PCS2[i])
  {
   glBeginQueryARB(GL_SAMPLES_PASSED_ARB, q2[i]);
   o2->renderFace(i);
   glEndQueryARB(GL_SAMPLES_PASSED_ARB);
  }
 }
 glPopMatrix();
 
 glFlush();
 
 int c = 0, available;
 unsigned int sampleCount;
 
 for(i=0; i<o2->nf; i++)
 {
  glGetQueryObjectivARB(q2[i], GL_QUERY_RESULT_AVAILABLE_ARB, &available); 
  glGetQueryObjectuivARB(q2[i], GL_QUERY_RESULT_ARB, &sampleCount);
  PCS2[i] = sampleCount > 0;
  if (PCS2[i]) c++;
 }
 
 return c;
}

f3 blue = {0.f, 0.f, .7f};
f3 red = {1.f, 0.f, 0.f};

void Draw(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

 glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
 glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 memset(PCS1, 1, obj1.nf);
 memset(PCS2, 1, obj2.nf);
 
 //unsigned int cont[10];
 
 // glGenQueriesARB(10, cont);
 int  i, available;
 unsigned int sampleCount;

 int c1, c2, c;

 glDisable(GL_LIGHTING);     // no es necesario usar modelo de iluminacion!

 // cuadrar el project view para verlo desde un lado!
 // asumo que obj2 es fijo!!!

 glLoadIdentity();
 glTranslatef(0.f, 0.f, -3.f);
 glRotatef(90.f, 1.f, 0.f, 0.f);
 glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
 glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);

 //obj1.render();
 //obj2.render();

 c1 = pasada(&obj1, &obj2, PCS1, PCS2, pos, zero, query2, blue, red);
 printf("%d ", c1);

 if (c1)
 {
  glLoadIdentity();
  glTranslatef(0.f, 0.f, -3.f);
  glRotatef(90.f, 0.f, 1.f, 0.f);
  glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
  glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);
  glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //obj2.render();

  c2 = pasada(&obj2, &obj1, PCS2, PCS1, zero, pos, query1, red, blue);
  printf("%d (%d) ", c2, c1*c2);
  if (c2)
  {
   glLoadIdentity();
   glTranslatef(0.f, 0.f, -3.f);
   glRotatef(90.f, 0.f, 0.f, 1.f);
   glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
   glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);
   glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
   glDepthMask(GL_TRUE);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   c1 = pasada(&obj1, &obj2, PCS1, PCS2, pos, zero, query2, blue, red);
   printf("%d ", c1);

   if (c1)
   {
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -3.f);
    glRotatef(90.f, 1.f, 0.f, 0.f);
    glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
    glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);
    glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    c2 = pasada(&obj2, &obj1, PCS2, PCS1, zero, pos, query1, red, blue);
    printf("%d (%d) ", c2, c1*c2);

    if (c2)
    {
     glLoadIdentity();
     glTranslatef(0.f, 0.f, -3.f);
     glRotatef(90.f, 1.f, 0.f, 0.f);
     glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
     glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);
     glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
     glDepthMask(GL_TRUE);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     c1 = pasada(&obj1, &obj2, PCS1, PCS2, pos, zero, query2, blue, red);
     printf("%d ", c1);

     if (c1)
     {
      glLoadIdentity();
      glTranslatef(0.f, 0.f, -3.f);
      glRotatef(90.f, 0.f, 1.f, 0.f);
      glScalef(1.f/obj2.dist2center, 1.f/obj2.dist2center, 1.f/obj2.dist2center);
      glTranslatef(-obj2.center[0], -obj2.center[1], -obj2.center[2]);
      glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
      glDepthMask(GL_TRUE);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      c2 = pasada(&obj2, &obj1, PCS2, PCS1, zero, pos, query1, red, blue);
      printf("%d (%d) ", c2, c1*c2);
     }
    }
   }
  }

 }
 printf("\n");

 // esto dejarlo para el rendering final!
 glLoadIdentity();
 
 glTranslatef(pan[0], pan[1], pan[2]);
 
 float m[16];
 MakeMatrix4x4(m,position,orientation);
 glMultMatrixf(m);

 glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
 glDepthMask(GL_TRUE);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glEnable(GL_LIGHTING);

 glPushMatrix();
 glTranslatef(pos[0], pos[1], pos[2]);
 obj1.render();
 glDisable(GL_LIGHTING);
 glColor3f(1.f, 1.f, 1.f);
 for(i=0; i<obj1.nf; i++)
 {
  if (PCS1[i])
   obj1.renderWire(i);
 }
 glPopMatrix();
 glEnable(GL_LIGHTING);
 obj2.render();
 glDisable(GL_LIGHTING);
 //glColor3f(1.f, 1.f, 1.f);
 for(i=0; i<obj2.nf; i++)
 {
  if (PCS2[i])
   obj2.renderWire(i);
 }


/*
 glColor3f(0.f, 0.f, 1.f);
 glPushMatrix();
 glTranslatef(pos[0], pos[1], pos[2]);
 for(i=0; i<obj1.nf; i++)
  if (PCS1[i])
   obj1.renderFace(i);
 //obj1.render();
 glPopMatrix();
 glColor3f(1.f, 0.f, 0.f); 
 for(i=0; i<obj2.nf; i++)
  if (PCS2[i])
   obj2.renderFace(i);
   */

 goto fin;


// glLoadIdentity();


 if (c1)
 {
  glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  c2 = pasada(&obj2, &obj1, PCS2, PCS1, zero, pos, query1, red, blue);
  printf("%d ", c2);
  printf("(%d) ",c1*c2);
  if (c2)
  {
   glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
   glDepthMask(GL_TRUE);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   c1 = pasada(&obj1, &obj2, PCS1, PCS2, pos, zero, query2, blue, red);
   printf("%d ", c1);
   if (c1)
   {
    glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    c2 = pasada(&obj2, &obj1, PCS2, PCS1, zero, pos, query1, red, blue);
    printf("%d ", c2);
    printf("(%d) ",c1*c2);
   }
   //printf("(%d) ",c1*c2);
  }
 }

 printf("\n");

 goto fin;

 
 glColor3f(0.2f, .8f, 0.2f);

 glPushMatrix();
 glTranslatef(pos[0], pos[1], pos[2]);
 obj1.render();  // el primer objeto
 glPopMatrix(); 
 glColor3f(1.f, 0.5f, 1.f);
 
 glDepthFunc(GL_GEQUAL);            // solo pasan el depth test aquellos que estan por detras!
 glDepthMask(GL_FALSE);             // no escribimos en el depth buffer

 for(i=0; i<obj2.nf; i++)
 {
  glBeginQueryARB(GL_SAMPLES_PASSED_ARB, query2[i]);
  obj2.renderFace(i);                                      // dibujamos cara a cara
  glEndQueryARB(GL_SAMPLES_PASSED_ARB);
 }
 glFlush();                    // ????????????????

// int available=-1;
// unsigned int sampleCount;

 c = 0;    // para contar cuantas caras estan en el pcs

 for(i=0; i<obj2.nf; i++)
 {
  glGetQueryObjectivARB(query2[i], GL_QUERY_RESULT_AVAILABLE_ARB, &available); 
  glGetQueryObjectuivARB(query2[i], GL_QUERY_RESULT_ARB, &sampleCount);
  if (!available) printf("no");
  PCS2[i] = sampleCount > 0;   // si algun fragmento esta por detras (no se ve) entonces pertenece al PCS
  if (PCS2[i]) c++;
 }
 
 printf("%d ", c);  // numero de fragmentos en el PCS
 
 if (c>0)  // el PCS tiene objetos
 {
  glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // ahora hay que hacer otra pasada
  
  // ahora dibujamos unicamente los que estan en el PCS2:
  
  glColor3f(0.f, 1.f, 0.f);   // verde
  for(i=0; i<obj2.nf; i++)
  {
   if (PCS2[i])
   {
    obj2.renderFace(i);
   }
  }
  
  glColor3f(1.f, 1.f, 1.f);   // blanco
  
  glDepthFunc(GL_GEQUAL);
  glDepthMask(GL_FALSE);
  
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  
  for(i=0; i<obj1.nf; i++)
  {
   glBeginQueryARB(GL_SAMPLES_PASSED_ARB, query1[i]);
   obj1.renderFace(i);
   glEndQueryARB(GL_SAMPLES_PASSED_ARB);
  }
  
  glPopMatrix();
  
  glFlush();
  
  c = 0;
  
  for(i=0; i<obj1.nf; i++)
  {
   glGetQueryObjectivARB(query1[i], GL_QUERY_RESULT_AVAILABLE_ARB, &available); 
   glGetQueryObjectuivARB(query1[i], GL_QUERY_RESULT_ARB, &sampleCount);
   PCS1[i] = sampleCount > 0;
   if (PCS1[i]) c++;
  }
  
  printf("%d - ", c);
  if (c>0)  // el otro PCS tiene objetos
  {
   //printf("*    ");
   
   glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
   glDepthMask(GL_TRUE);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // ahora hay que hacer otra pasada
   
   glPushMatrix();
   glTranslatef(pos[0], pos[1], pos[2]);
   
   for(i=0; i<obj1.nf; i++)
   {
    if (PCS1[i])
     obj1.renderFace(i);
   }
   
   glPopMatrix();
   
   glDepthFunc(GL_GEQUAL);
   glDepthMask(GL_FALSE);
   for(i=0; i<obj2.nf; i++)
   {
    if (PCS2[i])
    {
     glBeginQueryARB(GL_SAMPLES_PASSED_ARB, query2[i]);
     obj2.renderFace(i);
     glEndQueryARB(GL_SAMPLES_PASSED_ARB);
    }
   }

   glFlush();
   
   c = 0;
   
   for(i=0; i<obj2.nf; i++)
   {
    glGetQueryObjectivARB(query2[i], GL_QUERY_RESULT_AVAILABLE_ARB, &available); 
    glGetQueryObjectuivARB(query2[i], GL_QUERY_RESULT_ARB, &sampleCount);
    PCS2[i] = sampleCount > 0;
    if (PCS2[i]) c++;
   }
   
   printf("(%d)",c);
   
   if (c>0)
   {
    glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // ahora hay que hacer otra pasada
    
    for(i=0; i<obj2.nf; i++)
    {
     if (PCS2[i])
      obj2.renderFace(i);
    }
    
    
    glDepthFunc(GL_GEQUAL);
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);    
    for(i=0; i<obj1.nf; i++)
    {
     if (PCS1[i])
     {
      glBeginQueryARB(GL_SAMPLES_PASSED_ARB, query1[i]);
      obj1.renderFace(i);
      glEndQueryARB(GL_SAMPLES_PASSED_ARB);
     }
    }
    glPopMatrix();
    
    glFlush();
    
    c = 0;
    
    for(i=0; i<obj1.nf; i++)
    {
     glGetQueryObjectivARB(query1[i], GL_QUERY_RESULT_AVAILABLE_ARB, &available); 
     glGetQueryObjectuivARB(query1[i], GL_QUERY_RESULT_ARB, &sampleCount);
     PCS1[i] = sampleCount > 0;
     if (PCS1[i]) c++;
    }
    
    printf("(%d)    ",c);
   }

  }
 }   // el PCS tiene objetos


 glDepthFunc(GL_LESS);    // restauramos el depth buffer como estaba originalmente
 glDepthMask(GL_TRUE);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //glEnable(GL_LIGHTING);

 glDepthFunc(GL_LESS);
 glDepthMask(GL_TRUE);

 glColor3f(0.f, 0.f, 1.f);
 glPushMatrix();
 glTranslatef(pos[0], pos[1], pos[2]);
 for(i=0; i<obj1.nf; i++)
  if (PCS1[i])
   obj1.renderFace(i);
 //obj1.render();
 glPopMatrix();
 glColor3f(1.f, 0.f, 0.f); 
 for(i=0; i<obj2.nf; i++)
  if (PCS2[i])
   obj2.renderFace(i);
 //obj2.render();
 

/*
glBeginQueryARB(GL_SAMPLES_PASSED_ARB, cont[0]);
 obj1.render();
glEndQueryARB(GL_SAMPLES_PASSED_ARB);
 obj2.render();

 glFlush();
 */

 //int available=-1;
 //unsigned int sampleCount;

//glGetQueryObjectivARB(cont[0], GL_QUERY_RESULT_AVAILABLE_ARB, &available);

//glGetQueryObjectuivARB(cont[0], GL_QUERY_RESULT_ARB, &sampleCount);
//printf("%d %d\n", sampleCount, available);

// glDeleteQueriesARB(10, cont);
fin:
	glutSwapBuffers();
}

static void Key(unsigned char key, int x, int y)
{
 switch (key)
 {
 case 27:
  exit(0);
 }

// Draw();
}

#define mov 1.0f

void specialKey(int key, int x, int y)
{
 //printf("%u",key);
	switch (key) {
 case 100: // izq 
  pos[0] += mov;
  break;
 case 102: // der
  pos[0] -= mov;
  break;
 case 101: // up
  pos[1] += mov;
  break;
 case 103: // down
  pos[1] -= mov;
  break;
 case 104: // pgup
  pos[2] += mov;
  break;
 case 105: // pgdn
  pos[2] -= mov;
  break;
	}
 Draw();
}

void mouseClick(int button, int state, int x, int y)
{
 /*
 y = Height - y;
 if (x>=10 && x<=110 && y>=10 && y<=110 && state==0)
 {
  clickOnPanel = true;
  if (checkClickOnPanel(x, y)) Draw();
 }
 else clickOnPanel=false;
 */
}

int main(int argc, char **argv)
{
 obj1.load("..\\..\\data\\FinalProtesis.off");          
 //obj2.load("..\\..\\data\\cuchillo.off");   // obj2 es fijo!
 //obj2.load("..\\..\\data\\PieEscalado2.off");   // obj2 es fijo!
 obj2.load("..\\..\\data\\FinalTibiaSinTapa.off");   // obj2 es fijo!

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("GLUTBASE");
	Init();

 //int r = ewgl_InitExtensions();

 if (!QueryExtension("GL_ARB_occlusion_query"))
 {
  printf("Extension GL_ARB_occlusion_query not found\n");
  return -1;
 }
 printf("Extension GL_ARB_occlusion_query found\n");

 query1 = new unsigned int [obj1.nf];
 query2 = new unsigned int [obj2.nf];

 glGenQueriesARB(obj1.nf, query1);
 glGenQueriesARB(obj2.nf, query2);

 PCS1 = new bool[obj1.nf];
 PCS2 = new bool[obj2.nf];

 //Init();

	glutReshapeFunc(OnSize);
	glutReshapeWindow(Width,Height);
 glutDisplayFunc(Draw);
	glutKeyboardFunc(Key);
 glutPassiveMotionFunc(mousepassivemotion);
 glutMotionFunc(mouseactivemotion);
 glutSpecialFunc(specialKey);
 glutMouseFunc(mouseClick);

	glutMainLoop();

 // faltan los delete!

 return 0;
}