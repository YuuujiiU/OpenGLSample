//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

void Rendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.6, 0.6);
	glVertex2f(-0.6, -0.6);
	glVertex2f(0.6, -0.6);
	glVertex2f(0.6, 0.6);

	glEnd();
	glutSwapBuffers();
}



int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("retangle");
	glutDisplayFunc(Rendering);


	glutMainLoop();
	return 0;
}