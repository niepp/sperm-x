// SysReg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "SysReg.h"
#include "GloablFun.h"
#include "MainFrm.h"
#include "Spermview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SysReg dialog
extern CSpermView* theView1;
extern CString theStr;

SysReg::SysReg(CWnd* pParent /*=NULL*/)
	: CDialog(SysReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SysReg)
	m_sn = _T("");
	//}}AFX_DATA_INIT
}

void SysReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SysReg)
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_sn);
	DDV_MaxChars(pDX, m_sn, 19);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SysReg, CDialog)
	//{{AFX_MSG_MAP(SysReg)
	ON_BN_CLICKED(IDC_BUTTON_REGSTER, OnButtonRegster)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SysReg message handlers

void SysReg::OnButtonRegster()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(CheckRegister(m_sn)) {
		HKEY hKey;
		RegCreateKey(HKEY_LOCAL_MACHINE, theStr, &hKey);
		RegSetValue(hKey, NULL, REG_SZ, m_sn, m_sn.GetLength());
		RegCloseKey(hKey);
		((CMainFrame*)AfxGetMainWnd())->m_bMenuItemDataOption = true;
		MessageBox("注册成功");
		CDialog::OnOK();
	}
	else 
		MessageBox("失败，非法注册码");
}
