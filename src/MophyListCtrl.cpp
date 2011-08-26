// MophyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MophyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMophyListCtrl

TCHAR* MophyColOrder[][2] = 
{
	//  活力常规检测
	TEXT("pName"), TEXT("姓名"),
	TEXT("pCaseNO"), TEXT("样本号"),
	TEXT("pDetectDateTime"), TEXT("取精日期"),
	TEXT("pTotalNormalRatio"), TEXT("形态正常率"),
	NULL, NULL
};

CMophyListCtrl::CMophyListCtrl()
{
}

CMophyListCtrl::~CMophyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMophyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMophyListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMophyListCtrl message handlers

int CMophyListCtrl::SetListCtrlData(const _RecordsetPtr &rs)
{
	int i , colN;
	DeleteAllItems();
	int nColumnCount = GetHeaderCtrl()->GetItemCount();
	for (i=0;i < nColumnCount; i++)
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
	i = 0;

	while (MophyColOrder[i][0])
	{
		lc.iSubItem = i;
		lc.pszText = MophyColOrder[i][1];
		InsertColumn(i, &lc);
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
			_variant_t vt = rs->GetCollect((LPCSTR)CString(MophyColOrder[j][0]));
			_bstr_t ss = (_bstr_t)vt;
			lvitem.pszText = (char*)ss;
			if( j == 0)
				InsertItem(&lvitem);
			else if(j==2) {
				CString cs(lvitem.pszText);						
				// ***
				int len=10;
				if(isdigit(cs[6])) {
					if(isdigit(cs[9]))
						len = 10;
					else
						len = 9;
				}
				else {
					if(isdigit(cs[8]))
						len = 9;
					else
						len = 8;
				}
				SetItemText(i,j,cs.Left(len));
			}
			else if(j==3) {
				CString cs;
				double vr = atof(lvitem.pszText);
				cs.Format("%.1lf%%", vr);
				SetItemText(i,j,cs.Left(10));
			}
			else
				SetItemText(i,j,lvitem.pszText);
		}
		++i;
		rs->MoveNext();
	}
		
	return i;
}
