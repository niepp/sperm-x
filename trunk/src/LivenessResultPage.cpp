// LivenessResultPage.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LivenessResultPage.h"
#include "LiveDataRetriveDlg.h"
#include "MainFrm.h"
#include "SpermView.h"
#include "selbookmarks.h"
#include "DataListCtrl.h"
#include "AllFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpermView* theView1;

/////////////////////////////////////////////////////////////////////////////
// CLivenessResultPage property page
extern _ConnectionPtr theConnection;
IMPLEMENT_DYNCREATE(CLivenessResultPage, CPropertyPage)

CLivenessResultPage::CLivenessResultPage() : CPropertyPage(CLivenessResultPage::IDD)
{
	//{{AFX_DATA_INIT(CLivenessResultPage)
	//}}AFX_DATA_INIT
}

CLivenessResultPage::~CLivenessResultPage()
{
}

void CLivenessResultPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLivenessResultPage)
	DDX_Control(pDX, IDC_LISTDATA, m_wndLiveList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLivenessResultPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLivenessResultPage)
	ON_NOTIFY(NM_CLICK, IDC_LISTDATA, OnClickListdata)
	ON_WM_CREATE()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLivenessResultPage message handlers

BEGIN_EVENTSINK_MAP(CLivenessResultPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CLivenessResultPage)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CLivenessResultPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

BOOL CLivenessResultPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	static CString strCol;

	CLiveDataRetriveDlg& dlg= *(((CMainFrame*) AfxGetMainWnd())->m_pWndLivedlg);	
	//CString strSQLGetCol("select distinct name from syscolumns  where id=object_id('livenessresult') \
	//	or id = object_id('spermmovitypara') or id = object_id('spermnumstatics')");
	//CString strSQLGetLiveResCol("select distinct name  from syscolumns  where id=object_id('livenessresult')");
	//CString strSQLGetmovityCol("select distinct name  from syscolumns  where id=object_id('spermmovitypara')");
	//CString strSQLGetnumstaticCol("select distinct name  from syscolumns  where id=object_id('spermnumstatics')");
	CString strSQLLiveForDisplay("select *,"
		"livenessresult.pdetectno as pdetectno , "
		"spermnumstatics.pactivespermnum as pactivespermnum , "
		"spermnumstatics.pFrontSpermNum as pFrontSpermNum , "
		"spermnumstatics.pStraightSpermNum as spermnumstatics , "
		"spermnumstatics.pActiveSpermRatio as pActiveSpermRatio , "
		"spermnumstatics.pActiveSpermDensity as pActiveSpermDensity , "
		"spermnumstatics.pCurveSpermNum as pCurveSpermNum "
		"into livefordisplay  ");
	try
	{
		if(strCol.IsEmpty())
		{		

			std::set<CString>column_names;
			std::set<CString>live_column_names;
			std::set<CString>movity_column_names;
			std::set<CString>numstatic_column_names;

			GetTableColumnNames(theConnection, column_names, "livenessresult");
			GetTableColumnNames(theConnection, column_names, "spermmovitypara");
			GetTableColumnNames(theConnection, column_names, "spermnumstatics");

			GetTableColumnNames(theConnection, live_column_names, "livenessresult");
			GetTableColumnNames(theConnection, movity_column_names, "spermmovitypara");
			GetTableColumnNames(theConnection, numstatic_column_names, "spermnumstatics");

			std::set<CString>::const_iterator citr = column_names.begin();
			for (; citr != column_names.end(); ++citr)
			{
				CString e(", ");
				std::set<CString>::const_iterator c = citr;
				if(c++ != column_names.end())
				{
					e = " ";
				}
				CString vt = *citr;				
				int afind = (live_column_names.find(vt) != live_column_names.end());
				int bfind = (movity_column_names.find(vt) != movity_column_names.end());
				int cfind = (numstatic_column_names.find(vt) != numstatic_column_names.end());
				if( afind+bfind+cfind == 1)
					strSQLLiveForDisplay+=(char*)(_bstr_t)vt+e;
				else if(afind)
					strSQLLiveForDisplay+=CString("livenessresult.")+(char*)(_bstr_t)vt+e;
				else if(bfind)
					strSQLLiveForDisplay+=CString("spermmovitypara.")+(char*)(_bstr_t)vt+e;

			}

			strSQLLiveForDisplay+="from livenessresult,spermmovitypara,spermnumstatics,  \
								  basicinfo,spermchait,image_table \
								  where livenessresult.pdetectno = spermmovitypara.pdetectno and \
								  livenessresult.pdetectno = spermnumstatics.pdetectno and \
								  livenessresult.pdetectno = basicinfo.pdetectno and \
								  livenessresult.pdetectno = spermchait.pdetectno and \
								  livenessresult.pdetectno = image_table.pdetectno ";

			if(IsTableExist(theConnection,"livefordisplay") == true)
			{
				theConnection->Execute("drop table livefordisplay",NULL,adCmdText);
			}
			theConnection->Execute((LPCSTR)strSQLLiveForDisplay,NULL,adCmdText);

			strCol = strSQLLiveForDisplay ;
		}
		strSQLLiveForDisplay = strCol;
		CString str("select count(*) from livefordisplay");
		_RecordsetPtr rs=theConnection->Execute((LPCSTR)str,NULL,adCmdText);

		dlg.m_dp[0].nTotalRecord=rs->GetCollect((long)0).iVal;
		dlg.m_dp[0].nPageRecord = NUMPERPAGE;
		dlg.m_dp[0].nTotalPage = dlg.m_dp[0].nTotalRecord / dlg.m_dp[0].nPageRecord +
			(dlg.m_dp[0].nTotalRecord % dlg.m_dp[0].nPageRecord!=0);

		CString queryinfo;
		queryinfo.Format("共查询到%d条记录",dlg.m_dp[0].nTotalRecord);
		dlg.SetDlgItemText(IDC_STATIC_QUERY,queryinfo);
		if(dlg.m_dp[0].nTotalRecord == 0)
		{
			CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
			pMainFrm->m_pWndLivedlg->GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndLivedlg->GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndLivedlg->GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndLivedlg->GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndLivedlg->GetDlgItem(IDC_PAGENUM)->EnableWindow(FALSE);
			return FALSE;
		}

		int lowRow, upRow;
		dlg.GetPageBound(1,lowRow,upRow,dlg.m_dp[0]);

		GetRecordSet(rs,lowRow,upRow);
		int n=m_wndLiveList.SetData(rs);


		queryinfo.Format("第 %d / %d 页",dlg.m_dp[0].nCurPage,dlg.m_dp[0].nTotalPage);
		dlg.SetDlgItemText(IDC_PAGENUM,queryinfo);

		dlg.GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(dlg.m_dp[dlg.IsInqueryState()].nCurPage != 1);
		dlg.GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(dlg.m_dp[dlg.IsInqueryState()].nCurPage != 1);
		dlg.GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(dlg.m_dp[dlg.IsInqueryState()].nCurPage != dlg.m_dp[dlg.IsInqueryState()].nTotalPage);
		dlg.GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(dlg.m_dp[dlg.IsInqueryState()].nCurPage != dlg.m_dp[dlg.IsInqueryState()].nTotalPage );
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLivenessResultPage::OnClickListdata(NMHDR* pNMHDR, LRESULT* pResult) 
{   
	NM_LISTVIEW*   pNMListView  = (NM_LISTVIEW*)pNMHDR; 

	LV_COLUMN  lc;
	lc.mask  = LVCF_TEXT;
	TCHAR pszText[40];
	lc.pszText =  pszText;
	lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
	int nCol = 0;
	while (m_wndLiveList.GetColumn(nCol,&lc))
	{
		if(CString(lc.pszText) == TEXT("检测号"))
			break;
		++nCol;
	}

	CString str=m_wndLiveList.GetItemText(pNMListView->iItem,nCol);
	if(str == "" ) return;

	CString strSQLinfo("select distinct *,basicinfo.pdetectno as pdetectno "
		"from basicinfo,spermchait "
		"where basicinfo.pdetectno=spermchait.pdetectno and "
		"basicinfo.pdetectno='");
	strSQLinfo= strSQLinfo + str + CString("'");
	
	CLiveDataRetriveDlg& dlg= *(((CMainFrame*) AfxGetMainWnd())->m_pWndLivedlg);
	dlg.UpdateData();
	try
	{
		_variant_t var;
		COleDateTime pddt;
		_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);

		var = rs->GetCollect("pDetectno");
		dlg.m_strEditDetectNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pCaseNO");
		dlg.m_strEditCaseNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pname");
		dlg.m_strEditName = (char*)(_bstr_t)var;
		var = rs->GetCollect("pspermvolume");
		dlg.m_strEditSpermVolume = (char*)(_bstr_t)var;
		var = rs->GetCollect("page");
		dlg.m_strEditAge = (char*)(_bstr_t)var;
		pddt=rs->GetCollect("pdetectdatetime");
		dlg.m_oledateDetectDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		dlg.m_oletimeDetectTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pSampleNO");
		dlg.m_strEditSampleNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pdilutionratio");
		dlg.m_strEditDilutionRatio = (char*)(_bstr_t)var;
		var = rs->GetCollect("pShape");
		dlg.m_strEditShape = (char*)(_bstr_t)var;
		pddt=rs->GetCollect("pspermgetdatetime");
		dlg.m_oledateSpermGetDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		dlg.m_oletimeSpermGetTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pcolor");
		dlg.m_strEditSpermColor =  (char*)(_bstr_t)var;
		var = rs->GetCollect("pcohesion");
		dlg.m_strEditCohension =  (char*)(_bstr_t)var;
		var = rs->GetCollect("pRoomTempera");
		dlg.m_strEditRoomTempera = (char*)(_bstr_t)var;
		var = rs->GetCollect("pdaysofabstinency");
		dlg.m_strEditAbstinency = (char*)(_bstr_t)var;
		var = rs->GetCollect("pliquifystate");
		dlg.m_strEditLiquifyState = (char*)(_bstr_t)var;
		var = rs->GetCollect("psmell");
		dlg.m_strEditSmell = (char*)(_bstr_t)var;
		var = rs->GetCollect("pthickness");
		dlg.m_strEditThickness = (char*)(_bstr_t)var;
		var = rs->GetCollect("pph");
		dlg.m_strEditPH = (char*)(_bstr_t)var;
		var = rs->GetCollect("pspermmethod");
		dlg.m_strEditSpermWay = (char*)(_bstr_t)var;
		var = rs->GetCollect("pliquifytime");
		dlg.m_strEditLiquifyTime = (char*)(_bstr_t)var;
		dlg.GetDlgItem(IDC_BTN_PRINT)->EnableWindow(TRUE);
		dlg.UpdateData(FALSE);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
	
	*pResult = 0;

	// Update UI button
	UINT uSelectedCount = m_wndLiveList.GetSelectedCount();
	BOOL enable = uSelectedCount==2 ? TRUE : FALSE;
	CDialog *pdlg = ((CMainFrame*)AfxGetMainWnd())->m_pWndLivedlg;
	::SendMessage(pdlg->GetSafeHwnd(), USER_MSG_SHOW_COMPARE_PRINT_BTN, (WPARAM)enable, 0);

	enable = uSelectedCount>=1 ? TRUE : FALSE;
	::SendMessage(pdlg->GetSafeHwnd(), USER_MSG_SHOW_DEL_SEL_BTN, (WPARAM)enable, 0);
}

int CLivenessResultPage::FindCol(_RecordsetPtr &rs,const _variant_t& colName)
{
	rs->MoveFirst();
	while ( !rs->EndOfFile)
	{
		_variant_t vt = rs->GetCollect((long)0);
		if(colName == vt)
			return TRUE;
		rs->MoveNext();
	}
	return FALSE;
}

void CLivenessResultPage::GetRecordSet(_RecordsetPtr &rs, int row1, int row2)
{
	ASSERT(row1>=0&&row2>=0&&row1<=row2);
	CString strSQL;
	CString tablename;
	if(((CLiveDataRetriveDlg*)(GetParent()->GetParent()->GetParent()))->IsInqueryState())
		tablename = "liveforinquery";
	else
		tablename = "livefordisplay";
	strSQL.Format("select * from (select top %d * from %s) a\
		where pdetectno not in(select top %d pdetectno from %s)"
		, row2, tablename, row1, tablename);
	if(row1 == 0 )
	{
		strSQL.Format("select  * from (select top %d * from %s) a "
			,row2,tablename);
	}
	else
	{
		strSQL.Format("select  * from (select top %d * from %s) a \
			where pdetectno not in(select top %d pdetectno from %s)"
			,row2,tablename,row1,tablename);
	}
	if(rs == NULL)
		rs.CreateInstance("adodb.recordset");
	rs = theConnection->Execute((LPCSTR)strSQL, NULL, adCmdText);
}

int CLivenessResultPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
