//{{AFX_INCLUDES()
#include "datagrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_LIVENESSRESULTPAGE_H__17E8FC36_ED35_4E55_80D5_F7BF8D104822__INCLUDED_)
#define AFX_LIVENESSRESULTPAGE_H__17E8FC36_ED35_4E55_80D5_F7BF8D104822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LivenessResultPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLivenessResultPage dialog
#include "resource.h"
#include "DataListCtrl.h"

class CLivenessResultPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLivenessResultPage)

// Construction
public:
	void GetRecordSet(_RecordsetPtr &rs, int row1, int row2);
	int  FindCol(_RecordsetPtr &rs,const _variant_t& colName);
	CLivenessResultPage();
	~CLivenessResultPage();

// Dialog Data
	//{{AFX_DATA(CLivenessResultPage)
	enum { IDD = IDD_DIALOG_LIVENESSRESULT };
	CDataListCtrl	m_wndLiveList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLivenessResultPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLivenessResultPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListdata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIVENESSRESULTPAGE_H__17E8FC36_ED35_4E55_80D5_F7BF8D104822__INCLUDED_)
