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
	
CString   theStrPathApp; // ������ľ���·��

CString   theStr = "Software\\happynp@gmail.com\\admin";

CSpermApp::CSpermApp()
{
	//ȡ�����г���ľ���·��
	CString sPath;    
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); // �õ��ļ�����·�� 
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
	//  �������

	TEXT("pdetectno"),TEXT("����"),
	// �� spermnumstatics
	TEXT("pspermtotalnum"),TEXT("��������"),
	TEXT("pdetectedsperm"),TEXT("��⾫����"),
	TEXT("pactivespermnum"),TEXT("��Ծ������"),
	TEXT("pmovespermnum"),TEXT("�������"),
	TEXT("pfrontspermnum"),TEXT("ǰ���˶�������"),
	TEXT("pstraightspermnum"),TEXT("ֱ���˶�������"),
	TEXT("pcurvespermnum"),TEXT("�����˶�������"),
	TEXT("pspermtotalratio"),TEXT("�����ܱ���"),
	TEXT("pactivespermratio"),TEXT("��Ծ���ӱ���"),
	TEXT("pmovespermratio"),TEXT("����ӱ���"),
	TEXT("pfrontspermratio"),TEXT("ǰ���˶����ӱ���"),
	TEXT("pstraightspermratio"),TEXT("ֱ���˶����ӱ���"),
	TEXT("pcurvespermratio"),TEXT("�����˶����ӱ���"),

	TEXT("pspermtotaldensity"),TEXT("�������ܶ�"),
	TEXT("pactivespermdensity"),TEXT("��Ծ�����ܶ�"),
	TEXT("pmovespermdensity"),TEXT("������ܶ�"),
	TEXT("pfrontspermdensity"),TEXT("ǰ���˶������ܶ�"),
	TEXT("pstraightspermdensity"),TEXT("ֱ���˶������ܶ�"),
	TEXT("pcurvespermdensity"),TEXT("�����˶������ܶ�"),	 //  ��livenessresult
	TEXT("pspermdensity"),TEXT("�����ܶ�(��/ml)"),
	TEXT("pstraightspermnum"),TEXT("ǰ���˶�������"),
	TEXT("pcurvespermnum"),TEXT("�����˶�������"),
	TEXT("pactivespermnum"),TEXT("��Ծ������"),
	TEXT("pactivespermratio"),TEXT("��Ծ���ӱ���"),
	TEXT("pactivespermdensity"),TEXT("��Ծ�����ܶ�"),
	TEXT("plevelaratio"),TEXT("a�����ӱ���"),
	TEXT("plevelbratio"),TEXT("b�����ӱ���"),
	TEXT("plevelcratio"),TEXT("c�����ӱ���"),
	TEXT("pleveldratio"),TEXT("d�����ӱ���"),
	TEXT("pleveladensity"),TEXT("a�������ܶ�"),
	TEXT("plevelbdensity"),TEXT("b�������ܶ�"),
	TEXT("plevelcdensity"),TEXT("c�������ܶ�"),
	TEXT("plevelddensity"),TEXT("d�������ܶ�"),
	TEXT("plevelanum"),TEXT("a��������"),
	TEXT("plevelbnum"),TEXT("b��������"),
	TEXT("plevelcnum"),TEXT("c��������"),
	TEXT("pleveldnum"),TEXT("d��������"),
	// ��spermmovitypara
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
	//  ��̬ѧ���
	//  ��morpharesult
	TEXT("pdetectno"),TEXT("����"),
	TEXT("pnormalnum"),TEXT("������������"),
	TEXT("pabnormalnum"),TEXT("�쳣��������"),
	TEXT("normal_lengthNum"),TEXT("���ȴ�꾫����"),
	TEXT("normal_widthNum"),TEXT("��ȴ�꾫����"),
	TEXT("normal_areaNum"),TEXT("�����꾫����"),
	TEXT("normal_ellipticityNum"),TEXT("��Բ�ʴ�꾫����"),
	TEXT("normal_perfor_areaNum"),TEXT("���������꾫����"),
	TEXT("normal_head_areaNum"),TEXT("ͷ�������꾫����"),
	TEXT("normal_perimeterNum"),TEXT("�ܳ���꾫����"),
	TEXT("normal_head_perfor_areaNum"),TEXT("������ʴ�꾫����"),
	TEXT("normal_tail_lengthNum"),TEXT("β����꾫����"),
	TEXT("normal_tail_widthNum"),TEXT("β���꾫����"),
	TEXT("normal_tail_angleNum"),TEXT("����Ǵ�꾫����"),
	TEXT("normal_extensionNum"),TEXT("��չ�ȴ�꾫����"),
	TEXT("normal_symmetryNum"),TEXT("�Գƶȴ�꾫����"),
	TEXT("normal_rugaNum"),TEXT("���޶ȴ�꾫����"),
	TEXT("pheadabnormalnum"),TEXT("ͷ���쳣��������"),
	TEXT("pneckabnormalnum"),TEXT("�����쳣��������"),
	TEXT("ptailabnormalnum"),TEXT("β���쳣��������"),
	TEXT("pbighead"),TEXT("��ͷ��������"),
	TEXT("psmallhead"),TEXT("Сͷ��������"),
	TEXT("pbrushhead"),TEXT("ˢ״ͷ��������"),
	TEXT("pconehead"),TEXT("׶��ͷ��������"),
	TEXT("pcirhead"),TEXT("Բͷ��������"),
	TEXT("ppearhead"),TEXT("����ͷ��������"),
	TEXT("pnoshapehead"),TEXT("�޶���ͷ��������"),
	TEXT("psizeab"),TEXT("�ߴ��쳣��������"),
	TEXT("pangleab"),TEXT("�Ƕ��쳣��������"),
	TEXT("pinsab"),TEXT("�����쳣��������"),
	TEXT("pshorttail"),TEXT("��β��������"),
	TEXT("pirregular"),TEXT("�������ξ�������"),
	TEXT("pcirtail"),TEXT("��״β��������"),
	TEXT("pmultail"),TEXT("��β��������"),
	TEXT("pnotail"),TEXT("��β��������"),
	TEXT("pwind"),TEXT("������������"),
	TEXT("pperforab"),TEXT("�����쳣������Ŀ"),
	TEXT("psmallptab"),TEXT("����С����Ŀ"),
	NULL,NULL,
};

TCHAR* MorphaDataOrder[][2]=
{
	//  ��̬ѧ����
	//  ��MorphaSpermDataSet
	TEXT("pID"),TEXT("���ӱ��"),
	TEXT("IsNormal"),TEXT("����/�쳣"),
	TEXT("m_length"),TEXT("����"),
	TEXT("m_width"),TEXT("���"),
	TEXT("m_area"),TEXT("���"),
	TEXT("m_ellipticity"),TEXT("��Բ��"),
	TEXT("m_perfor_area"),TEXT("�������"),
	TEXT("m_head_area"),TEXT("ͷ�����"),
	TEXT("m_perimeter"),TEXT("�ܳ�"),
	TEXT("m_head_perfor_area"),TEXT("�������"),
	TEXT("m_tail_length"),TEXT("β��"),
	TEXT("m_tail_width"),TEXT("β��"),
	TEXT("m_tail_angle"),TEXT("�����"),
	TEXT("m_extension"),TEXT("��չ��"),
	TEXT("m_symmetry"),TEXT("�Գƶ�"),
	TEXT("m_ruga"),TEXT("���޶�"),
	NULL,NULL,
};

TCHAR* PatientInfoItemOrder[][2]=
{
	//  ������������
	//  ��PatientInfoListForQuery
	TEXT("pCaseNO"),TEXT("������"),
	TEXT("pName"),TEXT("����"),
	TEXT("pAge"),TEXT("����"),
	TEXT("pDetectNO"),TEXT("����"),
	TEXT("pSampleNO"),TEXT("������"),
	TEXT("pDeliverDoctor"),TEXT("�ͼ�ҽ��"),
	TEXT("pDetectDoctor"),TEXT("���ҽ��"),
	TEXT("pDetectDateTime"),TEXT("�������"),
	TEXT("pMemory"),TEXT("��ע"),	
	TEXT("pSpermGetDateTime"),TEXT("ȡ������"),
	TEXT("pCohesion"),TEXT("������"),
	TEXT("pColor"),TEXT("��ɫ"),
	TEXT("pDaysOfAbstinency"),TEXT("��������"),
	TEXT("pDilutionRatio"),TEXT("ϡ�ͱ�"),
	TEXT("pLiquifyState"),TEXT("Һ��״̬"),
	TEXT("pLiquifyTime"),TEXT("Һ��ʱ��"),
	TEXT("pPH"),TEXT("PHֵ"),
	TEXT("pRoomTempera"),TEXT("����"),
	TEXT("pShape"),TEXT("���"),
	TEXT("pSmell"),TEXT("��ζ"),
	TEXT("pSpermMethod"),TEXT("ȡ����ʽ"),
	TEXT("pSpermVolume"),TEXT("��Һ��"),
	TEXT("pThickness"),TEXT("ճ���"),
	NULL,NULL,
};

/////////////////////////////////////////////////////////////////////////////
// CSpermApp initialization

BOOL CSpermApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	// ������COM�����̲߳������::CoInitializeEx(NULL, COINIT_MULTITHREADED);  
	::CoInitialize(NULL); // ���߳������
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


