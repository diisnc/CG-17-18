#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "tinyxml2.h"
#include "xml-loader.h"
#include "engine.h"
using namespace tinyxml2;
using namespace std;

#define _USE_MATH_DEFINES
#define ESPACO 100

Tree arvore;
Translate trans;
std::vector<pair<string, pair<int, int>>> indiceFiguras;

pair<int, int> loadModel(string object) {
	ifstream file;
	file.open(object);

	int numTriangulos;
	file >> numTriangulos;

	float *v;
	v = (float *)malloc(sizeof(float) * numTriangulos * 3 * 3);
	int i = 0;
	while (!file.eof())
		file >> v[i++];

	unsigned int t;
	glGenBuffers(1, &t);
	glBindBuffer(GL_ARRAY_BUFFER, t);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numTriangulos * 3 * 3, v, GL_STATIC_DRAW);

	free(v);
	return make_pair(t, numTriangulos);
}

void getPontoCatmullRom(float t, int *indices, float *res, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
	{ 1.0f, -2.5f,  2.0f, -0.5f },
	{ -0.5f,  0.0f,  0.5f,  0.0f },
	{ 0.0f,  1.0f,  0.0f,  0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
	deriv[0] = 0.0; deriv[1] = 0.0; deriv[2] = 0.0;
	// Compute point res = T * M * P
	// where Pi = p[indices[i]] 
	// ...
	for (int i = 0; i < 3; i++) {
		res[i] = (((pow(t, 3)*m[0][0] + pow(t, 2)*m[1][0] + t * m[2][0] + m[3][0]) * (trans.pontos)[indices[0]][i])
			+ ((pow(t, 3)*m[0][1] + pow(t, 2)*m[1][1] + t * m[2][1] + m[3][1])*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 3)*m[0][2] + pow(t, 2)*m[1][2] + t * m[2][2] + m[3][2])*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 3)*m[0][3] + pow(t, 2)*m[1][3] + t * m[2][3] + m[3][3])*(trans.pontos)[indices[3]][i]));

		deriv[i] = (((pow(t, 2)*m[0][0] * 3 + pow(t, 1)*m[1][0] * 2 + m[2][0] + 0)*(trans.pontos)[indices[0]][i])
			+ ((pow(t, 2)*m[0][1] * 3 + pow(t, 1)*m[1][1] * 2 + m[2][1] + 0)*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 2)*m[0][2] * 3 + pow(t, 1)*m[1][2] * 2 + m[2][2] + 0)*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 2)*m[0][3] * 3 + pow(t, 1)*m[1][3] * 2 + m[2][3] + 0)*(trans.pontos)[indices[3]][i]));
	}
}

void getPontoGlobalCatmullRom(float gt, float *res, float *deriv) {

	float t = gt * (trans.numPontos); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

				   // indices store the pontos
	int indices[4];
	indices[0] = (index + (trans.numPontos) - 1) % (trans.numPontos);
	indices[1] = (indices[0] + 1) % (trans.numPontos);
	indices[2] = (indices[1] + 1) % (trans.numPontos);
	indices[3] = (indices[2] + 1) % (trans.numPontos);
	getPontoCatmullRom(t, indices, res, deriv);
}

void toMatrix(Translate t) {
	trans = t;

	t.matrix[0] = (float*)malloc(sizeof(float) * 2);
	t.matrix[0][0] = 0; // t=0;
	t.matrix[0][1] = 0; // distancia=0;

	float tt = 0, pAnterior[3], deriv[3], point[3];
	getPontoGlobalCatmullRom(tt, pAnterior, deriv);

	for (int i = 1; i < (int)t.numPontos * ESPACO + 1; i++) {

		t.matrix[i] = (float*)malloc(sizeof(float) * 2);
		tt = (float)i / (float)(t.numPontos*ESPACO);
		t.matrix[i][0] = tt;
		getPontoGlobalCatmullRom(tt, point, deriv);

		t.matrix[i][1] = sqrt(powf((point[0] - pAnterior[0]), 2) +
			powf((point[1] - pAnterior[1]), 2) +
			powf((point[2] - pAnterior[2]), 2)) +
			t.matrix[i - 1][1];

		pAnterior[0] = point[0];
		pAnterior[1] = point[1];
		pAnterior[2] = point[2];
	}
}

Tree getTreeFROMXML(XMLElement* node) {

	Tree t;
	XMLElement* child = node->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement()) {
		string tag = child->Value();

		if (strcmp(tag.c_str(), "translate") == 0) {
			t.modelo.translacao.vazio = false;
			t.modelo.translacao.tempo = child->IntAttribute("time");

			vector<float> pts;

			XMLElement *pModel = child->FirstChildElement();

			for (; pModel != NULL; pModel = pModel->NextSiblingElement()) {
				pts.push_back(pModel->FloatAttribute("X"));
				pts.push_back(pModel->FloatAttribute("Y"));
				pts.push_back(pModel->FloatAttribute("Z"));
			}

			t.modelo.translacao.numPontos = pts.size() / 3;
			t.modelo.translacao.pontos = (float**)malloc(sizeof(float*) * t.modelo.translacao.numPontos);
			t.modelo.translacao.matrix = (float**)malloc(sizeof(float*) * t.modelo.translacao.numPontos*ESPACO + 1);

			vector<float>::iterator itpts;
			int aux = 0;

			for (itpts = pts.begin(); itpts != pts.end(); itpts++) {
				t.modelo.translacao.pontos[aux] = (float*)malloc(sizeof(float) * 3);

				t.modelo.translacao.pontos[aux][0] = *itpts;
				itpts++;
				t.modelo.translacao.pontos[aux][1] = *itpts;
				itpts++;
				t.modelo.translacao.pontos[aux][2] = *itpts;
				aux++;
			}
			toMatrix(t.modelo.translacao);
		}
		else if (strcmp(tag.c_str(), "rotate") == 0) {
			t.modelo.rotacao.x = child->DoubleAttribute("axisX");
			t.modelo.rotacao.y = child->DoubleAttribute("axisY");
			t.modelo.rotacao.z = child->DoubleAttribute("axisZ");
			if (child->DoubleAttribute("angulo"))
				t.modelo.rotacao.angulo = child->DoubleAttribute("angulo");
			else if (child->DoubleAttribute("time"))
				t.modelo.rotacao.tempo = child->DoubleAttribute("time");
		}
		else if (strcmp(tag.c_str(), "scale") == 0) {
			t.modelo.escala.vazio = false;
			t.modelo.escala.x = child->DoubleAttribute("X");
			t.modelo.escala.y = child->DoubleAttribute("Y");
			t.modelo.escala.z = child->DoubleAttribute("Z");
		}
		else if (strcmp(tag.c_str(), "models") == 0) {
			XMLElement* modelsNode = child->FirstChildElement();
			for (; modelsNode != nullptr; modelsNode = modelsNode->NextSiblingElement()) {
				string figureName = modelsNode->Attribute("file");
				std::vector<pair<string, pair<int, int>>>::iterator it;

				for (it = indiceFiguras.begin(); it != indiceFiguras.end(); it++) {
					if (it->first == figureName.c_str()) {
						t.modelo.vboIndice = it->second.first;
						t.modelo.triangulos = it->second.second;
					}
				}
				if (t.modelo.vboIndice == -1) {
					pair<int, int> ret = loadModel(figureName.c_str());
					t.modelo.vboIndice = ret.first;
					t.modelo.triangulos = ret.second;
					indiceFiguras.push_back(make_pair(figureName.c_str(), ret));
				}
			}
		}
		else if (strcmp(tag.c_str(), "group") == 0) {
			Tree newTree;
			newTree = getTreeFROMXML(child);
			if (child->Attribute("a") != nullptr)
				newTree.modelo.nome = child->Attribute("a");
			else
				newTree.modelo.nome = "nada";
			t.subArvores.push_back(newTree);
		}
	}
	return t;
}

void loadSceneXML() {
	// Load XML document
	XMLDocument file;
	XMLError load = file.LoadFile("scene.xml");

	if (load != XML_SUCCESS) {
		throw std::runtime_error("O documento XML não foi carregado com sucesso. \n");
	}
	XMLElement *scene = file.FirstChildElement("scene");

	if (scene == nullptr) {
		throw std::runtime_error("Erro ao procurar elemento \"scene\" no XML. \n");
	}
	arvore = getTreeFROMXML(scene);
}
