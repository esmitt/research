#include <./gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <list>
#include <sys/types.h>
#include <sys/timeb.h>

#include "../../include/off.h"
#include "../../include/aabbtree.h"
#include "../../include/octree.h"
#include "../../include/obbtree.h"
#include "../../include/collisiondetection.h"
#include "../../include/materials.h"
#include "../../include/ruta.h"
#include "../../include/aabbtreecollision.h"
#include "../../include/obbtreecollision.h"
#include "../../include/quaternion.h"
#include "../../include/softrotation.h"

using namespace std;

int fullscreen;		
int x_position;		
int y_position;
int width;	
int height;

CollisionDetection *CD;

Ruta movement;

_timeb t_ini, t_fin;

bool playing = false;

int moveMode = 1; // 1: traslacion
                  // 0: escalamiento

int viewBB = false;

class OBBTree;

AABBTree *T1, *T2;
OBBTree *T1obb, *T2obb;
OctTree octree;

f3 pan= {0.f, 0.f, 0.f};

bool CDActivated = 0;
bool creatingRoute = 0;
list <float *> route;

bool renderTree = false;

int angX=0, angY=0, angZ=0;

int mousex=0;
int mousey=0;
int mousexold=0;
int mouseyold=0;

char file1[80], file2[80];

int cont_colis;
int cont_pasos;

int mousebutton;

f3	position={0.f, 0.f, -50.f};    // of the object

int dir = -1; // no dir!
float delmov = 0.5f;
int method = -1;    // 0 para AABBTree

int automatic = 0;

OFF obj1, obj2;
f3  pos1={0.f, 0.f, 0.f};
f3  pos2={0.f, 0.f, 0.f};

float M1[16], M2[16]; // matrices de transformacion para M1 y M2

#define getstring(f, s, t) {int l;fgets(s, t, f); l = strlen(s); s[l-1] = '\0';}

Quaternion orientation; // of the object!!


void mousepassivemotion(int x,int y){
	mousexold=mousex;
	mouseyold=mousey;
	mousex=x;
	mousey = height-y;
}

void Draw(void);

void mouseactivemotion(int x,int y){
	mousepassivemotion(x,y); // update mouse x&y

 if (mousebutton==0)  // lb
 {
  f3 mdir_old, mdir_new;
  MouseDir(mousexold,mouseyold,mdir_old);
  MouseDir(mousex,mousey,mdir_new);
  f3 zero={0.f, 0.f, 0.f};
  Quaternion q = VirtualTrackBall(zero,position,mdir_old,mdir_new);
  orientation = q*orientation;
 }
 else if (mousebutton==1)  // ruedita
 {
  int x = mousex - mousexold;
  int y = mousey - mouseyold;
  float dx = (float)x / 10.f;
  float dy = (float)y / 10.f;
  pan[0] += dx;
  pan[1] += dy;
 }
 else  // rb
 {
  int z = mousey - mouseyold;
  float dz = (float) z / 10.f;
  pan[2] += dz;
 }
 Draw();
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{				
 if (height == 0) {		
  height = 1;		
 }
 
 glViewport(0, 0, width, height);	
 
 glMatrixMode(GL_PROJECTION);	
 glLoadIdentity();		
 
 gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 500.0f);
 
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();		
}

int InitGL(int width, int height)
{				
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
 glEnable(GL_DEPTH_TEST);	
 glShadeModel(GL_SMOOTH);	
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 return 1;	
}

GLvoid Draw(GLvoid)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();

 glTranslatef(pan[0], pan[1], pan[2]);
 glTranslatef(0.f,0.f,-50.f);
 float m[16];
 MakeMatrix4x4(m,position,orientation);
 glMultMatrixf(m);

 //obj1.render();
 gold.set();
 glPushMatrix();
  glTranslatef(pos1[0], pos1[1], pos1[2]);
  glTranslatef(obj1.center[0], obj1.center[1], obj1.center[2]);
  glRotated(angX, 1, 0, 0);
  glRotated(angY, 0, 1, 0);
  glRotated(angZ, 0, 0, 1);
  glTranslatef(-obj1.center[0], -obj1.center[1], -obj1.center[2]);
  if (renderTree)
  {
   glColor3f(0.f, 0.f, 1.f);
   glDisable(GL_LIGHTING);
   T1obb->render(); 
   //T1->render(); 
   //octree.render();
   glEnable(GL_LIGHTING);
  }
  else 
  {
   if (viewBB) 
   {
    glDisable(GL_LIGHTING);
    T1obb->root->BB.render();
    //T1->root->BB.render();
    glEnable(GL_LIGHTING);
   }
   obj1.render();
  }
  glGetFloatv(GL_MODELVIEW_MATRIX, M1);
 glPopMatrix();
 
 perl.set();
 glPushMatrix();
  glTranslatef(pos2[0], pos2[1], pos2[2]);
  if (renderTree)
  {
   glColor3f(1.f, 0.f, 0.f);
   glDisable(GL_LIGHTING);
   //T2->render(); 
   T2obb->render(); 
   glEnable(GL_LIGHTING);
  }
  else 
  {
   if (viewBB) 
   {
    glDisable(GL_LIGHTING);
    //T2->root->BB.render();
    T2obb->root->BB.render();
    glEnable(GL_LIGHTING);
   }
   obj2.render();
  }
  glGetFloatv(GL_MODELVIEW_MATRIX, M2);
 glPopMatrix();

 if (creatingRoute)
 {
  glDisable(GL_LIGHTING);
  AABB bb(&obj1);
  f3 center;
  bb.getCenter(center);
  glTranslatef(center[0], center[1], center[2]);
  list <float*>::iterator it = route.begin();
  
  glColor3f(0.f, 0.f, 1.f);
  glBegin(GL_LINE_STRIP);
  while(it!=route.end())
  {
   float *node = *it;
   glVertex3fv(node);
   it++;
  }
  glColor3f(1.f, 1.f, 1.f);
  glVertex3fv(pos1);  

  glEnd();
  glEnable(GL_LIGHTING);
 }

 glColor3f(0.f, 0.f, 1.f);
 glutSwapBuffers();
}

void keyReleased(unsigned char key, int x, int y)
{
}

void releaseRoute()
{
 list <float*>::iterator it = route.begin();
 while(it!=route.end())
 {
  float *l = *it;
  delete [] l;
  it++;
 }
 route.clear();
}

void saveRoute()
{
 char fname[80];
 getstring(stdin, fname, 80);
 FILE *f = fopen(fname, "wb");
 if (!f)
 {
  printf("can't open %s\n");
  releaseRoute();
  return;
 }
 int s = route.size();
 fwrite(&s, sizeof(float *), 1, f);
 list <float*>::iterator it = route.begin();
 while(it!=route.end())
 {
  float *l = *it;
  fwrite(l, sizeof(float *), 3, f);
  it++;
 }
 fclose(f);
}

void addPoint2Route()
{
 float *newVertex = new float[3];
 copyf3(newVertex, pos1);
 route.push_back(newVertex);
}

void keyPressed(unsigned char key, int x, int y)
{
 if (key==27) exit(0);
 switch(key)
 {
 case 27:
  exit(0);
 case ' ':
  if (creatingRoute)
  {
   printf("Can't enable collision detection while creating a route");
   return;
  }
  CDActivated = !CDActivated;
  printf("%s\n",CDActivated?"Collision detection enabled":"Collision detection disabled");
  //Draw();
  break;
 case 'r':  // crea una ruta
  if (!creatingRoute)
  {
   CDActivated = false;
   printf("Creating route: hit 'n' to include point. 'r' to finish\nCollision detection disabled\n");
   creatingRoute = 1;
  }
  else
  {
   creatingRoute = 0;
   printf("Route complete\nEnter filename to save route: ");
   saveRoute();
  }
  break;
 case 'n':  // agrega un punto a la ruta
  if (creatingRoute)
  {
   printf("Point (%f,%f,%f) added to route\n", pos1[0], pos1[1], pos1[2]);
   addPoint2Route();
  }
  break;
 case 'p': // play route
  if (automatic)
  {
   _ftime(&t_ini);
   printf("Playing route\n");
   movement.begin(1.f);
   playing = true;
   cont_colis=cont_pasos=0;

   //Draw();
  }
  else
  {
   printf("Need automatic mode to play route\n");
  }
  break;
 case 't':
  renderTree = !renderTree;
  Draw();
  break;
 case 'b':
  viewBB = !viewBB;
  Draw();
  break;
 case 'c': // check
  Draw();
  {bool r = CD->Detect();}
  break;
 case 'm': // modo: traslacion/rotacion
  moveMode = !moveMode;
 }
}

void specialKeyPressed(int key, int x, int y)
{
 // 100, 101, 102, 103, 104, 105 left, up ,right, down, pgup, pgdn
 switch(key)
 {
 case 100:  // left
  dir = 0;
  break;
 case 102:  // right
  dir = 1;
  break;
 case 101:  // up
  dir = 2;
  break;
 case 103:  // down
  dir = 3;
  break;
 case 104:  // pgup
  dir = 4;
  break;
 case 105:  // pgdn
  dir = 5;
  break;
 }
 //Draw();
}

void specialKeyReleased(int key, int x, int y)
{
 dir = -1;
}

f3 posant;
bool lastcd = false;

GLvoid Idle(GLvoid)
{
 if (playing)
 {
  //movement.next();
  if (!movement.end())
  {
   movement.next();
   cont_pasos++;
   if (!CDActivated)
    copyf3(pos1, movement.currentPos);
   else
   {
    if (CD->Detect())
    {
     cont_colis++;
     printf("~");
     copyf3(pos1, posant);
     //lastcd = true;
    }
    else
    {
     printf(".");
     copyf3(posant, pos1);
     copyf3(pos1, movement.currentPos);
    }
   }
   Draw();
  }
  else
  {
   _ftime(&t_fin);
   playing = false;
   printf("\nRoute ended\n");
   unsigned int ms = (t_fin.time - t_ini.time + 1)*1000 + (1000-t_ini.millitm) + t_fin.millitm;
   printf("%u msec\n", ms);
   printf("%d/%d\n", cont_colis,cont_pasos);
  }
  return;
 }

 if (moveMode==1)   // traslacion
 {
  switch(dir)
  {
  case 0: //
   copyf3(posant, pos1);
   pos1[0] -= delmov;
   Draw();
   break;
  case 1: //
   copyf3(posant, pos1);
   pos1[0] += delmov;
   Draw();
   break;
  case 2: //
   copyf3(posant, pos1);
   pos1[1] -= delmov;
   Draw();
   break;
  case 3: //
   copyf3(posant, pos1);
   pos1[1] += delmov;
   Draw();
   break;
  case 4: //
   copyf3(posant, pos1);
   pos1[2] -= delmov;
   Draw();
   break;
  case 5: //
   copyf3(posant, pos1);
   pos1[2] += delmov;
   Draw();
   break;
  } 
 }
 else          // rotacion
 {
  switch(dir)
  {
  case 0: //
   angX += 5;
   //copyf3(posant, pos1);
   //pos1[0] -= delmov;
   Draw();
   break;
  case 1: //
   angX -= 5;
   //copyf3(posant, pos1);
   //pos1[0] += delmov;
   Draw();
   break;
  case 2: //
   angY += 5;
   //copyf3(posant, pos1);
   //pos1[1] -= delmov;
   Draw();
   break;
  case 3: //
   angY -= 5;
   //copyf3(posant, pos1);
   //pos1[1] += delmov;
   Draw();
   break;
  case 4: //
   angZ += 5;
   //copyf3(posant, pos1);
   //pos1[2] -= delmov;
   Draw();
   break;
  case 5: //
   angZ -= 5;
   //copyf3(posant, pos1);
   //pos1[2] += delmov;
   Draw();
   break;
  }
 }

 //if (CDActivated && CD->Detect())
 if (CDActivated)
  if (CD->Detect())
 {
  copyf3(pos1, posant);
  printf("~");
 }
 
}

/* How to stop rendering if obscured/iconised */
void Icon(int state)
{
 switch (state) {
 case GLUT_VISIBLE:
  glutIdleFunc(Idle);
  break;
 case GLUT_NOT_VISIBLE:
  glutIdleFunc(NULL);
  break;
 default:
  break;
 } 
}

void mouseClick(int button, int state, int x, int y)
{
  mousebutton = button;
}

CollisionDetection *init_aabb()
{
 T1 = new AABBTree();
 T1->read(file1);
 T2 = new AABBTree();
 T2->read(file2);
 AABBTreeCollision *cdObject = new AABBTreeCollision(T1, T2, &obj1, &obj2, M1, M2);
 return cdObject;
}

CollisionDetection *init_obb()
{
 T1obb = new OBBTree();
 T1obb->read(file1);
 T2obb = new OBBTree();
 T2obb->read(file2);
 OBBTreeCollision *cdObject = new OBBTreeCollision(T1obb, T2obb, &obj1, &obj2, M1, M2);
 return cdObject;
}


CollisionDetection * init_method(int method)
{
 switch(method)
 {
 case 0:
  return init_aabb();
  break;
 case 1:
  return init_obb();
  break;
 }
 return NULL;
}

int main(int argc, char **argv)
{
 fullscreen = 0;		
 x_position = 50;	
 y_position = 50;
 width = 640;	
 height = 480;

 const char defaultfname[] = "test.ini";
 const char *fname = defaultfname;

 if (argc>1)
 {
  fname = argv[1];
 }

 printf("using file %s\n", fname);

 char obj1off[80], obj2off[80];
 FILE *f = fopen(fname, "r");
 if (!f)
 {
  printf("Archivo test.ini no existe o esta malo\n");
  return 0;
 }
 char algo[20];
 char movingstyle[20];
 getstring(f, obj1off, 80)       // movil
 getstring(f, obj2off, 80)       // fijo 
 getstring(f, file1, 80)      // movil
 getstring(f, file2, 80)      // fijo
 getstring(f, algo, 80)
 getstring(f, movingstyle, 80)

 if (!strcmpi(algo,"AABBTREE")) method = 0;
 else if (!strcmpi(algo, "OBBTREE")) method = 1;

 if (method==-1)
 {
  printf("Method not recognized\nAborting\n");
  return 0;
 }

 CD = init_method(method);

 if (!strcmpi(movingstyle, "MANUAL")) automatic = 0;
 else 
 {
  char fname[80];
  getstring(f, fname, 80)
  movement.load(fname);
  automatic = 1;
 }
 fclose(f);

 obj1.load(obj1off);
 obj2.load(obj2off);

 init_method(method);

 AABB b1(&obj1);
 AABB b2(&obj2);

 //octree.read("..\\..\\data\\finalprotesis.oct");

 glutInit(&argc, argv);
 
 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
 
 glutInitWindowSize(640, 480); 
 glutInitWindowPosition(20, 20); 
	glutCreateWindow("GLUT"); 
 glutDisplayFunc(Draw); 
 glutIdleFunc(Idle); 
 glutReshapeFunc(ReSizeGLScene); 
 glutIgnoreKeyRepeat(1); 
 glutKeyboardFunc(keyPressed); 
 glutKeyboardUpFunc(keyReleased); 
 glutSpecialFunc(specialKeyPressed);
 glutSpecialUpFunc(specialKeyReleased); 
 glutVisibilityFunc(Icon); 
 glutPassiveMotionFunc(mousepassivemotion);
 glutMotionFunc(mouseactivemotion);
 glutMouseFunc(mouseClick);
 InitGL(width, height);
 
 glutMainLoop();

 return 1;
}
