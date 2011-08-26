// DISDLG.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "DISDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDISDLG dialog


CDISDLG::CDISDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CDISDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDISDLG)
	m_playtype = 4;
	m_framefrequence = 5;
	//}}AFX_DATA_INIT
}


void CDISDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDISDLG)
	DDX_Radio(pDX, IDC_RADIO7, m_playtype);
	DDX_Text(pDX, IDC_EDIT1, m_framefrequence);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDISDLG, CDialog)
	//{{AFX_MSG_MAP(CDISDLG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDISDLG message handlers
