#if !defined(AFX_MORPHADATAGATHER_H__3A6D1D2A_7AA5_4A56_926B_109EA256B5D3__INCLUDED_)
#define AFX_MORPHADATAGATHER_H__3A6D1D2A_7AA5_4A56_926B_109EA256B5D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaDataGather.h : header file
//
#include "datalistctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMorphaDataGather dialog

class CMorphaDataGather : public CDialog
{
// Construction
public:
	CMorphaDataGather(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMorphaDataGather)
	enum { IDD = IDD_DIALOG_MORPHA_DATA_GATHER };
	CDataListCtrl	m_List;

	CString	m_strInfo;
	CString	m_length;
	CString	m_width;
	CString	m_area;
	CString	m_ellipticity;
	CString	m_perfor_area;
	CString	m_head_area;
	CString	m_perimeter;
	CString	m_head_perfor_area;
	CString	m_tail_length;
	CString	m_tail_width;
	CString	m_tail_angle;
	CString	m_extension;
	CString	m_symmetry;
	CString	m_ruga;

	//}}AFX_DATA

	LPBYTE m_lpData;
	LPBITMAPINFOHEADER m_lpInfo;
	void ReadImage(const CString& pID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMorphaDataGather)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMorphaDataGather)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListMorphadata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHADATAGATHER_H__3A6D1D2A_7AA5_4A56_926B_109EA256B5D3__INCLUDED_)
