#pragma once


// CLivePrintView ��ͼ

class CLivePrintView : public CView
{
	DECLARE_DYNCREATE(CLivePrintView)

public:
	CLivePrintView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLivePrintView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void OnFilePrintPreview();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
};


