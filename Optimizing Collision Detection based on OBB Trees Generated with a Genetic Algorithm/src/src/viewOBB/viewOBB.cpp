/*
#pragma warning(disable:4786)

#include <map>
#include <list>

using namespace std;
*/

#include <stdio.h>
#include <windows.h>
#include "GL/glew.h"
#include <.\gl\gl.h>
#include <.\gl\glut.h>
#include <math.h>

#include "..\..\include\defs.h"
#include "..\..\include\obbtree.h"
#include "..\..\include\off.h"

///#include <GL/glut.h>
/// Shaders handles
GLhandleARB m_program;
GLhandleARB m_fragmentShader, m_vertexShader;
#ifdef WIN32
	#pragma comment(lib, "glut32.lib")
	#pragma comment(lib, "glew32.lib")
#endif

OBBTree T;
OFF obj;

typedef float f3[3];

bool wireFrame = false;

char* fileRead(char* name)
{
	FILE *file;
	char *content = NULL;
	int count = 0;

	if(name != NULL)
	{
		file = fopen(name, "rt");
		if (file == NULL)return content;
		fseek(file, 0, SEEK_END);
		count = ftell(file);
		rewind(file);
		if(count > 0)
		{
			content = (char*)malloc(sizeof(char)*(count+1));
			count = (int)fread(content, sizeof(char), count, file);
			content[count] = '\0';
		}
		fclose(file);
	}
	return content;
}

void setFragmentShader()
{
	char* sourceCode;
	m_fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	sourceCode = fileRead("fragment.sh");
	const char* fragmentCode = sourceCode;
	glShaderSourceARB(m_fragmentShader, 1, &fragmentCode, NULL);
	free(sourceCode);
	glCompileShaderARB(m_fragmentShader);
}

void setVertexShader()
{
	char* sourceCode;
	m_vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	sourceCode = fileRead("vertex.sh");
	const char* vertexCode = sourceCode;
	glShaderSourceARB(m_vertexShader, 1, &vertexCode, NULL);
	free(sourceCode);
	glCompileShaderARB(m_vertexShader);
}

void printInfoLog(GLhandleARB obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
						&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

/*
inline void subf3(f3 r, f3 a, f3 b)
{
 r[0] = a[0] - b[0];
 r[1] = a[1] - b[1];
 r[2] = a[2] - b[2];
}

inline void addf3(f3 r, f3 a, f3 b)
{
 r[0] = a[0] + b[0];
 r[1] = a[1] + b[1];
 r[2] = a[2] + b[2];
}

inline void floatbyf3(f3 r, float a, f3 b)
{
 r[0] = a * b[0];
 r[1] = a * b[1];
 r[2] = a * b[2];
}

inline void cross(f3 V, f3 a, f3 b)
{
 V[0] = a[1]*b[2]-a[2]*b[1];
 V[1] = a[2]*b[0]-a[0]*b[2];
 V[2] = a[0]*b[1]-a[1]*b[0];
}

inline float mod(f3 v)
{
 return (float) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

inline void normalize(f3 v)
{
 float n = mod(v);
 v[0]/=n;
 v[1]/=n;
 v[2]/=n;
}

inline float dot(f3 a, f3 b)
{
 return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
*/
int Width=500, Height=500;
bool viewOFFs = false;

f3 pan={0.f, 0.f, 48.f};
f3 trl = {0.f, 0.f, 0.f};

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

 Material blue={{0.f, 1.f, 1.f, 1.f},
                {0.f, 1.f, 1.f, 1.f},
                {0.f, 1.f, 1.f, 1.f},
                50.f
 };

 Material red={{1.f, 0.f, 0.f, 1.f},
                {1.f, 0.f, 0.f, 1.f},
                {1.f, 0.f, 0.f, 1.f},
                50.f
 };

 Material green={{0.f, 1.f, 0.f, 1.f},
                 {0.f, 1.f, 0.f, 1.f},
                 {0.f, 1.f, 0.f, 1.f},
                50.f
 };

void Init(void) {

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)Width/Height, 0.05f, 700.0f);
		glMatrixMode(GL_MODELVIEW);

  //glEnable(GL_LIGHTING);
  //glEnable(GL_NORMALIZE);
  //glEnable(GL_LIGHT0);

 //OpenGL
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	
	float lightAmbient[] = {1,0.5,0};
	//float lightPosition[] = {5,4,-1, 0};
	float lightPosition[] = {0.3,-0.35,0.5, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	float lightDiffuse[] = {1,1,1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	float lightSpecular[] = {1,1,1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightSpecular);

	float matAmbient[] = {0.1,0.1,0.1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	//float matDiffuse[] = {0.2,0.1,1};
	float matDiffuse[] = {0,0.6,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	float matSpecular[] = {0.9,0.0,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	float shininess = 20;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//polished_silver.set();
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//GLSL
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(0);
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	printf("Ready for GLSL\n");
	else
	{
		printf("Not totally ready :( \n");
		exit(1);
	}
	
	setVertexShader();
	setFragmentShader();
	m_program = glCreateProgramObjectARB();
	glAttachObjectARB(m_program, m_fragmentShader);
	glAttachObjectARB(m_program, m_vertexShader);
	glLinkProgramARB(m_program);
	glUseProgramObjectARB(m_program);
	
	//glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);
  //glPolygonOffset(1.f, 1.f);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_NORMALIZE);	
  //glShadeModel(GL_SMOOTH);
	
  //glParam();

	/*
 glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) Width, 0.0, (GLfloat) Height);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);								
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	*/

 /// glEnable(GL_LINE_SMOOTH);
   /// glEnable(GL_BLEND);
   /// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   /// glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    //glLineWidth(1.5);

}

void OnSize(int width, int height)
{
	Width=width;
	Height=height;
 glViewport(0,0,Width, Height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45.0f, (GLdouble)Width/Height, 1.0f, 700.0f);
		gluPerspective(45.0f, (GLdouble)Width/Height, .05f, 700.0f);
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
		//gluPerspective(45.0f, (GLdouble)w/h, 0.05f, 700.0f);
		gluPerspective(45.0f, (GLdouble)w/h, .05f, 700.0f);
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

int button = 0;
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
 
 if (button==1)
 {
  f3 mdir_old, mdir_new;
  MouseDir(mousexold,mouseyold,mdir_old);
  MouseDir(mousex,mousey,mdir_new);
  f3 zero={0.f, 0.f, 0.f};
  Quaternion q = VirtualTrackBall(zero,position,mdir_old,mdir_new);
  orientation = q*orientation;
  Draw();
 }
 else if (button==2)
 {
  // pan
  pan[0] += (mousex - mousexold)/40.f;
  pan[1] += (mousey - mouseyold)/40.f;
  Draw();
 }
 else if (button==3)
 {
  // zoom
  pan[2] += (mousey - mouseyold)/40.f;
  Draw();
 }
}



bool some = false;
int value = 3;
void Draw(void)
{
	//glClearColor(0.0f, .0f, .0f, 0.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 glClearStencil(0x0);

 glLoadIdentity();

 //glTranslatef(pan[0], pan[1], pan[2]);

 glTranslatef(pan[0], pan[1]-0.1f, pan[2]+1.70f);
 
 //glTranslatef(pan[0], pan[1], pan[2]);

 float m[16];
 MakeMatrix4x4(m,position,orientation);
 glMultMatrixf(m);

 ///glRotatef(25,1.5,0.5, 0);
 glRotatef(90,1,0, 0);
 //glColor3f(0.f, 0.f, 0.f);
 //glColor3f(1.f, 0.62f, 0.f);
 //glLineWidth(2.0);
 if(some)
 {
	glDisable(GL_LIGHTING);
	//glColor3ub(255,255,255);
	//obj.render();
	glLineWidth(1.5f);
	glColor3ub(0,0,255);
	 T.render(value);
	 glDisable(GL_CULL_FACE);
	 glLineWidth(1.f);
	 glEnable(GL_LIGHTING);
 }
 else
	 obj.render();
 //glutSolidTeapot(0.5);
 //obj.render();
 //T.renderColors(&obj);
 /*
 if (wireFrame)
 {
  glColor3f(1.f, 1.f, 1.f);
  obj.renderWire();
 }
*/
 //obj.render();
 //obj.renderWire();
	glutSwapBuffers();
}
void destroy()
{
	glDetachObjectARB(m_program, m_fragmentShader);
	glDetachObjectARB(m_program, m_vertexShader);
	glDeleteObjectARB(m_program);
}
static void Key(unsigned char key, int x, int y)
{
 switch (key)
 {
 case 'w':
  wireFrame = !wireFrame;
  Draw();
  break;
 case '+':
	 value++;
	 break;
 case '-':
	 value--;
	 break;
 case 't':
	 some = !some;
	 break;
 case 27:
  destroy();
	 exit(0);
 }
 glutPostRedisplay();

}

static void specialKey(int key, int x, int y)
{
 //printf("%u",key);
	switch (key) {
 case 100: // izq
  trl[0] += 0.05f;
  break;
 case 102: // der
  trl[0] -= 0.05f;
  break;
 case 101: // up
  trl[1] += 0.05f;
  break;
 case 103: // down
  trl[1] -= 0.05f;
  break;
 case 104: // pgup
  trl[2] += 0.05f;
  break;
 case 105: // pgdn
  trl[2] -= 0.05f;
  break;
	}
 Draw();

}


void mouseClick(int button, int state, int x, int y)
{
 if (state==0) // presionar
 {
  ::button = button + 1;
 }
 else ::button = 0;

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

void listExtensions()
{
 //char str[4096] = glGetString(GL_EXTENSIONS);
 //return;

 char *p = (char *) glGetString(GL_EXTENSIONS);
 printf("%s\n", p);
 char *end = p + strlen(p);
 int e = 0;
 while (p < end) {
  e++;
  unsigned int n = strcspn(p, " ");

  //if ((strlen(extName)==n) && (strncmp(extName,p,n)==0)) {
   
 // }
  p += (n + 1);
 }
 printf("%d extensions\n", e);
}




int main(int argc, char **argv)
{
 //glMinmaxEXT();
 //ob1.load("finalprotesis.off");
 //ob2.load("finaltibiasintapa.off");
 //ob1.load("cat10.off");
 //ob2.load("teapot.off");

 	//obj.load("BUNNY.OFF");
	//T.read("BUNNY.obb");
	obj.load("questionmark.OFF");
	T.read("questionmark.obb");
 //T.read("protesis.obb");
 //obj.load("..\\..\\data\\FinalProtesis.off");
 //T.read("tibia.obb");
	//T.read("some.obb");
 //obj.load("FinalTibiaSinTapa.off");
 //T.read(argv[1]);
 //obj.load(argv[2]);
	//T.read("C:\\Users\\esmitt.LABCOMPGRAF\\Desktop\\obbs\\src\\createobb\\MCsphere.obb");
	//T.read("C:\\Users\\esmitt.LABCOMPGRAF\\Desktop\\testing\\FinalProtesisMC.obb");
	//obj.load("C:\\Users\\esmitt.LABCOMPGRAF\\Desktop\\testing\\FinalProtesis.off");
	//T.read("C:\\Users\\esmitt.LABCOMPGRAF\\Desktop\\obbs\\src\\createobb\\Genem30.obb");
	//obj.load("C:\\Users\\esmitt.LABCOMPGRAF\\Desktop\\obbs\\src\\createobb\\m30.off");
 T.assignColors();
 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH);
	glutCreateWindow("GLUTBASE");
	Init();

 //char version[1024];
 //strcpy(version,(const char *) glGetString(GL_VENDOR));
 printf("%s\n", glGetString(GL_VENDOR));
 printf("%s\n", glGetString(GL_RENDERER));
 printf("OpenGL %s\n", glGetString(GL_VERSION));
 //listExtensions();
 printf("value =%lf\n",T.GetVolumeValue());
 //printf("leaves =%d\n",T.countLeaves());
 //printf("total =%d\n",T.getTotal());
 //printf("worst =%d\n",T.getWorst());
 printf("nt average =%lf\n",(double)T.getTotal()/(double)T.countLeaves());
 //printf("get path lenght =%d\n",T.GetPathLenght());
 //double fDepthAverage = (double)T.DepthAverage()/(double)T.countLeaves();
 //printf("depth average =%lf\n", fDepthAverage);
 //double fStandardDeviation = (T.StandardDeviation(fDepthAverage)*1.0)/((double)T.countLeaves() - 1);
 //printf("standard deviation =%lf\n", sqrt(fStandardDeviation));
 bool ext = QueryExtension("GL_ARB_vertex_program");
 if (ext) printf("stencil_wrap\n");

 ext = QueryExtension("GL_EXT_histogram");
 if (ext) printf("stencil_wrap\n");

 glutReshapeWindow(1024,758);

	
 glutReshapeFunc(OnSize);
	glutReshapeWindow(Width,Height);
 glutDisplayFunc(Draw);
	glutKeyboardFunc(Key);
 glutPassiveMotionFunc(mousepassivemotion);
 glutMotionFunc(mouseactivemotion);
 glutSpecialFunc(specialKey);
 glutMouseFunc(mouseClick);

	glutMainLoop();

 return 0;
}