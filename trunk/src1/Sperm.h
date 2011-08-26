// Sperm.h : main header file for the SPERM application
//

#if !defined(AFX_SPERM_H__96B36626_422A_4410_A98A_03F02202B587__INCLUDED_)
#define AFX_SPERM_H__96B36626_422A_4410_A98A_03F02202B587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Sperm_i.h"

/////////////////////////////////////////////////////////////////////////////
// CSpermApp:
// See Sperm.cpp for the implementation of this class
//

class CSpermApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSpermApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpermApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSpermApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LRESULT AfterLogin(WPARAM w,LPARAM l);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPERM_H__96B36626_422A_4410_A98A_03F02202B587__INCLUDED_)
