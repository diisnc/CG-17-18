#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;

#define _USE_MATH_DEFINES
#define SPACE 100

struct Scale {
	bool vazio = true;
	float x, y, z;
};

struct Rotation {
	bool vazio = true;
	float x, y, z, angulo = -1, tempo = -1;
};

struct Translate {
	bool vazio = true;
	float tempo = NULL;
	float **points;
	int numPointers;
	float **matrix;
};

struct Color {
	bool vazio = true;
	float *diffuseColor = NULL,
		*specularColor = NULL,
		*emissiveColor = NULL,
		*ambientColor = NULL;
};

struct Light {
	string type;
	float pos[4];
	Color color;
	float cons = NULL, quad = NULL, linear = NULL, spotCutOff = NULL, *spotDirection = NULL, spotExponent = NULL;
	unsigned int id;
};

struct Figure {
	string name;
	int vboIndex = -1, normalIndex, textureIndex, textureIndexVBO = -1, triangles = 0;
	Color color;
	Scale scale;
	Rotation rotate;
	Translate translate;
};

struct Tree {
	Figure figure;
	vector<Light> lights;
	std::vector<Tree> subtrees;
};

extern map<string, int> figureIndex;
extern map<string, int> figureTriangles;
extern map<string, int> texturesIndex;
extern map<string, int> textureCoord;
extern map<string, int> normalIndex;
extern Tree tree;
extern Translate trans;
extern unsigned int light_number;

void getGlobalCatmullRomPoint(float gt, float *res, float *deriv);
void toMatrix(Translate t);
Tree getGroup(XMLElement *node);
void loadSceneXML();
