#if !defined(AFX_LIVEIMAGEPAGE_H__9AD6FA3E_E8BA_438B_A3C2_005BD66CA433__INCLUDED_)
#define AFX_LIVEIMAGEPAGE_H__9AD6FA3E_E8BA_438B_A3C2_005BD66CA433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LiveImagePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiveImagePage dialog
#include "resource.h"
#include "DrawStatic.h"
class CLiveImagePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLiveImagePage)

// Construction
public:
	void GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER& lpHead, LPBYTE& lpData);
	CLiveImagePage();
	~CLiveImagePage();

// Dialog Data
	//{{AFX_DATA(CLiveImagePage)
	enum { IDD = IDD_DIALOG_LIVEIMAGE };
	CDrawStatic	m_wndDrawImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLiveImagePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLiveImagePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
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

#endif // !defined(AFX_LIVEIMAGEPAGE_H__9AD6FA3E_E8BA_438B_A3C2_005BD66CA433__INCLUDED_)
