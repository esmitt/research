#ifndef __DEFS_H__
#define __DEFS_H__

#include <math.h>

#define assertbytesread(a,b) if (a!=b) return false;

typedef float f3[3];
typedef float f33[3][3];

inline void subf3(f3 r, f3 a, f3 b);
inline void addf3(f3 r, f3 a, f3 b);
inline void floatbyf3(f3 r, float a, f3 b);
inline void cross(f3 V, f3 a, f3 b);
inline float mod(f3 v);
inline void normalize(f3 v);
inline float dot(f3 a, f3 b);
inline void copyf3(f3 a, f3 b);
inline float distf3(f3 a, f3 b);

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

inline void copyf3(f3 a, f3 b)
{
 a[0] = b[0];
 a[1] = b[1];
 a[2] = b[2];
}

inline float distf3(f3 a, f3 b)
{
 f3 aux;
 subf3(aux, a, b);
 return mod(aux);
}

#define tol 1E-5f
inline bool equalf3(f3 a, f3 b)
{
 return ((float)fabs(a[0]-b[0])<tol) &&
        ((float)fabs(a[1]-b[1])<tol) &&
        ((float)fabs(a[2]-b[2])<tol);
}

#endif