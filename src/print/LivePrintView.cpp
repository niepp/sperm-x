// LivePrintView.cpp : 实现文件
//

#include "stdafx.h"
#include "../Sperm.h"
#include "LivePrintView.h"


// CLivePrintView

IMPLEMENT_DYNCREATE(CLivePrintView, CView)

CLivePrintView::CLivePrintView()
{

}

CLivePrintView::~CLivePrintView()
{
//	MessageBox("~CLivePrintView");
}

BEGIN_MESSAGE_MAP(CLivePrintView, CView)
END_MESSAGE_MAP()


// CLivePrintView 绘图

void CLivePrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CLivePrintView 诊断

#ifdef _DEBUG
void CLivePrintView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLivePrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLivePrintView 消息处理程序

void CLivePrintView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	ASSERT(FALSE);
}

void CLivePrintView::OnFilePrintPreview()
{
	CView::OnFilePrintPreview();
}
BOOL CLivePrintView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// TODO:  调用 DoPreparePrinting 以调用“打印”对话框
	DoPreparePrinting(pInfo);
	return TRUE;
}
