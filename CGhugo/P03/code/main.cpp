#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alpha = 0;
float beta = 0;
float radius2 = 5;

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


void drawCylinder(float radius, float height, int slices) {

	// put code to draw cylinder in here
	float alpha = 2 * M_PI / slices;

	for (int slice = 0; slice <= slices ; slice++) {
		// BOTTOM
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0); // red
		glVertex3f(0.0f, -height / 2, 0.0f);    // centre
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * slice), -height / 2, radius * cos(alpha * slice));
		glEnd();

		// TOP
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1); // blue
		glVertex3f(0.0f, height / 2, 0.0f); // centre
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * (slice + 1)), height / 2, radius * cos(alpha * (slice + 1)));
		glEnd();

		// SIDES
		glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 0);
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * slice), -height / 2, radius * cos(alpha * slice));
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(0, 1, 1);
		glVertex3f(radius * sin(alpha * slice), height / 2, radius * cos(alpha * slice));
		glVertex3f(radius * sin(alpha * (slice + 1)), -height / 2, radius * cos(alpha * (slice + 1)));
		glVertex3f(radius * sin(alpha * (slice + 1)), height / 2, radius * cos(alpha * (slice + 1)));
		glEnd();

	}
}

void drawTeapot(float x) {
	glColor3f(0, 1, 1);
	glutWireTeapot(x);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(radius2 * cos(beta) * sin(alpha), radius2*sin(beta) , radius2*cos(beta)*cos(alpha),
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	//drawCylinder(1,2,100);
	drawTeapot(1);
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {
	switch(c) {
	case 'w': radius2 -= 0.1; break;
	case 's': radius2 += 0.1; break;
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
		case GLUT_KEY_LEFT: alpha -= 0.1;  break;
		case GLUT_KEY_RIGHT: alpha += 0.1; break;
		case GLUT_KEY_DOWN:  if (beta + 0.1 < M_PI/2) beta += 0.1; break;
		case GLUT_KEY_UP:  if (beta - 0.1 > -M_PI /2) beta -= 0.1;  break;
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
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
