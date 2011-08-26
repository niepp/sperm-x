#if !defined(AFX_LIVEDETECTEDPAMASETTINGDLG_H__A57095C3_8535_47AD_A3F8_F4A0E2BA86CA__INCLUDED_)
#define AFX_LIVEDETECTEDPAMASETTINGDLG_H__A57095C3_8535_47AD_A3F8_F4A0E2BA86CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LiveDetectedPamaSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectedPamaSettingDlg dialog

class CLiveDetectedPamaSettingDlg : public CDialog
{
// Construction
public:
	CLiveDetectedPamaSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLiveDetectedPamaSettingDlg)
	enum { IDD = IDD_LIVEDETECTED_PAMA_SETTING };
	double	m_dDepth;
	double  m_dStrThrd;
	double  m_dLinThr;
	double  m_dCurThr;
	double  m_dV0Thr;
	double  m_dV1Thr;
	double  m_dV2Thr;
	int		m_nEnlargeRatio;
	//}}AFX_DATA

void ReadInitParamDataFromFile();
void WriteParamDataToFile();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLiveDetectedPamaSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLiveDetectedPamaSettingDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIVEDETECTEDPAMASETTINGDLG_H__A57095C3_8535_47AD_A3F8_F4A0E2BA86CA__INCLUDED_)
