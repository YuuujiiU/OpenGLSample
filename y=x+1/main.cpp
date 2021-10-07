#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);//set display-window color to white.

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 200.0);//set projection parametres.


}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);//clear display window


	glColor3f(1, 0, 0);//red
	glBegin(GL_POINTS);
	//aixs_x
	for (float i = 0; i <= 200; i += 0.05)
	{
		glVertex2i(100, i);

	}
	//axis_y
	for (float j = 0; j <= 200; j += 0.05)
	{
		glVertex2i(j, 100);

	}
	glColor3f(0, 1, 0);//green
	for (float k = 0; k <= 300; k += 0.001)
	{
		glVertex2i(k, k+5);

	}

	//for (float i = 0; i <= 20; i += 0.05)
	//{
	//	glVertex2f(0, i);
	//}

	glEnd();
	glFlush();

}
int main(int argc, char* argv[]) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Example");

	init();
	glutDisplayFunc(lineSegment);


	glutMainLoop();
	return 0;
}