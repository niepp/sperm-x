#if !defined(AFX_FLUORERESULTPROPPAGE_H__C663F3E5_FD7D_4BB6_96E9_51DE76CA2037__INCLUDED_)
#define AFX_FLUORERESULTPROPPAGE_H__C663F3E5_FD7D_4BB6_96E9_51DE76CA2037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FluoreResultPropPage.h : header file
//
#include "FluoreListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CFluoreResultPropPage dialog

class CFluoreResultPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFluoreResultPropPage)

// Construction
public:
	CFluoreResultPropPage();
	~CFluoreResultPropPage();

// Dialog Data
	//{{AFX_DATA(CFluoreResultPropPage)
	enum { IDD = IDD_FLUORE_RESULT_PROPPAGE };
	CFluoreListCtrl	m_FluoreResultList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFluoreResultPropPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFluoreResultPropPage)
	afx_msg void OnClickListFluoreResult(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nFluoreResultListIndex;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUORERESULTPROPPAGE_H__C663F3E5_FD7D_4BB6_96E9_51DE76CA2037__INCLUDED_)
