#include <iostream>
#include <fstream>  
#include <string>
#include <vector>
#include <Windows.h>
#include "Point.h"

#define _USE_MATH_DEFINES
#include <math.h>

using std::string;

/*

TODO:

•   Plane (a square in the XZ plane, centred in the origin, made with 2 triangles)
•   Box (requires X, Y and Z dimensions, and optionally the number of divisions)
•   Sphere (requires radius, slices and stacks)
•   Cone (requires bottom radius, height, slices and stacks)
•   EXTRA: • pyramid (height, width)
           • cylinder (radius, height, length, slices)

*/

/*
void plane(float width, string filename){
    FILE *out;
    fopen_s(&out, filename.c_str(),"w"); //open to write (cleans if file already exists or creates it if not)

    if( out != NULL ){
        std::vector<Point> vertices; // Vector to store the vertices

        //T1
        vertices.push_back(Point( width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0, width/2));
        vertices.push_back(Point( width/2, 0.0, width/2));

        //T2
        vertices.push_back(Point( width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0,-width/2));
        vertices.push_back(Point(-width/2, 0.0, width/2));
    }

    int nVert;
    for(nVert = 0, nVert < vertices.size(); nVert++){
        fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
    }
    fclose(out);

}

//box is placed on XZ plane
void box(float xcoord, float ycoord, float zcoord, int nDivisions, string filename){


}

void sphere(float radius, int slices, int stacks, string filename){


}

void cone(float radius, float height, int slices, int stacks, string filename){


}

void pyramid(float height, float width, float length, string filename){
    FILE *out;
    fopen_s(&out, filename.c_str(),"w"); //open to write

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

    }

    int nVert;
    for(nVert = 0, nVert < vertices.size(); nVert++){
        fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
    }
    fclose(out);

}

void cylinder(float radius, float height, int slices, string filename){


}
*/


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
            // plane(width,argv[3]);
        }
        // BOX
        else if(form.compare("box") == 0 && argc == 7) {
            std::cout << "Está a criar um cubo." << std::endl;
            
            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "POINT: (" << argv[2] << "," << argv[3] << "," << argv[4] << ")" << std::endl;
            float x = std::stof(argv[2]);
            std::cout << x << std::endl;
            float y = std::stof(argv[3]);
            std::cout << y << std::endl;
            float z = std::stof(argv[4]);
            std::cout << z << std::endl;

            std::cout << "DIVISIONS: "<< argv[5] << std::endl;
            int divisions = std::stoi(argv[5]);
            std::cout << divisions << std::endl;

            std::cout << "FILENAME: " << argv[6] << std::endl;

            // Call triangle criation function
            // box(x,y,z,divisions,argv[6]);
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
            // sphere(raio,slices,stacks,argv[5]);
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
            // cone(raio,altura,slices,stacks,argv[6]);
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
            // pyramid(altura,largura,comprimento,argv[5]);
        }
        // CYLINDER
        else if(form.compare("cylinder") == 0 && argc == 6) {
            std::cout << "Está a criar um cilindro." << std::endl;

            // Prints Coordinates, Number of Divisions and Filename just for DEBUG purposes
            std::cout << "RAIO= " << argv[2] << std::endl;
            float raio = std::stof(argv[2]);
            std::cout << raio << std::endl;
            std::cout << "ALTURA= " << argv[3] << std::endl;
            float altura = std::stof(argv[3]);
            std::cout << altura << std::endl;
            std::cout << "SLICES= " << argv[4] << std::endl;
            int fatias = std::stoi(argv[4]);
            std::cout << fatias << std::endl;
            std::cout << "FILENAME: " << argv[5] << std::endl;
            
            // Call triangle criation function
            // cylinder(raio,altura,fatias,argv[5]);
        }
        else {
            std::cout << argv[1] << " não é válido." << std::endl;
        }
    }


    return 0;
    
}