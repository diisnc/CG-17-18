#include <iostream>
#include <fstream>  
#include <string>
#include <vector>
#include "point.h"
#include <math.h>
#define _USE_MATH_DEFINES
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
    fopen_s(&out, fileName.c_str(), "w"); //open to write

    if( out != NULL ){
        std::vector<Point> vertices; //Vector to store the vertices

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
    for(nVert = 0; nVert < vertices.size(); nVert++){
        fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
    }
    fclose(out);

}

//box is placed on XZ plane
void box(float xcoord, float ycoord, float zcoord, int nDivisions, string fileName){


}

void sphere(float radius, int slices, int stacks, string fileName){


}

void cone(float radius, float height, int slices, int stacks, string fileName){


}

//NOTA: CRIAR NA MAIN PARA A PIRAMIDE
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

    }

    int nVert;
    for(nVert = 0; nVert < vertices.size(); nVert++){
        fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
    }
    fclose(out);

}

/*
DÚVIDA:
    - AS STACKS SÃO WORTH?
*/
void cylinder(float radius, float height, int stacks, int slices, string fileName){
    FILE *out;
    fopen_s(&out, fileName.c_str(), "w"); //open to write
    float alpha = 2*M_PI/slices;

    if( out != NULL){
        std::vector<Point> vertices; //Vector to store the vertices

        float alpha = 2*M_PI / slices; 
        for (int slice = 0; slice < slices; slice++) {
            
            //BOTTOM - always drawn at XZplane = 0
            vertices.push_back(Point(0.0, 0.0, 0.0 ));
            vertices.push_back(Point(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1))));
            vertices.push_back(Point(radius * sin(alpha * slice), -height / 2, radius * cos(alpha * slice)));

            //TOP - always drawn at XZplane = height
            vertices.push_back(Point( 0.0, height, 0.0 ));
            vertices.push_back(Point(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice)));
            vertices.push_back(Point(radius * sin(alpha * (slice + 1)), height / 2, radius * cos(alpha * (slice + 1))));

            for(){
            
            //SIDES
            vertices.push_back(Point(  ));
            vertices.push_back(Point(  ));
            vertices.push_back(Point(  ));

            vertices.push_back(Point(  ));
            vertices.push_back(Point(  ));
            vertices.push_back(Point(  ));

            }
        }

        int nVert;
        for(nVert = 0; nVert < vertices.size(); nVert++){
            fprintf(out, "%f %f %f \n", vertices[nVert].getX(), vertices[nVert].getY(), vertices[nVert].getZ());
        }
        
        fclose(out);
}


int main(int argc, char** argv) {

    SetConsoleOutputCP(CP_UTF8); // UTF-8 mode for Windows console

    if(argv[1] != NULL) { // Checks if no argv[1] is not empty

        std::string form = argv[1];
        std::cout << form << " é argv1." << std::endl;

        // PLANE
        if(form.compare("plane") == 0 && argc == 21) {
            std::cout << "Está a criar um plano." << std::endl;
            // Create file and his output stream
            std::ofstream outfile (argv[20]);
            // Read inputs and put them in file, line by line
            for(int i = 1; i < argc-1; i++) { // argc-1 because file name is not a vertex coordinate
                outfile << argv[i] << std::endl;
            }
            outfile.close();

            std::cout << "Triângulo 1:" << std::endl;
            std::cout << "A= (" << argv[2] << "," << argv[3] << "," << argv[4] << ")" << std::endl;
            std::cout << "B= (" << argv[5] << "," << argv[6] << "," << argv[7] << ")" << std::endl;
            std::cout << "C= (" << argv[8] << "," << argv[9] << "," << argv[10] << ")" << std::endl;
            std::cout << "Triângulo 2:" << std::endl;
            std::cout << "A= (" << argv[11] << "," << argv[12] << "," << argv[13] << ")" << std::endl;
            std::cout << "B= (" << argv[14] << "," << argv[15] << "," << argv[16] << ")" << std::endl;
            std::cout << "C= (" << argv[17] << "," << argv[18] << "," << argv[19] << ")" << std::endl;

        }
        // BOX
        else if(form.compare("box") == 0 && argc == 27) {
            std::cout << "Está a criar um quadrado." << std::endl;
            // Create file and his output stream
            std::ofstream outfile (argv[26]);
            // Read inputs and put them in file, line by line
            for(int i = 1; i < argc-1; i++) {
                outfile << argv[i] << std::endl;
            }
            outfile.close();

            std::cout << "A= (" << argv[2] << "," << argv[3] << "," << argv[4] << ")" << std::endl;
            std::cout << "B= (" << argv[5] << "," << argv[6] << "," << argv[7] << ")" << std::endl;
            std::cout << "C= (" << argv[8] << "," << argv[9] << "," << argv[10] << ")" << std::endl;
            std::cout << "D= (" << argv[11] << "," << argv[12] << "," << argv[13] << ")" << std::endl;
            std::cout << "E= (" << argv[14] << "," << argv[15] << "," << argv[16] << ")" << std::endl;
            std::cout << "F= (" << argv[17] << "," << argv[18] << "," << argv[19] << ")" << std::endl;
            std::cout << "G= (" << argv[20] << "," << argv[21] << "," << argv[22] << ")" << std::endl;
            std::cout << "H= (" << argv[23] << "," << argv[24] << "," << argv[25] << ")" << std::endl;
        }
        // SPHERE
        else if(form.compare("sphere") == 0 && argc == 6) {
            std::cout << "Está a criar uma esfera." << std::endl;
            // Create file and his output stream
            std::ofstream outfile (argv[5]);
            // Read inputs and put them in file, line by line
            for(int i = 1; i < argc-1; i++) {
                outfile << argv[i] << std::endl;
            }
            outfile.close();

            std::cout << "Raio= " << argv[2] << std::endl;
            std::cout << "Slices= " << argv[3] << std::endl;
            std::cout << "Stacks= " << argv[4] << std::endl;
        }
        // CONE
        else if(form.compare("cone") == 0 && argc == 7) {
            std::cout << "Está a criar um cone." << std::endl;
            // Create file and his output stream
            std::ofstream outfile (argv[6]);
            // Read inputs and put them in file, line by line
            for(int i = 1; i < argc-1; i++) {
                outfile << argv[i] << std::endl;
            }
            outfile.close();

            std::cout << "Raio= " << argv[2] << std::endl;
            std::cout << "Altura= " << argv[3] << std::endl;
            std::cout << "Slices= " << argv[4] << std::endl;
            std::cout << "Stacks= " << argv[5] << std::endl;
        }
        // CYLINDER
        else if(form.compare("cylinder") == 0 && argc == 6) {
            std::cout << "Está a criar um cilindro." << std::endl;
            // Create file and his output stream
            std::ofstream outfile (argv[5]);
            // Read inputs and put them in file, line by line
            for(int i = 1; i < argc-1; i++) {
                outfile << argv[i] << std::endl;
            }
            outfile.close();

            std::cout << "Raio= " << argv[2] << std::endl;
            float raio = std::stof(argv[2]);
            std::cout << raio << std::endl;
            std::cout << "Altura= " << argv[3] << std::endl;
            float altura = std::stof(argv[3]);
            std::cout << altura << std::endl;
            std::cout << "Slices= " << argv[4] << std::endl;
            float fatias = std::stof(argv[4]);
            std::cout << fatias << std::endl;
            
            //getCylinder(raio,altura,fatias);
        }
        else {
            std::cout << argv[1] << " não é válido." << std::endl;
        }
    }


    return 0;
    
}
/*
void getCylinder(float radius, float height, int slices) {

	float alpha = (2 * M_PI) / slices;
	int i = 0;

	for (i = 0; i < slices; i++) {

        // BOTTOM
        std::cout << 0 << std::endl;
        std::cout << "Raio= " << argv[2] << std::endl;
        std::cout << "Raio= " << argv[2] << std::endl;
        std::cout << "Raio= " << argv[2] << std::endl;
        std::cout << "Raio= " << argv[2] << std::endl;
        std::cout << "Raio= " << argv[2] << std::endl;
		glVertex3f(0, 0, 0);
		glVertex3f(sin(alpha*(i + 1))*radius,0,cos(alpha*(i+1))*radius);
		glVertex3f(sin(alpha*(i))*radius, 0, cos(alpha*(i))*radius);

		// TOP
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1 - (0.01 * i), 0);
		glVertex3f(0, height, 0);
		glVertex3f(sin(alpha*(i))*radius, height, cos(alpha*(i))*radius);
		glVertex3f(sin(alpha*(i + 1))*radius, height, cos(alpha*(i + 1))*radius);
		glEnd();

		// MIDDLE FIRST TRIANGLE
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1 - (0.01 * i), 0);
		glVertex3f(sin(alpha*(i))*radius, 0, cos(alpha*(i))*radius);
		glVertex3f(sin(alpha*(i + 1))*radius, height, cos(alpha*(i + 1))*radius);
		glVertex3f(sin(alpha*(i))*radius, height, cos(alpha*(i))*radius);
		glEnd();

		// MIDDLE SECOND TRIANGLE
		glBegin(GL_TRIANGLES);
		glColor3f(1 - (0.01 * i), 1 - (0.01 * i), 0);
		glVertex3f(sin(alpha*(i))*radius, 0, cos(alpha*(i))*radius);
		glVertex3f(sin(alpha*(i + 1))*radius, 0, cos(alpha*(i + 1))*radius);
		glVertex3f(sin(alpha*(i + 1))*radius, height, cos(alpha*(i + 1))*radius);
		glEnd();


	}

}
*/