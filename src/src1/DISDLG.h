#if !defined(AFX_DISDLG_H__BCDA2576_28D9_4DB4_BECC_F8A190047DAC__INCLUDED_)
#define AFX_DISDLG_H__BCDA2576_28D9_4DB4_BECC_F8A190047DAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DISDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDISDLG dialog

class CDISDLG : public CDialog
{
// Construction
public:
	CDISDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDISDLG)
	enum { IDD = IDD_DIALOG_DISPLAY };
	int		m_playtype;
	int		m_framefrequence;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDISDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDISDLG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISDLG_H__BCDA2576_28D9_4DB4_BECC_F8A190047DAC__INCLUDED_)
