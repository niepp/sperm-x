// ActiveResultPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "ActiveResultPropPage.h"
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
// CActiveResultPropPage property page

IMPLEMENT_DYNCREATE(CActiveResultPropPage, CPropertyPage)

CActiveResultPropPage::CActiveResultPropPage() : CPropertyPage(CActiveResultPropPage::IDD)
{
	//{{AFX_DATA_INIT(CActiveResultPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nActiveResultListIndex = -1;
}

CActiveResultPropPage::~CActiveResultPropPage()
{
}

void CActiveResultPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActiveResultPropPage)
	DDX_Control(pDX, IDC_LIST_ACTIVE_RESULT, m_ActiveResultList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CActiveResultPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CActiveResultPropPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACTIVE_RESULT, OnClickListActiveResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveResultPropPage message handlers

void CActiveResultPropPage::OnClickListActiveResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView  = (NM_LISTVIEW*)pNMHDR; 

	m_nActiveResultListIndex = pNMListView->iItem;

	CString strItemTextName = m_ActiveResultList.GetItemText(pNMListView->iItem, 0);
	if(strItemTextName == "" ) return;

	CString strItemTextCaseNO = m_ActiveResultList.GetItemText(pNMListView->iItem, 1);
	if(strItemTextCaseNO == "" ) return;

	CString strItemTextSpermDate = m_ActiveResultList.GetItemText(pNMListView->iItem, 2);
	if(strItemTextSpermDate == "" ) return;

	CCombinationAnalyseDlg& theDlg = *(((CMainFrame*) AfxGetMainWnd())->m_pWndCombinDlg);

	CString strSQLQuery;
	if(theDlg.m_chk_datetime==TRUE) {
		strSQLQuery.Format("select distinct * from BasicData \
							where BasicData.pName = '%s' \
							and BasicData.pCaseNO = '%s' \
							and YEAR(BasicData.pDetectDateTime)  = '%s' \
							and MONTH(BasicData.pDetectDateTime) = '%s' \
							and DAY(BasicData.pDetectDateTime)   = '%s' ", 
							strItemTextName,
							strItemTextCaseNO,
							strItemTextSpermDate.Left(4),
							GetMonth_From_CString(strItemTextSpermDate),
							GetDay_From_CString(strItemTextSpermDate));
	}
	else {
		strSQLQuery.Format("select distinct * from BasicData \
							where BasicData.pName = '%s' \
							and BasicData.pCaseNO = '%s'", 
							strItemTextName,
							strItemTextCaseNO);
	}

	theDlg.UpdateData();

	try
	{
		_variant_t var;
		_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLQuery, NULL, adCmdText);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}

	CString cs = m_ActiveResultList.GetItemText(pNMListView->iItem, 3);
	theDlg.m_ActiveData.spermTotalDensity = atof(cs);
	cs = m_ActiveResultList.GetItemText(pNMListView->iItem, 4);
	theDlg.m_ActiveData.moveSpermRatio = atof(cs);
	cs = m_ActiveResultList.GetItemText(pNMListView->iItem, 5);
	theDlg.m_ActiveData.movespeed = atof(cs);

	theDlg.UpdateData(FALSE);

	*pResult = 0;
}

BOOL CActiveResultPropPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}
