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
		float x, y, z;
};


class Figure {
	public:
		std::vector<Coordinate> figura;
		int triangles; //actualmente a não ser usado
};

float px = 0;
float py = 0;
float pz = 0;

float dx = 0;
float dy = 0;
float dz = 0;

float alpha = 0;
float beta = 0;
float radium = 7.0;


std::vector<Figure> figures;

void getFigures() {
	XMLDocument doc;
	std::vector<string> figuresToLoad; //vector que vai conter nome das figuras presentes no ficheiro XML
	XMLError load = doc.LoadFile("scene.xml"); //abre ficheiro XML
	//se conseguiu abrir o ficheiro vai colocar no vetor o nome  das figuras a carregar
	if (load != XML_SUCCESS) {
		printf("Erro no ficheiro xml.\n");
		return;
	}

		XMLNode *pRoot = doc.FirstChildElement("scene"); 
		if (pRoot == nullptr) return;

		XMLElement *sceneFigures = pRoot->FirstChildElement("model");
		for (; sceneFigures != nullptr; sceneFigures = sceneFigures->NextSiblingElement("model")) {
			string newFigure = sceneFigures->Attribute("file");
			figuresToLoad.push_back(newFigure);
		}


	for (auto i: figuresToLoad) {
		ifstream file;
		Figure newFig;
		file.open(i);
		
		int nTriangles;
		file >> nTriangles;
		while (!file.eof()) {
			Coordinate newC;
			file >> newC.x >> newC.y >> newC.z;
			newFig.figura.push_back(newC);
		}
				
		figures.push_back(newFig);
	}
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

void drawFigures() {

	std::vector<Figure>::iterator it_fig;
	
	for (it_fig = figures.begin(); it_fig != figures.end(); it_fig++){
		std::vector<Coordinate> crdnt = it_fig->figura;
		std::vector<Coordinate>::iterator it;

		glBegin(GL_TRIANGLES);
		int color;
		glColor3f(0, 0, 0.2);
		
		for (it = crdnt.begin(), color = 0; it != crdnt.end(); it++, color++) {
			
			if (color % 3 == 0) {
				glColor3f(0, 0, 0.4);
				if (color%6 == 0) glColor3f(0, 0, 0.2);
			}

			glVertex3f(it->x, it->y, it->z);
		}
		glEnd();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	px = radium*cosf(beta)*sinf(alpha);
	py = radium*sinf(beta);
	pz = radium*cosf(alpha)*cosf(beta);
	gluLookAt(px, py, pz,
		dx, dy, dz,
		0.0f, 1.0f, 0.0f);



	getFigures();
	drawFigures();

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c)
	{
	case 'd': dx += 0.1;
		break;
	case 'a': dx -= 0.1;
		break;
	case 'w': dy += 0.1;
		break;
	case 's': dy -= 0.1;
		break;
	case 'q': dz -= 0.1;
		break;
	case 'e': dy += 0.1;
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
	glutPostRedisplay();
}


void processSpecialKeys(int key_code, int xx, int yy) {
	switch (key_code)
	{
	case GLUT_KEY_RIGHT: alpha += M_PI / 16;
		break;
	case GLUT_KEY_LEFT: alpha -= M_PI / 16;
		break;
	case GLUT_KEY_UP:
		if (beta + 0.05 >= 1.5)
			beta = 1.5;
		else
			beta += 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (beta - 0.05 <= -1.5)
			beta = -1.5;
		else
			beta -= 0.05;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
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
