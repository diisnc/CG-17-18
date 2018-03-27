#pragma once
#ifndef	XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <string>
#include "engine.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using std::string;

namespace xmlLoader {
	engine::modelTree loadSceneXML(const char* path);
	engine::modelTree getModelTreeFromXML(XMLElement* root);
	engine::model loadModel(string path);
};

#endif
