#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
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


class Coordinate {
	public:
		Coordinate() :empty(true) {}
		bool empty;
		float x, y, z, angle;
};

class Figure {
public:
	string name;
	std::vector<std::vector<Coordinate>> figura;
	Coordinate rotate, translate, scale;
	int triangles; //actualmente a não ser usado
};

	class Tree {
		public:
			Figure figure;
			std::vector<Tree> subtrees;
	};

Tree tree;

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float camX, camY, camZ;
float rodaI = 0.0;

std::vector<Coordinate> loadFigure(string object) {
	ifstream file;
	std::vector<Coordinate> newFig;
	file.open(object);

	int nTriangles;
	file >> nTriangles;
	while (!file.eof()) {
		Coordinate newC;
		file >> newC.x >> newC.y >> newC.z;
		newFig.push_back(newC);

	}
	return newFig;
}

Tree getGroup(XMLElement* node) {
	
	Tree t;
	XMLElement* child = node->FirstChildElement(); 

	for (; child != nullptr; child = child->NextSiblingElement()) {
		string tag = child->Value();

		if (strcmp(tag.c_str(), "translate") == 0) {
			t.figure.translate.empty = false;
			t.figure.translate.x = child->DoubleAttribute("X");
			t.figure.translate.y = child->DoubleAttribute("Y");
			t.figure.translate.z = child->DoubleAttribute("Z");
		}
		else if (strcmp(tag.c_str(), "rotate") == 0) {
			t.figure.rotate.empty = false;
			t.figure.rotate.x = child->DoubleAttribute("axisX");
			t.figure.rotate.y = child->DoubleAttribute("axisY");
			t.figure.rotate.z = child->DoubleAttribute("axisZ");
			t.figure.rotate.angle = child->DoubleAttribute("angle");
		}
		else if (strcmp(tag.c_str(), "scale") == 0) {
			t.figure.scale.empty = false;
			t.figure.scale.x = child->DoubleAttribute("X");
			t.figure.scale.y = child->DoubleAttribute("Y");
			t.figure.scale.z = child->DoubleAttribute("Z");
		}
		else if (strcmp(tag.c_str(), "models") == 0) {
			XMLElement* modelsNode = child->FirstChildElement();
			for (; modelsNode != NULL; modelsNode = modelsNode->NextSiblingElement()) {
				string figureName = modelsNode->Attribute("file");
				std::vector<Coordinate> newFig = loadFigure(figureName);
				t.figure.figura.push_back(newFig);
			}	
		}
		else if (strcmp(tag.c_str(), "group") == 0) {
			Tree newTree = getGroup(child);
			newTree.figure.name = child->Attribute("a");
			t.subtrees.push_back(newTree);
		}
	}

	return t;
}

void getFigures() {
	XMLDocument doc;						   //vector que vai conter nome das figuras presentes no ficheiro XML
	XMLError load = doc.LoadFile("scene.xml"); //abre ficheiro XML
											   //se conseguiu abrir o ficheiro vai colocar no vetor o nome  das figuras a carregar
	if (load != XML_SUCCESS) {
		printf("Erro no ficheiro xml.\n");
		return;
	}
	XMLElement *pRoot = doc.FirstChildElement("scene");
	if (pRoot == nullptr) return;
	tree = getGroup(pRoot);

}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawFigure(std::vector<std::vector<Coordinate>> figura) {

	std::vector<std::vector<Coordinate>>::iterator it_fig;

	for (it_fig = figura.begin(); it_fig != figura.end(); it_fig++) {
		std::vector<Coordinate>::iterator it = it_fig->begin();

		glBegin(GL_TRIANGLES);
		int color = 0;
		glColor3f(0, 0, 0.2);

		for(;it!=it_fig->end();it++){

			if (color % 3 == 0) {
				glColor3f(0.6, 0.6, 0.6);
				if (color % 6 == 0) glColor3f(0.7, 0.7, 0.7);
			}

			glVertex3f(it->x, it->y, it->z);
			color++;
		}

		glEnd();
	}
}

void drawScene(Tree t) {

	glPushMatrix();

	if (!t.figure.translate.empty)
		glTranslatef(t.figure.translate.x, t.figure.translate.y, t.figure.translate.z);
	if (!t.figure.rotate.empty)
		glRotatef(t.figure.rotate.angle, t.figure.rotate.x, t.figure.rotate.y, t.figure.rotate.z);
	if (!t.figure.scale.empty)
		glScalef(t.figure.scale.x, t.figure.scale.y, t.figure.scale.z);

	drawFigure(t.figure.figura);


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

	glBegin(GL_LINE_STRIP); // eixo dos yy
	glVertex3f(0.0f, 20.0f, 0.0f);
	glVertex3f(0.0f, -20.0f, 0.0f);
	glEnd();
	
	glBegin(GL_LINE_STRIP); // eixo dos zz
	glVertex3f(0.0f, 0.0f, 20.0f);
	glVertex3f(0.0f, 0.0f, -20.0f);
	glEnd();

	glBegin(GL_LINE_STRIP); // eixo dos xx
	glVertex3f(20.0f, 0.0f, 0.0f);
	glVertex3f(-20.0f, 0.0f, 0.0f);
	glEnd();
	
	// End of frame
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
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920,1080);
	glutCreateWindow("engine 1.2");
		
// Required callback registry 
	getFigures();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}