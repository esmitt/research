#include <stdio.h>

#include "..\..\include\defs.h"
#include "..\..\include\off.h"
#include "..\..\include\obb.h"
#include "..\..\include\obbnode.h"
#include "..\..\include\obbtree.h"
#include "..\..\include\tickscounter.h"

// Settings: ..\..\data\finalprotesis.off protesis.obb
#include <string>
#include <iostream>
using namespace std;
int main(int args, char **argv)
{
 
/*
	argv[1] = "FinalProtesis.off";
 argv[2] = "FinalProtesis.obb";
 */
 
	//argv[1] = "bunny.off";
	//argv[2] = "bunny.obb";
	//argv[1] = "sphere.off";
	//argv[2] = "sphere.obb";
/*	
	argv[1] = "sphere.off";
	if(USE_GENETIC)
		argv[2] = "Genesphere.obb";
	else
		argv[2] = "MCsphere.obb";
*/		
 /*
 if (args!=3)
 {
  printf("uso: createabb infile.off outfile.aabb\n");
  return 0;
 }
 */
 OFF off;
 if (off.load(argv[1]))
 {
	CTicksCounter mtime;
	mtime.Start();
	 OBBTree tree(&off);
	 cout << mtime.Stop() << endl;
  //OBBTree tree;

  tree.write(argv[2]);

  //tree.read("tibia.obb");

  std::string filename;
  filename.append(argv[2]);
  int c = tree.countLeaves();
  int t = tree.getTotal();
  int m = tree.getWorst();
  int a = tree.getAverage();
  //printf("Leaves: %d\nTotal triangles: %d\nWorst node: %d\nAverage: %d\n", c, t, m, a);
  //printf("Volumen = %f\n",tree.GetVolumeValue());
 }
 else
 {
  printf("file not found: %s\n", argv[1]);
  return 0;
 }

 return 1;
}
