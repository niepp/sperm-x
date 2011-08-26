#if !defined(AFX_MORPHAEDGEDLG_H__68CC9E11_35A0_4337_9997_E33B0562EF94__INCLUDED_)
#define AFX_MORPHAEDGEDLG_H__68CC9E11_35A0_4337_9997_E33B0562EF94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaEdgeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMorphaEdgeDlg dialog
#include "DrawStatic.h"

class CMorphaEdgeDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CMorphaEdgeDlg)

// Construction
public:
	void GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER &lpHead, LPBYTE &lpData);
	CMorphaEdgeDlg();
	~CMorphaEdgeDlg();

// Dialog Data
	//{{AFX_DATA(CMorphaEdgeDlg)
	enum { IDD = IDD_DIALOG_MORPHAIMGEDGE };
	CDrawStatic	m_wndEdgeImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMorphaEdgeDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMorphaEdgeDlg)
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

#endif // !defined(AFX_MORPHAEDGEDLG_H__68CC9E11_35A0_4337_9997_E33B0562EF94__INCLUDED_)
