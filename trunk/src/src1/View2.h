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

	CString m_sTotalSpermNum;    //��Ұ�о�������
	CString m_sMovementSpermNum; //�������Ŀ
	CString m_sActiveSpermNum;	 //��Ծ������Ŀ

	CString m_sGradeA;
	CString m_sGradeB;
	CString m_sGradeC;
	CString m_sGradeD;

	CString m_sLineMoveSpermNum; //ֱ���˶�������
	CString m_sCurveMoveSpermNum;//�����˶�������

	CString m_sActiveSpermDensity; // ��Ծ�����ܶ�
	CString m_sTotalSpermDensity;  // �ܾ����ܶ�
	CString m_sMovementSpermRatio; // ����ӱ��ʣ��������

	//}}AFX_DATA

	int m_nTotalSpermNum;    //��Ұ�о�������
	int m_nMovementSpermNum; //�������Ŀ
	int m_nActiveSpermNum;	 //��Ծ������Ŀ

	double m_dGradeA;
	double m_dGradeB;
	double m_dGradeC;
	double m_dGradeD;

	int m_nLineMoveSpermNum; //ֱ���˶�������
	int m_nCurveMoveSpermNum;//�����˶�������

	double m_dActiveSpermDensity; // ��Ծ�����ܶ�
	double m_dTotalSpermDensity;  // �ܾ����ܶ�
	double m_dMovementSpermRatio; // ����ӱ��ʣ��������

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
