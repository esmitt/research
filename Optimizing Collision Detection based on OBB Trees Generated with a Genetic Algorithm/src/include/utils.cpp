#include "utils.h"
CMath* CMath::m_pInstance = 0;

CMath::CMath()
{
	seed1 = (int)time(NULL);
	//seed2 = seed1 = 985321;
	seed2 = (int)time(NULL);
}

CMath::~CMath(){}
	
CMath* CMath::getInstance()
{
	if(m_pInstance == 0)
		m_pInstance = new CMath();
	return m_pInstance;
}

double CMath::getRandom()
{
	int k, result;
	k = seed1 / RAND_Q1;
	seed1 = RAND_A1 * (seed1 - k * RAND_Q1) - k * RAND_R1;
	if (seed1 < 0) seed1 += RAND_M1;
	k = seed2 / RAND_Q2;
	seed2 = RAND_A2 * (seed2 - k * RAND_Q2) - k * RAND_R2;
	if (seed2 < 0) seed2 += RAND_M2;
	result = seed1 - seed2;
	if(result < 1) result += RAND_M1 - 1;
	return result * (double) RAND_SCALE1;
}

int CMath::getRandomFromZeroTo(int limit)
{
	return (int)(getRandom() * (limit + 1));
}