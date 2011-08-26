#if !defined(AFX_MORPHAPADDLG_H__14E6D7A3_EF83_4C39_816A_8A68461AAE5A__INCLUDED_)
#define AFX_MORPHAPADDLG_H__14E6D7A3_EF83_4C39_816A_8A68461AAE5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaPadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMorphaPadDlg dialog
#include "drawstatic.h"
class CMorphaPadDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CMorphaPadDlg)

// Construction
public:
	void GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER& lpHead, LPBYTE& lpData);
	CMorphaPadDlg();
	~CMorphaPadDlg();

// Dialog Data
	//{{AFX_DATA(CMorphaPadDlg)
	enum { IDD = IDD_DIALOG_MORPHAIMGPAD };
	CDrawStatic	m_wndPadImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMorphaPadDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMorphaPadDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LPBITMAPINFOHEADER m_lpHead;
	LPBYTE m_lpData;
	CString m_strDetectNO;
	int m_nHScrollPos;
	int m_nVScrollPos;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHAPADDLG_H__14E6D7A3_EF83_4C39_816A_8A68461AAE5A__INCLUDED_)
