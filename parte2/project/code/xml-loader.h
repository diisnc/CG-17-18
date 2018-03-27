#pragma once
#ifndef	XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <string>
#include "engine.h"

namespace xmlLoader {
	engine::modelTree loadSceneXML(const char* path);
	engine::modelTree xmlLoader::getModelTreeFromXML(XMLElement* root);
	engine::model loadModel(string path);
};

#endif
