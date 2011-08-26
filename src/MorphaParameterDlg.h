#if !defined(AFX_MORPHAPARAMETERDLG_H__FA5929D2_A174_4D3F_8A8D_07DCDBAC8A4C__INCLUDED_)
#define AFX_MORPHAPARAMETERDLG_H__FA5929D2_A174_4D3F_8A8D_07DCDBAC8A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaParameterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMorphaParameterDlg dialog

class CMorphaParameterDlg : public CDialog
{
// Construction
public:
	CMorphaParameterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMorphaParameterDlg)
	enum { IDD = IDD_DIALOG_MORPHAPARAMETER };
	CSliderCtrl	m_sliderThreshAdjustPart;
	CSliderCtrl	m_sliderThreshAdjust;
	CSliderCtrl	m_sliderGrowStep;
	int		m_nMicroscope;
	CComboBox	m_ComCtrl;
	CString	m_inf_length;
	CString	m_inf_area;
	CString	m_inf_ellipticity;
	CString	m_inf_perfor_area;
	CString	m_inf_head_area;
	CString	m_inf_perimeter;
	CString	m_inf_width;
	CString	m_inf_tail_length;
	CString	m_inf_tail_width;
	CString	m_inf_tail_angle;
	CString	m_inf_head_perfor_area;	
	CString	m_inf_extension;
	CString	m_inf_symmetry;
	CString	m_inf_ruga;
	CString	m_sup_length;
	CString	m_sup_area;
	CString	m_sup_ellipticity;
	CString	m_sup_perfor_area;
	CString	m_sup_head_area;
	CString	m_sup_perimeter;
	CString	m_sup_width;
	CString	m_sup_tail_length;
	CString	m_sup_tail_width;
	CString	m_sup_tail_angle;
	CString	m_sup_head_perfor_area;	
	CString	m_sup_extension;
	CString	m_sup_symmetry;
	CString	m_sup_ruga;
	CString	m_strGrowStep;
	CString	m_strThreshAdjust;
	CString	m_strThreshAdjustPart;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMorphaParameterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMorphaParameterDlg)
	afx_msg void OnSelchangeCombox();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ReadAndSetParameterData(CString fileName);

	int m_ThreshAdjustPart;
	int m_ThreshAdjust;
	double m_GrowStep;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHAPARAMETERDLG_H__FA5929D2_A174_4D3F_8A8D_07DCDBAC8A4C__INCLUDED_)
