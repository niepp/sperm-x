// DrawStatic.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "DrawStatic.h"
#include "LiveImagePage.h"
#include "ImageConvert.h"
#include "AllFunction.h"
#include "MainFrm.h"
#include "SpermView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawStatic

#define TOOLTIPCTRL_nID  894389439

CDrawStatic::CDrawStatic()
{
	m_lpData = NULL;
	m_lpHead = NULL;
	m_pToolTipCtrl = NULL;
}

CDrawStatic::~CDrawStatic()
{
	if(m_lpHead)
		delete m_lpHead;
	if(m_lpData)
		delete []m_lpData;

	delete m_pToolTipCtrl;
	m_pToolTipCtrl = NULL;

}


BEGIN_MESSAGE_MAP(CDrawStatic, CStatic)
	//{{AFX_MSG_MAP(CDrawStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawStatic message handlers

void CDrawStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_lpHead == NULL) return;
	CMainFrame* pMainfrm=(CMainFrame*)AfxGetMainWnd();
	CLiveImagePage* pImgpage=(CLiveImagePage*)GetParent();
	SCROLLINFO si_V,si_H;
	pImgpage->GetScrollInfo(SB_VERT,&si_V);
	pImgpage->GetScrollInfo(SB_HORZ,&si_H);
	
	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,m_lpHead->biWidth,m_lpHead->biHeight);
	memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(rect,RGB(255,255,255));
	
	StretchDIBits(memDC.m_hDC,0,0,rect.Width(),rect.Height(),
		si_H.nPos,m_lpHead->biHeight-1-si_V.nPage-si_V.nPos,rect.Width(),rect.Height()
		,m_lpData,(BITMAPINFO*)m_lpHead,DIB_RGB_COLORS,SRCCOPY);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
}

void CDrawStatic::CreateImage(const LPBITMAPINFOHEADER head, const LPBYTE data)
{
	ASSERT(head != NULL);
	ASSERT(data != NULL);
	if(m_lpHead!=NULL) {delete m_lpHead;m_lpHead = NULL;}
	m_lpHead=new BITMAPINFOHEADER;
	ASSERT(m_lpHead != NULL);
	memcpy(m_lpHead,head,sizeof(BITMAPINFOHEADER));
	if(m_lpData!=NULL) {delete []m_lpData;m_lpData = NULL;}
	m_lpData = new BYTE[m_lpHead->biSizeImage];
	ASSERT(m_lpData != NULL);
	memcpy(m_lpData,data,sizeof(BYTE)*m_lpHead->biSizeImage);
}

BOOL CDrawStatic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_pToolTipCtrl == NULL)
	{
		m_pToolTipCtrl = new CToolTipCtrl;
	}
	if(m_pToolTipCtrl->m_hWnd == NULL)
	{
		m_pToolTipCtrl->Create(this, TTS_ALWAYSTIP);
		EnableToolTips(TRUE); 
		m_pToolTipCtrl->SetMaxTipWidth(1500);  
	}
	if( m_pToolTipCtrl != NULL && pMsg->message== WM_MOUSEMOVE )  
		m_pToolTipCtrl->RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

CString TOCString(int d)
{
	CString cs;
	cs.Format("%d", d);
	return cs;
}

CString TOCString(double d)
{
	CString cs;
	cs.Format("%.2lf", d);
	return cs;
}

void CDrawStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int hScrollPos, vScrollPos;

	CMorphaEdgeDlg *pdlg = (CMorphaEdgeDlg*)GetParent();
	CPropertySheet* pSheet=(CPropertySheet*)(pdlg->GetParent());
	CStatic* pStatic=(CStatic*)pSheet->GetParent();
	CMorphaDataRetriveDlg* pDlg=(CMorphaDataRetriveDlg*)pStatic->GetParent();

	SCROLLINFO hsi,vsi;
	switch(pSheet->GetActiveIndex())	
	{
	case 0:
		break;
	case 1:
		((CMorphaPadDlg*)GetParent())->GetScrollInfo(SB_HORZ,&hsi);
		hScrollPos = hsi.nPos;
		((CMorphaPadDlg*)GetParent())->GetScrollInfo(SB_VERT,&vsi);
		vScrollPos = vsi.nPos;
		break;
	case 2:
		((CMorphaEdgeDlg*)GetParent())->GetScrollInfo(SB_HORZ,&hsi);
		hScrollPos = hsi.nPos;
		((CMorphaEdgeDlg*)GetParent())->GetScrollInfo(SB_VERT,&vsi);
		vScrollPos = vsi.nPos;
		break;
	}

    int i, nIndex, n;
	n = pDlg->m_vSpermInfo.size();
	
	CPoint sp = CPoint(hScrollPos, vScrollPos); 

	point += sp;
	const int r = 40;
	double d, min = 1<<20;
	nIndex = -1;
	CPoint p, centerPos;

	for(i=0; i<n; i++)
	{
        p = CPoint(pDlg->m_vSpermInfo[i].pos.y, pDlg->m_vSpermInfo[i].pos.x);
		if( CSpermView::IsInRect(point, r, p) )
		{
			d =  CSpermView::dist(point, p);
			if(d<min)
			{
				min = d;
				nIndex = i;
				centerPos = p;
			}
		}
	}

	if( -1 != nIndex)
	{
		CString cs;
		cs.Format("第%d号精子 ---> ", pDlg->m_vSpermInfo[nIndex].nSpermNO);
		cs += (CString)((pDlg->m_vSpermInfo[nIndex].bIsNormal==true) ? "正常" : "异常") + "\r\n";
	
		cs += "长度: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_length) +" 微米 "+ "\r"; 
		cs += "宽度: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_width) +" 微米 "	+ "\r\n"; 
		cs += "面积: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_area) +" 平方微米 " + "\r";
	
		cs += "椭圆率: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_ellipticity) + "\r\n";  
		cs += "顶体面积: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_perfor_area) +" 平方微米 " + "\r"; 
		cs += "头部面积: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_head_area) +" 平方微米 "	+ "\r\n";  
		cs += "周长: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_perimeter) +" 微米 " + "\r";
	
		cs += "颈长: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_tail_length) +" 微米 " + "\r\n";
		cs += "颈宽: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_tail_width) +" 微米 " + "\r";  
		cs += "插入角: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_tail_angle) + " 度 "+ "\r\n";  
		
		cs += "顶体比率: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_head_perfor_area) + "\r";  
		cs += "伸展度: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_extension)	+ "\r\n";  
		cs += "对称度: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_symmetry) + "\r";
		cs += "皱褶度: " + TOCString(pDlg->m_vSpermInfo[nIndex].smPara.m_ruga) + "\r\n"; 
		
		centerPos -= sp;
		CRect  rc(centerPos.x-r, centerPos.y-r, centerPos.x+r, centerPos.y+r);
		m_pToolTipCtrl->AddTool(this, cs, &rc, TOOLTIPCTRL_nID);
		m_pToolTipCtrl->Activate(TRUE);
		m_pToolTipCtrl->DelTool(this);

	}
	CStatic::OnMouseMove(nFlags, point);
}

