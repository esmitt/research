#include "aabb.h"

AABB::AABB(f3 min, f3 max)
{
 copyf3(this->max, max);
 copyf3(this->min, min);
}

AABB::AABB()
{
 min[0] = min[1] = min[2] = 
 max[0] = max[1] = max[2] = 0.f;
}

AABB::AABB(OFF *obj)
{
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
}

AABB::AABB(OFF *obj, int n, int *tri)
{
 max[0] = max[1] = max[2] = -1E9;
 min[0] = min[1] = min[2] =  1E9;
 for(int i=0; i<n; i++)
 {
  for(int j=0; j<3; j++)
  {
   if (max[0] < obj->vertex[obj->face[tri[i]][j]][0]) max[0] = obj->vertex[obj->face[tri[i]][j]][0];
   if (max[1] < obj->vertex[obj->face[tri[i]][j]][1]) max[1] = obj->vertex[obj->face[tri[i]][j]][1];
   if (max[2] < obj->vertex[obj->face[tri[i]][j]][2]) max[2] = obj->vertex[obj->face[tri[i]][j]][2];

   if (min[0] > obj->vertex[obj->face[tri[i]][j]][0]) min[0] = obj->vertex[obj->face[tri[i]][j]][0];
   if (min[1] > obj->vertex[obj->face[tri[i]][j]][1]) min[1] = obj->vertex[obj->face[tri[i]][j]][1];
   if (min[2] > obj->vertex[obj->face[tri[i]][j]][2]) min[2] = obj->vertex[obj->face[tri[i]][j]][2];
  }
 }
}

void AABB::init(f3 min, f3 max)
{
 copyf3(this->max, max);
 copyf3(this->min, min);
}

void AABB::init(OFF *obj)
{
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
}

void AABB::init(OFF *obj, int n, int *tri)
{
 max[0] = max[1] = max[2] = -1E9;
 min[0] = min[1] = min[2] =  1E9;
 for(int i=0; i<n; i++)
 {
  for(int j=0; j<3; j++)
  {
   if (max[0] < obj->vertex[obj->face[tri[i]][j]][0]) max[0] = obj->vertex[obj->face[tri[i]][j]][0];
   if (max[1] < obj->vertex[obj->face[tri[i]][j]][1]) max[1] = obj->vertex[obj->face[tri[i]][j]][1];
   if (max[2] < obj->vertex[obj->face[tri[i]][j]][2]) max[2] = obj->vertex[obj->face[tri[i]][j]][2];

   if (min[0] > obj->vertex[obj->face[tri[i]][j]][0]) min[0] = obj->vertex[obj->face[tri[i]][j]][0];
   if (min[1] > obj->vertex[obj->face[tri[i]][j]][1]) min[1] = obj->vertex[obj->face[tri[i]][j]][1];
   if (min[2] > obj->vertex[obj->face[tri[i]][j]][2]) min[2] = obj->vertex[obj->face[tri[i]][j]][2];
  }
 }
}

int AABB::operator = (AABB &otro)
{
 copyf3(max, otro.max);
 copyf3(min, otro.min);
 return 1;
}

void AABB::getSize(f3 s)
{
 s[0] = max[0] - min[0];
 s[1] = max[1] - min[1];
 s[2] = max[2] - min[2];
}

void AABB::getCenter(f3 s)
{
 s[0] = (max[0] + min[0]) / 2.f;
 s[1] = (max[1] + min[1]) / 2.f;
 s[2] = (max[2] + min[2]) / 2.f;
}

bool AABB::inside(OFF *obj, i3 t)
{
 return inside(obj->vertex[t[0]]) && inside(obj->vertex[t[1]]) && inside(obj->vertex[t[2]]);
}

int AABB::countVertex(OFF *obj, i3 t)
{
 int r = 0;
 for(int i=0; i<3; i++)
  if (inside(obj->vertex[t[i]])) r++;
 return r;
}

bool AABB::inside(f3 v)
{
 return v[0] <= max[0] && v[0] >= min[0] &&
        v[1] <= max[1] && v[1] >= min[1] &&
        v[2] <= max[2] && v[2] >= min[2];
}

void AABB::write(FILE *f)
{
 fwrite(min, sizeof(float), 3, f);
 fwrite(max, sizeof(float), 3, f);
}

bool AABB::read(FILE *f)
{
 int r;
 r=fread(min, sizeof(float), 3, f);
 assertbytesread(r,3)
 r=fread(max, sizeof(float), 3, f);
 assertbytesread(r,3)
 return true;
}

void AABB::split(int axis, AABB *l, AABB *r, float t)
{
 float c = min[axis] + (max[axis]-min[axis])*t;
 *l = *this;
 *r = *this;

 l->max[axis] = c;
 r->min[axis] = c;
}

int AABB::size()
{
 return sizeof(*this);
}

void AABB::render()
{
#ifndef _NOT_OPENGL_
 glBegin(GL_LINE_LOOP);
  glVertex3f(min[0], min[1], min[2]);
  glVertex3f(min[0], max[1], min[2]);
  glVertex3f(max[0], max[1], min[2]);
  glVertex3f(max[0], min[1], min[2]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3f(min[0], min[1], max[2]);
  glVertex3f(min[0], max[1], max[2]);
  glVertex3f(max[0], max[1], max[2]);
  glVertex3f(max[0], min[1], max[2]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3f(min[0], min[1], min[2]);
  glVertex3f(min[0], max[1], min[2]);
  glVertex3f(min[0], max[1], max[2]);
  glVertex3f(min[0], min[1], max[2]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3f(max[0], min[1], min[2]);
  glVertex3f(max[0], max[1], min[2]);
  glVertex3f(max[0], max[1], max[2]);
  glVertex3f(max[0], min[1], max[2]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3f(min[0], min[1], min[2]);
  glVertex3f(min[0], max[1], min[2]);
  glVertex3f(max[0], max[1], min[2]);
  glVertex3f(max[0], min[1], min[2]);
 glEnd();

 glBegin(GL_LINE_LOOP);
  glVertex3f(min[0], min[1], max[2]);
  glVertex3f(min[0], max[1], max[2]);
  glVertex3f(max[0], max[1], max[2]);
  glVertex3f(max[0], min[1], max[2]);
 glEnd();

#endif
}
