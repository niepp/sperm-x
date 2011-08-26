#if !defined(AFX_MOPHYRESULTPROPPAGE_H__BEB2BCA4_80C3_4B63_A7A4_BA8ABD1B5C66__INCLUDED_)
#define AFX_MOPHYRESULTPROPPAGE_H__BEB2BCA4_80C3_4B63_A7A4_BA8ABD1B5C66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MophyResultPropPage.h : header file
//
#include "MophyListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMophyResultPropPage dialog

class CMophyResultPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMophyResultPropPage)

// Construction
public:
	CMophyResultPropPage();
	~CMophyResultPropPage();

// Dialog Data
	//{{AFX_DATA(CMophyResultPropPage)
	enum { IDD = IDD_MOPHY_RESULT_PROPPAGE };
	CMophyListCtrl	m_MophyResultList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMophyResultPropPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMophyResultPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListMophyResult(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nMophyResultListIndex;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOPHYRESULTPROPPAGE_H__BEB2BCA4_80C3_4B63_A7A4_BA8ABD1B5C66__INCLUDED_)
