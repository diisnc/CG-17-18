#pragma once
#ifndef	XMLLOADER_H
#define XMLLOADER_H

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
#include "engine.h"
using namespace tinyxml2;
using namespace std;

struct Tree {
	Figure modelo;
	std::vector<Tree> subArvores;
};

extern Translate trans;
extern Tree arvore;

void getPontoCatmullRom(float t, int *indices, float *res, float *deriv);
void getPontoGlobalCatmullRom(float gt, float *res, float *deriv);
void loadSceneXML();
Tree getTreeFROMXML(XMLElement* node);

#endif
