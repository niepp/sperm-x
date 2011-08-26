// RegisterThread.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "RegisterThread.h"
#include "AllFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterThread

IMPLEMENT_DYNCREATE(CRegisterThread, CWinThread)

CRegisterThread::CRegisterThread()
{
}

CRegisterThread::~CRegisterThread()
{
}

BOOL CRegisterThread::InitInstance()
{
	m_Connection.CreateInstance("adodb.connection");
	return TRUE;
}

int CRegisterThread::ExitInstance()
{
	if(m_Connection->GetState() == adStateOpen)
		m_Connection->Close();
	m_Connection.Release();
	CoUninitialize();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRegisterThread, CWinThread)
	//{{AFX_MSG_MAP(CRegisterThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(USER_THDMSG_REGISTER,Register)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterThread message handlers


void CRegisterThread::Register(WPARAM w, LPARAM l)
{
	try
	{	
		// 连接数据源	
		CString& m_strServerName=*((CString*)w);
		CString str("Provider=SQLOLEDB.1;\
			Persist Security Info=False;Data Source=");
		str+=m_strServerName;		
		if(m_Connection->GetState() == adStateClosed)
			m_Connection->Open((LPCSTR)str,(LPCSTR)"sa"
				,(LPCSTR)"sa",adConnectUnspecified);
		m_Connection->PutCursorLocation(adUseClient);


		_bstr_t sqlUseDatabase("use master");
		m_Connection->Execute(sqlUseDatabase,NULL,adCmdText);

		//   创建保存用户名密码的表
		if(!IsTableExist(m_Connection, TEXT("username")))
		{
			CString strSQLuser("create table username\
			(username varchar(20) primary key,\
			 password varchar(20))");
			m_Connection->Execute((LPCTSTR)strSQLuser,NULL,adCmdText);
		}	
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		PostQuitMessage(0);
		return;
	}
	m_regdlg.m_strServerName = *((CString*)w);
	m_regdlg.m_hParentWnd  = (HWND)l;
	m_regdlg.DoModal();
	PostQuitMessage(0);
}
