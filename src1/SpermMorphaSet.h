#if !defined(AFX_SPERMMORPHASET_H__779D716B_ECD7_4429_8C8B_911F2AF8F222__INCLUDED_)
#define AFX_SPERMMORPHASET_H__779D716B_ECD7_4429_8C8B_911F2AF8F222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpermMorphaSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpermMorphaSet dialog

class CSpermMorphaSet : public CDialog
{
// Construction
public:
	CSpermMorphaSet(int index=-1,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpermMorphaSet)
	enum { IDD = IDD_DIALOG_MORPHASET };
	int		m_nMorphaHead;
	int		m_nMorphaPerfor;
	int		m_nMorphaSmallPoint;
	int		m_nMorphaTail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpermMorphaSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpermMorphaSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nSpermIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPERMMORPHASET_H__779D716B_ECD7_4429_8C8B_911F2AF8F222__INCLUDED_)
