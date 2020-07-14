#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include "defs.h"

inline float area(f3 p, f3 q, f3 r)
{
 f3 aux1,aux2;
 f3 V;
 subf3(aux1,p,q);
 subf3(aux2,p,r);
 cross(V,aux1,aux2);
 return .5f * mod(V);
}

inline float det(f33 M)
{
 return M[0][0]*M[1][1]*M[2][2]-M[0][0]*M[2][1]*M[1][2]+
        M[1][0]*M[2][1]*M[0][2]-M[0][2]*M[1][1]*M[2][0]+
        M[2][0]*M[0][1]*M[1][2]-M[0][1]*M[1][0]*M[2][2];
}


#endif