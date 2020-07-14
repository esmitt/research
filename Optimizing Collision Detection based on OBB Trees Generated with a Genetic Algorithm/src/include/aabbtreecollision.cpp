#include "AABBTreeCollision.h"

class OBB
{
public:
 f3 ori;
 f3 axis[3];
 f3 dist;
};

inline void transformVertex(f3 r, float *M1, f3 v)
{
 r[0] = r[1] = r[2] = 0.f;
 for(int i=0; i<3; i++)
 {
  for(int j=0; j<4; j++)
  {
   r[i] += (j==3?1.f:v[j]) * M1[j*4+i];
  }
 }
 /*
 for(int j=0; j<4; j++)
 {
  r[3] += 1.f * M1[j*4+3];
 }*/
}

inline void transformVertex_notranslations(f3 r, float *M1, f3 v)
{
 r[0] = r[1] = r[2] = 0.f;
 for(int i=0; i<3; i++)
 {
  for(int j=0; j<3; j++)
  {
   r[i] += v[j] * M1[j*4+i];
  }
 }
 /*
 for(int j=0; j<4; j++)
 {
  r[3] += 1.f * M1[j*4+3];
 }*/
}

inline void transformBB(AABB &ori,float *M, OBB &dest)
{
 f3 dir[3]={{1.f,0.f,0.f},{0.f,1.f,0.f},{0.f,0.f,1.f}};
 /*
 copyf3(dest.axis[0], dir[0]);
 copyf3(dest.axis[1], dir[1]);
 copyf3(dest.axis[2], dir[2]);
 subf3(dest.dist, ori.max, ori.min);
 copyf3(dest.ori, ori.min);
 addf3(dest.ori, dest.ori, M+12);
 return;
 */
 transformVertex(dest.ori, M, ori.min);
 transformVertex_notranslations(dest.axis[0], M, dir[0]);
 transformVertex_notranslations(dest.axis[1], M, dir[1]);
 transformVertex_notranslations(dest.axis[2], M, dir[2]);
 subf3(dest.dist, ori.max, ori.min);
 normalize(dest.axis[0]);
 normalize(dest.axis[1]);
 normalize(dest.axis[2]);
}

namespace AABBns
{
 float *M1, *M2;
}

inline float project(f3 v, f3 D)
{
 // proyecta el punto v sobre la recta D
 return dot(v, D)/dot(D, D);
}

inline void project(f3 *vl, f3 D, float &m, float &M)
{
 // proyecta los 8 vertices de vl sobre la recta D y deja los bordes en m y M
 m = 1E9f; M = -1E9f;
 for(int i=0; i<8; i++)
 {
  float t = project(vl[i], D);
  if (t > M) M = t;
  if (t < m) m = t;
 }
}

inline bool projectAndTry(f3 D, f3 *vl1, f3 *vl2)
// proyecta los 8 puntos de vl1 ylos 8 de vl2 sobre D
// y verifica si hay un plano separador, en cuyo caso retorna falso
// convertirla en macro cuando funcione!
{
 float max1, max2, min1, min2;  // distancia max. y min. para las proyecciones
 normalize(D);
 project(vl1, D, min1, max1);
 project(vl2, D, min2, max2);
 if (max1 < min2 || max2 < min1) // plano separador encontrado => no hay colision!
  return false;
 return true;
}

inline void findVertex(OBB &bb, f3 *v)
{
 f3 d1, d2, d3;

 floatbyf3(d1, bb.dist[0], bb.axis[0]); // buscar los 
 floatbyf3(d2, bb.dist[1], bb.axis[1]); // ejes escalados
 floatbyf3(d3, bb.dist[2], bb.axis[2]);

 copyf3(v[0], bb.ori);
 addf3(v[1], bb.ori, d1);
 addf3(v[2], v[1], d2);
 addf3(v[3], bb.ori, d2);
 addf3(v[4], v[0], d3);
 addf3(v[5], v[4], d1);
 addf3(v[6], v[5], d2);
 addf3(v[7], v[4], d2);
}

inline bool detect(OBB &bb1, OBB &bb2)
{
 f3 D;

 f3 bb1Vertex[8], bb2Vertex[8];  // contiene los vertices de cada BB

 findVertex(bb1, bb1Vertex);
 findVertex(bb2, bb2Vertex);

 copyf3(D, bb1.axis[0]);  // primer plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 copyf3(D, bb1.axis[1]);  // segundo plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 copyf3(D, bb1.axis[2]);  // 3er plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 copyf3(D, bb2.axis[0]);  // 4to plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 copyf3(D, bb2.axis[1]);  // 5to plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 copyf3(D, bb2.axis[2]);  // 6to plano
 if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
 // faltan otros 9, inter bb!
 for(int i=0; i<3; i++)
 {
  for(int j=0; j<3; j++)
  {
   if (equalf3(bb1.axis[i], bb2.axis[j])) continue;
   cross(D, bb1.axis[i], bb2.axis[j]);
   if (!projectAndTry(D, bb1Vertex, bb2Vertex)) return false;
  }
 } 

 return true;
}

inline bool detect(AABBNode *T1, AABBNode *T2) 
// detecta colisiones entre dos nodos AABB
{
 OBB bb1, bb2;
 transformBB(T1->BB, AABBns::M1, bb1);
 transformBB(T2->BB, AABBns::M2, bb2);

 if (detect(bb1, bb2))
 {
  bool leaf1 = T1->isLeaf();
  bool leaf2 = T2->isLeaf();
  if (leaf1 && leaf2) // ambos son hojas ==> ver triangulo a triangulo
  {
   printf("%d",T1->nt * T2->nt);
   return true;
  }
  if (leaf1 && !leaf2)
  {   // T1 es hoja, T2 no
    OBB bb2_l,bb2_r;
    transformBB(T2->left->BB, AABBns::M2, bb2_l);
    transformBB(T2->right->BB, AABBns::M2, bb2_r);
    if (detect(bb1, bb2_l))
     if (detect(T1, T2->left)) return true;
    if (detect(bb1, bb2_r))
     if (detect(T1, T2->right)) return true;
  } else if (!leaf1 && leaf2)
  {   // T1 no es hoja, T2 si
    OBB bb1_l,bb1_r;
    transformBB(T1->left->BB, AABBns::M1, bb1_l);
    transformBB(T1->right->BB, AABBns::M1, bb1_r);
    if (detect(bb1_l, bb2))
     if (detect(T1->left, T2)) return true;
    if (detect(bb1_r, bb2))
     if (detect(T1->right, T2)) return true;
  } else
  {
   // ni T1 ni T2 son hojas
   OBB bb1_l, bb1_r, bb2_l, bb2_r;
   transformBB(T1->left->BB, AABBns::M1, bb1_l);
   transformBB(T1->right->BB, AABBns::M1, bb1_r);
   transformBB(T2->left->BB, AABBns::M2, bb2_l);
   transformBB(T2->right->BB, AABBns::M2, bb2_r);
   if (detect(bb1_l, bb2_l)) 
    if (detect(T1->left, T2->left)) return true;
   if (detect(bb1_l, bb2_r)) 
    if (detect(T1->left, T2->right)) return true;
   if (detect(bb1_r, bb2_l)) 
    if (detect(T1->right, T2->left)) return true;
   if (detect(bb1_r, bb2_r)) 
    if (detect(T1->right, T2->right)) return true; 
   return false;
  }
 }
 return false;
}

bool AABBTreeCollision::Detect()
{
 AABBns::M1 = M1;
 AABBns::M2 = M2;
 return detect(T1->root, T2->root);
}

AABBTreeCollision::AABBTreeCollision(AABBTree *T1, AABBTree *T2, OFF *off1, OFF *off2, float *M1, float *M2)
{
 this->T1 = T1;
 this->T2 = T2;
 this->off1 = off1;
 this->off2 = off2;
 this->M1 = M1;
 this->M2 = M2;
}

AABBTreeCollision::~AABBTreeCollision()
{
 if (T1) delete T1;
 if (T2) delete T2;
}
