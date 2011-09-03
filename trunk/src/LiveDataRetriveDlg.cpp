// LiveDataRetriveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LiveDataRetriveDlg.h"
#include "DataListCtrl.h"
#include "LivenessResultPage.h"
#include "PrinteDlg.h"
#include "AllFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLiveDataRetriveDlg dialog



extern _ConnectionPtr theConnection;
extern CString GetConnectIP();

CLiveDataRetriveDlg::CLiveDataRetriveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveDataRetriveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLiveDataRetriveDlg)
	m_strEditName = _T("");
	m_strEditDetectNO = _T("");
	m_oledateDetectDate = COleDateTime::GetCurrentTime();
	m_oletimeDetectTime = COleDateTime::GetCurrentTime();
	m_oledateSpermGetDate = COleDateTime::GetCurrentTime();
	m_oletimeSpermGetTime = COleDateTime::GetCurrentTime();
	m_strEditCaseNO = _T("");
	m_strEditDilutionRatio = _T("");
	m_strEditLiquifyState = _T("");
	m_strEditSampleNO = _T("");
	m_strEditSpermColor = _T("");
	m_oletimeTO = COleDateTime::GetCurrentTime();
	m_oletimeFROM = COleDateTime::GetCurrentTime();
	m_strEditInputDetectNO = _T("");
	m_strEditInputName = _T("");
	m_strEditOtherValue = _T("");
	m_strEditSmell = _T("");
	m_strEditSpermVolume = _T("");
	m_strEditCohension = _T("");
	m_strEditAbstinency = _T("");
	m_strEditAge = _T("");
	m_strEditLiquifyTime = _T("");
	m_strEditPH = _T("");
	m_strEditRoomTempera = _T("");
	m_strEditShape = _T("");
	m_strEditSpermWay = _T("");
	m_strEditThickness = _T("");
	m_BChkDetectDate = FALSE;
	m_BChkDetectNO = FALSE;
	m_BChkName = FALSE;
	m_BChkOther = FALSE;
	m_strCmBOtherSelection = _T("");
	m_strPageNum = _T("");
	m_pttype = 0;
	//}}AFX_DATA_INIT

	m_sheet.AddPage(&m_livenessPage);
	m_sheet.AddPage(&m_imagePage);

	m_ModifyStatus = WANTMODIFY;
	
	m_bBtnPreparePrint = false;
	
}

void CLiveDataRetriveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveDataRetriveDlg)
	DDX_Control(pDX, IDC_PAGENUM, m_wndPageEdit);
	DDX_Control(pDX, IDC_EDIT_INPUTDETECTNO, m_wndInputDetectNO);
	DDX_Control(pDX, IDC_BTN_PREPAGE, m_wndPrePage);
	DDX_Control(pDX, IDC_BTN_NEXTPAGE, m_wndNextPage);
	DDX_Control(pDX, IDC_BTN_LASTPAGE, m_wndLastPage);
	DDX_Control(pDX, IDC_BTN_FIRSTPAGE, m_wndFirstPage);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strEditName);
	DDX_Text(pDX, IDC_EDIT_DETECTNO, m_strEditDetectNO);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETECTDATE, m_oledateDetectDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETECTTIME, m_oletimeDetectTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SPERMGETDATE, m_oledateSpermGetDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SPERMGETTIME, m_oletimeSpermGetTime);
	DDX_Text(pDX, IDC_EDIT_CASENO, m_strEditCaseNO);
	DDX_Text(pDX, IDC_EDIT_DILUTIONRATIO, m_strEditDilutionRatio);
	DDX_Text(pDX, IDC_EDIT_LIQUIFYSTATE, m_strEditLiquifyState);
	DDX_Text(pDX, IDC_EDIT_SAMPLENO, m_strEditSampleNO);
	DDX_Text(pDX, IDC_EDIT_SPERMCOLOR, m_strEditSpermColor);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_INPUT_TO, m_oletimeTO);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_INPUT_FROM, m_oletimeFROM);
	DDX_Text(pDX, IDC_EDIT_INPUTDETECTNO, m_strEditInputDetectNO);
	DDX_Text(pDX, IDC_EDIT_INPUTNAME, m_strEditInputName);
	DDX_Text(pDX, IDC_EDIT_OTHERVALUE, m_strEditOtherValue);
	DDX_Text(pDX, IDC_EDIT_SMELL, m_strEditSmell);
	DDX_Text(pDX, IDC_EDIT_SPERMVOLUME, m_strEditSpermVolume);
	DDX_Text(pDX, IDC_EDIT_COHENSION, m_strEditCohension);
	DDX_Text(pDX, IDC_EDIT_ABSTINENCY, m_strEditAbstinency);
	DDX_Text(pDX, IDC_EDIT_AGE, m_strEditAge);
	DDX_Text(pDX, IDC_EDIT_LIQUIFYTIME, m_strEditLiquifyTime);
	DDX_Text(pDX, IDC_EDIT_PH, m_strEditPH);
	DDX_Text(pDX, IDC_EDIT_ROOMTEMPERA, m_strEditRoomTempera);
	DDX_Text(pDX, IDC_EDIT_SHAPE, m_strEditShape);
	DDX_Text(pDX, IDC_EDIT_SPERMWAY, m_strEditSpermWay);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_strEditThickness);
	DDX_Check(pDX, IDC_CHK_DETECTDATE, m_BChkDetectDate);
	DDX_Check(pDX, IDC_CHK_DETECTNO, m_BChkDetectNO);
	DDX_Check(pDX, IDC_CHK_NAME, m_BChkName);
	DDX_Check(pDX, IDC_CHK_OTHER, m_BChkOther);
	DDX_CBString(pDX, IDC_COMBO_OTHERSELECTION, m_strCmBOtherSelection);
	DDX_Text(pDX, IDC_PAGENUM, m_strPageNum);
	DDX_Radio(pDX, IDC_POOR, m_pttype);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLiveDataRetriveDlg, CDialog)
	//{{AFX_MSG_MAP(CLiveDataRetriveDlg)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_MORPHAINQUERY, OnButtonLiveinquery)
	ON_BN_CLICKED(IDC_BTN_FIRSTPAGE, OnBtnFirstpage)
	ON_BN_CLICKED(IDC_BTN_PREPAGE, OnBtnPrepage)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, OnBtnNextpage)
	ON_BN_CLICKED(IDC_BTN_LASTPAGE, OnBtnLastpage)
	ON_WM_LBUTTONDBLCLK()
	ON_EN_KILLFOCUS(IDC_EDIT_INPUTDETECTNO, OnKillfocusEditInputdetectno)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUTNAME, OnKillfocusEditInputname)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_INPUT_FROM, OnKillfocusDatetimepickerInputFrom)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_INPUT_TO, OnKillfocusDatetimepickerInputTo)
	ON_BN_CLICKED(IDC_CHK_DETECTDATE, OnChkDetectdate)
	ON_BN_CLICKED(IDC_CHK_DETECTNO, OnChkDetectno)
	ON_BN_CLICKED(IDC_CHK_NAME, OnChkName)
	ON_BN_CLICKED(IDC_CHK_OTHER, OnChkOther)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_BN_CLICKED(IDC_BTN_INFO_MODIFY, OnBtnInfoModify)
	ON_WM_KILLFOCUS()
	ON_EN_KILLFOCUS(IDC_EDIT_AGE, OnKillfocusEditAge)
	ON_EN_KILLFOCUS(IDC_EDIT_ABSTINENCY, OnKillfocusEditAbstinency)
	ON_EN_KILLFOCUS(IDC_EDIT_SPERMVOLUME, OnKillfocusEditSpermvolume)
	ON_EN_KILLFOCUS(IDC_EDIT_LIQUIFYTIME, OnKillfocusEditLiquifytime)
	ON_EN_KILLFOCUS(IDC_EDIT_DILUTIONRATIO, OnKillfocusEditDilutionratio)
	ON_EN_KILLFOCUS(IDC_EDIT_PH, OnKillfocusEditPh)
	ON_EN_KILLFOCUS(IDC_EDIT_ROOMTEMPERA, OnKillfocusEditRoomtempera)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_COMPARE_PRINT, OnBtnComparePrint)
	ON_BN_CLICKED(IDC_BTN_DEL_SEL, OnBtnDelSel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(USER_MSG_SHOW_COMPARE_PRINT_BTN, OnUserMsgComparePrintBtn)
	ON_MESSAGE(USER_MSG_SHOW_DEL_SEL_BTN, OnUserMsgDelSeltBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveDataRetriveDlg message handlers

BOOL CLiveDataRetriveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon,TRUE);

	CWnd* pWnd=GetDlgItem(IDC_STATIC_QUERYINFO);
	pWnd->ModifyStyleEx(0,WS_EX_CONTROLPARENT);

	m_sheet.Create(pWnd, WS_CHILD|WS_VISIBLE,WS_EX_CONTROLPARENT);

	CRect rect;
	pWnd->GetClientRect(rect);
	m_sheet.SetWindowPos(pWnd,0,0,rect.Width(),rect.Height()
		,SWP_NOZORDER | SWP_NOACTIVATE);

	GetDlgItem(IDC_BTN_PRINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_COMPARE_PRINT)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_DEL_SEL)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CLiveDataRetriveDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	switch(m_sheet.GetActiveIndex())
	{
	case 0:
		break;
	case 1:
		m_imagePage.GetScrollInfo(SB_VERT,&si);
		si.nPos+=-zDelta/4;
		if(si.nPos<=si.nMin) si.nPos = si.nMin;
		if(si.nPos>=si.nMax) si.nPos = si.nPos;
		m_imagePage.SetScrollInfo(SB_VERT,&si);
		m_imagePage.m_nVScrollPos = si.nPage;
		m_imagePage.m_wndDrawImage.Invalidate();
		break;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

CLiveDataRetriveDlg::~CLiveDataRetriveDlg()
{

}

void CLiveDataRetriveDlg::OnButtonLiveinquery() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_PRINT)->EnableWindow(FALSE);
    //  如果处于查询状态，则生成liveforinquery表
	if(IsInqueryState())
	{
			CString strSQLinfo("create table liveforinquery as select livefordisplay.*  from  \
					livefordisplay,basicinfo,spermchait \
					where livefordisplay.pdetectno = basicinfo.pdetectno \
					and   livefordisplay.pdetectno = spermchait.pdetectno ");
			if(m_BChkDetectNO | m_BChkName | m_BChkDetectDate | m_BChkOther)
			{
				if( m_BChkDetectNO )
				{
					m_strEditInputDetectNO.Replace('*','%');
					m_strEditInputDetectNO.Replace('?','_');
					strSQLinfo += CString(" and basicinfo.pdetectno like'")+
						m_strEditInputDetectNO+CString("' ");
				}
				if(m_BChkName)
				{
					m_strEditInputName.Replace('*','%');
					m_strEditInputName.Replace('?','_');	
					strSQLinfo += CString(" and pname like'")+m_strEditInputName
						+CString("'");
				}
				if(m_BChkDetectDate)
				{
					CString strfrom,strto;
					strfrom.Format("'%d-%02d-%02d'",m_oletimeFROM.GetYear()
						,m_oletimeFROM.GetMonth(),m_oletimeFROM.GetDay());
					strto.Format("'%d-%02d-%02d'",m_oletimeTO.GetYear()
						,m_oletimeTO.GetMonth(),m_oletimeTO.GetDay());	
					strSQLinfo += CString(" and pdetectdatetime between ")+
						strfrom+CString(" and ")+strto;
				}
			}
			try
			{
				if(IsTableExist(theConnection,"liveforinquery"))
				{
					CString dv("drop view liveforinquery");
					theConnection->Execute((LPCTSTR)dv,NULL,adCmdText);
				}
				theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);

				CString strN("select count(*) from liveforinquery");
				_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strN,NULL,adCmdText);
				int cn = rs->GetCollect((long)0).iVal;
				m_dp[1].nCurPage = 1;
				m_dp[1].nPageRecord = NUMPERPAGE;
				m_dp[1].nTotalRecord = cn;
				m_dp[1].nTotalPage = cn/ m_dp[1].nPageRecord + (cn%m_dp[1].nPageRecord!=0);
				if(&m_wndPageEdit == GetFocus())
				{
					CString pagenum;
					GetDlgItemText(IDC_PAGENUM,pagenum);
					int i = 0;
					for(i=0;i<pagenum.GetLength();++i)
						if(pagenum[i]<'0'||pagenum[i]>'9')
							break;
					if(pagenum.GetLength()==0 || i<pagenum.GetLength())	
					{
						CString info;
						info.Format("找不到 '%s' 页!",pagenum);
						MessageBox(info);
					}
					else
					{
						int cn = atoi((LPCTSTR)pagenum);
						if(cn > m_dp[1].nTotalPage || cn <= 0 )
						{
							CString info;
							info.Format("找不到 '%s' 页!",pagenum);
							MessageBox(info);
						}
						else
						{
							m_dp[1].nCurPage = cn;
						}
					}
				}
				if(cn == -1)
					return;
				CString str;
				str.Format("共查询到%d条记录。",cn);
				SetDlgItemText(IDC_STATIC_QUERY,str);	
				if(cn == 0) 
				{
					CString tempstr;
					tempstr.Format("select * from liveforinquery");
					m_livenessPage.m_wndLiveList.SetData(tempstr);
					ClearCtrls();
					return;
				}
				m_dp[1].nCurPage--;
				OnBtnNextpage();
			}
			catch (_com_error& e)
			{
				MessageBox(e.Description());
				return;
			}	
	}
	else   //  处于非查询状态
	{
		try
		{
			CString strN("select count(*) from livefordisplay");
			_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strN,NULL,adCmdText);
			int cn = rs->GetCollect((long)0).iVal;
			m_dp[0].nCurPage = 1;
			m_dp[0].nPageRecord = NUMPERPAGE;
			m_dp[0].nTotalRecord = cn;
			m_dp[0].nTotalPage = cn/ m_dp[0].nPageRecord + (cn%m_dp[0].nPageRecord!=0);
			if(cn == -1)
				return;
			CString str;
			str.Format("共查询到%d条记录。",cn);
			SetDlgItemText(IDC_STATIC_QUERY,str);	
			if(&m_wndPageEdit == GetFocus())
			{
				CString pagenum;
				GetDlgItemText(IDC_PAGENUM,pagenum);
				int i = 0;
				for(i=0;i<pagenum.GetLength();++i)
					if(pagenum[i]<'0'||pagenum[i]>'9')
						break;
				if(pagenum.GetLength()==0 || i<pagenum.GetLength())	
				{
					CString info;
					info.Format("找不到 '%s' 页!",pagenum);
					MessageBox(info);
				}
				else
				{
					int cn = atoi((LPCTSTR)pagenum);
					if(cn > m_dp[0].nTotalPage || cn <= 0 )
					{
						CString info;
						info.Format("找不到 '%s' 页!",pagenum);
						MessageBox(info);

					}
					else
					{
						m_dp[0].nCurPage = cn;
					}
				}
			}
			if(cn == 0) 
			{
				CString tempstr;
				tempstr.Format("select * from liveforinquery");
				m_livenessPage.m_wndLiveList.SetData(tempstr);
				ClearCtrls();
				return;
			}
			m_dp[0].nCurPage--;
			OnBtnNextpage();
		}
		catch (_com_error& e)
		{	
			MessageBox(e.Description());
			return;
		}
	}
}

void CLiveDataRetriveDlg::DeleteDetectNO(const CString &tablename)
{
	CString strSQLinfo("select livenessresult.pdetectno\
		from basicinfo,livenessresult,spermmovitypara,spermnumstatics\
		where livenessresult.pdetectno = spermmovitypara.pdetectno\
		and   livenessresult.pdetectno = spermnumstatics.pdetectno\
		and   livenessresult.pdetectno = basicinfo.pdetectno");
}

void CLiveDataRetriveDlg::GetPageBound(int curPage, int &low, int &up,CLiveDataRetriveDlg::DividePage& dp)
{
	ASSERT(curPage>=1 && curPage<= dp.nTotalPage);
	dp.nCurPage = curPage;
	low = (curPage-1)*dp.nPageRecord;
	up =  curPage*dp.nPageRecord;
	if(up > dp.nTotalRecord)
		up = dp.nTotalRecord;		
}

void CLiveDataRetriveDlg::OnBtnFirstpage() 
{
	// TODO: Add your control notification handler code here
	int lowRow,upRow;
	GetPageBound(1,lowRow,upRow,m_dp[IsInqueryState()]);
	_RecordsetPtr rs;
	m_livenessPage.GetRecordSet(rs,lowRow,upRow);
	int n = m_livenessPage.m_wndLiveList.SetData(rs);
	
	CString queryinfo;
	queryinfo.Format("第 %d / %d 页",m_dp[IsInqueryState()].nCurPage,m_dp[IsInqueryState()].nTotalPage);
	SetDlgItemText(IDC_PAGENUM,queryinfo);

	GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage);
	GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage );
}

void CLiveDataRetriveDlg::OnBtnPrepage() 
{
	int lowRow,upRow;
	GetPageBound(m_dp[IsInqueryState()].nCurPage - 1,lowRow,upRow,m_dp[IsInqueryState()]);
	_RecordsetPtr rs;
	m_livenessPage.GetRecordSet(rs,lowRow,upRow);
	int n = m_livenessPage.m_wndLiveList.SetData(rs);
	
	CString queryinfo;


	queryinfo.Format("第 %d / %d 页",m_dp[IsInqueryState()].nCurPage,m_dp[IsInqueryState()].nTotalPage);
	SetDlgItemText(IDC_PAGENUM,queryinfo);

	GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage);
	GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage );
	
}

void CLiveDataRetriveDlg::OnBtnNextpage() 
{
	int lowRow,upRow;
	GetPageBound(m_dp[IsInqueryState()].nCurPage + 1,lowRow,upRow,m_dp[IsInqueryState()]);
	_RecordsetPtr rs;
	m_livenessPage.GetRecordSet(rs,lowRow,upRow);
	int n = m_livenessPage.m_wndLiveList.SetData(rs);
	
	CString queryinfo;

	queryinfo.Format("第 %d / %d 页",m_dp[IsInqueryState()].nCurPage,m_dp[IsInqueryState()].nTotalPage);
	SetDlgItemText(IDC_PAGENUM,queryinfo);

	GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage);
	GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage );
	
}

void CLiveDataRetriveDlg::OnBtnLastpage() 
{
	int lowRow,upRow;

	GetPageBound(m_dp[IsInqueryState()].nTotalPage ,lowRow,upRow,m_dp[IsInqueryState()]);
	_RecordsetPtr rs;
	m_livenessPage.GetRecordSet(rs,lowRow,upRow);
	int n = m_livenessPage.m_wndLiveList.SetData(rs);
	
	CString queryinfo;

	queryinfo.Format("第 %d / %d 页",m_dp[IsInqueryState()].nCurPage,m_dp[IsInqueryState()].nTotalPage);
	SetDlgItemText(IDC_PAGENUM,queryinfo);

	GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != 1);
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage);
	GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(m_dp[IsInqueryState()].nCurPage != m_dp[IsInqueryState()].nTotalPage );
	
}

void CLiveDataRetriveDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

bool CLiveDataRetriveDlg::IsInqueryState()
{
	UpdateData();
	if(m_BChkDetectDate || m_BChkDetectNO || m_BChkName || m_BChkOther)
		return true;
	return false;
}

void CLiveDataRetriveDlg::ClearCtrls()
{
	SetDlgItemText(IDC_EDIT_NAME,TEXT(""));
	SetDlgItemText(IDC_EDIT_DETECTNO,TEXT(""));
	SetDlgItemText(IDC_EDIT_CASENO ,TEXT(""));
	SetDlgItemText(IDC_EDIT_SAMPLENO ,TEXT(""));
	SetDlgItemText(IDC_EDIT_SPERMCOLOR,TEXT(""));
	SetDlgItemText(IDC_EDIT_SMELL ,TEXT(""));
	SetDlgItemText(IDC_EDIT_SPERMVOLUME,TEXT(""));
	SetDlgItemText(IDC_EDIT_DILUTIONRATIO ,TEXT(""));
	SetDlgItemText(IDC_EDIT_COHENSION ,TEXT(""));
	SetDlgItemText(IDC_EDIT_THICKNESS,TEXT(""));
	SetDlgItemText(IDC_EDIT_AGE ,TEXT(""));
	SetDlgItemText(IDC_EDIT_SHAPE ,TEXT(""));
	SetDlgItemText(IDC_EDIT_ROOMTEMPERA ,TEXT(""));
	SetDlgItemText(IDC_EDIT_PH ,TEXT(""));
	SetDlgItemText(IDC_EDIT_ABSTINENCY ,TEXT(""));
	SetDlgItemText(IDC_EDIT_SPERMWAY ,TEXT(""));
	SetDlgItemText(IDC_EDIT_LIQUIFYSTATE ,TEXT(""));
	SetDlgItemText(IDC_EDIT_LIQUIFYTIME,TEXT(""));
	GetDlgItem(IDC_BTN_FIRSTPAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LASTPAGE)->EnableWindow(FALSE);
}

void CLiveDataRetriveDlg::OnKillfocusEditInputdetectno() 
{
	UpdateData(TRUE);
	if( !m_BChkDetectNO ) return;
	OnButtonLiveinquery();
}

void CLiveDataRetriveDlg::OnKillfocusEditInputname() 
{
	UpdateData(TRUE);
	if( !m_BChkName ) return;
	OnButtonLiveinquery();	
	
}

void CLiveDataRetriveDlg::OnKillfocusDatetimepickerInputFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	if( !m_BChkDetectDate ) return;
	OnButtonLiveinquery();
	*pResult = 0;
}

void CLiveDataRetriveDlg::OnKillfocusDatetimepickerInputTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	if( !m_BChkDetectDate ) return;
	OnButtonLiveinquery();
	*pResult = 0;
}

void CLiveDataRetriveDlg::OnChkDetectdate() 
{

	OnButtonLiveinquery();
	
}

void CLiveDataRetriveDlg::OnChkDetectno() 
{

	OnButtonLiveinquery();
	
}

void CLiveDataRetriveDlg::OnChkName() 
{

	OnButtonLiveinquery();	
}

void CLiveDataRetriveDlg::OnChkOther() 
{

	OnButtonLiveinquery();
	
}

void CLiveDataRetriveDlg::OnBtnPrint()
{
	int reportype = 3;
	switch(m_pttype)
	{
	case 0:
		reportype = 3;
		break;
	case 1:
		reportype = 4;
		break;
	default:
		reportype = 3;
	}

	CString strdetect(m_strEditDetectNO);
	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("data");
	parm.username = TEXT("");
	parm.passwd = TEXT("");
	CPrinteDlg cpld(reportype,parm,strdetect);
	cpld.DoModal();
}

void CLiveDataRetriveDlg::SaveModify()
{
	if( m_strEditDetectNO == _T("") ) return;
	CString strSQLinfo("update spermchait\n");
	strSQLinfo += CString("set pDaysOfAbstinency = ") + m_strEditAbstinency + CString(",\n");
	strSQLinfo += CString("pSpermMethod = '") + m_strEditSpermWay + CString("',\n");
	strSQLinfo += CString("pPH = ") + m_strEditPH + CString(",\n");
	strSQLinfo += CString("pThickness = '") + m_strEditThickness + CString("',\n");
	strSQLinfo += CString("pSpermVolume = ") + m_strEditSpermVolume + CString(",\n");
	strSQLinfo += CString("pShape = '") + m_strEditShape + CString("',\n");
	strSQLinfo += CString("pSmell = '") + m_strEditSmell + CString("',\n");
	strSQLinfo += CString("pColor = '") + m_strEditSpermColor + CString("',\n");
	strSQLinfo += CString("pCohesion = '") + m_strEditCohension + CString("',\n");
	strSQLinfo += CString("pLiquifyState = '") + m_strEditLiquifyState + CString("',\n");
	strSQLinfo += CString("pRoomTempera = ") + m_strEditRoomTempera + CString(",\n");
	strSQLinfo += CString("pLiquifyTime = ") + m_strEditLiquifyTime + CString(",\n");
	strSQLinfo += CString("pDilutionRatio = ") + m_strEditDilutionRatio + CString("\n");
	strSQLinfo += CString("where pDetectNO = '") + m_strEditDetectNO + CString("'");

	try {
		theConnection->Execute((LPCTSTR)strSQLinfo, NULL, adCmdText);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		DataBack();
		UpdateData(FALSE);
		return;
	}

	strSQLinfo = CString("update basicinfo\n");
	strSQLinfo += CString("set pCaseNO = '") + m_strEditCaseNO + CString("',\n");
	strSQLinfo += CString("pSampleNO = '") + m_strEditSampleNO + CString("',\n");
	strSQLinfo += CString("pName = '") + m_strEditName + CString("',\n");
	strSQLinfo += CString("pAge = ") + m_strEditAge + CString("\n");
	strSQLinfo += CString("where pDetectNO = '") + m_strEditDetectNO + CString("'");

// 	fp = fopen("sql_2.txt", "w");
// 	fprintf(fp, strSQLinfo);
// 	fclose(fp);
// 	MessageBox(strSQLinfo);

	try
	{
		theConnection->Execute((LPCTSTR)strSQLinfo, NULL, adCmdText);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		DataBack();
		UpdateData(FALSE);
		return;
	}		
}

void CLiveDataRetriveDlg::DataBack()
{
	CString strSQLinfo("select distinct * from basicinfo,spermchait\
		where basicinfo.pdetectno=spermchait.pdetectno and \
		basicinfo.pdetectno='");
	strSQLinfo= strSQLinfo + m_strEditDetectNO + CString("'");
	
	UpdateData();
	try
	{
		_variant_t var;
		COleDateTime pddt;
		_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);

		var = rs->GetCollect("pDetectno");
		m_strEditDetectNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pCaseNO");
		m_strEditCaseNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pname");
		m_strEditName = (char*)(_bstr_t)var;
		var = rs->GetCollect("pspermvolume");
		m_strEditSpermVolume = (char*)(_bstr_t)var;
		var = rs->GetCollect("page");
		m_strEditAge = (char*)(_bstr_t)var;
		pddt=rs->GetCollect("pdetectdatetime");
		m_oledateDetectDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		m_oletimeDetectTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pSampleNO");
		m_strEditSampleNO = (char*)(_bstr_t)var;
		var = rs->GetCollect("pdilutionratio");
		m_strEditDilutionRatio = (char*)(_bstr_t)var;
		var = rs->GetCollect("pShape");
		m_strEditShape = (char*)(_bstr_t)var;
		pddt=rs->GetCollect("pspermgetdatetime");
		m_oledateSpermGetDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		m_oletimeSpermGetTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pcolor");
		m_strEditSpermColor =  (char*)(_bstr_t)var;
		var = rs->GetCollect("pcohesion");
		m_strEditCohension =  (char*)(_bstr_t)var;
		var = rs->GetCollect("pRoomTempera");
		m_strEditRoomTempera = (char*)(_bstr_t)var;
		var = rs->GetCollect("pdaysofabstinency");
		m_strEditAbstinency = (char*)(_bstr_t)var;
		var = rs->GetCollect("pliquifystate");
		m_strEditLiquifyState = (char*)(_bstr_t)var;
		var = rs->GetCollect("psmell");
		m_strEditSmell = (char*)(_bstr_t)var;
		var = rs->GetCollect("pthickness");
		m_strEditThickness = (char*)(_bstr_t)var;
		var = rs->GetCollect("pph");
		m_strEditPH = (char*)(_bstr_t)var;
		var = rs->GetCollect("pspermmethod");
		m_strEditSpermWay = (char*)(_bstr_t)var;
		var = rs->GetCollect("pliquifytime");
		m_strEditLiquifyTime = (char*)(_bstr_t)var;
		UpdateData(FALSE);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
}

void CLiveDataRetriveDlg::OnBtnInfoModify() 
{
	// TODO: Add your control notification handler code here
	BOOL  bModify;

	if( m_ModifyStatus == WANTMODIFY ) {
		bModify = FALSE;
		SetDlgItemText(IDC_BTN_INFO_MODIFY, "保存");
		m_ModifyStatus = SAVEMODIFY;	
	}
	else {
		bModify = TRUE;
		UpdateData();
		SaveModify();
		SetDlgItemText(IDC_BTN_INFO_MODIFY, "修改");
		m_ModifyStatus = WANTMODIFY;
	}

	CEdit*   pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);   
		pEdit->SetReadOnly(bModify);  //这是读写。若为只读，将FALSE改为TRUE
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CASENO); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SAMPLENO); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SPERMCOLOR);
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SMELL); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SPERMVOLUME);
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DILUTIONRATIO); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COHENSION); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_THICKNESS);
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AGE); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHAPE); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ROOMTEMPERA );
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PH);
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ABSTINENCY); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SPERMWAY); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LIQUIFYSTATE); 
	    pEdit->SetReadOnly(bModify);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LIQUIFYTIME);
	    pEdit->SetReadOnly(bModify);

}

void CLiveDataRetriveDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
}

//#include "boost\regex.hpp"
void CLiveDataRetriveDlg::OnKillfocusEditAge() 
{
	// TODO: Add your control notification handler code here
	CString tmp(m_strEditAge);
	UpdateData();
// 	string regstr = "[1-9]\\d";
// 	boost::expression(regstr);
// 
// 	if( boost::regex_match(m_strEditAge, boost::expression) )
// 	{
// 		cout<<"OK\n";
// 	}
}

void CLiveDataRetriveDlg::OnKillfocusEditAbstinency() 
{
	// TODO: Add your control notification handler code here
	
}

void CLiveDataRetriveDlg::OnKillfocusEditSpermvolume() 
{
	// TODO: Add your control notification handler code here
	
}

void CLiveDataRetriveDlg::OnKillfocusEditLiquifytime() 
{
	// TODO: Add your control notification handler code here
	
}

void CLiveDataRetriveDlg::OnKillfocusEditDilutionratio() 
{
	// TODO: Add your control notification handler code here
	
}

void CLiveDataRetriveDlg::OnKillfocusEditPh() 
{
	// TODO: Add your control notification handler code here
	
}

void CLiveDataRetriveDlg::OnKillfocusEditRoomtempera() 
{
	// TODO: Add your control notification handler code here
	
}


void CLiveDataRetriveDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	CDialog::OnClose();
}

void CLiveDataRetriveDlg::ReadRecordFromDatabase(const CString &strSQLinfo)
{
	try
	{
		_variant_t var;
		COleDateTime pddt;
		_RecordsetPtr rs=theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);
	
		var = rs->GetCollect("pDetectno");
		m_strEditDetectNO = (char*)(_bstr_t)var;

		var = rs->GetCollect("pCaseNO");
		m_strEditCaseNO = (char*)(_bstr_t)var;

		var = rs->GetCollect("pname");
		m_strEditName = (char*)(_bstr_t)var;

		var = rs->GetCollect("pspermvolume");
		m_strEditSpermVolume = (char*)(_bstr_t)var;

		var = rs->GetCollect("page");
		m_strEditAge = (char*)(_bstr_t)var;

		pddt=rs->GetCollect("pdetectdatetime");
		m_oledateDetectDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		m_oletimeDetectTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pSampleNO");
		m_strEditSampleNO = (char*)(_bstr_t)var;

		var = rs->GetCollect("pdilutionratio");
		m_strEditDilutionRatio = (char*)(_bstr_t)var;

		var = rs->GetCollect("pShape");
		m_strEditShape = (char*)(_bstr_t)var;

		pddt=rs->GetCollect("pspermgetdatetime");
		m_oledateSpermGetDate.SetDate(pddt.GetYear(),pddt.GetMonth(),pddt.GetDay());
		m_oletimeSpermGetTime.SetTime(pddt.GetHour(),pddt.GetMinute(),pddt.GetSecond());
		var = rs->GetCollect("pcolor");
		m_strEditSpermColor =  (char*)(_bstr_t)var;

		var = rs->GetCollect("pcohesion");
		m_strEditCohension =  (char*)(_bstr_t)var;

		var = rs->GetCollect("pRoomTempera");
		m_strEditRoomTempera = (char*)(_bstr_t)var;

		var = rs->GetCollect("pdaysofabstinency");
		m_strEditAbstinency = (char*)(_bstr_t)var;

		var = rs->GetCollect("pliquifystate");
		m_strEditLiquifyState = (char*)(_bstr_t)var;

		var = rs->GetCollect("psmell");
		m_strEditSmell = (char*)(_bstr_t)var;

		var = rs->GetCollect("pthickness");
		m_strEditThickness = (char*)(_bstr_t)var;

		var = rs->GetCollect("pph");
		m_strEditPH = (char*)(_bstr_t)var;

		var = rs->GetCollect("pspermmethod");
		m_strEditSpermWay = (char*)(_bstr_t)var;

		var = rs->GetCollect("pliquifytime");
		m_strEditLiquifyTime = (char*)(_bstr_t)var;

		UpdateData(FALSE);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}

}

void CLiveDataRetriveDlg::OnBtnComparePrint() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pListCtrl = NULL;
	pListCtrl = dynamic_cast<CListCtrl*>(&m_livenessPage.m_wndLiveList);
	ASSERT(pListCtrl != NULL);
	
	UINT uSelectedCount = pListCtrl->GetSelectedCount();
	int  nFirst, nSecond;
	nFirst = nSecond = 1;
	// Update all of the selected items.
	if (uSelectedCount == 2)
	{
		nFirst = pListCtrl->GetNextItem(-1, LVNI_SELECTED);
		nSecond = pListCtrl->GetNextItem(nFirst, LVNI_SELECTED);
		// 
		LV_COLUMN  lc;
		lc.mask  = LVCF_TEXT ;
		TCHAR pszText[40];
		lc.pszText =  pszText;
		lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
		int nCol = 0;
		while (pListCtrl->GetColumn(nCol,&lc))
		{
			if(CString(lc.pszText) == TEXT("检测号"))
				break;
			++nCol;
		}
		CString str_first = pListCtrl->GetItemText(nFirst, nCol);
		CString str_second = pListCtrl->GetItemText(nSecond, nCol);

// 		CString str;
// 		str.Format("%d\t%d\n", nFirst, nSecond);
// 		str += str_first + "\n" + str_second;
// 		MessageBox(str);

		OnComparePrint(str_first, str_second);
		GetDlgItem(IDC_BTN_COMPARE_PRINT)->EnableWindow(FALSE);
	}
}

void CLiveDataRetriveDlg::OnComparePrint(CString wParam, CString lParam) 
{
	int reportype = 3; // 0 综合报告 1 形态学报告 2 活力分析报告 3 对比分析报告
	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("data");
	parm.username = TEXT("");
	parm.passwd = TEXT("");
	CPrinteDlg cpld(reportype, parm, wParam, lParam);
	cpld.DoModal();	
	
}

LRESULT CLiveDataRetriveDlg::OnUserMsgComparePrintBtn(WPARAM w, LPARAM l)
{
	BOOL enable = (BOOL)w;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_COMPARE_PRINT);
	pBtn->EnableWindow(enable);
	return S_OK;
}

LRESULT CLiveDataRetriveDlg::OnUserMsgDelSeltBtn(WPARAM w, LPARAM l)
{
	BOOL enable = (BOOL)w;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_DEL_SEL);
	pBtn->EnableWindow(enable);
	return S_OK;
}

void CLiveDataRetriveDlg::OnBtnDelSel() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确认删除所选记录?", "删除确认", MB_OKCANCEL | MB_ICONEXCLAMATION )!=IDOK)
	{
		return;
	}

	CListCtrl* pListCtrl = NULL;
	pListCtrl = dynamic_cast<CListCtrl*>(&m_livenessPage.m_wndLiveList);
	ASSERT(pListCtrl != NULL);
	
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		TRACE0("No items were selected!\n");
		MessageBox(_T("没有选中任何记录"));
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = pListCtrl->GetNextSelectedItem(pos);
			TRACE1("Item %d was selected!\n", nItem);
			// you could do your own processing on nItem here
			// delete all of the selected items.
			LV_COLUMN  lc;
			lc.mask  = LVCF_TEXT ;
			TCHAR pszText[40];
			lc.pszText =  pszText;
			lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
			int nCol = 0;
			while (pListCtrl->GetColumn(nCol,&lc))
			{
				if(CString(lc.pszText) == TEXT("检测号"))
					break;
				++nCol;
			}
			CString strKey = pListCtrl->GetItemText(nItem, nCol);
			delByKey(strKey);
		}
		
		OnButtonLiveinquery();
		ClearItem();
		GetDlgItem(IDC_BTN_PRINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COMPARE_PRINT)->EnableWindow(FALSE);

	}
}

void CLiveDataRetriveDlg::ClearItem()
{
	m_strEditName = _T("");
	m_strEditDetectNO = _T("");
	m_oledateDetectDate = COleDateTime::GetCurrentTime();
	m_oletimeDetectTime = COleDateTime::GetCurrentTime();
	m_oledateSpermGetDate = COleDateTime::GetCurrentTime();
	m_oletimeSpermGetTime = COleDateTime::GetCurrentTime();
	m_strEditCaseNO = _T("");
	m_strEditDilutionRatio = _T("");
	m_strEditLiquifyState = _T("");
	m_strEditSampleNO = _T("");
	m_strEditSpermColor = _T("");
	m_oletimeTO = COleDateTime::GetCurrentTime();
	m_oletimeFROM = COleDateTime::GetCurrentTime();
	m_strEditInputDetectNO = _T("");
	m_strEditInputName = _T("");
	m_strEditOtherValue = _T("");
	m_strEditSmell = _T("");
	m_strEditSpermVolume = _T("");
	m_strEditCohension = _T("");
	m_strEditAbstinency = _T("");
	m_strEditAge = _T("");
	m_strEditLiquifyTime = _T("");
	m_strEditPH = _T("");
	m_strEditRoomTempera = _T("");
	m_strEditShape = _T("");
	m_strEditSpermWay = _T("");
	m_strEditThickness = _T("");
	m_strCmBOtherSelection = _T("");
	m_strPageNum = _T("");

	UpdateData(FALSE);

}

void CLiveDataRetriveDlg::delByKey(const CString& strKey)
{
	CString Table_Arr[9] = {
			"livenessresult",
			"spermmovitypara",
			"spermnumstatics",
			"basicinfo",
			"spermchait",
			"image_table",
			"velDistribute",
			"resultkind",
			"cmpwithWHO"
	};
	CString strSQL;
	for(int i=0; i<9; i++)
	{
		strSQL.Format("delete from %s where pdetectNO = '%s'", Table_Arr[i],strKey);
		theConnection->Execute((LPCTSTR)strSQL, NULL, adCmdText);
	}
}
