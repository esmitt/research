#include <stdio.h>

#include "..\..\include\defs.h"
#include "..\..\include\off.h"
#include "..\..\include\kdop.h"

f3 d[14]={
          {1.f, 0.f, 0.f},
          {-1.f, 0.f, 0.f},
          {0.f, 1.f, 0.f},
          {0.f, -1.f, 0.f},
          {0.f, 0.f, 1.f},
          {0.f, 0.f, -1.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f}
         };


int main(int args, char **argv)
{
 if (args!=3)
 {
  printf("uso: createabb infile.off outfile.aabb\n");
  return 0;
 }

 addf3(d[6], d[0], d[2]);
 addf3(d[6], d[6], d[4]);
 normalize(d[6]);
 floatbyf3(d[7], -1.f, d[6]);

 addf3(d[8], d[0], d[2]);
 addf3(d[8], d[8], d[5]);
 normalize(d[8]);
 floatbyf3(d[9], -1.f, d[8]);

 addf3(d[10], d[1], d[2]);
 addf3(d[10], d[10], d[4]);
 normalize(d[10]);
 floatbyf3(d[11], -1.f, d[10]);

 addf3(d[12], d[1], d[2]);
 addf3(d[12], d[12], d[5]);
 normalize(d[12]);
 floatbyf3(d[13], -1.f, d[12]);

 OFF off;
 if (off.load(argv[1]))
 {
  KDOP kdop(14, d);
  kdop.build(&off);
  /*
  AABBTree tree(&off);
  tree.write(argv[2]);
  int c = tree.countLeaves();
  int t = tree.getTotal();
  int m = tree.getWorst();
  int a = tree.getAverage();
  printf("Leaves: %d\nTotal triangles: %d\nWorst node: %d\nAverage: %d\n", c, t, m, a);
  */
 }
 else
 {
  printf("file not found: %s\n", argv[1]);
  return 0;
 }

 return 1;
}
