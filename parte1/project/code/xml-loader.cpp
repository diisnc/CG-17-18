#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "xml-loader.h"
#include "engine.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using std::string;
using std::ifstream;

/**
 * Loads an XML file containing various references
 * to .3d files, which in turn contain vertices.
 * Returns a vector of models, which in turn are a vector of vertices.
 */
std::vector<engine::model> xmlLoader::loadSceneXML(const char* path) {

	// Load XML document
	XMLDocument file;
	XMLError result = file.LoadFile(path);

	if (result != XML_SUCCESS) {
		throw std::runtime_error("O documento XML não foi carregado com sucesso.\n");
	}

	XMLNode *scene = file.FirstChildElement("scene");
	if (scene == nullptr) {
		throw std::runtime_error("Erro ao procurar elemento \"scene\" no XML.\n");
	}

	// Will contain paths to .3d models present in XML
	std::vector<string> modelPaths;

	XMLElement *models = scene->FirstChildElement("model");
	while (models != nullptr) {
		string newModel = models->Attribute("file");  // Get model file path
		modelPaths.push_back(newModel);               // Put path on model paths vector
		models = models->NextSiblingElement("model"); // Get next model
	}

	// Loop through model paths and load models

	std::vector<engine::model> loadedModels;

	for (auto p : modelPaths) {
		ifstream modelFile;
		modelFile.open(p);
		engine::model newModel;

		while (!modelFile.eof()) {
			engine::vertex vertex;
			modelFile >> vertex.x >> vertex.y >> vertex.z;
			newModel.vertices.push_back(vertex);
		}
		loadedModels.push_back(newModel);
	}
	return loadedModels;
}
