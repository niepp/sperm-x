#if !defined(AFX_BROWERDLG_H__652C6058_DE0C_4532_B249_6199139C507A__INCLUDED_)
#define AFX_BROWERDLG_H__652C6058_DE0C_4532_B249_6199139C507A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrowerDlg dialog

class CBrowerDlg : public CDialog
{
// Construction
public:
	CBrowerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrowerDlg)
	enum { IDD = IDD_DIALOG_BROWSER };
	int		m_framechoose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrowerDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWERDLG_H__652C6058_DE0C_4532_B249_6199139C507A__INCLUDED_)
