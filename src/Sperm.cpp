// Sperm.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sperm.h"
#include "logindlg.h"
#include "MainFrm.h"
#include "SpermDoc.h"
#include "SpermView.h"
#include "Splash.h"
#include "splash.h"
#include <initguid.h>
#include "LiveDetectDLG.h"
#include "Sperm_i.c"
#include "LoginThread.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpermApp

BEGIN_MESSAGE_MAP(CSpermApp, CWinApp)
	//{{AFX_MSG_MAP(CSpermApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpermApp construction
extern CEvent theEvent;
	
CString   theStrPathApp; // 本程序的绝对路径

CString   theStr = "Software\\happynp@gmail.com\\admin";

CSpermApp::CSpermApp()
{
	//取得运行程序的绝对路径
	CString sPath;    
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); // 得到文件绝对路径 
	sPath.ReleaseBuffer();
	int nPos;
	nPos  = sPath.ReverseFind (_T('\\'));
	sPath = sPath.Left(nPos);			
	theStrPathApp = sPath;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpermApp object

CSpermApp theApp;
_ConnectionPtr theConnection;

TCHAR* LiveColOrder[][2]=
{
	//  活力检测

	TEXT("pdetectno"),TEXT("检测号"),
	// 表 spermnumstatics
	TEXT("pspermtotalnum"),TEXT("精子总数"),
	TEXT("pdetectedsperm"),TEXT("检测精子数"),
	TEXT("pactivespermnum"),TEXT("活跃精子数"),
	TEXT("pmovespermnum"),TEXT("活动精子数"),
	TEXT("pfrontspermnum"),TEXT("前向运动精子数"),
	TEXT("pstraightspermnum"),TEXT("直线运动精子数"),
	TEXT("pcurvespermnum"),TEXT("曲线运动精子数"),
	TEXT("pspermtotalratio"),TEXT("精子总比率"),
	TEXT("pactivespermratio"),TEXT("活跃精子比率"),
	TEXT("pmovespermratio"),TEXT("活动精子比率"),
	TEXT("pfrontspermratio"),TEXT("前向运动精子比率"),
	TEXT("pstraightspermratio"),TEXT("直线运动精子比率"),
	TEXT("pcurvespermratio"),TEXT("曲线运动精子比率"),

	TEXT("pspermtotaldensity"),TEXT("精子总密度"),
	TEXT("pactivespermdensity"),TEXT("活跃精子密度"),
	TEXT("pmovespermdensity"),TEXT("活动精子密度"),
	TEXT("pfrontspermdensity"),TEXT("前向运动精子密度"),
	TEXT("pstraightspermdensity"),TEXT("直线运动精子密度"),
	TEXT("pcurvespermdensity"),TEXT("曲线运动精子密度"),	 //  表livenessresult
	TEXT("pspermdensity"),TEXT("精子密度(个/ml)"),
	TEXT("pstraightspermnum"),TEXT("前向运动精子数"),
	TEXT("pcurvespermnum"),TEXT("曲线运动精子数"),
	TEXT("pactivespermnum"),TEXT("活跃精子数"),
	TEXT("pactivespermratio"),TEXT("活跃精子比率"),
	TEXT("pactivespermdensity"),TEXT("活跃精子密度"),
	TEXT("plevelaratio"),TEXT("a级精子比率"),
	TEXT("plevelbratio"),TEXT("b级精子比率"),
	TEXT("plevelcratio"),TEXT("c级精子比率"),
	TEXT("pleveldratio"),TEXT("d级精子比率"),
	TEXT("pleveladensity"),TEXT("a级精子密度"),
	TEXT("plevelbdensity"),TEXT("b级精子密度"),
	TEXT("plevelcdensity"),TEXT("c级精子密度"),
	TEXT("plevelddensity"),TEXT("d级精子密度"),
	TEXT("plevelanum"),TEXT("a级精子数"),
	TEXT("plevelbnum"),TEXT("b级精子数"),
	TEXT("plevelcnum"),TEXT("c级精子数"),
	TEXT("pleveldnum"),TEXT("d级精子数"),
	// 表spermmovitypara
	TEXT("pvap"),TEXT("VAP"),
	TEXT("pvcl"),TEXT("VCL"),
	TEXT("pvsl"),TEXT("VSL"),
	TEXT("pwob"),TEXT("WOB"),
	TEXT("pstr"),TEXT("STR"),
	TEXT("plin"),TEXT("LIN"),
	TEXT("paln"),TEXT("ALH"),
	TEXT("pbcf"),TEXT("BCF"),

	NULL,NULL,
};

TCHAR* MorphaColOrder[][2]=
{
	//  形态学检测
	//  表morpharesult
	TEXT("pdetectno"),TEXT("检测号"),
	TEXT("pnormalnum"),TEXT("正常精子总数"),
	TEXT("pabnormalnum"),TEXT("异常精子总数"),
	TEXT("normal_lengthNum"),TEXT("长度达标精子数"),
	TEXT("normal_widthNum"),TEXT("宽度达标精子数"),
	TEXT("normal_areaNum"),TEXT("面积达标精子数"),
	TEXT("normal_ellipticityNum"),TEXT("椭圆率达标精子数"),
	TEXT("normal_perfor_areaNum"),TEXT("顶体面积达标精子数"),
	TEXT("normal_head_areaNum"),TEXT("头部面积达标精子数"),
	TEXT("normal_perimeterNum"),TEXT("周长达标精子数"),
	TEXT("normal_head_perfor_areaNum"),TEXT("顶体比率达标精子数"),
	TEXT("normal_tail_lengthNum"),TEXT("尾长达标精子数"),
	TEXT("normal_tail_widthNum"),TEXT("尾宽达标精子数"),
	TEXT("normal_tail_angleNum"),TEXT("插入角达标精子数"),
	TEXT("normal_extensionNum"),TEXT("伸展度达标精子数"),
	TEXT("normal_symmetryNum"),TEXT("对称度达标精子数"),
	TEXT("normal_rugaNum"),TEXT("皱褶度达标精子数"),
	TEXT("pheadabnormalnum"),TEXT("头部异常精子总数"),
	TEXT("pneckabnormalnum"),TEXT("颈部异常精子总数"),
	TEXT("ptailabnormalnum"),TEXT("尾部异常精子总数"),
	TEXT("pbighead"),TEXT("大头精子总数"),
	TEXT("psmallhead"),TEXT("小头精子总数"),
	TEXT("pbrushhead"),TEXT("刷状头精子总数"),
	TEXT("pconehead"),TEXT("锥形头精子总数"),
	TEXT("pcirhead"),TEXT("圆头精子总数"),
	TEXT("ppearhead"),TEXT("梨形头精子总数"),
	TEXT("pnoshapehead"),TEXT("无定形头精子总数"),
	TEXT("psizeab"),TEXT("尺寸异常精子总数"),
	TEXT("pangleab"),TEXT("角度异常精子总数"),
	TEXT("pinsab"),TEXT("插入异常精子总数"),
	TEXT("pshorttail"),TEXT("短尾精子总数"),
	TEXT("pirregular"),TEXT("不规则形镜子总数"),
	TEXT("pcirtail"),TEXT("环状尾精子总数"),
	TEXT("pmultail"),TEXT("多尾精子总数"),
	TEXT("pnotail"),TEXT("无尾精子总数"),
	TEXT("pwind"),TEXT("弯曲精子总数"),
	TEXT("pperforab"),TEXT("顶体异常精子数目"),
	TEXT("psmallptab"),TEXT("胞浆小滴数目"),
	NULL,NULL,
};

TCHAR* MorphaDataOrder[][2]=
{
	//  形态学数据
	//  表MorphaSpermDataSet
	TEXT("pID"),TEXT("精子编号"),
	TEXT("IsNormal"),TEXT("正常/异常"),
	TEXT("m_length"),TEXT("长度"),
	TEXT("m_width"),TEXT("宽度"),
	TEXT("m_area"),TEXT("面积"),
	TEXT("m_ellipticity"),TEXT("椭圆率"),
	TEXT("m_perfor_area"),TEXT("顶体面积"),
	TEXT("m_head_area"),TEXT("头部面积"),
	TEXT("m_perimeter"),TEXT("周长"),
	TEXT("m_head_perfor_area"),TEXT("顶体比率"),
	TEXT("m_tail_length"),TEXT("尾长"),
	TEXT("m_tail_width"),TEXT("尾宽"),
	TEXT("m_tail_angle"),TEXT("插入角"),
	TEXT("m_extension"),TEXT("伸展度"),
	TEXT("m_symmetry"),TEXT("对称度"),
	TEXT("m_ruga"),TEXT("皱褶度"),
	NULL,NULL,
};

TCHAR* PatientInfoItemOrder[][2]=
{
	//  病历样本数据
	//  表PatientInfoListForQuery
	TEXT("pCaseNO"),TEXT("样本号"),
	TEXT("pName"),TEXT("姓名"),
	TEXT("pAge"),TEXT("年龄"),
	TEXT("pDetectNO"),TEXT("检测号"),
	TEXT("pSampleNO"),TEXT("病历号"),
	TEXT("pDeliverDoctor"),TEXT("送检医生"),
	TEXT("pDetectDoctor"),TEXT("检测医生"),
	TEXT("pDetectDateTime"),TEXT("检测日期"),
	TEXT("pMemory"),TEXT("备注"),	
	TEXT("pSpermGetDateTime"),TEXT("取精日期"),
	TEXT("pCohesion"),TEXT("凝集度"),
	TEXT("pColor"),TEXT("颜色"),
	TEXT("pDaysOfAbstinency"),TEXT("禁欲天数"),
	TEXT("pDilutionRatio"),TEXT("稀释比"),
	TEXT("pLiquifyState"),TEXT("液化状态"),
	TEXT("pLiquifyTime"),TEXT("液化时间"),
	TEXT("pPH"),TEXT("PH值"),
	TEXT("pRoomTempera"),TEXT("室温"),
	TEXT("pShape"),TEXT("外观"),
	TEXT("pSmell"),TEXT("气味"),
	TEXT("pSpermMethod"),TEXT("取精方式"),
	TEXT("pSpermVolume"),TEXT("精液量"),
	TEXT("pThickness"),TEXT("粘稠度"),
	NULL,NULL,
};

/////////////////////////////////////////////////////////////////////////////
// CSpermApp initialization

BOOL CSpermApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	// 调用了COM得子线程才用这个::CoInitializeEx(NULL, COINIT_MULTITHREADED);  
	::CoInitialize(NULL); // 主线程用这个
	theConnection.CreateInstance("adodb.connection");
  	CLogInDlg Dialog;
  	if( Dialog.DoModal() == IDCANCEL)
  		return FALSE;
 	AfterLogin(0,0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSpermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSpermApp message handlers


BOOL CSpermApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

int CSpermApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	theConnection.Release();
	CoUninitialize();
	return CWinApp::ExitInstance();
}

	
CSpermModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CSpermModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CSpermModule::Lock()
{
	AfxOleLockApp();
	return 1;
}

LPCTSTR CSpermModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

LRESULT CSpermApp::AfterLogin(WPARAM w, LPARAM l)
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return S_OK;
	}
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);


	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSpermDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSpermView));
	AddDocTemplate(pDocTemplate);


 	if (!ProcessShellCommand(cmdInfo))
 		return S_FALSE;

	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	return S_OK;
}


