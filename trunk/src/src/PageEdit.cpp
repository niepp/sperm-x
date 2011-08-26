// PageEdit.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "PageEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageEdit

CPageEdit::CPageEdit()
{
}

CPageEdit::~CPageEdit()
{
}


BEGIN_MESSAGE_MAP(CPageEdit, CEdit)
	//{{AFX_MSG_MAP(CPageEdit)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageEdit message handlers

void CPageEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SetSel(0,-1);
}


