// LivePrintFrame.cpp : ʵ���ļ�
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


// CLivePrintFrame ��Ϣ�������

BOOL CLivePrintFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pView = new CLivePrintView;
	m_pView->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,pContext);
	return TRUE;
}

void CLivePrintFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFrameWnd::OnClose();
}

void CLivePrintFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	//	m_pView->DestroyWindow();
	//	ShowWindow(SW_HIDE);
}
