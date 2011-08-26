#if !defined(AFX_LOGINTHREAD_H__0BDA2A8F_D5ED_485E_B8A4_EE0823E97C17__INCLUDED_)
#define AFX_LOGINTHREAD_H__0BDA2A8F_D5ED_485E_B8A4_EE0823E97C17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginThread.h : header file
//

#include "afxmt.h"
#define WM_THDMSG_GETLOGININFO  (WM_USER + 105)
#define WM_THDMSG_EXITTHD       (WM_USER + 106)
/////////////////////////////////////////////////////////////////////////////
// CLoginThread thread
#include "LogInDlg.h"

class CLoginThread : public CWinThread
{
	DECLARE_DYNCREATE(CLoginThread)
protected:
	CLoginThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void GetLoginInfo(WPARAM w,LPARAM l);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLoginThread();

	// Generated message map functions
	//{{AFX_MSG(CLoginThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	ConnectInfoThd* m_ThdInfo;
	_ConnectionPtr m_Connection;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINTHREAD_H__0BDA2A8F_D5ED_485E_B8A4_EE0823E97C17__INCLUDED_)
