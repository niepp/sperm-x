// LivePrintView.cpp : ʵ���ļ�
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


// CLivePrintView ��ͼ

void CLivePrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CLivePrintView ���

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


// CLivePrintView ��Ϣ�������

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

	// TODO:  ���� DoPreparePrinting �Ե��á���ӡ���Ի���
	DoPreparePrinting(pInfo);
	return TRUE;
}
