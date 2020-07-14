#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <windows.h>
#include <.\gl\gl.h>

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

#endif