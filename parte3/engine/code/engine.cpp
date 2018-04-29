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

float dx = 0;
float dy = 0;
float dz = 0;
float alfa = 0.0f, beta = 0.5f, radius = 15.0f;
float coordCamX, coordCamY, coordCamZ;
float rodaI = 0.0;
float tempo = 0;
static float p;

float up[3] = { 0.0,1.0,0.0 };
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

void renderCurvaCatmullRom() {

	float res[3];
	float deriv[3];
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++) {
		getPontoGlobalCatmullRom(i / 1000.0, res, deriv);
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
	if (!t.modelo.translacao.vazio) {
		trans = t.modelo.translacao;
		renderCurvaCatmullRom();
		tempo = glutGet(GLUT_ELAPSED_TIME);
		float aux = fmod(tempo, (float)(t.modelo.translacao.tempo * 1000)) / (t.modelo.translacao.tempo * 1000); // 1000 is the number of catmull-rom's division
		float dist = aux * t.modelo.translacao.matrix[(int)(t.modelo.translacao.numPontos)*ESPACO][1];

		for (int i = 0; i < (int)(t.modelo.translacao.numPontos)*ESPACO + 1; i++) {
			if (dist == t.modelo.translacao.matrix[i][1])
				p = t.modelo.translacao.matrix[i][0];
			else if (dist > t.modelo.translacao.matrix[i][1] && dist < t.modelo.translacao.matrix[i + 1][1]) {
				float f = (float)(dist - t.modelo.translacao.matrix[i][1]) / (t.modelo.translacao.matrix[i + 1][1] - t.modelo.translacao.matrix[i][1]);
				p = t.modelo.translacao.matrix[i][0] * (1 - f) + (t.modelo.translacao.matrix[i + 1][0] * f);
			}
		}

		float ponto[3], deriv[3];
		float m[16];
		getPontoGlobalCatmullRom(p, ponto, deriv);

		glTranslatef(ponto[0], ponto[1], ponto[2]);

		if (strcmp(t.modelo.nome.c_str(), "Cometa") == 0) {

			cross(deriv, up, leftCometa);
			cross(leftCometa, deriv, up);
			normalize(leftCometa);
			normalize(up);
			normalize(deriv);

			buildRotMatrix(deriv, up, leftCometa, m);
			glMultMatrixf(m);
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
