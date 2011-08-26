// PrinteDlg.cpp : implementation file
//
#include "StdAfx.h"
#include "MainFrm.h"
#include "PrinteDlg.h"

#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent theCrystalEvent;
/////////////////////////////////////////////////////////////////////////////
// CPrinteDlg dialog

void CPrinteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrinteDlg)
	DDX_Control(pDX, IDC_ACTIVEXREPORTVIEWER1, m_Viewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrinteDlg, CDialog)
	//{{AFX_MSG_MAP(CPrinteDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinteDlg message handlers

void CPrinteDlg::OnSize(UINT nType, int cx, int cy) 
{	
	CDialog::OnSize(nType, cx, cy);
	
	if(m_Viewer.GetSafeHwnd())
	{
		m_Viewer.MoveWindow(0,0,cx,cy);
	} 
	
	// TODO: Add your message handler code here	
}

CPrinteDlg::CPrinteDlg(int reporttype,crxparm parm, CString wParam, CString lParam, CWnd *pParent)
	: CDialog(CPrinteDlg::IDD, pParent)
{
	ASSERT(checktypevalid(reporttype));
	m_parm = parm;
	m_wParam = wParam;
	m_lParam = lParam;
	m_reporttype = reporttype;
	iscrxready = FALSE;
	m_Report = NULL;
}

BOOL CPrinteDlg::checktypevalid(int reporttype)
{
	if(reporttype==COMBIN)//综合分析报表
		return TRUE;
	if(reporttype==MOPHY)//形态学报表
		return TRUE;	
	if(reporttype==COMPARE) //对比分析报表
		return TRUE;
	if(reporttype==ACTIVE)//活力报表
		return TRUE;
	if(reporttype==ACTIVESIMPLE)//简单活力报表
		return TRUE;

	return false;
}

BOOL CPrinteDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	try
	{
		dummy.vt = VT_EMPTY;
		CMainFrame* Frm = (CMainFrame*)AfxGetMainWnd();
		IApplicationPtr &pApplication = Frm->m_Application;
		if(pApplication == NULL) {
			WaitForSingleObject(theCrystalEvent, INFINITE);  // 等待初始化处理线程完成
			IStream *pStream = Frm->m_pStream;		
			HRESULT h = ::CoGetInterfaceAndReleaseStream(pStream,
														 __uuidof(IApplication),
														 (void**)&pApplication);
		}

		if(pApplication==NULL)
		{
			::AfxMessageBox("水晶报表初始化失败");
			return FALSE;
		}
	
		CString  strPath(__argv[0]);
		strPath  =  strPath.Left(strPath.ReverseFind('\\')); 
		CString reportpath;

		if(m_reporttype==COMBIN)
		{
			reportpath = strPath + "\\CombinationAnalyse.rpt";
		}
		else if(m_reporttype==MOPHY)
		{
			reportpath = strPath + "\\MophyReportMultyHead.rpt";
		}
		else if(m_reporttype==ACTIVESIMPLE)
		{
			reportpath = strPath + "\\ActivateSpermReportSimple.rpt";
		}
		else if(m_reporttype==ACTIVE)
		{
			reportpath = strPath + "\\ActivateSpermReport.rpt";
		}
		else if(m_reporttype==COMPARE)
		{
			reportpath = strPath + "\\CompareReport.rpt";
		}
		else {
			ASSERT(FALSE);
			return FALSE;
		}
		m_Report = pApplication->OpenReport(_bstr_t(reportpath), dummy);
		if(m_Report==NULL)
		{
			::AfxMessageBox("未找到报表文件");
			return FALSE;
		}
		HRESULT hr = m_Report->DiscardSavedData();

		if(FAILED(hr))
		{
			::AfxMessageBox("错误");
			return FALSE;
		}

		VARIANT postdata1, postdata2;
		VariantInit(&postdata1); 
		VariantInit(&postdata2);
		postdata1.vt = VT_BSTR; 
		postdata2.vt = VT_BSTR;
		BSTR bstr1 = m_wParam.AllocSysString();
		BSTR bstr2 = m_lParam.AllocSysString();
		postdata1.bstrVal = bstr1;
		postdata2.bstrVal = bstr2;
				
		hr = m_Report->Database->Tables->Item[1]->SetLogOnInfo(m_parm.ipadd.GetBuffer(255),
															m_parm.database.GetBuffer(255),
															m_parm.username.GetBuffer(255),
															m_parm.passwd.GetBuffer(255));
		if(FAILED(hr))
		{
			::AfxMessageBox("错误");
			this->SendMessage(WM_CLOSE);
			return FALSE;
		}
		
		_bstr_t str="id";
		if(m_reporttype==COMPARE) str = "ID_before";
		hr = m_Report->ParameterFields->GetItemByName(str)->SetCurrentValue(postdata1);
		if(m_lParam!=_T(""))
			hr = m_Report->ParameterFields->GetItemByName("ID_after")->SetCurrentValue(postdata2);
		if(FAILED(hr))
		{
			::AfxMessageBox("错误");
			this->SendMessage(WM_CLOSE);
			return FALSE;
		}
		::SysAllocString(bstr1);
		::SysAllocString(bstr2);
		VariantClear(&postdata1);
		VariantClear(&postdata2);

		SetWindowPos(NULL,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),0); 
		m_Viewer.SetReportSource(m_Report);
		m_Viewer.ViewReport();
		m_Viewer.Zoom(1);
	}
	catch (_com_error& e)
	{
		HandleError(e);
		return FALSE;
	}

	iscrxready = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrinteDlg::HandleError(_com_error &e)
{
	IErrorInfo* pErrorInfo = e.ErrorInfo();
	HRESULT hr = e.Error();
	if (pErrorInfo)
	{
		BSTR bsDesc = NULL;
		pErrorInfo->GetDescription( &bsDesc );
		_bstr_t sDesc( bsDesc, false );
		
		::MessageBox( 0, sDesc.operator LPCTSTR(), "", MB_OK );
		
		pErrorInfo->Release();
	}
}

void CPrinteDlg::OnDestroy()
{
	CDialog::OnDestroy();
	if(iscrxready==TRUE)
	{
		try	{
			m_Report.Release();
		}
		catch(_com_error e)
		{
			HandleError(e);
		}
	}
}
