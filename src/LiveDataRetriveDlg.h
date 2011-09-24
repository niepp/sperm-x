#if !defined(AFX_LIVEDATARETRIVEDLG_H__82E00AAE_42F6_4483_AACE_6DEFA2AB47C2__INCLUDED_)
#define AFX_LIVEDATARETRIVEDLG_H__82E00AAE_42F6_4483_AACE_6DEFA2AB47C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LiveDataRetriveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiveDataRetriveDlg dialog

#include "DataListCtrl.h"
#include "LivenessResultPage.h"
#include "LiveImagePage.h"
#include "PageEdit.h"
#include "resource.h"
#include "./print/LivePrintFrame.h"
class CLiveDataRetriveDlg : public CDialog
{
// Construction
struct  DividePage
{
	int nTotalRecord;
	int nPageRecord;
	int nCurPage;
	int nTotalPage;

};

public:
	CLiveDataRetriveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLiveDataRetriveDlg)
	enum { IDD = IDD_DIALOG_LIVE_RETRIVE };
	CPageEdit	m_wndPageEdit;
	CPageEdit	m_wndInputDetectNO;
	CButton	m_wndPrePage;
	CButton	m_wndNextPage;
	CButton	m_wndLastPage;
	CButton	m_wndFirstPage;
	CString	m_strEditName;
	CString	m_strEditDetectNO;
	COleDateTime	m_oledateDetectDate;
	COleDateTime	m_oletimeDetectTime;
	COleDateTime	m_oledateSpermGetDate;
	COleDateTime	m_oletimeSpermGetTime;
	CString	m_strEditCaseNO;
	CString	m_strEditDilutionRatio;
	CString	m_strEditLiquifyState;
	CString	m_strEditSampleNO;
	CString	m_strEditSpermColor;
	COleDateTime	m_oletimeTO;
	COleDateTime	m_oletimeFROM;
	CString	m_strEditInputDetectNO;
	CString	m_strEditInputName;
	CString	m_strEditOtherValue;
	CString	m_strEditSmell;
	CString	m_strEditSpermVolume;
	CString	m_strEditCohension;
	CString	m_strEditAbstinency;
	CString	m_strEditAge;
	CString	m_strEditLiquifyTime;
	CString	m_strEditPH;
	CString	m_strEditRoomTempera;
	CString	m_strEditShape;
	CString	m_strEditSpermWay;
	CString	m_strEditThickness;
	BOOL	m_BChkDetectDate;
	BOOL	m_BChkDetectNO;
	BOOL	m_BChkName;
	BOOL	m_BChkOther;
	CString	m_strCmBOtherSelection;
	CString	m_strPageNum;
	int		m_pttype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLiveDataRetriveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CLiveDataRetriveDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonLiveinquery();
	afx_msg void OnBtnFirstpage();
	afx_msg void OnBtnPrepage();
	afx_msg void OnBtnNextpage();
	afx_msg void OnBtnLastpage();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillfocusEditInputdetectno();
	afx_msg void OnKillfocusEditInputname();
	afx_msg void OnKillfocusDatetimepickerInputFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusDatetimepickerInputTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkDetectdate();
	afx_msg void OnChkDetectno();
	afx_msg void OnChkName();
	afx_msg void OnChkOther();
	afx_msg void OnBtnPrint();
	afx_msg void OnBtnInfoModify();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKillfocusEditAge();
	afx_msg void OnKillfocusEditAbstinency();
	afx_msg void OnKillfocusEditSpermvolume();
	afx_msg void OnKillfocusEditLiquifytime();
	afx_msg void OnKillfocusEditDilutionratio();
	afx_msg void OnKillfocusEditPh();
	afx_msg void OnKillfocusEditRoomtempera();
	afx_msg void OnClose();
	afx_msg void OnBtnComparePrint();
	afx_msg void OnBtnDelSel();
	//}}AFX_MSG
	afx_msg LRESULT OnUserMsgComparePrintBtn(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUserMsgDelSeltBtn(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	void ClearCtrls();
	bool IsInqueryState();
	void GetPageBound(int curPage,int& low,int & up,DividePage& dp);
	void DeleteDetectNO(const CString& tablename);
	~CLiveDataRetriveDlg();

	CPropertySheet m_sheet;
	CLivenessResultPage m_livenessPage;
	CLiveImagePage  m_imagePage;
	
	DividePage m_dp[2];  //  0 ·Ç²éÑ¯Ì¬  1 ²éÑ¯Ì¬

private:

	enum ModifyStatus{ WANTMODIFY, SAVEMODIFY }m_ModifyStatus;
	void DataBack();
	void SaveModify();

	bool m_bBtnPreparePrint;
public:
	void ReadRecordFromDatabase(const CString &strSQLinfo);

	void OnComparePrint(CString wParam, CString lParam);

	void delByKey(const CString&);
	
	void ClearItem();
public:
	CLivePrintFrame* m_pLivePrintFrame;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIVEDATARETRIVEDLG_H__82E00AAE_42F6_4483_AACE_6DEFA2AB47C2__INCLUDED_)
