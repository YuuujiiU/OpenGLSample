// OpenGL.cpp: implementation of the COpenGL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenGL.h"
//#include ".\opengl.h"
#include "math.h"
#include "gl\glaux.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COpenGL::COpenGL()
{
	m_bEnd = TRUE;
	m_hWnd = NULL;
	m_hDC = NULL;
	m_bOnly = FALSE;
	m_cnFrame = 0;
	m_fps = 0;
	m_timeBegin = 0;
	m_bViewChanged = FALSE;
	m_bLight = FALSE;
	m_bContinue = FALSE;
	LightAmbient[0]= 0.2f;LightAmbient[1]= 0.2f;LightAmbient[2]= 0.2f;LightAmbient[3]= 1.0f;
//	LightAmbient[0]= 1.0f;LightAmbient[1]= 1.0f;LightAmbient[2]= 1.0f;LightAmbient[3]= 1.0f;	
	LightDiffuse[0]= 0.8f;LightDiffuse[1]= 0.8f;LightDiffuse[2]= 0.8f;LightDiffuse[3]= 1.0f;
	LightSpecular[0]= 1.0f;LightSpecular[1]= 1.0f;LightSpecular[2]= 1.0f;LightSpecular[3]= 1.0f;
	LightPosition[0]= 0.0f;LightPosition[1]= 0.0f;LightPosition[2]= 0.0f;LightPosition[3]= 1.0f;
}

COpenGL::~COpenGL()
{
	int n=scene.size();
	for(int i=0;i<n;i++)
	{
		delete scene[i];
		scene[i]=0;
	}
	scene.clear();
	if(!m_bEnd)
		End();
}
//control
VOID COpenGL::SetControlRoam(BOOL bRoam)
{
	control.SetControlRoam(bRoam);
}
BOOL COpenGL::SetRotateType(CString type)
{
	return control.SetRotateType(type);
}
BOOL COpenGL::SetMoveType(CString type)
{
	return control.SetMoveType(type);
}
BOOL COpenGL::OnMouse(UINT message, UINT wParam, UINT lParam)
{
	return control.OnMouse(message,wParam,lParam);
}
BOOL COpenGL::OnKey(UINT message, UINT nChar)
{
	return control.OnKey(message,nChar);
}
//font
BOOL COpenGL::Font3D(CString str,int *color)
{
	if(color)
	{
		GLfloat LightAmbient[4]; 	
		LightAmbient[0]= GetRValue(*color)/255.0;LightAmbient[1]= GetGValue(*color)/255.0;LightAmbient[2]= GetBValue(*color)/255.0;
		glColor3f(LightAmbient[0],LightAmbient[1],LightAmbient[2]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,LightAmbient);
	}
	return font.Font3D(str);
}
BOOL COpenGL::Font2D(CString str, float x, float y, int size)
{
	return font.Font2D(str,x,y,size);
}
//view
BOOL COpenGL::SetStep(float mstep, float sstep)
{
	return view.SetStep(mstep,sstep);
}
BOOL COpenGL::GetStep(float &mstep, float &sstep)
{
	return view.GetStep(mstep,sstep);
}
BOOL COpenGL::SetView(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat p, GLfloat r)
{
	return view.SetView(x,y,z,h,p,r);
}
BOOL COpenGL::SetView(GLfloat m[], GLfloat r[])
{
	return view.SetView(m[0],m[1],m[2],r[0],r[1],r[2]);
}
BOOL COpenGL::SetLookAt(float x, float y, float z, float tx, float ty, float tz)
{
	return view.SetLookAt(x,y,z,tx,ty,tz);
}
BOOL COpenGL::SetLookAt(float tx, float ty, float tz)
{
	return view.SetLookAt(tx,ty,tz);
}
BOOL COpenGL::SetLookAt(bool bLookAt)
{
	return view.SetLookAt(bLookAt);
}
BOOL COpenGL::SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt)
{
	return view.SetInitView(x,y,z,x1,y1,z1,bLookAt);
}
BOOL COpenGL::SetInitStep(GLfloat m,GLfloat s)
{
	return view.SetInitStep(m,s);
}
BOOL COpenGL::RestoreView()
{
	return view.RestoreView();
}
BOOL COpenGL::RestoreStep()
{
	return view.RestoreStep();
}
BOOL COpenGL::GetLookCenter(float center[])
{
	for(int i=0;i<3;i++)
	{
		center[i]=view.m_fLookTarget[i];
	}
	return TRUE;
}
BOOL COpenGL::GetView(float pos[],float dir[])
{
	for(int i=0;i<3;i++)
	{
		if(pos)
			pos[i]=view.m_moveView[i];
		if(dir)
			dir[i]=view.m_rotateView[i];
	}
	return TRUE;
}

BOOL COpenGL::GetViewVector(float vec[])
{
	for(int i=0;i<3;i++)
		vec[i] = view.m_rotateVector[i];
	return TRUE;
}
float *COpenGL::GetViewMatrix()
{
	return view.m_viewMatrix;
}
float *COpenGL::CalculateClip()
{
	return frus.CalculateClip();
}
void COpenGL::GetModelMatrix(float mat[])
{
	float iview[16],mv[16];
	InverseMatrix(iview,view.m_viewMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,mv);
	glPushMatrix();
	glLoadMatrixf(iview);
	glMultMatrixf(mv);
	glGetFloatv(GL_MODELVIEW_MATRIX,mat);
	glPopMatrix();
}
BOOL COpenGL::Init(HWND hWnd,BOOL bOnly)
{
	m_bEnd = FALSE;
	m_hWnd = hWnd;
	control.SetParm(hWnd,&view);
	m_bOnly = bOnly;
	static PIXELFORMATDESCRIPTOR pfd = SetPixelFormatDescriptor();
	m_hDC = GetDC(hWnd);
	int pf = ChoosePixelFormat(m_hDC, &pfd);
    if(!pf)	return FALSE;
    if (!SetPixelFormat(m_hDC, pf, &pfd))
		return FALSE;
	m_hglrc = wglCreateContext(m_hDC);
	if(bOnly)
		wglMakeCurrent(m_hDC,m_hglrc);
	//初始化设置，设置背景色和深度色
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
//	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	SetInitView(0,0,5,0,0,0,FALSE);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
//	glEnable(GL_COLOR_MATERIAL );
	PostInit();
//	OnPaint();
	m_timeBegin=timeGetTime();//取得开始绘制的时间。
	return TRUE;
}

BOOL COpenGL::OnSize(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	m_PerspectiveParam[0]=60;
	m_PerspectiveParam[1]=(GLfloat)w/h;
	m_PerspectiveParam[2]=0.1;
	m_PerspectiveParam[3]=100000;
	gluPerspective(m_PerspectiveParam[0],m_PerspectiveParam[1],m_PerspectiveParam[2],m_PerspectiveParam[3]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_rect.SetRect(0,0,w,h);
	m_pixLength=2*tan(AngToRad(30.0*w/h))/w;
	m_pixLength2=m_pixLength*m_pixLength;
	return TRUE;
}
BOOL COpenGL::OnViewChange(void)
{
	m_bViewChanged = TRUE;
	return TRUE;
}
BOOL COpenGL::OnViewActualChange(void)
{
	
	return TRUE;
}
BOOL COpenGL::OnDraw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();	//主绘制场景
	ShowBeforeView();
	OnView();
	m_iTriangle = 0;
	glInitNames();
	InDraw();
	glPopMatrix();
//	glFlush();//只有单缓冲才需要写。
	m_bViewChanged = FALSE;
		
/*
	if(!m_timeBegin)
	{
		m_timeBegin = GetTickCount();
		view.ShowTriNum(m_iTriangle);
	}
	else
	{
		m_cnFrame ++;
		UINT tickcount = GetTickCount();
		if(tickcount-m_timeBegin>500)
		{
			m_fps = m_cnFrame*1000.0/(tickcount-m_timeBegin);
			view.ShowFPS(m_fps);
			view.ShowTriNum(m_iTriangle);
			m_timeBegin = tickcount;
			m_cnFrame = 0;
		}
	}	
*/
	return TRUE;
}

BOOL COpenGL::OnPaint()
{
	long timebegin=timeGetTime();
	static int m_iTriangleOld;
	if(!m_bOnly)
	{
		wglMakeCurrent(m_hDC,m_hglrc);
		OnDraw();
		wglMakeCurrent(m_hDC,NULL);
	}
	else
		OnDraw();
	SwapBuffers(m_hDC);
	if(m_bContinue) 
		InvalidateRect(m_hWnd,NULL,TRUE);
	long curtime = timeGetTime();
	long usetime=curtime-timebegin;
	m_RenderTime = usetime;
	m_fps=1000.0/usetime;
	static lasttime=0;
	static int sumtime=0;
	static iframe=0;
	sumtime+=usetime;
	iframe++;
//	view.ShowFPS(usetime);
	if(curtime-lasttime>400&&iframe>2)//大于0.4秒就显示.显示平均值
	{
		m_aveRenderTime=(float)sumtime/iframe;
		m_avefps=(float)iframe*1000/sumtime;
		view.ShowFPS(m_avefps);		
		sumtime=0;
		iframe=0;
		lasttime=curtime;
	}
	if(m_iTriangle!=m_iTriangleOld)
	{
		m_iTriangleOld=m_iTriangle;
		view.ShowTriNum(m_iTriangle);
	//	view.ShowTriNum(m_cnFrame);
	}
	m_cnFrame++;//计算绘制的第几帧。		
	return TRUE;
}

BOOL COpenGL::End()
{
	if(m_bEnd) return FALSE;
	if(m_bOnly)
		wglMakeCurrent(m_hDC,NULL);
	wglDeleteContext(m_hglrc);
	m_bEnd = TRUE;
	return TRUE;
}


BOOL COpenGL::InDraw()
{
	GLfloat LightAmbient[]= { 0.3f,0.3f,0.3f, 1.0f };
	GLfloat LightDiffuse[]= { 0.4f,0.4f,0.4f, 1.0f };
	glColor3fv(LightAmbient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,LightAmbient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,LightDiffuse);
	glutSolidTeapot(2);
	glPushMatrix();
	glTranslatef(-3.6,2,0);
	Font3D("OpenGL Show");
	glPopMatrix();
	DrawCoord(0,0,0,5,TRUE);
	return TRUE;
}

BOOL COpenGL::PostInit()
{
	return TRUE;
}

PIXELFORMATDESCRIPTOR COpenGL::SetPixelFormatDescriptor()
{
	PIXELFORMATDESCRIPTOR pfd = //描述像素格式
	{
        sizeof(PIXELFORMATDESCRIPTOR),		// size of this pfd	结构大小
			1,                              // version number	结构版本，必须为1
			PFD_DRAW_TO_WINDOW |            // support window	像素格式选项标志（附表）
			PFD_SUPPORT_OPENGL |		    // support OpenGL
			PFD_GENERIC_ACCELERATED|		// 加速
			PFD_DOUBLEBUFFER,				// double buffered
			PFD_TYPE_RGBA,                  // RGBA type		PFD_RGBA|PFD_INDEX RGB窗口或索引窗口
			24,                             // 24-bit color depth	所需颜色索引位数
			0,								// color bits ignored	所需红色位数
			0,								// color bits ignored	使用调色板转换为红色时，颜色位变化的数目
			0,								// color bits ignored   所需绿色位数
			0,								// color bits ignored   使用调色板转换为绿色时，颜色位变化的数目
			0,								// color bits ignored   所需蓝色位数
			0,								// color bits ignored   使用调色板转换为蓝色时，颜色位变化的数目            
			0,                              // no alpha buffer		所需的alpha颜色位数；对于一般的OpenGL安装而言，其值始终未0
			0,                              // shift bit ignored	转换为alpha颜色位数变化的数目；对于一般的OpenGL安装而言，其值始终未0
			0,                              // no accumulation buffer	实现累计所需要的颜色索引位数
			0, 0, 0, 0,                     // accum bits ignored	实现累计所需要的红色、绿色、蓝色、alpha色位数
			16,                             // 16-bit z-buffer		所需要的深度缓冲区位数
			0,                              // no stencil buffer	所需要的模版缓冲区位数
			0,                              // no auxiliary buffer	所需要的辅助缓冲区位数；对于一般的OpenGL安装而言，其值始终未0
			PFD_MAIN_PLANE,                 // main layer			WIN32API不使用和支持本变量
			0,                              // reserved				为将来的使用而准备的保留变量
			0,								// layer masks ignored	WIN32API不使用和支持本变量
			0,								// 指定RGBA或透明的索引色；本变量适应于覆盖图的像素格式 
			0								// WIN32API不使用和支持本变量
    };	
	//像素格式选项标志：
	return pfd;
}

BOOL COpenGL::OnView()
{
	m_bContinue=control.AutoMove();
	if(view.ShowView())
	{
		if(m_bViewChanged)
			return OnViewActualChange();
	}
	return FALSE;
}
VOID COpenGL::SetLight(BOOL bLight,float *ambient,float *diffuse,float *specular,float *position)
{
	int i;
	if(ambient)
	{
		for(i=0;i<4;i++)
			LightAmbient[i]=ambient[i];
	}
	if(diffuse)
	{
		for(i=0;i<4;i++)
			LightDiffuse[i]=diffuse[i];
	}
	if(specular)
	{
		for(i=0;i<4;i++)
			LightSpecular[i]=specular[i];
	}
	if(position)
	{
		for(i=0;i<4;i++)
			LightPosition[i]=position[i];
	}
	if(m_bLight)
	{
		if(bLight) return;
		else
		{
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
			m_bLight = FALSE;
		}
	}
	else
	{
		if(!bLight) return;
		else
		{	m_bLight = TRUE;
			glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
			glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
		}
	}
}
VOID COpenGL::SetMode(int mode)
{
	switch(mode) {
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	}
}

BOOL COpenGL::ShowBeforeView()
{
	return TRUE;
}

VOID COpenGL::SetStatus(CStatusBar *pStatus)
{
	view.SetStatus(pStatus);
}
BOOL COpenGL::DrawArrow(GLfloat x,GLfloat y,GLfloat z,GLfloat xend,GLfloat yend,GLfloat zend,INT color)
{
	glDisable(GL_TEXTURE_2D);
	GLfloat col[4],width;
	glGetFloatv(GL_CURRENT_COLOR,col);
	glGetFloatv(GL_LINE_WIDTH,&width);
	int POLYGON_MODE[2];
	glGetIntegerv(GL_POLYGON_MODE,POLYGON_MODE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	GLfloat dis=0.2;
	glLineWidth(1.5);
	GLfloat LightAmbient[4];
	LightAmbient[0]= GetRValue(color)/255.0;LightAmbient[1]= GetGValue(color)/255.0;LightAmbient[2]= GetBValue(color)/255.0;
	glColor3fv(LightAmbient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,LightAmbient);
	GLfloat LightDiffuse[4]={0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,LightDiffuse);

	{
		GLfloat dx,dy,dz,len,ddx,ddy,ddz,alen=0.2,factor,axx,axy,axz;
		dx = x - xend;
		dy = y - yend;
		dz = z - zend;
		if(dx==0&&dy==0&&dz==0) return FALSE;
		len = sqrt(dx*dx+dy*dy+dz*dz);
		if (len<=alen) alen=len/2;
		glPushMatrix();
		//--------------------------------------
		glTranslatef(xend,yend,zend);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(dx,dy,dz);
		glEnd();
		//---------------------------------------
		if(dz==0&&dy==0&&dx!=0)
		{
			axx=-dz;
			axy=0;
			axz=dx;
		}
		else
		{
			axx=0;
			axy=dz;
			axz=-dy;
		}
		//---------------------------------------
		factor = alen/len;
		ddx = factor*dx;
		ddy = factor*dy;
		ddz = factor*dz;
		glPushMatrix();
		glRotatef(30,axx,axy,axz);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(ddx,ddy,ddz);
		glEnd();
		glPopMatrix();
		//----------------------------------------
		glRotatef(-30,axx,axy,axz);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(ddx,ddy,ddz);
		glEnd();
		//----------------------------------------
		glPopMatrix();
	}

	glColor4f(col[0],col[1],col[2],col[3]);
	glLineWidth(width);
	glPolygonMode(GL_FRONT,POLYGON_MODE[0]);
	glPolygonMode(GL_BACK,POLYGON_MODE[1]);
	return TRUE;
}
BOOL COpenGL::DrawCoord(float x, float y, float z, float len, BOOL bMark,int *Color)
{
	GLfloat Ambient[]= { 1.0f,1.0f,1.0f, 1.0f }; 	
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glDisable(GL_TEXTURE_2D);
	GLfloat col[4],width;
	glGetFloatv(GL_CURRENT_COLOR,col);
	glGetFloatv(GL_LINE_WIDTH,&width);
	int POLYGON_MODE[2];
	glGetIntegerv(GL_POLYGON_MODE,POLYGON_MODE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	GLfloat dis=0.2;
	glLineWidth(2);
	GLfloat colorV[]= { 1.0f,0.0f,0.0f, 1.0f }; 	
	if(Color)
	{
		colorV[0]= GetRValue(Color[0])/255.0;colorV[1]= GetGValue(Color[0])/255.0;colorV[2]= GetBValue(Color[0])/255.0;
	}
	glColor3f(colorV[0],colorV[1],colorV[2]);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,colorV);
	GLfloat LightDiffuse[4]={0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,LightDiffuse);
	glBegin(GL_LINES);
	glVertex3f(x,y,z);
	glVertex3f(len+x,y,z);
	glVertex3f(len+x,y,z);
	glVertex3f(len+x-dis,y+dis,z);
	glVertex3f(len+x,y,z);
	glVertex3f(len+x-dis,y-dis,z);
	glEnd();
	if(bMark)
	{
		glPushMatrix();
		glTranslatef(len+x,y,z);
		glScalef(0.5,0.5,0.5);		
		font.Font3D("X");
		glPopMatrix();
	}
	if(Color)
	{
		colorV[0]= GetRValue(Color[1])/255.0;colorV[1]= GetGValue(Color[1])/255.0;colorV[2]= GetBValue(Color[1])/255.0;
	}	
	else
	{
		colorV[0]= 0.0f;colorV[1]= 1.0f;colorV[2]= 0.0f;
	}
	glColor3f(colorV[0],colorV[1],colorV[2]);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,colorV);
	glBegin(GL_LINES);
	glVertex3f(x,y,z);
	glVertex3f(x,len+y,z);
	glVertex3f(x,len+y,z);
	glVertex3f(x-dis,len+y-dis,z);
	glVertex3f(x,len+y,z);
	glVertex3f(x+dis,len+y-dis,z);
	glEnd();
	if(bMark)
	{
		glPushMatrix();
		glTranslatef(x,len+y,z);
		glScalef(0.5,0.5,0.5);
		font.Font3D("Y");
		glPopMatrix();
	}
	if(Color)
	{
		colorV[0]= GetRValue(Color[2])/255.0;colorV[1]= GetGValue(Color[2])/255.0;colorV[2]= GetBValue(Color[2])/255.0;
	}	
	else
	{
		colorV[0]= 0.0f;colorV[1]= 0.0f;colorV[2]= 1.0f;
	}
	glColor3f(colorV[0],colorV[1],colorV[2]);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,colorV);
	glBegin(GL_LINES);
	glVertex3f(x,y,z);
	glVertex3f(x,y,len+z);
	glVertex3f(x,y,len+z);
	glVertex3f(x+dis,y,len+z-dis);
	glVertex3f(x,y,len+z);
	glVertex3f(x-dis,y,len+z-dis);
	glEnd();
	if(bMark)
	{		
		glPushMatrix();
		glTranslatef(x,y,len+z);
		glScalef(0.5,0.5,0.5);		
		font.Font3D("Z");
		glPopMatrix();
	}
	glColor4f(col[0],col[1],col[2],col[3]);
	glLineWidth(width);
	glPolygonMode(GL_FRONT,POLYGON_MODE[0]);
	glPolygonMode(GL_BACK,POLYGON_MODE[1]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	return TRUE;
}

COpenGL::FontGL::FontGL()
{
	m_bHaveCreate3DFont = false;
	m_pFontSelected = NULL;
}
COpenGL::FontGL::~FontGL()
{
	if(m_bHaveCreate3DFont)
		glDeleteLists(m_iDisplayListStart, m_iNumberChars) ;
	if(m_pFontSelected)
		delete m_pFontSelected;
}
BOOL COpenGL::FontGL::Font2D(CString str, float x, float y, int size)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1,1,1);
	//Print text
	glRasterPos2f(x, y);
	for(int i=0; i<str.GetLength(); ++i)
	{
		switch(size) {
		case 1:
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str.GetAt(i));
			break;
		case 2:
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str.GetAt(i));
			break;
		case 3:
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str.GetAt(i));
			break;
		}
	}
	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	return TRUE;
}

BOOL COpenGL::FontGL::Create3DFont()
{
	// Initialize Default Font Structure
	m_logFontDefault.lfHeight 		= -10 ;
	m_logFontDefault.lfWidth 		= 0 ;
	m_logFontDefault.lfEscapement 	= 0 ;
	m_logFontDefault.lfOrientation 	= 0 ;
	m_logFontDefault.lfWeight 		= FW_NORMAL ;
	m_logFontDefault.lfItalic 		= FALSE ;
	m_logFontDefault.lfUnderline 	= FALSE ;
	m_logFontDefault.lfStrikeOut 	= FALSE ;
	m_logFontDefault.lfCharSet		= ANSI_CHARSET ;
	m_logFontDefault.lfOutPrecision = OUT_TT_PRECIS ;
	m_logFontDefault.lfClipPrecision= CLIP_DEFAULT_PRECIS ;
	m_logFontDefault.lfQuality 		= PROOF_QUALITY ;
	m_logFontDefault.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN ;
	strcpy(m_logFontDefault.lfFaceName, "Arial") ;  
	// Create Default Font
	BOOL bResult =
		m_FontDefault.CreateFontIndirect(&m_logFontDefault) ;
	ASSERT(bResult) ;
	
	// Setup wglGetFontOutline Parameters
	m_iFormat 		= m_iFormatDefault 		= WGL_FONT_POLYGONS ;
	m_fDeviation 	= m_fDeviationDefault 	= 0.0f ;
	m_fExtrusion 	= m_fExtrusionDefault 	= 0.1f ;
	
	// Delete existing display list numbers ;
	m_pFontSelected = new CFont;
	m_pFontSelected->CreateFontIndirect(&m_logFontDefault) ;
	
	// Display list varaibles.
	m_iFirstChar = 32 ;
	m_iLastChar = 126 ;
	m_iNumberChars = m_iLastChar - m_iFirstChar ;
	
	m_iDisplayListStart = 0 ;
	m_iDisplayListBase = 0 ;
	
	if (m_iDisplayListStart != 0 )
	{	
		// Detelete the existing lists.
		glDeleteLists(m_iDisplayListStart, m_iNumberChars) ;
	}
	
	// Generate new display lists.
	m_iDisplayListStart = glGenLists(m_iNumberChars) ;
	m_iDisplayListBase = m_iDisplayListStart - m_iFirstChar ; // Does this work???
	// Get the DC for the current rendering context.	
	HDC hdc = wglGetCurrentDC();
	CDC* pdc = CDC::FromHandle(hdc) ;
	
	// Select the font.
	CFont* pOldFont = (CFont*)pdc->SelectObject(m_pFontSelected) ;
	
	// Generate the display lists...
	GLYPHMETRICSFLOAT agmf[128]; // Fixed size...
	
	bResult = 
		wglUseFontOutlines(	hdc,
		m_iFirstChar,		// First character
		m_iNumberChars,		// Number
		m_iDisplayListStart,	// Starting Display list number
		m_fDeviation,		// Deviation
		m_fExtrusion,		// Extrusion
		m_iFormat,			// Format or WGL_FONT_LINES
		agmf) ;				// Information pointer
	
	if (!bResult)
	{
		TRACE("Error in wglUseFontOutlines.\r\n") ;
	}
	
	if (pOldFont) pdc->SelectObject(pOldFont) ;
	
	// Calculate the Center of the string:
//	int iLen = m_theString.GetLength() ;
//	m_dCenterOffset = 0 ;
//	for (int i = 0 ; i < iLen  ; i++)
//	{
//		m_dCenterOffset += agmf[__toascii(m_theString[i])-m_iFirstChar].gmfCellIncX  ;
//	}
//	m_dCenterOffset /= 2 ;
	return TRUE;
}

BOOL COpenGL::FontGL::Font3D(CString str)
{
	int front;
	glGetIntegerv(GL_FRONT_FACE,&front);
	if(!m_bHaveCreate3DFont)
	{
		if(Create3DFont())
			m_bHaveCreate3DFont = true;
		else
			return false;
	}
	glListBase(m_iDisplayListBase) ; //NOTE CAN THIS BE NEGATIVE????
	glCallLists(str.GetLength(),GL_UNSIGNED_BYTE,(LPCTSTR)str ) ;	
	glFrontFace(front);
	return TRUE;
}

COpenGL::View::View()
{
	for(int i=0;i<16;i++)
		m_RotateMatrix[i]=0;
	m_RotateMatrix[0]=1;m_RotateMatrix[5]=1;m_RotateMatrix[10]=1;m_RotateMatrix[15]=1;
	m_pStatus=NULL;
	m_bLookTarget = NULL;
	m_fLookTarget[0]=m_fLookTarget[1]=m_fLookTarget[2] = 0.0;
	SetInitStep(0.1,0.1);
	m_bLookTargetHaveMove = FALSE;
}
COpenGL::View::~View()
{

}
BOOL COpenGL::View::SetInitStep(GLfloat m,GLfloat s)
{
	m_InitStep[0]=m;
	m_InitStep[1]=s;
	return SetStep(m,s);
}
BOOL COpenGL::View::SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt)
{
	m_InitView[0]=x;m_InitView[1]=y;m_InitView[2]=z;
	m_InitView[3]=x1;m_InitView[4]=y1;m_InitView[5]=z1;
	m_InitViewType = bLookAt;
	m_bLookTargetHaveMove = FALSE;
	if(bLookAt)
	{
		m_rotateView[0]=0;
		m_rotateView[1]=0;
		m_rotateView[2]=0;
		return SetLookAt(x,y,z,x1,y1,z1);
	}
	else
	{
		m_fLookTarget[0]=0;
		m_fLookTarget[1]=0;
		m_fLookTarget[2]=0;
		return SetView(x,y,z,x1,y1,z1);
	}
}
BOOL COpenGL::View::RestoreView()
{
	BOOL ret=SetInitView(m_InitView[0],m_InitView[1],m_InitView[2],m_InitView[3],m_InitView[4],m_InitView[5],m_InitViewType);
	g_MyGL.OnViewChange();
	//这里用SetInitView而不用SetView是为了方便。
	g_MyGL.OnPaint();
	return ret;
}
BOOL COpenGL::View::RestoreStep()
{
	return SetStep(m_InitStep[0],m_InitStep[1]);
}

BOOL COpenGL::View::SetView()
{
	for(int sw=0;sw<3;sw++)
	{
		if(m_rotateView[sw] >= 360)
			m_rotateView[sw] -= 360;
		else if(m_rotateView[sw] < 0)
			m_rotateView[sw] += 360;
		if(m_rotateView[sw] < 0.001)
			m_rotateView[sw] = 0;
	}

	glPushMatrix();
	glLoadIdentity();
	glRotatef(m_rotateView[0],0,1,0);
	glRotatef(m_rotateView[1],1,0,0);
	glRotatef(m_rotateView[2],0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,m_RotateMatrix);
	glPopMatrix();


/*
	//////////////////////////////////////////////////////////////////////////
	//测试更改
	glPushMatrix();
	glLoadIdentity();
	float tempx[16],tempy[16];
	glRotatef(m_rotateView[2],0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,tempx);
	glRotatef(m_rotateView[1],tempx[0],tempx[1],tempx[2]);
	glRotatef(m_rotateView[2],0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,tempy);
	glRotatef(m_rotateView[0],tempy[4],tempy[5],tempy[6]);
	glRotatef(m_rotateView[1],tempx[0],tempx[1],tempx[2]);
	glRotatef(m_rotateView[2],0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,m_RotateMatrix);
	glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
*/
	
	m_rotateVector[0] = -m_RotateMatrix[8];
	m_rotateVector[1] = -m_RotateMatrix[9];
	m_rotateVector[2] = -m_RotateMatrix[10];
	m_upVector[0] = m_RotateMatrix[4];
	m_upVector[1] = m_RotateMatrix[5];
	m_upVector[2] = m_RotateMatrix[6];

	if(m_pStatus)
	{
		CString strm,strr;
		strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
		strr.Format("h=%.2f,p=%.2f,r=%.2f",m_rotateView[0],m_rotateView[1],m_rotateView[2]);
		m_pStatus->SetPaneText(1,strm);
		m_pStatus->SetPaneText(2,strr);
	}
	return TRUE;
}
BOOL COpenGL::View::SetView(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat p, GLfloat r)
{
	m_moveView[0]=x;
	m_moveView[1]=y;
	m_moveView[2]=z;
	m_rotateView[0]=h;
	m_rotateView[1]=p;
	m_rotateView[2]=r;
	SetView();
	g_MyGL.OnViewChange();//这个函数不会由用户调用，会由OnViewChange调用因此不需要设定改变位
	return SetLookAt(false);
}
BOOL COpenGL::View::SetLookAt(float tx, float ty, float tz)
{
	m_fLookTarget[0]=tx;
	m_fLookTarget[1]=ty;
	m_fLookTarget[2]=tz;
	double dx = tx-m_moveView[0],dy=ty-m_moveView[1],dz=tz-m_moveView[2],dd,temp,verify;
	dd = sqrt(dx*dx+dy*dy+dz*dz);
	dx /= dd;
	dy /= dd;
	dz /= dd;
	if(m_rotateView[1]>90&&m_rotateView[1]<270)
		m_rotateView[1] = 180-RadToAng(asin(dy));
	else
		m_rotateView[1] = RadToAng(asin(dy));
	temp = cos(AngToRad(m_rotateView[1]));
	m_rotateView[0] = RadToAng(asin(-dx/temp));
	verify = -cos(AngToRad(m_rotateView[0]))*temp;
	if(verify*dz<0)
	{
		m_rotateView[0]=180-m_rotateView[0];
		verify = -cos(AngToRad(m_rotateView[0]))*temp;
	}

	SetView();
	g_MyGL.OnViewChange();
	return SetLookAt(true);
}
BOOL COpenGL::View::SetLookAt(BOOL bLookAt)
{
	m_bLookTarget = bLookAt;
	return TRUE;
}
BOOL COpenGL::View::SetLookAt(float x, float y, float z, float tx, float ty, float tz)
{
	m_moveView[0]=x;
	m_moveView[1]=y;
	m_moveView[2]=z;
	return SetLookAt(tx,ty,tz);
}

BOOL COpenGL::View::MoveViewDis(int len)
{
	GLfloat dis,pro;
	if(len == 0)
		return FALSE;
	float dir[3];
//	if(m_bLookTarget)
	{
		dir[0] = m_moveView[0] - m_fLookTarget[0];
		dir[1] = m_moveView[1] - m_fLookTarget[1];
		dir[2] = m_moveView[2] - m_fLookTarget[2];
	}
/*
	else
	{
		dir[0] = m_moveView[0];
		dir[1] = m_moveView[1];
		dir[2] = m_moveView[2];
	}
*/
	dis = sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
	if(dis==0)
	{
		if(len<0)
			return FALSE;
		else
			m_moveView[2] = len*m_moveStep;
	}
	else
	{
		if((len<0) && (-len*m_moveStep >= dis))//移近
			pro = -0.99;
		else pro = len*m_moveStep/dis;
		for(int i=0;i<3;i++)
			m_moveView[i] += pro*dir[i];
	}
	if(m_pStatus)
	{
		CString strm;
		strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
		m_pStatus->SetPaneText(1,strm);
	}
	g_MyGL.OnViewChange();
	return TRUE;
}

BOOL COpenGL::View::RotateView(int sw, int len)
{
	if(!m_bLookTarget)
	{
		m_rotateView[sw] += m_rotateStep*len;
		SetView();
	}
	else
	{
		if(m_bLookTargetHaveMove)
		{
			m_bLookTargetHaveMove = FALSE;
			SetLookAt(m_fLookTarget[0],m_fLookTarget[1],m_fLookTarget[2]);
		}
		else
		{
			double dis;
			float dx = m_fLookTarget[0]-m_moveView[0],dy=m_fLookTarget[1]-m_moveView[1],dz=m_fLookTarget[2]-m_moveView[2];
			dis = sqrt(dx*dx+dy*dy+dz*dz);
			m_rotateView[sw] -= m_rotateStep*len;
			SetView();
			m_moveView[0] = m_fLookTarget[0]+dis*m_RotateMatrix[8];//m_viewMatrix[8]表示cosx
			m_moveView[1] = m_fLookTarget[1]+dis*m_RotateMatrix[9];//m_viewMatrix[9]表示cosy
			m_moveView[2] = m_fLookTarget[2]+dis*m_RotateMatrix[10];//m_viewMatrix[10]表示cosz
			if(m_pStatus)
			{
				CString strm;
				strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
				m_pStatus->SetPaneText(1,strm);
			}
		}
	}
	g_MyGL.OnViewChange();
	return TRUE;
}

BOOL COpenGL::View::MoveView(int sw, int len)
{
	m_moveView[sw] += m_moveStep*len;
	if(m_pStatus)
	{
		CString strm;
		strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
		m_pStatus->SetPaneText(1,strm);
	}
	m_bLookTargetHaveMove = TRUE;
	g_MyGL.OnViewChange();
	return TRUE;
}
BOOL COpenGL::View::MoveViewRel(int sw, int len)
{
	float temp,dis[3];
	temp = len * m_moveStep;
	for(int i=0;i<3;i++)
	{
		dis[i]=m_RotateMatrix[sw*4+i]*temp;
		m_moveView[i] += dis[i];
	}
	if(m_bLookTarget)//如果按着shift移动，可以移动中心点。
	{
		for(i=0;i<3;i++)
		{
			m_fLookTarget[i] += dis[i];
		}
	}	
	if(m_pStatus)
	{
		CString strm;
		strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
		m_pStatus->SetPaneText(1,strm);
	}
	m_bLookTargetHaveMove = TRUE;
	g_MyGL.OnViewChange();
	return TRUE;
}
BOOL COpenGL::View::SetStep(float mstep, float sstep)
{
	m_moveStep = mstep;
	m_rotateStep = sstep;
	return TRUE;
}
BOOL COpenGL::View::GetStep(float &mstep, float &sstep)
{
	mstep=m_moveStep;
	sstep=m_rotateStep;
	return TRUE;
}
BOOL COpenGL::View::ShowView()
{
	gluLookAt(m_moveView[0],m_moveView[1],m_moveView[2],
		m_moveView[0]+m_rotateVector[0],m_moveView[1]+m_rotateVector[1],
		m_moveView[2]+m_rotateVector[2],m_upVector[0],m_upVector[1],m_upVector[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX,m_viewMatrix);
//	TRACE("%f,%f,%f\n",m_moveView[0],m_moveView[1],m_moveView[2]);
	return TRUE;
}
BOOL COpenGL::View::SetStatus(CStatusBar *pStatus)
{
	m_pStatus = pStatus;
	pStatus->SetPaneInfo(1,0,0,150);
	pStatus->SetPaneInfo(2,0,0,150);
	pStatus->SetPaneInfo(3,0,0,50);
	pStatus->SetPaneInfo(4,0,0,100);
	if(m_pStatus)
	{
		CString strm,strr,strfps;
		strm.Format("x=%.2f,y=%.2f,z=%.2f",m_moveView[0],m_moveView[1],m_moveView[2]);
		strr.Format("h=%.2f,p=%.2f,r=%.2f",m_rotateView[0],m_rotateView[1],m_rotateView[2]);
		strfps.Format("%.1f fps",0);
		m_pStatus->SetPaneText(1,strm);
		m_pStatus->SetPaneText(2,strr);
		m_pStatus->SetPaneText(3,strfps);
	}
	return TRUE;
}
BOOL COpenGL::View::ShowFPS(float fps)
{
	if(m_pStatus)
	{
		CString strfps;
		strfps.Format("%.1f fps",fps);
		m_pStatus->SetPaneText(3,strfps);
	}
	return TRUE;
}
BOOL COpenGL::View::ShowTriNum(int num)
{
	if(m_pStatus)
	{
		CString strtri;
		strtri.Format("%d tri",num);
		m_pStatus->SetPaneText(4,strtri);
	}
	return TRUE;
}
COpenGL::Control::Control()
{
	m_pView = 0;
	m_bMove = false;
	m_bRotate = false;
	SetMoveType("XY");
	SetRotateType("XY");
	m_keyMX=m_keyMY=m_keyMZ=m_keyRH=m_keyRP=m_keyRR=m_keyFN=false;
	m_keyChar = false;
	m_bShift = FALSE;
	m_bControlRoam = TRUE;
}
COpenGL::Control::~Control()
{
	m_pView = 0;
}

BOOL COpenGL::Control::SetParm(HWND hWnd,View *p)
{
	m_hWnd = hWnd;
	m_pView = p;
	return TRUE;
}

BOOL COpenGL::Control::MoveBegin(UINT lParam)
{
	SetCapture(m_hWnd);
	m_bMove = TRUE;
	m_iMouseOldPosX = LOWORD(lParam);  // horizontal position of cursor 
	m_iMouseOldPosY = HIWORD(lParam);  // vertical position of cursor 
	return TRUE;
}

BOOL COpenGL::Control::MoveEnd()
{
	ReleaseCapture();
	m_bMove = FALSE;
	return TRUE;
}

BOOL COpenGL::Control::RotateBegin(UINT lParam)
{
	SetCapture(m_hWnd);
	m_bRotate = TRUE;
	m_iMouseOldPosX = LOWORD(lParam);  // horizontal position of cursor 
	m_iMouseOldPosY = HIWORD(lParam);  // vertical position of cursor 	
	return TRUE;
}

BOOL COpenGL::Control::RotateEnd()
{
	ReleaseCapture();
	m_bRotate = FALSE;
	return TRUE;
}

BOOL COpenGL::Control::OnMoveRotate(UINT xPos,UINT yPos,UINT fwKeys)
{
	if(!m_bMove&&!m_bRotate) return FALSE;
	int dx = xPos - m_iMouseOldPosX ,dy = m_iMouseOldPosY - yPos;
	m_iMouseOldPosX = xPos;
	m_iMouseOldPosY = yPos;
	if(abs(dx)>1000||abs(dy)>1000) 
		return FALSE;
	if(m_bControlRoam)
		m_pView->SetLookAt(fwKeys&MK_SHIFT);
	else
	{
		dx = -dx; dy = -dy;
		m_pView->SetLookAt(!(fwKeys&MK_SHIFT));
	}
	if(m_bMove)
	{
		switch(m_iMoveType) {
		case MV_XY:	//向左右上下方向移动视点
			m_pView->MoveViewRel(0,dx);
			m_pView->MoveViewRel(1,dy);
			break;
		case MV_XZ:	//向左右前后方向移动视点
			m_pView->MoveViewRel(0,dx);
			m_pView->MoveViewRel(2,-dy);
			break;
		case MV_FN://很少用到，鼠标中轮移动远近和键盘移动都不会调用到这个函数，只有设置了移动模式，然后鼠标左键加上下移动才会调用它。
			m_pView->MoveViewDis(dy);
			break;
		}
	}
	else if(m_bRotate)
	{
		switch(m_iRotateType) {
		case MR_XY:	//在ＸＹ平面上旋转视点
			m_pView->RotateView(0,-dx);
			m_pView->RotateView(1,dy);
			break;
		case MR_XZ:	//在ＸＺ平面上旋转视点
			m_pView->RotateView(0,-dx);
			m_pView->RotateView(2,-dy);
			break;
		}
	}
	else return false;
	g_MyGL.OnPaint();
	return TRUE;
}

BOOL COpenGL::Control::SetMoveType(CString type)
{
	if(type == "XY")
		m_iMoveType = MV_XY;
	else if(type == "XZ")
		m_iMoveType = MV_XZ;
	else if(type=="X")
		m_iMoveType = MV_X;
	else if(type=="Y")
		m_iMoveType = MV_Y;
	else if(type=="Z")
		m_iMoveType = MV_Z;
	else if(type=="FN")
		m_iMoveType = MV_FN;
	return TRUE;
}

BOOL COpenGL::Control::SetRotateType(CString type)
{
	if(type == "XY")
		m_iRotateType = MR_XY;
	else if(type == "XZ")
		m_iRotateType = MR_XZ;
	else if(type=="X")
		m_iRotateType = MR_X;
	else if(type=="Y")
		m_iRotateType = MR_Y;
	else if(type=="Z")
		m_iRotateType = MR_Z;
	return TRUE;
}

BOOL COpenGL::Control::OnMoveFarNear(short len)
{	
	if (!m_pView->MoveViewDis(len/3))
		return FALSE;
	g_MyGL.OnPaint();
	return TRUE;
}

BOOL COpenGL::Control::AutoMove()
{
	static int last=0;
	if(m_bControlRoam==TRUE)
		m_pView->SetLookAt(m_bShift);
	else
		m_pView->SetLookAt(!m_bShift);
	BOOL m=FALSE;
	int n=GetTickCount();float o;
	if(last)
		o=(n-last)/20.0;
	else
		o=1;
	if(m_keyMX)
	{
		m_pView->MoveViewRel(0,m_keyMX*o);
		m=TRUE;
	}
	if(m_keyMY)
	{
		m_pView->MoveViewRel(1,m_keyMY*o);
		m=TRUE;
	}
	if(m_keyMZ)
	{
		m_pView->MoveViewRel(2,m_keyMZ*o);	
		m=TRUE;
	}
	if(m_keyFN)
	{
		m_pView->MoveViewDis(m_keyFN*o);
		m=TRUE;
	}
	if(m_keyRH)
	{
		m_pView->RotateView(0,m_keyRH);
		m=TRUE;
	}
	if(m_keyRP)
	{
		m_pView->RotateView(1,m_keyRP);
		m=TRUE;
	}
	if(m_keyRR)
	{
		m_pView->RotateView(2,m_keyRR);
		m=TRUE;
	}
	if(m)last=n;
	else last=0;
	return m;
}

BOOL COpenGL::Control::OnKeyDown(UINT nChar)
{
	if(g_MyGL.OnKeyDown(nChar)) return TRUE;
	int dis = 10;
	switch(nChar)
	{
	case VK_SHIFT:
		m_bShift = TRUE;
		break;
	case VK_SPACE://复位
		g_MyGL.RestoreView();
		break;
	case '+':
	case '=':
		{
			float mstep,sstep;
			g_MyGL.GetStep(mstep,sstep);
			mstep = mstep*1.1;
			g_MyGL.SetStep(mstep,sstep);
		}
	break;
	case '-':
	case '_':
		{
			float mstep,sstep;
			g_MyGL.GetStep(mstep,sstep);
			mstep=mstep*0.9;
			if(mstep<=0.001)mstep=0.001;
			g_MyGL.SetStep(mstep,sstep);
		}
		break;
	case 'A'://m_keyMX
	case 'a':
		m_keyMX = -dis;
		break;
	case 'D':
	case 'd':
		m_keyMX = +dis;
		break;
	case 'W'://m_keyMY
	case 'w':
		m_keyMY = +dis;
		break;
	case 'S':
	case 's':
		m_keyMY = -dis;
		break;
	case 'Q'://m_keyMZ
	case 'q':
		m_keyMZ = -dis;
		break;
	case 'E':
	case 'e':
		m_keyMZ = +dis;
		break;
	case 'Z'://m_keyFN
	case 'z':
		m_keyFN = dis;
		break;
	case 'C':
	case 'c':
		m_keyFN = -dis;
		break;

	case 'J'://m_keyMX
	case 'j':
		m_keyRH = +dis;
		break;
	case 'L':
	case 'l':
		m_keyRH = -dis;
		break;
	case 'I'://m_keyMY
	case 'i':
		m_keyRP = +dis;
		break;
	case 'K':
	case 'k':
		m_keyRP = -dis;
		break;
	case 'U'://m_keyMZ
	case 'u':
		m_keyRR = +dis;
		break;
	case 'O':
	case 'o':
		m_keyRR = -dis;
		break;
	default:
		return FALSE;
	}
	g_MyGL.OnPaint();
	return TRUE;
}

BOOL COpenGL::Control::OnKeyUp(UINT nChar)
{	
	switch(nChar)
	{
	case VK_SHIFT:
		m_bShift = FALSE;
		break;
	case 'A'://m_keyMX
	case 'a':
	case 'D':
	case 'd':
		m_keyMX = 0;
		break;
	case 'W'://m_keyMY
	case 'w':
	case 'S':
	case 's':
		m_keyMY = 0;
		break;
	case 'Q'://m_keyMZ
	case 'q':
	case 'E':
	case 'e':
		m_keyMZ = 0;
		break;
	case 'Z'://m_keyFN
	case 'z':
	case 'C':
	case 'c':
		m_keyFN = 0;
		break;

	case 'J'://m_keyRH
	case 'j':
	case 'L':
	case 'l':
		m_keyRH = 0;
		break;
	case 'I'://m_keyRP
	case 'i':
	case 'K':
	case 'k':
		m_keyRP = 0;
		break;
	case 'U'://m_keyRR
	case 'u':
	case 'O':
	case 'o':
		m_keyRR = 0;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
VOID COpenGL::Control::SetControlRoam(BOOL bRoam)
{
	m_bControlRoam = bRoam;
}
BOOL COpenGL::Control::SwitchType(BOOL bMove)
{
	if(bMove)
	{
		if(m_iMoveType==MV_XY)
			m_iMoveType = MV_XZ;
		else if(m_iMoveType==MV_XZ)
			m_iMoveType = MV_XY;
		return TRUE;
	}
	else
	{
		if(m_iRotateType==MR_XY)
			m_iRotateType = MR_XZ;
		else if(m_iRotateType == MR_XZ)
			m_iRotateType = MR_XY;
		return TRUE;
	}
	return FALSE;
}

BOOL COpenGL::Control::OnKey(UINT message, UINT nChar)
{
	switch(message)
	{
	case WM_KEYDOWN:
		if(nChar>0XA5&&nChar<0XF6)	m_keyChar = TRUE;
		else OnKeyDown(nChar);
		break;
	case WM_KEYUP:
		OnKeyUp(nChar);
		m_keyChar = FALSE;
		break;
	case WM_CHAR:
		if(m_keyChar)
			OnKeyDown(nChar);
		break;
	}
	return TRUE;
}
BOOL COpenGL::Control::OnMouse(UINT message, UINT wParam, UINT lParam)
{
	switch(message) {
	case WM_LBUTTONDOWN:
		MoveBegin(lParam);
		break;
	case WM_LBUTTONUP:
		MoveEnd();
		break;
	case WM_LBUTTONDBLCLK:
		if(wParam&MK_CONTROL)
			SwitchType(TRUE);
		break;
	case WM_RBUTTONDOWN:
		RotateBegin(lParam);
		break;
	case WM_RBUTTONUP:
		RotateEnd();
		break;
	case WM_RBUTTONDBLCLK:
		if(wParam&MK_CONTROL)
			SwitchType(FALSE);
		break;
	case WM_MOUSEMOVE:
		OnMoveRotate(LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	case WM_MOUSEWHEEL:
		OnMoveFarNear(HIWORD(wParam));
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
COpenGL::CFrustum::CFrustum()
{
}
COpenGL::CFrustum::~CFrustum()
{
}
float *COpenGL::CFrustum::CalculateClip()
{
	GLfloat proj[16],modl[16];
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );
	m_Clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	m_Clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	m_Clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	m_Clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
	
	m_Clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	m_Clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	m_Clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	m_Clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
	
	m_Clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	m_Clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	m_Clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	m_Clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
	
	m_Clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	m_Clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	m_Clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	m_Clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];		
	CalculateFrustum();
	return m_Clip;
}
void COpenGL::CFrustum::NormalParam(float param[])
{
	double nor;
	nor = sqrt(param[0]*param[0]+param[1]*param[1]+param[2]*param[2]);
	for(int i=0;i<4;i++)
		param[i] /= nor;
}
void COpenGL::CFrustum::CalculateFrustum()
{
	m_Frustum[RIGHT][0] = m_Clip[ 3] - m_Clip[ 0];
	m_Frustum[RIGHT][1] = m_Clip[ 7] - m_Clip[ 4];
	m_Frustum[RIGHT][2] = m_Clip[11] - m_Clip[ 8];
	m_Frustum[RIGHT][3] = m_Clip[15] - m_Clip[12];	
	NormalParam(m_Frustum[RIGHT]);
	
	// This will extract the LEFT side of the frustum
	m_Frustum[LEFT][0] = m_Clip[ 3] + m_Clip[ 0];
	m_Frustum[LEFT][1] = m_Clip[ 7] + m_Clip[ 4];
	m_Frustum[LEFT][2] = m_Clip[11] + m_Clip[ 8];
	m_Frustum[LEFT][3] = m_Clip[15] + m_Clip[12];
	NormalParam(m_Frustum[LEFT]);	

	// This will extract the BOTTOM side of the frustum
	m_Frustum[BOTTOM][0] = m_Clip[ 3] + m_Clip[ 1];
	m_Frustum[BOTTOM][1] = m_Clip[ 7] + m_Clip[ 5];
	m_Frustum[BOTTOM][2] = m_Clip[11] + m_Clip[ 9];
	m_Frustum[BOTTOM][3] = m_Clip[15] + m_Clip[13];
	NormalParam(m_Frustum[BOTTOM]);
	
	// This will extract the TOP side of the frustum
	m_Frustum[TOP][0] = m_Clip[ 3] - m_Clip[ 1];
	m_Frustum[TOP][1] = m_Clip[ 7] - m_Clip[ 5];
	m_Frustum[TOP][2] = m_Clip[11] - m_Clip[ 9];
	m_Frustum[TOP][3] = m_Clip[15] - m_Clip[13];
	NormalParam(m_Frustum[TOP]);
	
	// This will extract the BACK side of the frustum
	m_Frustum[BACK][0] = m_Clip[ 3] - m_Clip[ 2];
	m_Frustum[BACK][1] = m_Clip[ 7] - m_Clip[ 6];
	m_Frustum[BACK][2] = m_Clip[11] - m_Clip[10];
	m_Frustum[BACK][3] = m_Clip[15] - m_Clip[14];
	NormalParam(m_Frustum[BACK]);
	
	// This will extract the FRONT side of the frustum
	m_Frustum[FRONT][0] = m_Clip[ 3] + m_Clip[ 2];
	m_Frustum[FRONT][1] = m_Clip[ 7] + m_Clip[ 6];
	m_Frustum[FRONT][2] = m_Clip[11] + m_Clip[10];
	m_Frustum[FRONT][3] = m_Clip[15] + m_Clip[14];
	NormalParam(m_Frustum[FRONT]);
}
float COpenGL::CFrustum::CalculateDistance(float v[])
{
	float min = 0;
	for(int i=0;i<6;i++)//>0表示在内部
	{
		float d;
		d = m_Frustum[i][A] * v[0] + m_Frustum[i][B] * v[1] + m_Frustum[i][C] * v[2] + m_Frustum[i][D];
		if(d<min)
		{
			min = d;
		}
	}
	return min;
}
int COpenGL::CFrustum::CalculatePosition(float v[])
{
	//书上的顺序是左右下上前后。我的顺序是右左下上后前
	//使用三角形多个视点与判断是否在视锥体内的方法的缺点：
	//1、有些不在视点的线段（经过交点）被认为在视点中
	//2、当点在视锥体后面，判断是否在某个面之外会出现错误。
	int ret = 0;
	for(int i=0;i<6;i++)//>0表示在内部
	{
		float d;
		d = m_Frustum[i][A] * v[0] + m_Frustum[i][B] * v[1] + m_Frustum[i][C] * v[2] + m_Frustum[i][D];
		if(d<=0)
		{
			ret |= 1<<i;
		}
	}
	return ret;
}
int COpenGL::CFrustum::CalculateBoundingBox(float v[])
{
	//判断八个点在视锥体的位置
	//点1:v[0],v[2],v[4];
	//点2:v[0],v[2],v[5];
	//点3:v[0],v[3],v[4];
	//点4:v[0],v[3],v[5];
	//点5:v[1],v[2],v[4];
	//点6:v[1],v[2],v[5];
	//点7:v[1],v[3],v[4];
	//点8:v[1],v[3],v[5];
	int index[24]={
		0,2,4,
		0,2,5,
		0,3,4,
		0,3,5,
		1,2,4,
		1,2,5,
		1,3,4,
		1,3,5
	};
	int ret[8];
	memset(ret,0,32);
	for(int n=0;n<8;n++)
	{
		for(int i=0;i<6;i++)//>0表示在内部
		{
			float d;
			d = m_Frustum[i][A] * v[index[3*n]] + m_Frustum[i][B] * v[index[3*n+1]] + m_Frustum[i][C] * v[index[3*n+2]] + m_Frustum[i][D];
			if(d<=0)
			{
				ret[n] |= 1<<i;
			}
		}
	}
	if((ret[0]|ret[1]|ret[2]|ret[3]|ret[4]|ret[5]|ret[6]|ret[7])==0)//全为0
		return 1;//全在视锥体中
	else if((ret[0]&&ret[1]&&ret[2]&&ret[3]&&ret[4]&&ret[5]&&ret[6]&&ret[7])==0)
		return 2;//至少有一个点在视锥体中
	else if((ret[0]&ret[1]&ret[2]&ret[3]&ret[4]&ret[5]&ret[6]&ret[7])==0)//与操作后为0
		return 0;//部分在视锥体中
	else return -1;
}
int COpenGL::CFrustum::ConvertPosition(float v[])
{
	float a[4];
	for(int i=0;i<4;i++)
		a[i]=m_Clip[0+i]*v[0]+m_Clip[4+i]*v[1]+m_Clip[8+i]*v[2]+m_Clip[12+i];
	if(a[3]<-0.001||a[3]>0.001)
	{
		for(i=0;i<3;i++)
			a[i]/=a[3];
	}
	else
		a[3]=0;
	v[0] = a[0];
	v[1] = a[1];
	v[2] = a[2];
	int ret = 0;
	if(a[3]<0)	//判断w是否小于0，如果小于0，符号取反。
	{
		if(a[0]<1)ret|=1;	//RIGHT
		if(a[0]>-1)ret|=2;	//LEFT
		if(a[1]>-1)ret|=4;	//BOTTOM
		if(a[1]<1)ret|=8;	//UP
		if(a[2]<1)ret|=16;	//BACK
		if(a[2]>-1)ret|=32;	//FRONT
	}
	else if(a[3]>0)
	{
		if(a[0]>1)ret|=1;	//RIGHT
		if(a[0]<-1)ret|=2;	//LEFT
		if(a[1]<-1)ret|=4;	//BOTTOM
		if(a[1]>1)ret|=8;	//UP
		if(a[2]>1)ret|=16;	//BACK
		if(a[2]<-1)ret|=32;	//FRONT
	}
	else//肯定在裁剪区外！！！
	{
		//	AfxMessageBox("error");
		if(a[0]>0)ret|=1;	//RIGHT
		if(a[0]<0)ret|=2;	//LEFT
		if(a[1]<0)ret|=4;	//BOTTOM
		if(a[1]>0)ret|=8;	//UP
		if(a[2]>0)ret|=16;	//BACK
		if(a[2]<0)ret|=32;	//FRONT
	}
	return ret;
}

COpenGL::CScene::CScene()
{
	m_bList=TRUE;
	m_uList=0;
	iTri=0;
	m_InitMatrix = new float[16];
	for(int i=0;i<16;i++)
		m_InitMatrix[i]=0;
	m_InitMatrix[0]=1;
	m_InitMatrix[5]=1;
	m_InitMatrix[10]=1;
	m_InitMatrix[15]=1;
}


COpenGL::CScene::~CScene()
{
	if(m_uList>0)
		glDeleteLists(m_uList,1);
	groups.clear();
	materials.clear();
	maps.clear();
	vertices.clear();
	vertices_normal.clear();
	delete m_InitMatrix;
}
void COpenGL::CScene::Reset()
{
	if(m_uList>0)
		glDeleteLists(m_uList,1);
	groups.clear();
	materials.clear();
	maps.clear();
	vertices.clear();
	vertices_normal.clear();
	delete m_InitMatrix;
}
void COpenGL::CScene::Draw(int &numTriangle)
{
	if(m_uList>0)
	{
		glCallList(m_uList);
	}
	else render();
	numTriangle+=iTri;
}
void COpenGL::CScene::SetInitMatrix(float matrix[])
{
	for(int i=0;i<16;i++)
		m_InitMatrix[i]=matrix[i];
}
void COpenGL::CScene::GetDirect(const char *str,CString &path,CString &pathPre){
	path=str;	
	int i=path.ReverseFind('/');
	int j=path.ReverseFind('\\');
	int k=i>j?i:j;
	if(k>=0)
		path = path.Left(k+1);
	else
		path.Empty();

	BOOL isAbs;
	if(path.GetLength()<=1)
		isAbs = FALSE;
	else if(path.GetAt(1)==':')
		isAbs = TRUE;
	else isAbs = FALSE;
	if(!isAbs)//给出的是相对路径
		path = m_path +path;
	else path.Empty();
	if(path.IsEmpty())
		pathPre.Empty();
	else
	{
		pathPre = path;
		if(k==i)
			pathPre.TrimRight("/");
		else
			pathPre.TrimRight("\\");
		i=pathPre.ReverseFind('/');
		j=pathPre.ReverseFind('\\');
		k=i>j?i:j;
		if(k>=0)
			pathPre = pathPre.Left(k+1);
		else
			pathPre.Empty();
	}
}
void COpenGL::CScene::Init(float* matrix,BOOL bList)
{
	m_bList = bList;
	if(matrix)
		SetInitMatrix(matrix);
	int n=maps.size();
	MATMAP map;
	CString path,pathPre;
	for(int i=0;i<n;i++)
	{
		map = maps[i];
		GetDirect(map.mapname,path,pathPre);
		if(!LoadTextureMap(map,path))
				if(!LoadTextureMap(map,pathPre+"Textures/"))
					LoadTextureMap(map,path+"Textures/");

		maps[i]=map;
	}
	CreateNormal();
}
float *COpenGL::CScene::GetInitMatrix()
{
	return m_InitMatrix;
}
void COpenGL::CScene::SetTriNum(int tri)
{
	iTri = tri;
}
int COpenGL::CScene::GetTriNum()
{
	return iTri;
}
void COpenGL::CScene::SetListNo(UINT List)
{
	m_uList = List;
}
UINT COpenGL::CScene::GetListNo()
{
	return m_uList;
}
void COpenGL::CScene::render()
{
	VECTOR p[3];
	NORMAL nor[4];
	INDEX id;
	MAPCOORD coord[3];
	iTri = 0;
	int n=groups.size();
	int m=0,l=0;
	vector<VECTOR> *pVer;//点列表
	vector<NORMAL> *pVer_Nor;//点法向量
	pVer=&vertices;
	pVer_Nor=&vertices_normal;
	
	for(int i=0;i<n;i++)//所有组
	{
		if(groups[i].isOn==false)
			continue;
		if(vertices.size()==0)
		{
			pVer = &groups[i].vertices;
			pVer_Nor=&groups[i].vertices_normal;
		}
		m=groups[i].material_index.size();
		for(int j=0;j<m;j++)//所有材质
		{
			//材质操作
			MAT mat;
			MAT_INDEX M_INDEX;
			MATMAP matmap;
			M_INDEX = groups[i].material_index[j];
			if(M_INDEX.mat_index>=0)
			{

				mat = materials[M_INDEX.mat_index];
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.mat_ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.mat_diffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.mat_specular);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat.mat_emission);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat.mat_shininess);

			}
			else
			{
				mat.mat_ambient[0]=0.4;mat.mat_ambient[1]=0.4;mat.mat_ambient[2]=0.4;mat.mat_ambient[3]=1;
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat.mat_ambient);				
			}
			if(M_INDEX.tex_index>=0)
				matmap = maps[M_INDEX.tex_index];
			else matmap.texture=0;
			if(M_INDEX.color!=-1)
			{
				BYTE r,g,b;
				r=GetRValue(M_INDEX.color);
				g=GetGValue(M_INDEX.color);
				b=GetBValue(M_INDEX.color);
				glColor3ub(r,g,b);
				if(M_INDEX.mat_index==-1)
				{
					mat.mat_ambient[0]=r/255.0;mat.mat_ambient[1]=g/255.0;mat.mat_ambient[2]=b/255.0;mat.mat_ambient[3]=1;
					mat.mat_diffuse[0]=0;mat.mat_diffuse[1]=0;mat.mat_diffuse[2]=0;mat.mat_diffuse[3]=1;
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.mat_ambient);									
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.mat_diffuse);
				}
			}
			else 
 				glColor3fv(mat.mat_ambient);
			l=M_INDEX.facets_index.size();
			iTri += l;	
			if(matmap.texture==0)//没有帖图
			{
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLES);
				for(int k=0;k<l;k++){//所有面
					int iface;
					iface=M_INDEX.facets_index[k];
					id = groups[i].facets[iface];
					nor[3]=groups[i].facets_normal[iface];
					for(int d=0;d<3;d++){
						p[d]=(*pVer)[id.a[d]];
						nor[d]=(*pVer_Nor)[id.a[d]];
						if(nor[d].bValid)
							glNormal3fv(nor[d].a);
						else
							glNormal3fv(nor[3].a);
						glVertex3fv(p[d].a);
					}
				}
				glEnd();	
			}
			else//有帖图，还需要法向量吗？？？？
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, matmap.texture);
				glBegin(GL_TRIANGLES);
				for(int k=0;k<l;k++)//所有面
				{
					int iface;
					iface=M_INDEX.facets_index[k];
					id = groups[i].facets[iface];
					nor[3]=groups[i].facets_normal[iface];					
					for(int d=0;d<3;d++){
						p[d]=(*pVer)[id.a[d]];
						nor[d]=(*pVer_Nor)[id.a[d]];						
						coord[d]=groups[i].mapcoord[id.a[d]];
						if(nor[d].bValid)
							glNormal3fv(nor[d].a);
						else
							glNormal3fv(nor[3].a);					
						glTexCoord2fv(coord[d].a);
						glVertex3fv(p[d].a);
					}
				}
				glEnd();
			}
		}
		if(m==0/*&&l>0*/)//会不会出现有些面有材质，有些面没有材质的情况？现在没有考虑。会的
		{			
			float test[4]={0.4,0.4,0.4,1};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, test);
			glColor3fv(test);
			glDisable(GL_TEXTURE_2D);
			l=groups[i].facets.size();
			iTri += l;
			glBegin(GL_TRIANGLES);
			for(int k=0;k<l;k++)//所有面
			{
				id = groups[i].facets[k];
				nor[3]=groups[i].facets_normal[k];
				for(int d=0;d<3;d++)
				{
					p[d]=(*pVer)[id.a[d]];
					nor[d]=(*pVer_Nor)[id.a[d]];					
					if(nor[d].bValid)
						glNormal3fv(nor[d].a);
					else
						glNormal3fv(nor[3].a);
					glVertex3fv(p[d].a);
				}
			}

			glEnd();
		}
	}	
}
bool COpenGL::CScene::CreateNormal(BOOL bVertices)
{
	vector<VECTOR> *pVer;//点列表
	vector<NORMAL> *pVer_Nor;//点法向量	
	pVer=&vertices;
	pVer_Nor=&vertices_normal;
	int t=groups.size();
	//产生点法向量列表。
	for(int i=0;i<t;i++)
	{
		if(vertices.size()==0)
		{
			pVer = &groups[i].vertices;
			pVer_Nor=&groups[i].vertices_normal;
		}
		//int m=groups[i].vertices.size();
		int m=(*pVer).size();
		for(int j=0;j<m;j++)
		{
			//修改点位置。
			VECTOR dotvector;
		//	dotvector = groups[i].vertices[j];
			dotvector = (*pVer)[j];
			double w;
			w=m_InitMatrix[3]*dotvector.a[0]+m_InitMatrix[7]*dotvector.a[1]+m_InitMatrix[11]*dotvector.a[2]+m_InitMatrix[15];
			for(int l=0;l<3;l++)
			//	groups[i].vertices[j].a[l]=float((m_InitMatrix[l]*dotvector.a[0]+m_InitMatrix[4+l]*dotvector.a[1]+m_InitMatrix[8+l]*dotvector.a[2]+m_InitMatrix[12+l])/w);			
				(*pVer)[j].a[l]=float((m_InitMatrix[l]*dotvector.a[0]+m_InitMatrix[4+l]*dotvector.a[1]+m_InitMatrix[8+l]*dotvector.a[2]+m_InitMatrix[12+l])/w);			

			COpenGL::CScene::NORMAL nor;
			nor.bValid = bVertices;
			//groups[i].vertices_normal.push_back(nor);//法向量
			(*pVer_Nor).push_back(nor);//法向量
		}
	}
	for(i=0;i<t;i++)
	{
		if(vertices.size()==0)
		{
			pVer = &groups[i].vertices;
			pVer_Nor=&groups[i].vertices_normal;
		}		
		int m=groups[i].facets.size();
		for(int j=0;j<m;j++)
		{
			//计算面法向向量	
			VECTOR dotvector[3];
			for(int k=0;k<3;k++)
				dotvector[k]=(*pVer)[groups[i].facets[j].a[k]];
			//	dotvector[k]=groups[i].vertices[groups[i].facets[j].a[k]];
			NORMAL nor;
			//CalculateNormal(dotvector[0].a,dotvector[1].a,dotvector[2].a,nor.a);
			CalculateNormal(dotvector[0].a,dotvector[1].a,dotvector[2].a,nor.a);
			groups[i].facets_normal.push_back(nor);
			if(!bVertices)
				continue;
			for(k=0;k<3;k++)
			{
				int d;
				d = groups[i].facets[j].a[k];
				//判断方向是否相同。
				float a[3],b[3],sum=0;
				for(int l=0;l<3;l++)
				{
					b[l]=groups[i].facets_normal[j].a[l];
					//a[l]=groups[i].vertices_normal[d].a[l];
					a[l]=(*pVer_Nor)[d].a[l];
					sum += fabs(a[l]);
				}
				if(sum>0.01)//表示不是初始值。
				{
					COpenGL::Normalize(a);
					if(a[0]*b[0]+a[1]*b[1]+a[2]*b[2]<0.8)//如果是边界则不容和法向量823,点乘小，说明夹角大，不共曲面，不能中和
						(*pVer_Nor)[d].bValid = FALSE;
						//groups[i].vertices_normal[d].bValid = FALSE;
				}
				//if(groups[i].vertices_normal[d].bValid == TRUE)
				if((*pVer_Nor)[d].bValid == TRUE)
					for(l=0;l<3;l++)
						(*pVer_Nor)[d].a[l]+=b[l];
						//groups[i].vertices_normal[d].a[l]+=b[l];
			}
		}
		if(bVertices)
		{
			//int n=groups[i].vertices.size();
			int n=(*pVer).size();
			for(j=0;j<n;j++)
				Normalize((*pVer_Nor)[j].a);
			//	Normalize(groups[i].vertices_normal[j].a);
		}
	}
	if(m_bList)
		UseShowList();
	return true;
}
UINT COpenGL::CScene::SetTextureMap(ImageData *TextureImage)
{
	UINT texture=0;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST); // 线形滤波
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST); // 线形滤波
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
	if(TextureImage->m_nColor==4)
		gluBuild2DMipmaps(GL_TEXTURE_2D,  4,
		TextureImage->m_Width, TextureImage->m_Height,
		GL_RGBA, GL_UNSIGNED_BYTE, TextureImage->m_pData);
	else if(TextureImage->m_nColor==3)
		gluBuild2DMipmaps(GL_TEXTURE_2D,  3,
		TextureImage->m_Width, TextureImage->m_Height,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->m_pData);
	else texture=0;
	return texture;
}
UINT COpenGL::CScene::LoadTextureMap(MATMAP &map,CString path)
{
	ImageData *TextureImage;				// Create Storage Space For The Texture	
	CString filename;
	filename = path+map.mapname;
	int color=RGB(0,0,0);
	TextureImage=(ImageData*)g_MyGL.picLoader.GetData(filename.GetBuffer(0),&color);
	if(TextureImage)
	{
		map.texture=SetTextureMap(TextureImage);
		delete TextureImage;
	}
	else
		map.texture=0;
	return map.texture;
}
UINT COpenGL::CScene::LoadTextureMap(CString name,CString path)
{
	ImageData *TextureImage;				// Create Storage Space For The Texture	
	CString filename;
	filename = path+name;	
	int color=RGB(0,0,0);
	TextureImage=(ImageData*)g_MyGL.picLoader.GetData(filename.GetBuffer(0),&color);
	UINT texture=0;
	if(TextureImage)
	{
		texture=SetTextureMap(TextureImage);
		delete TextureImage;
	}
	return texture;	
}
UINT COpenGL::CScene::LoadTextureCubeMap(CString name[],CString path)
{
	ImageData *TextureImage;				// Create Storage Space For The Texture	
	CString filename;

	glEnable ( GL_TEXTURE_CUBE_MAP_EXT );
	//glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	UINT texture=0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, texture);
	glTexParameteri ( GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );	
	for(int i=0;i<6;i++)
	{
		filename = path+name[i];
		int color=RGB(0,0,0);
		TextureImage=(ImageData*)g_MyGL.picLoader.GetData(filename.GetBuffer(0),&color);
		UINT texture=0;
		if(TextureImage)
		{
			if(TextureImage->m_nColor==4)
				gluBuild2DMipmaps ( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, GL_RGB8, 
				TextureImage->m_Width, TextureImage->m_Height, 
				GL_RGBA, GL_UNSIGNED_BYTE, TextureImage->m_pData );
			else if(TextureImage->m_nColor==3)
				gluBuild2DMipmaps ( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, GL_RGB8, 
				TextureImage->m_Width, TextureImage->m_Height, 
				GL_RGB, GL_UNSIGNED_BYTE, TextureImage->m_pData );			
			else texture=0;
			delete TextureImage;
		}
	}
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	glDisable ( GL_TEXTURE_CUBE_MAP_EXT );	
	return texture;	
}
bool COpenGL::CScene::UseShowList()
{
	if(m_uList!=0)
		return false;
	m_uList=glGenLists(1);
	glNewList(m_uList,GL_COMPILE);
	render();
	glEndList();
	return true;
}
COpenGL::CPictureLoad::CPictureLoad()
{
	m_pPict = NULL;
	m_bInit=FALSE;
	m_bTransparence=FALSE;
}
VOID COpenGL::CPictureLoad::Init()
{
	m_hdc=wglGetCurrentDC();
	m_memBMP.CreateCompatibleBitmap(CDC::FromHandle(m_hdc),1024,768);
	m_memDC.CreateCompatibleDC(CDC::FromHandle(m_hdc));
	m_memDC.SelectObject(m_memBMP);
	int mx = GetDeviceCaps(m_hdc,HORZSIZE);
	int my = GetDeviceCaps(m_hdc,VERTSIZE);
	int px = GetDeviceCaps(m_hdc,HORZRES);
	int py = GetDeviceCaps(m_hdc,VERTRES);
	m_PixelToMillimeterX = mx*100/px;
	m_PixelToMillimeterY = my*100/py;
	m_bInit = TRUE;
}
COpenGL::CPictureLoad::~CPictureLoad()
{
	if(m_bInit)
	{
		DeleteObject(m_memBMP);
		DeleteObject(m_memDC);
		UnloadPicture();
	}
}
//-----------------------------------------------------------------------------
// Does: Free The Allocated Memory That Holdes The IPicture Interface Data
// ~~~~ And Clear Picture Information
//
// Note: This Might Also Be Useful If U Only Need To Show The Picture Once
// ~~~~~ Or If U Copy The Picture To The Device Context, So It Can Still
// Remain On Screen - But IPicture Data Is Not Needed No More
//
//-----------------------------------------------------------------------------
void COpenGL::CPictureLoad::UnloadPicture()
//=============================================================================
{
	if(m_pPict != NULL){
		m_pPict->Release();
		m_pPict = NULL;
	}
}
//-----------------------------------------------------------------------------
// Does: Open a Resource And Load It Into IPicture (Interface)
// ~~~~ (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// Note: When Adding a Bitmap Resource It Would Automatically Show On "Bitmap"
// ~~~~ This NOT Good Coz We Need To Load It From a Custom Resource "BMP"
// To Add a Custom Rresource: Import Resource -> Open As -> Custom
// (Both .BMP And .DIB Should Be Found Under "BMP")
//
// InPut: ResourceName - As a UINT Defined (Example: IDR_PICTURE_RESOURCE)
// ~~~~~ ResourceType - Type Name (Example: "JPG")
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL COpenGL::CPictureLoad::Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType)
//=============================================================================
{
	HGLOBAL hGlobal = NULL;
	HRSRC hSource = NULL;
	LPVOID lpVoid = NULL;
	int nSize = 0;
	BOOL bResult=FALSE;
	if(m_pPict != NULL) UnloadPicture(); // Important - Avoid Leaks...
	
	hSource = FindResource(hInstance, lpszResourceName, ResourceType);
	
	if(hSource == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "FindResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	hGlobal = LoadResource(hInstance, hSource);
	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "LoadResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "LockResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	nSize = (UINT)SizeofResource(hInstance, hSource);
	if(LoadPictureData((BYTE*)hGlobal, nSize)) bResult = TRUE;
	
	UnlockResource(hGlobal); // 16Bit Windows Needs This
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	return(bResult);
}
//-----------------------------------------------------------------------------
// Does: Open a File And Load It Into IPicture (Interface)
// ~~~~ (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// InPut: sFilePathName - Path And FileName Target To Save
// ~~~~~ 
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL COpenGL::CPictureLoad::Load(CString sFilePathName)
//=============================================================================
{
//	if(!PathFileExists(sFilePathName))return FALSE;
	BOOL bResult = FALSE;
	CFile PictureFile;
	CFileException e;
	int nSize = 0;
	
	if(m_pPict != NULL) UnloadPicture(); // Important - Avoid Leaks...
	
	if(PictureFile.Open(sFilePathName, CFile::modeRead | CFile::typeBinary, &e))
	{
		nSize = PictureFile.GetLength();
		BYTE* pBuffer = new BYTE[nSize];
		
		if(PictureFile.Read(pBuffer, nSize) > 0)
		{
			if(LoadPictureData(pBuffer, nSize)) bResult = TRUE;
		}
		
		PictureFile.Close();
		delete [] pBuffer;
	}
	else // Open Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
//		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
//		MessageBoxEx(hWnd, szCause,NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}
	return(bResult);
}
//-----------------------------------------------------------------------------
// Does: Read The Picture Data From a Source (File / Resource)
// ~~~~ And Load It Into The Current IPicture Object In Use
//
// InPut: Buffer Of Data Source (File / Resource) And Its Size
// ~~~~~ 
//
// OutPut: Feed The IPicture Object With The Picture Data
// ~~~~~~ (Use Draw Functions To Show It On a Device Context)
// TRUE If Succeeded...
//-----------------------------------------------------------------------------
BOOL COpenGL::CPictureLoad::LoadPictureData(BYTE *pBuffer, int nSize)
//=============================================================================
{	
	BOOL bResult = FALSE;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	
	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "Can not allocate enough memory\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pBuffer, nSize);
	GlobalUnlock(hGlobal);
	
	IStream* pStream = NULL;
	
	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr;
		if((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPict)) == E_NOINTERFACE)
		{
			HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
			MessageBoxEx(hWnd, "IPicture interface is not supported\t", NULL, /*ERROR_TITLE, */MB_OK | MB_ICONSTOP, LANG_ENGLISH);
			return(FALSE);
		}
		else // S_OK
		{
			pStream->Release();
			pStream = NULL;
			bResult = TRUE;
		}
	}
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	CalculateWidthHeight();
	return(bResult);
}

//-----------------------------------------------------------------------------
// Does: Draw The Loaded Picture Direct To The Client DC
// ~~~~
//
// Note: Bigger OR Smaller Dimentions Than The Original Picture Size
// ~~~~ Will Draw The Picture Streached To Its New Given NEW Dimentions...
//
// InPut: pDC - Given DC To Draw On
// ~~~~~ pSrcRect- Dimentions Of The Picture To Draw From(As a Rectangle)
// DrawRect - Dimentions Of The Picture To Draw To(As a Rectangle)
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
//=============================================================================
void COpenGL::CPictureLoad::Render(CDC* pDC, LPRECT pDrawRect, LPRECT pSrcRect/*=NULL*/,LPCRECT prcWBounds/*=NULL*/)
{
	if (pDC == NULL || m_pPict == NULL) return ;
	
	CRect recrDest(pDrawRect);
	
	long Width = 0;
	long Height = 0;
	m_pPict->get_Width(&Width);
	m_pPict->get_Height(&Height);
	
	CRect SrcRect(0,0,Width,Height);
	
	if(pSrcRect)
	{
		SrcRect=*pSrcRect;
	}
	CRect DrawRect(pDrawRect);
	HRESULT hrP = NULL;
	
	hrP = m_pPict->Render(pDC->m_hDC,
		DrawRect.left, // Left
		DrawRect.top, // Top
		DrawRect.Width(), // Right
		DrawRect.Height(), // Bottom
		SrcRect.left,
		SrcRect.bottom,
		SrcRect.Width(),
		-SrcRect.Height(),
		prcWBounds
		);
	if (SUCCEEDED(hrP)) return;
	AfxThrowMemoryException();
	return;
}
// Does: Saves The Picture That Is Stored In The IPicture Object As a Bitmap
// ~~~~ (Converts From Any Known Picture Type To a Bitmap / Icon File)
//
// InPut: sFilePathName - Path And FileName Target To Save
// ~~~~~
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL COpenGL::CPictureLoad::SaveAsBitmap(CString sFilePathName)
//=============================================================================
{
	BOOL bResult = FALSE;
	ILockBytes *Buffer = 0;
	IStorage *pStorage = 0;
	IStream *FileStream = 0;
	BYTE *BufferBytes;
	STATSTG BytesStatistics;
	DWORD OutData;
	long OutStream;
	CFile BitmapFile; 
	CFileException e;
	double SkipFloat = 0;
	DWORD ByteSkip = 0;
	_ULARGE_INTEGER RealData;

	CreateILockBytesOnHGlobal(NULL, TRUE, &Buffer); // Create ILockBytes Buffer

	HRESULT hr = ::StgCreateDocfileOnILockBytes(Buffer,
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);

	hr = pStorage->CreateStream(L"PICTURE",
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &FileStream);

	m_pPict->SaveAsFile(FileStream, TRUE, &OutStream); // Copy Data Stream
	FileStream->Release();
	pStorage->Release();
	Buffer->Flush(); 

	// Get Statistics For Final Size Of Byte Array
	Buffer->Stat(&BytesStatistics, STATFLAG_NONAME);

	// Cut UnNeeded Data Coming From SaveAsFile() (Leave Only "Pure" Picture Data)
	SkipFloat = (double(OutStream) / 512); // Must Be In a 512 Blocks...
	if(SkipFloat > DWORD(SkipFloat)) 
		ByteSkip = (DWORD)SkipFloat + 1;
	else ByteSkip = (DWORD)SkipFloat;
	ByteSkip = ByteSkip * 512; // Must Be In a 512 Blocks...

	// Find Difference Between The Two Values
	ByteSkip = (DWORD)(BytesStatistics.cbSize.QuadPart - ByteSkip);

	// Allocate Only The "Pure" Picture Data
	RealData.LowPart = 0;
	RealData.HighPart = 0;
	RealData.QuadPart = ByteSkip;
	BufferBytes = (BYTE*)malloc(OutStream);
	if(BufferBytes == NULL)
	{
		Buffer->Release();
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "Can not allocate enough memory\t",NULL, /*ERROR_TITLE,*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	}

	Buffer->ReadAt(RealData, BufferBytes, OutStream, &OutData);

	if(BitmapFile.Open(sFilePathName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite, &e))
	{
		BitmapFile.Write(BufferBytes, OutData);
		BitmapFile.Close();
		bResult = TRUE;
	}
	else // Write File Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, szCause, NULL, /*ERROR_TITLE,*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}

	Buffer->Release();
	free(BufferBytes);

	return(bResult);
}

LONG COpenGL::CPictureLoad::get_Height()
{
	LONG nHeight = 0;
	if (m_pPict != NULL)
	{
		m_pPict->get_Height(&nHeight);
	}
	return nHeight;
}
int COpenGL::CPictureLoad::GetHeight()
{
	return m_Height;
}
int COpenGL::CPictureLoad::GetWidth()
{
	return m_Width;
}
VOID COpenGL::CPictureLoad::CalculateWidthHeight()
{
	m_Height=get_Height()/m_PixelToMillimeterY;
	m_Width=get_Width()/m_PixelToMillimeterX;		
}
LONG COpenGL::CPictureLoad::get_Width()
{
	LONG nWidth = 0;
	if (m_pPict != NULL)
	{
		m_pPict->get_Width(&nWidth);
	}
	return nWidth;
}

VOID * COpenGL::CPictureLoad::GetTGAData(CString sFilePathName)
{
	ImageData *pImageData;
	WORD width = 0, height = 0;			// The dimensions of the image
	byte length = 0;					// The length in bytes to the pixels
	byte imageType = 0;					// The image type (RLE, RGB, Alpha...)
	byte bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
	int i = 0;							// A counter
	if((pFile = fopen(sFilePathName, "rb")) == NULL) 
	{
		return 0;
	}
	pImageData = new ImageData;
	fread(&length, sizeof(byte), 1, pFile);
	fseek(pFile,1,SEEK_CUR); 
	fread(&imageType, sizeof(byte), 1, pFile);
	// Skip past general information we don't care about
	fseek(pFile, 9, SEEK_CUR); 

	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(byte), 1, pFile);
	fseek(pFile, length + 1, SEEK_CUR); 

	int tr,tg,tb;
	if(m_bTransparence)
	{
		tr=GetRValue(m_iTransparence);
		tg=GetGValue(m_iTransparence);
		tb=GetBValue(m_iTransparence);
	}
	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = 4;
			stride = channels * width;
			pImageData->m_pData = new unsigned char[stride * height];

			for(int y = 0; y < height; y++)
			{
				unsigned char *pLine = &(pImageData->m_pData[stride * y]);

				fread(pLine, stride, 1, pFile);
			
				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for(i = 0; i < stride; i += channels)
				{
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
					pLine[i+3]=255;
					if(m_bTransparence)
						if(pLine[i]==tr&&pLine[i+1]==tg&&pLine[i+2]==tb)
							pLine[i+3]=0;						
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 4;
			stride = channels * width;
			pImageData->m_pData = new unsigned char[stride * height];

			for(int i = 0; i < width*height; i++)
			{
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.

				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x3f) << 2;
				r = ((pixels >> 11) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->m_pData[i * 3 + 0] = r;
				pImageData->m_pData[i * 3 + 1] = g;
				pImageData->m_pData[i * 3 + 2] = b;

				pImageData->m_pData[i*3+3]=255;
				if(m_bTransparence)
					if(pImageData->m_pData[i*3]==tr&&pImageData->m_pData[i*3+1]==tg&&pImageData->m_pData[i*3+2]==tb)
						pImageData->m_pData[i*3+3]=0;						
					
			}
		}	
		else
		{
			return false;
		}
	}
	else
	{
		// Create some variables to hold the rleID, current colors read, channels, & stride.
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;
		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->m_pData = new unsigned char[stride * height];
		byte *pColors = new byte [channels];

		while(i < width*height)
		{
			fread(&rleID, sizeof(byte), 1, pFile);
			
			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				rleID++;

				while(rleID)
				{
					fread(pColors, sizeof(byte) * channels, 1, pFile);

					// Store the current pixel in our image array
					pImageData->m_pData[colorsRead + 0] = pColors[2];
					pImageData->m_pData[colorsRead + 1] = pColors[1];
					pImageData->m_pData[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->m_pData[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				fread(pColors, sizeof(byte) * channels, 1, pFile);

				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->m_pData[colorsRead + 0] = pColors[2];
					pImageData->m_pData[colorsRead + 1] = pColors[1];
					pImageData->m_pData[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->m_pData[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
		delete[] pColors;
	}

	fclose(pFile);

	// Fill in our tImageTGA structure to pass back
	pImageData->m_nColor = channels;
	pImageData->m_Width  = width;
	pImageData->m_Height = height;
	return pImageData;
}
void * COpenGL::CPictureLoad::GetData(CString sFilePathName,int *color)
{
	if(!m_bInit)
		Init();	
	int tr,tg,tb;
	if(color==NULL)m_bTransparence=FALSE;
	else
	{
		m_bTransparence=TRUE;
		m_iTransparence=*color;
		tr=GetRValue(m_iTransparence);
		tg=GetGValue(m_iTransparence);
		tb=GetBValue(m_iTransparence);
	}
	sFilePathName.MakeLower();
	if(sFilePathName.Find(".jpg")==-1&&sFilePathName.Find(".gif")==-1&&sFilePathName.Find(".bmp")==-1)
	{
		if(sFilePathName.Find(".tga")==-1)
			return 0;
		else
		{
			return GetTGAData(sFilePathName);
		}
	}
	if(!Load(sFilePathName))
		return 0;
	ImageData *pImageData;
	pImageData = new ImageData;
	pImageData->m_Width=m_Width;
	pImageData->m_Height=m_Height;
	CRect rect(0,0,m_Width,m_Height);
	Render(&m_memDC,rect);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(CDC::FromHandle(m_hdc),m_Width,m_Height);
	CDC dc;
	dc.CreateCompatibleDC(CDC::FromHandle(m_hdc));
	dc.SelectObject(bitmap);
	dc.BitBlt(0,0,m_Width,m_Height,&m_memDC,0,0,SRCCOPY);
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	int nColor = bm.bmBitsPixel/8;
	unsigned char *data;
	data = new unsigned char[m_Width * m_Height * nColor];
	bitmap.GetBitmapBits(m_Width * m_Height * nColor, data);
	if(nColor==3||nColor==4)
	{
		pImageData->m_nColor=4;
		pImageData->m_pData = new unsigned char[m_Width * m_Height * 4];
		int stride = nColor*m_Width;
		memcpy(pImageData->m_pData,data,m_Width * m_Height * nColor);
		for(int y=0;y<m_Height;y++)
		{
			int y2=m_Height-y-1;
			unsigned char *pLine = &(pImageData->m_pData[stride * y]);
			unsigned char *pLine2 = &(data[stride * y2]);
			for(int i = 0; i < stride; i += nColor)
			{

				pLine[i]=pLine2[i+2];
				pLine[i+1]=pLine2[i+1];
				pLine[i+2]=pLine2[i];
				pLine[i+3]=255;
				if(m_bTransparence)
					if(pLine[i]==tr&&pLine[i+1]==tg&&pLine[i+2]==tb)
						pLine[i+3]=0;
			}
		}
	}
	else if(nColor==2)
	{
		pImageData->m_nColor=4;
		int r=0, g=0, b=0;
		unsigned short pixels = 0;
		pImageData->m_pData = new unsigned char[m_Width * m_Height * 4];
		int stride1 = pImageData->m_nColor*m_Width;
		int stride2 = 2*m_Width;
		for(int y=0;y<m_Height;y++)
		{
			int y2=m_Height-y-1;
			unsigned char *pLine = &(pImageData->m_pData[stride1 * y]);
			unsigned char *pLine2 = &(data[stride2 * y2]);
			for(int i = 0, j = 0; i < stride1; i += pImageData->m_nColor, j+= 2)
			{
				pixels=MAKEWORD(pLine2[j],pLine2[j+1]);
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x3f) << 2;
				r = ((pixels >> 11) & 0x1f) << 3;
				
				pLine[i]=r;
				pLine[i+1]=g;
				pLine[i+2]=b;
				pLine[i+3]=255;
				if(m_bTransparence)
					if(pLine[i]==tr&&pLine[i+1]==tg&&pLine[i+2]==tb)
						pLine[i+3]=0;
			}			
		}
	}
	delete data;
	return pImageData;
}

void COpenGL::CShape::DrawBall(float rad,int lon,int lat)
{
	if(lat%2==1)lat++;
	float x,y,z,s1=2*3.14159/lon,s2=3.14159/lat,temp,temp1,temp2,y1,y2;
	int i,j,k;
	for(k=-1;k<=1;k+=2)
	{
		//顶部
		glBegin(GL_TRIANGLE_FAN );
		glNormal3f(0,k,0);
		glVertex3f(0,k*rad,0);
		temp=rad*sin(s2);y=k*rad*cos(s2);
		for(i=0;i<lon;i++)
		{
			x=temp*cos(i*s1);
			z=temp*sin(i*s1);
			glNormal3f(x/rad,y/rad,z/rad);
			glVertex3f(x,y,z);
		}
		glNormal3f(temp/rad,y/rad,0);
		glVertex3f(temp,y,0);
		glEnd();
		//上半部
		for(i=1;i<lat/2-1;i++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			temp1=rad*sin(i*s2);
			temp2=rad*sin(i*s2+s2);
			y1=k*rad*cos(i*s2);
			y2=k*rad*cos(i*s2+s2);
			for(j=0;j<lon;j++)
			{
				x=temp1*cos(j*s1);
				z=temp1*sin(j*s1);
				glNormal3f(x/rad,y1/rad,z/rad);
				glVertex3f(x,y1,z);
				x=temp2*cos(j*s1);
				z=temp2*sin(j*s1);
				glNormal3f(x/rad,y2/rad,z/rad);
				glVertex3f(x,y2,z);
			}
			glNormal3f(temp1/rad,y1/rad,0);
			glVertex3f(temp1,y1,0);
			glNormal3f(temp2/rad,y2/rad,0);
			glVertex3f(temp2,y2,0);
			glEnd();			
		}
		//中部
		temp1=rad*sin(i*s2);
		temp2=rad;
		y1=k*rad*cos(i*s2);
		y2=0;
		glBegin(GL_TRIANGLE_STRIP);
		for(int j=0;j<lon;j++)
		{
			x=temp1*cos(j*s1);
			z=temp1*sin(j*s1);
			glNormal3f(x/rad,y1/rad,z/rad);
			glVertex3f(x,y1,z);
			x=temp2*cos(j*s1);
			z=temp2*sin(j*s1);
			glNormal3f(x/rad,y2/rad,z/rad);
			glVertex3f(x,y2,z);
		}
		glNormal3f(temp1/rad,y1/rad,0);
		glVertex3f(temp1,y1,0);
		glNormal3f(temp2/rad,y2/rad,0);
		glVertex3f(temp2,y2,0);
		glEnd();
	}
}

BOOL COpenGL::CalculateNormal(float v1[], float v2[], float v3[], float normal[])
{
	float vect1[3],vect2[3];
	for(int i=0;i<3;i++)
	{
		vect1[i] = v2[i]-v1[i];
		vect2[i] = v3[i]-v2[i];
	}
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];
	Normalize(normal);
	return TRUE;
}
BOOL COpenGL::CalculateNormal(double v1[], double v2[], double v3[], double normal[])
{
	double vect1[3],vect2[3];
	for(int i=0;i<3;i++)
	{
		vect1[i] = v2[i]-v1[i];
		vect2[i] = v3[i]-v2[i];
	}
	normal[0]=vect1[1]*vect2[2]-vect1[2]*vect2[1];
	normal[1]=vect1[2]*vect2[0]-vect1[0]*vect2[2];
	normal[2]=vect1[0]*vect2[1]-vect1[1]*vect2[0];
	Normalize(normal);
	return TRUE;
}

BOOL COpenGL::Normalize(float normal[])//计算标准化
{
	double nor;
	nor = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
	for(int i=0;i<3;i++)
		normal[i] /= nor;
	return TRUE;
}
BOOL COpenGL::Normalize(double normal[])//计算标准化
{
	double nor;
	nor = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
	for(int i=0;i<3;i++)
		normal[i] /= nor;
	return TRUE;
}

double COpenGL::InverseMatrix(float imat1[], float mat1[])
{
	double dimat[16],dmat[16];
	for(int i=0;i<16;i++)
		dmat[i]=mat1[i];
	double ret =InverseMatrix(dimat,dmat);
	for(i=0;i<16;i++)
		imat1[i]=dimat[i];
	return ret;
}
double COpenGL::InverseMatrix(double imat1[], double mat1[])
{
	double imat[16];
	int i,j,k;
	for(i=0;i<16;i++)imat[i]=mat1[i];
	int is[4];
	int js[4];
	double fDet = 1.0f,temp;
	int f = 1;
	
	for ( k = 0; k < 4; k ++)
	{
		// 第一步，全选主元
		double fMax = 0.0f;
		for ( i = k; i < 4; i ++)
		{
			for ( j = k; j < 4; j ++)
			{
				const double f = fabs(imat[j*4+i]);
				if (f > fMax)
				{
					fMax	= f;
					is[k]	= i;
					js[k]	= j;
				}
			}
		}
		if (fabs(fMax) < 0.00001f)
			return 0;
		
		if (is[k] != k)
		{
			f = -f;
			temp = imat[k];imat[k]=imat[is[k]];imat[is[k]]=temp;
			temp = imat[4+k];imat[4+k]=imat[4+is[k]];imat[4+is[k]]=temp;
			temp = imat[8+k];imat[8+k]=imat[8+is[k]];imat[8+is[k]]=temp;
			temp = imat[12+k];imat[12+k]=imat[12+is[k]];imat[12+is[k]]=temp;
		}
		if (js[k] != k)
		{
			f = -f;
			temp = imat[k*4];imat[k*4]=imat[js[k]*4];imat[js[k]*4]=temp;
			temp = imat[k*4+1];imat[k*4+1]=imat[js[k]*4+1];imat[js[k]*4+1]=temp;
			temp = imat[k*4+2];imat[k*4+2]=imat[js[k]*4+2];imat[js[k]*4+2]=temp;
			temp = imat[k*4+3];imat[k*4+3]=imat[js[k]*4+3];imat[js[k]*4+3]=temp;
		}
		
		// 计算行列值
		fDet *= imat[k*4+k];
		
		// 计算逆矩阵
		
		// 第二步
		imat[k*4+k] = 1.0/imat[k*4+k];
		// 第三步
		for ( j = 0; j < 4; j ++)
		{
			if (j != k)
				imat[j*4+k]*=imat[k*4+k];
			//m(k, j) *= m(k, k);
		}
		// 第四步
		for ( i = 0; i < 4; i ++)
		{
			if (i != k)
			{
				for	(j = 0; j < 4; j ++)
				{
					if (j != k)
						imat[j*4+i] -= imat[k*4+i]*imat[j*4+k];
					//m(i, j) = m(i, j) - m(i, k) * m(k, j);
				}
			}
		}
		// 第五步
		for (i = 0; i < 4; i ++)
		{
			if (i != k)
				imat[k*4+i] *= -imat[k*4+k];
			//m(i, k) *= -m(k, k);
		}
	}
	
	for	(k = 3; k >= 0; k --)
	{
		if (js[k] != k)
		{
			temp = imat[k];imat[k]=imat[js[k]];imat[js[k]]=temp;
			temp = imat[4+k];imat[4+k]=imat[4+js[k]];imat[4+js[k]]=temp;
			temp = imat[8+k];imat[8+k]=imat[8+js[k]];imat[8+js[k]]=temp;
			temp = imat[12+k];imat[12+k]=imat[12+js[k]];imat[12+js[k]]=temp;
		}
		if (is[k] != k)
		{
			temp = imat[k*4];imat[k*4]=imat[is[k]*4];imat[is[k]*4]=temp;
			temp = imat[1+k*4];imat[1+k*4]=imat[1+is[k]*4];imat[1+is[k]*4]=temp;
			temp = imat[2+k*4];imat[2+k*4]=imat[2+is[k]*4];imat[2+is[k]*4]=temp;
			temp = imat[3+k*4];imat[3+k*4]=imat[3+is[k]*4];imat[3+is[k]*4]=temp;
		}
	}
	for(i=0;i<16;i++)
		imat1[i] = imat[i];
	return fDet * f;
}

int COpenGL::CreateScene(int n)
{
	CScene *s;
	for(int i=0;i<n;i++)
	{
		s= new CScene();
		scene.push_back(s);
	}
	return scene.size()-1;
}
int COpenGL::GetSceneNum()
{
	return scene.size();
}
COpenGL::CScene * COpenGL::GetScene(int n)
{
	CScene *pScene;
	pScene = scene[n];
	return pScene;
}
BOOL COpenGL::DrawScene(int &numTri,int start,int end)
{
	int n=scene.size();
	int endpos = end>=start?end:n-1;
	for(int i=start;i<=endpos;i++)
	{
		glPushName(i);
		scene[i]->Draw(numTri);
	}
	return TRUE;
}


void COpenGL::ProcessSelect(int xPos, int yPos)
{
#define bufferlength 64
	UINT selectBuffer[bufferlength];
	int hits,viewport[4];
	glSelectBuffer(bufferlength,selectBuffer);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	gluPickMatrix(xPos,viewport[3]-yPos,2,2,viewport);
	gluPerspective(m_PerspectiveParam[0],m_PerspectiveParam[1],m_PerspectiveParam[2],m_PerspectiveParam[3]);
	glMatrixMode(GL_MODELVIEW);
	OnPaint();
	hits=glRenderMode(GL_RENDER);
	//处理

	//恢复
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}













