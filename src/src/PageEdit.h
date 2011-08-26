#if !defined(AFX_PAGEEDIT_H__7781DF71_0B7A_4736_B480_D98BE995CB9D__INCLUDED_)
#define AFX_PAGEEDIT_H__7781DF71_0B7A_4736_B480_D98BE995CB9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageEdit window

class CPageEdit : public CEdit
{
// Construction
public:
	CPageEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageEdit)
	virtual   void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEEDIT_H__7781DF71_0B7A_4736_B480_D98BE995CB9D__INCLUDED_)
