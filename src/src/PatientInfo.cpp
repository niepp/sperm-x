// PatientInfo.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "PatientInfo.h"
#include "MainFrm.h"
#include "SpermView.h"
#include "View3.h"
#include "AllFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatientInfo dialog

extern CView3* theView3;
extern CString theStrPathApp;
extern _ConnectionPtr theConnection;

CPatientInfo::CPatientInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CPatientInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatientInfo)
	m_strColor = _T("");
	m_strMemory = _T("");
	m_strLiquifyState = _T("");
	m_strDetectNO = _T("");
	m_strName = _T("");
	m_strCaseNO = _T("");
	m_oleSpermGetDate = COleDateTime::GetCurrentTime();
	m_oleSpermGetTime = COleDateTime::GetCurrentTime();
	m_strSendDoctor = _T("");
	m_strDetectDoctor = _T("");
	m_strSampleNO = _T("");
	m_oleSpermDetectTime = COleDateTime::GetCurrentTime();
	m_oleSpermDetectDate = COleDateTime::GetCurrentTime();
	m_strSperVolume = _T("");
	m_strAbstinency = _T("");
	m_strAge = _T("");
	m_strCohension = _T("");
	m_strLiquifyTime = _T("");
	m_strPH = _T("");
	m_strRoomTempera = _T("");
	m_strShape = _T("");
	m_strSmell = _T("");
	m_strSpermWay = _T("");
	m_strThickness = _T("");
	m_strDilutionRatio = _T("1");
	//}}AFX_DATA_INIT
	m_strNameFromDataBase = _T("");
}


void CPatientInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatientInfo)
	DDX_Control(pDX, IDC_COMBO_SMELL, m_wndCbxSmell);
	DDX_Control(pDX, IDC_EDIT_NAME, m_wndEditName);
	DDX_Control(pDX, IDC_EDIT_DILUTIONRATIO, m_wndEditDilutionRatio);
	DDX_Control(pDX, IDC_EDIT_DETECTNO, m_wndEditDetectNO);
	DDX_Control(pDX, IDC_EDIT_CASENO, m_wndEdigCaseNO);
	DDX_Control(pDX, IDC_COMBO_AGE, m_wndCbxAge);
	DDX_Control(pDX, IDC_COMBO_THICKNESS, m_wndCbxThickness);
	DDX_Control(pDX, IDC_COMBO_SPERMWAY, m_wndCbxSpermWay);
	DDX_Control(pDX, IDC_COMBO_SHAPE, m_wndCbxShape);
	DDX_Control(pDX, IDC_COMBO_ROOMTEMPERA, m_wndCbxRoomTempera);
	DDX_Control(pDX, IDC_COMBO_LIQUIFYSTATE, m_wndCbxLiquify);
	DDX_Control(pDX, IDC_COMBO_PH, m_wndCbxPH);
	DDX_Control(pDX, IDC_COMBO_LIQUIFYTIME, m_wndCbxLiquifyTime);
	DDX_Control(pDX, IDC_COMBO_COHENSION, m_wndCbxCohension);
	DDX_Control(pDX, IDC_COMBO_ABSTINENCY, m_wndCbxAbstinency);
	DDX_Control(pDX, IDC_COMBO_SPERMVOLUME, m_wndCbxSpermVolume);
	
	DDX_Control(pDX, IDC_COMBO_SENDDOCTOR, m_wndCbxSendDoctor);
	DDX_Control(pDX, IDC_COMBO_DETECTDOCTOR, m_wndCbxDetectDoctor);

	DDX_Text(pDX, IDC_EDIT_SPERMCOLOR, m_strColor);
	DDX_Text(pDX, IDC_EDIT_MEMORY, m_strMemory);
	DDX_Text(pDX, IDC_EDIT_DETECTNO, m_strDetectNO);
	DDV_MaxChars(pDX, m_strDetectNO, 20);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CASENO, m_strCaseNO);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SPERMGETDATE, m_oleSpermGetDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SPERMGETTIME, m_oleSpermGetTime);
	
	DDX_Text(pDX, IDC_EDIT_SAMPLENO, m_strSampleNO);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETECTTIME, m_oleSpermDetectTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETECTDATE, m_oleSpermDetectDate);
	DDX_CBString(pDX, IDC_COMBO_SPERMVOLUME, m_strSperVolume);
	DDX_CBString(pDX, IDC_COMBO_ABSTINENCY, m_strAbstinency);
	DDX_CBString(pDX, IDC_COMBO_AGE, m_strAge);
	DDX_CBString(pDX, IDC_COMBO_COHENSION, m_strCohension);
	DDX_CBString(pDX, IDC_COMBO_LIQUIFYTIME, m_strLiquifyTime);
	DDX_CBString(pDX, IDC_COMBO_LIQUIFYSTATE, m_strLiquifyState);
	DDX_CBString(pDX, IDC_COMBO_PH, m_strPH);
	DDX_CBString(pDX, IDC_COMBO_ROOMTEMPERA, m_strRoomTempera);
	DDX_CBString(pDX, IDC_COMBO_SHAPE, m_strShape);
	DDX_CBString(pDX, IDC_COMBO_SMELL, m_strSmell);
	DDX_CBString(pDX, IDC_COMBO_SPERMWAY, m_strSpermWay);
	DDX_CBString(pDX, IDC_COMBO_THICKNESS, m_strThickness);

	DDX_CBString(pDX, IDC_COMBO_SENDDOCTOR, m_strSendDoctor);
	DDX_CBString(pDX, IDC_COMBO_DETECTDOCTOR, m_strDetectDoctor);
	DDX_Text(pDX, IDC_EDIT_DILUTIONRATIO, m_strDilutionRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatientInfo, CDialog)
	//{{AFX_MSG_MAP(CPatientInfo)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADD_DOCTOR, OnBtnAddDoctor)
	ON_EN_CHANGE(IDC_EDIT_CASENO, OnChangeEditCaseno)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatientInfo message handlers

BOOL CPatientInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon,TRUE);

	CString strSQLQuery("select pDetectNO from BasicInfo");

    try
	{		
		_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLQuery,NULL,adCmdText);
		int nCaseNO = 0, Maxv = 0;
		while(!rs->EndOfFile) {
			_variant_t var = rs->GetFields()->GetItem((long)0)->GetValue();
			CString cs =  (char*)(_bstr_t)var; //转换为字符串
			nCaseNO = atoi((LPCTSTR)cs);
			if(nCaseNO > Maxv ) Maxv = nCaseNO;
			rs->MoveNext();
		}
		m_strDetectNO.Format("%d", Maxv+1);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
	}

	UpdateData(FALSE);

	int i;
	for(i=15;i<=50;++i)
	{
		CString str;
		str.Format("%d",i);
		int res = m_wndCbxAge.InsertString(i-15,str);
		ASSERT( res!= CB_ERR );
	}
	m_wndCbxAge.SetCurSel(15);          // 30
	m_wndCbxSpermVolume.SetCurSel(7);   // 6 
	m_wndCbxLiquifyTime.SetCurSel(2);   // 20
	m_wndCbxAbstinency.SetCurSel(4);    // 5
    m_wndCbxRoomTempera.SetCurSel(4);   // 24
	m_wndCbxSpermWay.SetCurSel(0);      // 手淫
	m_wndCbxThickness.SetCurSel(1);     // 一般
	m_wndCbxCohension.SetCurSel(0);     // +
	m_wndCbxSmell.SetCurSel(0);         // 正常
	m_wndCbxShape.SetCurSel(0);         // 乳白
	m_wndCbxPH.SetCurSel(0);            // 6.5
    m_wndCbxLiquify.SetCurSel(0);       // 液化 
	ReadDataRecordToCombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatientInfo::OnOK()
{
	UpdateData(TRUE);
	if( m_strCaseNO == _T("") || m_strName == _T("") ) {
		MessageBox("样本号和姓名输入不能为空！");
		return;
	}

	CString strDetectDate,strDetectTime,strAge;
	strDetectTime.Format("%d:%d:%d",m_oleSpermDetectTime.GetHour(),m_oleSpermDetectTime.GetMinute(),
		                 m_oleSpermDetectTime.GetSecond());	
	strDetectDate.Format("%d/%d/%d ",m_oleSpermDetectDate.GetMonth(),m_oleSpermDetectDate.GetDay(),
		            m_oleSpermDetectDate.GetYear());
	strAge.Format("%d",GetDlgItemInt(IDC_COMBO_AGE));
	CString strBasicInfo,strSpermChait;
	strBasicInfo=CString("('")+m_strDetectNO+ "','"+m_strCaseNO+ "','"+
		m_strName+ "','"+strAge+"','"+strDetectDate+strDetectTime+"','"+
		m_strDetectDoctor+"','"+m_strSendDoctor+ "','"+m_strMemory+
		" ',' "+m_strSampleNO+"')";
	strBasicInfo = CString("insert into basicinfo values")+strBasicInfo;

	CString strSpermGetDate,strSpermGetTime;
	strSpermGetTime.Format("%d:%d:%d",m_oleSpermGetTime.GetHour(),m_oleSpermGetTime.GetMinute(),
		                 m_oleSpermGetTime.GetSecond());	
	strSpermGetDate.Format("%d/%d/%d ",m_oleSpermGetDate.GetMonth(),m_oleSpermGetDate.GetDay(),
		            m_oleSpermGetDate.GetYear());	
	strSpermChait=CString("insert into spermchait values('");
	strSpermChait+=m_strDetectNO+"','"+strSpermGetDate+strSpermGetTime+"','"+
		m_strAbstinency+"','"+m_strSpermWay+"','"+m_strSperVolume+"','"+
		m_strLiquifyTime+"','"+m_strShape+"','"+m_strDilutionRatio+"','"+m_strPH+
		"','"+m_strThickness+"','"+m_strSmell+"','"+m_strColor+"','"+m_strCohension+
		"','"+m_strLiquifyState+"','"+m_strRoomTempera+"')";
	try
	{
		theConnection->Execute((LPCTSTR)strBasicInfo,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSpermChait,NULL,adCmdText);
		CDialog::OnOK();
		CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
		CView3* pView=(CView3*)pMainFrm->GetView3();
		pView->m_strDetectNO=m_strDetectNO;
		pView->m_strPatientName=m_strName;
		pView->UpdateData(FALSE);

		pView->m_pwndLiveDetectDlg->m_dDilutionRatio = atof((LPCTSTR)m_strDilutionRatio);

	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
	}
}

void CPatientInfo::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);
}

void CPatientInfo::ReadDataRecordToCombox()
{
	CString strSQLSendDoctorList("select distinct strSendDoctor from doctor where strSendDoctor<>'' ");
	CString strSQLDetectDoctorList("select distinct strDetectDoctor from doctor where strDetectDoctor<>'' ");
	try
	{
		_RecordsetPtr rsSendDoctor, rsDetectDoctor;
		if(IsTableExist(theConnection, "doctor"))
		{
			rsSendDoctor = theConnection->Execute((LPCTSTR)strSQLSendDoctorList,NULL,adCmdText);
			rsDetectDoctor = theConnection->Execute((LPCTSTR)strSQLDetectDoctorList,NULL,adCmdText);
			_variant_t vt;
			while(!rsSendDoctor->EndOfFile)
			{
				 vt = rsSendDoctor->GetCollect("strSendDoctor");
				 m_wndCbxSendDoctor.AddString((char*)(_bstr_t)vt);
				 rsSendDoctor->MoveNext();
			}
			while(!rsDetectDoctor->EndOfFile)
			{
				 vt = rsDetectDoctor->GetCollect("strDetectDoctor");
				 m_wndCbxDetectDoctor.AddString((char*)(_bstr_t)vt);
				 rsDetectDoctor->MoveNext();
			}
		}
	}
	catch(_com_error& e)
	{
		MessageBox(e.Description());
	}
}

void CPatientInfo::OnBtnAddDoctor() 
{
	// TODO: Add your control notification handler code here
	CString strSendDoctor;
	CString strDetectDoctor;
	GetDlgItemText(IDC_COMBO_SENDDOCTOR, strSendDoctor);
	GetDlgItemText(IDC_COMBO_DETECTDOCTOR, strDetectDoctor);
	if( strSendDoctor=="" || strDetectDoctor=="" )
		AfxMessageBox("不能加入空的");

	CString strSQLInserDoctorList;
	strSQLInserDoctorList.Format("insert doctor values('%s','%s')", strSendDoctor, strDetectDoctor);
	try
	{
		if(IsTableExist(theConnection, "doctor"))
		{
			theConnection->Execute((LPCTSTR)strSQLInserDoctorList,NULL,adCmdText);
		}
	}
	catch(_com_error& e) 
	{
		AfxMessageBox(e.Description());
	}
}

void CPatientInfo::OnChangeEditCaseno() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
		
	UpdateData(TRUE);

	CString	strSQLQuery;
	strSQLQuery.Format("select * from BasicInfo, Spermchait where pCaseNO = '%s' \
		and BasicInfo.pDetectNO = Spermchait.pDetectNO", 
						m_strCaseNO);

	try
	{
		_variant_t var;
		_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLQuery, NULL, adCmdText);
		if(!rs->EndOfFile) {
			if(MessageBox(_T("该样本号已存在，是否直接用该样本?"),TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION) == IDYES )
			{
				_variant_t vt = rs->GetCollect((LPCSTR)CString("pDetectNO"));
				theView3->m_strDetectNO = (char*)(_bstr_t)vt;
				vt = rs->GetCollect((LPCSTR)CString("pName"));
				theView3->m_strPatientName = (char*)(_bstr_t)vt;

				vt = rs->GetCollect((LPCSTR)CString("pDilutionRatio"));
				CString cs = (char*)(_bstr_t)vt;
				theView3->m_pwndLiveDetectDlg->m_dDilutionRatio = atof((LPCTSTR)cs);

				theView3->UpdateData(FALSE);
				CDialog::OnOK();
			}
			else {
				m_strCaseNO = "";
				UpdateData(FALSE);
			}
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
}

void CPatientInfo::OnChangeEditName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if( m_strNameFromDataBase != _T("") && m_strNameFromDataBase != m_strName ) {
		if( MessageBox(_T("姓名输入有误，使用数据库中的姓名?"), TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION ) == IDYES )
		{
			 m_strName = m_strNameFromDataBase;
			 UpdateData(FALSE);
		}
	}	
}
