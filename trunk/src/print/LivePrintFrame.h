#pragma once


// CLivePrintFrame 框架
#include "liveprintview.h"

class CLivePrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CLivePrintFrame)
public:
	CLivePrintFrame();           // 动态创建所使用的受保护的构造函数
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


