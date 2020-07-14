// define parameters
#ifndef __TRANDOMGENERATOR__
#define __TRANDOMGENERATOR__

#define KK 17
#define JJ 10
#define R1 13
#define R2  5
#define SAFE         // remove this line if you don't want self-test

typedef unsigned long uint32;       // use 32 bit integers

class TRandomGenerator {            // encapsulate random number generator
  public:
  void RandomInit(double seed);     // initialization
  double Random();                  // random function
  TRandomGenerator(double seed=0) { // constructor
    RandomInit(seed);} 
  protected:
  union {                           // used for conversion to float
    long double randp1;
    uint32 randbits[3];};
  int p1, p2;                       // indexes into buffer
  uint32 randbuffer[KK][2];         // history buffer
#ifdef SAFE
  uint32 randbufcopy[KK*2][2];      // used for self-test
#else
  uint32 scratch;                   // receives overrun from randbuffer
#endif
};

#endif