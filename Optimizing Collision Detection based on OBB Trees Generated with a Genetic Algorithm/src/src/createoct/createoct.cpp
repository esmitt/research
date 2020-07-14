#include <stdio.h>

#include "..\..\include\defs.h"
#include "..\..\include\off.h"
#include "..\..\include\octree.h"

int main(int args, char **argv)
{
 if (args!=3)
 {
  printf("uso: createabb infile.off outfile.aabb\n");
  return 0;
 }

 OFF off;
 if (off.load(argv[1]))
 {
 // OctTree tree ;
  OctTree tree(&off);
  tree.write(argv[2]);

  //tree.read(argv[2]);
  //tree.write(argv[2]);
  
  int c = tree.countLeaves();
  int t = tree.getTotal();
  int m = tree.getWorst();
  int a = tree.getAverage();
  int s = tree.size();

  printf("Leaves: %d\nTotal triangles: %d\nWorst node: %d\nAverage: %d\nSize: %d\n", c, t, m, a, s);
  
 }
 else
 {
  printf("file not found: %s\n", argv[1]);
  return 0;
 }

 return 1;
}
