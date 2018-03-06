#include "engine.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

void engine::drawFrame() {
	// Draw cilinder
	int slices = 10;
	int height = 2;
	int radius = 1;
	float alpha = 2 * M_PI / slices;

	for (int slice = 0; slice <= slices; slice++) {
		// BOTTOM
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0); // red
		glVertex3f(0.0f, -height / 2, 0.0f);    // centre
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * slice), -height / 2, radius * cos(alpha * slice));
		glEnd();

		// TOP
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1); // blue
		glVertex3f(0.0f, height / 2, 0.0f); // centre
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * (slice + 1)), height / 2, radius * cos(alpha * (slice + 1)));
		glEnd();

		// SIDES
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 0);
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * slice), -height / 2, radius * cos(alpha * slice));
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0, 1, 1);
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * (slice + 1)), height / 2, radius * cos(alpha * (slice + 1)));
		glEnd();

		glColor3f(0, 1, 1);
		glutWireTeapot(2);
	}
};
