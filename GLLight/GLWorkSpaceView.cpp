// GLWorkSpaceView.cpp : implementation of the CGLWorkSpaceView class
//

#include "stdafx.h"
#include "GLWorkSpace.h"

#include "GLWorkSpaceDoc.h"
#include "GLWorkSpaceView.h"
#include "myOpengl.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView

IMPLEMENT_DYNCREATE(CGLWorkSpaceView, CView)

BEGIN_MESSAGE_MAP(CGLWorkSpaceView, CView)
	//{{AFX_MSG_MAP(CGLWorkSpaceView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(ID_LIGHT_MOVINGLIGHT, OnLightMovinglight)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView construction/destruction

CGLWorkSpaceView::CGLWorkSpaceView()
{
	// TODO: add construction code here

}

CGLWorkSpaceView::~CGLWorkSpaceView()
{
}

BOOL CGLWorkSpaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView drawing

void CGLWorkSpaceView::OnDraw(CDC* pDC)
{
	CGLWorkSpaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	m_gl.OnPaint();
}

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView printing

BOOL CGLWorkSpaceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLWorkSpaceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLWorkSpaceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView diagnostics

#ifdef _DEBUG
void CGLWorkSpaceView::AssertValid() const
{
	CView::AssertValid();
}

void CGLWorkSpaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLWorkSpaceDoc* CGLWorkSpaceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLWorkSpaceDoc)));
	return (CGLWorkSpaceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceView message handlers

int CGLWorkSpaceView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if(!m_gl.Init(this->GetSafeHwnd()))
		AfxMessageBox("error!");

	SetTimer(1, 10, NULL);
	return 0;
}

void CGLWorkSpaceView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_gl.OnSize(cx,cy);
}

BOOL CGLWorkSpaceView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

BOOL CGLWorkSpaceView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP ||pMsg->message == WM_RBUTTONDOWN ||pMsg->message == WM_RBUTTONUP ||pMsg->message == WM_LBUTTONDBLCLK ||pMsg->message == WM_RBUTTONDBLCLK||pMsg->message==WM_MOUSEMOVE||pMsg->message==WM_MOUSEWHEEL)
		m_gl.OnMouse(pMsg->message,pMsg->wParam,pMsg->lParam);
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP || pMsg->message == WM_CHAR)
		m_gl.OnKey(pMsg->message,pMsg->wParam);
	
	return CView::PreTranslateMessage(pMsg);
}

void CGLWorkSpaceView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus = &pFrame->m_wndStatusBar;
	m_gl.SetStatus(pStatus);
}

void CGLWorkSpaceView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_gl.LightMotionStep();
	m_gl.OnPaint();

	CView::OnTimer(nIDEvent);
}

void CGLWorkSpaceView::OnLightMovinglight() 
{
	// TODO: Add your command handler code here
	m_gl.m_iExNum=1;
	m_gl.OnPaint();
	
}
