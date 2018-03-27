#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "engine.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using std::string;
using std::ifstream;
#include "xml-loader.h"
// The order of the includes above was carefully constructed.
// If you move the "using namespace"s, errors start appearing!

/**
 * Loads an XML file containing various references
 * to .3d files, alongside possible transformations,
 * all of these organized by groups.
 * Returns a tree of models, which emulates the original
 * hierarchy of the XML document.
 */
engine::modelTree xmlLoader::loadSceneXML(const char* path) {

  // Load XML document
  XMLDocument file;
  XMLError result = file.LoadFile(path);

  if (result != XML_SUCCESS) {
    throw std::runtime_error("O documento XML não foi carregado com sucesso.\n");
  }

  XMLElement *scene = file.FirstChildElement("scene");
  if (scene == nullptr) {
    throw std::runtime_error("Erro ao procurar elemento \"scene\" no XML.\n");
  }

  engine::modelTree modelTree = getModelTreeFromXML(scene);

  return modelTree;
}

/**
 * Returns a tree of models from an XML node.
 *
 * The root parameter is the XML node of a group, such as `<group name="Sun">`.
 *
 *  This group can contain:
 * - <translate>, <rotate>, or <scale>: transformations that
 *   apply to all models in the group (that appear below the
 *   transformations) and its subgroups;
 * - <models>: a list of the models included in the group;
 * - <group>: a subgroup that can include any of the previously
 *   mentioned elements, including other groups;
 */
engine::modelTree xmlLoader::getModelTreeFromXML(XMLElement* root) {

  // Tree of models that will be populated
  engine::modelTree tree;

  tree.applyTranslate = false;
  tree.applyRotate = false;
  tree.applyScale = false;

  // Loop through all children of this <group>
  XMLElement* child = root->FirstChildElement();
  while (child != nullptr) {
    // Get name of tag - can be "models", "translate", "rotate", "scale", or "group"
    string tag = child->Value();

    if (strcmp(tag.c_str(), "translate") == 0) {
      tree.applyTranslate = true;
      tree.translate.x = child->DoubleAttribute("x");
      tree.translate.y = child->DoubleAttribute("y");
      tree.translate.z = child->DoubleAttribute("z");
    } else if (strcmp(tag.c_str(), "rotate") == 0) {
      tree.applyRotate = true;
      tree.rotate.x = child->DoubleAttribute("axisX");
      tree.rotate.y = child->DoubleAttribute("axisY");
      tree.rotate.z = child->DoubleAttribute("axisZ");
      tree.rotateAngle = child->DoubleAttribute("angle");
    } else if (strcmp(tag.c_str(), "scale") == 0) {
      tree.applyScale = true;
      tree.scale.x = child->DoubleAttribute("x");
      tree.scale.y = child->DoubleAttribute("y");
      tree.scale.z = child->DoubleAttribute("z");
    } else if (strcmp(tag.c_str(), "models") == 0) {
      XMLElement* models = child->FirstChildElement();

      while (models != NULL) {
        string filename = models->Attribute("file");
        engine::model model = loadModel(filename);
        tree.models.push_back(model);

        models = models->NextSiblingElement();
      }
    } else if (strcmp(tag.c_str(), "group") == 0) {
      // Found a new group, make recursive call
      engine::modelTree newTree = getModelTreeFromXML(child);
      newTree.name = child->Attribute("name");
      tree.branches.push_back(newTree);
    }

    child = child->NextSiblingElement();
  }

  return tree;
}

// Loads a model from a given file path
// Example path: banana.3d
engine::model xmlLoader::loadModel(string path) {

  ifstream modelFile;
  modelFile.open(path);
  engine::model newModel;

  while (!modelFile.eof()) {
    engine::vertex vertex;
    modelFile >> vertex.x >> vertex.y >> vertex.z;
    newModel.vertices.push_back(vertex);
  }
  return newModel;
}
