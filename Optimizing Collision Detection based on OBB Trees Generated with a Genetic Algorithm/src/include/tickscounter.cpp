#include "stdafx.h"
#include "TicksCounter.h"


LARGE_INTEGER CTicksCounter::m_lFreq = LARGE_INTEGER();

CTicksCounter::CTicksCounter(void)
{
  m_startCalled = false;  
  if (CTicksCounter::m_lFreq.QuadPart==0)
    QueryPerformanceFrequency(&CTicksCounter::m_lFreq);
}

double CTicksCounter::Stop()
{
  LARGE_INTEGER lTime2;
  QueryPerformanceCounter(&lTime2);
  long long diffTicks = lTime2.QuadPart - m_lTime.QuadPart;//ticks passed
  return ((double)diffTicks/(double)m_lFreq.QuadPart);//secs passed
}

CTicksCounter::~CTicksCounter(void)
{
}

void CTicksCounter::Start()
{
  QueryPerformanceCounter(&m_lTime);
  m_startCalled = true;
}
