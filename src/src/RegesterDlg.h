#if !defined(AFX_REGESTERDLG_H__ECA8EEF0_A0F4_4E47_B672_D2E5404BFF9A__INCLUDED_)
#define AFX_REGESTERDLG_H__ECA8EEF0_A0F4_4E47_B672_D2E5404BFF9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegesterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegesterDlg dialog

class CRegesterDlg : public CDialog
{
// Construction
public:
	CRegesterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegesterDlg)
	enum { IDD = IDD_DIALOG_REGESTER };
	CString	m_strPass1;
	CString	m_strPass2;
	CString	m_strUsername;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegesterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegesterDlg)
	virtual void OnOK();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool IsUsernameIllegal(const char* username);
	_ConnectionPtr m_pConnection;
	CString m_strServerName;
	HWND m_hParentWnd;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGESTERDLG_H__ECA8EEF0_A0F4_4E47_B672_D2E5404BFF9A__INCLUDED_)
