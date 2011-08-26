// SpermDoc.cpp : implementation of the CSpermDoc class
//

#include "stdafx.h"
#include "Sperm.h"
#include "Spermview.h"
#include "SpermDoc.h"
#include "logindlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpermDoc

IMPLEMENT_DYNCREATE(CSpermDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpermDoc, CDocument)
	//{{AFX_MSG_MAP(CSpermDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpermDoc construction/destruction

CSpermDoc::CSpermDoc()
{
	// TODO: add one-time construction code here

}

CSpermDoc::~CSpermDoc()
{
}


BOOL CSpermDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	
	return TRUE;
	

}



/////////////////////////////////////////////////////////////////////////////
// CSpermDoc serialization

void CSpermDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//Write(ar.GetFile());
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSpermDoc diagnostics

#ifdef _DEBUG
void CSpermDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpermDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpermDoc commands

void CSpermDoc::Write(CFile *pFile)
{
}
