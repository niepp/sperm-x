#if !defined(AFX_VIEW2_H__73F99EA9_7B07_401C_8340_EF96B6108E12__INCLUDED_)
#define AFX_VIEW2_H__73F99EA9_7B07_401C_8340_EF96B6108E12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView2 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CView2 : public CFormView
{
protected:
	CView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView2)

// Form Data
public:
	//{{AFX_DATA(CView2)
	enum { IDD = IDD_DIALOG_RESULT };

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

	int m_nTotalSpermNum;    //视野中精子总数
	int m_nMovementSpermNum; //活动精子数目
	int m_nActiveSpermNum;	 //活跃精子数目

	double m_dGradeA;
	double m_dGradeB;
	double m_dGradeC;
	double m_dGradeD;

	int m_nLineMoveSpermNum; //直线运动精子数
	int m_nCurveMoveSpermNum;//曲线运动精子数

	double m_dActiveSpermDensity; // 活跃精子密度
	double m_dTotalSpermDensity;  // 总精子密度
	double m_dMovementSpermRatio; // 活动精子比率，即存活率

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CView2)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW2_H__73F99EA9_7B07_401C_8340_EF96B6108E12__INCLUDED_)
