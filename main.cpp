#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float translateX = 0;
float translateZ = 0;
float rotateXZ = 0; 
float rotateY = 0; 
float rotateX = 0;


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



/*

TODO:

•	Plane (a square in the XZ plane, centred in the origin, made with 2 triangles)
•	Box (requires X, Y and Z dimensions, and optionally the number of divisions)
•	Sphere (requires radius, slices and stacks)
•	Cone (requires bottom radius, height, slices and stacks)

*/


//2 triângulos
void plane(){

	glVertex3f( 1.0f, 0.0f,  1.0f);
	glVertex3f(-1.0f, 0.0f,  1.0f);
	glVertex3f( 1.0f, 0.0f, -1.0f);

	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f,  1.0f);
	glVertex3f( 1.0f, 0.0f, -1.0f);

}


//12 triângulos (6 faces * 2 triângulos por face)
void box(){

	//BASE SUP
	glVertex3f( 1.0f, 1.0f, 1.0f); //A
	glVertex3f( 1.0f, 1.0f,-1.0f); //B
	glVertex3f(-1.0f, 1.0f, 1.0f); //D

	//BASE SUP
	glVertex3f(-1.0f, 1.0f,-1.0f); //C
	glVertex3f( 1.0f, 1.0f,-1.0f); //B
	glVertex3f(-1.0f, 1.0f, 1.0f); //D

	//BASE INF
	glVertex3f();
	glVertex3f();
	glVertex3f();

	//BASE INF
	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

	glVertex3f();
	glVertex3f();
	glVertex3f();

}

void sphere(){



}

void cone(){


	
}

void pyramid() {

	glBegin(GL_TRIANGLES);

	//glColor3f(r,g,b): r=1 means full red, g=1 means full green, b=1 means full blue
	//T1 
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.f, 0.0f); //1 0 0 
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f); //1 0 0
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 0.0f, 1.0f); //1 0 0 ficava a face vermelha

															   //T2
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 0.0f, -1.0f);

	//T3
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -1.0f);

	//T4
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -1.0f);

	//T5
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -1.0f);

	//T6
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, -1.0f);


	glEnd();
}



void renderScene(void) {

	// clear buffers (depth and color buffers)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

		
	

	// put the geometric transformations here
	glTranslatef(translateX, 0, translateZ);
	glRotatef(rotateXZ, 1.0, 0.0, -1.0);
	glRotatef(rotateY, 0.0, 1.0, 0.0);
	glRotatef(rotateX, 1.0, 0.0, 0.0);

	// put drawing instructions here
	pyramid();


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
	glutKeyboardFunc(keyboardAuxFunc);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //tirar para ver a parte de baixo
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
