#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include "Point.h"


#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

void makePlane(float width, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");
	float side = width / 2;

	if (f != NULL) {
		// Vector where the points will be stored
		std::vector<Point> points;

		// first triangle
		points.push_back(Point(side, 0.0, -side));
		points.push_back(Point(-side, 0.0, -side));
		points.push_back(Point(-side, 0.0, side));

		// second triangle
		points.push_back(Point(side, 0.0, -side));
		points.push_back(Point(-side, 0.0, side));
		points.push_back(Point(side, 0.0, side));

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);

}

void makeBox(float x, float y, float z, int numberOfDivisions, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		// Vector where the points will be stored
		std::vector<Point> points;

		// xy with z positive
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {

			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				// first
				points.push_back(Point(px, py, z / 2));
				points.push_back(Point(px, py - (y / numberOfDivisions), z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py, z / 2));

				// second
				points.push_back(Point(px, py - (y / numberOfDivisions), z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py - (y / numberOfDivisions), z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py, z / 2));
			}

		}

		// zx with y positive
		for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				//first
				points.push_back(Point(px, y / 2, pz));
				points.push_back(Point(px + (x / numberOfDivisions), y / 2, pz));
				points.push_back(Point(px, y / 2, pz - (z / numberOfDivisions)));
				//second
				points.push_back(Point(px, y / 2, pz - (z / numberOfDivisions)));
				points.push_back(Point(px + (x / numberOfDivisions), y / 2, pz));
				points.push_back(Point(px + (x / numberOfDivisions), y / 2, pz - (z / numberOfDivisions)));
			}
		}

		// zy with x positive
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {
			for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
				// first 
				points.push_back(Point(x / 2, py, pz));
				points.push_back(Point(x / 2, py - (y / numberOfDivisions), pz));
				points.push_back(Point(x / 2, py, pz - (z / numberOfDivisions)));

				// second 
				points.push_back(Point(x / 2, py, pz - (z / numberOfDivisions)));
				points.push_back(Point(x / 2, py - (y / numberOfDivisions), pz));
				points.push_back(Point(x / 2, py - (y / numberOfDivisions), pz - (z / numberOfDivisions)));
			}
		}

		// xy with z negative
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {

			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				// first
				points.push_back(Point(px, py, -z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py, -z / 2));
				points.push_back(Point(px, py - (y / numberOfDivisions), -z / 2));


				// second
				points.push_back(Point(px, py - (y / numberOfDivisions), -z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py, -z / 2));
				points.push_back(Point(px + (x / numberOfDivisions), py - (y / numberOfDivisions), -z / 2));
			}
		}

		// zx with y negative
		for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				//first
				points.push_back(Point(px, -y / 2, pz));
				points.push_back(Point(px, -y / 2, pz - (z / numberOfDivisions)));
				points.push_back(Point(px + (x / numberOfDivisions), -y / 2, pz));

				//second
				points.push_back(Point(px, -y / 2, pz - (z / numberOfDivisions)));
				points.push_back(Point(px + (x / numberOfDivisions), -y / 2, pz - (z / numberOfDivisions)));
				points.push_back(Point(px + (x / numberOfDivisions), -y / 2, pz));

			}
		}

		// zy with x negative 
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {
			for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
				// first 
				points.push_back(Point(-x / 2, py, pz));
				points.push_back(Point(-x / 2, py, pz - (z / numberOfDivisions)));
				points.push_back(Point(-x / 2, py - (y / numberOfDivisions), pz));


				// second 
				points.push_back(Point(-x / 2, py, pz - (z / numberOfDivisions)));
				points.push_back(Point(-x / 2, py - (y / numberOfDivisions), pz - (z / numberOfDivisions)));
				points.push_back(Point(-x / 2, py - (y / numberOfDivisions), pz));
			}
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);

}

void makeSphere(float radius, float slices, float stacks, string filename) {
	int slice, stack;
	float x, y, z, r, h, alpha;

	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		// Vector where the points will be stored
		std::vector<Point> points;

		h = radius - (2 * radius) / stacks;
		r = sqrt(radius * radius - h * h);
		// top of the sphere
		for (slice = 0; slice <= slices; slice++) {
			x = 0; y = radius; z = 0;
			points.push_back(Point(x, y, z));
			alpha = slice * ((2 * M_PI) / slices);
			x = sin(alpha)*r; y = h; z = cos(alpha)*r;
			points.push_back(Point(x, y, z));
			alpha = (slice + 1)*((2 * M_PI) / slices);
			// height is the same
			x = sin(alpha)*r; z = cos(alpha)*r;
			points.push_back(Point(x, y, z));
		}

		// middle of the top half sphere
		for (stack = 1; stack < stacks / 2; stack++) {

			h = radius - ((2 * radius)*stack) / stacks; // Altura de cada nível da stack
			r = sqrt(radius*radius - h*h); // Raio da stack
			for (slice = 0; slice < slices; slice++) {
				//Triângulo inferior
				alpha = slice*(2 * M_PI) / slices;
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				h = radius - ((radius) * 2 * (stack + 1)) / stacks;
				r = sqrt(radius*radius - h*h);
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				alpha = ((slice + 1)*(2 * M_PI)) / slices;
				x = sin(alpha)*r;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));

				//Triângulo superior
				points.push_back(Point(x, y, z));
				h = radius - ((2 * radius)*stack) / stacks;
				r = sqrt(radius*radius - h*h);
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				alpha = slice*(2 * M_PI) / slices;
				x = sin(alpha)*r;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
			}
		}

		// middle of the bottom half sphere
		for (stack = 1; stack < stacks / 2; stack++) {

			h = -radius + ((2 * radius)*stack) / stacks; // Altura de cada nível da stack
			r = sqrt(radius*radius - h*h); // Raio da stack
			for (slice = 0; slice < slices; slice++) {
				//Triângulo inferior
				alpha = -slice*(2 * M_PI) / slices;
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				h = -radius + ((radius) * 2 * (stack + 1)) / stacks;
				r = sqrt(radius*radius - h*h);
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				alpha = -((slice + 1)*(2 * M_PI)) / slices;
				x = sin(alpha)*r;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));

				//Triângulo superior
				points.push_back(Point(x, y, z));
				h = -radius + ((2 * radius)*stack) / stacks;
				r = sqrt(radius*radius - h*h);
				x = sin(alpha)*r;
				y = h;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
				alpha = -slice*(2 * M_PI) / slices;
				x = sin(alpha)*r;
				z = cos(alpha)*r;
				points.push_back(Point(x, y, z));
			}
		}

		// bottom part
		h = -radius + (2 * radius) / stacks;
		r = sqrt(radius * radius - h * h);
		for (slice = 0; slice <= slices; slice++) {
			x = 0; y = -radius; z = 0;
			points.push_back(Point(x, y, z));
			alpha = -slice*((2 * M_PI) / slices);
			x = sin(alpha)*r;
			y = h;
			z = cos(alpha)*r;
			points.push_back(Point(x, y, z));
			alpha = -(slice + 1)*((2 * M_PI) / slices);
			x = sin(alpha)*r;
			z = cos(alpha)*r;
			points.push_back(Point(x, y, z));
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}
}

void makeCone(float radius, float height, int slices, int stacks, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		std::vector<Point> points;
		for (int slice = 0; slice < slices; slice++) {
			float alpha = slice * (2 * M_PI / slices);
			float nextAlpha = (slice + 1) * (2 * M_PI / slices);

			// BASE TRIANGLES
			points.push_back(Point(0.0f, 0.0f, 0.0f));
			points.push_back(Point(radius * sin(nextAlpha), 0.0f, radius * cos(nextAlpha)));
			points.push_back(Point(sin(alpha), 0.0f, radius * cos(alpha)));

			// SIDE TRIANGLES EXCEPT FOR THE TIP
			float previousStackHeight = 0, previousRadius = radius;
			for (int stack = 1; stack < stacks; stack++) {
				float stackHeight = height / stacks  * stack;
				float newRadius = (height - stackHeight) * radius / height;

				points.push_back(Point(newRadius * sin(alpha), stackHeight, newRadius * cos(alpha)));
				points.push_back(Point(previousRadius * sin(alpha), previousStackHeight, previousRadius * cos(alpha)));
				points.push_back(Point(previousRadius * sin(nextAlpha), previousStackHeight, previousRadius * cos(nextAlpha)));

				points.push_back(Point(newRadius * sin(alpha), stackHeight, newRadius * cos(alpha)));
				points.push_back(Point(previousRadius * sin(nextAlpha), previousStackHeight, previousRadius * cos(nextAlpha)));
				points.push_back(Point(newRadius * sin(nextAlpha), stackHeight, newRadius * cos(nextAlpha)));

				previousStackHeight = stackHeight;
				previousRadius = newRadius;
			}

			//TIP OF THE CONE
			points.push_back(Point(0.0, height, 0.0));
			points.push_back(Point(previousRadius * sin(alpha), previousStackHeight, previousRadius * cos(alpha)));
			points.push_back(Point(previousRadius * sin(nextAlpha), previousStackHeight, previousRadius * cos(nextAlpha)));
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);
}

void makeCylinder(float radius, float height, int slices, int stacks, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		std::vector<Point> points;
		for (int slice = 0; slice < slices; slice++) {
			float alpha = slice * (2 * M_PI / slices);
			float nextAlpha = (slice + 1) * (2 * M_PI / slices);

			// BOTTOM TRIANGLES
			points.push_back(Point (0.0f, 0.0f, 0.0f));
			points.push_back(Point(radius * sin(nextAlpha), 0.0f, radius * cos(nextAlpha)));
			points.push_back(Point(radius * sin(alpha), 0.0f, radius * cos(alpha)));

			float previousStackHeight = 0;
			for (int stack = 1; stack <= stacks; stack++) {
				float stackHeight = height / stacks  * stack;

				points.push_back(Point(radius * sin(alpha), stackHeight, radius * cos(alpha)));
				points.push_back(Point(radius * sin(alpha), previousStackHeight, radius * cos(alpha)));
				points.push_back(Point(radius * sin(nextAlpha), previousStackHeight, radius * cos(nextAlpha)));

				points.push_back(Point(radius * sin(alpha), stackHeight, radius * cos(alpha)));
				points.push_back(Point(radius * sin(nextAlpha), previousStackHeight, radius * cos(nextAlpha)));
				points.push_back(Point(radius * sin(nextAlpha), stackHeight, radius * cos(nextAlpha)));

				previousStackHeight = stackHeight;
			}

			// TOP TRIANGLES
			points.push_back(Point(0.0f, height, 0.0f));
			points.push_back(Point(radius * sin(alpha), height, radius * cos(alpha)));
			points.push_back(Point(radius * sin(nextAlpha), height, radius * cos(nextAlpha)));
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}

	}
	fclose(f);
}

void makePyramid(float length, float height, float width, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		// Vector where the points will be stored
		std::vector<Point> points;

		points.push_back(Point(0.0, height, 0.0));
		points.push_back(Point(-length, 0.0, width));
		points.push_back(Point(length, 0.0, width));

		points.push_back(Point(0.0, height, 0.0));
		points.push_back(Point(length, 0.0, width));
		points.push_back(Point(length, 0.0, -width));

		points.push_back(Point(0.0, height, 0.0));
		points.push_back(Point(length, 0.0, -width));
		points.push_back(Point(-length, 0.0, -width));

		points.push_back(Point(0.0, height, 0.0));
		points.push_back(Point(-length, 0.0, -width));
		points.push_back(Point(-length, 0.0, width));

		points.push_back(Point(-length, 0.0, -width));
		points.push_back(Point(length, 0.0, width));
		points.push_back(Point(-length, 0.0, width));

		points.push_back(Point(-length, 0.0, -width));
		points.push_back(Point(length, 0.0, -width));
		points.push_back(Point(length, 0.0, width));

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print everypoint into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);

}


int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Invalid parameters. Exiting..");
		return -1;
	}

	string type = argv[1];

	if (type == "plane") {
		float width = stof(argv[2]);
		string filename = argv[3];
		makePlane(width, filename);
		return 0;
	}

	if (type == "box") {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		int numberOfDivisions = stoi(argv[5]);
		string filename = argv[6];
		makeBox(x, y, z, numberOfDivisions, filename);
		return 0;
	}

	if (type == "sphere") {
		float radius = stof(argv[2]);
		int slices = stoi(argv[3]), stacks = stoi(argv[4]);
		string filename = argv[5];
		// STACKS BIGGER THAN 0 && SLICES BIGGER THAN 1
		if (slices > 1 && stacks > 0) {
			makeSphere(radius, slices, stacks, filename);
			return 0;
		}
		else {
			printf("Invalid parameters. Exiting..");
			return -1;
		}
		return 0;
	}

	if (type == "cone") {
		float radius = stof(argv[2]), height = stof(argv[3]);
		int slices = stoi(argv[4]), stacks = stoi(argv[5]);
		string filename = argv[6];
		// STACKS BIGGER THAN 0 && SLICES BIGGER THAN 1
		if (slices > 1 && stacks > 0) {
			makeCone(radius, height, slices, stacks, filename);
			return 0;
		}
		else {
			printf("Invalid parameters. Exiting..");
			return -1;
		}
	}

	if (type == "cylinder") {
		float radius = stof(argv[2]), height = stof(argv[3]);
		int slices = stoi(argv[4]), stacks = stoi(argv[5]);
		string filename = argv[6];
		// STACKS BIGGER THAN 0 && SLICES BIGGER THAN 1
		if (slices > 1 && stacks > 0) {
			makeCylinder(radius, height, slices, stacks, filename);
			return 0;
		}
		else {
			printf("Invalid parameters. Exiting..");
			return -1;
		}
	}

	if (type == "pyramid") {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		string filename = argv[5];
		makePyramid(x, y, z, filename);
		return 0;
	}

	// IF IT REACHED THIS POINT NO VALID OPTIONS WERE INSERTED
	printf("Invalid parameters. Exiting..");
	return -1;
}