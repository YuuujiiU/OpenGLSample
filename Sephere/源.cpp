#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include<stdio.h>

#include <math.h>


// Define a constant for the value of PI
#define GL_PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


float r = 10.0f;
float step;
float point = 0;
// Called to draw scene
void RenderScene(void)
{
	GLfloat x, y, z, angle,fine; // Storeage for coordinates and angles

							// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(1);

	r = 10.0f;
	// Save matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Call only once for all remaining points
	glBegin(GL_POINTS);
	for (angle = 0.0f; angle <= (2.0f*GL_PI); angle += 0.1f)
	{
		for (float fine = 0.0f; fine <= (2.0f*GL_PI); fine += 0.1f)
		{
			x = r*sin(angle)*cos(fine);
			y = r*sin(angle)*sin(fine);
			z = r*cos(angle);
			// Specify the point and move the Z value up a little	
			glVertex3f(x, y, z);
		}
	}

	// Done drawing points
	glEnd();

	// Restore transformations
	glPopMatrix();

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void InitRC()
{
	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//gluOrtho2D(0.0, 200.0, 0.0, 200.0);//set projection parametres.


}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (key > 356.0f)
		xRot = 0.0f;

	if (key < -1.0f)
		xRot = 355.0f;

	if (key > 356.0f)
		yRot = 0.0f;

	if (key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);//red
	glBegin(GL_POINTS);
	//aixs_y
	for (float i = 0; i <= 200; i += 0.05)
	{
		glVertex2i(100, i);

	}
	//axis_x
	for (float j = 0; j <= 200; j += 0.05)
	{
		glVertex2i(j, 100);

	}

	glEnd();
	glFlush();

}



int main(int argc, char* argv[])
{
	//Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//Init Window
	glutCreateWindow("Points Example");

	//Interactive
	glutDisplayFunc(lineSegment);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);

	InitRC();

	//Main Loop
	glutMainLoop();

	return 0;
}
