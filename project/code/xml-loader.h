#pragma once
#ifndef	XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <string>
#include "engine.h"

namespace xmlLoader {
	std::vector<engine::model> loadSceneXML(const char* path);
};

#endif
