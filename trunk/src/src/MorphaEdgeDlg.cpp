// MorphaEdgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MorphaEdgeDlg.h"
#include "ImageConvert.h"
#include "ImageProcess.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMorphaEdgeDlg property page
extern _ConnectionPtr theConnection;
IMPLEMENT_DYNCREATE(CMorphaEdgeDlg, CPropertyPage)

CMorphaEdgeDlg::CMorphaEdgeDlg() : CPropertyPage(CMorphaEdgeDlg::IDD)
{
	//{{AFX_DATA_INIT(CMorphaEdgeDlg)
	//}}AFX_DATA_INIT
	m_lpData = NULL ;
	m_lpHead = NULL;

	m_nHScrollPos = 0;
	m_nVScrollPos = 0;
}

CMorphaEdgeDlg::~CMorphaEdgeDlg()
{
	if(m_lpData)
	{
		delete []m_lpData;
		m_lpData = NULL;
	}
	if(m_lpHead)
	{
		delete m_lpHead;
		m_lpHead = NULL;
	}
}

void CMorphaEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMorphaEdgeDlg)
	DDX_Control(pDX, IDC_EDGE_IMAGE, m_wndEdgeImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMorphaEdgeDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CMorphaEdgeDlg)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorphaEdgeDlg message handlers

void CMorphaEdgeDlg::GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER &lpHead, LPBYTE &lpData)
{
	try
	{
		CString strSelect;
		strSelect.Format("select morphaimage.* from morphaimage,basicinfo,morpharesult,spermchait where \
			     morphaimage.pdetectno = basicinfo.pdetectno \
			and  morphaimage.pdetectno = morpharesult.pdetectno \
			and  morphaimage.pdetectno = spermchait.pdetectno \
			and  morphaimage.pdetectno ='%s' ",strDetectNO);

		_RecordsetPtr rs("adodb.recordset");
		rs->PutCursorLocation(adUseClient);
		rs->Open((LPCTSTR)strSelect,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);

		long nSize = rs->Fields->Item["edgeimage"]->ActualSize; 
		_variant_t varChunk;
		varChunk = rs->GetFields()->Item["edgeimage"]->GetChunk(nSize);

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

BOOL CMorphaEdgeDlg::OnSetActive() 
{
	CMainFrame* pMainfrm = (CMainFrame*)AfxGetMainWnd();
	CString curDetNO = pMainfrm->m_pWndMorphadlg->m_strEditDetectNO;

	if(curDetNO == "") 
		return CPropertyPage::OnSetActive();
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
		m_wndEdgeImage.CreateImage(m_lpHead,m_lpData);
	}	
	CRect rect;
	m_wndEdgeImage.GetClientRect(rect);
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
	m_wndEdgeImage.Invalidate();
	return CPropertyPage::OnSetActive();
}

void CMorphaEdgeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
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
	m_wndEdgeImage.Invalidate();	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMorphaEdgeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	m_nHScrollPos = si.nPos;
	SetScrollInfo(SB_HORZ,&si);
	m_wndEdgeImage.Invalidate();				
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
