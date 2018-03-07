#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "engine.h"
#include "xml-loader.h"

GLUquadric* qobj;
float translateX = 0;
float translateZ = 0;
float rotateXZ = 0;
float rotateY = 0;
float rotateX = 0;
GLenum mode = GL_FILL;

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

int main(int argc, char **argv) {

	// Init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
	// Callback registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Register keyboard callbacks
	glutSpecialFunc(keyboardSpecialKeys);
	glutKeyboardFunc(keyboardAuxFunc);
	
	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //tirar para ver a parte de baixo

	// Load scene XML
	std::vector<engine::model> scene = xmlLoader::loadSceneXML("scene.xml");

	// Send scene models to engine
	engine::loadScene(scene);

	// Enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
