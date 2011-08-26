// LiveImagePage.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LiveImagePage.h"
#include "LiveDataRetriveDlg.h"
#include "MainFrm.h"
#include "imageprocess.h"
#include "ImageConvert.h"
#include "DrawStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern _ConnectionPtr theConnection;
/////////////////////////////////////////////////////////////////////////////
// CLiveImagePage property page

IMPLEMENT_DYNCREATE(CLiveImagePage, CPropertyPage)

CLiveImagePage::CLiveImagePage() : CPropertyPage(CLiveImagePage::IDD)
{
	//{{AFX_DATA_INIT(CLiveImagePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lpData = NULL;
	m_lpHead = NULL;
	m_nHScrollPos = 0;
	m_nVScrollPos = 0;
}

CLiveImagePage::~CLiveImagePage()
{
	if(m_lpHead)
	{
		DeleteDIBImage(m_lpHead,m_lpData);
	}
}

void CLiveImagePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveImagePage)
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_wndDrawImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLiveImagePage, CPropertyPage)
	//{{AFX_MSG_MAP(CLiveImagePage)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveImagePage message handlers

BOOL CLiveImagePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLiveImagePage::GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER &lpHead, LPBYTE &lpData)
{
try
	{
		CString strSelect;
		strSelect.Format("select * from image_table where pdetectno='%s'",strDetectNO);
		_RecordsetPtr rs("adodb.recordset");
		rs->PutCursorLocation(adUseClient);
		rs->Open((LPCTSTR)strSelect,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);

		long nSize = rs->Fields->Item["pimage"]->ActualSize; 
		_variant_t varChunk;
		varChunk = rs->GetFields()->Item["pimage"]->GetChunk(nSize);

		//  读Jpg图像数据(二进制格式)
		LPBYTE pBuf = new BYTE[nSize];
		char  *pSafeArrayBuf = NULL;
		SafeArrayAccessData(varChunk.parray,(void **)&pSafeArrayBuf);
		memcpy(pBuf,pSafeArrayBuf,nSize);
		SafeArrayUnaccessData(varChunk.parray);	

		// pBuf -> lpHead, lpData
		Jpg2Bmp(pBuf, nSize, lpHead, lpData);

		delete []pBuf;
		pBuf = NULL;

	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
}

BOOL CLiveImagePage::OnSetActive() 
{
	CMainFrame* pMainfrm=(CMainFrame*)AfxGetMainWnd();
	CString curDetNO=pMainfrm->m_pWndLivedlg->m_strEditDetectNO;
	if(curDetNO == "") return CPropertyPage::OnSetActive();
	if((m_lpData == NULL && m_lpHead == NULL) || curDetNO!=m_strDetectNO)
	{
		if(m_lpHead)
		{
			DeleteDIBImage(m_lpHead,m_lpData);
		}
		m_strDetectNO = curDetNO;
		GetImage(curDetNO,m_lpHead,m_lpData);
		if(m_lpHead == NULL) return CPropertyPage::OnSetActive();
		m_nVScrollPos = 0;
		m_nHScrollPos = 0;
		m_wndDrawImage.CreateImage(m_lpHead,m_lpData);

	}	
	CRect rect;
	m_wndDrawImage.GetClientRect(rect);
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_lpHead->biHeight;
	si.nPage = rect.Height();
	si.nPos = m_nVScrollPos;
	si.nTrackPos = SB_THUMBTRACK;
	SetScrollInfo(SB_VERT,&si);	

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_lpHead->biWidth;
	si.nPage = rect.Width();
	si.nPos = m_nHScrollPos;
	si.nTrackPos = SB_THUMBTRACK;
	SetScrollInfo(SB_HORZ,&si);	
	m_wndDrawImage.Invalidate();
	return CPropertyPage::OnSetActive();
}

void CLiveImagePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
}

void CLiveImagePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	GetScrollInfo(SB_VERT,&si);
	switch(nSBCode)
	{
	case SB_LINEUP:
		si.nPos-=si.nPage/10;
		break;
	case SB_LINEDOWN:
		si.nPos+=si.nPage/10;
		break;
	case SB_PAGEUP:
		si.nPos-=si.nPage;
		break;
	case SB_PAGEDOWN:
		si.nPos+=si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = nPos;
		break;
	}
	if(si.nPos<=si.nMin)
		si.nPos=si.nMin;
	if(si.nPos>=si.nMax)
		si.nPos=si.nMax;
	SetScrollInfo(SB_VERT,&si);
	m_nVScrollPos = si.nPos;
	m_wndDrawImage.Invalidate();
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CLiveImagePage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ,&si);
	switch(nSBCode)
	{
	case SB_LINELEFT:
		si.nPos-=si.nPage/10;
		break;
	case SB_LINERIGHT:
		si.nPos+=si.nPage/10;
		break;
	case SB_PAGELEFT:
		si.nPos-=si.nPage;
		break;
	case SB_PAGERIGHT:
		si.nPos+=si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = nPos;
		break;
	}
	if(si.nPos<=si.nMin)
		si.nPos=si.nMin;
	if(si.nPos>=si.nMax)
		si.nPos=si.nMax;
	SetScrollInfo(SB_HORZ,&si);
	m_nHScrollPos = si.nPos;
	m_wndDrawImage.Invalidate();	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
