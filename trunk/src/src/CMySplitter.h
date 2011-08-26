//CMySplitter.h   
#pragma   once   
#include   "stdafx.h"   

class   CMySplitter   :   public   CSplitterWnd   
{   
    DECLARE_DYNCREATE(CMySplitter)   
		
protected:   
    afx_msg   void   OnLButtonDown(UINT   nFlags,   CPoint   point);   
    afx_msg   BOOL   OnSetCursor(CWnd*   pWnd,   UINT   nHitTest,   UINT   message);   
    afx_msg   void   OnMouseMove(UINT   nFlags,   CPoint   point);   
    
    DECLARE_MESSAGE_MAP()
};