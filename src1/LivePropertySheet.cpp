// CLivePropertySheet.cpp : implementation file
//

#include "stdafx.h"

#include "LivePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLivePropertySheet

IMPLEMENT_DYNAMIC(CLivePropertySheet, CPropertySheet)

CLivePropertySheet::CLivePropertySheet(CWnd*   pParentWnd)   
  :CPropertySheet(AFX_IDS_APP_TITLE,   pParentWnd)  
{
    AddPage(&m_livenessPage);
	AddPage(&m_imagePage);	
}

CLivePropertySheet::~CLivePropertySheet()
{    

}

BEGIN_MESSAGE_MAP(CLivePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CLivePropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLivePropertySheet message handlers

BOOL CLivePropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	return bResult;
}
