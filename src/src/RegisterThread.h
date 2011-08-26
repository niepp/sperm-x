#if !defined(AFX_REGISTERTHREAD_H__46FB4FBE_5FB4_464E_8DB0_5C55E6C82C5B__INCLUDED_)
#define AFX_REGISTERTHREAD_H__46FB4FBE_5FB4_464E_8DB0_5C55E6C82C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterThread.h : header file
//

#include "RegesterDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRegisterThread thread

#define  USER_THDMSG_REGISTER (WM_USER+107)

class CRegisterThread : public CWinThread
{
	DECLARE_DYNCREATE(CRegisterThread)
protected:
	CRegisterThread();           // protected constructor used by dynamic creation

// Attributes
public:
	void Register(WPARAM w,LPARAM l);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegisterThread();

	// Generated message map functions
	//{{AFX_MSG(CRegisterThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CRegesterDlg m_regdlg;
	_ConnectionPtr m_Connection;	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERTHREAD_H__46FB4FBE_5FB4_464E_8DB0_5C55E6C82C5B__INCLUDED_)
