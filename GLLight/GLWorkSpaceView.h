// GLWorkSpaceView.h : interface of the CGLWorkSpaceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWORKSPACEVIEW_H__9E7E8D3C_AAC0_483D_8060_50F05FA6DD08__INCLUDED_)
#define AFX_GLWORKSPACEVIEW_H__9E7E8D3C_AAC0_483D_8060_50F05FA6DD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "myOpengl.h"
//class CmyOpenGL;
class CGLWorkSpaceDoc;
class CGLWorkSpaceView : public CView
{
protected: // create from serialization only
	CGLWorkSpaceView();
	DECLARE_DYNCREATE(CGLWorkSpaceView)

// Attributes
public:
	CGLWorkSpaceDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLWorkSpaceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLWorkSpaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLWorkSpaceView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLightMovinglight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CmyOpenGL m_gl;
};

#ifndef _DEBUG  // debug version in GLWorkSpaceView.cpp
inline CGLWorkSpaceDoc* CGLWorkSpaceView::GetDocument()
   { return (CGLWorkSpaceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLWORKSPACEVIEW_H__9E7E8D3C_AAC0_483D_8060_50F05FA6DD08__INCLUDED_)
