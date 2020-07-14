#include <stdio.h>

#include "..\..\include\defs.h"
#include "..\..\include\off.h"
#include "..\..\include\aabbtree.h"

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
  AABBTree tree(&off);
  //tree.read("algo.txt");
  tree.write(argv[2]);
  int c = tree.countLeaves();
  int t = tree.getTotal();
  int m = tree.getWorst();
  int a = tree.getAverage();
  printf("Leaves: %d\nTotal triangles: %d\nWorst node: %d\nAverage: %d\n", c, t, m, a);
 }
 else
 {
  printf("file not found: %s\n", argv[1]);
  return 0;
 }

 return 1;
}
