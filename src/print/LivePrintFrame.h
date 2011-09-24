#pragma once


// CLivePrintFrame ���
#include "liveprintview.h"

class CLivePrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CLivePrintFrame)
public:
	CLivePrintFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLivePrintFrame();
	void OnFilePrintPreview();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CLivePrintView* m_pView;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};


