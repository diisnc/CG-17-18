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

class Scale {
public:
	Scale() : vazio(true) {}
	bool vazio;
	float x, y, z;
};

class Rotation {
public:
	Rotation() : vazio(true), angulo(-1), tempo(-1) {}
	bool vazio;
	float x, y, z, angulo, tempo;
};

class Translate {
public:
	Translate() : vazio(true), tempo(NULL) {}
	bool vazio;
	float tempo;
	float **points;
	int numPointers;
	float **matrix;
};

class Color {
public:
	Color() : diffuseColor(NULL), specularColor(NULL), emissiveColor(NULL), ambientColor(NULL), vazio(true) {}
	bool vazio;
	float *diffuseColor,
		*specularColor,
		*emissiveColor,
		*ambientColor;
};

class Light {
public:
	Light() : cons(NULL), quad(NULL), linear(NULL), spotCutOff(NULL), spotDirection(NULL), spotExponent(NULL) {}
	string type;
	float pos[4];
	Color color;
	float cons, quad, linear, spotCutOff, *spotDirection, spotExponent;
	unsigned int id;
};

unsigned int light_number;

class Figure {
public:
	Figure() : vboIndex(-1), triangles(0), textureIndexVBO(-1) {}
	string name;
	int vboIndex, normalIndex, textureIndex, textureIndexVBO, triangles;
	Color color;
	Scale scale;
	Rotation rotate;
	Translate translate;
};

class Tree {
public:
	Figure figure;
	vector<Light> lights;
	std::vector<Tree> subtrees;
};

map<string, int> figureIndex;
map<string, int> figureTriangles;
map<string, int> texturesIndex;
map<string, int> textureCoord;
map<string, int> normalIndex;
Tree tree;
Translate trans;

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float camX, camY, camZ;
float rodaI = 0.0;
float time = 0;
static float p;

float up[3] = { 0.0, 1.0, 0.0 };
float leftCometa[3];

int loadFigure(string object, float **v, float **n, float **t) {
	ifstream file;
	file.open(object);

	int nTriangles;
	file >> nTriangles;

	*v = (float *)malloc(sizeof(float) * nTriangles * 3 * 3);
	*n = (float *)malloc(sizeof(float) * nTriangles * 3 * 3);
	*t = (float *)malloc(sizeof(float) * nTriangles * 3 * 2);

	int i_v = 0, i_n = 0, i_t = 0, l;

	for (int k = 0; k < (nTriangles * 3); k++) {
		for (l = 1; l <= 3; l++) {
			if (l == 1)
				for (int it = 0; it < 3; it++)
					file >> (*v)[i_v++];
			if (l == 2)
				for (int it = 0; it < 3; it++)
					file >> (*n)[i_n++];
			if (l == 3)
				for (int it = 0; it < 2; it++)
					file >> (*t)[i_t++];
		}
	}
	return nTriangles;
}

void getCatmullRomPoint(float t, int *indices, float *res, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0;
	res[1] = 0.0;
	res[2] = 0.0;
	deriv[0] = 0.0;
	deriv[1] = 0.0;
	deriv[2] = 0.0;
	// Compute point res = T * M * P
	// where Pi = p[indices[i]]
	// ...
	for (int i = 0; i < 3; i++) {
		res[i] = (((pow(t, 3) * m[0][0] + pow(t, 2) * m[1][0] + t * m[2][0] + m[3][0]) * (trans.points)[indices[0]][i]) + ((pow(t, 3) * m[0][1] + pow(t, 2) * m[1][1] + t * m[2][1] + m[3][1]) * (trans.points)[indices[1]][i]) + ((pow(t, 3) * m[0][2] + pow(t, 2) * m[1][2] + t * m[2][2] + m[3][2]) * (trans.points)[indices[2]][i]) + ((pow(t, 3) * m[0][3] + pow(t, 2) * m[1][3] + t * m[2][3] + m[3][3]) * (trans.points)[indices[3]][i]));

		deriv[i] = (((pow(t, 2) * m[0][0] * 3 + pow(t, 1) * m[1][0] * 2 + m[2][0] + 0) * (trans.points)[indices[0]][i]) + ((pow(t, 2) * m[0][1] * 3 + pow(t, 1) * m[1][1] * 2 + m[2][1] + 0) * (trans.points)[indices[1]][i]) + ((pow(t, 2) * m[0][2] * 3 + pow(t, 1) * m[1][2] * 2 + m[2][2] + 0) * (trans.points)[indices[2]][i]) + ((pow(t, 2) * m[0][3] * 3 + pow(t, 1) * m[1][3] * 2 + m[2][3] + 0) * (trans.points)[indices[3]][i]));
	}
}

void getGlobalCatmullRomPoint(float gt, float *res, float *deriv) {

	float t = gt * (trans.numPointers); // this is the real global t
	int index = floor(t);							// which segment
	t = t - index;										// where within  the segment

														// indices store the points
	int indices[4];
	indices[0] = (index + (trans.numPointers) - 1) % (trans.numPointers);
	indices[1] = (indices[0] + 1) % (trans.numPointers);
	indices[2] = (indices[1] + 1) % (trans.numPointers);
	indices[3] = (indices[2] + 1) % (trans.numPointers);
	getCatmullRomPoint(t, indices, res, deriv);
}

void toMatrix(Translate t) {
	trans = t;

	t.matrix[0] = (float *)malloc(sizeof(float) * 2);
	t.matrix[0][0] = 0; // t=0;
	t.matrix[0][1] = 0; // distancia=0;

	float tt = 0, pAnterior[3], deriv[3], point[3];
	getGlobalCatmullRomPoint(tt, pAnterior, deriv);

	for (int i = 1; i < (int)t.numPointers * SPACE + 1; i++) {

		t.matrix[i] = (float *)malloc(sizeof(float) * 2);
		tt = (float)i / (float)(t.numPointers * SPACE);
		t.matrix[i][0] = tt;
		getGlobalCatmullRomPoint(tt, point, deriv);

		t.matrix[i][1] = sqrt(powf((point[0] - pAnterior[0]), 2) +
			powf((point[1] - pAnterior[1]), 2) +
			powf((point[2] - pAnterior[2]), 2)) +
			t.matrix[i - 1][1];

		pAnterior[0] = point[0];
		pAnterior[1] = point[1];
		pAnterior[2] = point[2];
	}
}
int getTexture(string name) {
	printf("%s\n", name.c_str());
	unsigned int id, img;
	unsigned int w, h;
	unsigned char *data;

	ilGenImages(1, &img);
	ilBindImage(img);
	ilLoadImage((ILstring)name.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	data = ilGetData();

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

Tree getGroup(XMLElement *node) {

	Tree t;
	XMLElement *child = node->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement()) {
		string tag = child->Value();
		//printf("tag : %s\n", tag.c_str());
		if (strcmp(tag.c_str(), "translate") == 0) {

			t.figure.translate.vazio = false;
			t.figure.translate.tempo = child->IntAttribute("time");

			vector<float> pts;

			XMLElement *pModel = child->FirstChildElement();

			for (; pModel != NULL; pModel = pModel->NextSiblingElement()) {
				pts.push_back(pModel->FloatAttribute("X"));
				pts.push_back(pModel->FloatAttribute("Y"));
				pts.push_back(pModel->FloatAttribute("Z"));
			}

			t.figure.translate.numPointers = pts.size() / 3;
			t.figure.translate.points = (float **)malloc(sizeof(float *) * t.figure.translate.numPointers);
			t.figure.translate.matrix = (float **)malloc(sizeof(float *) * t.figure.translate.numPointers * SPACE + 1);

			vector<float>::iterator itpts;
			int aux = 0;

			for (itpts = pts.begin(); itpts != pts.end(); itpts++) {
				t.figure.translate.points[aux] = (float *)malloc(sizeof(float) * 3);

				t.figure.translate.points[aux][0] = *itpts;
				itpts++;
				t.figure.translate.points[aux][1] = *itpts;
				itpts++;
				t.figure.translate.points[aux][2] = *itpts;
				aux++;
			}
			toMatrix(t.figure.translate);
		}
		else if (strcmp(tag.c_str(), "rotate") == 0) {

			t.figure.rotate.x = child->DoubleAttribute("axisX");
			t.figure.rotate.y = child->DoubleAttribute("axisY");
			t.figure.rotate.z = child->DoubleAttribute("axisZ");
			if (child->DoubleAttribute("angle"))
				t.figure.rotate.angulo = child->DoubleAttribute("angle");
			else if (child->DoubleAttribute("time"))
				t.figure.rotate.tempo = child->DoubleAttribute("time");
		}
		else if (strcmp(tag.c_str(), "scale") == 0) {

			t.figure.scale.vazio = false;
			t.figure.scale.x = child->DoubleAttribute("X");
			t.figure.scale.y = child->DoubleAttribute("Y");
			t.figure.scale.z = child->DoubleAttribute("Z");
		}
		else if (strcmp(tag.c_str(), "models") == 0) {

			XMLElement *modelsNode = child->FirstChildElement();
			for (; modelsNode != nullptr; modelsNode = modelsNode->NextSiblingElement()) {
				string figureName = modelsNode->Attribute("file");

				if (modelsNode->Attribute("texture") != NULL) {

					string name = modelsNode->Attribute("texture");
					auto index = texturesIndex.find(name.c_str());

					if (index != texturesIndex.end()) {
						t.figure.textureIndex = index->second;
					}
					else {
						int id = getTexture(name);
						t.figure.textureIndex = id;
						texturesIndex.insert(make_pair(name, id));
					}
				}
				else
					t.figure.textureIndex = 0;

				auto vIndex = figureIndex.find(figureName);
				auto nIndex = normalIndex.find(figureName);
				auto tIndex = figureTriangles.find(figureName);
				auto txIndex = textureCoord.find(figureName);

				if (vIndex != figureIndex.end()) {
					t.figure.vboIndex = vIndex->second;
					t.figure.normalIndex = nIndex->second;
					t.figure.triangles = tIndex->second;
					t.figure.textureIndexVBO = txIndex->second;
				}
				else {
					float *v, *n, *tx;
					int triangles = loadFigure(figureName.c_str(), &v, &n, &tx);
					printf("%s\n", figureName.c_str());

					if (strcmp(figureName.c_str(), "sun.3d") == 0) {
						for (int ij = 0; ij < triangles * 3 * 3; ij++) {
							n[ij] *= -1;
						}
					}

					unsigned indexV, indexN, indexT;

					glGenBuffers(1, &indexV);
					glBindBuffer(GL_ARRAY_BUFFER, indexV);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangles * 3 * 3, v, GL_STATIC_DRAW);

					glGenBuffers(1, &indexN);
					glBindBuffer(GL_ARRAY_BUFFER, indexN);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangles * 3 * 3, n, GL_STATIC_DRAW);

					glGenBuffers(1, &indexT);
					glBindBuffer(GL_ARRAY_BUFFER, indexT);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangles * 3 * 2, tx, GL_STATIC_DRAW);

					free(v);
					free(n);
					free(tx);

					t.figure.triangles = triangles;
					t.figure.vboIndex = indexV;
					t.figure.normalIndex = indexN;
					t.figure.textureIndexVBO = indexT;

					figureIndex.insert(make_pair(figureName.c_str(), indexV));
					figureTriangles.insert(make_pair(figureName.c_str(), triangles));
					normalIndex.insert(make_pair(figureName.c_str(), indexN));
					textureCoord.insert(make_pair(figureName.c_str(), indexT));
				}

				float white[4] = { 1, 1, 1, 1 };
				float black[4] = { 0, 0, 0, 0 };

				if (modelsNode->Attribute("ambR")) {
					t.figure.color.ambientColor = (float *)malloc(sizeof(float) * 4);
					t.figure.color.ambientColor[0] = modelsNode->DoubleAttribute("ambR");
					t.figure.color.ambientColor[1] = modelsNode->DoubleAttribute("ambG");
					t.figure.color.ambientColor[2] = modelsNode->DoubleAttribute("ambB");
					t.figure.color.ambientColor[3] = 1.0f;
				}
				else {
					t.figure.color.ambientColor = black;
				}

				if (modelsNode->Attribute("specR")) {
					t.figure.color.specularColor = (float *)malloc(sizeof(float) * 4);
					t.figure.color.specularColor[0] = modelsNode->DoubleAttribute("specR");
					t.figure.color.specularColor[1] = modelsNode->DoubleAttribute("specG");
					t.figure.color.specularColor[2] = modelsNode->DoubleAttribute("specB");
					t.figure.color.specularColor[3] = 1.0f;
				}
				else {
					t.figure.color.specularColor = white;
				}

				if (modelsNode->Attribute("diffR")) {
					t.figure.color.diffuseColor = (float *)malloc(sizeof(float) * 4);
					t.figure.color.diffuseColor[0] = modelsNode->DoubleAttribute("diffR");
					t.figure.color.diffuseColor[1] = modelsNode->DoubleAttribute("diffG");
					t.figure.color.diffuseColor[2] = modelsNode->DoubleAttribute("diffB");
					t.figure.color.diffuseColor[3] = 1.0f;
				}
				else {
					t.figure.color.diffuseColor = black;
				}

				if (modelsNode->Attribute("emissR")) {
					t.figure.color.emissiveColor = (float *)malloc(sizeof(float) * 4);
					t.figure.color.emissiveColor[0] = modelsNode->DoubleAttribute("emissR");
					t.figure.color.emissiveColor[1] = modelsNode->DoubleAttribute("emissR");
					t.figure.color.emissiveColor[2] = modelsNode->DoubleAttribute("emissR");
					t.figure.color.emissiveColor[3] = 1.0f;
				}
				else {
					t.figure.color.emissiveColor = white;
				}
			}
		}
		else if (strcmp(tag.c_str(), "group") == 0) {
			Tree newTree;
			newTree = getGroup(child);

			if (child->Attribute("a") != NULL)
				newTree.figure.name = child->Attribute("a");
			else
				newTree.figure.name = "nada";
			t.subtrees.push_back(newTree);
		}
		else if (strcmp(tag.c_str(), "lights") == 0) {
			Tree newTree;
			XMLElement *lightn = child->FirstChildElement();

			glEnable(GL_LIGHTING);
			for (; lightn && light_number < 8; lightn = lightn->NextSiblingElement(), light_number++) {

				glEnable(GL_LIGHT0 + light_number);
				Light newL;
				newL.id = light_number;

				if (lightn->Attribute("type"))
					newL.type = lightn->Attribute("type");

				newL.pos[0] = lightn->DoubleAttribute("posX");
				newL.pos[1] = lightn->DoubleAttribute("posY");
				newL.pos[2] = lightn->DoubleAttribute("posZ");
				newL.pos[3] = 1.0f;

				if (strcmp(newL.type.c_str(), "DIRECTIONAL") == 0)
					newL.pos[3] = 0.0f;

				if (lightn->Attribute("ambR")) {
					newL.color.ambientColor = (float *)malloc(sizeof(float) * 4);
					newL.color.ambientColor[0] = lightn->DoubleAttribute("ambR");
					newL.color.ambientColor[1] = lightn->DoubleAttribute("ambG");
					newL.color.ambientColor[2] = lightn->DoubleAttribute("ambB");
					newL.color.ambientColor[3] = 1.0f;
				}

				if (lightn->Attribute("specR")) {
					newL.color.specularColor = (float *)malloc(sizeof(float) * 4);
					newL.color.specularColor[0] = lightn->DoubleAttribute("specR");
					newL.color.specularColor[1] = lightn->DoubleAttribute("specG");
					newL.color.specularColor[2] = lightn->DoubleAttribute("specB");
					newL.color.specularColor[3] = 1.0f;
				}

				if (lightn->Attribute("diffR")) {
					newL.color.diffuseColor = (float *)malloc(sizeof(float) * 4);
					newL.color.diffuseColor[0] = lightn->DoubleAttribute("diffR");
					newL.color.diffuseColor[1] = lightn->DoubleAttribute("diffG");
					newL.color.diffuseColor[2] = lightn->DoubleAttribute("diffB");
					newL.color.diffuseColor[3] = 1.0f;
				}

				if (lightn->Attribute("emissR")) {
					newL.color.emissiveColor = (float *)malloc(sizeof(float) * 4);
					newL.color.emissiveColor[0] = lightn->DoubleAttribute("emissR");
					newL.color.emissiveColor[1] = lightn->DoubleAttribute("emissR");
					newL.color.emissiveColor[2] = lightn->DoubleAttribute("emissR");
					newL.color.emissiveColor[3] = 1.0f;
				}

				if (lightn->Attribute("constAtt"))
					newL.cons = lightn->DoubleAttribute("constAtt");

				if (lightn->Attribute("linearAtt"))
					newL.linear = lightn->DoubleAttribute("inearAtt");

				if (lightn->Attribute("quadAtt"))
					newL.quad = lightn->DoubleAttribute("quadAttt");

				if (lightn->Attribute("spotCutOff"))
					newL.spotCutOff = lightn->DoubleAttribute("spotCutOff");

				if (lightn->Attribute("spotDirectionX")) {
					newL.spotDirection = (float *)malloc(sizeof(float) * 4);
					newL.spotDirection[0] = lightn->DoubleAttribute("spotDirectionX");
					newL.spotDirection[1] = lightn->DoubleAttribute("spotDirectionY");
					newL.spotDirection[2] = lightn->DoubleAttribute("spotDirectionZ");
				}

				if (lightn->Attribute("spotExponent"))
					newL.spotExponent = lightn->DoubleAttribute("spotExponent");

				newTree.lights.push_back(newL);
			}
			t.subtrees.push_back(newTree);
		}
	}
	return t;
}

void getFigures() {
	XMLDocument doc;													 // vector que vai conter nome das figuras presentes no ficheiro XML
	XMLError load = doc.LoadFile("scene.xml"); // abre ficheiro XML
											   // se conseguiu abrir o ficheiro vai colocar no vetor o nome  das figuras a carregar
	if (load != XML_SUCCESS) {
		printf("Erro no ficheiro xml.\n");
		return;
	}
	XMLElement *pRoot = doc.FirstChildElement("scene");
	if (pRoot == nullptr)
		return;
	tree = getGroup(pRoot);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderCatmullRomCurve() {

	float res[3];
	float deriv[3];
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++) {
		getGlobalCatmullRomPoint(i / 1000.0, res, deriv);
		glVertex3fv(res);
		glNormal3fv(res);
	}
	glEnd();
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {
	m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = 0;
	m[4] = y[0];
	m[5] = y[1];
	m[6] = y[2];
	m[7] = 0;
	m[8] = z[0];
	m[9] = z[1];
	m[10] = z[2];
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

void cross(float *a, float *b, float *res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void drawScene(Tree t) {

	glPushMatrix();
	if (!t.figure.translate.vazio) {

		if (t.figure.translate.tempo != 0) {
			trans = t.figure.translate;
			float color[4] = { 1.0, 1.0, 1.0, 1.0 };

			renderCatmullRomCurve();
			time = glutGet(GLUT_ELAPSED_TIME);
			float aux = fmod(time, (float)(t.figure.translate.tempo * 1000)) / (t.figure.translate.tempo * 1000);
			float dist = aux * t.figure.translate.matrix[(int)(t.figure.translate.numPointers) * SPACE][1];

			for (int i = 0; i < (int)(t.figure.translate.numPointers) * SPACE + 1; i++) {
				if (dist == t.figure.translate.matrix[i][1])
					p = t.figure.translate.matrix[i][0];
				else if (dist > t.figure.translate.matrix[i][1] && dist < t.figure.translate.matrix[i + 1][1]) {
					float f = (float)(dist - t.figure.translate.matrix[i][1]) / (t.figure.translate.matrix[i + 1][1] - t.figure.translate.matrix[i][1]);
					p = t.figure.translate.matrix[i][0] * (1 - f) + (t.figure.translate.matrix[i + 1][0] * f);
				}
			}

			float ponto[3], deriv[3];
			float m[16];
			getGlobalCatmullRomPoint(p, ponto, deriv);

			glTranslatef(ponto[0], ponto[1], ponto[2]);

			if (strcmp(t.figure.name.c_str(), "Cometa") == 0) {

				cross(deriv, up, leftCometa);
				cross(leftCometa, deriv, up);
				normalize(leftCometa);
				normalize(up);
				normalize(deriv);

				buildRotMatrix(deriv, up, leftCometa, m);
				glMultMatrixf(m);
			}
		}
		else {
			glTranslatef(t.figure.translate.points[0][0], t.figure.translate.points[0][1], t.figure.translate.points[0][3]);
		}
	}

	if (t.figure.color.ambientColor != NULL) {
		//glMaterialfv(GL_FRONT, GL_AMBIENT, t.figure.color.ambientColor);
		//printf("vbo index : %d", t.figure.vboIndex);
	}

	if (t.figure.color.diffuseColor != NULL)
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, t.figure.color.diffuseColor);

		if (t.figure.color.emissiveColor != NULL)
			//glMaterialfv(GL_FRONT, GL_EMISSION, t.figure.color.emissiveColor);

			if (t.figure.color.specularColor != NULL)
				//glMaterialfv(GL_FRONT, GL_SPECULAR, t.figure.color.specularColor);

				if (!t.figure.rotate.vazio)
					if (t.figure.rotate.angulo != -1)
						glRotatef(t.figure.rotate.angulo, t.figure.rotate.x, t.figure.rotate.y, t.figure.rotate.z);
					else if (t.figure.rotate.tempo != -1) {
						time = glutGet(GLUT_ELAPSED_TIME);
						float aux = fmod(time, (float)(t.figure.rotate.tempo * 1000)) / (t.figure.rotate.tempo * 1000);
						float angle = aux * 360;
						glRotatef(angle, t.figure.rotate.x, t.figure.rotate.y, t.figure.rotate.z);
					}
					if (!t.figure.scale.vazio)
						glScalef(t.figure.scale.x, t.figure.scale.y, t.figure.scale.z);

					vector<Light>::iterator i = t.lights.begin();
					for (; i != t.lights.end(); i++) {

						glLightfv(GL_LIGHT0 + i->id, GL_POSITION, i->pos);

						if (i->color.ambientColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_AMBIENT, i->color.ambientColor);
						}

						if (i->color.diffuseColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_DIFFUSE, i->color.diffuseColor);
						}

						if (i->color.emissiveColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_EMISSION, i->color.emissiveColor);
						}

						if (i->color.specularColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_SPECULAR, i->color.specularColor);
						}

						if (strcmp(i->type.c_str(), "POINT") == 0) {

							if (i->cons) {
								float arr[1] = { i->cons };

								glLightfv(GL_LIGHT0 + i->id, GL_CONSTANT_ATTENUATION, arr);
							}

							if (i->linear) {
								float arr[1] = { i->linear };
								glLightfv(GL_LIGHT0 + i->id, GL_LINEAR_ATTENUATION, arr);
							}

							if (i->quad) {
								float arr[1] = { i->quad };
								glLightfv(GL_LIGHT0 + i->id, GL_QUADRATIC_ATTENUATION, arr);
							}
						}

						if (strcmp(i->type.c_str(), "SPOTLIGHT") == 0) {
							if (i->spotCutOff) {
								float arr[1] = { i->spotCutOff };
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_CUTOFF, arr);
							}

							if (i->spotDirection) {
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_DIRECTION, i->spotDirection);
							}

							if (i->spotExponent) {
								float arr[1] = { i->spotExponent };
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_EXPONENT, arr);
							}
						}
					}

					if (t.figure.vboIndex != -1) {

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.vboIndex);
						glVertexPointer(3, GL_FLOAT, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.normalIndex);
						glNormalPointer(GL_FLOAT, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.textureIndexVBO);
						glTexCoordPointer(2, GL_FLOAT, 0, 0);
						glBindTexture(GL_TEXTURE_2D, t.figure.textureIndex);

						glDrawArrays(GL_TRIANGLES, 0, t.figure.triangles * 3);
						glBindTexture(GL_TEXTURE_2D, 0);
					}

					std::vector<Tree>::iterator it;
					for (it = t.subtrees.begin(); it != t.subtrees.end(); it++)
						drawScene(*it);

					glPopMatrix();
}

void spherical2Cartesian() {
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void renderScene(void) {
	// clear buffers

	glClearColor(0, 0, 0.05, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	spherical2Cartesian();

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		dx, dy, dz,
		0.0f, 1.0f, 0.0f);

	glClearColor(0, 0, 0.05, 1);

	drawScene(tree);
	glLoadIdentity();
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c) {
	case 'd':
		dx -= 0.1;
		break;
	case 'a':
		dx += 0.1;
		break;
	case 'w':
		dy -= 0.1;
		break;
	case 's':
		dy += 0.1;
		break;
	case 'q':
		dz -= 5;
		break;
	case 'e':
		dz += 5;
		break;
	case 'i':
		radius -= 1.0f;
		break;
	case 'o':
		radius += 1.0f;
		break;
	case 'z':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'x':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 'c':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1;
		break;

	case GLUT_KEY_LEFT:
		alfa += 0.1;
		break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN:
		radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP:
		radius += 1.0f;
		break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Sistema Solar com Luz e Texturas");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glewInit();

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	// enter GLUT's main cycle
	light_number = 0;
	getFigures();
	glutMainLoop();

	return 1;
}