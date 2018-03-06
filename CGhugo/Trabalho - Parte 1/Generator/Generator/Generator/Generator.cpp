#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

void makePlane(float width, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");
	float side = width / 2;

	if (f != NULL) {
		// all triangles
		fprintf(f, "2\n");

		// first triangle
		fprintf(f, "%f %f %f\n", side, 0.0, -side);
		fprintf(f, "%f %f %f\n", -side, 0.0, -side);
		fprintf(f, "%f %f %f\n", -side, 0.0, side);

		// second triangle
		fprintf(f, "%f %f %f\n", side, 0.0, -side);
		fprintf(f, "%f %f %f\n", -side, 0.0, side);
		fprintf(f, "%f %f %f\n", side, 0.0, side);
	}

	fclose(f);

}

void makeBox(float x, float y, float z, int numberOfDivisions, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		// all triangles
		fprintf(f, "%d\n", 12 * numberOfDivisions * numberOfDivisions);

		// xy with z positive
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {

			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				// first
				fprintf(f, "%f %f %f\n", px, py, z / 2);
				fprintf(f, "%f %f %f\n", px, py - (y / numberOfDivisions), z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py, z / 2);

				// second
				fprintf(f, "%f %f %f\n", px, py - (y / numberOfDivisions), z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py - (y / numberOfDivisions), z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py, z / 2);
			}

		}

		// zx with y positive
		for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				//first
				fprintf(f, "%f %f %f\n", px, y / 2, pz);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), y / 2, pz);
				fprintf(f, "%f %f %f\n", px, y / 2, pz - (z / numberOfDivisions));
				//second
				fprintf(f, "%f %f %f\n", px, y / 2, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), y / 2, pz);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), y / 2, pz - (z / numberOfDivisions));
			}
		}

		// zy with x positive
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {
			for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
				// first 
				fprintf(f, "%f %f %f\n", x / 2, py, pz);
				fprintf(f, "%f %f %f\n", x / 2, py - (y / numberOfDivisions), pz);
				fprintf(f, "%f %f %f\n", x / 2, py, pz - (z / numberOfDivisions));

				// second 
				fprintf(f, "%f %f %f\n", x / 2, py, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", x / 2, py - (y / numberOfDivisions), pz);
				fprintf(f, "%f %f %f\n", x / 2, py - (y / numberOfDivisions), pz - (z / numberOfDivisions));
			}
		}

		// xy with z negative
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {

			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				// first
				fprintf(f, "%f %f %f\n", px, py, -z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py, -z / 2);
				fprintf(f, "%f %f %f\n", px, py - (y / numberOfDivisions), -z / 2);
				

				// second
				fprintf(f, "%f %f %f\n", px, py - (y / numberOfDivisions), -z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py, -z / 2);
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), py - (y / numberOfDivisions), -z / 2);
				
			}

		}

		// zx with y negative
		for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
			for (float px = -x / 2; px < x / 2; px += x / numberOfDivisions) {
				//first
				fprintf(f, "%f %f %f\n", px, -y / 2, pz);
				fprintf(f, "%f %f %f\n", px, -y / 2, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), -y / 2, pz);
				
				//second
				fprintf(f, "%f %f %f\n", px, -y / 2, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), -y / 2, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", px + (x / numberOfDivisions), -y / 2, pz);
				
			}
		}

		// zy with x negative 
		for (float py = y / 2; py > -y / 2; py -= y / numberOfDivisions) {
			for (float pz = z / 2; pz > -z / 2; pz -= z / numberOfDivisions) {
				// first 
				fprintf(f, "%f %f %f\n", -x / 2, py, pz);
				fprintf(f, "%f %f %f\n", -x / 2, py, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", -x / 2, py - (y / numberOfDivisions), pz);
				

				// second 
				fprintf(f, "%f %f %f\n", -x / 2, py, pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", -x / 2, py - (y / numberOfDivisions), pz - (z / numberOfDivisions));
				fprintf(f, "%f %f %f\n", -x / 2, py - (y / numberOfDivisions), pz);
				
			}
		}
	}

	fclose(f);

}

void makeSphere(float radius, float slices, int stacks, string filename) {

}

void makeCone(float radius, float height, int slices, int stacks, string filename) {

}

void makePyramid(float length, float height, float width, string filename) {
	FILE *f;
	fopen_s(&f, filename.c_str(), "w");

	if (f != NULL) {
		fprintf(f, "%f %f %f\n", 0.0, height, 0.0);
		fprintf(f, "%f %f %f\n", -length, 0.0, width);
		fprintf(f, "%f %f %f\n", length, 0.0, width);

		fprintf(f, "%f %f %f\n", 0.0, height, 0.0);
		fprintf(f, "%f %f %f\n", length, 0.0, width);
		fprintf(f, "%f %f %f\n", length, 0.0, -width);

		fprintf(f, "%f %f %f\n", 0.0, height, 0.0);
		fprintf(f, "%f %f %f\n", length, 0.0, -width);
		fprintf(f, "%f %f %f\n", -length, 0.0, -width);

		fprintf(f, "%f %f %f\n", 0.0, height, 0.0);
		fprintf(f, "%f %f %f\n", -length, 0.0, -width);
		fprintf(f, "%f %f %f\n", -length, 0.0, width);

		fprintf(f, "%f %f %f\n", -length, 0.0, -width);
		fprintf(f, "%f %f %f\n", length, 0.0, width);
		fprintf(f, "%f %f %f\n", -length, 0.0, width);
		

		fprintf(f, "%f %f %f\n", -length, 0.0, -width);
		fprintf(f, "%f %f %f\n", length, 0.0, -width);
		fprintf(f, "%f %f %f\n", length, 0.0, width);
		
	}

	fclose(f);

}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Invalid parameters. Exiting..");
		return -1;
	}

	string type = argv[1];

	if (type == "plane") {
		float width = stof(argv[2]);
		string filename = argv[3];
		makePlane(width, filename);
	}


	else if (type == "box") {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		int numberOfDivisions = stoi(argv[5]);
		string filename = argv[6];
		makeBox(x, y, z, numberOfDivisions, filename);
	}

	else if (type == "sphere") {
		float radius = stof(argv[2]);
		int slices = stoi(argv[3]), stacks = stoi(argv[4]);
		string filename = argv[5];
		makeSphere(radius, slices, stacks, filename);
	}

	else if (type == "cone") {
		float radius = stof(argv[2]), height = stof(argv[3]);
		int slices = stoi(argv[4]), stacks = stoi(argv[5]);
		string filename = argv[6];
		makeCone(radius, height, slices, stacks, filename);
	}

	else if (type == "pyramid") {
		float x = stof(argv[2]), y = stof(argv[3]), z = stof(argv[4]);
		string filename = argv[5];
		makePyramid(x,y, z, filename);
	}

	else {
		printf("Invalid parameters. Exiting..");
		return -1;
	}

}