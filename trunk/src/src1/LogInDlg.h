#if !defined(AFX_LOGINDLG_H__8BDFD9E8_C5D7_4FA4_864F_433801484302__INCLUDED_)
#define AFX_LOGINDLG_H__8BDFD9E8_C5D7_4FA4_864F_433801484302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogInDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogInDlg dialog
#define USER_MSG_AFTERCONNECTDATABASE (WM_USER+103)
#define USER_MSG_SETBUTTONSTATE       (WM_USER+104)
struct ConnectInfoThd{
	HWND hwnd;
	CString m_password;
	CString m_strServerName;
	CString m_strUsername;
};

class CLogInDlg : public CDialog
{
// Construction
public:
	LRESULT AfterConnectDatabase(WPARAM w,LPARAM l);
	const char* GetIP();
	CLogInDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CLogInDlg)
	enum { IDD = IDD_SYSTEM_LOGIN };
	CComboBox	m_cmbServerName;
	CString	m_strServerName;
	CString	m_password;
	CString	m_strUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogInDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogInDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonRegster();
	afx_msg void OnDropdownComboUsername();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LRESULT SetButtonState(WPARAM w,LPARAM l);
	CWinThread* m_pThread;
	CWinThread* m_pRegisterThread;

	afx_msg void OnCbnSelchangeComboServerName();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__8BDFD9E8_C5D7_4FA4_864F_433801484302__INCLUDED_)
