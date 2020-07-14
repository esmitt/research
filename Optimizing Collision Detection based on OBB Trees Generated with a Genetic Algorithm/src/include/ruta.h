#ifndef __RUTA_H__
#define __RUTA_H__

#include "defs.h"

class Ruta
{
 int n; // numero de nodos
 f3 *r;
 int tramo;   // en que tramo estoy (son n-1 tramos)
 float t;     // el t dentro de este tramo
 float tDelta; // desplazamiento de t para este tramo
 float tsize; // tama#o del tramo
 float getTSize(int t); // retorna el tama#o de un tramo
 bool endstatus;
public:
 
 f3 currentPos;

 Ruta();
 Ruta(char *fname);
 ~Ruta();

 void release();
 void load(char *fname);

 void begin(float delta=0.1f);
 float delta;
 void next();
 bool end();
};

#endif