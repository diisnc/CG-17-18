#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


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

void drawPlane()
{
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);

	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);
	glVertex3f(100.0f, 0, 100.0f);
	glEnd();
}
void drawTree(float x, float z)
{
	glPushMatrix();
	glTranslatef(x, 0, z);

	glRotatef(-90, 1, 0, 0);
	glColor3f(0.6, 0.2, 0.2);
	glutSolidCone(0.5, 5, 5, 1);

	glTranslatef(0, 0, 2);
	glColor3f(0.2, 0.6, 0.2);
	glutSolidCone(2, 6, 5, 1);

	glPopMatrix();
}
void placeTrees() {
	float alpha, r, x, z;
	srand(100000);
	int i = 0;

	while (i<100)
	{
		r = (rand() * 50) / RAND_MAX;
		alpha = (rand() * 2 * M_PI) / RAND_MAX;
		x = cos(alpha)*(r+50);
		z = sin(alpha)*(r+50);

		if (fabs(x) < 100 && fabs(z) < 100) {
			i++;
			drawTree(x,z);
		}
	}
}

void placeTorus() {
	glColor3f(1, 0, 1);
	glutSolidTorus(1, 2, 10, 10);
}

void placeTeapots() {
	
	// inside
	glColor3f(0, 0, 1);
	for (int x = 0; x < 360; x += 45) {
		glPushMatrix();
		glTranslatef(cos((x)*(M_PI / 180)) * 15, 2, sin((x)*(M_PI / 180)) * 15);
		glRotatef(-x, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
	}

	//outside
	glColor3f(1, 0, 0);
	for (int x = 0; x < 360; x += 22.5) {
		glPushMatrix();
		glTranslatef(cos((x)*(M_PI / 180)) * 35, 2, sin((x)*(M_PI / 180)) * 35);
		glRotatef(-x, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cos(beta)*sin(alfa)*radius, sin(beta)*radius, cos(beta)*cos(alfa)*radius, 0, 0, 0, 0, 1, 0);
	
	drawPlane();
	placeTrees();
	placeTorus();
	placeTeapots();

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");

	glClearColor(0.0f, 1.0, 1.0, 0.0);
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
