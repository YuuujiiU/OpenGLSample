// GLWorkSpaceDoc.h : interface of the CGLWorkSpaceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWORKSPACEDOC_H__0AE9E6A5_4DF5_44FC_A4AA_A1D15BA15673__INCLUDED_)
#define AFX_GLWORKSPACEDOC_H__0AE9E6A5_4DF5_44FC_A4AA_A1D15BA15673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGLWorkSpaceDoc : public CDocument
{
protected: // create from serialization only
	CGLWorkSpaceDoc();
	DECLARE_DYNCREATE(CGLWorkSpaceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLWorkSpaceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLWorkSpaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLWorkSpaceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLWORKSPACEDOC_H__0AE9E6A5_4DF5_44FC_A4AA_A1D15BA15673__INCLUDED_)
