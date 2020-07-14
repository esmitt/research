// names of header files may differ
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include "TRandomGenerator.h"

double TRandomGenerator::Random() {
  // returns a random number between 0 and 1.
  uint32 a, b;
  // generate next number
  b = _lrotr(randbuffer[p1][0], R1) + randbuffer[p2][0];
  a = _lrotr(randbuffer[p1][1], R2) + randbuffer[p2][1];
  randbuffer[p1][0] = a; randbuffer[p1][1] = b;
  // rotate list pointers
  if (--p1 < 0) p1 = KK - 1;
  if (--p2 < 0) p2 = KK - 1;
#ifdef SAFE
  // perform self-test
  if (randbuffer[p1][0] == randbufcopy[0][0] &&
    memcmp(randbuffer, randbufcopy[KK-p1], 2*KK*sizeof(uint32)) == 0) {
      // self-test failed
      if ((p2 + KK - p1) % KK != JJ) {
        // note: the way of printing error messages depends on system
        printf("Random number generator not initialized");}
      else {
        printf("Random number generator returned to initial state");}
      abort();}
#endif
  // convert to float
  randbits[0] = a;
#if 8 == 10
  randbits[1] = b | 0x80000000;              // 80 bits floats = 63 bits resolution
#elif 8 == 8
  randbits[1] = b & 0x000FFFFF | 0x3FF00000; // 64 bits floats = 52 bits resolution
#else
#error incompatible floating point format
#endif
  // normalize floating point number
  return randp1 - 1.;}

void TRandomGenerator::RandomInit (double seed) {
  // this function initializes the random number generator.
  // must be called before the first call to Random!
  int i, j; long double s;
  // get seed into an acceptable range
  if (seed < 0) seed = 0.147328926107 - seed;
  while (seed >= 1) seed *= 0.0817835148503;
  s = seed;
  // make random numbers and put them into the buffer
  for (i=0; i<KK; i++) {
    for (j=0; j<2; j++) {
      s = (sqrtl(s) + 0.147328926107) * 46.1063140045;
      s = s - floorl(s);
      *(double*)&randbuffer[i][j] = s;}}
  // set exponent of randp1 
  randp1 = 1.5;
  // initialize pointers to circular buffer
  p1 = 0;  p2 = JJ;
#ifdef SAFE
  memcpy (randbufcopy, randbuffer, 2*KK*sizeof(uint32));
  memcpy (randbufcopy[KK], randbuffer, 2*KK*sizeof(uint32));
#endif
}
