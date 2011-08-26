// BrowerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "BrowerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowerDlg dialog


CBrowerDlg::CBrowerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowerDlg)
	m_framechoose = 0;
	//}}AFX_DATA_INIT
}


void CBrowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowerDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_framechoose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowerDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowerDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowerDlg message handlers
