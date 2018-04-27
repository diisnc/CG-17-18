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
#define SPACE 100


class Scale {
public:
	Scale() :vazio(true) {}
	bool vazio;
	float x, y, z;
};

class Rotation {
public:
	Rotation() :vazio(true), angulo(-1), time(-1) {}
	bool vazio;
	float x, y, z, angulo, time;
};

class Translate {
public:
	Translate() :vazio(true) {}
	bool vazio;
	float time;
	float** pontos;
	int numPontos;
	float** matrix;
};

class Figure {
public:
	Figure() : vboIndice(-1), triangulos(0) {}
	string name;
	int vboIndice, triangulos;
	Scale scale;
	Rotation rotate;
	Translate translate;

};

class Tree {
public:
	Figure modelo;
	std::vector<Tree> subArvores;
};

std::vector<pair<string, pair<int, int>>> figureIndexes;

Tree arvore;
Translate trans;

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float coordCamX, coordCamY, coordCamZ;
float rodaI = 0.0;
float time = 0;
static float p;

float up[3] = { 0.0,1.0,0.0 };
float leftCometa[3];

pair<int, int> loadFigure(string object) {
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

void getCatmullRomPoint(float t, int *indices, float *res, float *deriv) {

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
		res[i] = (((pow(t, 3)*m[0][0] + pow(t, 2)*m[1][0] + t*m[2][0] + m[3][0]) * (trans.pontos)[indices[0]][i])
			+ ((pow(t, 3)*m[0][1] + pow(t, 2)*m[1][1] + t*m[2][1] + m[3][1])*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 3)*m[0][2] + pow(t, 2)*m[1][2] + t*m[2][2] + m[3][2])*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 3)*m[0][3] + pow(t, 2)*m[1][3] + t*m[2][3] + m[3][3])*(trans.pontos)[indices[3]][i]));

		deriv[i] = (((pow(t, 2)*m[0][0] * 3 + pow(t, 1)*m[1][0] * 2 + m[2][0] + 0)*(trans.pontos)[indices[0]][i])
			+ ((pow(t, 2)*m[0][1] * 3 + pow(t, 1)*m[1][1] * 2 + m[2][1] + 0)*(trans.pontos)[indices[1]][i])
			+ ((pow(t, 2)*m[0][2] * 3 + pow(t, 1)*m[1][2] * 2 + m[2][2] + 0)*(trans.pontos)[indices[2]][i])
			+ ((pow(t, 2)*m[0][3] * 3 + pow(t, 1)*m[1][3] * 2 + m[2][3] + 0)*(trans.pontos)[indices[3]][i]));
	}
}

void getGlobalCatmullRomPoint(float gt, float *res, float *deriv) {

	float t = gt * (trans.numPontos); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

				   // indices store the pontos
	int indices[4];
	indices[0] = (index + (trans.numPontos) - 1) % (trans.numPontos);
	indices[1] = (indices[0] + 1) % (trans.numPontos);
	indices[2] = (indices[1] + 1) % (trans.numPontos);
	indices[3] = (indices[2] + 1) % (trans.numPontos);
	getCatmullRomPoint(t, indices, res, deriv);
}


void toMatrix(Translate t) {
	trans = t;

	t.matrix[0] = (float*)malloc(sizeof(float) * 2);
	t.matrix[0][0] = 0; // t=0;
	t.matrix[0][1] = 0; // distancia=0;

	float tt = 0, pAnterior[3], deriv[3], point[3];
	getGlobalCatmullRomPoint(tt, pAnterior, deriv);

	for (int i = 1; i < (int)t.numPontos * SPACE + 1; i++) {

		t.matrix[i] = (float*)malloc(sizeof(float) * 2);
		tt = (float)i / (float)(t.numPontos*SPACE);
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

Tree getGroup(XMLElement* node) {

	Tree t;
	XMLElement* child = node->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement()) {
		string tag = child->Value();

		if (strcmp(tag.c_str(), "translate") == 0) {
			t.modelo.translate.vazio = false;
			t.modelo.translate.time = child->IntAttribute("time");

			vector<float> pts;

			XMLElement *pModel = child->FirstChildElement();

			for (; pModel != NULL; pModel = pModel->NextSiblingElement()) {
				pts.push_back(pModel->FloatAttribute("X"));
				pts.push_back(pModel->FloatAttribute("Y"));
				pts.push_back(pModel->FloatAttribute("Z"));
			}

			t.modelo.translate.numPontos = pts.size() / 3;
			t.modelo.translate.pontos = (float**)malloc(sizeof(float*) * t.modelo.translate.numPontos);
			t.modelo.translate.matrix = (float**)malloc(sizeof(float*) * t.modelo.translate.numPontos*SPACE + 1);

			vector<float>::iterator itpts;
			int aux = 0;

			for (itpts = pts.begin(); itpts != pts.end(); itpts++) {
				t.modelo.translate.pontos[aux] = (float*)malloc(sizeof(float) * 3);

				t.modelo.translate.pontos[aux][0] = *itpts;
				itpts++;
				t.modelo.translate.pontos[aux][1] = *itpts;
				itpts++;
				t.modelo.translate.pontos[aux][2] = *itpts;
				aux++;
			}
			toMatrix(t.modelo.translate);
		}
		else if (strcmp(tag.c_str(), "rotate") == 0) {
			t.modelo.rotate.x = child->DoubleAttribute("axisX");
			t.modelo.rotate.y = child->DoubleAttribute("axisY");
			t.modelo.rotate.z = child->DoubleAttribute("axisZ");
			if (child->DoubleAttribute("angulo"))
				t.modelo.rotate.angulo = child->DoubleAttribute("angulo");
			else if (child->DoubleAttribute("time"))
				t.modelo.rotate.time = child->DoubleAttribute("time");
		}
		else if (strcmp(tag.c_str(), "scale") == 0) {
			t.modelo.scale.vazio = false;
			t.modelo.scale.x = child->DoubleAttribute("X");
			t.modelo.scale.y = child->DoubleAttribute("Y");
			t.modelo.scale.z = child->DoubleAttribute("Z");
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
			newTree = getGroup(child);
			if (child->Attribute("a") != nullptr)
				newTree.modelo.name = child->Attribute("a");
			else
				newTree.modelo.name = "nada";
			t.subArvores.push_back(newTree);
		}
	}
	return t;
}

void getFigures() {
	XMLDocument doc;						   // vector que vai conter nome das figuras presentes no ficheiro XML
	XMLError load = doc.LoadFile("scene.xml"); // abre ficheiro XML
											   // se conseguiu abrir o ficheiro vai colocar no vetor o nome  das figuras a carregar
	if (load != XML_SUCCESS) {
		printf("Erro no ficheiro xml.\n");
		return;
	}
	XMLElement *pRoot = doc.FirstChildElement("scene");
	if (pRoot == nullptr) return;
	arvore = getGroup(pRoot);

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
	}
	glEnd();
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {
	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0; m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0; m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0; m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]); a[0] = a[0] / l; a[1] = a[1] / l; a[2] = a[2] / l;
}

void drawScene(Tree t) {

	glPushMatrix();
	if (!t.modelo.translate.vazio) {
		trans = t.modelo.translate;
		renderCatmullRomCurve();
		time = glutGet(GLUT_ELAPSED_TIME);
		float aux = fmod(time, (float)(t.modelo.translate.time * 1000)) / (t.modelo.translate.time * 1000); // 1000 is the number of catmull-rom's division
		float dist = aux*t.modelo.translate.matrix[(int)(t.modelo.translate.numPontos)*SPACE][1];

		for (int i = 0; i < (int)(t.modelo.translate.numPontos)*SPACE + 1; i++) {
			if (dist == t.modelo.translate.matrix[i][1])
				p = t.modelo.translate.matrix[i][0];
			else if (dist > t.modelo.translate.matrix[i][1] && dist < t.modelo.translate.matrix[i + 1][1]) {
				float f = (float)(dist - t.modelo.translate.matrix[i][1]) / (t.modelo.translate.matrix[i + 1][1] - t.modelo.translate.matrix[i][1]);
				p = t.modelo.translate.matrix[i][0] * (1 - f) + (t.modelo.translate.matrix[i + 1][0] * f);
			}
		}

		float ponto[3], deriv[3];
		float m[16];
		getGlobalCatmullRomPoint(p, ponto, deriv);

		glTranslatef(ponto[0], ponto[1], ponto[2]);

		if (strcmp(t.modelo.name.c_str(), "Cometa") == 0) {
			
			cross(deriv, up, leftCometa);
			cross(leftCometa, deriv, up);
			normalize(leftCometa);
			normalize(up);
			normalize(deriv);

			buildRotMatrix(deriv, up, leftCometa, m);
			glMultMatrixf(m);
		}
	}

	if (!t.modelo.rotate.vazio)
		if (t.modelo.rotate.angulo != -1)
			glRotatef(t.modelo.rotate.angulo, t.modelo.rotate.x, t.modelo.rotate.y, t.modelo.rotate.z);
		else if (t.modelo.rotate.time != -1) {
			time = glutGet(GLUT_ELAPSED_TIME);
			float aux = fmod(time, (float)(t.modelo.rotate.time * 1000)) / (t.modelo.rotate.time * 1000);
			float angulo = aux * 360;
			glRotatef(angulo, t.modelo.rotate.x, t.modelo.rotate.y, t.modelo.rotate.z);
		}
	if (!t.modelo.scale.vazio)
		glScalef(t.modelo.scale.x, t.modelo.scale.y, t.modelo.scale.z);

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
	coordCamX = radius * cos(beta) * sin(alfa);
	coordCamY = radius * sin(beta);
	coordCamZ = radius * cos(beta) * cos(alfa);
}

void renderScene(void) {
	// clear buffers

	glClearColor(0, 0, 0.05, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	spherical2Cartesian();

	// set the camera
	glLoadIdentity();
	gluLookAt(coordCamX, coordCamY, coordCamZ,
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
	getFigures();
	glutMainLoop();

	return 1;
}