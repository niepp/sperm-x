#if !defined(AFX_MOPHYLISTCTRL_H__F4A89B3A_5D2F_4084_ADD6_2B08DCE7AC72__INCLUDED_)
#define AFX_MOPHYLISTCTRL_H__F4A89B3A_5D2F_4084_ADD6_2B08DCE7AC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MophyListCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMophyListCtrl window

class CMophyListCtrl : public CListCtrl
{
// Construction
public:
	CMophyListCtrl();

// Attributes
public:
	int SetListCtrlData(const _RecordsetPtr &rs);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMophyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMophyListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMophyListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOPHYLISTCTRL_H__F4A89B3A_5D2F_4084_ADD6_2B08DCE7AC72__INCLUDED_)
