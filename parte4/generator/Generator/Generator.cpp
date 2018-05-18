#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
const int NUMBER_INDEXES = 16;
unsigned int *patches;
float *control_points;

void makePlane(float width, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");
	float side = width / 2;

	if (f != NULL) {
		// Vector where the points will be stored
		std::vector<Point> points;
		std::vector<TexturePoint> texture_points;
		Point normal;

		// first triangle
		points.push_back(Point(side, 0.0, side));
		texture_points.push_back(TexturePoint(1, 0));
		points.push_back(Point(side, 0.0, -side));
		texture_points.push_back(TexturePoint(1, 1));
		points.push_back(Point(-side, 0.0, -side));
		texture_points.push_back(TexturePoint(0, 1));

		// second triangle
		points.push_back(Point(side, 0.0, side));
		texture_points.push_back(TexturePoint(1, 0));
		points.push_back(Point(-side, 0.0, -side));
		texture_points.push_back(TexturePoint(0, 1));
		points.push_back(Point(-side, 0.0, side));
		texture_points.push_back(TexturePoint(0, 0));

		// normal 
		normal = Point(0, 1, 0);

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print every point into the file
		for (int point = 0; point < points.size(); point++) {
			// Triangle point
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());

			// Normal
			fprintf(f, "%f %f %f\n", normal.getX(), normal.getY(), normal.getZ());

			// Texture Point
			fprintf(f, "%f %f\n", texture_points[point].getX(), texture_points[point].getY());
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

		// Print every point into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);

}

void makeSphere(float radius, int slices, int stacks, string filename) {
	float alpha = 0;
	float beta = -(M_PI) / 2;
	float angle_y = (M_PI) / stacks;
	float slice_size_texture = 1.0f / slices;
	float stack_size_texture = 1.0f / stacks;
	float incStacks = stack_size_texture;

	// Vectors used to store all the sphere's points (coordinate, normal and texture)
	vector<Point> all_coordinate;
	vector<Point> all_normal;
	vector<TexturePoint> all_texture;

	// Point at the bottom of the sphere
	all_coordinate.push_back(Point(0, -radius, 0));
	all_normal.push_back(Point(0, -1, 0));
	all_texture.push_back(TexturePoint(0.5f, 0.0f));

	// Saves every point of the sphere into vectors (except for the top and bottom ones)
	for (int i = 0; i <= stacks; beta += angle_y, i++) {
		for (int slice = 0; slice < slices; slice++) {
			if (i == 0) {
				all_texture.push_back(TexturePoint((slice * slice_size_texture) + (slice_size_texture / 2), 0));
				continue;
			}
			if (i == stacks) {
				all_texture.push_back(TexturePoint((slice * slice_size_texture) + (slice_size_texture / 2), incStacks));
				continue;
			}

			alpha = ((2 * M_PI) / (slices)) * slice;
			float x = radius * cos(beta) * sin(alpha);
			float z = radius * cos(beta) * cos(alpha);
			float y = radius * sin(beta);
			all_coordinate.push_back(Point(x, y, z));
			all_normal.push_back(Point(x / radius, y / radius, z / radius));
			all_texture.push_back(TexturePoint((slice * slice_size_texture), incStacks));
		}

		if (i != 0 && i != stacks) {
			all_texture.push_back(TexturePoint(1, incStacks));
			incStacks += stack_size_texture;
		}
	}

	// Point at the top of the sphere
	all_coordinate.push_back(Point(0, radius, 0));
	all_normal.push_back(Point(0, 1, 0));
	all_texture.push_back(TexturePoint(0.5, 1));

	// Vectors used to store all the triangle points in the sphere
	vector<Point> triangle_coordinate;
	vector<Point> triangle_normal;
	vector<TexturePoint> triangle_texture;

	// Bottom of the sphere
	int counter;
	int number_points = all_coordinate.size();
	for (counter = 1; counter < number_points && all_coordinate[counter].getY() == all_coordinate[counter + 1].getY(); counter++) {
		triangle_coordinate.push_back(all_coordinate[counter]);
		triangle_coordinate.push_back(all_coordinate[0]);
		triangle_coordinate.push_back(all_coordinate[counter + 1]);
		
		triangle_normal.push_back(all_normal[counter]);
		triangle_normal.push_back(all_normal[0]);
		triangle_normal.push_back(all_normal[counter + 1]);

		triangle_texture.push_back(all_texture[counter + (int)stacks - 1]);
		triangle_texture.push_back(all_texture[counter]);
		triangle_texture.push_back(all_texture[counter + (int)stacks]);
	}

	// Last triangle of the bottom of the sphere
	triangle_coordinate.push_back(all_coordinate[counter]);
	triangle_coordinate.push_back(all_coordinate[0]);
	triangle_coordinate.push_back(all_coordinate[1]);

	triangle_normal.push_back(all_normal[counter]);
	triangle_normal.push_back(all_normal[0]);
	triangle_normal.push_back(all_normal[1]);

	triangle_texture.push_back(all_texture[counter + (int)stacks - 1]);
	triangle_texture.push_back(all_texture[counter]);
	triangle_texture.push_back(all_texture[counter + (int)stacks]);

	// Middle stacks of the sphere
	int fixed = counter, fixed_texture = counter + 1, previous = fixed_texture, texture_counter = 0;
	counter++;
	bool last = false;

	for (int stack = 1; stack < stacks - 1; stack++) {
		last = false;
		texture_counter = fixed_texture + previous;
		previous = texture_counter;

		for (counter; counter < number_points && all_coordinate[counter].getY() == all_coordinate[counter + 1].getY(); counter++) {
			last = true;

			// First triangle
			triangle_coordinate.push_back(all_coordinate[counter]);
			triangle_coordinate.push_back(all_coordinate[counter - fixed]);
			triangle_coordinate.push_back(all_coordinate[counter + 1]);

			triangle_normal.push_back(all_normal[counter]);
			triangle_normal.push_back(all_normal[counter - fixed]);
			triangle_normal.push_back(all_normal[counter + 1]);

			triangle_texture.push_back(all_texture[texture_counter]);
			triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
			triangle_texture.push_back(all_texture[texture_counter + 1]);

			// Second triangle
			triangle_coordinate.push_back(all_coordinate[counter + 1]);
			triangle_coordinate.push_back(all_coordinate[counter - fixed]);
			triangle_coordinate.push_back(all_coordinate[counter + 1 - fixed]);

			triangle_normal.push_back(all_normal[counter + 1]);
			triangle_normal.push_back(all_normal[counter - fixed]);
			triangle_normal.push_back(all_normal[counter + 1 - fixed]);

			triangle_texture.push_back(all_texture[texture_counter + 1]);
			triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
			triangle_texture.push_back(all_texture[texture_counter + 1 - fixed_texture]);

			texture_counter++;
		}

		// Last triangles of each stack
		if (last) {
			// First triangle
			triangle_coordinate.push_back(all_coordinate[counter]);
			triangle_coordinate.push_back(all_coordinate[counter - fixed]);
			triangle_coordinate.push_back(all_coordinate[counter + 1 - fixed]);

			triangle_normal.push_back(all_normal[counter]);
			triangle_normal.push_back(all_normal[counter - fixed]);
			triangle_normal.push_back(all_normal[counter + 1 - fixed]);

			triangle_texture.push_back(all_texture[texture_counter]);
			triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
			triangle_texture.push_back(all_texture[texture_counter + 1]);

			// Second triangle
			triangle_coordinate.push_back(all_coordinate[counter - fixed]);
			triangle_coordinate.push_back(all_coordinate[counter + 1 - (2 * fixed)]);
			triangle_coordinate.push_back(all_coordinate[counter + 1 - fixed]);

			triangle_normal.push_back(all_normal[counter - fixed]);
			triangle_normal.push_back(all_normal[counter + 1 - (2 * fixed)]);
			triangle_normal.push_back(all_normal[counter + 1 - fixed]);

			triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
			triangle_texture.push_back(all_texture[texture_counter + 1 - fixed_texture]);
			triangle_texture.push_back(all_texture[texture_counter + 1]);

			texture_counter++;
			counter++;
		}
	}

	// Top of the sphere
	texture_counter++;
	int k = counter - fixed;
	for (k = counter - fixed; k < number_points - 2; k++) {
		triangle_coordinate.push_back(all_coordinate[k]);
		triangle_coordinate.push_back(all_coordinate[k + 1]);
		triangle_coordinate.push_back(all_coordinate[number_points - 1]);

		triangle_normal.push_back(all_normal[k]);
		triangle_normal.push_back(all_normal[k + 1]);
		triangle_normal.push_back(all_normal[number_points - 1]);

		triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
		triangle_texture.push_back(all_texture[texture_counter + 1 - fixed_texture]);
		triangle_texture.push_back(all_texture[texture_counter]);

		texture_counter++;
	}

	// Last triangle of the top of the sphere
	triangle_coordinate.push_back(all_coordinate[k]);
	triangle_coordinate.push_back(all_coordinate[k + 1 - fixed]);
	triangle_coordinate.push_back(all_coordinate[number_points - 1]);

	triangle_normal.push_back(all_normal[k]);
	triangle_normal.push_back(all_normal[k + 1 - fixed]);
	triangle_normal.push_back(all_normal[number_points - 1]);

	triangle_texture.push_back(all_texture[texture_counter - fixed_texture]);
	triangle_texture.push_back(all_texture[texture_counter + 1 - fixed_texture]);
	triangle_texture.push_back(all_texture[texture_counter]);

	// Print to file
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	// Print the number of triangles into the file
	fprintf(f, "%d\n", triangle_coordinate.size() / 3);

	for (int point = 0; point < triangle_coordinate.size(); point++) {
		// Coordinate point
		fprintf(f, "%f %f %f\n", triangle_coordinate[point].getX(), triangle_coordinate[point].getY(), triangle_coordinate[point].getZ());
		// Normal
		fprintf(f, "%f %f %f\n", triangle_normal[point].getX(), triangle_normal[point].getY(), triangle_normal[point].getZ());
		// Texture point
		fprintf(f, "%f %f\n", triangle_texture[point].getX(), triangle_texture[point].getY());
	}

	fclose(f);
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

			// Tip of the cone
			points.push_back(Point(0.0, height, 0.0));
			points.push_back(Point(previousRadius * sin(alpha), previousStackHeight, previousRadius * cos(alpha)));
			points.push_back(Point(previousRadius * sin(nextAlpha), previousStackHeight, previousRadius * cos(nextAlpha)));
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print every point into the file
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

			// Bottom triangles
			points.push_back(Point(0.0f, 0.0f, 0.0f));
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

			// Top triangles
			points.push_back(Point(0.0f, height, 0.0f));
			points.push_back(Point(radius * sin(alpha), height, radius * cos(alpha)));
			points.push_back(Point(radius * sin(nextAlpha), height, radius * cos(nextAlpha)));
		}

		// Print the number of triangles into the file
		fprintf(f, "%d\n", points.size() / 3);

		// Print every point into the file
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

		// Print every point into the file
		for (int point = 0; point < points.size(); point++) {
			fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		}
	}

	fclose(f);

}

void readPatch(string filename) {
	ifstream file(filename.c_str());
	string firstLine, line;
	getline(file, firstLine);
	int number_patches = atoi(firstLine.c_str());
	patches = (unsigned int*)malloc(sizeof(unsigned int) * number_patches * NUMBER_INDEXES);

	// Save Patches
	for (int patch_number = 0; patch_number < number_patches; patch_number++) {
		getline(file, line);
		istringstream indexes(line);
		string indexCP;
		for (int position = 0; position < NUMBER_INDEXES && getline(indexes, indexCP, ','); position++)
			patches[patch_number * NUMBER_INDEXES + position] = atoi(indexCP.c_str());
	}

	// Save Control Points
	getline(file, firstLine);
	int number_control_points = atoi(firstLine.c_str());
	control_points = (float *)malloc(sizeof(float) * 3 * number_control_points);

	for (int control_point = 0; control_point < number_control_points; control_point++) {
		getline(file, line);
		istringstream indexes(line);
		string indexCP;
		for (int position = 0; position < 3 && getline(indexes, indexCP, ','); position++)
			control_points[control_point * 3 + position] = (float)atof(indexCP.c_str());
	}
}

void cross(float *a, float *b, float *res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

Point bezierPointNormal(int patch_number, float u, float v) {
	float tangentU[3] = { 0.0f, 0.0f, 0.0f };
	float tangentV[3] = { 0.0f, 0.0f, 0.0f };
	float normal[3] = { 0.0f, 0.0f, 0.0f };

	// Bernstein polynomials
	float bu[4] = { powf(1 - u, 3), 3 * u * powf(1 - u, 2) , 3 * powf(u, 2) * (1 - u) ,powf(u, 3) };
	float bv[4] = { powf(1 - v, 3), 3 * v * powf(1 - v, 2), 3 * powf(v, 2) * (1 - v), powf(v, 3) };
	float dbu[4] = { (-3)*(powf(1 - u, 2)), 3 * (powf((1 - u),2)) - 6 * u*(1 - u) , 6 * u*(1 - u) - 3 * (powf(u, 2)) ,3 * powf(u, 2) };
	float dbv[4] = { (-3)*(powf(1 - v, 2)), 3 * (powf((1 - v),2)) - 6 * v*(1 - v) , 6 * v*(1 - v) - 3 * (powf(v, 2)) ,3 * powf(v, 2) };

	// Sum
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			int index_inside_patch = j * 4 + i;
			int indexCP = patches[patch_number * NUMBER_INDEXES + index_inside_patch];

			tangentU[0] += dbu[j] * control_points[indexCP * 3 + 0] * bv[i];
			tangentU[1] += dbu[j] * control_points[indexCP * 3 + 1] * bv[i];
			tangentU[2] += dbu[j] * control_points[indexCP * 3 + 2] * bv[i];
		}
	}

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			int index_inside_patch = i * 4 + j;
			int indexCP = patches[patch_number * NUMBER_INDEXES + index_inside_patch];

			tangentV[0] += dbv[j] * control_points[indexCP * 3 + 0] * bu[i];
			tangentV[1] += dbv[j] * control_points[indexCP * 3 + 1] * bu[i];
			tangentV[2] += dbv[j] * control_points[indexCP * 3 + 2] * bu[i];
		}
	}

	// Cross tangents
	cross(tangentV, tangentU, normal);
	// Normalize to obtain normal
	normalize(normal);

	Point res(normal[0], normal[1], normal[2]);

	return res;
}

Point bezierPoint(int patch_number, float u, float v) {
	Point p(0, 0, 0);

	// Bernstein Polynomial
	float bpu[4] = { powf(1 - u, 3), 3 * u * powf(1 - u, 2), 3 * powf(u, 2) * (1 - u), powf(u, 3) };
	float bpv[4] = { powf(1 - v, 3), 3 * v * powf(1 - v, 2), 3 * powf(v, 2) * (1 - v), powf(v, 3) };

	// Bezier Patch sum
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++) {
			int index_inside_patch = j * 4 + i;
			int indexCP = patches[patch_number * NUMBER_INDEXES + index_inside_patch];
			p.setX(p.getX() + control_points[indexCP * 3 + 0] * bpu[j] * bpv[i]);
			p.setY(p.getY() + control_points[indexCP * 3 + 1] * bpu[j] * bpv[i]);
			p.setZ(p.getZ() + control_points[indexCP * 3 + 2] * bpu[j] * bpv[i]);
		}

	return p;
}

void makeBezier(string  filenamePatch, string filename3d, int tesselationLevel) {
	readPatch(filenamePatch);
	vector<Point> points;
	vector<Point> normal;
	vector<TexturePoint> texture_points;

	for (int patch_number = 0; patch_number < 32; patch_number++) {
		for (int tesselation_v = 0; tesselation_v < tesselationLevel; tesselation_v++) {
			float v = (float)tesselation_v / tesselationLevel;
			for (int tesselation_u = 0; tesselation_u < tesselationLevel; tesselation_u++) {
				float u = (float)tesselation_u / tesselationLevel;
				// first triangle
				points.push_back(bezierPoint(patch_number, u, v));
				points.push_back(bezierPoint(patch_number, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
				points.push_back(bezierPoint(patch_number, (u + (1.0f / tesselationLevel)), v));

				// second triangle
				points.push_back(bezierPoint(patch_number, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
				points.push_back(bezierPoint(patch_number, u, v));
				points.push_back(bezierPoint(patch_number, u, (v + (1.0f / tesselationLevel))));

				// first normal point
				normal.push_back(bezierPointNormal(patch_number, u, v));
				normal.push_back(bezierPointNormal(patch_number, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
				normal.push_back(bezierPointNormal(patch_number, (u + (1.0f / tesselationLevel)), v));

				// second normal point
				normal.push_back(bezierPointNormal(patch_number, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
				normal.push_back(bezierPointNormal(patch_number, u, v));
				normal.push_back(bezierPointNormal(patch_number, u, (v + (1.0f / tesselationLevel))));

				// first texture Points
				texture_points.push_back(TexturePoint(u, v));
				texture_points.push_back(TexturePoint(u + (1.0f / tesselationLevel), (v + (1.0f / tesselationLevel))));
				texture_points.push_back(TexturePoint(u + (1.0f / tesselationLevel), v));

				// second texture point
				texture_points.push_back(TexturePoint(u + (1.0f / tesselationLevel), (v + (1.0f / tesselationLevel))));
				texture_points.push_back(TexturePoint(u, v));
				texture_points.push_back(TexturePoint(u, (v + (1.0f / tesselationLevel))));
			}
		}
	}

	FILE *f;
	fopen_s(&f, filename3d.c_str(), "w");

	// Print the number of triangles into the file
	fprintf(f, "%d\n", points.size() / 3);

	// Print every point into the file
	for (int point = 0; point < points.size() && point < normal.size() && point < texture_points.size(); point++) {
		// Triangle point
		fprintf(f, "%f %f %f\n", points[point].getX(), points[point].getY(), points[point].getZ());
		// Normal point
		fprintf(f, "%f %f %f\n", normal[point].getX(), normal[point].getY(), normal[point].getZ());
		// Texture point
		fprintf(f, "%f %f\n", texture_points[point].getX(), texture_points[point].getY());
	}

	fclose(f);

	// Free memory allocated in readPatch
	free(patches);
	free(control_points);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Input inválida. Por favor tente novamente.");
		return -1;
	}

	string type = argv[1];

	if (type == "plane" && argc >= 4) {
		float width = stof(argv[2]);
		string filename = argv[3];
		makePlane(width, filename);
		return 0;
	}

	if (type == "box" && argc >= 7) {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		int numberOfDivisions = stoi(argv[5]);
		string filename = argv[6];
		makeBox(x, y, z, numberOfDivisions, filename);
		return 0;
	}

	if (type == "sphere" && argc >= 6) {
		float radius = stof(argv[2]);
		int slices = stoi(argv[3]), stacks = stoi(argv[4]);
		string filename = argv[5];

		if (slices > 1 && stacks > 0) {
			makeSphere(radius, slices, stacks, filename);
			return 0;
		}
		else {
			printf("Input inválida. Por favor tente novamente.");
			return -1;
		}
		return 0;
	}

	if (type == "cone" && argc >= 7) {
		float radius = stof(argv[2]), height = stof(argv[3]);
		int slices = stoi(argv[4]), stacks = stoi(argv[5]);
		string filename = argv[6];

		if (slices > 1 && stacks > 0) {
			makeCone(radius, height, slices, stacks, filename);
			return 0;
		}
		else {
			printf("Input inválida. Por favor tente novamente.");
			return -1;
		}
	}

	if (type == "cylinder") {
		float radius = stof(argv[2]), height = stof(argv[3]);
		int slices = stoi(argv[4]), stacks = stoi(argv[5]);
		string filename = argv[6];

		if (slices > 1 && stacks > 0) {
			makeCylinder(radius, height, slices, stacks, filename);
			return 0;
		} else {
			printf("Input inválida. Por favor tente novamente.");
			return -1;
		}
	}

	if (type == "pyramid" && argc >= 6) {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		string filename = argv[5];
		makePyramid(x, y, z, filename);
		return 0;
	}

	if (type == "bezier" && argc >= 5) {
		makeBezier(argv[2], argv[3], atoi(argv[4]));
		return 0;
	}

	// If it reached this point no valid option was inserted
	printf("Input inválida, por favor tente novamente.");
	return -1;
}