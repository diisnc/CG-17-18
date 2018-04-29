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

	case GLUT_KEY_F1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;

	case GLUT_KEY_F2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;

	case GLUT_KEY_F3: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;

	default:
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
	loadSceneXML();
	glutMainLoop();

	return 1;
}