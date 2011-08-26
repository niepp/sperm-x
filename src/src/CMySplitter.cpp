//CMySplitter.cpp   
#include "stdAfx.h"
#include   "CMySplitter.h"   

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMySplitter,   CSplitterWnd)   

BEGIN_MESSAGE_MAP(CMySplitter,   CSplitterWnd)   
ON_WM_LBUTTONDOWN()   
ON_WM_SETCURSOR()   
ON_WM_MOUSEMOVE()   
END_MESSAGE_MAP()   

void   CMySplitter::OnLButtonDown(UINT   nFlags,   CPoint   point)   
{             
	// 直接返回   
    return; 
}   

BOOL   CMySplitter::OnSetCursor(CWnd*   pWnd,   UINT   nHitTest,   UINT   message)   
{         
	// 当光标进入分割窗口时，不允许改变样子   
    return   CSplitterWnd::OnSetCursor(pWnd,nHitTest,message);   
}   

void   CMySplitter::OnMouseMove(UINT   nFlags,   CPoint   point)   
{                 
	// 跳过调用CSplitterWnd::OnMouseMove,因为他会引起光标的改变   
	CWnd::OnMouseMove(nFlags,point);
}   
