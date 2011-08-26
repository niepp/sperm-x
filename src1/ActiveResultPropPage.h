#if !defined(AFX_ACTIVERESULTPROPPAGE_H__5305EE9F_F9C4_4BF1_B4CE_DB4D06121EEA__INCLUDED_)
#define AFX_ACTIVERESULTPROPPAGE_H__5305EE9F_F9C4_4BF1_B4CE_DB4D06121EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveResultPropPage.h : header file
//


#include "ActiveListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CActiveResultPropPage dialog

class CActiveResultPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CActiveResultPropPage)

// Construction
public:
	CActiveResultPropPage();
	~CActiveResultPropPage();

// Dialog Data
	//{{AFX_DATA(CActiveResultPropPage)
	enum { IDD = IDD_ACTIVE_RESULT_PROPPAGE };
	CActiveListCtrl	m_ActiveResultList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CActiveResultPropPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CActiveResultPropPage)
	afx_msg void OnClickListActiveResult(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	int m_nActiveResultListIndex;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVERESULTPROPPAGE_H__5305EE9F_F9C4_4BF1_B4CE_DB4D06121EEA__INCLUDED_)
