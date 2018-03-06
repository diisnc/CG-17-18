#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

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


float translateX = 0;
float translateZ = 0;
float rotateLR = 0; //left right
float rotateUP = 0; //up down

void pyramid () {
	// put drawing instructions here
	glBegin(GL_TRIANGLES);
	glColor3f(1.0 , 0.5 , 0.0 );
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);

	glColor3f(0, 1, 1);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glColor3f(1.0, 0.5, 0.0);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	glColor3f(0, 1, 1);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);

	glColor3f(0.5, 1, 0);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);

	glColor3f(0.5, 1, 0);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);

	glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	// put the geometric transformations here
	glTranslatef(translateX, 0, translateZ);
	glRotatef(rotateLR, 1.0, 0.0, -1.0);
	glRotatef(rotateUP, 0.0, 1.0, 0.0);

	// put drawing geometric transformations here
	pyramid();
	
	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events
void keyboardSpecialKeys(int key_code, int x, int y) {
	switch (key_code) {
		case GLUT_KEY_UP: translateZ--; break;
		case GLUT_KEY_DOWN: translateZ++; break;
		case GLUT_KEY_LEFT: translateX--; break;
		case GLUT_KEY_RIGHT: translateX++; break;
	}
	glutPostRedisplay();
}

void keyboardAuxFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': rotateLR += 10; break;
	case 's': rotateLR -= 10; break;
	case 'a': rotateUP -= 10; break;
	case 'd': rotateUP += 10; break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// put here the registration of the keyboard callbacks
	glutSpecialFunc(keyboardSpecialKeys);
	glutKeyboardFunc(keyboardAuxFunc);;


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
