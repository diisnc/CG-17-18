#include <vector>
#include <string>
#include "xml-loader.h"
#include "engine.h"

using engine::vertex;

/**
 * Loads an XML file containing various references
 * to .3d files, which in turn contain vertices.
 * Returns a vector of models, which in turn are a vector of vertices,
 * which in turn are structs of x, y and z integers (representing coordinates).
 */
std::vector<vertex> xmlLoader::loadXML(std::string path) {

}