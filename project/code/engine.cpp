
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "engine.h"

// Glut has to be included last to avoid errors
// (for example, C2381 'exit': redefinition; '__declspec(noreturn)' or '[[noreturn]]' differs)
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using std::vector;
using engine::model;
using engine::vertex;

vector<model> engine::scene;

void engine::loadScene(vector<model> scene) {
	engine::scene = scene;
}

void engine::drawFrame() {

	// Iterate over models
	vector<model>::iterator i;
	for (i = scene.begin(); i != scene.end(); i++) {
		std::vector<vertex> vertices = i->vertices;

		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 0.2);
		int color = 0;

		// Iterate over vertices
		std::vector<vertex>::iterator j;
		for (j = vertices.begin(), color = 0; j != vertices.end(); j++, color++) {

			// Vary colors
			if (color % 3 == 0) {
				glColor3f(0, 0, 0.4);
				if (color % 6 == 0) {
					glColor3f(0, 0, 0.2);
				}
			}
			glVertex3f(j->x, j->y, j->z); // Get vertex coordinates by dereferencing pointer
		}
		glEnd();
	}
}
