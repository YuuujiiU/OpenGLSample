//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

void Rendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	//glColor3f(0.1, 0.2, 0.3);//GL_TRIANGLE_STRIP

	////glVertex3f(-1, 0.5, 0);
	////glVertex3f(-1, -0.5, 0);
	////glVertex3f(0, 0.5, 0);

	//////glColor3f(0.5, 0.2, 0.3);

	////glVertex3d(0,-0.5,0);

	//////glColor3f(0.8, 0.2, 0.3);

	////glVertex3d(1,0.5,0);//GL_TRIANGLE_FAN
	////glVertex3d(1,-0.5,0);

	//glVertex3d(0,0 ,1 );
	//glVertex3d(0,0.5 ,0 );
	//glVertex3d(0.5,0 ,0 );

	glColor3f(1.0,0.0,0.0);
	glVertex3d(0, -0.5, 0);

	glColor3f(0.0,1.0 ,0.0 );
	glVertex3d(-0.5, 0, 0);

	glColor3f(0.0,0.0 ,1.0 );
	glVertex3d(0, 0.5, 0);



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
	glutCreateWindow("Triangle");
	glutDisplayFunc(Rendering);


	glutMainLoop();
	return 0;
}