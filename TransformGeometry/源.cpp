#include<Windows.h>
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"UTools.h"


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutCreateWindow("Simple");
	LoadFromFile();
	glutDisplayFunc(RenderScene);
	
	glutMainLoop();

	return 0;
}

