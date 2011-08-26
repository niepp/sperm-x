// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0B57EEC8_1BE4_4216_9F75_E05A2018103C__INCLUDED_)
#define AFX_MAINFRM_H__0B57EEC8_1BE4_4216_9F75_E05A2018103C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "sperm.h"

#include "CMySplitter.h"
#include "LiveDataRetriveDlg.h"
#include "MorphaDataRetriveDlg.h"
#include "CombinationAnalyseDlg.h"

#import   "craxdrt.dll"   no_namespace

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMySplitter m_wndSplitter;

public:
	CMorphaDataRetriveDlg  *m_pWndMorphadlg;
	CLiveDataRetriveDlg  *m_pWndLivedlg;
	CCombinationAnalyseDlg *m_pWndCombinDlg;

	CStatusBar  m_wndStatusBar;
	int         m_nQueryPage;  //   0 形态学查询，1 活力查询

	int m_hFrm; 
	int m_wFrm;
	int m_splterW;
	int m_splterH;

	bool m_bMenuItemDataOption;

	IApplicationPtr m_Application;
	IStream *m_pStream;
	static UINT LoadCrystalApp(LPVOID);
	LRESULT MsgGetCrystalIAppPtr(WPARAM wParam, LPARAM lParam);
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
 	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);

	void OnViewStatusBar();
	CWnd* GetView3();
	CWnd* GetView1();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void OnDataLive();
	afx_msg void OnDataMorpha();
	afx_msg void OnDataBackup();
	afx_msg void OnDataRestor();
	afx_msg void OnUserManage();
	afx_msg void OnMenuGlobalAnalyse();
	afx_msg void OnMorphaGatherData();
	afx_msg void OnReg();
	afx_msg void OnUpdateDataOptionS(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0B57EEC8_1BE4_4216_9F75_E05A2018103C__INCLUDED_)
