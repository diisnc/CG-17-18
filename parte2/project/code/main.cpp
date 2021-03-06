#define _USE_MATH_DEFINES
#include <math.h>
#include "engine.h"
#include "xml-loader.h"

// Glut has to be included last to avoid errors
// (for example, C2381 'exit': redefinition; '__declspec(noreturn)' or '[[noreturn]]' differs)
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLUquadric* qobj;
float translateX = 0;
float translateZ = 0;
float rotateXZ = 0;
float rotateY = 0;
float rotateX = 0;
GLenum mode = GL_FILL;

/*
TODO:
•	Plane (a square in the XZ plane, centred in the origin, made with 2 triangles)
•	Box (requires X, Y and Z dimensions, and optionally the number of divisions)
•	Sphere (requires radius, slices and stacks)
•	Cone (requires bottom radius, height, slices and stacks)
*/


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void axis_system() {

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);

	glEnd();
}

void renderScene(void) {

	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);

	// Clear buffers (depth and color)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	axis_system();

	// Geometric transformations
	glTranslatef(translateX, 0, translateZ);
	glRotatef(rotateXZ, 1.0, 0.0, -1.0);
	glRotatef(rotateY, 0.0, 1.0, 0.0);
	glRotatef(rotateX, 1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, mode);
	
	// Draw frame
	engine::drawFrame();

	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events
void keyboardSpecialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: translateZ--; break; //move-se sobre o eixoZ
		case GLUT_KEY_DOWN: translateZ++; break;
		case GLUT_KEY_LEFT: translateX--; break; //move-se sobre o eixoX
		case GLUT_KEY_RIGHT: translateX++; break;
		case GLUT_KEY_F1: mode = GL_LINE; break;
		case GLUT_KEY_F2: mode = GL_FILL; break;
		case GLUT_KEY_F3: mode = GL_POINT; break;
	}
	glutPostRedisplay();
}

void keyboardAuxFunc(unsigned char key, int x, int y) {
	switch (key) {
		case 'w': rotateXZ -= 10; break;
		case 's': rotateXZ += 10; break;
		case 'a': rotateY -= 10; break;
		case 'd': rotateY += 10; break;
		case 'n': rotateX += 10; break;
		case 'm': rotateX -= 10; break;
	}
	glutPostRedisplay();
}

float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}

	glutPostRedisplay();
}

void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);

	glutPostRedisplay();
}

int main(int argc, char **argv) {

	// Init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Parte 2");
		
	// Callback registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Register keyboard callbacks
	glutSpecialFunc(keyboardSpecialKeys);
	glutKeyboardFunc(keyboardAuxFunc);

	// Register mouse callbacks
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	
	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //tirar para ver a parte de baixo

	// Load scene XML
	engine::modelTree scene = xmlLoader::loadSceneXML("scene.xml");

	// Send scene models to engine
	engine::loadScene(scene);

	// Enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
