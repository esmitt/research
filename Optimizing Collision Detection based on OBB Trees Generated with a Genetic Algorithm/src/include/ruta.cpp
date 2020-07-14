#include <stdio.h>
#include "ruta.h"

Ruta::Ruta()
{
 n = 0;
 r = NULL;
 endstatus = true;
}

Ruta::Ruta(char *fname)
{
 n = 0;
 r = NULL;
 endstatus = true;
 load(fname);
}

Ruta::~Ruta()
{
 release();
}

void Ruta::release()
{
 if (r) delete [] r;
}

void Ruta::load(char *fname)
{
 release();
 FILE *f = fopen(fname, "rb");
 if (!f)
 {
  n = 0;
  r = NULL;
  return;
 }

 fread(&n, sizeof(int), 1, f);
 r = new f3[n];
 fread(r, sizeof(float)*3, n, f);
 fclose(f); 
}

float Ruta::getTSize(int t)
{
 f3 aux;
 subf3(aux, r[t+1], r[t]);
 return mod(aux);
}

void Ruta::begin(float delta)
{
 endstatus = false; 
 this->delta = delta;
 tramo = 0;
 t = 0.f;
 tsize = getTSize(0);
 copyf3(currentPos, r[0]);
 tDelta = delta/tsize;     // tsize/delta = # de pasos para este tama#o de
}

void Ruta::next()
{
 if (!endstatus)
 {
  t += tDelta;
  if (t>=1.f)
  { // se acabo este tramo!
   float tn = t-1.f; // el nuevo t
  //f3 aux;
   //floatbyf3(aux, tn, tsize); // cuanto me sobra
   //float sobra=mod(aux);
   float sobra = tsize * tn;
   tramo++;
   if (tramo==n-1)  // finalizo la ruta
   {
    endstatus = true;
    return;
   }
   tsize = getTSize(tramo)-sobra;
   tDelta = delta/tsize;
   t = tn;
  }
  //else
  {
   f3 aux1, aux2;
   floatbyf3(aux1, 1.f-t, r[tramo]);
   floatbyf3(aux2, t, r[tramo+1]);
   addf3(currentPos, aux1, aux2);
  }
 }
}

bool Ruta::end()
{
 return endstatus;
}

