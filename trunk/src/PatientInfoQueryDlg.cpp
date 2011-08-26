// PatientInfoQueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MainFrm.h"
#include "PatientInfoQueryDlg.h"
#include "View3.h"
#include "AllFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CView3* theView3;
extern _ConnectionPtr theConnection;
/////////////////////////////////////////////////////////////////////////////
// CPatientInfoQueryDlg dialog

CPatientInfoQueryDlg::CPatientInfoQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatientInfoQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatientInfoQueryDlg)
	m_bChkName = FALSE;
	m_bChkCaseNO = FALSE;
	m_strInputCaseNO = _T("");
	m_strInputName = _T("");
	//}}AFX_DATA_INIT
	mSelItemIndex = -1;
}

void CPatientInfoQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatientInfoQueryDlg)
	DDX_Control(pDX, IDC_PATIENT_INFO_LIST, m_PatientInfoListCtr);
	DDX_Check(pDX, IDC_CHK_NAME, m_bChkName);
	DDX_Check(pDX, IDC_CHK_CASENO, m_bChkCaseNO);
	DDX_Text(pDX, IDC_EDIT_INPUTCASENO, m_strInputCaseNO);
	DDX_Text(pDX, IDC_EDIT_INPUTNAME, m_strInputName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPatientInfoQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CPatientInfoQueryDlg)
	ON_BN_CLICKED(IDC_CHK_NAME, OnChkName)
	ON_BN_CLICKED(IDC_CHK_CASENO, OnChkCaseno)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBtnQuery)
	ON_BN_CLICKED(IDC_BTN_SEL_OK, OnBtnSelOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatientInfoQueryDlg message handlers

void CPatientInfoQueryDlg::OnQuery()
{
	CString strSQLGetNum, strSQLQuery;
	UpdateData(TRUE);
	if( m_bChkName && m_bChkCaseNO ) {
		strSQLGetNum.Format("select count(*) from PatientInfoListForQuery where pName = '%s' and pCaseNO = '%s'", m_strInputName, m_strInputCaseNO);
		strSQLQuery.Format("select * from PatientInfoListForQuery where pName = '%s' and pCaseNO = '%s'", m_strInputName, m_strInputCaseNO);
	}
	else if( m_bChkName ) {
		strSQLGetNum.Format("select count(*) from PatientInfoListForQuery where pName = '%s'", m_strInputName);
		strSQLQuery.Format("select * from PatientInfoListForQuery where pName = '%s'", m_strInputName);
	}
	else if( m_bChkCaseNO ) {		
		strSQLGetNum.Format("select count(*) from PatientInfoListForQuery where pCaseNO = '%s'", m_strInputCaseNO);
		strSQLQuery.Format("select * from PatientInfoListForQuery where pCaseNO = '%s'", m_strInputCaseNO);
	}
	else {
		strSQLGetNum.Format("select count(*) from PatientInfoListForQuery");
		strSQLQuery.Format("select * from PatientInfoListForQuery");
	}

	try {
		_RecordsetPtr rs = theConnection->Execute((LPCSTR)strSQLGetNum,NULL,adCmdText);
		long nTotalRecord = rs->GetCollect((long)0).iVal;
		CString queryinfo;
 		queryinfo.Format("共查询到%ld条记录", nTotalRecord);
 		SetDlgItemText(IDC_STATIC_QUERY, queryinfo);
		
		rs = theConnection->Execute((LPCSTR)strSQLQuery,NULL,adCmdText);
		((CMainFrame*)AfxGetMainWnd())->m_nQueryPage = 3;
		m_PatientInfoListCtr.SetData(rs);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
}

void CPatientInfoQueryDlg::OnChkName()
{
	// TODO: Add your control notification handler code here
	OnQuery();
}

void CPatientInfoQueryDlg::OnChkCaseno()
{
	// TODO: Add your control notification handler code here
	OnQuery();
}

void CPatientInfoQueryDlg::OnBtnSelOk()
{
	// TODO: Add your control notification handler code here
	CString strDetectNO, strName, strDilutionRatio;
	long nSel = m_PatientInfoListCtr.GetSelectedCount();
	if( nSel > 0 ) {
		mSelItemIndex = m_PatientInfoListCtr.GetNextItem(-1, LVNI_SELECTED);
			
		// 
		LV_COLUMN  lc;
		lc.mask  = LVCF_TEXT ;
		TCHAR pszText[40];
		lc.pszText =  pszText;
		lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
		int nCol = 0;
		while (m_PatientInfoListCtr.GetColumn(nCol,&lc))
		{
			if(CString(lc.pszText) == TEXT("检测号"))
				strDetectNO = m_PatientInfoListCtr.GetItemText(mSelItemIndex, nCol);
			if(CString(lc.pszText) == TEXT("姓名"))
				strName = m_PatientInfoListCtr.GetItemText(mSelItemIndex, nCol);
			if(CString(lc.pszText) == TEXT("稀释比"))
				strDilutionRatio = m_PatientInfoListCtr.GetItemText(mSelItemIndex, nCol);
				theView3->m_pwndLiveDetectDlg->m_dDilutionRatio = atof((LPCTSTR)strDilutionRatio);
			++nCol;
		}
		theView3->m_strDetectNO = strDetectNO;
		theView3->m_strPatientName = strName;
		theView3->UpdateData(FALSE);
	}
	CDialog::OnOK();
}

int CPatientInfoQueryDlg::FindCol(_RecordsetPtr &rs,const _variant_t& colName)
{
	rs->MoveFirst();
	while ( !rs->EndOfFile )
	{
		_variant_t vt = rs->GetCollect((long)0);
		if(colName == vt)
			return TRUE;
		rs->MoveNext();
	}
	return FALSE;
}


BOOL CPatientInfoQueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CString strSQLGetColName("select distinct name from syscolumns \
	//						  where id=object_id('basicinfo') \
	//						  or id=object_id('spermchait')");
	//CString strSQLGet_basicinfo_Col("select distinct name from syscolumns where id=object_id('basicinfo')");
	//CString strSQLGet_spermchait_Col("select distinct name from syscolumns where id=object_id('spermchait')");
	CString strSQLPatientInfoList("create table PatientInfoListForQuery(");
	try
	{
		//_RecordsetPtr rsGet__Col = theConnection->Execute((LPCSTR)strSQLGet_basicinfo_Col,NULL,adCmdText);
		//_RecordsetPtr rsGet_spermchait_Col = theConnection->Execute((LPCSTR)strSQLGet_spermchait_Col,NULL,adCmdText);
		//_RecordsetPtr rsGetCol = theConnection->Execute((LPCSTR)strSQLGetColName,NULL,adCmdText);
		
		std::set<CString>column_names;
		std::set<CString>basicinfo_column_names;
		std::set<CString>spermchait_column_names;
		GetTableColumnNames(theConnection, column_names, "basicinfo");
		GetTableColumnNames(theConnection, column_names, "spermchait");
		GetTableColumnNames(theConnection, basicinfo_column_names, "basicinfo");
		GetTableColumnNames(theConnection, spermchait_column_names, "spermchait");

		if( column_names.size() > 0 ) {
			strSQLPatientInfoList += (char*)(_bstr_t)((*column_names.begin()).GetString());
		}

		std::set<CString>::const_iterator citr = column_names.begin();
		for (; citr != column_names.end(); ++citr)
		{
			const char* vt = (*column_names.begin()).GetString();
			strSQLPatientInfoList += CString(",") + vt;
		}
		strSQLPatientInfoList += CString(") as select ");

		for (; citr != column_names.end(); ++citr)
		{
			CString e(", ");
			std::set<CString>::const_iterator c = citr;
			if(c++ != column_names.end())
			{
				e = " ";
			}
			const char* vt = (*column_names.begin()).GetString();
			int afind = (basicinfo_column_names.find(vt) != basicinfo_column_names.end());
			int bfind = (spermchait_column_names.find(vt) != spermchait_column_names.end());
			if(afind>0) strSQLPatientInfoList += CString("basicinfo.") + vt + e;
			else if(bfind>0) strSQLPatientInfoList += CString("spermchait.") + vt + e;
		}

		strSQLPatientInfoList += CString("from basicinfo, spermchait \
						where basicinfo.pDetectNO = spermchait.pDetectNO");

// 		FILE *fp = fopen("C:\\np\\project\\sperm\\sql_script.txt", "w");
// 		fprintf(fp, "%s", strSQLPatientInfoList.GetBuffer(strSQLPatientInfoList.GetLength()));
// 		fclose(fp);

		if(!IsTableExist(theConnection,"PatientInfoListForQuery"))
 				theConnection->Execute((LPCSTR)strSQLPatientInfoList,NULL,adCmdText);

		CString strQuery("select count(*) from PatientInfoListForQuery");
		_RecordsetPtr rs = theConnection->Execute((LPCSTR)strQuery,NULL,adCmdText);	
		int nTotalRecord = rs->GetCollect((long)0).iVal;
		CString queryinfo;
 		queryinfo.Format("共查询到%d条记录", nTotalRecord);
 		SetDlgItemText(IDC_STATIC_QUERY, queryinfo);
		
		strQuery = "select * from PatientInfoListForQuery";
		rs = theConnection->Execute((LPCSTR)strQuery,NULL,adCmdText);
		((CMainFrame*)AfxGetMainWnd())->m_nQueryPage = 3;
		m_PatientInfoListCtr.SetData(rs);

	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatientInfoQueryDlg::OnBtnQuery() 
{
	// TODO: Add your control notification handler code here
	OnQuery();
}

