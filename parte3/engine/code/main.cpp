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
using namespace tinyxml2;
using namespace std;

#define _USE_MATH_DEFINES
#define ESPACO 100


class Scale {
public:
	Scale() :vazio(true) {}
	bool vazio;
	float x, y, z;
};

class Rotation {
public:
	Rotation() :vazio(true), angulo(-1), tempo(-1) {}
	bool vazio;
	float x, y, z, angulo, tempo;
};

class Translate {
public:
	Translate() :vazio(true) {}
	bool vazio;
	float tempo;
	float** pontos;
	int numPontos;
	float** matriz;
};

class Model {
public:
	Model() : vboIndice(-1), triangulos(0) {}
	string name;
	int vboIndice, triangulos;
	Scale escala;
	Rotation rotacao;
	Translate translacao;

};

class Tree {
public:
	Model modelo;
	std::vector<Tree> subArvores;
};

std::vector<pair<string, pair<int, int>>> figureIndexes;

Tree arvore;
Translate trans;

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float cameraX, cameraY, cameraZ;
float rodaI = 0.0;
float tempo = 0;
static float p;

float up[3] = { 0.0,1.0,0.0 };
float leftCometa[3];

pair<int, int> loadFigure(string object) {
	ifstream file;
	file.open(object);

	int numTriang;
	file >> numTriang;

	float *v;
	v = (float *)malloc(sizeof(float) * numTriang * 3 * 3);
	int i = 0;
	while (!file.eof())
		file >> v[i++];

	unsigned int t;
	glGenBuffers(1, &t);
	glBindBuffer(GL_ARRAY_BUFFER, t);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numTriang * 3 * 3, v, GL_STATIC_DRAW);

	free(v);
	return make_pair(t, numTriang);
}

void getCatmullRomPoint(float t, int *indices, float *polinom, float *deriv) {

	// catmull-rom matriz
	float catMatrix[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
	{ 1.0f, -2.5f,  2.0f, -0.5f },
	{ -0.5f,  0.0f,  0.5f,  0.0f },
	{ 0.0f,  1.0f,  0.0f,  0.0f } };

	polinom[0] = 0.0; polinom[1] = 0.0; polinom[2] = 0.0;
	deriv[0] = 0.0; deriv[1] = 0.0; deriv[2] = 0.0;
	// Compute point polinom = T * M * P
	// where Pi = p[indices[i]] 
	// ...
	for (int i = 0; i < 3; i++) {
		polinom[i] = (((pow(t, 3)*catMatrix[0][0] + pow(t, 2)*catMatrix[1][0] + t*catMatrix[2][0] + catMatrix[3][0]) * (trans.pontos)[indices[0]][i])
			+ ((pow(t, 3)*catMatrix[0][1] + pow(t, 2)*catMatrix[1][1] + t*catMatrix[2][1] + catMatrix[3][1])*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 3)*catMatrix[0][2] + pow(t, 2)*catMatrix[1][2] + t*catMatrix[2][2] + catMatrix[3][2])*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 3)*catMatrix[0][3] + pow(t, 2)*catMatrix[1][3] + t*catMatrix[2][3] + catMatrix[3][3])*(trans.pontos)[indices[3]][i]));

		deriv[i] = (((pow(t, 2)*catMatrix[0][0] * 3 + pow(t, 1)*catMatrix[1][0] * 2 + catMatrix[2][0] + 0)*(trans.pontos)[indices[0]][i])
			+ ((pow(t, 2)*catMatrix[0][1] * 3 + pow(t, 1)*catMatrix[1][1] * 2 + catMatrix[2][1] + 0)*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 2)*catMatrix[0][2] * 3 + pow(t, 1)*catMatrix[1][2] * 2 + catMatrix[2][2] + 0)*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 2)*catMatrix[0][3] * 3 + pow(t, 1)*catMatrix[1][3] * 2 + catMatrix[2][3] + 0)*(trans.pontos)[indices[3]][i]));
	}
}

void getPontoGlobalCatmull(float gt, float *polinom, float *deriv) {

	float t = gt * (trans.numPontos); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

				   // indices store the pontos
	int indices[4];
	indices[0] = (index + (trans.numPontos) - 1) % (trans.numPontos);
	indices[1] = (indices[0] + 1) % (trans.numPontos);
	indices[2] = (indices[1] + 1) % (trans.numPontos);
	indices[3] = (indices[2] + 1) % (trans.numPontos);
	getCatmullRomPoint(t, indices, polinom, deriv);
}


void toMatrix(Translate t) {
	trans = t;

	t.matriz[0] = (float*)malloc(sizeof(float) * 2);
	t.matriz[0][0] = 0; // t=0;
	t.matriz[0][1] = 0; // distancia=0;

	float tt = 0, pAnterior[3], deriv[3], point[3];
	getPontoGlobalCatmull(tt, pAnterior, deriv);

	for (int i = 1; i < (int)t.numPontos * ESPACO + 1; i++) {

		t.matriz[i] = (float*)malloc(sizeof(float) * 2);
		tt = (float)i / (float)(t.numPontos*ESPACO);
		t.matriz[i][0] = tt;
		getPontoGlobalCatmull(tt, point, deriv);

		t.matriz[i][1] = sqrt(powf((point[0] - pAnterior[0]), 2) +
			powf((point[1] - pAnterior[1]), 2) +
			powf((point[2] - pAnterior[2]), 2)) +
			t.matriz[i - 1][1];

		pAnterior[0] = point[0];
		pAnterior[1] = point[1];
		pAnterior[2] = point[2];
	}
}

Tree getModelTreeFromXML(XMLElement* node) {

	Tree t;
	XMLElement* child = node->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement()) {
		string tag = child->Value();

		if (strcmp(tag.c_str(), "translacao") == 0) {
			t.modelo.translacao.vazio = false;
			t.modelo.translacao.tempo = child->IntAttribute("tempo");

			vector<float> pts;

			XMLElement *pModel = child->FirstChildElement();

			for (; pModel != NULL; pModel = pModel->NextSiblingElement()) {
				pts.push_back(pModel->FloatAttribute("X"));
				pts.push_back(pModel->FloatAttribute("Y"));
				pts.push_back(pModel->FloatAttribute("Z"));
			}

			t.modelo.translacao.numPontos = pts.size() / 3;
			t.modelo.translacao.pontos = (float**)malloc(sizeof(float*) * t.modelo.translacao.numPontos);
			t.modelo.translacao.matriz = (float**)malloc(sizeof(float*) * t.modelo.translacao.numPontos*ESPACO + 1);

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
		else if (strcmp(tag.c_str(), "rotacao") == 0) {
			t.modelo.rotacao.x = child->DoubleAttribute("axisX");
			t.modelo.rotacao.y = child->DoubleAttribute("axisY");
			t.modelo.rotacao.z = child->DoubleAttribute("axisZ");
			if (child->DoubleAttribute("angulo"))
				t.modelo.rotacao.angulo = child->DoubleAttribute("angulo");
			else if (child->DoubleAttribute("tempo"))
				t.modelo.rotacao.tempo = child->DoubleAttribute("tempo");
		}
		else if (strcmp(tag.c_str(), "escala") == 0) {
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

				for (it = figureIndexes.begin(); it != figureIndexes.end(); it++) {
					if (it->first == figureName.c_str()) {
						t.modelo.vboIndice = it->second.first;
						t.modelo.triangulos = it->second.second;
					}
				}
				if (t.modelo.vboIndice == -1) {
					pair<int, int> ret = loadFigure(figureName.c_str());
					t.modelo.vboIndice = ret.first;
					t.modelo.triangulos = ret.second;
					figureIndexes.push_back(make_pair(figureName.c_str(), ret));
				}
			}
		}
		else if (strcmp(tag.c_str(), "group") == 0) {
			Tree newTree;
			newTree = getModelTreeFromXML(child);
			if (child->Attribute("a") != nullptr)
				newTree.modelo.name = child->Attribute("a");
			else
				newTree.modelo.name = "nada";
			t.subArvores.push_back(newTree);
		}
	}
	return t;
}

void loadModel() {
	XMLDocument doc;						   // vector que vai conter nome das figuras presentes no ficheiro XML
	XMLError load = doc.LoadFile("scene.xml"); // abre ficheiro XML
											   // se conseguiu abrir o ficheiro vai colocar no vetor o nome  das figuras a carregar
	if (load != XML_SUCCESS) {
		printf("Erro no ficheiro xml.\n");
		return;
	}
	XMLElement *pRoot = doc.FirstChildElement("scene");
	if (pRoot == nullptr) return;
	arvore = getModelTreeFromXML(pRoot);

}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matriz as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matriz mode
	glMatrixMode(GL_MODELVIEW);
}

void renderCurvaCatmull() {

	float polinom[3];
	float deriv[3];
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++) {
		getPontoGlobalCatmull(i / 1000.0, polinom, deriv);
		glVertex3fv(polinom);
	}
	glEnd();
}

void getMatrizRotacao(float *x, float *y, float *z, float *catMatrix) {
	catMatrix[0] = x[0]; catMatrix[1] = x[1]; catMatrix[2] = x[2]; catMatrix[3] = 0; catMatrix[4] = y[0]; catMatrix[5] = y[1]; catMatrix[6] = y[2]; catMatrix[7] = 0; catMatrix[8] = z[0]; catMatrix[9] = z[1]; catMatrix[10] = z[2]; catMatrix[11] = 0; catMatrix[12] = 0; catMatrix[13] = 0; catMatrix[14] = 0; catMatrix[15] = 1;
}

void cross(float *a, float *b, float *polinom) {
	polinom[0] = a[1] * b[2] - a[2] * b[1];
	polinom[1] = a[2] * b[0] - a[0] * b[2];
	polinom[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]); a[0] = a[0] / l; a[1] = a[1] / l; a[2] = a[2] / l;
}

void drawScene(Tree t) {

	glPushMatrix();
	if (!t.modelo.translacao.vazio) {
		trans = t.modelo.translacao;
		renderCurvaCatmull();
		tempo = glutGet(GLUT_ELAPSED_TIME);
		float aux = fmod(tempo, (float)(t.modelo.translacao.tempo * 1000)) / (t.modelo.translacao.tempo * 1000); // 1000 is the number of catmull-rom's division
		float dist = aux*t.modelo.translacao.matriz[(int)(t.modelo.translacao.numPontos)*ESPACO][1];

		for (int i = 0; i < (int)(t.modelo.translacao.numPontos)*ESPACO + 1; i++) {
			if (dist == t.modelo.translacao.matriz[i][1])
				p = t.modelo.translacao.matriz[i][0];
			else if (dist > t.modelo.translacao.matriz[i][1] && dist < t.modelo.translacao.matriz[i + 1][1]) {
				float f = (float)(dist - t.modelo.translacao.matriz[i][1]) / (t.modelo.translacao.matriz[i + 1][1] - t.modelo.translacao.matriz[i][1]);
				p = t.modelo.translacao.matriz[i][0] * (1 - f) + (t.modelo.translacao.matriz[i + 1][0] * f);
			}
		}

		float ponto[3], deriv[3];
		float catMatrix[16];
		getPontoGlobalCatmull(p, ponto, deriv);

		glTranslatef(ponto[0], ponto[1], ponto[2]);

		if (strcmp(t.modelo.name.c_str(), "Cometa") == 0) {
			
			cross(deriv, up, leftCometa);
			cross(leftCometa, deriv, up);
			normalize(leftCometa);
			normalize(up);
			normalize(deriv);

			getMatrizRotacao(deriv, up, leftCometa, catMatrix);
			glMultMatrixf(catMatrix);
		}
	}

	if (!t.modelo.rotacao.vazio)
		if (t.modelo.rotacao.angulo != -1)
			glRotatef(t.modelo.rotacao.angulo, t.modelo.rotacao.x, t.modelo.rotacao.y, t.modelo.rotacao.z);
		else if (t.modelo.rotacao.tempo != -1) {
			tempo = glutGet(GLUT_ELAPSED_TIME);
			float aux = fmod(tempo, (float)(t.modelo.rotacao.tempo * 1000)) / (t.modelo.rotacao.tempo * 1000);
			float angulo = aux * 360;
			glRotatef(angulo, t.modelo.rotacao.x, t.modelo.rotacao.y, t.modelo.rotacao.z);
		}
	if (!t.modelo.escala.vazio)
		glScalef(t.modelo.escala.x, t.modelo.escala.y, t.modelo.escala.z);

	//printf("nome %s // triangulos %d\n", t.modelo.name.c_str(), t.modelo.triangulos);

	if (t.modelo.vboIndice != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, t.modelo.vboIndice);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, t.modelo.triangulos * 3);
	}
	std::vector<Tree>::iterator it;
	for (it = t.subArvores.begin(); it != t.subArvores.end(); it++)
		drawScene(*it);

	glPopMatrix();
}

void spherical2Cartesian() {
	cameraX = radius * cos(beta) * sin(alfa);
	cameraY = radius * sin(beta);
	cameraZ = radius * cos(beta) * cos(alfa);
}

void renderScene(void) {
	// clear buffers

	glClearColor(0, 0, 0.05, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	spherical2Cartesian();

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ,
		dx, dy, dz,
		0.0f, 1.0f, 0.0f);

	glClearColor(0, 0, 0.05, 1);

	drawScene(arvore);
	glLoadIdentity();
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c)
	{
	case 'd': dx -= 0.1;
		break;
	case 'a': dx += 0.1;
		break;
	case 'w': dy -= 0.1;
		break;
	case 's': dy += 0.1;
		break;
	case 'q': dz -= 5;
		break;
	case 'e': dz += 5;
		break;
	case 'i': radius -= 1.0f;
		break;
	case 'o': radius += 1.0f;
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
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

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

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
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
	glutCreateWindow("engine 1.2");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glewInit();

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	// enter GLUT's main cycle
	loadModel();
	glutMainLoop();

	return 1;
}