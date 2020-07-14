#ifndef __OFF_H__
#define __OFF_H__

#include <stdlib.h>
#include <windows.h>
#ifndef _NOT_OPENGL_
 #include <.\gl\gl.h>
#endif
//mine
#include "Point3fv.h"
#include "obb_struct.h"

typedef float f3[3];
typedef int i3[3];

class OFF
{
public:
  OFF();
  OFF(char *fname);
  ~OFF();
  bool load(char *fname);
  void release();
  void render();
  void render(int *f, int nf);
  void renderColor(int *f, int nf, float r, float g, float b);
  inline void renderFace(int f);
  void renderWire();
  void renderWire(int f);
  void savePolys(char *fname, int *f, int nf);
  f3 *normal;
  f3 *vertex;
  f3 center;
  f3 size;
  float dist2center;
  f3 max, min;
  i3 *face;
  int nf, nv;
  unsigned int mem();
  //bool displayAABB();
  //this is mine
  	//OBB_struct m_boundingBox;			//object oriented baounding box
	//void calculateVolume();	//calculate a volume of the OBB
	//void setOrientation(CPoint3fv vX, CPoint3fv vY, CPoint3fv vZ);	//set the orientation of the OBB
	//OBB_struct& getOrientation();
};

void OFF::renderFace(int i)
{
#ifndef _NOT_OPENGL_
 glBegin(GL_TRIANGLES);
   glVertex3fv(vertex[face[i][0]]);
   glVertex3fv(vertex[face[i][1]]);
   glVertex3fv(vertex[face[i][2]]);
 glEnd();
#endif
}

#endif