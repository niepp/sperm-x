
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Sperm.h"
#include "SpermView.h"
#include "view3.h"
#include "MainFrm.h"
#include "Splash.h"
#include "logindlg.h"
#include "morphaDataRetriveDlg.h"
#include "LiveDataretrivedlg.h"
#include "UserManageDlg.h"
#include "PrinteDlg.h"
#include "MorphaDataGather.h"
#include "SysReg.h"
#include "GloablFun.h"

#include "afxmt.h"

extern CEvent theCrystalEvent;
extern _ConnectionPtr theConnection;

#include <odbcinst.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString theStr;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_DATA_LIVE, OnDataLive)
	ON_COMMAND(ID_DATA_MORPHA, OnDataMorpha)
	ON_COMMAND(ID_DATA_BACKUP, OnDataBackup)
	ON_COMMAND(ID_DATA_RESTOR, OnDataRestor)
	ON_COMMAND(ID_USER_MANAGE, OnUserManage)
	ON_COMMAND(IDC_MENU_GLOBAL_ANALYSE, OnMenuGlobalAnalyse)
	ON_COMMAND(ID_MORPHA_GATHER_DATA, OnMorphaGatherData)
	ON_COMMAND(ID_REG, OnReg)
	ON_UPDATE_COMMAND_UI(ID_DATA_OPTION_S, OnUpdateDataOptionS)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_GET_CRYSTAL_IAPP_PTR, MsgGetCrystalIAppPtr)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

	HWND hWnd = ::GetDesktopWindow();
	HDC  hDC  = ::GetDC(hWnd);
	m_hFrm = ::GetDeviceCaps(hDC, VERTRES);
	m_wFrm = ::GetDeviceCaps(hDC, HORZRES);
	::ReleaseDC(hWnd, hDC);
    m_splterW = m_wFrm*0.75;
	m_splterH = m_hFrm;

	LONG lValue=0;
	RegQueryValue(HKEY_LOCAL_MACHINE, theStr, NULL, &lValue);
	if(lValue==0)
		m_bMenuItemDataOption = false;
	else {
		char *pBuf=new char[lValue];
		RegQueryValue(HKEY_LOCAL_MACHINE, theStr, pBuf, &lValue);
		CString str_sn(pBuf);
		delete []pBuf; pBuf = NULL;
		m_bMenuItemDataOption = CheckRegister(str_sn);
	}

	m_pWndLivedlg   = new CLiveDataRetriveDlg();
	m_pWndMorphadlg = new CMorphaDataRetriveDlg();

	m_pWndCombinDlg = new CCombinationAnalyseDlg();
	
	m_Application = NULL;
	m_pStream = NULL;
	AfxBeginThread((AFX_THREADPROC)LoadCrystalApp, this);
	
}

UINT CMainFrame::LoadCrystalApp(LPVOID param)
{
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IApplicationPtr pApplication = NULL;
	IStream* pStream = NULL;
	pApplication.CreateInstance("CrystalRuntime.Application");
	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(__uuidof(IApplication),
														 pApplication,
														 &pStream);
	CMainFrame* Frm = (CMainFrame*)param;
	Frm->m_pStream = pStream;
	::CoUninitialize();
	theCrystalEvent.SetEvent();
	::SendMessage(Frm->GetSafeHwnd(), MSG_GET_CRYSTAL_IAPP_PTR, 0, 0);
	if(pApplication) { return TRUE; }
	else return FALSE;
}

LRESULT CMainFrame::MsgGetCrystalIAppPtr(WPARAM wParam, LPARAM lParam)
{
	if(m_Application == NULL) {
		WaitForSingleObject(theCrystalEvent, INFINITE);  // 等待初始化处理线程完成		
		HRESULT h = ::CoGetInterfaceAndReleaseStream(m_pStream,
													 __uuidof(IApplication),
													 (void**)&m_Application);
		return h;
	}
	return S_OK;
}

CMainFrame::~CMainFrame()
{
	delete m_pWndLivedlg; m_pWndLivedlg = NULL;
	delete m_pWndMorphadlg; m_pWndMorphadlg = NULL;
	delete m_pWndCombinDlg; m_pWndCombinDlg = NULL;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,ID_MY_STATUSBAR) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

  	m_wndStatusBar.SetPaneInfo(1,0,0,400);
	m_wndStatusBar.SetPaneInfo(2,0,0,50);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX);


	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);	

	return 0;

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this, 1, 2))
		return false;
	
	CSize  czleft(m_splterW, m_splterH);
	CSize  czright(m_wFrm - m_splterW, m_hFrm);

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSpermView), czleft, pContext) )
		return false;

	if(!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CView3), czright, pContext) ) 
		return false;

	CRect rc;
	GetClientRect(&rc);
	CSize paneSizeStaff(rc.Width(),rc.Height());
	m_wndSplitter.SetColumnInfo(0,m_splterW,0);

    return true;
		
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX |
		       WS_MAXIMIZEBOX| WS_MAXIMIZE | WS_SYSMENU;
	
	cs.lpszName="精子检测系统";
	cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,0,LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

LRESULT CMainFrame::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nHitTest=CFrameWnd::OnNcHitTest(point);
	return CFrameWnd::OnNcHitTest(point);
}

CWnd * CMainFrame::GetView1()
{ 
	return m_wndSplitter.GetPane(0,0);
}

CWnd* CMainFrame::GetView3()
{
	return m_wndSplitter.GetPane(0,1);
}

void CMainFrame::OnViewStatusBar()
{
	m_wndStatusBar.ShowWindow((m_wndStatusBar.GetStyle()&WS_VISIBLE)==0);
    RecalcLayout();
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_wndStatusBar.GetStyle()&WS_VISIBLE)!=0);
    pCmdUI->SetCheck(m_wndStatusBar.ShowWindow(m_wndStatusBar.GetStyle()));

}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CView3* pView3=(CView3*)GetView3();
	ASSERT(pView3&&pView3->m_hWnd);
	if(pView3->m_pwndMorphaDetDlg&&pView3->m_pwndMorphaDetDlg->m_hWnd)
	{
		pView3->m_pwndMorphaDetDlg->SendMessage(WM_CLOSE,1,2);
		if(pView3->m_pwndMorphaDetDlg->m_nYesNo == IDNO)
			return;
	}

	if(pView3->m_pwndLiveDetectDlg&&pView3->m_pwndLiveDetectDlg->m_hWnd)
	{
		pView3->m_pwndLiveDetectDlg->SendMessage(WM_CLOSE);
		if(pView3->m_pwndLiveDetectDlg->m_nYesNo == IDNO)
			return;
	}
	CFrameWnd::OnClose();
}

void CMainFrame::OnDataMorpha() 
{
	m_nQueryPage = 0;
	if( m_pWndMorphadlg->m_hWnd == NULL )
		m_pWndMorphadlg->Create(IDD_DIALOG_MORPHA_RETRIVE);
	m_pWndMorphadlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnDataLive() 
{
	m_nQueryPage = 1;
	if( m_pWndLivedlg->m_hWnd == NULL )
		m_pWndLivedlg->Create(IDD_DIALOG_LIVE_RETRIVE);
	m_pWndLivedlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnDataBackup() 
{
	 static char filter[]="备份文件(*.bak)|*.bak|所有文件(*.*)|*.*||";
	 CFileDialog dlg(FALSE , NULL , NULL , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter);
	 dlg.m_ofn.lpstrTitle = "保存数据库" ;
	 if(dlg.DoModal() == IDOK)
	 {
		CString path=dlg.GetPathName();
		if(path.Right(4) != ".bak" )
			path += ".bak";
		CFileFind find;
		if(find.FindFile(path)==TRUE)
			CFile::Remove(path);
		CString strSQL;
		strSQL.Format("backup database sperm to disk='%s'",path);
		try
		{
			theConnection->Execute((LPCTSTR)strSQL,NULL,adCmdText);
		}
		catch (_com_error& e)
		{
			MessageBox(e.Description());
			return;
		}
	 } 
}

void CMainFrame::OnDataRestor() 
{
	 static char filter[]="还原文件(*.bak)|*.bak|所有文件(*.*)|*.*||";
	 CFileDialog dlg(TRUE , NULL , NULL , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter);
	 dlg.m_ofn.lpstrTitle = "还原数据库" ;
	 if(dlg.DoModal() == IDOK)
	 {
		CString path=dlg.GetPathName();
		CString strSQL,strSEL1,strSEL2;
		strSQL.Format("restore database sperm from disk='%s'",path);
		strSEL1.Format("use master");
		strSEL2.Format("use sperm");
		try
		{	
			theConnection->Execute((LPCTSTR)strSEL1,NULL,adCmdText);
			theConnection->Execute((LPCTSTR)strSQL,NULL,adCmdText);
			theConnection->Execute((LPCTSTR)strSEL2,NULL,adCmdText);
		}
		catch (_com_error& e)
		{
			MessageBox(e.Description());
			return;
		}
	 } 	
}

void CMainFrame::OnUserManage() 
{
	CUserManageDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnMenuGlobalAnalyse() 
{
	// TODO: Add your command handler code here
	assert(m_pWndCombinDlg);
	if( m_pWndCombinDlg->m_hWnd == NULL )
		m_pWndCombinDlg->Create(IDD_DLG_COMBINATION_ANALYSE);
	m_pWndCombinDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnMorphaGatherData()
{
	// TODO: Add your command handler code here
	m_nQueryPage = 2;
	CMorphaDataGather mdg;
	mdg.DoModal();
}

void CMainFrame::OnReg() 
{
	// TODO: Add your command handler code here
	SysReg srDlg;
	srDlg.DoModal();
}

void CMainFrame::OnUpdateDataOptionS(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	m_bMenuItemDataOption = true;
	pCmdUI->Enable(m_bMenuItemDataOption);
}
