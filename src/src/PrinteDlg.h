//{{AFX_INCLUDES()
#include "crystalreportviewer10.h"
//}}AFX_INCLUDES
#if !defined(AFX_PRINTEDLG_H__7A41F1AA_967B_4372_BED9_A14FFE0A3EEA__INCLUDED_)
#define AFX_PRINTEDLG_H__7A41F1AA_967B_4372_BED9_A14FFE0A3EEA__INCLUDED_
#include "StdAfx.h"
#include "sperm.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrinteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrinteDlg dialog
typedef struct crxconnparm
{
	CString ipadd;
	CString database;
	CString username;
	CString passwd;
} crxparm;

enum ReportType{ COMBIN=0, MOPHY, COMPARE, ACTIVESIMPLE, ACTIVE };

class CPrinteDlg : public CDialog
{
// Construction
public:
	void HandleError(_com_error &e);
	int m_reporttype;
	BOOL checktypevalid(int reporttype);
	CPrinteDlg(int reporttype, crxparm parm, CString wParam=_T(""), CString lParam=_T(""), CWnd *pParent=NULL);

// Dialog Data
	//{{AFX_DATA(CPrinteDlg)
	enum { IDD = IDD_PTNDLG };
	CCrystalReportViewer10	m_Viewer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	IReportPtr m_Report;
	VARIANT dummy;
	crxparm m_parm;
	CString m_wParam; // 连接参数1
	CString m_lParam; // 连接参数2
	BOOL iscrxready;

	// Generated message map functions
	//{{AFX_MSG(CPrinteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTEDLG_H__7A41F1AA_967B_4372_BED9_A14FFE0A3EEA__INCLUDED_)
