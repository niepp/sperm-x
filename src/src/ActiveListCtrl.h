#if !defined(AFX_ACTIVELISTCTRL_H__D3256B6B_F12A_4E32_8336_3BAD2B169BA2__INCLUDED_)
#define AFX_ACTIVELISTCTRL_H__D3256B6B_F12A_4E32_8336_3BAD2B169BA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActiveListCtrl window

class CActiveListCtrl : public CListCtrl
{
// Construction
public:
	CActiveListCtrl();

// Attributes
public:
	
// Operations
public:
   	int SetListCtrlData(const _RecordsetPtr &rs);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CActiveListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CActiveListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVELISTCTRL_H__D3256B6B_F12A_4E32_8336_3BAD2B169BA2__INCLUDED_)
