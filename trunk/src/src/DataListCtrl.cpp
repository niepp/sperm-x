// DataListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "DataListCtrl.h"

extern TCHAR*  LiveColOrder[][2];
extern TCHAR*  MorphaColOrder[][2];
extern TCHAR*  MorphaDataOrder[][2];
extern TCHAR*  PatientInfoItemOrder[][2];

#include "MainFrm.h"
#include "PatientInfoQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl
extern _ConnectionPtr theConnection;

CDataListCtrl::CDataListCtrl()
{

}

CDataListCtrl::~CDataListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDataListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CDataListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl message handlers


int CDataListCtrl::SetData(const _RecordsetPtr &rs)
{
	int i , colN;
	DeleteAllItems();
	int nColumnCount = GetHeaderCtrl()->GetItemCount();
	for (i=0;i < nColumnCount;i++)
	{
		DeleteColumn(0);
	}

	LONG lStyle = SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FLATSB ;
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)lStyle);
	long n;
	rs->GetFields()->get_Count(&n);
	LV_COLUMN lc;
	lc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH /*| LVCF_FMT*/;
	lc.cx = 100;
	i=0;
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	switch(pMainFrm->m_nQueryPage)
	{
	case 0:                     //  形态学查询
		while (MorphaColOrder[i][0])
		{
			lc.iSubItem = i;
			lc.pszText = MorphaColOrder[i][1];
			InsertColumn(i,&lc);
			++i;
		}	
		colN = i;
		i = 0;
		while (!rs->EndOfFile)
		{		
			for(int j=0;j<colN;++j)
			{
				LV_ITEM lvitem;
				lvitem.mask = LVIF_TEXT;
				lvitem.state = 0;      
				lvitem.stateMask = 0; 
				lvitem.iItem = i;
				lvitem.iSubItem = j ;
				_variant_t vt=rs->GetCollect((LPCSTR)CString(MorphaColOrder[j][0]));
				_bstr_t ss= (_bstr_t)vt;
				lvitem.pszText = (char*)ss;
				if( j == 0)
					InsertItem(&lvitem);
				else
					SetItemText(i,j,lvitem.pszText);
			}
			++i;
			rs->MoveNext();
		}
		return i;	
		break;
	case 1:                     //  活力查询
		while (LiveColOrder[i][0])
		{
			lc.iSubItem = i;
			lc.pszText = LiveColOrder[i][1];
			InsertColumn(i,&lc);
			++i;
		}	
		colN = i;i=0;
		while (!rs->EndOfFile)
		{		
			for(int j=0;j<colN;++j)
			{
				LV_ITEM lvitem;
				lvitem.mask = LVIF_TEXT;
				lvitem.state = 0;      
				lvitem.stateMask = 0; 
				lvitem.iItem = i;
				lvitem.iSubItem = j ;
				_variant_t vt=rs->GetCollect((LPCSTR)CString(LiveColOrder[j][0]));
				_bstr_t ss= (_bstr_t)vt;
				lvitem.pszText = (char*)ss;
				if( j == 0)
					InsertItem(&lvitem);
				else
					SetItemText(i,j,lvitem.pszText);
			}
			++i;
			rs->MoveNext();
		}
		return i;
		break;
	case 2:  // 形态学样本库
		while (MorphaDataOrder[i][0])
		{
			lc.iSubItem = i;
			lc.pszText = MorphaDataOrder[i][1];
			InsertColumn(i, &lc);
			++i;
		}
		colN = i;
		i = 0;
		while (!rs->EndOfFile)
		{		
			for(int j=0; j<colN; ++j)
			{
				LV_ITEM lvitem;
				lvitem.mask = LVIF_TEXT;
				lvitem.state = 0;      
				lvitem.stateMask = 0; 
				lvitem.iItem = i;
				lvitem.iSubItem = j;
				_variant_t vt=rs->GetCollect((LPCSTR)CString(MorphaDataOrder[j][0]));
				_bstr_t ss = (_bstr_t)vt;
				lvitem.pszText = (char*)ss;
				if(MorphaDataOrder[j][1]==TEXT("正常/异常"))
					lvitem.pszText = ((((bool)vt) == true) ? ("正常") : ("异常"));
				if( j == 0)
					InsertItem(&lvitem);
				else
					SetItemText(i,j,lvitem.pszText);
			}
			++i;
			rs->MoveNext();
		}
		return i;
		break;
	case 3:
		while (PatientInfoItemOrder[i][0])
		{
			lc.iSubItem = i;
			lc.pszText = PatientInfoItemOrder[i][1];
			InsertColumn(i, &lc);
			++i;
		}	
		colN = i;
		i = 0;
		while (!rs->EndOfFile)
		{		
			for(int j=0; j<colN; ++j)
			{
				LV_ITEM lvitem;
				lvitem.mask = LVIF_TEXT;
				lvitem.state = 0;      
				lvitem.stateMask = 0; 
				lvitem.iItem = i;
				lvitem.iSubItem = j;
				_variant_t vt  = rs->GetCollect((LPCSTR)CString(PatientInfoItemOrder[j][0]));
				_bstr_t ss = (_bstr_t)vt;
				lvitem.pszText = (char*)ss;
				if( j == 0)
					InsertItem(&lvitem);
				else
					SetItemText(i,j,lvitem.pszText);
			}
			++i;
			rs->MoveNext();
		}
		return i;
		break;
	}
	return i;
}


int CDataListCtrl::SetData(const CString &strSQL)
{
	_RecordsetPtr rsPtr;
	rsPtr.CreateInstance("adodb.recordset");
	try
	{
		rsPtr->CursorLocation = adUseClient;
		rsPtr->Open((LPCTSTR)strSQL,theConnection.GetInterfacePtr(),
				adOpenDynamic, adLockOptimistic, adCmdText);

		return SetData(rsPtr);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return -1;
	}
}

