#pragma once
#ifndef	ENGINE_H
#define ENGINE_H

#include <vector>

namespace engine {

	// A vertex has x y and z coordinates
	struct vertex {
		float x, y, z;
	};

	// A model is a collection of vertices
	struct model {
		std::vector<vertex> vertices;
	};

	// A Tree of models
	// Introduced in part 2, for model groups.
	// Each branch has:
	// - A name - specified in the XML with the "name" attribute
	//  (example: <group name="AsteroidBelt">);
	// - A vector of models;
	// - An applyTranslate, applyRotate, and applyScale booleans
	//   that determine wether to apply these transforms to the
	//   current tree branch & sub branches;
	// - A translate, rotate, scale coordinate that
	//   should be applied to all models (including sub branches)
	//   if not null. If rotate is not null, a value is expected for
	//   rotateAngle;
	// - A vector of sub branches.
	//
	// For simplicity, the vertex data structure is used for transformations,
	// since each transformation has an X, Y, and Z value, just like a vertex.
	struct modelTree {
		std::string name;
		std::vector<model> models;
		bool applyTranslate = false;
		bool applyRotate = false;
		bool applyScale = false;
		vertex translate;
		vertex rotate;
		float rotateAngle;
		vertex scale;
		std::vector<modelTree> branches;
	};

	// Scene that can be loaded with loadScene().
	// "extern" means it has to be defined once in a source (.cpp) file
	extern std::vector<model> scene;

	void loadScene(std::vector<model> scene); // Loads a scene
	void drawFrame(); // Draws a frame of the scene
};

#endif
