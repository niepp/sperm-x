// SpermDoc.h : interface of the CSpermDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPERMDOC_H__25526B18_AF21_4F80_8779_4AF8B036F566__INCLUDED_)
#define AFX_SPERMDOC_H__25526B18_AF21_4F80_8779_4AF8B036F566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSpermDoc : public CDocument
{
protected: // create from serialization only
	CSpermDoc();
	DECLARE_DYNCREATE(CSpermDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpermDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Write(CFile *pFile);
	virtual ~CSpermDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpermDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPERMDOC_H__25526B18_AF21_4F80_8779_4AF8B036F566__INCLUDED_)
