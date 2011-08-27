// SpermView.h : interface of the CSpermView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPERMVIEW_H__5E420D3D_4A9A_4202_BE82_1146A85F23AC__INCLUDED_)
#define AFX_SPERMVIEW_H__5E420D3D_4A9A_4202_BE82_1146A85F23AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include  <qedit.h>
#include "SpermDoc.h"
#include "LiveDetectDLG.h"

class CSpermView : public CScrollView
{
protected: // create from serialization only
	CSpermView();
	DECLARE_DYNCREATE(CSpermView)
public:
	
// BOOL csIsInRect(CPoint c, int r, CPoint p);

// Attributes
public:
	static BOOL IsInRect(CPoint c,int r,CPoint p);
	CSpermDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpermView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:

	enum{
		VIDEO_PLAY,      // 视频播放状态
		FRAME_ADJUST_RESULT, // 显示实时调整结果状态
		VIDEO_RESULT,	 // 视频分析结果显示状态
		MORPHA_IMAGE,    // 形态学图像打开显示状态
		MORPHA_RESULT,   // 形态学图像结果显示状态
		INITIAL			 // 初始态
	}m_eViewShowMode;  // 视图区显示模式

	CBrush m_brushBackground;// 背景画刷

	// 形态学
	BOOL m_bAnalyseFinished;
	int m_nOpenOpt; // 2 形态学 1 活力分析
	int m_nPos;              // 形态学，当前选中精子编号
    CvvImage m_imgMorphaImg; // 形态学分析图像


	// 处理中的序列图
	LPBYTE m_lpImage[FRAME_NUM];			// 序列帧数据区指针
	LPBITMAPINFOHEADER m_lpBMIH[FRAME_NUM]; // 序列帧信息头的指针

	// 轨迹跟踪结果图
	LPBYTE m_lpTrackResultBmData;			   // 轨迹跟踪结果图数据区指针
	LPBITMAPINFOHEADER m_lpTrackResultBmInfo;  // 轨迹跟踪结果图信息头的指针

	// 手工调整图
	LPBYTE	m_lpAdjustBmData;
	LPBITMAPINFOHEADER m_lpAdjustBmInfo;

	virtual ~CSpermView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);

// Generated message map functions
//protected:
public:
	//{{AFX_MSG(CSpermView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnImage();
	afx_msg void OnPrintTable( ) ;
	afx_msg void OnPrintPreview( );
	afx_msg void OnPrintSetup();
	afx_msg void OnDataOption();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnButtonLeftarrow();
	afx_msg void OnButtonRightarrow();
	afx_msg void OnUpdateButtonLeftarrow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRightarrow(CCmdUI* pCmdUI);
	afx_msg void OnDeleteSperm();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSpermMorset();
	afx_msg void OnSpermNormal();
	afx_msg void OnUpdateFileOpenS(CCmdUI* pCmdUI);
	afx_msg void OnSpermDel();
	afx_msg void OnSpermAddA();
	afx_msg void OnSpermAddB();
	afx_msg void OnSpermAddC();
	afx_msg void OnSpermAddD();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuAddNormal2DB();
	afx_msg void OnMenuAddAbnormal2DB();
//	afx_msg void OnMenuDbVerdict();
	//}}AFX_MSG
	afx_msg LRESULT OnCapFrmFinished(WPARAM wParam,LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
	void AddContrast(int x);
	int ChoosePoint(int x);

	//控制各项按钮的有效状态
	public:
		void PreviewResult(int reprottype,CString detectno);

		void GuoPing();
		void UserShowSperm();
		int  GetSpermIndex(CPoint pt);  //  pt为屏幕坐标
		void DrawMemResImg(CDC* pDC);
		void DrawMemDCImage(CDC *pDC1,const CRect rect);

		void DrawImage(CDC* pDC);
		static double dist(CPoint p, CPoint q);
		void FileOpenMorphaImage();
		void FileOpenDiskVideoFile();

		// { 2008_8_31
		CFile *m_pFile;
		BOOL ReadImageFromDisk(int i, CFile *pFile);
		BOOL WriteImageToDisk(int i, CFile *pFile);
		// } 2008_8_31

		int m_nDeviceID;
		bool  m_bMenuItemFileOpen;

		int m_Indx;
		CPoint m_pt;
		
		// { 2009_3_31
		UINT mMoveShowTimer;
		int  mFrm;
		void markTheNewSperm(const POINT& pt, COLORTYPE ct);
		CPoint mClick_pt;
		// } 2009_3_31
};

#ifndef _DEBUG  // debug version in SpermView.cpp
inline CSpermDoc* CSpermView::GetDocument()
   { return (CSpermDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPERMVIEW_H__5E420D3D_4A9A_4202_BE82_1146A85F23AC__INCLUDED_)



