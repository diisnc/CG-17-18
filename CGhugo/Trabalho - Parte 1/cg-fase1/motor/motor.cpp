//
//  main.cpp
//  Motor
//
//  Created by Tiago Conceição on 14/03/14.
//  Copyright (c) 2014 Tiago Conceição. All rights reserved.
//

#include "motor.h"
#include <GLUT/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define FILL 1
#define LINE 2
#define POINT 3

using namespace std;

Cena cena;

float alpha = 0.5, beta = 0.5, radius = 10;
float xOrigin = -1;
float yOrigin = -1;

void changeSize(int w, int h)
{
	if(h == 0) h = 1;
    float ratio = w * 1.0 / h;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(cos(beta)*sin(alpha)*radius, sin(beta)*radius, cos(beta)*cos(alpha)*radius, 0,0,0, 0,1,0);
    
    glBegin(GL_TRIANGLES);
        for(list<Modelo>::iterator itm = cena.modelos.begin(); itm != cena.modelos.end(); itm++)
            for(list<Ponto>::iterator itp = itm->pontos.begin(); itp != itm->pontos.end(); itp++)
                glVertex3f(itp->x,itp->y,itp->z);
    glEnd();
    
    glutPostRedisplay();
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
    {
        case 27: exit(0); break;
		case 119 : radius -= 0.1; break;
		case 115 : radius += 0.1; break;
    }
}

void processSpecialKeys(int key, int x1, int y1)
{
	switch (key)
    {
		case GLUT_KEY_LEFT : alpha -= 0.1; break;
		case GLUT_KEY_RIGHT : alpha += 0.1; break;
		case GLUT_KEY_UP : if(beta < M_PI/2) beta += 0.1; break;
		case GLUT_KEY_DOWN : if(beta > -M_PI/2) beta -= 0.1; break;
	}
}

void mouseButton(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_UP) { xOrigin = -1; yOrigin = -1; }
		else { xOrigin = x; yOrigin = y; }
	}
}

void mouseMove(int x, int y)
{
	if(xOrigin >= 0)
    {
        alpha = (x + xOrigin) * 0.01;
        if(beta < M_PI/2) beta = (y - yOrigin) * 0.01;
        if(beta > -M_PI/2) beta = (y - yOrigin) * 0.01;
    }
}

void processFillMenu(int option)
{
	switch (option)
    {
		case FILL: glPolygonMode(GL_FRONT, GL_FILL); break;
		case LINE: glPolygonMode(GL_FRONT, GL_LINE); break;
        case POINT: glPolygonMode(GL_FRONT, GL_POINT); break;
	}
}

void createGLUTMenus()
{
    int menu;
    menu = glutCreateMenu(processFillMenu);
    
    glutAddMenuEntry("Fill",FILL);
	glutAddMenuEntry("Line",LINE);
    glutAddMenuEntry("Point",POINT);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
    cena.load(argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(400,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
    
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
    
    createGLUTMenus();
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
	glutMainLoop();
	
	return 0;
}


