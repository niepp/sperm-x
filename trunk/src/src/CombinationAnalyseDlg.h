#if !defined(AFX_COMBINATIONANALYSEDLG_H__E884A20F_64F5_4DA5_BF83_EEAABB545C94__INCLUDED_)
#define AFX_COMBINATIONANALYSEDLG_H__E884A20F_64F5_4DA5_BF83_EEAABB545C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CombinationAnalyseDlg.h : header file
//

#include "ActiveResultPropPage.h"
#include "MophyResultPropPage.h"
#include "FluoreResultPropPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCombinationAnalyseDlg dialog

typedef struct commbinBasicData {
	CString pName;
	
	CString pCaseNO;

	COleDateTime pDetectDateTime;

// 	CString	pShape;
// 		
// 	double	pSpermVolume;
// 	
// 	CString	pSmell;
// 		
// 	double	pPH;
// 		
// 	double	pDilutionRatio;
// 		
// 	int	pLiquifyTime;
// 		
// 	CString	pThickness;
// 		
// 	CString	pCohesion;
// 		
// 	CString pLiquifyState;
// 		
// 	int	pRoomTempera;

}BasicData;

//  µ≤‚÷µ
typedef struct commbinActiveData {
	double spermTotalDensity;
	double moveSpermRatio;
	double frontSpermRatio;
	double movespeed;
	commbinActiveData(double dtd = 0.0, double dmr = 0.0, double dfr = 0.0, double dms = 0.0) {
		spermTotalDensity = dtd;
		moveSpermRatio    = dmr;
		frontSpermRatio   = dfr;
		movespeed         = dms;
	}
}ActiveData;

typedef struct commbinMophyData {
	double spermTotalNormalRatio;
	commbinMophyData(double dtn = 0.0) {
		spermTotalNormalRatio = dtn;
	}
}MophyData;

typedef struct commbinFluoreData {
	double spermBeforeNormalRatio;
	double spermAfterNormalRatio;
	commbinFluoreData(double dbn = 0.0, double dan = 0.0) {
		spermBeforeNormalRatio = dbn;
		spermAfterNormalRatio  = dan;
	}
}FluoreData;

class CCombinationAnalyseDlg : public CDialog
{
// Construction
public:
	CCombinationAnalyseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCombinationAnalyseDlg)
	enum { IDD = IDD_DLG_COMBINATION_ANALYSE };
	CString	m_strEditShape;
	CString	m_strEditSpermVolume;
	CString	m_strEditSmell;
	CString	m_strEditPH;
	CString	m_strEditDilutionRatio;
	CString	m_strEditLiquifyTime;
	CString	m_strEditThickness;
	CString	m_strEditCohension;
	CString	m_strEditLiquifyState;
	CString	m_strEditRoomTempera;
	CString	m_strEditInputPatientNO;
	CString	m_strEditInputName;
	CString m_strEditInputRptDoc;
	COleDateTime	m_InputDateTime;
	BOOL	m_chk_datetime;
	double	m_midu;
	double	m_huolv;
	double	m_fanyinlv;
	double	m_wanzhenglv;
	double	m_xingtailv;
	double	m_sudu;
	CString	m_strDiagnostic;
	//}}AFX_DATA

	bool	m_bmidu;
	bool	m_bhuolv;
	bool	m_bsudu;
	bool	m_bxingtailv;
	double  m_dzhishu;

	CString strName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombinationAnalyseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCombinationAnalyseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInquery();
	afx_msg void OnBtnCombinAnalyse();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnCheckDatetime();
	afx_msg void OnChangeEditInputpatientno();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChangeEditInputname();
	afx_msg void OnBtnComput();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditMidu();
	afx_msg void OnChangeEditHuolv();
	afx_msg void OnChangeEditSudu();
	afx_msg void OnChangeEditXingtailv();
	afx_msg void OnBtnRptPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

public:

	CPropertySheet        m_shtPropertySheet;
	CActiveResultPropPage m_WndActiveResultPropPage;
	CMophyResultPropPage  m_WndMophyResultPropPage;
	CFluoreResultPropPage m_WndFluoreResultPropPage;

	commbinActiveData  m_ActiveData;
	commbinMophyData   m_MophyData;
	commbinFluoreData  m_FluoreData;

	double m_dGenerateIndex;

	void ClearVar();
	double ComputeGenerateIndex();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBINATIONANALYSEDLG_H__E884A20F_64F5_4DA5_BF83_EEAABB545C94__INCLUDED_)
