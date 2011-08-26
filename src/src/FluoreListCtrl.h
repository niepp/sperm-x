#if !defined(AFX_FLUORELISTCTRL_H__F97D229E_691A_4E7C_969D_B6ABB0415CC5__INCLUDED_)
#define AFX_FLUORELISTCTRL_H__F97D229E_691A_4E7C_969D_B6ABB0415CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FluoreListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFluoreListCtrl window
class CFluoreListCtrl : public CListCtrl
{
// Construction
public:
	CFluoreListCtrl();

// Attributes
public:
	int SetListCtrlData(const _RecordsetPtr &rs);
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluoreListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFluoreListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFluoreListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUORELISTCTRL_H__F97D229E_691A_4E7C_969D_B6ABB0415CC5__INCLUDED_)
