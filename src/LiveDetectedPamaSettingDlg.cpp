// LiveDetectedPamaSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LiveDetectedPamaSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectedPamaSettingDlg dialog
extern CString theStrPathApp;

CLiveDetectedPamaSettingDlg::CLiveDetectedPamaSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveDetectedPamaSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLiveDetectedPamaSettingDlg)
	m_dDepth = 10.0;
	m_dStrThrd = .5;
	m_dLinThr  = .6;
	m_dCurThr  = .12;
	m_dV0Thr   = 10;
	m_dV1Thr   = 20;
	m_dV2Thr   = 40;
	m_nEnlargeRatio = 20;
	//}}AFX_DATA_INIT
}

void CLiveDetectedPamaSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveDetectedPamaSettingDlg)
	DDX_Text(pDX, IDC_DEPTH, m_dDepth);
	DDX_Text(pDX, IDC_STR_THRD, m_dStrThrd);
	DDX_Text(pDX, IDC_LIN_THRD, m_dLinThr);
	DDX_Text(pDX, IDC_CUR_THRD, m_dCurThr);
	DDX_Text(pDX, IDC_V0_THRD, m_dV0Thr);
	DDX_Text(pDX, IDC_V1_THRD, m_dV1Thr);
	DDX_Text(pDX, IDC_V2_THRD, m_dV2Thr);
	DDX_Text(pDX, IDC_ENLARGE_RATIO, m_nEnlargeRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLiveDetectedPamaSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLiveDetectedPamaSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectedPamaSettingDlg message handlers

void CLiveDetectedPamaSettingDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	WriteParamDataToFile();
	CDialog::OnOK();
}

void CLiveDetectedPamaSettingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CLiveDetectedPamaSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon,true);

	ReadInitParamDataFromFile();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLiveDetectedPamaSettingDlg::ReadInitParamDataFromFile()
{
	CString fileName = "LiveDetected_Parameter_Data.txt";
    CString sPath(theStrPathApp);
	sPath+= "\\config\\";
	CreateDirectory((LPCTSTR)sPath, NULL);
	
	CString  filePathAndName; 
	filePathAndName = sPath+fileName;
	
	fstream  fOut;
	try{
		fOut.open(filePathAndName, ios::in);
		fOut>>m_dDepth
			>>m_dStrThrd
			>>m_dLinThr
			>>m_dCurThr
			>>m_dV0Thr
			>>m_dV1Thr
			>>m_dV2Thr
			>>m_nEnlargeRatio;
		fOut.close();
	}
	catch (...) {
		
		MessageBox(_T("文件读入出错."), TEXT("ERROR"), MB_ICONERROR);
	}
}

void CLiveDetectedPamaSettingDlg::WriteParamDataToFile()
{
	CString fileName = "LiveDetected_Parameter_Data.txt";
    CString sPath(theStrPathApp);
	sPath+= "\\config\\";
	CreateDirectory((LPCTSTR)sPath, NULL);

	CString  filePathAndName; 
	filePathAndName = sPath+fileName;

	fstream  fOut;
	try{
		fOut.open(filePathAndName, ios::out|ios::trunc);
		fOut<<m_dDepth
			<<"\t"<<m_dStrThrd
			<<"\t"<<m_dLinThr
			<<"\t"<<m_dCurThr
			<<"\t"<<m_dV0Thr
			<<"\t"<<m_dV1Thr
			<<"\t"<<m_dV2Thr
			<<"\t"<<m_nEnlargeRatio;
		fOut.close();
	}
	catch (...) {
		MessageBox(_T("文件写入出错."), TEXT("ERROR"), MB_ICONERROR);
	}
}