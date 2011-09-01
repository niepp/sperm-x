// MorphaResDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MorphaResDlg.h"
#include "MainFrm.h"
#include "AllFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMorphaResDlg property page
extern _ConnectionPtr theConnection;
IMPLEMENT_DYNCREATE(CMorphaResDlg, CPropertyPage)

CMorphaResDlg::CMorphaResDlg() : CPropertyPage(CMorphaResDlg::IDD)
{
	//{{AFX_DATA_INIT(CMorphaResDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMorphaResDlg::~CMorphaResDlg()
{
}

void CMorphaResDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMorphaResDlg)
	DDX_Control(pDX, IDC_LIST_MORPHADATA, m_wndMorphaDataList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMorphaResDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CMorphaResDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MORPHADATA, OnClickListMorphadata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorphaResDlg message handlers

BOOL CMorphaResDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CMorphaDataRetriveDlg& dlg = *(pMainFrm->m_pWndMorphadlg);
	
	CString strSQLCRMfi("select morpharesult.* into morphafordisplay \
	    from morpharesult,basicinfo,morphaimage,spermchait,morpharesultratio \
		where morpharesult.pdetectno = basicinfo.pdetectno \
		  and morpharesult.pdetectno = morphaimage.pdetectno \
		  and morpharesult.pdetectno = spermchait.pdetectno \
		  and morpharesult.pdetectno = morpharesultratio.pdetectno");

	try
	{
		if(IsTableExist(theConnection, "morphafordisplay") == true)
		{
			theConnection->Execute((LPCTSTR)"drop table morphafordisplay",NULL,adCmdText);
		}
		theConnection->Execute((LPCTSTR)strSQLCRMfi,NULL,adCmdText);

		CString strGetN("select count(*) from morphafordisplay");
		_RecordsetPtr rs=theConnection->Execute((LPCSTR)strGetN,NULL,adCmdText);
		
		dlg.m_dp[0].nCurPage = 1;
		dlg.m_dp[0].nTotalRecord=rs->GetCollect((long)0).iVal;
		dlg.m_dp[0].nPageRecord = NUMPERPAGE;
		dlg.m_dp[0].nTotalPage = dlg.m_dp[0].nTotalRecord / dlg.m_dp[0].nPageRecord +
							 (dlg.m_dp[0].nTotalRecord % dlg.m_dp[0].nPageRecord!=0);
		CString queryinfo;
 		queryinfo.Format("共查询到%d条记录",dlg.m_dp[0].nTotalRecord);
 		dlg.SetDlgItemText(IDC_STATIC_QUERY,queryinfo);
		if(dlg.m_dp[0].nTotalRecord == 0 ) 
		{
			CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
			pMainFrm->m_pWndMorphadlg->GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndMorphadlg->GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndMorphadlg->GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndMorphadlg->GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(FALSE);
			pMainFrm->m_pWndMorphadlg->GetDlgItem(IDC_PAGENUM)->EnableWindow(FALSE);
			return FALSE;
		}
		
		int lowRow, upRow;
		dlg.GetPageBound(1,lowRow,upRow,dlg.m_dp[0]);


		GetRecordSet(rs,lowRow,upRow);
		int n=dlg.m_resdlg.m_wndMorphaDataList.SetData(rs);
		
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
void CMorphaResDlg::GetRecordSet(_RecordsetPtr &rs, int row1, int row2)
{
	ASSERT(row1>=0&&row2>=0&&row1<=row2);
	CString strSQL;
	CString tablename;
	if(((CMorphaDataRetriveDlg*)(GetParent()->GetParent()->GetParent()))->IsInqueryState())
		tablename = "morphaforinquery";
	else
		tablename = "morphafordisplay";
	if(row1 == 0 )
		strSQL.Format("select  * from (select top %d * from %s) a "
			,row2,tablename);
	else
		strSQL.Format("select  * from (select top %d * from %s) a \
			where pdetectno not in(select top %d pdetectno from %s)"
			,row2,tablename,row1,tablename);
	if(rs == NULL)
		rs.CreateInstance("adodb.recordset");
	rs = theConnection->Execute((LPCSTR)strSQL,NULL,adCmdText);
}

void CMorphaResDlg::OnClickListMorphadata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW*   pNMListView = (NM_LISTVIEW*)pNMHDR; 

	LV_COLUMN  lc;
	lc.mask  = LVCF_TEXT ;
	TCHAR pszText[40];
	lc.pszText =  pszText;
	lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
	int nCol = 0;
	while (m_wndMorphaDataList.GetColumn(nCol,&lc))
	{
		if(CString(lc.pszText) == TEXT("检测号"))
			break;
		++nCol;
	}
	CString str=m_wndMorphaDataList.GetItemText(pNMListView->iItem,nCol);
	if(str == "") return;
	CString strSQLinfo("select distinct *,basicinfo.pdetectno as 'pDetectno' from basicinfo,spermchait\
		where basicinfo.pdetectno=spermchait.pdetectno and \
		basicinfo.pdetectno='");
	strSQLinfo= strSQLinfo + str + CString("'");

	CPropertySheet* pSheet=(CPropertySheet*)GetParent();
	CStatic* pStatic=(CStatic*)pSheet->GetParent();
	CMorphaDataRetriveDlg* pDlg=(CMorphaDataRetriveDlg*)pStatic->GetParent();
	
	try
	{
		_variant_t var;
		COleDateTime pddt;
		_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);
		var = rs->GetCollect("pDetectno");
		pDlg->m_strEditDetectNO = (char*)(_bstr_t)var;
// 		pDlg->m_strEditDetectNO.TrimLeft();
// 		pDlg->m_strEditDetectNO.TrimRight();
		var = rs->GetCollect("pCaseNO");
		pDlg->m_strEditCaseNO = (char*)(_bstr_t)var;
// 		pDlg->m_strEditCaseNO.TrimLeft();
// 		pDlg->m_strEditCaseNO.TrimRight();
		var = rs->GetCollect("pname");
		pDlg->m_strEditName = (char*)(_bstr_t)var;
// 		pDlg->m_strEditName.TrimLeft();
// 		pDlg->m_strEditName.TrimRight();
		var = rs->GetCollect("pspermvolume");
		pDlg->m_strEditSpermVolume = (char*)(_bstr_t)var;
// 		pDlg->m_strEditSpermVolume.TrimLeft();
// 		pDlg->m_strEditSpermVolume.TrimRight();
		var = rs->GetCollect("page");
		pDlg->m_strEditAge = (char*)(_bstr_t)var;
// 		pDlg->m_strEditAge.TrimLeft();
// 		pDlg->m_strEditAge.TrimRight();
		pddt=rs->GetCollect("pdetectdatetime");
		pDlg->m_oledateDetectDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		pDlg->m_oletimeDetectTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pSampleNO");
		pDlg->m_strEditSampleNO = (char*)(_bstr_t)var;
// 		pDlg->m_strEditSampleNO.TrimLeft();
// 		pDlg->m_strEditSampleNO.TrimRight();
		var = rs->GetCollect("pdilutionratio");
		pDlg->m_strEditDilutionRatio = (char*)(_bstr_t)var;
// 		pDlg->m_strEditDilutionRatio.TrimLeft();
// 		pDlg->m_strEditDilutionRatio.TrimRight();
		var = rs->GetCollect("pShape");
		pDlg->m_strEditShape = (char*)(_bstr_t)var;
// 		pDlg->m_strEditShape.TrimLeft();
// 		pDlg->m_strEditShape.TrimRight();
		pddt=rs->GetCollect("pspermgetdatetime");
		pDlg->m_oledateSpermGetDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		pDlg->m_oletimeSpermGetTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pcolor");
		pDlg->m_strEditSpermColor =  (char*)(_bstr_t)var;
// 		pDlg->m_strEditSpermColor.TrimLeft();
// 		pDlg->m_strEditSpermColor.TrimRight();
		var = rs->GetCollect("pcohesion");
		pDlg->m_strEditCohension =  (char*)(_bstr_t)var;
// 		pDlg->m_strEditCohension.TrimLeft();
// 		pDlg->m_strEditCohension.TrimRight();
		var = rs->GetCollect("pRoomTempera");
		pDlg->m_strEditRoomTempera = (char*)(_bstr_t)var;
// 		pDlg->m_strEditRoomTempera.TrimLeft();
// 		pDlg->m_strEditRoomTempera.TrimRight();
		var = rs->GetCollect("pdaysofabstinency");
		pDlg->m_strEditAbstinency = (char*)(_bstr_t)var;
// 		pDlg->m_strEditAbstinency.TrimLeft();
// 		pDlg->m_strEditAbstinency.TrimRight();
		var = rs->GetCollect("pliquifystate");
		pDlg->m_strEditLiquifyState = (char*)(_bstr_t)var;
// 		pDlg->m_strEditLiquifyState.TrimLeft();
// 		pDlg->m_strEditLiquifyState.TrimRight();
		var = rs->GetCollect("psmell");
		pDlg->m_strEditSmell = (char*)(_bstr_t)var;
// 		pDlg->m_strEditSmell.TrimLeft();
// 		pDlg->m_strEditSmell.TrimRight();
		var = rs->GetCollect("pthickness");
		pDlg->m_strEditThickness = (char*)(_bstr_t)var;
// 		pDlg->m_strEditThickness.TrimLeft();
// 		pDlg->m_strEditThickness.TrimRight();
		var = rs->GetCollect("pph");
		pDlg->m_strEditPH = (char*)(_bstr_t)var;
// 		pDlg->m_strEditPH.TrimLeft();
// 		pDlg->m_strEditPH.TrimRight();
		var = rs->GetCollect("pspermmethod");
		pDlg->m_strEditSpermWay = (char*)(_bstr_t)var;
// 		pDlg->m_strEditSpermWay.TrimLeft();
// 		pDlg->m_strEditSpermWay.TrimRight();
		var = rs->GetCollect("pliquifytime");
		pDlg->m_strEditLiquifyTime = (char*)(_bstr_t)var;
// 		pDlg->m_strEditLiquifyTime.TrimLeft();
// 		pDlg->m_strEditLiquifyTime.TrimRight();
		pDlg->UpdateData(FALSE);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
	
	//
	str.Format("select * from morphasperminfo where pid like('%s%%')", pDlg->m_strEditDetectNO);
	try
	{
		_RecordsetPtr rs=theConnection->Execute((LPCTSTR)str,NULL,adCmdText);
		_variant_t vt;
		SingleSpermInfo ssInfo;

		pDlg->m_vSpermInfo.clear();

		while(!rs->EndOfFile)
		{
			vt = rs->GetCollect("x");
			ssInfo.pos.x = (long)vt;
			vt = rs->GetCollect("y");
			ssInfo.pos.y = (long)vt;

			vt = rs->GetCollect("pid");
			CString cs;
			cs = (char*)(_bstr_t)vt;

			int nl = cs.ReverseFind('_');
			nl = cs.GetLength()-nl-1;
			cs = cs.Right(nl);
			ssInfo.nSpermNO = atol((LPCTSTR)cs);

			vt = rs->GetCollect("IsNormal");
			ssInfo.bIsNormal = (bool)vt;

			vt = rs->GetCollect("m_length");
			ssInfo.smPara.m_length = (double)vt;
			vt = rs->GetCollect("m_width");
			ssInfo.smPara.m_width = (double)vt;
			vt = rs->GetCollect("m_area");
			ssInfo.smPara.m_area = (double)vt;
			
			vt = rs->GetCollect("m_ellipticity");
			ssInfo.smPara.m_ellipticity = (double)vt;
			vt = rs->GetCollect("m_perfor_area");
			ssInfo.smPara.m_perfor_area = (double)vt;
			vt = rs->GetCollect("m_head_area");
			ssInfo.smPara.m_head_area = (double)vt;
			vt = rs->GetCollect("m_perimeter");
			ssInfo.smPara.m_perimeter = (double)vt;
			vt = rs->GetCollect("m_head_perfor_area");
			ssInfo.smPara.m_head_perfor_area = (double)vt;
			
			vt = rs->GetCollect("m_tail_length");
			ssInfo.smPara.m_tail_length = (double)vt;
			vt = rs->GetCollect("m_tail_width");
			ssInfo.smPara.m_tail_width = (double)vt;
			vt = rs->GetCollect("m_tail_angle");
			ssInfo.smPara.m_tail_angle = (double)vt;

			vt = rs->GetCollect("m_extension");
			ssInfo.smPara.m_extension = (double)vt;
			vt = rs->GetCollect("m_symmetry");
			ssInfo.smPara.m_symmetry = (double)vt;
			vt = rs->GetCollect("m_ruga");
			ssInfo.smPara.m_ruga = (double)vt;
	
			pDlg->m_vSpermInfo.push_back(ssInfo);
			
			rs->MoveNext();

		}

		// Update UI button
		UINT uSelectedCount = m_wndMorphaDataList.GetSelectedCount();
		BOOL enable = uSelectedCount>=1 ? TRUE : FALSE;
		CDialog *pdlg = ((CMainFrame*)AfxGetMainWnd())->m_pWndMorphadlg;

		::SendMessage(pdlg->GetSafeHwnd(), USER_MSG_SHOW_PRINT_BTN, (WPARAM)enable, 0);
		::SendMessage(pdlg->GetSafeHwnd(), USER_MSG_SHOW_MOPHY_DEL_SEL_BTN, (WPARAM)enable, 0);

	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}

	*pResult = 0;
}
