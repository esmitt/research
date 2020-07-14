#ifndef __KDOP_H__
#define __KDOP_H__

#include "defs.h"
#include "off.h"

class KDOP
{
public:
 int k;
 f3 *dir;   // vectores directores
 float *d;  // D de la ecuacion del plano para cada plano
 KDOP(int k, f3* dir);
 ~KDOP();
 void build(OFF *obj);
 void build(OFF *obj, int nt, int *t);
};

#endif