#if !defined(AFX_USERMANAGEDLG_H__EE62624B_21F2_471A_99E6_E0D7742A36CB__INCLUDED_)
#define AFX_USERMANAGEDLG_H__EE62624B_21F2_471A_99E6_E0D7742A36CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserManageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg dialog

class CUserManageDlg : public CDialog
{
// Construction
public:
	CUserManageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserManageDlg)
	enum { IDD = IDD_DIALOG_USERMANAGE };
	CListBox	m_wndUserList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserManageDlg)
	afx_msg void OnButtonUseradd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUserdelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERMANAGEDLG_H__EE62624B_21F2_471A_99E6_E0D7742A36CB__INCLUDED_)
