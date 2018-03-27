﻿
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
using engine::modelTree;

modelTree engine::scene;

void engine::loadScene(modelTree scene) {
	engine::scene = scene;
}

void engine::drawFrame() {
	// Call drawScene (required because it's recursive)
	engine::drawScene(engine::scene);
}

void engine::drawScene(modelTree tree) {
	// Backup previous transformations
	// This saves current transformations, but doesn't reset them,
	// so transformations done on previous recursive calls are still applied
	glPushMatrix();

	// Apply transformations
	// These transformations will be applied to all models of the current group,
	// as well as models of all sub trees, unless overriden.
	if (tree.applyTranslate) {
		glTranslatef(tree.translate.x, tree.translate.y, tree.translate.z);
	}
	if (tree.applyRotate) {
		glRotatef(tree.rotateAngle, tree.rotate.x, tree.rotate.y, tree.rotate.z);
	}
	if (tree.applyScale) {
		glScalef(tree.scale.x, tree.scale.y, tree.scale.z);
	}

	// Draw all models of current tree node
	std::vector<model>::iterator i;
	for (i = tree.models.begin(); i != tree.models.end(); i++) {
		drawModel(*i);
	}

	// Draw all sub trees
	std::vector<modelTree>::iterator j;
	for (j = tree.branches.begin(); j != tree.branches.end(); j++) {
		drawScene(*j);
	}

	// Reset transformations
	// This takes us back to the transformations state
	// of the previous model group
	glPopMatrix();
}

void engine::drawModel(model m) {
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0.2);
	int color = 0;

	// Iterate over vertices
	std::vector<vertex> vertices = m.vertices;
	std::vector<vertex>::iterator j;
	for (j = vertices.begin(), color = 0; j != vertices.end(); j++, color++) {

		// Vary colors
		if (color % 3 == 0) {
			glColor3f(0.6, 0.6, 0.5);
			if (color % 6 == 0) {
				glColor3f(0.6, 1, 0.9);
			}
		}
		glVertex3f(j->x, j->y, j->z); // Get vertex coordinates by dereferencing pointer
	}
	glEnd();
}

/* Parte 2

void drawFigure(std::vector<engine::model> figura) {

	std::vector<std::vector<Coordinate>>::iterator it_fig;

	for (it_fig = figura.begin(); it_fig != figura.end(); it_fig++) {
		std::vector<Coordinate>::iterator it = it_fig->begin();

		glBegin(GL_TRIANGLES);
		int color = 0;
		glColor3f(0, 0, 0.2);

		for (; it != it_fig->end(); it++) {

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

void drawScene(modelTree tree) {


}
*/
