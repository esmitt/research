#include <stdio.h>
#include <windows.h>
#include <.\gl\gl.h>
#include <.\gl\glaux.h>

#include "defs.h"

#include "offext.h"

TextureManager::~TextureManager()
{
 if (objectName)
  delete [] objectName;
}

TextureManager::TextureManager()
{
 nTextures = 0;
 objectName = NULL;
}

int pot2anterior(int x)
// retorna la mayor potencia de 2 menor o igual a x
// Esto es, pot2anterior(x)=r sii 2^r<=x y 2^(r+1)>x
{
 int xc = x;
 int i=0;
 char bit=0;
 while(bit==0)
 {
  bit = x&1;
  x>>=1;
 }
 if (x==0) return xc;
 // x no es 0
 bit = 0;
 for(i=31; !bit; i--)
 {
  bit = (xc&(1<<i))>>i;
 }
 return 1<<(i+1);
}

void TextureManager::loadBMP(char *fname, int texname)
{
 FILE *f = fopen(fname, "r");
 if (f)
 {
  fclose(f);
  AUX_RGBImageRec *img = auxDIBImageLoad(fname);
  glBindTexture(GL_TEXTURE_2D, texname);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  int newsx = pot2anterior(img->sizeX);
  int newsy = pot2anterior(img->sizeY);
  unsigned char *data = img->data;
  if (newsx!=img->sizeX || newsy!=img->sizeY)
  {
   unsigned char *newdata = (unsigned char *) malloc(newsx*newsy*3); // asumo rgb
   gluScaleImage(GL_RGB, img->sizeX, img->sizeY, GL_UNSIGNED_BYTE, data, newsx, newsy, GL_UNSIGNED_BYTE, newdata);
   free(data);
   data = newdata;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newsx, newsy, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  free(data);
 } 
}

int TextureManager::addTexture(char *fname)
{
 int index = filename[string(fname)];
 if (index)
 {
  // el archivo ya esta cargado!  
 }
 else
 {
  unsigned int textname;
  glGenTextures(1,&textname);
  index = textname;
  filename[string(fname)] = textname;
  objectNameList.push_back(textname);
  int dotpos = string(fname).rfind('.');
  if (dotpos>0)
  {
   char *p =fname+dotpos+1;
   if (!strcmpi(p,"BMP"))
   {
    loadBMP(fname,textname);
   }
   else
   {
    // extension desconocida
    return -1;
   }
  }
  else
  {
   // extension desconocida! (sin extension)
   return -1;
  }  
 }
 return index;
}

void OFFExt::render_color_no_light()
{
 for(int i=0; i<np; i++)
 {
  glColor3fv(surfaces[surf_index[i]].color);
  glBegin(GL_TRIANGLES);
   glVertex3fv(vertex[poly[i][0]]);
   glVertex3fv(vertex[poly[i][1]]);
   glVertex3fv(vertex[poly[i][2]]);
  glEnd();
 }
}

void OFFExt::render_color_no_texture()
{
 for(int i=0; i<np; i++)
 {
  surface *s = &surfaces[surf_index[i]];
  float Amb[4]={0.f, 0.f, 0.f, 1.f};
  float Diff[4] = {s->color[0]*s->diff, s->color[1]*s->diff,s->color[2]*s->diff, 1.f};
  float Spec[4] = {s->color[0]*s->spec, s->color[1]*s->spec,s->color[2]*s->spec, 1.f};
  float Exp = 100.f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spec);
  glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, Exp);
  //glColor3fv(surfaces[surf_index[i]].color);
  glBegin(GL_TRIANGLES);
   glNormal3fv(normal[poly[i][0]]);
   glVertex3fv(vertex[poly[i][0]]);
   glNormal3fv(normal[poly[i][1]]);
   glVertex3fv(vertex[poly[i][1]]);
   glNormal3fv(normal[poly[i][2]]);
   glVertex3fv(vertex[poly[i][2]]);
  glEnd();
 }
}

void OFFExt::render_color()
{
 for(int i=0; i<np; i++)
 {
  surface *s = &surfaces[surf_index[i]];
  float Amb[4]={0.f, 0.f, 0.f, 1.f};
  float Diff[4] = {s->color[0]*s->diff, s->color[1]*s->diff,s->color[2]*s->diff, 1.f};
  float Spec[4] = {s->color[0]*s->spec, s->color[1]*s->spec,s->color[2]*s->spec, 1.f};
  float Exp = 100.f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Spec);
  glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, Exp);
  //glColor3fv(surfaces[surf_index[i]].color);
  if(s->map>-1)
  {
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, textNames[s->map]);
   //int r=glIsTexture(textNames[s->map]);
   glBegin(GL_TRIANGLES);
    glTexCoord2f(1.f-uv_vert[poly[i][0]][0],uv_vert[poly[i][0]][1]);
    glNormal3fv(normal[poly[i][0]]);
    glVertex3fv(vertex[poly[i][0]]);
    glTexCoord2f(1.f-uv_vert[poly[i][1]][0],uv_vert[poly[i][1]][1]);
    glNormal3fv(normal[poly[i][1]]);
    glVertex3fv(vertex[poly[i][1]]);
    glTexCoord2f(1.f-uv_vert[poly[i][2]][0],uv_vert[poly[i][2]][1]);
    glNormal3fv(normal[poly[i][2]]);
    glVertex3fv(vertex[poly[i][2]]);
   glEnd();
  }
  else
  {
   glDisable(GL_TEXTURE_2D);
   glBegin(GL_TRIANGLES);
    glNormal3fv(normal[poly[i][0]]);
    glVertex3fv(vertex[poly[i][0]]);
    glNormal3fv(normal[poly[i][1]]);
    glVertex3fv(vertex[poly[i][1]]);
    glNormal3fv(normal[poly[i][2]]);
    glVertex3fv(vertex[poly[i][2]]);
   glEnd();
  }
 }
}

OFFExt::~OFFExt()
{
 release();
}

void OFFExt::release()
{
 if (normal) delete [] normal;
 if (vertex) delete [] vertex;
 if (poly) delete [] poly;
 if (uv_vert) delete [] uv_vert;
 if (surf_index) delete [] surf_index;
 if (surfaces) delete [] surfaces;
 if (maps)
 {
  for(int i=0; i<nt; i++)
   delete [] maps[i];
  delete [] maps;
 }
 if (TM) delete TM;
 if (textNames) delete [] textNames;
}

OFFExt::OFFExt()
{
 init();
}

OFFExt::OFFExt(char *fname)
{
 init();
 load(fname);
}

void OFFExt::init()
{
 np = ns = nt = nv = 0;
 normal = NULL;
 vertex = NULL;
 poly = NULL;
 uv_vert = NULL;
 surf_index = NULL;
 surfaces = NULL;
 maps = NULL;
 TM = NULL;
 textNames = NULL;
}

#define STOP {fclose(f); return false;}

bool OFFExt::load(char *fname)
{
 FILE *f = fopen(fname, "rt");
 if (!f) return false;
 char header[80];
 fscanf(f,"%s",header);
 if (strcmpi(header,"EXTOFF")) STOP
 fscanf(f,"%d %d %d %d", &nv, &np, &ns, &nt);
 vertex = new f3[nv];
 normal = new f3[nv];
 //f3 *normal_poly = new f3[np];
 surf_index = new int[np];
 poly = new i3[np];
 surfaces = new surface[ns];
 maps = new char*[nt];
 if (nt>0)
 {
  uv_vert = new f2[nv];
  TM = new TextureManager();
 }
 else
  uv_vert = NULL;

 f3 max = {-1E10, -1E10, -1E10}, min = {1E10, 1E10, 1E10};


 for(int i=0; i<nv; i++)
 {
  char uv[10];
  normal[i][0] = normal[i][1] = normal[i][2] = 0.f;
  fscanf(f,"%f %f %f %s", vertex[i], vertex[i]+1, vertex[i]+2, uv);

  if (vertex[i][0] > max[0]) max[0] = vertex[i][0];
  if (vertex[i][1] > max[1]) max[1] = vertex[i][1];
  if (vertex[i][2] > max[2]) max[2] = vertex[i][2];

  if (vertex[i][0] < min[0]) min[0] = vertex[i][0];
  if (vertex[i][1] < min[1]) min[1] = vertex[i][1];
  if (vertex[i][2] < min[2]) min[2] = vertex[i][2];

  if (nt>0 && uv[0]=='U')
  {
   // tiene coord. de textura!
   fscanf(f,"%f %f",uv_vert[i],uv_vert[i]+1);
  }
 }

 center[0] = (max[0] + min[0]) / 2.f;
 center[1] = (max[1] + min[1]) / 2.f;
 center[2] = (max[2] + min[2]) / 2.f;

 length = __max(max[0]-min[0], __max(max[1]-min[1], max[2]-min[2]));

 minmax[0][0] = min[0];
 minmax[0][1] = min[1];
 minmax[0][2] = min[2];
 minmax[1][0] = max[0];
 minmax[1][1] = max[1];
 minmax[1][2] = max[2];

 radius = 0.f;
 for(i=0; i<nv; i++)
 {
  f3 d;
  subf3(d, vertex[i], center);
  float dist = mod(d);
  if (dist>radius) radius = dist;
 }

 for(i=0; i<np; i++)
 {
  char uv[10];
  int nv;
  fscanf(f,"%d",&nv);
  if (nv!=3) STOP
  fscanf(f,"%d %d %d %s", poly[i], poly[i]+1, poly[i]+2, uv);

  //if(i==208)
  // i=i;
  // OJO: en el futuro hacer algo con uv... por ahora nada :)
  fscanf(f,"%d", surf_index+i);

  float *p1 = vertex[poly[i][0]];
  float *p2 = vertex[poly[i][1]];
  float *p3 = vertex[poly[i][2]];
  f3 v1, v2, n;
  subf3(v1, p2, p1);
  subf3(v2, p3, p2);
  cross(n, v1, v2);
  addf3(normal[poly[i][0]], normal[poly[i][0]], n);
  addf3(normal[poly[i][1]], normal[poly[i][1]], n);
  addf3(normal[poly[i][2]], normal[poly[i][2]], n);
 }

 for(i=0; i<nv; i++)
  normalize(normal[i]);

 for(i=0; i<ns; i++)
 {
  char map[10];
  fscanf(f, "%f %f %f %f %f %s",&surfaces[i].color[0], &surfaces[i].color[1], &surfaces[i].color[2],
                                &surfaces[i].diff, &surfaces[i].spec, map);
  if (map[0]=='M')
  {
   fscanf(f, "%d", &surfaces[i].map);
  }
  else
   surfaces[i].map = -1;
 }

 if(nt)
 {
  char trash[10];
  fgets(trash, 200, f);
  textNames = new int[nt];
 }

 for(i=0; i<nt; i++)
 {
  char mapname[200];
  fgets(mapname, 200, f);
  int l=strlen(mapname)-1;
  mapname[l] = '\0';
  maps[i] = new char[l+1];
  strcpy(maps[i], mapname);
  textNames[i] = TM->addTexture(mapname);
 }

 //delete [] normal_poly;
 fclose(f);

 center[2] *= -1.f; // ojo: para viewfrustum

 return true;
}
