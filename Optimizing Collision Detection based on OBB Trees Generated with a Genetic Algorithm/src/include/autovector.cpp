#include "autovector.h"

#include <math.h>
#include <stdlib.h>

void tri_diag(float *a,float *d,float *e,float *z,int n,float tol)
{
register int i,j,k,l;
float f;
float g;
float h;
float hh;

	for( i = 1; i <= n; i++ )
	{
		for( j = 1; j <= i; j++ )
		{
			z[(i-1)*n+j] = a[(i-1)*n+j];
		};
	};

	for( i = n; i >= 2; i--)
	{
		l = i-2;
		f = z[(i-1)*n + i-1];

		g = 0.;
		for( k = 1; k <= l; k++ )
		{
			g += z[(i-1)*n+k]*z[(i-1)*n+k];
		};
		h = g + f*f;

		if( g <= tol )
		{
			e[i] = f;
			h = 0.;
			goto skip;
		};

		l++;
		if( f >= 0. )
		{
			g = e[i] = -(float)sqrt(h);
		}
		else
		{
			g = e[i] = (float)sqrt(h);
		};
		h = h - f*g;
		z[(i-1)*n+i-1] = f-g;
		f = 0.;
		for( j = 1; j <= l; j++ )
		{
			z[(j-1)*n+i] = z[(i-1)*n+j]/h;
			g = 0.;
			for( k = 1; k <= j; k++)
			{
				g += z[(j-1)*n+k]*z[(i-1)*n+k];	
			};
			for( k = j+1; k <= l; k++ )
			{
				g += z[(k-1)*n+j]*z[(i-1)*n+k];					
			};
			e[j] = g/h;
			f += g*z[(j-1)*n+i];
		};
		hh = f/(h+h);
		for( j = 1; j <= l; j++ )
		{
			f = z[(i-1)*n+j];
			g = e[j] - hh*f;
			e[j] = g;
			for( k = 1; k <= j; k++ )
			{
				z[(j-1)*n+k] = z[(j-1)*n+k] - f*e[k] - g*z[(i-1)*n+k];
			};
		};
skip:		d[i] = h;
	};

	d[1] = e[1] = 0.;
	for( i = 1; i <=n; i++ )
	{
		l = i - 1;
		if( d[i] != 0. )
		{
			for( j = 1; j <= l; j++ )
			{
				g = 0.;
				for( k = 1; k <= l; k++ )
				{
					g += z[(i-1)*n+k]*z[(k-1)*n+j];
				};
				for( k = 1; k <= l; k++ )
				{
					z[(k-1)*n+j] = z[(k-1)*n+j] - g*z[(k-1)*n+i];
				};
			};
		};
		d[i] = z[(i-1)*n+i];
		z[(i-1)*n+i] = 1.;
		for( j = 1; j <= l; j++ )
		{
			z[(i-1)*n+j] = z[(j-1)*n+i] = 0.;
		};
	};
};

/**********************************************************************
    routine to find eigenstructure of real tri-diagonal matrix
    			 uses QL algorithm
          returns  0: sucess      -1: failure to converge
  NOTE: code was taken from an old ALGOL program and contains goto's
**********************************************************************/
int calc_eigenstructure(float *d,float *e,float *z,int n,float macheps)
{
register int i,j,k,l,m;
float b,c,f,g,h,p,r,s;

	for( i = 2; i <=n; i++ )
	{
		e[i-1] = e[i];
	};
	e[n] = b = f = 0.;
	for( l = 1; l <= n; l++ )
	{
		j = 0;
		h = macheps*( (float)fabs(d[l]) + (float)fabs(e[l]) );
		if( b < h ) b = h;
		for( m = l; m <= n; m++ )
		{
			if( fabs(e[m]) <= b ) goto cont;
		};
cont:		if( m == l ) goto root;
nextit:		if( j == 30 ) return(-1);
		j++;
		p = (d[l+1] - d[l])/(2.f*e[l]);
		r = (float)sqrt(p*p+1);
		if( p < 0. )
		{
			h = d[l] - e[l]/(p-r);
		}
		else
		{
			h = d[l] - e[l]/(p+r);
		};
		for( i = l; i <= n; i++ )
		{
			d[i] = d[i] - h;
		};
		f += h;
		p = d[m];
		c = 1.;
		s = 0.;
		for( i = m-1; i >= l; i-- )
		{
			g = c*e[i];
			h = c*p;
			if( fabs(p) >= fabs(e[i]) )
			{
				c = e[i]/p;
				r = (float)sqrt(c*c+1);
				e[i+1] = s*p*r;
				s = c/r;
				c = 1.f/r;
			}
			else
			{
				c = p/e[i];
				r = (float)sqrt(c*c+1);
				e[i+1] = s*e[i]*r;
				s = 1.f/r;
				c = c/r;
			};
			p = c*d[i] - s*g;
			d[i+1] = h + s*(c*g + s*d[i]);
			for( k = 1; k <= n; k++ )
			{
				h = z[(k-1)*n+i+1];
				z[(k-1)*n+i+1] = s*z[(k-1)*n+i] + c*h;
				z[(k-1)*n+i] = c*z[(k-1)*n+i] - s*h;
			};
		};
		e[l] = s*p;
		d[l] = c*p;
		if( fabs(e[l]) > b ) goto nextit;
root:		d[l] = d[l] + f;
	};
	
/* order the eigenvectors  */
	for( i = 1; i <= n; i++ )
	{
		k = i;
		p = d[i];
		for( j = i+1; j <= n; j++ )
		{
			if( d[j] < p ) 
			{
				k = j;
				p = d[j];
			};
		};
		if( k != i )
		{
			d[k] = d[i];
			d[i] = p;
			for( j = 1; j <= n; j++ )
			{
				p = z[(j-1)*n+i];
				z[(j-1)*n+i] = z[(j-1)*n+k];
				z[(j-1)*n+k] = p;
			};
		};
	};
	return(0);
};


//////////////////////////////////////////////////////////////////////////////////////////
#define NR_END 1
#define FREE_ARG char*

float *vector(long nl,long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((unsigned int) ((nh-nl+1+NR_END)*sizeof(float)));
	//if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(float * v,long nl,long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
	a[k][l]=h+s*(g-h*tau);

void jacobi(float **a,int n,float *d,float **v,int *nrot)
//float **a,**v,d[];
//int *nrot,n;
{
	int j,iq,ip,i;
	float tresh,theta,tau,t,sm,s,h,g,c,*b,*z;

	b=vector(1,n);
	z=vector(1,n);
	for (ip=1;ip<=n;ip++) {
		for (iq=1;iq<=n;iq++) v[ip][iq]=0.0;
		v[ip][ip]=1.0;
	}
	for (ip=1;ip<=n;ip++) {
		b[ip]=d[ip]=a[ip][ip];
		z[ip]=0.0;
	}
	*nrot=0;
	for (i=1;i<=50;i++) {
		sm=0.0;
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++)
				sm += (float)fabs(a[ip][iq]);
		}
		if (sm == 0.0) {
			free_vector(z,1,n);
			free_vector(b,1,n);
			return;
		}
		if (i < 4)
			tresh=0.2f*sm/(n*n);
		else
			tresh=0.0;
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++) {
				g=(float)(100.0*fabs(a[ip][iq]));
				if (i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
					&& (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
					a[ip][iq]=0.0;
				else if (fabs(a[ip][iq]) > tresh) {
					h=d[iq]-d[ip];
					if ((float)(fabs(h)+g) == (float)fabs(h))
						t=(a[ip][iq])/h;
					else {
						theta=0.5f*h/(a[ip][iq]);
						t=1.0f/(float)(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) t = -t;
					}
					c=(float)(1.0/sqrt(1+t*t));
					s=t*c;
					tau=s/(1.0f+c);
					h=t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq]=0.0;
					for (j=1;j<=ip-1;j++) {
						ROTATE(a,j,ip,j,iq)
					}
					for (j=ip+1;j<=iq-1;j++) {
						ROTATE(a,ip,j,j,iq)
					}
					for (j=iq+1;j<=n;j++) {
						ROTATE(a,ip,j,iq,j)
					}
					for (j=1;j<=n;j++) {
						ROTATE(v,j,ip,j,iq)
					}
					++(*nrot);
				}
			}
		}
		for (ip=1;ip<=n;ip++) {
			b[ip] += z[ip];
			d[ip]=b[ip];
			z[ip]=0.0;
		}
	}
	//nrerror("Too many iterations in routine jacobi");
}
#undef ROTATE

