#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include<stdio.h>

// Define a constant for the value of PI
#define GL_PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
GLfloat fStep = 0.1;

void RenderX(void)
{
	//Init Data
	float y = 0;
	float x = -1;

	//Init Scene
	glColor3f(1, 0, 0);
	glPointSize(9);

	//Input

	//Process
	glBegin(GL_POINTS);
	for (x = -1; x <= 1; x += fStep)
	{
		glVertex2f(x, y);
	}
	glEnd();

	//Output

}

void RenderY()
{
	float x = 0;
	float y = -1;

	glColor3f(1, 0, 0);
	glPointSize(9);

	for (y = -1; y <= 1; y += fStep)
	{
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}
}

void RenderCircul()
{
	GLfloat x, y, z, angle; // Storeage for coordinates and angles

							// Clear the window with current clearing color
	//glClear(GL_COLOR_BUFFER_BIT);

	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(9);


	// Save matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Call only once for all remaining points
	glBegin(GL_POINTS);

	for (angle = 0.0f; angle <= (2.0f*GL_PI)*3.0f; angle += 0.1f)
	{
		x = 0.5f*cos(angle);
		y = 0.5f*sin(angle);
		glVertex2f(x,y);
	}

	// Done drawing points
	glEnd();

	// Restore transformations
	glPopMatrix();

	// Flush drawing commands
	glutSwapBuffers();
}

void Rendering()
{
	glClear(GL_COLOR_BUFFER_BIT);

	RenderX();
	RenderY();
	RenderCircul();
	glFlush();
}

void  InitGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("Example");

	InitGL();
	glutDisplayFunc(Rendering);
;

	glutMainLoop();
	return 0;
}