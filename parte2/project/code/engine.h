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

	// Scene that can be loaded with loadScene().
	// "extern" means it has to be defined once in a source (.cpp) file
	extern std::vector<model> scene;

	void loadScene(std::vector<model> scene); // Loads a scene
	void drawFrame(); // Draws a frame of the scene
};

#endif
