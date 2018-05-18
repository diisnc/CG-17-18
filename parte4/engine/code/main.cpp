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
#include "engine.h"
using namespace tinyxml2;
using namespace std;

#define _USE_MATH_DEFINES
#define SPACE 100

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
	loadSceneXML();
	glutMainLoop();

	return 1;
}
