typedef float f3[3];
typedef float f2[3];
typedef int i3[3];

#pragma warning(disable:4786)

#include <map>
#include <list>

#include <string>

using namespace std;

class TextureManager
{
 struct less_str
 {
  bool operator () (string a,string b) {return a<b;}
 };

public:
 map <string,int/*,less_str*/> filename;
 list <int> objectNameList;
 int nTextures;
 int *objectName;

 TextureManager();
 ~TextureManager();
 int addTexture(char *fname);
 void loadBMP(char *fname, int texname);
};

class OFFExt
{
 struct surface
 {
  f3 color;
  float diff;
  float spec;
  int map;
 };

 /*
 Asumo que todos son triangulos!
 */
public:

 int nv;  // numero de vertices
 int np;  // numero de poligonos
 int ns;  // numero de supeficies
 int nt;  // numero de triangulos

 i3  *poly;
 f3  *vertex;
 f3  *normal;
 f2  *uv_vert;
 int *surf_index;
 surface *surfaces;
 char **maps;
 int *textNames;

 TextureManager *TM;

 void init();
 void release();

 
 f3 center;
 float length;
 float radius;
 f3 minmax[2];
 
 OFFExt();
 OFFExt(char *fname);
 ~OFFExt();
 bool load(char *fname);
 
 void render_color_no_light();
 void render_color_no_texture();
 void render_color();
 
};
