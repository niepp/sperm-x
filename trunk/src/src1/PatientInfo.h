#if !defined(AFX_PATIENTINFO_H__56BDB8B7_356D_420E_A807_8CF767E84EB5__INCLUDED_)
#define AFX_PATIENTINFO_H__56BDB8B7_356D_420E_A807_8CF767E84EB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatientInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatientInfo dialog
class CPatientInfo : public CDialog
{
// Construction
public:
	CPatientInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatientInfo)
	enum { IDD = IDD_PATIENT };
	CComboBox	m_wndCbxSmell;
	
	CEdit	m_wndEditName;
	CEdit	m_wndEditDilutionRatio;
	CEdit	m_wndEditDetectNO;
	CEdit	m_wndEdigCaseNO;

	CComboBox	m_wndCbxAge;
	CComboBox	m_wndCbxThickness;
	CComboBox	m_wndCbxSpermWay;
	CComboBox	m_wndCbxShape;
	CComboBox	m_wndCbxRoomTempera;
	CComboBox	m_wndCbxPH;
	CComboBox	m_wndCbxLiquifyTime;
	CComboBox	m_wndCbxCohension;
	CComboBox	m_wndCbxAbstinency;
	CComboBox	m_wndCbxSpermVolume;
	CComboBox	m_wndCbxLiquify;    //
	CComboBox   m_wndCbxSendDoctor;
	CComboBox   m_wndCbxDetectDoctor;

	CString	m_strColor;
	CString	m_strMemory;
	CString	m_strLiquifyState;
	CString	m_strDetectNO;
	CString	m_strName;
	CString	m_strCaseNO;
	COleDateTime	m_oleSpermGetDate;
	COleDateTime	m_oleSpermGetTime;
	CString	m_strSendDoctor;
	CString	m_strDetectDoctor;
	CString	m_strSampleNO;
	COleDateTime	m_oleSpermDetectTime;
	COleDateTime	m_oleSpermDetectDate;
	CString	m_strSperVolume;
	CString	m_strAbstinency;
	CString	m_strAge;
	CString	m_strCohension;
	CString	m_strLiquifyTime;
	CString	m_strPH;
	CString	m_strRoomTempera;
	CString	m_strShape;
	CString	m_strSmell;
	CString	m_strSpermWay;
	CString	m_strThickness;
	CString	m_strDilutionRatio;
	//}}AFX_DATA

	CString m_strNameFromDataBase;
	void ReadDataRecordToCombox();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatientInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatientInfo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnAddDoctor();
	afx_msg void OnChangeEditCaseno();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATIENTINFO_H__56BDB8B7_356D_420E_A807_8CF767E84EB5__INCLUDED_)
