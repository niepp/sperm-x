#if !defined(AFX_DATALISTCTRL_H__46C42311_C88E_439A_A9D1_343B53E58649__INCLUDED_)
#define AFX_DATALISTCTRL_H__46C42311_C88E_439A_A9D1_343B53E58649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl window

class CDataListCtrl : public CListCtrl
{
// Construction
public:
	CDataListCtrl();
// Attributes
public:
	int SetData(const _RecordsetPtr &rs);
	int SetData(const CString &strSQL);
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDataListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALISTCTRL_H__46C42311_C88E_439A_A9D1_343B53E58649__INCLUDED_)
