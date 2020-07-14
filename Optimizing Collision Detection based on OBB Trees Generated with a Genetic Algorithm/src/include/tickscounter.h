#pragma once

class CTicksCounter
{
public:
  CTicksCounter(void);
  ~CTicksCounter(void);
  void   Start();
  double Stop();

private:

  static LARGE_INTEGER m_lFreq;
  LARGE_INTEGER m_lTime;
  bool m_startCalled;
};