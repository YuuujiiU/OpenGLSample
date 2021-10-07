// myOpenGL.cpp: implementation of the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLWorkSpace.h"
#include "myOpenGL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CmyOpenGL::CmyOpenGL()
{
	m_fStep=0;
	m_iExNum=1;
}

CmyOpenGL::~CmyOpenGL()
{

}

BOOL CmyOpenGL::PostInit()
{
//	glClearColor(0.6f,0.6f,0.9f,0.0f);
//	SetLight(TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);	

	glFrontFace(GL_CCW);
	
	InitQuadric();
//	glEnable(GL_LIGHTING);
//	if(m_iExNum==1)
	{		
		SetLight1();
		SetLight2();
		SetLight3();
		SetLight4();
	}
//	if(m_iExNum==2)
	{
		InitList();
	}	
	
	return TRUE;
}

BOOL CmyOpenGL::InDraw()
{
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_LINE);

//	if(m_iExNum==1)
	{
//		LightMotionStep();
		LightMove();
	}
	if(m_iExNum==2)
	{
		TestList();
	}

//	DrawCoord(0,0,0,5,TRUE);	
	return TRUE;
}

void CmyOpenGL::InitQuadric()
{
	m_qobj = gluNewQuadric();
	//	gluQuadricCallback(m_qobj, GLU_ERROR, errorCallback);
	
	gluQuadricDrawStyle(m_qobj, GLU_FILL); /* smooth shaded */
	gluQuadricNormals(m_qobj, GLU_SMOOTH);
	   
}
void CALLBACK CmyOpenGL::errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Quadric Error: %s\n", estring);
	exit(0);
}
void CmyOpenGL::DrawSphere(float pos[],	float radius)
{
	glPushMatrix();
	
	glTranslatef(pos[0], pos[1], pos[2]);
	gluSphere(m_qobj, radius, 100, 100);
	
	//	gluCylinder(m_qobj, baseRadius, topRadius, height, 10, 1);
	
	glPopMatrix();
}

void CmyOpenGL::DrawTriangle()
{
	glBegin(GL_TRIANGLES);
		glVertex3f(0,1,0);
		glVertex3f(-1,0,0);
		glVertex3f(1,0,0);
	glEnd();
}

void CmyOpenGL::LightMove()
{
	//和对象一起移动光源: 所有的变换在光源位置和对象定
	//义之前调用
	//固定对象，移动光源: 先定义对象，进行变换后，再定
	//义光源位置
	//固定光源，移动对象: 先定义光源位置，进行变换后，
	//再定义对象

	float no_mat[]={0.0f,0,0,1.0};
	float mat_ambient[]={0.7f,0.7f,0.7f,1};
	float mat_ambient_color[]={0.8f,0.8f,0.3f,1};
	float mat_diffuse[]={0.1f,0.5f,0.8f,1};
	float mat_specular[]={1.0f,1,1,1};
	float no_shininess[]={0.0f};
	float low_shininess[]={5.0f};
	float high_shininess[]={100.0f};
	float mat_emission[]={1.0f,0.0f,0.0f,0};

	float pos[]={0.0f,0,1.5,1};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
		//固定对象，移动光源: 先定义对象，进行变换后，再定
		//义光源位置
		//Light1
		glPushMatrix();
			glRotated(m_fStep, 0,1,0);
			glLightfv(GL_LIGHT1, GL_POSITION, pos);

			glTranslated(0.0,0.0,1.5);
			glDisable(GL_LIGHTING);
			glColor3f(1,0,0);
			glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
			auxSolidSphere(0.05);
			glEnable(GL_LIGHTING);	
		glPopMatrix();
		//Light2
		glPushMatrix();
			glRotated(m_fStep+2, 1,0,0);
			glLightfv(GL_LIGHT2, GL_POSITION, pos);
		
			glTranslated(0.0,0.0,1.5);
			glDisable(GL_LIGHTING);
			glColor3f(0,1,0);
			auxWireSphere(0.05);
			glEnable(GL_LIGHTING);	
		glPopMatrix();	
		//Light3
		glPushMatrix();
			glRotated(m_fStep+4, 1,1,1);
			glLightfv(GL_LIGHT3, GL_POSITION, pos);
		
			glTranslated(0.0,0.0,1.5);
			glDisable(GL_LIGHTING);
			glColor3f(0,0,1);
			auxWireSphere(0.05);
			glEnable(GL_LIGHTING);	
		glPopMatrix();
		//Light4
		glPushMatrix();
			glLightfv(GL_LIGHT4, GL_POSITION, pos);
			glDisable(GL_LIGHTING);
				glColor3f(1,1,1);
				auxWireSphere(0.05);
			glEnable(GL_LIGHTING);
		glPopMatrix();
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_ambient);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);		
		auxSolidSphere(0.5);

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);		
		auxSolidTorus(0.275,0.85);
	glPopMatrix();
}
void  CmyOpenGL::LightMotionStep()//CALLBACK
{
	m_fStep += 0.4;
	if(m_fStep>360)
	{
		m_fStep -= 360;
	}//= (m_iStep+1)%360;
}
void CmyOpenGL::SetLight1()
{
	float ambient[]={0.0f,0,0,1};
	float diffuse[]={1.0f,0,0,1};
	float specular[]={1.0f,0,0,1};
	float position[]={0.0f,3,2,0};
	
	float lmodel_ambient[]={0.4f,0.4f,0.4f,1.0f};
	float local_view[]={0.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	
}
void CmyOpenGL::SetLight2()
{
	float ambient[]={0.0f,0,0,1};
	float diffuse[]={0.0f,1.0f,0,1};
	float specular[]={1.0f,0,0,1};
	float position[]={0.0f,3,2,0};
	float lmodel_ambient[]={0.4f,0.4f,0.4f,1.0f};
	float local_view[]={0.0f};
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT2, GL_POSITION, position);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
}
void CmyOpenGL::SetLight3()
{
	float ambient[]={0.0f,0,0,1};
	float diffuse[]={0.0f,0.0f,1.0f,1};
	float specular[]={1.0f,0,0,1};
	float position[]={0.0f,3,2,0};
	float lmodel_ambient[]={0.4f,0.4f,0.4f,1.0f};
	float local_view[]={0.0f};
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT3, GL_POSITION, position);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);
}
void CmyOpenGL::SetLight4()
{
	float ambient[]={0.0f,0,0,1};
	float diffuse[]={0.0f,0.0f,1.0f,1};
	float specular[]={1.0f,0,0,1};
	float position[]={0.0f,3,2,0};
	float spot_direction[] = {1,1,0.5};
	float lmodel_ambient[]={0.4f,0.4f,0.4f,1.0f};
	float local_view[]={0.0f};
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT4, GL_POSITION, position);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specular);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT4);
}

void CmyOpenGL::InitList()
{
	glNewList(m_iListNum, GL_COMPILE);
		glutSolidTorus(0.3,0.6,20,20);
		glTranslatef(2,0,0);
	glEndList();
}
void CmyOpenGL::TestList()
{
	for(int i=0;i<6;i++)
		glCallList(m_iListNum);
}