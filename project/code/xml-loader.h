#pragma once
#ifndef	XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <string>
#include "engine.h"

using engine::vertex;

namespace xmlLoader {
	std::vector<vertex> loadXML(std::string path);
};

#endif
