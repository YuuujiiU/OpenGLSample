#include <gl/glut.h>

float	fRotateTri	= 0;
float	fRotateQuad = 0;

void Init()
{
	glEnable(GL_DEPTH_TEST); //������Ȳ���
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.8f); //����Ϊ��ɫ
}

void Reshape(int w, int h)
{
	if (0 == h)
		h = 1;
	
	glViewport(0, 0, w, h); //���������ߴ�
	
	glMatrixMode(GL_PROJECTION); //ָ����ǰ����ͶӰ�����ջ
	glLoadIdentity(); //����ͶӰ����
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void RenderATriangle()
{
	glLoadIdentity();		
	glTranslatef(-1.5f,0.0f,-6.0f);
	glRotatef(fRotateTri,0.0f,1.0f,0.0f);
	glBegin(GL_TRIANGLES);	
	glColor3f(1.0f,0.0f,0.0f);	
	glVertex3f( 0.0f, 1.0f, 0.0f);
	glColor3f(0.0f,1.0f,0.0f);		
	glVertex3f(-1.0f,-1.0f, 0.0f);	
	glColor3f(0.0f,0.0f,1.0f);	
	glVertex3f( 1.0f,-1.0f, 0.0f);	
	glEnd();								
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderATriangle();

	
	glutSwapBuffers();
	
}

void myIdle(void) //�ڿ���ʱ���ã��ﵽ����Ч��

{
	fRotateTri+=0.2f;

	RenderScene();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600, 400);
	glutCreateWindow("Demo");
	
	glutReshapeFunc(Reshape);
	
	glutDisplayFunc(RenderScene);
	
	glutIdleFunc(myIdle);
	
	Init();
	
	glutMainLoop();
	
	return 0;
	
} 
