// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "OptionDlg.h"
#include "Spermview.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog
#include "View3.h"
#include "CaptureVideo.h"
extern CView3* theView3;
extern CSpermView* theView1;

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
	m_option = 2;
	//}}AFX_DATA_INIT
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_option);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

void COptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CSpermView *pview1 = ((CSpermView*)((CMainFrame*)AfxGetMainWnd())->GetView1());
	pview1->m_nOpenOpt = m_option;
	theView1->SetScrollSizes(MM_TEXT,CSize(100,100));
	BeginWaitCursor();
	switch(m_option)
	{
		case 0:
			if(((CComboBox*)GetDlgItem(IDC_CMB_DEVICE))->GetCount() <= 0 )
			{
				AfxMessageBox("请检查摄像机连线设置");
				return;
			}		
			theView3->GetDlgItem(IDC_BUTTON_MORPHADETECT)->EnableWindow(TRUE);
			theView3->GetDlgItem(IDC_BTN_LIVEDETECTED)->EnableWindow(TRUE);
			pview1->m_eViewShowMode = CSpermView::VIDEO_PLAY;
			theView3->m_eDetectMode = CView3::CAMERA; // 无法完全确定
			theView1->m_nDeviceID = ((CComboBox*)GetDlgItem(IDC_CMB_DEVICE))->GetCurSel();
			theView3->m_pwndLiveDetectDlg->m_cap.Init( ((CComboBox*)GetDlgItem(IDC_CMB_DEVICE))->GetCurSel(),pview1->GetSafeHwnd() );
			if(theView3->m_pwndLiveDetectDlg->m_pVideoPlay) {
				theView3->m_pwndLiveDetectDlg->m_pVideoPlay->OnVideoStop();
				delete theView3->m_pwndLiveDetectDlg->m_pVideoPlay;
				theView3->m_pwndLiveDetectDlg->m_pVideoPlay = NULL;
			}
			theView1->m_imgMorphaImg.Destroy();
		
			break;
		case 1:
			pview1->m_eViewShowMode = CSpermView::VIDEO_PLAY;
			theView3->m_eDetectMode = CView3::VIDEO;
			if(theView3->m_pwndLiveDetectDlg->m_cap.IsOpen())
				theView3->m_pwndLiveDetectDlg->m_cap.Stop();
			theView1->m_imgMorphaImg.Destroy();

			break;
		case 2:
			if(theView3->m_pwndLiveDetectDlg->m_pVideoPlay){
				theView3->m_pwndLiveDetectDlg->m_pVideoPlay->OnVideoStop();
				delete theView3->m_pwndLiveDetectDlg->m_pVideoPlay;
				theView3->m_pwndLiveDetectDlg->m_pVideoPlay = NULL;
			}
			if(theView3->m_pwndLiveDetectDlg->m_cap.IsOpen())
				theView3->m_pwndLiveDetectDlg->m_cap.Stop();
			theView3->m_eDetectMode = CView3::MORPHAIMAGE;
			break;
	}
	EndWaitCursor();
	theView1->m_eViewShowMode = CSpermView::INITIAL;
	theView1->Invalidate();

	CDialog::OnOK();
	
}

BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int n=((CSpermView*)((CMainFrame*)AfxGetMainWnd())->GetView1())->m_nOpenOpt;
	m_option=n;
	UpdateData(FALSE);	

	theView3->m_pwndLiveDetectDlg->m_cap.EnumDevices(GetDlgItem(IDC_CMB_DEVICE)->GetSafeHwnd());
	((CComboBox*)GetDlgItem(IDC_CMB_DEVICE))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
