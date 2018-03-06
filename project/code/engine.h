#pragma once
#ifndef	ENGINE_H
#define ENGINE_H

namespace engine {

	// A vertex has x y and z coordinates
	struct vertex {
		double x, y, z;
	};

	// A model is a collection of vertices
	struct model {
		std::vector<vertex> vertices;
	};

	static void drawFrame();
};

#endif
