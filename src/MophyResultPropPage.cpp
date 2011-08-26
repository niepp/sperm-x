// MophyResultPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MophyResultPropPage.h"
#include "CombinationAnalyseDlg.h"
#include "MainFrm.h"
#include "GloablFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern _ConnectionPtr theConnection;
/////////////////////////////////////////////////////////////////////////////
// CMophyResultPropPage property page

IMPLEMENT_DYNCREATE(CMophyResultPropPage, CPropertyPage)

CMophyResultPropPage::CMophyResultPropPage() : CPropertyPage(CMophyResultPropPage::IDD)
{
	//{{AFX_DATA_INIT(CMophyResultPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMophyResultListIndex = -1;
}

CMophyResultPropPage::~CMophyResultPropPage()
{
}

void CMophyResultPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMophyResultPropPage)
	DDX_Control(pDX, IDC_LIST_MOPHY_RESULT, m_MophyResultList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMophyResultPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMophyResultPropPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MOPHY_RESULT, OnClickListMophyResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMophyResultPropPage message handlers

BOOL CMophyResultPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMophyResultPropPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

void CMophyResultPropPage::OnClickListMophyResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView  = (NM_LISTVIEW*)pNMHDR; 

	m_nMophyResultListIndex = pNMListView->iItem;

	CString strItemTextName = m_MophyResultList.GetItemText(pNMListView->iItem, 0);
	if(strItemTextName == "" ) return;

	CString strItemTextCaseNO = m_MophyResultList.GetItemText(pNMListView->iItem, 1);
	if(strItemTextCaseNO == "" ) return;

	CString strItemTextSpermDate = m_MophyResultList.GetItemText(pNMListView->iItem, 2);
	if(strItemTextSpermDate == "" ) return;

	CCombinationAnalyseDlg& theDlg = *(((CMainFrame*) AfxGetMainWnd())->m_pWndCombinDlg);

	CString cs = m_MophyResultList.GetItemText(pNMListView->iItem, 3);
	theDlg.m_MophyData.spermTotalNormalRatio = atof(cs);	

	theDlg.UpdateData(FALSE);
	
	/*
	CString strSQLQuery;
	strSQLQuery.Format("select distinct * from MophyData \
						where pName = '%s' \
						and pCaseNO = '%s' \
						and YEAR(pDetectDateTime)  = '%s' \
						and MONTH(pDetectDateTime) = '%s' \
						and DAY(pDetectDateTime)   = '%s' ", 
						strItemTextName,
						strItemTextCaseNO,
						strItemTextSpermDate.Left(4),
						GetMonth_From_CString(strItemTextSpermDate),
						GetDay_From_CString(strItemTextSpermDate));

	CCombinationAnalyseDlg& theDlg = *(((CMainFrame*) AfxGetMainWnd())->m_pWndCombinDlg);

	try
	{
		_variant_t var;
		_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLQuery, NULL, adCmdText);	
		var = rs->GetCollect("pTotalNormalRatio");
		theDlg.m_MophyData.spermTotalNormalRatio = (double)var;
		theDlg.m_chk_mophy = TRUE;
		theDlg.UpdateData(FALSE);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
*/
	*pResult = 0;
}
