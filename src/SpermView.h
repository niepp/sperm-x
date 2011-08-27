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
		VIDEO_PLAY,      // ��Ƶ����״̬
		FRAME_ADJUST_RESULT, // ��ʾʵʱ�������״̬
		VIDEO_RESULT,	 // ��Ƶ���������ʾ״̬
		MORPHA_IMAGE,    // ��̬ѧͼ�����ʾ״̬
		MORPHA_RESULT,   // ��̬ѧͼ������ʾ״̬
		INITIAL			 // ��ʼ̬
	}m_eViewShowMode;  // ��ͼ����ʾģʽ

	CBrush m_brushBackground;// ������ˢ

	// ��̬ѧ
	BOOL m_bAnalyseFinished;
	int m_nOpenOpt; // 2 ��̬ѧ 1 ��������
	int m_nPos;              // ��̬ѧ����ǰѡ�о��ӱ��
    CvvImage m_imgMorphaImg; // ��̬ѧ����ͼ��


	// �����е�����ͼ
	LPBYTE m_lpImage[FRAME_NUM];			// ����֡������ָ��
	LPBITMAPINFOHEADER m_lpBMIH[FRAME_NUM]; // ����֡��Ϣͷ��ָ��

	// �켣���ٽ��ͼ
	LPBYTE m_lpTrackResultBmData;			   // �켣���ٽ��ͼ������ָ��
	LPBITMAPINFOHEADER m_lpTrackResultBmInfo;  // �켣���ٽ��ͼ��Ϣͷ��ָ��

	// �ֹ�����ͼ
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

	//���Ƹ��ť����Ч״̬
	public:
		void PreviewResult(int reprottype,CString detectno);

		void GuoPing();
		void UserShowSperm();
		int  GetSpermIndex(CPoint pt);  //  ptΪ��Ļ����
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



