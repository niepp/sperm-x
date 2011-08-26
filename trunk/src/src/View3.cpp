// View3.cpp : implementation file
//

#include "stdafx.h"
#include "Sperm.h"
#include "View3.h"
#include "MainFrm.h"
#include "SpermView.h"
#include "PatientInfo.h"
#include "LiveDetectDLG.h"
#include "MorphaDetectDLG.h"
#include "OptionDlg.h"
#include "PatientInfoQueryDlg.h"
#include "ImageProcess.h"
extern CSpermView* theView1;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CView3* theView3;

IMPLEMENT_DYNCREATE(CView3, CFormView)
CView3::CView3()
	: CFormView(CView3::IDD)
{
	//{{AFX_DATA_INIT(CView3)
	m_strDetectNO = _T("");
	m_strPatientName = _T("");

	m_sTotalSpermNum = _T("");    // 视野中精子总数
	m_sMovementSpermNum = _T(""); // 活动精子数目
	m_sActiveSpermNum = _T("");	  // 活跃精子数目
	m_sGradeA = _T("");
	m_sGradeB = _T("");
	m_sGradeC = _T("");
	m_sGradeD = _T("");
	m_sLineMoveSpermNum   = _T(""); // 直线运动精子数
	m_sCurveMoveSpermNum  = _T(""); // 曲线运动精子数
	m_sActiveSpermDensity = _T(""); // 活跃精子密度
	m_sTotalSpermDensity  = _T(""); // 总精子密度
	m_sMovementSpermRatio = _T(""); // 活动精子比率，即存活率
	//}}AFX_DATA_INIT
	state=false;
	m_pwndLiveDetectDlg = new CLiveDetectDLG;
	m_pwndMorphaDetDlg  = new CMorphaDetectDLG;

}

CView3::~CView3()
{
	delete m_pwndLiveDetectDlg;
	delete m_pwndMorphaDetDlg;
}

void CView3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CView3)
	DDX_Text(pDX, IDC_EDIT_DETECTNO, m_strDetectNO);
	DDX_Text(pDX, IDC_EDIT_PATIENTNAME, m_strPatientName);
	
	DDX_Text(pDX, IDC_EDIT_TOT_SPERM_NUM, m_sTotalSpermNum);
	DDX_Text(pDX, IDC_EDIT_MOVE_SPERM_NUM, m_sMovementSpermNum);
	DDX_Text(pDX, IDC_EDIT_SPERM_DENSITY, m_sTotalSpermDensity);
	DDX_Text(pDX, IDC_EDIT_MOVE_RATIO, m_sMovementSpermRatio);
	DDX_Text(pDX, IDC_EDIT_A_CLASS, m_sGradeA);
	DDX_Text(pDX, IDC_EDIT_B_CLASS, m_sGradeB);
	DDX_Text(pDX, IDC_EDIT_C_CLASS, m_sGradeC);
	DDX_Text(pDX, IDC_EDIT_D_CLASS, m_sGradeD);
	DDX_Text(pDX, IDC_EDIT_LIN_NUM, m_sLineMoveSpermNum);
	DDX_Text(pDX, IDC_EDIT_CUR_NUM, m_sCurveMoveSpermNum);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_NUM, m_sActiveSpermNum);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_DENSITY, m_sActiveSpermDensity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CView3, CFormView)
	//{{AFX_MSG_MAP(CView3)
	ON_BN_CLICKED(IDC_BTN_LIVEDETECTED, OnBtnLiveDetected)

	ON_BN_CLICKED(IDC_PATIENT, OnPatient)
	ON_BN_CLICKED(IDC_BUTTON_MORPHADETECT, OnButtonMorphadetect)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_PATIENT_INFO_QUERY, OnBtnPatientInfoQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView3 diagnostics

#ifdef _DEBUG
void CView3::AssertValid() const
{
	CFormView::AssertValid();
}

void CView3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3 message handlers

void CView3::OnBtnLiveDetected() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CSpermView* pView1 = (CSpermView*)pFrame->GetView1();

	if(m_pwndLiveDetectDlg->m_pVideoPlay == NULL && !m_pwndLiveDetectDlg->m_cap.IsOpen() )
	{
		MessageBox("摄像头未打开且视频未载入!","文件打开错误",MB_ICONWARNING);
		return;
	}

	if(m_strPatientName==_T("") || m_strDetectNO == _T("") )
	{
		MessageBox("检测之前应输入病历","请首先输入病历",MB_ICONWARNING);
		return;
	}
	
	int xx = (int)m_eDetectMode | (int)CView3::AVI;
	m_eDetectMode = (CView3::DETECTMODE)xx;

	m_pwndLiveDetectDlg->m_SLDResult.strDetedNO = m_strDetectNO;

	if(m_pwndLiveDetectDlg->m_hWnd==NULL)
	{
		m_pwndLiveDetectDlg->Create(IDD_DIALOG_LIVEDETECT, this);
	}

	// 界面控制
	m_pwndLiveDetectDlg->m_bPamaSetting  = true;
	m_pwndLiveDetectDlg->m_bModeChange   = true;
	m_pwndLiveDetectDlg->m_bAutoAnalyse  = false;
	m_pwndLiveDetectDlg->m_bContinueDetected = false;
	m_pwndLiveDetectDlg->m_bFinishDetected   = false;
	m_pwndLiveDetectDlg->OnUpdateButtonState();
	m_pwndLiveDetectDlg->ShowWindow(SW_SHOW);
	m_pwndLiveDetectDlg->UpdateWindow();
	 
}

void CView3::OnPatient() 
{
	if(theView1->m_imgMorphaImg.GetImage() == NULL && !m_pwndLiveDetectDlg->m_cap.IsOpen() 
		&& m_pwndLiveDetectDlg->m_pVideoPlay == NULL )
	{
		MessageBox("摄像头未打开且视频,形态学图像未载入!","文件打开错误",MB_ICONWARNING);
		return;
	}
    CPatientInfo dlg;
	dlg.DoModal();	
	// 0331
}

void CView3::OnButtonMorphadetect() 
{
	// TODO: Add your control notification handler code here
	
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	IplImage* img=((CSpermView*)pMainFrm->GetView1())->m_imgMorphaImg.GetImage();
	
	if(theView1->m_imgMorphaImg.GetImage() == NULL && !m_pwndLiveDetectDlg->m_cap.IsOpen() )
	{
		MessageBox("摄像头未打开且形态学图像未载入!","文件打开错误",MB_ICONWARNING);
		return;
	}

	if(m_strPatientName==_T("") || m_strDetectNO == _T("") )
	{
		MessageBox("检测之前应输入病历","请首先输入病历",MB_ICONWARNING);
		return;
	}

	int xx;
	switch(theView1->m_nOpenOpt)
	{
	case LIVE_VIDEO_CAMERA:
		xx=(int)m_eDetectMode | (int)CView3::BMP;
		m_eDetectMode=(CView3::DETECTMODE)xx;
		break;
	case MORPHA: 
		if(img==NULL)
		{
			MessageBox("形态学分析图片尚未载入!","文件打开错误",MB_ICONWARNING);
			return;
		}
		break;
	}
	if(m_pwndMorphaDetDlg->m_hWnd==NULL)
		m_pwndMorphaDetDlg->Create(IDD_DIALOG_MORPHADETECT);
	m_pwndMorphaDetDlg->ShowWindow(SW_SHOW);
}

void CView3::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
    theView3 = this;
}

HBRUSH CView3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CView3::OnBtnPatientInfoQuery() 
{
	// TODO: Add your control notification handler code here
	CPatientInfoQueryDlg dlg;
	dlg.DoModal();
}
