// CombinationAnalyseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "CombinationAnalyseDlg.h"
#include "PrinteDlg.h"
#include "AllFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCombinationAnalyseDlg dialog

extern CString theStrPathApp;
extern _ConnectionPtr theConnection;

void CCombinationAnalyseDlg::ClearVar()
{
	m_strEditShape = _T("");
	m_strEditSpermVolume = _T("");
	m_strEditSmell = _T("");
	m_strEditPH = _T("");
	m_strEditDilutionRatio = _T("");
	m_strEditLiquifyTime = _T("");
	m_strEditThickness = _T("");
	m_strEditCohension = _T("");
	m_strEditLiquifyState = _T("");
	m_strEditRoomTempera = _T("");
	m_strEditInputPatientNO = _T("");
	m_strEditInputName = _T("");
	m_strEditInputRptDoc = _T("");
	m_InputDateTime = COleDateTime::GetCurrentTime();

	m_chk_datetime = FALSE;

	strName = _T("");
}

CCombinationAnalyseDlg::CCombinationAnalyseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombinationAnalyseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCombinationAnalyseDlg)
	m_strEditInputPatientNO = _T("");
	m_strEditInputName = _T("");
	m_strEditInputRptDoc = _T("");
	m_InputDateTime = COleDateTime::GetCurrentTime();
	m_chk_datetime = FALSE;
	m_midu = 0.0;
	m_huolv = 0.0;
	m_fanyinlv = 0.0;
	m_wanzhenglv = 0.0;
	m_xingtailv = 0.0;
	m_sudu = 0.0;
	m_strDiagnostic = _T("\
男性生育力指数FI是反映男性生育能力的重要参考，FI越大表明生育能力越好：\r\n\
FI > 1 表明男性生育能力正常；\r\n\
FI = 0 表明完全没有生育能力；\r\n\
0 < FI < 1 表明存在不同程度的生育障碍.\n");
	//}}AFX_DATA_INIT
	m_shtPropertySheet.AddPage(&m_WndActiveResultPropPage);
	m_shtPropertySheet.AddPage(&m_WndMophyResultPropPage);
	m_shtPropertySheet.AddPage(&m_WndFluoreResultPropPage);

	m_dGenerateIndex = 0.0;

	m_bmidu = false;
	m_bhuolv = false;
	m_bsudu = false;
	m_bxingtailv = false;

	m_dzhishu = 0.0;

}

void CCombinationAnalyseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCombinationAnalyseDlg)
	DDX_Text(pDX, IDC_EDIT_INPUTPATIENTNO, m_strEditInputPatientNO);
	DDX_Text(pDX, IDC_EDIT_INPUTNAME, m_strEditInputName);
	DDX_Text(pDX, IDC_EDIT_RPT_DOC, m_strEditInputRptDoc);
	DDX_DateTimeCtrl(pDX, IDC_INPUT_DATETIMEPICKER, m_InputDateTime);
	DDX_Check(pDX, IDC_CHECK_DATETIME, m_chk_datetime);
	DDX_Text(pDX, IDC_EDIT_MIDU, m_midu);
	DDV_MinMaxDouble(pDX, m_midu, 0., 1000.);
	DDX_Text(pDX, IDC_EDIT_HUOLV, m_huolv);
	DDV_MinMaxDouble(pDX, m_huolv, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_FANYINLV, m_fanyinlv);
	DDV_MinMaxDouble(pDX, m_fanyinlv, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_WANZHENGLV, m_wanzhenglv);
	DDV_MinMaxDouble(pDX, m_wanzhenglv, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_XINGTAILV, m_xingtailv);
	DDV_MinMaxDouble(pDX, m_xingtailv, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_SUDU, m_sudu);
	DDV_MinMaxDouble(pDX, m_sudu, 0., 10000.);
	DDX_Text(pDX, IDC_EDIT_STR_DIAGNOSTIC, m_strDiagnostic);
	DDV_MaxChars(pDX, m_strDiagnostic, 1000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCombinationAnalyseDlg, CDialog)
	//{{AFX_MSG_MAP(CCombinationAnalyseDlg)
	ON_BN_CLICKED(IDC_BUTTON_INQUERY, OnBtnInquery)
	ON_BN_CLICKED(IDC_BTN_COMBIN_ANALYSE, OnBtnCombinAnalyse)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_DATETIME, OnCheckDatetime)
	ON_EN_CHANGE(IDC_EDIT_INPUTPATIENTNO, OnChangeEditInputpatientno)
	ON_WM_KILLFOCUS()
	ON_EN_CHANGE(IDC_EDIT_INPUTNAME, OnChangeEditInputname)
	ON_BN_CLICKED(IDC_BTN_COMPUT, OnBtnComput)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_MIDU, OnChangeEditMidu)
	ON_EN_CHANGE(IDC_EDIT_HUOLV, OnChangeEditHuolv)
	ON_EN_CHANGE(IDC_EDIT_SUDU, OnChangeEditSudu)
	ON_EN_CHANGE(IDC_EDIT_XINGTAILV, OnChangeEditXingtailv)
	ON_BN_CLICKED(IDC_BTN_RPT_PRINT, OnBtnRptPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCombinationAnalyseDlg message handlers

BOOL CCombinationAnalyseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd= GetDlgItem(IDC_SHEET_POS_STATIC);
	pWnd->ModifyStyleEx(0,WS_EX_CONTROLPARENT);

	m_shtPropertySheet.Create(pWnd,WS_CHILD|WS_VISIBLE,WS_EX_CONTROLPARENT);

	CRect rect;
	pWnd->GetClientRect(rect);
	m_shtPropertySheet.SetWindowPos( pWnd, 0, 0, rect.Width(), rect.Height(),
		                             SWP_NOZORDER |SWP_NOACTIVATE );
		
	GetDlgItem(IDC_BTN_COMPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RPT_PRINT)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

double CCombinationAnalyseDlg::ComputeGenerateIndex() 
{
	double FI = 0.0;
	if( abs(100-m_MophyData.spermTotalNormalRatio) <= 1e-3 ) return FI = 0.0;
	FI = m_ActiveData.moveSpermRatio*0.01 * m_ActiveData.spermTotalDensity*1000000 * m_ActiveData.movespeed*0.001 \
		/ ( 1000000 * (100-m_MophyData.spermTotalNormalRatio)*0.01 );
	return FI;
}

void CCombinationAnalyseDlg::OnBtnInquery()
{
	// TODO: Add your control notification handler code here
    // 基本理化指标
	CString strSQLCreateViewBasicData;
	strSQLCreateViewBasicData.Format("\
								create table BasicData \
								as select \
								BasicInfo.pName, BasicInfo.pCaseNO, BasicInfo.pDetectDateTime, \
								SpermChait.pShape, SpermChait.pSpermVolume, SpermChait.pSmell, SpermChait.pPH, SpermChait.pDilutionRatio, \
								SpermChait.pLiquifyTime, SpermChait.pThickness, SpermChait.pCohesion, SpermChait.pLiquifyState, SpermChait.pRoomTempera \
								from BasicInfo, SpermChait \
								where BasicInfo.pDetectNO = SpermChait.pDetectNO \
								and BasicInfo.pCaseNO = '%s' ", m_strEditInputPatientNO);
	
	// 常规分析数据
	CString strSQLCreateViewActiveData;
	strSQLCreateViewActiveData.Format("\
								create table ActiveData(pName, pCaseNO, pDetectDateTime, pSpermTotalDensity, pMoveSpermRatio, pFrontSpermRatio, pMoveSpeed) \
								as select \
								BasicInfo.pName, \
								BasicInfo.pCaseNO, \
								BasicInfo.pDetectDateTime, \
								SpermNumStatics.pSpermTotalDensity, \
								SpermNumStatics.pMoveSpermRatio*100, \
								SpermNumStatics.pFrontSpermRatio*100, \
								spermmovitypara.pVAP \
								from BasicInfo, SpermNumStatics, spermmovitypara \
								where BasicInfo.pDetectNO = SpermNumStatics.pDetectNO \
								and SpermNumStatics.pDetectNO = spermmovitypara.pDetectNO \
								and BasicInfo.pCaseNO = '%s' ", m_strEditInputPatientNO);

	// 形态学分析数据
	CString strSQLCreateViewMophyData;
	strSQLCreateViewMophyData.Format("\
								create table MophyData(pName, pCaseNO, pDetectDateTime, pTotalNormalRatio) \
								as select \
								BasicInfo.pName, \
								BasicInfo.pCaseNO, \
								BasicInfo.pDetectDateTime, \
								MorphaPartialRes.dTotalNormalRatio*100 \
								from BasicInfo, MorphaPartialRes \
								where BasicInfo.pDetectNO = MorphaPartialRes.pDetectNO \
								and BasicInfo.pCaseNO = '%s' ", m_strEditInputPatientNO);
	
	// 荧光染色分析数据
	CString strSQLCreateViewFluoreData;
	strSQLCreateViewFluoreData.Format(" \
								create table FluoreData(pName, pCaseNO, pDetectDateTime, pBeforeNormalRatio, pAfterNormalRatio) \
								as select \
								BasicInfo.pName, \
								BasicInfo.pCaseNO, \
								BasicInfo.pDetectDateTime, \
								CASE \
									WHEN FResult.BeforeNormalSpermNumbers=0 THEN CAST(0 as decimal(5,2)) \
									ELSE 100*(CAST(FResult.BeforeNormalSpermNumbers as decimal(5,2))) / ( CAST(FResult.BeforeNormalSpermNumbers as decimal(5,2))) \
								END , \
								CASE \
									WHEN FResult.AfterNormalSpermNumbers=0 THEN CAST(0 as decimal(5,2)) \
									ELSE 100*(CAST(FResult.AfterNormalSpermNumbers as decimal(5,2))) / ( CAST(FResult.AfterNormalSpermNumbers as decimal(5,2))) \
								END \
								from BasicInfo, FResult \
								where BasicInfo.pDetectNO = FResult.pDetectNO \
								and BasicInfo.pCaseNO = '%s' ", m_strEditInputPatientNO);

	try {		
		if( IsTableExist(theConnection,"BasicData") )
			theConnection->Execute((LPCTSTR)("drop table BasicData"), NULL, adCmdText);
		if( IsTableExist(theConnection,"ActiveData") )	
			theConnection->Execute((LPCTSTR)("drop table ActiveData"), NULL, adCmdText);
		if( IsTableExist(theConnection,"MophyData") )
			theConnection->Execute((LPCTSTR)("drop table MophyData"), NULL, adCmdText);
		if( IsTableExist(theConnection,"FluoreData") )
			theConnection->Execute((LPCTSTR)("drop table FluoreData"), NULL, adCmdText);
	}
	catch (_com_error& e)
	{	
		MessageBox(e.Description());
		return;
	}

	try {
		theConnection->Execute((LPCTSTR)strSQLCreateViewBasicData, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLCreateViewActiveData, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLCreateViewMophyData, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLCreateViewFluoreData, NULL, adCmdText);
	}
	catch (_com_error& e)
	{	
		MessageBox(e.Description());
		return;
	}

	CString strSQLQueryActiveData;
	CString strSQLQueryMophyData;
	CString strSQLQueryFluoreData;

	UpdateData(TRUE);

	if( m_chk_datetime == TRUE ) {
		strSQLQueryActiveData.Format("select * from ActiveData \
									where pName = '%s' \
									and pCaseNO = '%s' \
									and YEAR(pDetectDateTime) = '%d'\
									and MONTH(pDetectDateTime) = '%d'\
									and DAY(pDetectDateTime) = '%d'", 
									m_strEditInputName,
									m_strEditInputPatientNO,
									m_InputDateTime.GetYear(),
									m_InputDateTime.GetMonth(),
									m_InputDateTime.GetDay());
		strSQLQueryMophyData.Format("select * from MophyData \
									where pName = '%s' \
									and pCaseNO = '%s' \
									and YEAR(pDetectDateTime) = '%d'\
									and MONTH(pDetectDateTime) = '%d'\
									and DAY(pDetectDateTime) = '%d'", 
									m_strEditInputName,
									m_strEditInputPatientNO,
									m_InputDateTime.GetYear(),
									m_InputDateTime.GetMonth(),
									m_InputDateTime.GetDay());

		strSQLQueryFluoreData.Format("select * from FluoreData \
									where pName = '%s' \
									and pCaseNO = '%s' \
									and YEAR(pDetectDateTime) = '%d'\
									and MONTH(pDetectDateTime) = '%d'\
									and DAY(pDetectDateTime) = '%d'", 
									m_strEditInputName,
									m_strEditInputPatientNO,
									m_InputDateTime.GetYear(),
									m_InputDateTime.GetMonth(),
									m_InputDateTime.GetDay());
	}
	else {
		strSQLQueryActiveData.Format("select * from ActiveData \
									where pName = '%s' \
									and pCaseNO = '%s'", 
									m_strEditInputName,
									m_strEditInputPatientNO);
		strSQLQueryMophyData.Format("select * from MophyData \
									where pName = '%s' \
									and pCaseNO = '%s'", 
									m_strEditInputName,
									m_strEditInputPatientNO);

		strSQLQueryFluoreData.Format("select * from FluoreData \
									where pName = '%s' \
									and pCaseNO = '%s'", 
									m_strEditInputName,
									m_strEditInputPatientNO);
	}

	try {
		_RecordsetPtr rs;	
		m_shtPropertySheet.SetActivePage(&m_WndFluoreResultPropPage);
		rs = theConnection->Execute((LPCTSTR)strSQLQueryFluoreData, NULL, adCmdText);
		m_WndFluoreResultPropPage.m_FluoreResultList.SetListCtrlData(rs);

		m_shtPropertySheet.SetActivePage(&m_WndMophyResultPropPage);
		rs = theConnection->Execute((LPCTSTR)strSQLQueryMophyData, NULL, adCmdText);
		m_WndMophyResultPropPage.m_MophyResultList.SetListCtrlData(rs);

		m_shtPropertySheet.SetActivePage(&m_WndActiveResultPropPage);
		rs = theConnection->Execute((LPCTSTR)strSQLQueryActiveData, NULL, adCmdText);
		m_WndActiveResultPropPage.m_ActiveResultList.SetListCtrlData(rs);
	}
	catch (_com_error& e)
	{	
		MessageBox(e.Description());
		return;
	}
}

extern CString GetConnectIP();

void CCombinationAnalyseDlg::OnBtnCombinAnalyse() 
{
	// TODO: Add your control notification handler code here
	if(  ( m_WndActiveResultPropPage.m_nActiveResultListIndex >=0 )
	  && ( m_WndMophyResultPropPage.m_nMophyResultListIndex   >=0 )
	 )
	{

		m_dGenerateIndex = ComputeGenerateIndex();

		UpdateData(TRUE);

		// 写到数据库
		CString strSQLCombinationResult(" \
								create table CombinationResult( \
								pCaseNO varchar(20) Not Null,  \
								pName varchar(20), \
								pRptDoctor varchar(20),\
								pDensitye decimal(20,3), \
								pMoveRatio decimal(20,3), \
								pFrontRatio decimal(20,3), \
								pMoveSpeed decimal(20,3), \
								pNormalRatio decimal(20,3), \
								pBeforeNormalRatio decimal(20,3), \
								pAfterNormalRatio decimal(20,3), \
								pGenerateIndex decimal(20,3), \
								pDiagnostic char(1000))");

		CString strDate;
		strDate.Format("%d/%d/%d",
						m_InputDateTime.GetMonth(),
						m_InputDateTime.GetDay(), 
						m_InputDateTime.GetYear());
		CString strSQLRecord;
		strSQLRecord.Format(" \
			insert into CombinationResult values( \
											'%s', \
											'%s', \
											'%s', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%lf', \
											'%s')",
											m_strEditInputPatientNO,
											m_strEditInputName,
											m_strEditInputRptDoc,
											m_ActiveData.spermTotalDensity,
											m_ActiveData.moveSpermRatio,
											m_ActiveData.frontSpermRatio,
                                            m_ActiveData.movespeed,
											m_MophyData.spermTotalNormalRatio,
											m_FluoreData.spermBeforeNormalRatio,
											m_FluoreData.spermAfterNormalRatio,
											m_dGenerateIndex,
											m_strDiagnostic);
		if(m_strEditInputPatientNO=="" || m_strEditInputName=="")
		{
			MessageBox("样本号和姓名不能为空!","错误",MB_ICONWARNING);
			return;
		}

		if( IsTableExist(theConnection,"CombinationResult") ) {
			try {
				theConnection->Execute((LPCTSTR)("drop table CombinationResult"), NULL, adCmdText);
			}
			catch (_com_error& e)
			{	
				MessageBox(e.Description());
				return;
			}
		}
		try {
			theConnection->Execute((LPCTSTR)strSQLCombinationResult, NULL, adCmdText);
			theConnection->Execute((LPCTSTR)strSQLRecord, NULL, adCmdText);
		}
		catch (_com_error& e)
		{	
			MessageBox(e.Description());
			return;
		}

		int reportype = 0; // 0 综合分析 1 多头 2 活力
		CString strdetect(m_strEditInputPatientNO);
		crxparm parm;
		parm.ipadd = TEXT( GetConnectIP() );
		parm.database = TEXT("sperm");
		parm.username = TEXT("sa");
		parm.passwd = TEXT("sa");
		
		CPrinteDlg cpld(reportype,parm,strdetect);
		cpld.DoModal();
		
	}
	else{
		if( m_WndActiveResultPropPage.m_nActiveResultListIndex < 0 ) {
			MessageBox("缺少活力分析的数据");
		}
		else if( m_WndMophyResultPropPage.m_nMophyResultListIndex < 0 ) {
			MessageBox("缺少形态学分析的数据");
		}
	}

}

void CCombinationAnalyseDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ClearVar();
	DestroyWindow();
	CDialog::OnClose();
}

void CCombinationAnalyseDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnClose();
}

void CCombinationAnalyseDlg::OnCheckDatetime() 
{
	// TODO: Add your control notification handler code here
	if( m_chk_datetime == FALSE ) m_chk_datetime = TRUE;
	else m_chk_datetime = FALSE;
}

void CCombinationAnalyseDlg::OnChangeEditInputpatientno() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	CString	strSQLQuery;
	strSQLQuery.Format("select * from ActiveData \
						where pCaseNO = '%s' ", 
						m_strEditInputPatientNO);
	if( IsTableExist(theConnection,"ActiveData") ) {
		try
		{
			
			_variant_t var;
			_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLQuery, NULL, adCmdText);
			if(!rs->EndOfFile) {
				var = rs->GetCollect("pName");
				strName = m_strEditInputName = (char*)(_bstr_t)var;
				UpdateData(FALSE);
			}
			else {
				strName = m_strEditInputName = _T("");
				UpdateData(FALSE);
			}
		}
		catch(_com_error& e)
		{
			AfxMessageBox(e.Description());
			return;
		}
	}
}

void CCombinationAnalyseDlg::OnChangeEditInputname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if( strName != _T("") && strName != m_strEditInputName ) {
		if( MessageBox(_T("姓名输入有误，使用数据库中的姓名?"), TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION ) == IDYES )
		{
			 m_strEditInputName = strName;
			 UpdateData(FALSE);
		}
	}	

}

void CCombinationAnalyseDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code heres	
}


void CCombinationAnalyseDlg::OnBtnComput() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	double v = 0;
	if(abs(100-m_xingtailv)<1e-3) v = 0;
	else v = (m_huolv*0.01) * (m_midu * 1000000) * (m_sudu*0.001) / ((100-m_xingtailv)*0.01 * 1000000);
	m_dzhishu = v;
	CString cs;
	cs.Format("%.2lf", v);
	SetDlgItemText(IDC_STATIC_ZHISHU, cs);
	GetDlgItem(IDC_BTN_RPT_PRINT)->EnableWindow(TRUE);
}

void CCombinationAnalyseDlg::OnBtnRptPrint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// 写到数据库
	CString strSQLCombinationResult(" \
										create table CombinationResult( \
										pCaseNO varchar(20) Not Null, \
										pName varchar(20),\
										pRptDoctor varchar(20),\
										pDensitye decimal(20,3), \
										pMoveRatio decimal(20,3), \
										pMoveSpeed decimal(20,3), \
										pNormalRatio decimal(20,3), \
										pBeforeNormalRatio decimal(20,3), \
										pAfterNormalRatio decimal(20,3), \
										pGenerateIndex decimal(20,3), \
										pDiagnostic char(1000))");

	CString strSQLRecord;
	strSQLRecord.Format(" \
							insert into CombinationResult values( \
							'%s', \
							'%s', \
							'%s', \
							'%lf', \
							'%lf', \
							'%lf', \
							'%lf', \
							'%lf', \
							'%lf', \
							'%lf', \
							'%s')",
							m_strEditInputPatientNO,
							m_strEditInputName,
							m_strEditInputRptDoc,
							m_midu,
							m_huolv,
							m_sudu,
							m_xingtailv,
							m_wanzhenglv,
							m_fanyinlv,
							m_dzhishu,
							m_strDiagnostic);
	
	if( IsTableExist(theConnection,"CombinationResult") ) {
		try {
			theConnection->Execute((LPCTSTR)("drop table CombinationResult"), NULL, adCmdText);
		}
		catch (_com_error& e)
		{	
			MessageBox(e.Description());
			return;
		}
	}
	try {
		theConnection->Execute((LPCTSTR)strSQLCombinationResult, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLRecord, NULL, adCmdText);
	}
	catch (_com_error& e)
	{	
		MessageBox(e.Description());
		return;
	}

	int reportype = 0; // 0 综合分析 1 多头 2 活力
	CString strdetect(m_strEditInputPatientNO);
	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("sperm");
	parm.username = TEXT("sa");
	parm.passwd = TEXT("sa");
	
	CPrinteDlg cpld(reportype,parm,strdetect);
	cpld.DoModal();	
}

HBRUSH CCombinationAnalyseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{   
		COLORREF color = RGB(255,0,0);
		UINT IDArray[] = {
				IDC_STATIC_1,
				IDC_STATIC_2,
				IDC_STATIC_3,
				IDC_STATIC_4,
				IDC_STATIC_5
		};
		for(int k=0; k<sizeof(IDArray)/sizeof(UINT); k++)
		{
			if( pWnd->GetDlgCtrlID() == IDArray[k] )
			{				
				pDC->SetTextColor(color);
			}		
		}
	}  
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CCombinationAnalyseDlg::OnChangeEditMidu() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bmidu = true;
	if(m_bmidu && m_bhuolv && m_bsudu && m_bxingtailv) 
		GetDlgItem(IDC_BTN_COMPUT)->EnableWindow(TRUE);
}

void CCombinationAnalyseDlg::OnChangeEditHuolv() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bhuolv = true;
	if(m_bmidu && m_bhuolv && m_bsudu && m_bxingtailv) 
		GetDlgItem(IDC_BTN_COMPUT)->EnableWindow(TRUE);
}

void CCombinationAnalyseDlg::OnChangeEditSudu() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bsudu = true;
	if(m_bmidu && m_bhuolv && m_bsudu && m_bxingtailv) 
		GetDlgItem(IDC_BTN_COMPUT)->EnableWindow(TRUE);
}

void CCombinationAnalyseDlg::OnChangeEditXingtailv() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_bxingtailv = true;
	if(m_bmidu && m_bhuolv && m_bsudu && m_bxingtailv) 
		GetDlgItem(IDC_BTN_COMPUT)->EnableWindow(TRUE);
}

