// UserManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "UserManageDlg.h"
#include "RegesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg dialog

extern _ConnectionPtr theConnection;

CUserManageDlg::CUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserManageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserManageDlg)
	DDX_Control(pDX, IDC_LIST_USER, m_wndUserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialog)
	//{{AFX_MSG_MAP(CUserManageDlg)
	ON_BN_CLICKED(IDC_BUTTON_USERADD, OnButtonUseradd)
	ON_BN_CLICKED(IDC_BUTTON_USERDELETE, OnButtonUserdelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg message handlers

void CUserManageDlg::OnButtonUseradd() 
{

	try
	{	
		CRegesterDlg dlg;
		dlg.DoModal();
		_bstr_t str("select * from master..username");
		_RecordsetPtr rs("adodb.recordset");
		rs->Open((LPCTSTR)str,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);
		int i;
		while( m_wndUserList.GetCount() )
			 m_wndUserList.DeleteString(0);
		int cn=rs->GetRecordCount();
		for(i=0;i<cn;++i)
		{
			_variant_t var = rs->GetCollect((long)0);
			m_wndUserList.AddString((char*)(_bstr_t)var);
			rs->MoveNext();
		}
		
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
	m_wndUserList.SetFocus();
	m_wndUserList.SetCurSel(0);

}

BOOL CUserManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon,TRUE);
	try
	{	
		_bstr_t str("select * from master..username");
		_RecordsetPtr rs("adodb.recordset");
		rs->Open((LPCTSTR)str,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);

		int i;
		while( m_wndUserList.GetCount() )
			 m_wndUserList.DeleteString(0);
		int cn=rs->GetRecordCount();
		for(i=0;i<cn;++i)
		{
			_variant_t var = rs->GetCollect((long)0);
			m_wndUserList.AddString((char*)(_bstr_t)var);
			rs->MoveNext();
		}	
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return FALSE;
	}
	m_wndUserList.SetFocus();
	m_wndUserList.SetCurSel(0);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserManageDlg::OnButtonUserdelete() 
{

	if(m_wndUserList.GetCount() == 0) return;
	CString str;
	m_wndUserList.GetText(m_wndUserList.GetCurSel(),str);
	
	try
	{	
		CString strSQL,strSel;
		strSQL.Format("delete from master.dbo.username where username='%s'",str);
	    theConnection->Execute((LPCTSTR)strSQL,NULL,adCmdText);
		strSel.Format("select * from master.dbo.username");
		_bstr_t str("select * from master..username");
		_RecordsetPtr rs("adodb.recordset");
		rs->Open((LPCTSTR)str,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);
		int i;
		while( m_wndUserList.GetCount() )
			 m_wndUserList.DeleteString(0);
		int cn=rs->GetRecordCount();
		for(i=0;i<cn;++i)
		{
			_variant_t var = rs->GetCollect((long)0);
			m_wndUserList.AddString((char*)(_bstr_t)var);
			rs->MoveNext();
		}	
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
	m_wndUserList.SetFocus();
	m_wndUserList.SetCurSel(0);
}
