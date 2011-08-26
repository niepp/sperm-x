//{{AFX_INCLUDES()
#include "datagrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATARETRIVEDLG_H__2F3C48AD_E5CB_486E_872C_FA9FFCED72A1__INCLUDED_)
#define AFX_DATARETRIVEDLG_H__2F3C48AD_E5CB_486E_872C_FA9FFCED72A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DataListCtrl.h"
#include "MorphaEdgeDlg.h"
#include "MorphaPadDlg.h"
#include "MorphaResDlg.h"
#include "Pageedit.h"

#include "SperMopyParameter.h"  

typedef struct SingleSpermInfo
{
	SperMopyParameter  smPara;    // 各项参数
	POINT			   pos;		  // 位置
	bool			   bIsNormal; // 是否正常
	int                nSpermNO;  // 某次检测的第几个精子
}SingleSpermInfo;


class CMorphaDataRetriveDlg : public CDialog
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
	void ClearCtrls();
	void GetPageBound(int curPage, int &low, int &up,DividePage& dp);
	bool IsInqueryState();

	void ReadRecordFromDatabase(const CString& strSQLinfo);
	void OnPrint() { OnBtnPrintmultihead(); }
	CMorphaDataRetriveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMorphaDataRetriveDlg)
	enum { IDD = IDD_DIALOG_MORPHA_RETRIVE };
	CPageEdit	m_wndPageNum;
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
	BOOL	m_BChkDetectDate;
	BOOL	m_BChkDetectNO;
	BOOL	m_BChkName;
	BOOL	m_BChkOther;
	CString	m_strCmBOtherSelection;
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
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMorphaDataRetriveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMorphaDataRetriveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInquery();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBtnFirstpage();
	afx_msg void OnBtnLastpage();
	afx_msg void OnBtnNextpage();
	afx_msg void OnBtnPrepage();
	afx_msg void OnChkDetectdate();
	afx_msg void OnChkDetectno();
	afx_msg void OnChkName();
	afx_msg void OnChkOther();
	afx_msg void OnKillfocusDatetimepickerInputFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusDatetimepickerInputTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditInputdetectno();
	afx_msg void OnKillfocusEditInputname();
	afx_msg void OnBtnPrintsimple();
	afx_msg void OnBtnPrintmultihead();
	afx_msg void OnClose();
	afx_msg void OnBtnInfoModifyMopy();
	afx_msg void OnBtnDelSel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnUserMsgDelSeltBtn(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUserMsgPrtBtn(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	CPropertySheet m_sheet;
	CMorphaResDlg  m_resdlg;
	CMorphaPadDlg  m_paddlg;
	CMorphaEdgeDlg m_edgedlg;
	DividePage     m_dp[2];
	vector<SingleSpermInfo> m_vSpermInfo;

private:

	enum ModifyStatus{ WANTMODIFY, SAVEMODIFY }m_ModifyStatus;
	void DataBack();
	void SaveModify();

	void delByKey(const CString&);
	void ClearItem();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATARETRIVEDLG_H__2F3C48AD_E5CB_486E_872C_FA9FFCED72A1__INCLUDED_)
