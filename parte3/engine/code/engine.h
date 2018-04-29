#pragma once
#ifndef	ENGINE_H
#define ENGINE_H

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
using namespace tinyxml2;
using namespace std;

struct Scale {
	bool vazio = true;
	float x, y, z;
};

struct Rotation {
	bool vazio = true;
	float x, y, z, angulo = -1, tempo = -1;
};

struct Translate {
	bool vazio = true;
	float tempo;
	float** pontos;
	int numPontos;
	float** matrix;
};

struct Figure {
	string nome;
	int vboIndice = -1, triangulos = 0;
	Scale escala;
	Rotation rotacao;
	Translate translacao;
};

extern float dx;
extern float dy;
extern float dz;
extern float alfa, beta, radius;
extern float coordCamX, coordCamY, coordCamZ;
extern float rodaI;
extern float tempo;
extern float p;

void spherical2Cartesian();
void renderScene(void);
void changeSize(int w, int h);

#endif
