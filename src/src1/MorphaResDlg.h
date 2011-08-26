#if !defined(AFX_MORPHARESDLG_H__B5381599_DB00_41E9_ABF7_D5DEE73EC5E7__INCLUDED_)
#define AFX_MORPHARESDLG_H__B5381599_DB00_41E9_ABF7_D5DEE73EC5E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaResDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMorphaResDlg dialog
#include "datalistctrl.h"

class CMorphaResDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CMorphaResDlg)

// Construction
public:
	void GetRecordSet(_RecordsetPtr &rs, int row1, int row2);
	CMorphaResDlg();
	~CMorphaResDlg();

// Dialog Data
	//{{AFX_DATA(CMorphaResDlg)
	enum { IDD = IDD_DIALOG_MORPHARESULT };
	CDataListCtrl	m_wndMorphaDataList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMorphaResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMorphaResDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListMorphadata(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHARESDLG_H__B5381599_DB00_41E9_ABF7_D5DEE73EC5E7__INCLUDED_)
