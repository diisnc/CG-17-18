#include<stdio.h>
#include<windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Point.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/*

TODO:

•   Plane (a square in the XZ plane, centred in the origin, made with 2 triangles)
•   Box (requires X, Y and Z dimensions, and optionally the number of divisions)
•   Sphere (requires radius, slices and stacks)
•   Cone (requires bottom radius, height, slices and stacks)
•   EXTRA: • pyramid (height, width)
           • cylinder (radius, height, length, slices)

*/


void plane(float width, string fileName){

    FILE *out;
    fopen_s(&out, fileName.c_str(),"w"); //open to write (cleans if file already exists or creates it if not)

    if(out != NULL) {
        std::vector<Point> vertices; // Vector to store the vertices

        //T1
        vertices.push_back(Point( width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0, width/2));
        vertices.push_back(Point( width/2, 0.0, width/2));

        //T2
        vertices.push_back(Point( width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0, width/2));

        // Sending vertices to .3d file
        for(int nVert = 0; nVert < vertices.size(); nVert++) {
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
    }

    fclose(out);

}


// TENHO QUE METER ROW E COLUMN, NÃO CHEGA DIV
void box(float x, float y, float z, int nDivisions, string fileName){

    FILE *out;
    fopen_s(&out, fileName.c_str(),"w"); //open to write (cleans if file already exists or creates it if not)

    if(out != NULL) {

        std::vector<Point> vertices; // Vector to store the vertices
        float unitx = x / nDivisions; // Size of a width unit (like an alpha in polar coordenates)
        float unity = y / nDivisions; // Size of a height unit (like an alpha in polar coordenates)
        float unitz = z / nDivisions; // Size of a length unit (like an alpha in polar coordenates)

        for(int col = 0; col < nDivisions; col++) {

            for(int dep = 0; dep < nDivisions; dep++) {

                // CIMA
                // F-E-D
                vertices.push_back(Point(unitx*(col+1), y, unitz*dep));
                vertices.push_back(Point(unitx*col, y, unitz*dep));
                vertices.push_back(Point(unitx*col, y, unitz*(dep+1)));
                // F-D-C
                vertices.push_back(Point(unitx*(col+1), y, unitz*dep));
                vertices.push_back(Point(unitx*col, y, unitz*(dep+1)));
                vertices.push_back(Point(unitx*(col+1), y, unitz*(dep+1)));

                // BAIXO
                // F-D-E
                vertices.push_back(Point(unitx*(col+1), 0, unitz*dep));
                vertices.push_back(Point(unitx*col, 0, unitz*(dep+1)));
                vertices.push_back(Point(unitx*col, 0, unitz*dep));
                // F-C-D
                vertices.push_back(Point(unitx*(col+1), 0, unitz*dep));
                vertices.push_back(Point(unitx*(col+1), 0, unitz*(dep+1)));
                vertices.push_back(Point(unitx*col, 0, unitz*(dep+1)));

            }

        }

        for(int dep = 0; dep < nDivisions; dep++) {

            for(int row = 0; row < nDivisions; row++) {

                // DIREITA
                // F-C-B
                vertices.push_back(Point(x, unity*(row+1), unitz*dep));
                vertices.push_back(Point(x, unity*(row+1), unitz*(dep+1)));
                vertices.push_back(Point(x, unity*row, unitz*(dep+1)));
                // F-B-G
                vertices.push_back(Point(x, unity*(row+1), unitz*dep));
                vertices.push_back(Point(x, unity*row, unitz*(dep+1)));
                vertices.push_back(Point(x, unity*row, unitz*dep));

                // ESQUERDA
                // F-B-C
                vertices.push_back(Point(0, unity*(row+1), unitz*dep));
                vertices.push_back(Point(0, unity*row, unitz*(dep+1)));
                vertices.push_back(Point(0, unity*(row+1), unitz*(dep+1)));
                // F-G-B
                vertices.push_back(Point(0, unity*(row+1), unitz*dep));
                vertices.push_back(Point(0, unity*row, unitz*dep));
                vertices.push_back(Point(0, unity*row, unitz*(dep+1)));

            }

        }

        for(int col = 0; col < nDivisions; col++) {

            for(int row = 0; row < nDivisions; row++) {

                // FRENTE
                // C-D-A
                vertices.push_back(Point(unitx*(col+1), unity*(row+1), z));
                vertices.push_back(Point(unitx*col, unity*(row+1), z));
                vertices.push_back(Point(unitx*col, unity*row, z));
                // C-A-B
                vertices.push_back(Point(unitx*(col+1), unity*(row+1), z));
                vertices.push_back(Point(unitx*col, unity*row, z));
                vertices.push_back(Point(unitx*(col+1), unity*row, z));

                // TRAS
                // C-A-D
                vertices.push_back(Point(unitx*(col+1), unity*(row+1), 0));
                vertices.push_back(Point(unitx*col, unity*row, 0));
                vertices.push_back(Point(unitx*col, unity*(row+1), 0));
                // C-B-A
                vertices.push_back(Point(unitx*(col+1), unity*(row+1), 0));
                vertices.push_back(Point(unitx*(col+1), unity*row, 0));
                vertices.push_back(Point(unitx*col, unity*row, 0));

            }

        }

        // Sending vertices to .3d file
        for(int nVert = 0; nVert < vertices.size(); nVert++) {
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }

    }

    fclose(out);
}


void sphere(float r, int slices, int stacks, string fileName){
    FILE *out;
    fopen_s(&out, fileName.c_str(), "w"); //open to write

    if( out != NULL){
        std::vector<Point> vertices; //Vector to store the vertices

        for (int slice = 0; slice < slices; slice++) {
		    for (int stack = 0; stack <= stacks; stack++) {
                float alpha1 = (2 * M_PI / slices) * slice;
                float alpha2 = (2 * M_PI / slices) * (slice + 1);
	            float beta1 = (2 * M_PI / stacks) * stack;
                float beta2 = (2 * M_PI / stacks) * (stack + 1);

			    vertices.push_back(Point(r*cos(beta2)*sin(alpha1), r*sin(beta2), r*cos(beta2)*cos(alpha1)));
			    vertices.push_back(Point(r*cos(beta1)*sin(alpha1), r*sin(beta1), r*cos(beta1)*cos(alpha1)));
			    vertices.push_back(Point(r*cos(beta2)*sin(alpha2), r*sin(beta2), r*cos(beta2)*cos(alpha2)));

    		    vertices.push_back(Point(r*cos(beta2)*sin(alpha2), r*sin(beta2), r*cos(beta2)*cos(alpha2)));
	    		vertices.push_back(Point(r*cos(beta1)*sin(alpha1), r*sin(beta1), r*cos(beta1)*cos(alpha1)));
		    	vertices.push_back(Point(r*cos(beta1)*sin(alpha2), r*sin(beta1), r*cos(beta1)*cos(alpha2)));
            }
        }

        // Sending vertices to .3d file
        for(int nVert = 0; nVert < vertices.size(); nVert++) {
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
	}

    fclose(out);
}


void cone(float r, float height, int slices, int stacks, string fileName){
    FILE *out;
    fopen_s(&out, fileName.c_str(), "w"); //open to write

    if( out != NULL){
        std::vector<Point> vertices; //Vector to store the vertices

        float alpha = 2*M_PI / slices;
        for (int slice = 0; slice < slices; slice++) {

        //BOTTOM - always drawn at XZplane = 0
		vertices.push_back(Point(0.0, 0.0, 0.0));    // centre
		vertices.push_back(Point(r*sin(alpha*(slice+1)), 0.0, r*cos(alpha*(slice+1))));
		vertices.push_back(Point(r*sin(alpha*slice), 0.0, r*cos(alpha*slice)));

            float lowerR = r;
            float lowerStackHeight = 0;
            for(int stack = 0; stack <= stacks; stack++){
                float upperStackHeight = height/stacks*stack;
                float upperR = (height - upperStackHeight) * r / height;

                //SIDES
                vertices.push_back(Point(lowerR*sin(alpha*slice), lowerStackHeight, lowerR*cos(alpha*slice)));
                vertices.push_back(Point(lowerR*sin(alpha*(slice+1)), lowerStackHeight, lowerR*cos(alpha*(slice+1))));
                vertices.push_back(Point(upperR*sin(alpha*slice), upperStackHeight, upperR*cos(alpha*slice)));

                vertices.push_back(Point(lowerR*sin(alpha*(slice+1)), lowerStackHeight, lowerR*cos(alpha*(slice+1))));
                vertices.push_back(Point(upperR*sin(alpha*(slice+1)), upperStackHeight, upperR*cos(alpha*(slice+1))));
                vertices.push_back(Point(upperR*sin(alpha*slice), upperStackHeight, upperR*cos(alpha*slice)));

                lowerR = upperR;
                lowerStackHeight = upperStackHeight;
            }

        }

        // Sending vertices to .3d file
        for(int nVert = 0; nVert < vertices.size(); nVert++) {
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
    }

    fclose(out);
}


void pyramid(float height, float width, float length, string fileName){
    FILE *out;
    fopen_s(&out, fileName.c_str(), "w"); //open to write

    if( out != NULL){
        std::vector<Point> vertices; //Vector to store the vertices

        //T1(CIMA, BAIXO, BAIXO)
        vertices.push_back(Point( 0.0, height, 0.0));
        vertices.push_back(Point(-length/2, 0.0, width/2));
        vertices.push_back(Point( length/2, 0.0, width/2));

        //T2
        vertices.push_back(Point( 0.0, height, 0.0));
        vertices.push_back(Point( length/2, 0.0, width/2));
        vertices.push_back(Point( length/2, 0.0, -width/2));

        //T3
        vertices.push_back(Point( 0.0, height, 0.0));
        vertices.push_back(Point(-length/2, 0.0, -width/2));
        vertices.push_back(Point(-length/2, 0.0, width/2));

        //T4
        vertices.push_back(Point( 0.0, height, 0.0));
        vertices.push_back(Point( length/2, 0.0, -width/2));
        vertices.push_back(Point(-length/2, 0.0, -width/2));

        //T5
        vertices.push_back(Point( length/2, 0.0, width/2));
        vertices.push_back(Point(-length/2, 0.0, width/2));
        vertices.push_back(Point(-length/2, 0.0, -width/2));

        //T6
        vertices.push_back(Point( length/2, 0.0, width/2));
        vertices.push_back(Point(-length/2, 0.0, -width/2));
        vertices.push_back(Point( length/2, 0.0, -width/2));

        // Sending vertices to .3d file
        int nVert;
        for(nVert = 0; nVert < vertices.size(); nVert++){
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
    }

    fclose(out);
}


/*
Point in the work report that stacks here are worthless
*/
void cylinder(float r, float height, int stacks, int slices, string fileName){
    FILE *out;
    fopen_s(&out, fileName.c_str(), "w"); //open to write

    if(out != NULL){
        std::vector<Point> vertices; //Vector to store the vertices

        float alpha = 2*M_PI / slices;
        for (int slice = 0; slice < slices; slice++) {

            //BOTTOM - always drawn at XZplane = 0
            vertices.push_back(Point(0.0, 0.0, 0.0));
            vertices.push_back(Point(r*sin(alpha*(slice + 1)), 0.0, r*cos(alpha*(slice+1))));
            vertices.push_back(Point(r*sin(alpha*slice), 0.0, r*cos(alpha*slice)));

            //TOP - always drawn at XZplane = height
            vertices.push_back(Point(0.0, height, 0.0));
            vertices.push_back(Point(r*sin(alpha*slice), height, r*cos(alpha*slice)));
            vertices.push_back(Point(r*sin(alpha*(slice+1)), height, r*cos(alpha*(slice+1))));

            float lowerStackHeight = 0;
            for(int stack = 0; stack <= stacks; stack++){
                float upperStackHeight = height/stacks*stack;

                //SIDES
                vertices.push_back(Point(r*sin(alpha*slice), lowerStackHeight, r*cos(alpha*slice)));
                vertices.push_back(Point(r*sin(alpha*(slice+1)), lowerStackHeight, r*cos(alpha*(slice+1))));
                vertices.push_back(Point(r*sin(alpha*slice), upperStackHeight, r*cos(alpha*slice)));

                vertices.push_back(Point(r*sin(alpha*(slice+1)), lowerStackHeight, r*cos(alpha*(slice+1))));
                vertices.push_back(Point(r*sin(alpha*(slice+1)), upperStackHeight, r*cos(alpha*(slice+1))));
                vertices.push_back(Point(r*sin(alpha*slice), upperStackHeight, r*cos(alpha*slice)));

                lowerStackHeight = upperStackHeight;
            }
        }

        // Sending vertices to .3d file
        int nVert;
        for(nVert = 0; nVert < vertices.size(); nVert++){
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
    }

    fclose(out);
}

/**
 * Parte 3 - readPatchFile
 * Lê um ficheiro Patch
 * numberOfIndexes é O número de indíces de cada patch
 */
void readPatchFile(unsigned int** patches, float** controlPoints, string path, int numberOfIndexes) {
    ifstream file(path.c_str());
    string firstLine;
    getline(file, firstLine);
    int numberOfPatches = atoi(firstLine.c_str());
    // Allocate space for all entries
    *patches = (unsigned int *) malloc(sizeof(unsigned int) * numberOfPatches * numberOfIndexes);

    // Get patches from file
    string line;
    for (int i = 0; i < numberOfPatches; i++) {
        getline(file, line);
        istringstream entries(line);
        string controlPointIndex;
        // Get control point indices
        for (int j = 0; j < numberOfIndexes && getline(entries, controlPointIndex, ','); j++) {
            (*patches)[i * numberOfIndexes + j] = atoi(controlPointIndex.c_str());
        }
    }

    // Get control points from file
    string cpLine; // Control Points Line
    getline(file, cpLine);
    int numberOfControlPoints = atoi(cpLine.c_str());
    *controlPoints = (float *) malloc(sizeof(float) * 3 * numberOfControlPoints ); /* Three cordinates per control point */ 

    for (int k = 0; k < numberOfControlPoints; k++) { // Loop over control point lines
        getline(file, line);
        istringstream cpStream(line); // Control Points Stream
        string cpEntry; // Control Point Entry
        for (int l = 0; l < 3 && getline(cpStream, cpEntry, ','); l++) { // Loop over control points
            (*controlPoints)[k * 3 + l] = (float) atof(cpEntry.c_str());
        }
    }
}

/**
 * Parte 3 - getBezierPoint
 * Recebe vetor de patches e pontos de controlo,
 * número de indíces, número do patch pretendido,
 * e u & v que controlam a tesselation.
 */
Point getBezierPoint(unsigned int* patches, float* controlPoints, int numberOfIndexes, int patchNumber, float u, float v) {
    // Initialize point
    Point p(0, 0, 0);

    // Polinómio de Bernstein
    float bernsteinU[4] = { powf(1 - u, 3), 3 * u * powf(1 - u, 2), 3 * powf(u, 2) * (1 - u), powf(u, 3) };
    float bernsteinV[4] = { powf(1 - v, 3), 3 * v * powf(1 - v, 2), 3 * powf(v, 2) * (1 - v), powf(v, 3) };

    // Bezier Patch sum
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int indexInPatch = i * 4 + j;
            int indexCP = patches[patchNumber * numberOfIndexes + indexInPatch];
            p.setX(p.getX() + controlPoints[indexCP * 3 + 0] * bernsteinU[i] * bernsteinV[j]);
            p.setY(p.getY() + controlPoints[indexCP * 3 + 1] * bernsteinU[i] * bernsteinV[j]);
            p.setZ(p.getZ() + controlPoints[indexCP * 3 + 2] * bernsteinU[i] * bernsteinV[j]);
        }
    }
    return p;
}

/**
 * Parte 3 - Make Bezier
 * 1. Lê um ficheiro patch e armazena os dados;
 * 2. Gera a superfície de Bezier com um dado
 *    nível de tesselation;
 * 3. Com recurso a polinomiais de Bernstein,
 *    calcula os pontos do modelo.
 */
void bezier(string pathToPatchFile, string pathTo3DFile, int tesselationLevel) {
    // Read Patch file
    const int numberOfIndexes = 16; // Number of indexes on patch file
    unsigned int* patches = NULL;
    float* controlPoints = NULL;

    readPatchFile(&patches, &controlPoints, pathToPatchFile, numberOfIndexes);
    std::vector<Point> points; // Points will be stored here
    
    // Iterate over patches
    for (int i = 0; i < 32; i++) {
        // Achieve desired tesselation level
        for (int j = 0; j < tesselationLevel; j++) { // j = tesselation V
            float v = (float) j / tesselationLevel;
            for (int k = 0; k < tesselationLevel; k++) { // k = tesselation U
                float u = (float) k / tesselationLevel;

                // First triangle
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, u, v));
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, (u + (1.0f / tesselationLevel)), v));
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
                // Second Triangle
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, (u + (1.0f / tesselationLevel)), (v + (1.0f / tesselationLevel))));
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, u, (v + (1.0f / tesselationLevel))));
                points.push_back(getBezierPoint(patches, controlPoints, numberOfIndexes, i, u, v));
            }
        }
    }

    FILE *f;
    fopen_s(&f, pathTo3DFile.c_str(), "w");

    fprintf(f, "%d\n", points.size() / 3);

    // Print all coordinates to the generated 3D file
    for (int i = 0; i < points.size(); i++) {
        fprintf(f, "%f %f %f\n", points[i].getX(), points[i].getY(), points[i].getZ());
    }

    fclose(f);

    // Free memory allocated in readPatchFile
    free(patches);
    free(controlPoints);
}

int main(int argc, char** argv) {

    SetConsoleOutputCP(CP_UTF8); // UTF-8 mode for Windows console

    if(argv[1] != NULL) { // Checks if program receives nothing to process

        std::string form = argv[1];
        std::cout << form << " é argv1." << std::endl;

        // PLANE
        if(form.compare("plane") == 0 && argc == 4) {
            std::cout << "Está a criar um plano." << std::endl;

            // Prints width and filename just for DEBUG purposes
            std::cout << "WIDTH: " << argv[2] << std::endl;
            float width = std::stof(argv[2]);
            std::cout << width << std::endl;

            std::cout << "FILENAME: " << argv[3] << std::endl;

            // Call triangle criation function
            plane(width,argv[3]);
        }
        // BOX
        else if(form.compare("box") == 0 && argc == 7) {
            std::cout << "Está a criar um cubo." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "LARGURA: " << argv[2] << std::endl;
            float largura = std::stof(argv[2]);
            std::cout << largura << std::endl;

            std::cout << "ALTURA: " << argv[3] << std::endl;
            float altura = std::stof(argv[3]);
            std::cout << altura << std::endl;

            std::cout << "COMPRIMENTO: " << argv[4] << std::endl;
            float comprimento = std::stof(argv[4]);
            std::cout << comprimento << std::endl;

            std::cout << "DIVISIONS: "<< argv[5] << std::endl;
            int divisions = std::stoi(argv[5]);
            std::cout << divisions << std::endl;

            std::cout << "FILENAME: " << argv[6] << std::endl;

            // Call triangle criation function
            box(largura,altura,comprimento,divisions,argv[6]);
        }
        // SPHERE
        else if(form.compare("sphere") == 0 && argc == 6) {
            std::cout << "Está a criar uma esfera." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "RAIO= " << argv[2] << std::endl;
            float raio = std::stof(argv[2]);
            std::cout << raio << std::endl;

            std::cout << "SLICES= " << argv[3] << std::endl;
            int slices = std::stoi(argv[3]);
            std::cout << slices << std::endl;

            std::cout << "STACKS= " << argv[4] << std::endl;
            int stacks = std::stoi(argv[4]);
            std::cout << stacks << std::endl;

            std::cout << "FILENAME: " << argv[5] << std::endl;

            // Call triangle criation function
            sphere(raio,slices,stacks,argv[5]);
        }
        // CONE
        else if(form.compare("cone") == 0 && argc == 7) {
            std::cout << "Está a criar um cone." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "RAIO= " << argv[2] << std::endl;
            float raio = std::stof(argv[2]);
            std::cout << raio << std::endl;

            std::cout << "ALTURA= " << argv[3] << std::endl;
            float altura = std::stof(argv[3]);
            std::cout << altura << std::endl;

            std::cout << "SLICES= " << argv[4] << std::endl;
            int slices = std::stoi(argv[4]);
            std::cout << slices << std::endl;

            std::cout << "STACKS= " << argv[5] << std::endl;
            int stacks = std::stoi(argv[5]);
            std::cout << stacks << std::endl;

            std::cout << "FILENAME: " << argv[6] << std::endl;

            // Call triangle criation function
            cone(raio,altura,slices,stacks,argv[6]);
        }
        // PYRAMID
        else if(form.compare("pyramid") == 0 && argc == 6) {
            std::cout << "Está a criar uma pirâmide." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "ALTURA= " << argv[2] << std::endl;
            float altura = std::stof(argv[2]);
            std::cout << altura << std::endl;

            std::cout << "LARGURA= " << argv[3] << std::endl;
            float largura = std::stof(argv[3]);
            std::cout << largura << std::endl;

            std::cout << "COMPRIMENTO= " << argv[4] << std::endl;
            float comprimento = std::stof(argv[4]);
            std::cout << comprimento << std::endl;

            std::cout << "FILENAME: " << argv[5] << std::endl;

            // Call triangle criation function
            pyramid(altura,largura,comprimento,argv[5]);
        }
        // CYLINDER - ESTÁ A DAR MAL, SÓ APARECE UMA FATIA EM CIMA
        else if(form.compare("cylinder") == 0 && argc == 7) {
            std::cout << "Está a criar um cilindro." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "RAIO= " << argv[2] << std::endl;
            float raio = std::stof(argv[2]);
            std::cout << raio << std::endl;
            std::cout << "ALTURA= " << argv[3] << std::endl;
            float altura = std::stof(argv[3]);
            std::cout << altura << std::endl;
            std::cout << "STACKS= " << argv[4] << std::endl;
            int slices = std::stoi(argv[4]);
            std::cout << slices << std::endl;
            std::cout << "SLICES= " << argv[5] << std::endl;
            int fatias = std::stoi(argv[5]);
            std::cout << fatias << std::endl;
            std::cout << "FILENAME: " << argv[6] << std::endl;


            // Call triangle criation function
            cylinder(raio,altura,slices,fatias,argv[6]);
        }
        // Parte 3 - Bezier
        // Requer 5 argumentos: generator bezier <pathToPatchFile> <pathTo3DFile> <tesselationLevel>
        else if (form.compare("bezier") == 0 && argc >= 5)
        {
            bezier(argv[2], argv[3], atoi(argv[4]));
            
        }
        else {
            std::cout << argv[1] << " não é válido." << std::endl;
        }
    }


    return 0;
}