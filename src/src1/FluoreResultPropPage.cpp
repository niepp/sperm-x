// FluoreResultPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "FluoreResultPropPage.h"
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
// CFluoreResultPropPage property page

IMPLEMENT_DYNCREATE(CFluoreResultPropPage, CPropertyPage)

CFluoreResultPropPage::CFluoreResultPropPage() : CPropertyPage(CFluoreResultPropPage::IDD)
{
	//{{AFX_DATA_INIT(CFluoreResultPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nFluoreResultListIndex = -1;
}

CFluoreResultPropPage::~CFluoreResultPropPage()
{
}

void CFluoreResultPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFluoreResultPropPage)
	DDX_Control(pDX, IDC_LIST_FLUORE_RESULT, m_FluoreResultList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFluoreResultPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFluoreResultPropPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FLUORE_RESULT, OnClickListFluoreResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFluoreResultPropPage message handlers

BOOL CFluoreResultPropPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

void CFluoreResultPropPage::OnClickListFluoreResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView  = (NM_LISTVIEW*)pNMHDR; 

	m_nFluoreResultListIndex = pNMListView->iItem;

	CString strItemTextName = m_FluoreResultList.GetItemText(pNMListView->iItem, 0);
	if(strItemTextName == "" ) return;

	CString strItemTextCaseNO = m_FluoreResultList.GetItemText(pNMListView->iItem, 1);
	if(strItemTextCaseNO == "" ) return;

	CString strItemTextSpermDate = m_FluoreResultList.GetItemText(pNMListView->iItem, 2);
	if(strItemTextSpermDate == "" ) return;
	
	CCombinationAnalyseDlg& theDlg = *(((CMainFrame*) AfxGetMainWnd())->m_pWndCombinDlg);
	
	CString cs = m_FluoreResultList.GetItemText(pNMListView->iItem, 3);
	theDlg.m_FluoreData.spermBeforeNormalRatio = atof(cs);
	cs = m_FluoreResultList.GetItemText(pNMListView->iItem, 4);
	theDlg.m_FluoreData.spermAfterNormalRatio = atof(cs);
	theDlg.UpdateData(FALSE);

	/*
	CString strSQLQuery;
	strSQLQuery.Format("select distinct * from FluoreData \
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
		var = rs->GetCollect("pBeforeNormalRatio");
		theDlg.m_FluoreData.spermBeforeNormalRatio = (double)var;
		var = rs->GetCollect("pAfterNormalRatio");
		theDlg.m_FluoreData.spermAfterNormalRatio = (double)var;
		
		theDlg.m_chk_fluore = TRUE;
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
