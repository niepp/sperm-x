#if !defined(AFX_VIEW3_H__CDBA7A0F_08EE_4B6A_8AE3_376E9CA3E088__INCLUDED_)
#define AFX_VIEW3_H__CDBA7A0F_08EE_4B6A_8AE3_376E9CA3E088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView3 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "LiveDetectDLG.h"
#include "MorphaDetectDLG.h"

class CView3 : public CFormView
{
protected:
	CView3();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView3)

// Form Data
public:
	//{{AFX_DATA(CView3)
	enum { IDD = IDD_DIALOG_BUTTON };
	CString	m_strDetectNO;
	CString	m_strPatientName;

	CString m_sTotalSpermNum;    //视野中精子总数
	CString m_sMovementSpermNum; //活动精子数目
	CString m_sActiveSpermNum;	 //活跃精子数目
	CString m_sGradeA;
	CString m_sGradeB;
	CString m_sGradeC;
	CString m_sGradeD;
	CString m_sLineMoveSpermNum; //直线运动精子数
	CString m_sCurveMoveSpermNum;//曲线运动精子数
	CString m_sActiveSpermDensity; // 活跃精子密度
	CString m_sTotalSpermDensity;  // 总精子密度
	CString m_sMovementSpermRatio; // 活动精子比率，即存活率
	//}}AFX_DATA

// Attributes
public:
	enum DETECTMODE{CAMERA=0X100,VIDEO=0X1000,AVI=0X1,BMP=0X10,
		CAMERAAVI=0X101,CAMERABMP=0X110,VIDEOAVI=0X1001,VIDEOBMP=0X1010,MORPHAIMAGE=0X1111
	} m_eDetectMode;

public:
	bool state;
	CLiveDetectDLG* m_pwndLiveDetectDlg;
	CMorphaDetectDLG* m_pwndMorphaDetDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:

	virtual ~CView3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CView3)
	afx_msg void OnBtnLiveDetected();
	afx_msg void OnPatient();
	afx_msg void OnButtonMorphadetect();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnPatientInfoQuery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3_H__CDBA7A0F_08EE_4B6A_8AE3_376E9CA3E088__INCLUDED_)
