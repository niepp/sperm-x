// SpermMorphaSet.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "SpermMorphaSet.h"
#include "MorphaDetectDLG.h"
#include "View3.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpermMorphaSet dialog


CSpermMorphaSet::CSpermMorphaSet(int index,CWnd* pParent /*=NULL*/)
	: CDialog(CSpermMorphaSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpermMorphaSet)
	m_nMorphaHead = -1;
	m_nMorphaPerfor = -1;
	m_nMorphaSmallPoint = -1;
	m_nMorphaTail = -1;
	//}}AFX_DATA_INIT
	
	m_nSpermIndex = index;
}


void CSpermMorphaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpermMorphaSet)
	DDX_Radio(pDX, IDC_RADIO_HEAD, m_nMorphaHead);
	DDX_Radio(pDX, IDC_RADIO_PERFOR, m_nMorphaPerfor);
	DDX_Radio(pDX, IDC_RADIO_SMALLPOINT, m_nMorphaSmallPoint);
	DDX_Radio(pDX, IDC_RADIO_TAIL, m_nMorphaTail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpermMorphaSet, CDialog)
	//{{AFX_MSG_MAP(CSpermMorphaSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpermMorphaSet message handlers

BOOL CSpermMorphaSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpermMorphaSet::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nSpermIndex == -1) return;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView= (CView3*)pMainFrm->GetView3();
	SperMopyParameter t=pView->m_pwndMorphaDetDlg->m_vSperMopyParameter[m_nSpermIndex];
	tagSpermMorphaNum& m_MhNum=pView->m_pwndMorphaDetDlg->m_MhNum;
	UpdateData(TRUE);
	t.m_headAb = m_nMorphaHead ;
	t.m_perforAb = m_nMorphaPerfor;
	t.m_smallpointAb = m_nMorphaSmallPoint;
	t.m_tailAb = m_nMorphaTail;
	//如果用户已经设置，按照用户设置修改参数
	if( m_nMorphaHead!=-1||
		m_nMorphaPerfor!=-1 ||
		m_nMorphaSmallPoint != -1 ||
		m_nMorphaTail !=-1
		)
	{
		// 用户标记为正常
		if( m_nMorphaHead == 7 &&
			m_nMorphaPerfor == 1 &&
			m_nMorphaTail == 6 &&
			m_nMorphaSmallPoint == 1)
		{
			int quantity=(pView->m_pwndMorphaDetDlg->m_vbIsNormal[m_nSpermIndex].IsNormalVector[0] !=TRUE );
			pView->m_pwndMorphaDetDlg->m_MhNum.NormalSpermNum += quantity;
			pView->m_pwndMorphaDetDlg->m_MhNum.AbnormalSpermNum -= quantity;
			pView->m_pwndMorphaDetDlg->m_MhNum.AbnormalPerforpermNum -= quantity;
			pView->m_pwndMorphaDetDlg->m_MhNum.HeadAbnormalSpermNum -= quantity;
			pView->m_pwndMorphaDetDlg->m_MhNum.TailAbnormalSpermNum -= quantity;
			
			pView->m_pwndMorphaDetDlg->m_vbIsNormal[m_nSpermIndex].IsNormalVector[0] = TRUE;
		}
		else  // 否则用户标记为异常
		{
			int quantity= (pView->m_pwndMorphaDetDlg->m_vbIsNormal[m_nSpermIndex].IsNormalVector[0] != FALSE );
			pView->m_pwndMorphaDetDlg->m_MhNum.NormalSpermNum -= quantity;
			pView->m_pwndMorphaDetDlg->m_MhNum.AbnormalSpermNum += quantity;
			pView->m_pwndMorphaDetDlg->m_vbIsNormal[m_nSpermIndex].IsNormalVector[0]= FALSE;
			switch(t.m_tailAb)
			{
			case 0:
				++m_MhNum.IrregularpermNum;
				break;
			case 1:
				++m_MhNum.ShortTailpermNum;
				break;
			case 2:
				++m_MhNum.CircleTailSpermNum;
				break;
			case 3:
				++m_MhNum.MultiTailSpermNum;
				break;
			case 4:
				++m_MhNum.NoTailSpermNum;
				break;
			case 5:
				++m_MhNum.WindSpermNum;
				break;
			default:
				break;
			}
			switch(t.m_headAb)
			{
			case 0:
				++m_MhNum.BigHeadSpermNum;
				break;
			case 1:
				++m_MhNum.SmallHeadSpermNum;
				break;
			case 2:
				++m_MhNum.CircleHeadSpermNum;
				break;
			case 3:
				++m_MhNum.ConeHeadSpermNum;
				break;
			case 4:
				++m_MhNum.PearHeadSpermNum;
				break;
			case 5:
				++m_MhNum.BrushHeadSpermNum;
				break;
			case 6:
				++m_MhNum.IrregularHeadSpermNum;
				break;
			default:
				break;
			}
			switch(t.m_perforAb)
			{
			case 0:
				++m_MhNum.AbnormalPerforpermNum;
				break;
			default:
				break;
			}
			switch(t.m_smallpointAb)
			{
			case 0:
				++m_MhNum.SmallPointAbSpermNum;
				break;
			default:
				break;
			}
		}
	}
	CMorphaDetectDLG*& pDlg=pView->m_pwndMorphaDetDlg;
	CString str;
	str.Format("目前一共检测了%d个视野,%d个精子,其中%d个正常,%d个异常.",
		pDlg->m_nVideoFiled,pDlg->m_MhNum.AbnormalSpermNum+pDlg->m_MhNum.NormalSpermNum,
		pDlg->m_MhNum.NormalSpermNum,pDlg->m_MhNum.AbnormalSpermNum);
	pDlg->GetDlgItem(IDC_DETECT_INFO)->SetWindowText(str);
	pDlg->UpdateData(FALSE);
	CDialog::OnOK();
}
