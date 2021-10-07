// myOpenGL.h: interface for the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOPENGL_H__9B9629AF_478E_4E10_AAC1_17EC21F7EFB5__INCLUDED_)
#define AFX_MYOPENGL_H__9B9629AF_478E_4E10_AAC1_17EC21F7EFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glaux.lib" )	

#include "OpenGL.h"

class CmyOpenGL : public COpenGL  
{
public:
	virtual BOOL InDraw(void);
	virtual BOOL PostInit(void);
	CmyOpenGL();
	virtual ~CmyOpenGL();
	//Draw model
	void InitQuadric();
	void CALLBACK errorCallback(GLenum errorCode);
	void DrawSphere(float pos[], float radius);

	void DrawTriangle();
	void LightMove();
	void LightMotionStep();//CALLBACK
	void SetLight1();
	void SetLight2();
	void SetLight3();
	void SetLight4();

	void InitList();
	void TestList();
	
public:
	GLUquadricObj *m_qobj;
	float m_fStep;
	int m_iExNum;
	UINT m_iListNum;

};

#endif // !defined(AFX_MYOPENGL_H__9B9629AF_478E_4E10_AAC1_17EC21F7EFB5__INCLUDED_)
