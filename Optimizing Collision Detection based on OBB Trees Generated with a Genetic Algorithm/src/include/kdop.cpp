#include "kdop.h"

KDOP::KDOP(int k, f3* dir)
{
 this->k = k;
 this->dir = dir;
 d = new float[k];
}

void KDOP::build(OFF *obj)
{
 for(int i=0; i<k; i++)  // por cada vector director
 {
  // proyectar todo el objeto sobre el vector
  // buscar el minimo y asignar el d correspondiente
 }
}

void KDOP::build(OFF *obj, int nt, int *t)
{
}

KDOP::~KDOP()
{
 if (d) delete d;
}


