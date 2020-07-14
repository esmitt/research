#ifndef UTIL_FILE
#define UTIL_FILE

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <time.h>

#define BIT_ZERO false
#define BIT_ONE true

#define DELETE_ARRAY(X)if(X)delete[] (X);(X) = NULL
#define ABS(a) ((a)>0)?(a):(-(a))

#define N_ALLELES 4			//x,y,z,w
#define N_DIGIT_PRECISION 1	//digitos de precision

//independientes del problema
#define DEFAULT_MAX_POPULATION 100
#define DEFAULT_MAX_GENERATION 50

#define EPSILON 0.00001
#define MAX(X,Y)((X)>(Y))?(X):(Y)
#define ABS(X)((X)>0)?(X):(-(X))


//variables
#define X_VALUE 0
#define Y_VALUE 1
#define Z_VALUE 2
#define W_VALUE 3
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

typedef unsigned char byte;

#define FLT_MIN -99999.f
#define FLT_MAX 99999.f

#define RAND_A1 40014
#define RAND_M1 2147483563
#define RAND_Q1 53668
#define RAND_R1 12211
#define RAND_A2 40692
#define RAND_M2 2147483399
#define RAND_Q2 52744
#define RAND_R2 3791
#define RAND_SCALE1 (1.0 / RAND_M1)

///
/// Clase que implementa un patron Singleton para los numeros random y otras funciones utiles
///
class CMath
{
protected:
	CMath();
	~CMath();

private:
	static CMath* m_pInstance;
	int seed1, seed2;

public:
	static CMath* getInstance();
	double getRandom();
	int getRandomFromZeroTo(int limit);
};

#endif

