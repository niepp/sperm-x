#if !defined(AFX_SYSREG_H__18112779_A5A4_4328_A835_97022A80ADEE__INCLUDED_)
#define AFX_SYSREG_H__18112779_A5A4_4328_A835_97022A80ADEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SysReg dialog

class SysReg : public CDialog
{
// Construction
public:
	SysReg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SysReg)
	enum { IDD = IDD_SYSTEM_REGISTER };
	CString	m_sn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SysReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SysReg)
	afx_msg void OnButtonRegster();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSREG_H__18112779_A5A4_4328_A835_97022A80ADEE__INCLUDED_)
