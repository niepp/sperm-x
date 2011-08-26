#if !defined(AFX_PATIENTINFOQUERYDLG_H__3C8294AC_A792_440C_BE59_A4A846F4A1CB__INCLUDED_)
#define AFX_PATIENTINFOQUERYDLG_H__3C8294AC_A792_440C_BE59_A4A846F4A1CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatientInfoQueryDlg.h : header file
//
#include "DataListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CPatientInfoQueryDlg dialog

class CPatientInfoQueryDlg : public CDialog
{
// Construction
public:
	CPatientInfoQueryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatientInfoQueryDlg)
	enum { IDD = IDD_PATIENT_INFO_QUERY };
	CDataListCtrl	m_PatientInfoListCtr;
	BOOL	m_bChkName;
	BOOL	m_bChkCaseNO;
	CString	m_strInputCaseNO;
	CString	m_strInputName;
	//}}AFX_DATA

public:
	void OnQuery();
	int FindCol(_RecordsetPtr &rs,const _variant_t& colName);
	UINT mSelItemIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatientInfoQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	// Generated message map functions
	//{{AFX_MSG(CPatientInfoQueryDlg)
	afx_msg void OnBtnSelOk();
	afx_msg void OnChkName();
	afx_msg void OnChkCaseno();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnQuery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATIENTINFOQUERYDLG_H__3C8294AC_A792_440C_BE59_A4A846F4A1CB__INCLUDED_)
