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
#include "xml-loader.h"
using namespace tinyxml2;
using namespace std;

#define _USE_MATH_DEFINES
#define SPACE 100

map<string, int> figureIndex;
map<string, int> figureTriangles;
map<string, int> texturesIndex;
map<string, int> textureCoord;
map<string, int> normalIndex;
Tree tree;
Translate trans;
unsigned int light_number;

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

void loadSceneXML() {
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
