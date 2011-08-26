// RegesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "RegesterDlg.h"
#include "LogInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegesterDlg dialog

extern _ConnectionPtr theConnection;
CRegesterDlg::CRegesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegesterDlg)
	m_strPass1 = _T("");
	m_strPass2 = _T("");
	m_strUsername = _T("");
	//}}AFX_DATA_INIT
}


void CRegesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegesterDlg)
	DDX_Text(pDX, IDC_EDIT_PASS1, m_strPass1);
	DDV_MaxChars(pDX, m_strPass1, 20);
	DDX_Text(pDX, IDC_EDIT_PASS2, m_strPass2);
	DDV_MaxChars(pDX, m_strPass2, 20);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDV_MaxChars(pDX, m_strUsername, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegesterDlg, CDialog)
	//{{AFX_MSG_MAP(CRegesterDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegesterDlg message handlers

bool CRegesterDlg::IsUsernameIllegal(const char* username)
{
	int sz=strlen(username);
	int i;
	for(i=0;i<sz;++i)
	{
		if( !(   (username[i]>='0' &&username[i]<='9')
			  || (username[i]>='a'&&username[i]<='z')
			  || (username[i]>='A'&&username[i]<='Z') 
			  || username[i]=='_' )
		  )  
		{
			return false;
		}
	}
	if(username[0]>='0' && username[0]<='9')
		return false;
	return true;
}

void CRegesterDlg::OnOK() 
{

	UpdateData(TRUE);
	if(m_strUsername.IsEmpty())
	{
		AfxMessageBox("用户名不能为空");
		return;
	}
	if(m_strPass1 != m_strPass2)
	{
		AfxMessageBox("密码不一致,请重新输入");
		return;
	}
	if(!IsUsernameIllegal(m_strUsername))
	{
		AfxMessageBox("用户名为英文,数字与下划线组成,且不能以数字开头.");
		return;
	}
	if(m_strPass1.IsEmpty())
	{
		AfxMessageBox("密码不能为空");
		return;
	}
	_bstr_t strUseMaster("use master");
	_bstr_t strSQlInsUser("insert into username values(");
	strSQlInsUser +=_bstr_t("'") + (LPCTSTR)m_strUsername+_bstr_t("','")
		+ (LPCTSTR)m_strPass1+_bstr_t("')");
	try
	{	
		m_pConnection->Execute(strUseMaster,NULL,adCmdText);
		m_pConnection->Execute(strSQlInsUser,NULL,adCmdText);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
	SendMessage(WM_CLOSE);
	CDialog::OnOK();
}

void CRegesterDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(m_hParentWnd,USER_MSG_SETBUTTONSTATE,1,0);
	m_pConnection.Release();
	CDialog::OnClose();
}

BOOL CRegesterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pConnection.CreateInstance("adodb.connection");
	CString str("Provider=SQLOLEDB.1;\
		Persist Security Info=False;Data Source=");
	str+=m_strServerName;
	try
	{
		m_pConnection->Open((LPCSTR)str,(LPCSTR)"sa"
				,(LPCSTR)"sa",adConnectUnspecified);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegesterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	SendMessage(WM_CLOSE);
	CDialog::OnCancel();
}
