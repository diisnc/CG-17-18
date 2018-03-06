

#include<stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define _USE_MATH_DEFINES
#include "tinyxml2.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace tinyxml2;
using namespace std;

float actX = 0.0f;
float actZ = 0.0f;
float actY = 0.0f;
float altura = 2.0f;
typedef struct ponto { float x, y, z; } Ponto;
typedef struct triangulo { Ponto p1, p2, p3; } Triangulo;
int vez = 0;
vector <Triangulo> vetor;

void desenharFigura(vector<Triangulo> triangulosFigura) {
	vector<Triangulo>::iterator iteradorTriangulos;

	for (iteradorTriangulos = triangulosFigura.begin(); iteradorTriangulos != triangulosFigura.end(); iteradorTriangulos++) {
		glBegin(GL_TRIANGLES);
			glVertex3f(iteradorTriangulos->p1.x, iteradorTriangulos->p1.y, iteradorTriangulos->p1.z);
			glVertex3f(iteradorTriangulos->p2.x, iteradorTriangulos->p2.y, iteradorTriangulos->p2.z);
			glVertex3f(iteradorTriangulos->p3.x, iteradorTriangulos->p3.y, iteradorTriangulos->p3.z);
		glEnd();
	}
}

void desenharCena() {
	XMLDocument xmlDoc;
	XMLError loadOk = xmlDoc.LoadFile("scene.xml");
	std::vector<string> todasFiguras;

	// Abrir o XML e retirar os atributos (figuras.3d)
	if (loadOk == XML_SUCCESS) {
		XMLNode * pRoot = xmlDoc.FirstChildElement("scene");	// vai buscar a scene
		if (pRoot == nullptr) return;	//XML_ERROR_FILE_READ_ERROR

		XMLElement * pListElement = pRoot->FirstChildElement("model");	//vai percorrer todos os models
		

		for (; pListElement != nullptr; pListElement = pListElement->NextSiblingElement("model")) {
			string novaFigura = pListElement->Attribute("file");	//get atributo file
			todasFiguras.push_back(novaFigura);	// guarda no vetor
		}
	}
	else return;	// N abriu o xml

	//Abrir todas as figuras e desenha-las
	vector<string>::iterator iteradorFiguras;
	for (iteradorFiguras = todasFiguras.begin(); iteradorFiguras != todasFiguras.end(); iteradorFiguras++) {
		// Guardar todos os triangulos do ficheiro em memória

		ifstream file(iteradorFiguras->c_str()); // abre ficheiro.3d da figura
		string ponto;
		vector<Ponto> todosPontos;

		//lê ficheiro linha a linha
		for (int flagNrTriangulos = 1; getline(file, ponto);) { 
			if (flagNrTriangulos != 1) {
				istringstream iss(ponto);
				string token;
				Ponto p;

				// tokenize de cada linha
				for (int posicao = 0; getline(iss, token, ' '); posicao++) {
					switch (posicao) {
					case 0: p.x = stof(token); break;
					case 1: p.y = stof(token); break;
					case 2: p.z = stof(token);
							todosPontos.push_back(p);
							break;
					}
				}
			}
			// se for a primeira linha (nr de triangulos no ficheiro) não é para fazer nada
			else { flagNrTriangulos = 0; }
		}

		file.close();
		vector<Ponto>::iterator iteradorPontos;
		vector<Triangulo> triangulosFigura;
		int contador = 1;

		for (iteradorPontos = todosPontos.begin(); iteradorPontos != todosPontos.end(); iteradorPontos++, contador++) {
			Triangulo t;
			switch (contador)
			{
				case 1: t.p1 = *iteradorPontos; break;
				case 2: t.p2 = *iteradorPontos; break;
				case 3: t.p3 = *iteradorPontos; contador = 0; vetor.push_back(t); break;
			}
		}

		desenharFigura(vetor);
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


float beta = M_PI / 2;
float alpha = 0;
float raio = 6;
float dx = 1.0;
float dy = 1.0;
float dz = 1.0;
float k = 1 / 10;

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Ponto p, plinha;
	p.x = raio*cos(beta)*sin(alpha);
	p.y = raio* sin(beta);
	p.z = raio* cos(beta)*cos(alpha);


	plinha.x = p.x + dx*k;
	plinha.y = p.y + dy*k;
	plinha.z = p.z + dz*k;

	// set the camera
	glLoadIdentity();
	gluLookAt(p.x, p.y, p.z,
		0, 0, 0,
		0.0f, 1.0f, 0.0f);

	// put drawing instructions here

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glutWireCone(3, 3, 100, 100);

	if (vez == 0) {
		desenharCena();
		vez++;
	}

	else desenharFigura(vetor);
	glLoadIdentity();

	
	// End of frame
	glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
	switch (key) {

	case ('a') : alpha += 0.3f;
		glutPostRedisplay();
		break;

	case ('d') : alpha -= 0.3f;
		glutPostRedisplay();
		break;


	case ('w') : beta += 0.3f;
		glutPostRedisplay();
		break;


	case ('s') : ; beta -= 0.3f;
		glutPostRedisplay();
		break;

	case ('z') : ; raio -= 0.3f;
		glutPostRedisplay();
		break;

	case ('x') : ; raio += 0.3f;
		glutPostRedisplay();
		break;

	case 27: exit(0);
		break;

	default:
		break;

	}
}

void specialkeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:	// On Left-Key
						//ycam = ( ycam - 2 ) % 360;	// Move camera one notch to the left
		dx -= 0.1f;
		dy -= 0.1f;
		dz -= 0.1f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:	// On Right-Key
							//ycam = ( ycam + 2 ) % 360;	// Move camera one notch to the right
		dx += 0.1f;
		dy += 0.1f;
		dz += 0.1f;
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:	// On Key-Up
						//xcam = ( xcam - 2 ) % 360;	// Move camera one notch to the up
		k += 0.1;

		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:	// On Key-Down
						//xcam = ( xcam + 2 ) % 360;	// Move camera one notch to the down
		k -= 0.1;

		glutPostRedisplay();
		break;

	default:
		break;
	}
	glutPostRedisplay(); // just update here....

}



int main(int argc, char **argv) {

	// put init here
	glutInit(&argc, argv); // Inicia o glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Define as propriedades da janela
	glutInitWindowPosition(100, 100); // top left window position
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG Etapa 1");

	// put callback registration here
	glutDisplayFunc(renderScene); // callback funcao responsavel pelos windows contents
	glutReshapeFunc(changeSize); // chamada quando a janela é criada e mudada de tamanho
	//glutIdleFunc(renderScene); // esta funcao é chamada quando a queue de eventos esta vazia
	// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutKeyboardFunc(teclado);
	//glutMouseFunc(mouseButton);
	glutSpecialFunc(specialkeys);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}

