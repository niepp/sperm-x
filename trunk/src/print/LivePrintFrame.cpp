// LivePrintFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "../Sperm.h"
#include "LivePrintFrame.h"


// CLivePrintFrame

IMPLEMENT_DYNCREATE(CLivePrintFrame, CFrameWnd)

CLivePrintFrame::CLivePrintFrame()
{

}

CLivePrintFrame::~CLivePrintFrame()
{
//	MessageBox("~CLivePrintFrame")
}
void CLivePrintFrame::OnFilePrintPreview()
{
	m_pView->OnFilePrintPreview();
}

BEGIN_MESSAGE_MAP(CLivePrintFrame, CFrameWnd)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLivePrintFrame 消息处理程序

BOOL CLivePrintFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pView = new CLivePrintView;
	m_pView->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,pContext);
	return TRUE;
}

void CLivePrintFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFrameWnd::OnClose();
}

void CLivePrintFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//	m_pView->DestroyWindow();
	//	ShowWindow(SW_HIDE);
}
