// LogInDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LogInDlg.h"
#include "RegesterDlg.h"
#include "LoginThread.h"
#include "RegisterThread.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogInDlg dialog


extern _ConnectionPtr theConnection;



CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogInDlg)
	m_password = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
	m_pThread = NULL ;
	m_pRegisterThread = NULL ;
}


void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogInDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER_NAME, m_cmbServerName);
	DDX_CBString(pDX, IDC_COMBO_SERVER_NAME, m_strServerName);
	DDX_Text(pDX, IDC_EDIT_PASS, m_password);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogInDlg, CDialog)
	//{{AFX_MSG_MAP(CLogInDlg)
	ON_BN_CLICKED(IDC_BUTTON_REGSTER, OnButtonRegster)
	//}}AFX_MSG_MAP
	ON_MESSAGE(USER_MSG_AFTERCONNECTDATABASE,AfterConnectDatabase)
	ON_MESSAGE(USER_MSG_SETBUTTONSTATE,SetButtonState)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER_NAME, &CLogInDlg::OnCbnSelchangeComboServerName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogInDlg message handlers

BOOL CLogInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//  设置窗口图标
    HICON   hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);//IDR_MAINFRAME为资源中的图标ID号   
    SetIcon(hIcon,true);   
	
	m_cmbServerName.AddString("(local)");
	m_cmbServerName.AddString(GetIP());
	m_cmbServerName.SetCurSel(0);
	GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
	UpdateData(TRUE);


	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

const char* CLogInDlg::GetIP()
{
	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 )
	{ 
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if((hostinfo = gethostbyname(name)) != NULL)
			{ //这些就是获得IP的函数
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
			} 
		} 
		WSACleanup( );
	}
	return ip;
}

void CLogInDlg::OnOK() 
{
	UpdateData(TRUE);
	SetButtonState(0,0);
	static ConnectInfoThd thd;
	thd.hwnd = m_hWnd;
	thd.m_password = m_password;
	thd.m_strServerName = m_strServerName;
	thd.m_strUsername = m_strUserName;
	m_pThread = AfxBeginThread(RUNTIME_CLASS(CLoginThread));
	m_pThread->PostThreadMessage(WM_THDMSG_GETLOGININFO,(WPARAM)&thd,0);
}

void CLogInDlg::OnButtonRegster() 
{
	UpdateData();
	static CString serverName;
	serverName = m_strServerName;
	SetButtonState(0,0);
	m_pRegisterThread = AfxBeginThread(RUNTIME_CLASS(CRegisterThread));
	m_pRegisterThread->PostThreadMessage(USER_THDMSG_REGISTER,(WPARAM)&serverName,(LPARAM)m_hWnd);

}

LRESULT CLogInDlg::AfterConnectDatabase(WPARAM w, LPARAM l)
{
	CDialog::OnOK();
	return S_OK;
}

void CLogInDlg::OnCancel() 
{

	if(m_pThread)
	{
		SetButtonState(1,0);
		m_pThread->PostThreadMessage(WM_QUIT,0,0);
		m_pThread = NULL;
	}
	else if(m_pRegisterThread)
	{
		SetButtonState(1,0);
		m_pRegisterThread->PostThreadMessage(WM_QUIT,0,0);
		m_pRegisterThread = NULL;
	}
	else
	{
		CDialog::OnCancel();
	}
}

LRESULT CLogInDlg::SetButtonState(WPARAM w, LPARAM l)
{
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(w);
	GetDlgItem(IDC_EDIT_PASS)->EnableWindow(w);
	GetDlgItem(IDC_COMBO_SERVER_NAME)->EnableWindow(w);
	GetDlgItem(IDC_BUTTON_REGSTER)->EnableWindow(w);
	GetDlgItem(IDOK)->EnableWindow(w);
	return S_OK;
}

void CLogInDlg::OnCbnSelchangeComboServerName()
{
	// TODO: 在此添加控件通知处理程序代码
}
