#ifndef __AUTOVECTOR_H__
#define __AUTOVECTOR_H__

void tri_diag(float *a,float *d,float *e,float *z,int n,float tole);

int calc_eigenstructure(float *d,float *e,float *z,int n,float macheps);

float *vector(long nl,long nh);

void free_vector(float * v,long nl,long nh);

void jacobi(float **a,int n,float *d,float **v,int *nrot);

#endif