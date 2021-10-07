// GLWorkSpaceDoc.cpp : implementation of the CGLWorkSpaceDoc class
//

#include "stdafx.h"
#include "GLWorkSpace.h"

#include "GLWorkSpaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceDoc

IMPLEMENT_DYNCREATE(CGLWorkSpaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CGLWorkSpaceDoc, CDocument)
	//{{AFX_MSG_MAP(CGLWorkSpaceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceDoc construction/destruction

CGLWorkSpaceDoc::CGLWorkSpaceDoc()
{
	// TODO: add one-time construction code here

}

CGLWorkSpaceDoc::~CGLWorkSpaceDoc()
{
}

BOOL CGLWorkSpaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceDoc serialization

void CGLWorkSpaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceDoc diagnostics

#ifdef _DEBUG
void CGLWorkSpaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGLWorkSpaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLWorkSpaceDoc commands
