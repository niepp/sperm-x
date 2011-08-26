#if !defined(AFX_DRAWSTATIC_H__184FA674_BDE1_4BF2_B9CC_8512D55CCC73__INCLUDED_)
#define AFX_DRAWSTATIC_H__184FA674_BDE1_4BF2_B9CC_8512D55CCC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawStatic window

class CDrawStatic : public CStatic
{
// Construction
public:
	CDrawStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawStatic)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void CreateImage(const LPBITMAPINFOHEADER head,const LPBYTE data);
	LPBITMAPINFOHEADER m_lpHead;
	LPBYTE   m_lpData;

	CToolTipCtrl  *m_pToolTipCtrl;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWSTATIC_H__184FA674_BDE1_4BF2_B9CC_8512D55CCC73__INCLUDED_)





















