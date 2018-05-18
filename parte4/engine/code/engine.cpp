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

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float camX, camY, camZ;
float rodaI = 0.0;
float time = 0;
static float p;

float up[3] = { 0.0, 1.0, 0.0 };
float leftCometa[3];

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
		glNormal3fv(res);
	}
	glEnd();
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {
	m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = 0;
	m[4] = y[0];
	m[5] = y[1];
	m[6] = y[2];
	m[7] = 0;
	m[8] = z[0];
	m[9] = z[1];
	m[10] = z[2];
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

void cross(float *a, float *b, float *res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void drawScene(Tree t) {

	glPushMatrix();
	if (!t.figure.translate.vazio) {

		if (t.figure.translate.tempo != 0) {
			trans = t.figure.translate;
			float color[4] = { 1.0, 1.0, 1.0, 1.0 };

			renderCatmullRomCurve();
			time = glutGet(GLUT_ELAPSED_TIME);
			float aux = fmod(time, (float)(t.figure.translate.tempo * 1000)) / (t.figure.translate.tempo * 1000);
			float dist = aux * t.figure.translate.matrix[(int)(t.figure.translate.numPointers) * SPACE][1];

			for (int i = 0; i < (int)(t.figure.translate.numPointers) * SPACE + 1; i++) {
				if (dist == t.figure.translate.matrix[i][1])
					p = t.figure.translate.matrix[i][0];
				else if (dist > t.figure.translate.matrix[i][1] && dist < t.figure.translate.matrix[i + 1][1]) {
					float f = (float)(dist - t.figure.translate.matrix[i][1]) / (t.figure.translate.matrix[i + 1][1] - t.figure.translate.matrix[i][1]);
					p = t.figure.translate.matrix[i][0] * (1 - f) + (t.figure.translate.matrix[i + 1][0] * f);
				}
			}

			float ponto[3], deriv[3];
			float m[16];
			getGlobalCatmullRomPoint(p, ponto, deriv);

			glTranslatef(ponto[0], ponto[1], ponto[2]);

			if (strcmp(t.figure.name.c_str(), "Cometa") == 0) {

				cross(deriv, up, leftCometa);
				cross(leftCometa, deriv, up);
				normalize(leftCometa);
				normalize(up);
				normalize(deriv);

				buildRotMatrix(deriv, up, leftCometa, m);
				glMultMatrixf(m);
			}
		}
		else {
			glTranslatef(t.figure.translate.points[0][0], t.figure.translate.points[0][1], t.figure.translate.points[0][3]);
		}
	}

	if (t.figure.color.ambientColor != NULL) {
		//glMaterialfv(GL_FRONT, GL_AMBIENT, t.figure.color.ambientColor);
		//printf("vbo index : %d", t.figure.vboIndex);
	}

	if (t.figure.color.diffuseColor != NULL)
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, t.figure.color.diffuseColor);

		if (t.figure.color.emissiveColor != NULL)
			//glMaterialfv(GL_FRONT, GL_EMISSION, t.figure.color.emissiveColor);

			if (t.figure.color.specularColor != NULL)
				//glMaterialfv(GL_FRONT, GL_SPECULAR, t.figure.color.specularColor);

				if (!t.figure.rotate.vazio)
					if (t.figure.rotate.angulo != -1)
						glRotatef(t.figure.rotate.angulo, t.figure.rotate.x, t.figure.rotate.y, t.figure.rotate.z);
					else if (t.figure.rotate.tempo != -1) {
						time = glutGet(GLUT_ELAPSED_TIME);
						float aux = fmod(time, (float)(t.figure.rotate.tempo * 1000)) / (t.figure.rotate.tempo * 1000);
						float angle = aux * 360;
						glRotatef(angle, t.figure.rotate.x, t.figure.rotate.y, t.figure.rotate.z);
					}
					if (!t.figure.scale.vazio)
						glScalef(t.figure.scale.x, t.figure.scale.y, t.figure.scale.z);

					vector<Light>::iterator i = t.lights.begin();
					for (; i != t.lights.end(); i++) {

						glLightfv(GL_LIGHT0 + i->id, GL_POSITION, i->pos);

						if (i->color.ambientColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_AMBIENT, i->color.ambientColor);
						}

						if (i->color.diffuseColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_DIFFUSE, i->color.diffuseColor);
						}

						if (i->color.emissiveColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_EMISSION, i->color.emissiveColor);
						}

						if (i->color.specularColor) {
							glLightfv(GL_LIGHT0 + i->id, GL_SPECULAR, i->color.specularColor);
						}

						if (strcmp(i->type.c_str(), "POINT") == 0) {

							if (i->cons) {
								float arr[1] = { i->cons };

								glLightfv(GL_LIGHT0 + i->id, GL_CONSTANT_ATTENUATION, arr);
							}

							if (i->linear) {
								float arr[1] = { i->linear };
								glLightfv(GL_LIGHT0 + i->id, GL_LINEAR_ATTENUATION, arr);
							}

							if (i->quad) {
								float arr[1] = { i->quad };
								glLightfv(GL_LIGHT0 + i->id, GL_QUADRATIC_ATTENUATION, arr);
							}
						}

						if (strcmp(i->type.c_str(), "SPOTLIGHT") == 0) {
							if (i->spotCutOff) {
								float arr[1] = { i->spotCutOff };
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_CUTOFF, arr);
							}

							if (i->spotDirection) {
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_DIRECTION, i->spotDirection);
							}

							if (i->spotExponent) {
								float arr[1] = { i->spotExponent };
								glLightfv(GL_LIGHT0 + i->id, GL_SPOT_EXPONENT, arr);
							}
						}
					}

					if (t.figure.vboIndex != -1) {

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.vboIndex);
						glVertexPointer(3, GL_FLOAT, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.normalIndex);
						glNormalPointer(GL_FLOAT, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, t.figure.textureIndexVBO);
						glTexCoordPointer(2, GL_FLOAT, 0, 0);
						glBindTexture(GL_TEXTURE_2D, t.figure.textureIndex);

						glDrawArrays(GL_TRIANGLES, 0, t.figure.triangles * 3);
						glBindTexture(GL_TEXTURE_2D, 0);
					}

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
	glLoadIdentity();
	glutSwapBuffers();
}
