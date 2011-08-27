// SpermView.cpp : implementation of the CSpermView class
//
#include "stdafx.h"
#include "dxtrans.h"
#include "spermview.h"

#include "MemDC.h"
#include "wingdi.h"
#include "math.h"
#include "spermview.h"
#include "ImageProcess.h"
#include "Sperm.h"
#include "DShowUtilities.h"

#include "SpermDoc.h"
#include "SpermView.h"
#include "browerdlg.h"
#include "mainfrm.h"
#include "math.h"
#include "View3.h"
#include "patientInfo.h"
#include "OptionDlg.h"
#include "spermview.h"
#include "MorphaDetectDLG.h"
#include "SpermMorphaSet.h"
#include "AllFunction.h"
#include "ImageConvert.h"
#include "resource.h"
#include "LiveDetectDLG.h"
#include "PrinteDlg.h"

#include "dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSpermView
extern _ConnectionPtr theConnection;
extern CSpermView* theView1;
extern CView3*   theView3;
extern CString   theStrPathApp;

IMPLEMENT_DYNCREATE(CSpermView, CScrollView)

BEGIN_MESSAGE_MAP(CSpermView, CScrollView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CSpermView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_IMAGE, OnImage)
	ON_COMMAND(ID_PRINT_TABLE, OnPrintTable)
	ON_COMMAND(ID_PRINT_PREVIEW, OnPrintPreview)
	ON_COMMAND(ID_PRINT_SETUP, OnPrintSetup)
	ON_COMMAND(ID_DATA_OPTION_S, OnDataOption)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_OPEN_S, OnFileOpen)
	ON_COMMAND(ID_BUTTON_LEFTARROW, OnButtonLeftarrow)
	ON_COMMAND(ID_BUTTON_RIGHTARROW, OnButtonRightarrow)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFTARROW, OnUpdateButtonLeftarrow)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHTARROW, OnUpdateButtonRightarrow)
	ON_COMMAND(ID_DELETE_SPERM, OnDeleteSperm)
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_COMMAND(ID_SPERM_MORSET, OnSpermMorset)
	ON_COMMAND(ID_NORMAL_SPERM, OnSpermNormal)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_S, OnUpdateFileOpenS)
	ON_COMMAND(ID_SPERM_DEL, OnSpermDel)
	ON_COMMAND(ID_MENU_A, OnSpermAddA)
	ON_COMMAND(ID_MENU_B, OnSpermAddB)
	ON_COMMAND(ID_MENU_C, OnSpermAddC)
	ON_COMMAND(ID_MENU_D, OnSpermAddD)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_ADD_NORMAL2DB, OnMenuAddNormal2DB)
	ON_COMMAND(ID_MENU_ADD_ABNORMAL2DB, OnMenuAddAbnormal2DB)
//	ON_COMMAND(ID_MENU_DB_VERDICT, OnMenuDbVerdict)
	//}}AFX_MSG_MAP
	// Standard printing commands
	
	ON_MESSAGE(USER_MM_CAP_FRM_HAS_BEEN_FINISHED, OnCapFrmFinished)

	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

CSpermView::CSpermView()
{
	// TODO: add construction code here
	m_bAnalyseFinished=false;
	m_nOpenOpt = 0;
    m_nPos = -1;

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1); ///加载位图
	m_brushBackground.CreatePatternBrush(&bmp); ///创建位图画刷
 
// { 3.29
	m_eViewShowMode = INITIAL;

	// 处理中的序列图
	memset(m_lpImage, NULL, sizeof(m_lpImage)); // 序列帧数据区指针
	memset(m_lpBMIH, NULL, sizeof(m_lpBMIH));   // 序列帧信息头的指针

	// 轨迹跟踪分析结果图
	m_lpTrackResultBmData = NULL;  // 轨迹跟踪结果图数据区指针
	m_lpTrackResultBmInfo = NULL;  // 轨迹跟踪结果图信息头的指针

	// 轨迹跟踪手工调整图
	m_lpAdjustBmData = NULL;
	m_lpAdjustBmInfo = NULL;

	m_nDeviceID = 0;

	m_bMenuItemFileOpen = true;
// } 3.29

	// { 2009_3_31
    mMoveShowTimer = 0;
	mFrm = 0;
	// } 2009_3_31

}

CSpermView::~CSpermView()
{
	int i = 0;
	
	for(i=0; i<FRAME_NUM; i++)
	{	
		delete []m_lpImage[i];
		m_lpImage[i] = NULL;
	}
	
	for(i=0; i<FRAME_NUM; i++)
	{	
		delete []m_lpBMIH[i];
		m_lpBMIH[i]  = NULL;
	}
		
}

BOOL CSpermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSpermView drawing

void CSpermView::OnDraw(CDC* pDC)
{
	theView1 = this;
	CSpermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(rect);
	CPoint sp= GetScrollPosition();
	rect.right  += sp.x;
	rect.bottom += sp.y;
	long wd = rect.Width();
	long ht = rect.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC,m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
	memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(rect, RGB(255,255,255));
	
	switch(m_eViewShowMode)
	{
	case VIDEO_PLAY: // 视频播放状态
		break;
		
	case FRAME_ADJUST_RESULT: // 显示实时调整结果状态
		if(	m_lpAdjustBmInfo != NULL)
		{
			::StretchDIBits(pDC->m_hDC,
   			    0, 0, wd, ht,		
				0, 0, m_lpAdjustBmInfo->biWidth, m_lpAdjustBmInfo->biHeight,	
				m_lpAdjustBmData, (LPBITMAPINFO)(m_lpAdjustBmInfo), DIB_RGB_COLORS, SRCCOPY);
		}
		return;
		break;
		
	case VIDEO_RESULT:  // 视频分析结果显示状态
		if(	m_lpTrackResultBmInfo != NULL)
		{
			::StretchDIBits(pDC->m_hDC,
				0, 0, wd, ht,
				0, 0, m_lpTrackResultBmInfo->biWidth, m_lpTrackResultBmInfo->biHeight,
				m_lpTrackResultBmData, (LPBITMAPINFO)(m_lpTrackResultBmInfo), DIB_RGB_COLORS, SRCCOPY);
		}
		return;
		break;
		
	case MORPHA_IMAGE:  // 形态学图像打开显示状态

		if(m_imgMorphaImg.GetImage())
		{	
			CSize sz(m_imgMorphaImg.Width(), m_imgMorphaImg.Height());
	        SetScrollSizes(MM_TEXT,sz);
			m_imgMorphaImg.Show(memDC.m_hDC,
								0, 0, m_imgMorphaImg.Width(), m_imgMorphaImg.Height()
							    );
		}
		break;

	case MORPHA_RESULT:           // 形态学图像结果显示状态
		if(m_bAnalyseFinished) {   // 形态学分析图
			DrawImage(&memDC);
		    CSize sz(m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
	        SetScrollSizes(MM_TEXT,sz);
		}
		break;
		
	case INITIAL:	// 初始态
	default:
		pDC->FillRect(rect,&m_brushBackground); //用背景画刷填充区域
		break; 
	}

	pDC->SetViewportOrg(-GetScrollPosition());
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

}

/////////////////////////////////////////////////////////////////////////////
// CSpermView printing

BOOL CSpermView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(4);
	pInfo->m_nNumPreviewPages = 9; 
	return DoPreparePrinting(pInfo);
}

void CSpermView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	pInfo->m_nNumPreviewPages = 9;
}

void CSpermView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSpermView diagnostics

#ifdef _DEBUG
void CSpermView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSpermView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSpermDoc* CSpermView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpermDoc)));
	return (CSpermDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CSpermView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
    CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView=(CView3*)pMainFrm->GetView3();
	if (pView->m_pwndLiveDetectDlg->m_pVideoPlay && pView->m_pwndLiveDetectDlg->m_bIsVideoPlay == TRUE) 
		return true;
	return CScrollView::OnEraseBkgnd(pDC);
}

// void CSpermView::InverseImage(LPBITMAPINFOHEADER lpSrcInfo, LPBYTE lpSrcData)
// {
// 	int i, j;
// 	int nh = lpSrcInfo->biHeight, nw = lpSrcInfo->biWidth;
// 	long lw = WIDTHBYTES((m_lpBMIH[0]->biWidth)*32);//每行的字节数
// 	for (i=0; i<nh; i++){
// 		for (j=0; j<nw; j++){
// 			if( lpSrcData[ i*lw + nw*4 + 0 ] >= 220 ){
// 				lpSrcData[ i*lw + nw*4 + 0 ] = 255 - lpSrcData[ i*lw + nw*4 + 0 ];
// 				lpSrcData[ i*lw + nw*4 + 1 ] = lpSrcData[ i*lw + nw*4 + 0 ];
// 				lpSrcData[ i*lw + nw*4 + 1 ] = lpSrcData[ i*lw + nw*4 + 0 ];
// 			}
// 		}
// 	}
// }

void CSpermView::OnImage() 
{
	// TODO: Add your command handler code here

	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERAAVI:	//  关闭摄像头
		theView3->m_pwndLiveDetectDlg->m_cap.Stop();
		break;
	case CView3::VIDEOAVI:  //  暂停视频文件
		theView3->m_pwndLiveDetectDlg->OnBtnPause();
		{
			theView3->m_pwndLiveDetectDlg->GetDlgItem(IDC_BTN_PLAY)->EnableWindow(false);
			theView3->m_pwndLiveDetectDlg->GetDlgItem(IDC_BTN_STOP)->EnableWindow(false);
			theView3->m_pwndLiveDetectDlg->GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(false);
			theView3->m_pwndLiveDetectDlg->GetDlgItem(IDC_BTN_RESUME)->EnableWindow(false);
		}
		break;
	}

	m_eViewShowMode = FRAME_ADJUST_RESULT;
	Invalidate();

#ifdef _SAVE_IMAGE_FRAME

	//取得运行程序的绝对路径   
	CString sPath(theStrPathApp);
	sPath += "\\Sperm_Images\\";
    if(CreateDirectory((LPCTSTR)sPath,NULL))
	    AfxMessageBox("CreateDirectory suceed...\n");

    CFile file; 	
	CFileException e;
	CDib *pdib = new CDib;

#endif // _SAVE_IMAGE_FRAME

	int m;
	long lw = WIDTHBYTES((m_lpBMIH[0]->biWidth)*32);//每行的字节数
	for(m=0; m<FRAME_NUM; m++)
	{

#ifdef _SAVE_IMAGE_FRAME

		pdib->m_pInfoHeader = m_lpBMIH[m];
		pdib->m_pData = m_lpImage[m];
		CString str;
		str.Format("%d.bmp", m+1);
		CString SaveBmpPath = sPath + str;  // 文件绝对路径
		file.Open( SaveBmpPath, CFile::modeCreate | CFile::modeWrite);

		pdib->SaveDibFile(&file);
		file.Close();

#endif // _SAVE_IMAGE_FRAME

		::Rgb2Gray(m_lpBMIH[m], m_lpImage[m]);  //灰度化

		//InverseImage(m_lpBMIH[m], m_lpImage[m]);
		//::MediaFilter(m_lpBMIH[m], m_lpImage[m]); //中值滤波	
		//::MeanFilter(m_lpBMIH[m], m_lpImage[m]);  //高斯平滑 
	}

	// 运动回放用的图像序列

	int nh = m_lpBMIH[0]->biHeight;
	int nw = m_lpBMIH[0]->biWidth;

	LPBITMAPINFOHEADER  (&pMoveImgSeqInfo)[FRAME_NUM] = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqInfo;
	LPBYTE 	(&pMoveImgSeqData)[FRAME_NUM] = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqData;
	for(m=0; m<FRAME_NUM; m++) {
		DeleteDIBImage(pMoveImgSeqInfo[m], pMoveImgSeqData[m]);
	}

	int i, j;
	for(m=0; m<FRAME_NUM; m++)
	{
		NewDIBImage(pMoveImgSeqInfo[m], pMoveImgSeqData[m], nw, nh, 4);
		for(i=0; i<nh; ++i)
			for(j=0; j<nw; ++j)
				memcpy(pMoveImgSeqData[m] + i*lw + j*4, m_lpImage[m] + i*lw + j*4, 3);
	}

}

LRESULT CSpermView::OnCapFrmFinished(WPARAM wParam,LPARAM lParam)
{
	switch(theView3->m_eDetectMode)	
	{
	case CView3::CAMERAAVI:
		GuoPing();
		break;
	case CView3::VIDEOAVI:
		GuoPing();
		break;
	}
	return S_OK;
}

CString GetConnectIP()
{
	CString  bstr = (char*)theConnection->GetConnectionString();
	int nl = bstr.Find("Data Source=") + strlen("Data Source=");
	int nr = bstr.Find(";", nl);
	return bstr.Mid(nl, nr-nl);
}

void CSpermView::OnPrintTable() 
{
/*	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("sperm");
	parm.username = TEXT("sa");
	parm.passwd = TEXT("sa");

	ReportPrinter cpr;	
	cpr.PrintReport(reportype,parm,pDectectNo);*/
}

void CSpermView::OnPrintPreview() 
{

	PreviewResult(2,"asgdhdfhgj");

/*		int reportype = 2;
	CString strdetect("asgdhdfhgj");
	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("sperm");
	parm.username = TEXT("sa");
	parm.passwd = TEXT("sa");

	CPrinteDlg cpld(reportype,parm,strdetect);
	cpld.DoModal();
*/

}

void CSpermView::OnPrintSetup() 
{
	// TODO: Add your command handler code here
//	SendMessage(WM_COMMAND,ID_FILE_PRINT_SETUP);
//	OnFilePrintSetup();
}

void CSpermView::OnDataOption() 
{
	COptionDlg dlg;
	dlg.DoModal();
}

void CSpermView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	

	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView=(CView3*)pMainFrm->GetView3();
	CMorphaDetectDLG* pWnd=(CMorphaDetectDLG*)pView->m_pwndMorphaDetDlg;
	if(pWnd==NULL || pWnd->m_hWnd == NULL) return;

	vector<SpermImage>& m_vRSpermImage = pWnd->m_vRSpermImage;
	vector<SpermImage>& m_vOSpermImage = pWnd->m_vOSpermImage;
	vector<SperMopyParameter>&m_vSperMopyParameter = pWnd->m_vSperMopyParameter;
	vector<SpermRegion>&m_vSpermRegion =  pWnd->m_vSpermRegion ;
	vector<IsSperNormal>&m_vbIsNormal = pWnd->m_vbIsNormal;
    int i, iPos = -1, n;
	n = m_vRSpermImage.size();
	
	CPoint sp = GetScrollPosition(); // 得到视图滚动了多少（以象素为单位）
	point += sp;

	double d, min = 1<<20;
	CPoint p;
	for(i=0; i<n; i++)
	{
		if( m_vSpermRegion[i].isDeleted == TRUE ) continue;
        p = CPoint(m_vSpermRegion[i].SpermCenterPos.y, m_vSpermRegion[i].SpermCenterPos.x);
		if( IsInRect(point, pWnd->m_nR, p) )
		{
			d = dist(point, p);
			if(d<min)
			{
				min = d;
				iPos = i;
			}
		}
	}

	pWnd->m_nSelNO = iPos;
	m_nPos = iPos;
	
	if(m_nPos == -1 ) return;

	CRect rc;
	GetClientRect(rc);
	CDC* pDC=GetDC();
	rc.bottom+=sp.y;
	rc.right+=sp.x;
	pDC->SetViewportOrg(-sp);

	DrawMemDCImage(pDC,rc);

	ReleaseDC(pDC);
	pWnd->UpdateData(FALSE);
	pWnd->Invalidate();
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CSpermView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnLButtonUp(nFlags, point);
}

void CSpermView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint sPoint=point+GetScrollPosition();
	ClientToScreen(&sPoint);
	int iPos=GetSpermIndex(sPoint);
	if(iPos != -1)
	{
		CSpermMorphaSet dlg(iPos);
		dlg.DoModal();	
		CPoint sp=GetScrollPosition();
		CRect rc;
		GetClientRect(rc);
		CDC* pDC=GetDC();
		rc.bottom+=sp.y;
		rc.right+=sp.x;
		pDC->SetViewportOrg(-sp);
		DrawMemDCImage(pDC,rc);
		ReleaseDC(pDC);	
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CSpermView::OnFileOpen()
{
	switch(m_nOpenOpt)
	{
	case LIVE_VIDEO_CAMERA : // 摄像头视频流采集 default
		break;
	case LIVE_VIEDO_FILE: // 硬盘视频文件采集
		FileOpenDiskVideoFile();
		break;
	case MORPHA: // 形态学分析图像
		FileOpenMorphaImage();
		break;
	}
}

void CSpermView::FileOpenDiskVideoFile()
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,"AVI文件(*.avi)|*.avi|Mpeg文件(*.mpg)|*.mpg|Mp3文件(*.mp3)|*.mp3|Wave文件(*.wav)|*.wav|All Files (*.*)|*.*||");
	dlg.m_ofn.lpstrTitle="打开多媒体文件";
	if(dlg.DoModal()==IDOK)
	{
		theView3->GetDlgItem(IDC_BUTTON_MORPHADETECT)->EnableWindow(FALSE);
		theView3->GetDlgItem(IDC_BTN_LIVEDETECTED)->EnableWindow(TRUE);

		if(theView3->m_pwndLiveDetectDlg->m_pVideoPlay)
		{
			theView3->m_pwndLiveDetectDlg->m_pVideoPlay->OnVideoStop();
			delete theView3->m_pwndLiveDetectDlg->m_pVideoPlay;
			theView3->m_pwndLiveDetectDlg->m_pVideoPlay = NULL;
		}

		theView3->m_pwndLiveDetectDlg->m_pVideoPlay = new CMyVideoPlay;
		theView3->m_pwndLiveDetectDlg->m_pVideoPlay->Init(this->GetSafeHwnd(), 
													  this->GetSafeHwnd(), 
													  dlg.GetPathName() );
		theView3->m_pwndLiveDetectDlg->m_pVideoPlay->OnVideoPlay();

		theView3->m_pwndLiveDetectDlg->m_dFrequency 
			= theView3->m_pwndLiveDetectDlg->m_pVideoPlay->GetVideoInfo().mFramePerSecond;
		m_eViewShowMode = VIDEO_PLAY;
		SetScrollSizes(MM_TEXT, CSize(100,100));
	}
}

void CSpermView::FileOpenMorphaImage()
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView=(CView3*)pMainFrm->GetView3();	
	CString strOpenFileType = "位图文件(*.bmp)|*.bmp|jpg文件(*.jpg;*.jpeg)|*.jpg;jpeg|All Files (*.*)|*.*||";
	CFileDialog OpenDlg(TRUE, "打开图片", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strOpenFileType,NULL);
	if(OpenDlg.DoModal() == IDOK)
	{	
		m_bAnalyseFinished = false; // 0314
		AfxGetMainWnd()->SetWindowText(CString("精子检测系统_")+OpenDlg.GetFileName()); // 0314
		bool bLoad = m_imgMorphaImg.Load(OpenDlg.GetPathName());
		if(bLoad == false)
		{
			CString strMsg;
			strMsg.Format("Load Morpah Image %s Error",OpenDlg.GetPathName());
			MessageBox(strMsg,"ERROR",MB_ICONERROR);
			return ;
		}
		CDC *pDC = GetDC();
		m_imgMorphaImg.Show(pDC->m_hDC,0,0,m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
		ReleaseDC(pDC);
		if(pView->m_pwndMorphaDetDlg->GetSafeHwnd())
		{
			pView->m_pwndMorphaDetDlg->InitStr();
			pView->m_pwndMorphaDetDlg->UpdateData(FALSE);
			pView->m_pwndMorphaDetDlg->clearworkspace();
			pView->m_pwndMorphaDetDlg->Invalidate();
			pView->m_pwndMorphaDetDlg->GetDlgItem(IDC_BUTTON_MORPHADETECT_FINISH)->EnableWindow(FALSE);
		}
		pView->m_pwndMorphaDetDlg->m_lsOperateIndex.clear();
		pView->GetDlgItem(IDC_BUTTON_MORPHADETECT)->EnableWindow(TRUE);
		pView->GetDlgItem(IDC_BTN_LIVEDETECTED)->EnableWindow(FALSE);
		++pView->m_pwndMorphaDetDlg->m_nVideoFiled;
		m_eViewShowMode = MORPHA_IMAGE;

		CSize sz(m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
		SetScrollSizes(MM_TEXT,sz);
		Invalidate();
	}
}

BOOL CSpermView::IsInRect(CPoint cen,int r,CPoint p)
{
     if(p.x < cen.x+r && p.x >cen.x-r &&
		p.y < cen.y+r && p.y >cen.y-r )
        return TRUE;
	 else
	    return FALSE;
}

double CSpermView::dist(CPoint p, CPoint q)
{
	float d = (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y);
    return sqrt(d);
}

void CSpermView::DrawImage(CDC *pDC)
{	
	int i,n;
	
	CMainFrame* pMainfrm=(CMainFrame*)AfxGetMainWnd();
	CMorphaDetectDLG* pWnd=(CMorphaDetectDLG*)((CView3*)pMainfrm->GetView3())->m_pwndMorphaDetDlg;
	if(pWnd==NULL || pWnd->m_hWnd == NULL) return;
	vector<SpermRegion>& m_vSpermRegion = pWnd->m_vSpermRegion;
	int& m_r=pWnd->m_nR;
	vector<IsSperNormal>& m_vbIsNormal=pWnd->m_vbIsNormal;
	vector<SperMopyParameter>& m_vSperMopyParameter=pWnd->m_vSperMopyParameter;
	LPBYTE& m_lpResBmDataSrc=pWnd->m_lpResBmDataSrc;
	LPBITMAPINFOHEADER& m_lpResBmInfoSrc= pWnd->m_lpResBmInfoSrc;

	LPBYTE& m_lpResEdgeBmDataSrc=pWnd->m_lpResEdgeBmDataSrc;
	LPBITMAPINFOHEADER& m_lpResEdgeBmInfoSrc= pWnd->m_lpResEdgeBmInfoSrc;

	switch(pWnd->m_wndCbxShowOpt.GetCurSel())
	{
	case 2:  //  画原图
		m_imgMorphaImg.Show(pDC->m_hDC,0,0,m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
		break;
	case 1:	 //  画填充图
		if(m_lpResBmInfoSrc && m_lpResBmDataSrc)
		{
			::StretchDIBits(pDC->GetSafeHdc(), 
				0,0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				m_lpResBmDataSrc, (LPBITMAPINFO)m_lpResBmInfoSrc, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	case 0:  // 画边缘图
		if(m_lpResEdgeBmDataSrc && m_lpResBmInfoSrc)
		{
			::StretchDIBits(pDC->GetSafeHdc(), 
				0,0,m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				m_lpResEdgeBmDataSrc, (LPBITMAPINFO)m_lpResBmInfoSrc, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	}
	if(pWnd->m_wndChkRectangle.GetCheck())
	{
		// 画边框
		CPen myRedPen(PS_SOLID, 1, RGB(255,0,0));
		CPen myBluePen(PS_SOLID, 1, RGB(0,0,255));
		CPen *myPen, *pOldPen;

		n = m_vSpermRegion.size();
		list<int>& m_lsOperateIndex = pWnd->m_lsOperateIndex;
		for(i=0; i<n; i++)
		{			
			if(m_vbIsNormal[i].IsNormalVector[0] == TRUE)
				myPen = &myBluePen;
			else myPen = &myRedPen;
			
			pOldPen = pDC->SelectObject(myPen);  // 选用红画笔
			pDC->SelectObject(GetStockObject(NULL_BRUSH));  // 空白画刷

			if( find(m_lsOperateIndex.begin(),pWnd->m_itListIndex,i )
				!= pWnd->m_itListIndex ) continue;
			CPoint lt = CPoint(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x - m_r);
			CPoint rt = CPoint(m_vSpermRegion[i].SpermCenterPos.y + m_r, m_vSpermRegion[i].SpermCenterPos.x - m_r);
			CPoint lb = CPoint(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r);
			CPoint rb = CPoint(m_vSpermRegion[i].SpermCenterPos.y + m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r);	
			CRect  rect = CRect(lt, rb);
            if( m_nPos == i)
			{
				pDC->SelectObject(pOldPen);
				COLORREF color; 
				if(m_vbIsNormal[i].IsNormalVector[0] == TRUE)
					color = RGB(0,0,255); // 选用蓝画笔
				else 
					color = RGB(255,0,0); // 选用红画笔

				CPen dotpen(PS_DASHDOT,1,color);
				pDC->SelectObject(&dotpen);
				pDC->Rectangle(rect);
			    pDC->SelectObject(myPen);
			}
			else
			{
				pDC->Rectangle(rect);
			}
		
		}
		if(n>0)	pDC->SelectObject(pOldPen);		
		// 编号
		{
			CPen myPen(PS_SOLID, 2, RGB(255,0,0));
			CPen *pOldPen = pDC->SelectObject(&myPen); //选用新画笔
			pDC->SetBkMode(TRANSPARENT);
			CString  strNo, cs;
			n = m_vbIsNormal.size();
			for(i=0; i<n; i++)
			{
				if( find(m_lsOperateIndex.begin(),pWnd->m_itListIndex,i)
					!= pWnd->m_itListIndex ) continue;
				
				pDC->SetTextColor(RGB(0,0,255));
				strNo.Format("%d", i+1);
				pDC->TextOut(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r - 15, strNo);
				
				if( m_vbIsNormal[i].IsNormalVector[0] == TRUE )
				{
					pDC->SetTextColor(RGB(0,0,255));
					cs = "正常";
				}
				else
				{
					pDC->SetTextColor(RGB(255,0,0));
					cs = "异常";
				}
				pDC->TextOut(m_vSpermRegion[i].SpermCenterPos.y - m_r + 20, m_vSpermRegion[i].SpermCenterPos.x + m_r - 17, cs);
				
			}	
			pDC->SelectObject(pOldPen);
		}
	}
	if(m_nPos != -1)
	{
		
		CString cs;
 		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_length);
 		pWnd->m_strLength      = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_width);
		pWnd->m_strWidth       = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_area);
		pWnd->m_strArea        = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_perimeter);
		pWnd->m_strPerimeter   = cs;
		
		cs.Format("%3.2lf", m_vSperMopyParameter[m_nPos].m_ellipticity);
		pWnd->m_strEllipticity = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_perfor_area);
		pWnd->m_strPerforArea = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_head_area);
		pWnd->m_strHeadArea   = cs;
		
		cs.Format("%.1f", m_vSperMopyParameter[m_nPos].m_head_perfor_area*100);
		pWnd->m_strHeadPerforArea  = cs + "%";
		
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_length);
		pWnd->m_strMitsomaLength = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_width);
		pWnd->m_strMitosomaWidth  = cs;
		
		cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_angle);
		pWnd->m_strMitosmaDevangle  = cs + "度";
		
		cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_ruga);
		pWnd->m_strRuga  = cs;
		
		cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_extension);
		pWnd->m_strExtension  = cs;
		
		cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_symmetry);
		pWnd->m_strSymmetry   = cs;
		
		// 精子编号	
		cs.Format("第%d号精子  -->> ", m_nPos+1);
		pWnd->m_strInfo  =  cs;
	
		cs = ( (m_vbIsNormal[m_nPos].IsNormalVector[0] == TRUE ) ? "正常" : "异常");
		pWnd->m_strIsNormal = cs;
		
		pWnd->UpdateData(FALSE);
		pWnd->Invalidate();
	}
}

void CSpermView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);	
}

void CSpermView::OnButtonLeftarrow() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView3= (CView3*)pMainFrm->GetView3();
	CMorphaDetectDLG*& pMorphaDlg=pView3->m_pwndMorphaDetDlg;

	list<int>& m_lsOperateIndex=pView3->m_pwndMorphaDetDlg->m_lsOperateIndex;
	list<int>::iterator& m_itListIndex=pView3->m_pwndMorphaDetDlg->m_itListIndex;
	--m_itListIndex;
	pMorphaDlg->AddSperm(*m_itListIndex);

	pMorphaDlg->FormResultImage();

	CString str;
	str.Format("目前一共检测了%d个视野,%d个精子,其中%d个正常,%d个异常.",
		pMorphaDlg->m_nVideoFiled,pMorphaDlg->m_MhNum.AbnormalSpermNum+pMorphaDlg->m_MhNum.NormalSpermNum,
		pMorphaDlg->m_MhNum.NormalSpermNum,pMorphaDlg->m_MhNum.AbnormalSpermNum);
	pMorphaDlg->GetDlgItem(IDC_DETECT_INFO)->SetWindowText(str);
	CPoint sp=GetScrollPosition();
	CRect rc;
	GetClientRect(rc);
	CDC* pDC=GetDC();
	rc.bottom+=sp.y;
	rc.right+=sp.x;
	pDC->SetViewportOrg(-sp);
	DrawMemDCImage(pDC,rc);
	ReleaseDC(pDC);
}

void CSpermView::OnButtonRightarrow() 
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView3= (CView3*)pMainFrm->GetView3();
	CMorphaDetectDLG*& pMorphaDlg=pView3->m_pwndMorphaDetDlg;

	list<int>& m_lsOperateIndex=pView3->m_pwndMorphaDetDlg->m_lsOperateIndex;
	list<int>::iterator& m_itListIndex=pView3->m_pwndMorphaDetDlg->m_itListIndex;
	pMorphaDlg->DeleteSperm(*m_itListIndex);
	++m_itListIndex;
	pMorphaDlg->FormResultImage();

	CString str;
	str.Format("目前一共检测了%d个视野,%d个精子,其中%d个正常,%d个异常.",
		pMorphaDlg->m_nVideoFiled,pMorphaDlg->m_MhNum.AbnormalSpermNum+pMorphaDlg->m_MhNum.NormalSpermNum,
		pMorphaDlg->m_MhNum.NormalSpermNum,pMorphaDlg->m_MhNum.AbnormalSpermNum);
	pMorphaDlg->GetDlgItem(IDC_DETECT_INFO)->SetWindowText(str);

	CPoint sp=GetScrollPosition();
	CRect rc;
	GetClientRect(rc);
	CDC* pDC=GetDC();
	rc.bottom+=sp.y;
	rc.right+=sp.x;
	pDC->SetViewportOrg(-sp);
	DrawMemDCImage(pDC,rc);
	ReleaseDC(pDC);
}

void CSpermView::OnUpdateButtonLeftarrow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CView3* pView3=(CView3*)pMainFrm->GetView3();
	list<int>& m_lsOperateIndex=pView3->m_pwndMorphaDetDlg->m_lsOperateIndex;
	list<int>::iterator& m_itListIndex=pView3->m_pwndMorphaDetDlg->m_itListIndex;
	pCmdUI->Enable(m_itListIndex != m_lsOperateIndex.begin());
}

void CSpermView::OnUpdateButtonRightarrow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CView3* pView3=(CView3*)pMainFrm->GetView3();
	list<int>& m_lsOperateIndex=pView3->m_pwndMorphaDetDlg->m_lsOperateIndex;
	list<int>::iterator& m_itListIndex=pView3->m_pwndMorphaDetDlg->m_itListIndex;
	pCmdUI->Enable(m_itListIndex != m_lsOperateIndex.end());
	
}

void CSpermView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	if(m_imgMorphaImg.GetImage() != NULL)
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
		
		int index=GetSpermIndex(point+GetScrollPosition());
		if( index == -1 ) return;
		CPoint lpoint=point;
		ScreenToClient(&lpoint);
		DWORD dw=0;
		dw |= lpoint.x;
		dw |= (lpoint.y<<16);
		SendMessage(WM_LBUTTONDOWN,0,(LPARAM)dw);
		
		CMenu menu;
		menu.LoadMenu(CG_IDR_POPUP_SPERM_VIEW);

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}

//
	// 活力的
	if(theView3->m_pwndLiveDetectDlg->m_bIsUserState)
	{	
		// 停止运动视频回放
		CDC *pDC = GetDC();
		CRect rc;
		GetClientRect(&rc);
		long cW = rc.Width();
		long cH = rc.Height();			

		KillTimer(mMoveShowTimer);
		mFrm = 0;
		long nw = m_lpTrackResultBmInfo->biWidth;
		long nh = m_lpTrackResultBmInfo->biHeight;
		::StretchDIBits(pDC->m_hDC,
						0, 0, cW, cH,
						0, 0, nw, nh,
						m_lpTrackResultBmData,
						(LPBITMAPINFO)(m_lpTrackResultBmInfo),
						DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(pDC);


		LPBYTE lpSrc = theView3->m_pwndLiveDetectDlg->m_lpBmData;
		LPBITMAPINFOHEADER lpInfo = theView3->m_pwndLiveDetectDlg->m_lpBmInfo;

		int width  = lpInfo->biWidth;
		int height = lpInfo->biHeight;

		CPoint pt  = point;
		ScreenToClient(&pt);
		mClick_pt = pt;

		pt.x *= width/double(cW);
		pt.y *= height/double(cH);

		pt.y = height - 1 - pt.y;

		m_pt = pt;
		
		CMenu menu;
		menu.LoadMenu(CG_IDR_POPUP_USERCHANGE);

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);

	}
//
}

void CSpermView::DrawMemDCImage(CDC *pDC1,const CRect rect)
{
	CMemDC dc(pDC1,rect);
	CMemDC pDC=&dc;
	theView1 = this;
	CSpermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	if(m_imgMorphaImg.GetImage())
	{
	   m_imgMorphaImg.Show(pDC->m_hDC,0,0,m_imgMorphaImg.Width(),m_imgMorphaImg.Height(),rect.left,rect.top);
	}

	if(m_bAnalyseFinished)   //  形态学分析图
	    DrawMemResImg(pDC);
	
}

void CSpermView::DrawMemResImg(CDC *pDC)
{
	int i,n;
	
	CMainFrame* pMainfrm=(CMainFrame*)AfxGetMainWnd();
	CMorphaDetectDLG* pWnd=(CMorphaDetectDLG*)((CView3*)pMainfrm->GetView3())->m_pwndMorphaDetDlg;
	if(pWnd==NULL || pWnd->m_hWnd == NULL) return;
	vector<SpermRegion>& m_vSpermRegion = pWnd->m_vSpermRegion;
	vector<SperMopyParameter>&m_vSperMopyParameter=pWnd->m_vSperMopyParameter ;
	int& m_r=pWnd->m_nR;
	vector<IsSperNormal>& m_vbIsNormal=pWnd->m_vbIsNormal;
	LPBYTE& m_lpResBmDataSrc=pWnd->m_lpResBmDataSrc;
	LPBITMAPINFOHEADER& m_lpResBmInfoSrc= pWnd->m_lpResBmInfoSrc;

	LPBYTE& m_lpResEdgeBmDataSrc=pWnd->m_lpResEdgeBmDataSrc;
	LPBITMAPINFOHEADER& m_lpResEdgeBmInfoSrc= pWnd->m_lpResEdgeBmInfoSrc;

	switch(pWnd->m_wndCbxShowOpt.GetCurSel())
	{
	case 2:  //  画原图
		m_imgMorphaImg.Show(pDC->m_hDC,0,0,m_imgMorphaImg.Width(),m_imgMorphaImg.Height());
		break;
	case 1:	 //   画填充图
		if(m_lpResBmInfoSrc && m_lpResBmDataSrc)
		{
			::StretchDIBits(pDC->GetSafeHdc(), 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				m_lpResBmDataSrc, (LPBITMAPINFO)m_lpResBmInfoSrc, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	case 0:  // 画边缘图
		if(m_lpResEdgeBmDataSrc && m_lpResBmInfoSrc)
		{
			::StretchDIBits(pDC->GetSafeHdc(), 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				0, 0, m_lpResBmInfoSrc->biWidth, m_lpResBmInfoSrc->biHeight, 
				m_lpResEdgeBmDataSrc, (LPBITMAPINFO)m_lpResBmInfoSrc, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	}
	
	n = m_vSpermRegion.size();
	list<int>& m_lsOperateIndex = pWnd->m_lsOperateIndex;
	if(pWnd->m_wndChkRectangle.GetCheck())
	{
		// 画边框
		CPen myRedPen(PS_SOLID, 1, RGB(255,0,0));
		CPen myBluePen(PS_SOLID, 1, RGB(0,0,255));
		CPen *myPen, *pOldPen;

		for(i=0; i<n; i++)
		{			
			if(m_vbIsNormal[i].IsNormalVector[0] == TRUE)
				myPen = &myBluePen;
			else myPen = &myRedPen;
			
			pOldPen = pDC->SelectObject(myPen);  // 选用红画笔
			pDC->SelectObject(GetStockObject(NULL_BRUSH));  // 空白画刷

			if( find(m_lsOperateIndex.begin(),pWnd->m_itListIndex,i )
				!= pWnd->m_itListIndex ) continue;
			CPoint lt = CPoint(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x - m_r);
			CPoint rt = CPoint(m_vSpermRegion[i].SpermCenterPos.y + m_r, m_vSpermRegion[i].SpermCenterPos.x - m_r);
			CPoint lb = CPoint(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r);
			CPoint rb = CPoint(m_vSpermRegion[i].SpermCenterPos.y + m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r);	
			CRect  rect = CRect(lt, rb);
            if( m_nPos == i)
			{
				pDC->SelectObject(pOldPen);
				COLORREF color; 
				if(m_vbIsNormal[i].IsNormalVector[0] == TRUE)
					color = RGB(0,0,255); // 选用蓝画笔
				else 
					color = RGB(255,0,0); // 选用红画笔

				CPen dotpen(PS_DASHDOT,1,color);
				pDC->SelectObject(&dotpen);
				pDC->Rectangle(rect);
			    pDC->SelectObject(myPen);
			}
			else
			{
				pDC->Rectangle(rect);
			}
		
		}
		pDC->SelectObject(pOldPen);		

		// 编号
		{
			CPen myPen(PS_SOLID, 2, RGB(255,0,0));
			CPen *pOldPen = pDC->SelectObject(&myPen); //选用新画笔
			pDC->SetBkMode(TRANSPARENT);
			CString  strNo, cs;
			n = m_vbIsNormal.size();
			for(i=0; i<n; i++)
			{
				if(    find(m_lsOperateIndex.begin(),pWnd->m_itListIndex,i)
					!= pWnd->m_itListIndex ) continue;
				
				pDC->SetTextColor(RGB(0,0,255));
				strNo.Format("%d", i+1);
				pDC->TextOut(m_vSpermRegion[i].SpermCenterPos.y - m_r, m_vSpermRegion[i].SpermCenterPos.x + m_r - 15, strNo);
				
				if( m_vbIsNormal[i].IsNormalVector[0] == TRUE )
				{
					pDC->SetTextColor(RGB(0,0,255));
					cs = "正常";
				}
				else
				{
					pDC->SetTextColor(RGB(255,0,0));
					cs = "异常";
				}
				pDC->TextOut(m_vSpermRegion[i].SpermCenterPos.y - m_r + 20, m_vSpermRegion[i].SpermCenterPos.x + m_r - 17, cs);
				
			}	
			pDC->SelectObject(pOldPen);
		}
	}
	
	{
		CPen myPen(PS_SOLID, 2, RGB(255,0,0));
		CPen *pOldPen = pDC->SelectObject(&myPen); //选用新画笔
		pDC->SetBkMode(TRANSPARENT);
		CString  strNo, cs;
		n = m_vbIsNormal.size();
		for(i=0; i<n; i++)
		{
			if(    find(m_lsOperateIndex.begin(),pWnd->m_itListIndex,i)
				!= pWnd->m_itListIndex ) continue;
			
			if(m_nPos != -1)
			{
				
				CString cs;
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_length);
				pWnd->m_strLength      = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_width);
				pWnd->m_strWidth       = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_area);
				pWnd->m_strArea        = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_perimeter);
				pWnd->m_strPerimeter   = cs;
				
				cs.Format("%3.2lf", m_vSperMopyParameter[m_nPos].m_ellipticity);
				pWnd->m_strEllipticity = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_perfor_area);
				pWnd->m_strPerforArea = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_head_area);
				pWnd->m_strHeadArea   = cs;
				
				cs.Format("%.1lf", m_vSperMopyParameter[m_nPos].m_head_perfor_area*100);
				pWnd->m_strHeadPerforArea  = cs + "%";
				
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_length);
				pWnd->m_strMitsomaLength = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_width);
				pWnd->m_strMitosomaWidth  = cs;
				
				cs.Format("%.2lf", m_vSperMopyParameter[m_nPos].m_tail_angle);
				pWnd->m_strMitosmaDevangle  = cs + "度";
				
				cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_ruga);
				pWnd->m_strRuga  = cs;
				
				cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_extension);
				pWnd->m_strExtension  = cs;
				
				cs.Format("%.2f", m_vSperMopyParameter[m_nPos].m_symmetry);
				pWnd->m_strSymmetry   = cs;
				
				// 精子编号	
				cs.Format("第%d号精子  -->> ", m_nPos+1);
				pWnd->m_strInfo = cs;
				
				cs = ( (m_vbIsNormal[m_nPos].IsNormalVector[0] == TRUE ) ? "正常" : "异常");
				pWnd->m_strIsNormal = cs;
				
				pWnd->UpdateData(FALSE);
				pWnd->Invalidate();			
			}
		}	
		pDC->SelectObject(pOldPen);
	}
}

void CSpermView::OnDeleteSperm() 
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView=(CView3*)pMainFrm->GetView3();
	CMorphaDetectDLG* pWnd=(CMorphaDetectDLG*)pView->m_pwndMorphaDetDlg;
	pWnd->OnButtonDeletesperm();
}

void CSpermView::OnMenuAddNormal2DB()
{
	// TODO: Add your command handler code here
	theView3->m_pwndMorphaDetDlg->WriteMorphaSpermData(
		theView3->m_pwndMorphaDetDlg->m_nSelNO, true);
}

void CSpermView::OnMenuAddAbnormal2DB()
{
	// TODO: Add your command handler code here
	theView3->m_pwndMorphaDetDlg->WriteMorphaSpermData(
		theView3->m_pwndMorphaDetDlg->m_nSelNO, false);
}

int CSpermView::GetSpermIndex(CPoint pt)
{
	ScreenToClient(&pt);
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView=(CView3*)pMainFrm->GetView3();
	CMorphaDetectDLG* pWnd=(CMorphaDetectDLG*)pView->m_pwndMorphaDetDlg;
	vector<SpermRegion>& m_vSpermRegion=pWnd->m_vSpermRegion;
	int i,iPos=-1;
	double d, min = 1<<20;
	CPoint p;
	for(i=0; i<m_vSpermRegion.size(); i++)
	{
		if( m_vSpermRegion[i].isDeleted == TRUE ) continue;
        p = CPoint(m_vSpermRegion[i].SpermCenterPos.y, m_vSpermRegion[i].SpermCenterPos.x);
		if( IsInRect(p, pWnd->m_nR, pt) )
		{
			d = dist(p, pt);
			if(d < min )
			{
				min = d;
				iPos = i;
			}
		}
	}
	pWnd->m_nSelNO = iPos;
	return iPos;
}


BOOL CSpermView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	CSize sz1=GetTotalSize();
	CRect rect;
	GetClientRect(rect);
	CSize sz2(rect.Width(),rect.Height());
	if(sz1.cy<sz2.cy)
		return CScrollView::OnMouseWheel(nFlags, zDelta, pt);;
	SCROLLINFO si;
	zDelta =  -zDelta;
	GetScrollInfo(SB_VERT,&si);
	si.nPos += zDelta/12;
	SetScrollInfo(SB_VERT,&si);


	CPoint sp=GetScrollPosition();
	CRect rc;
	GetClientRect(rc);
	CDC* pDC=GetDC();
	rc.bottom+=sp.y;
	rc.right+=sp.x;
	pDC->SetViewportOrg(-sp);
	DrawMemDCImage(pDC,rc);
	ReleaseDC(pDC);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CSpermView::UserShowSperm()
{
	CPoint sp=GetScrollPosition();
	CRect rc;
	GetClientRect(rc);
	CDC* pDC=GetDC();
	rc.bottom+=sp.y;
	rc.right+=sp.x;
	pDC->SetViewportOrg(-sp);
	DrawMemDCImage(pDC,rc);
	ReleaseDC(pDC);
}

void CSpermView::OnSize(UINT nType, int cx, int cy) 
{
	CSize sz(100,100);
	SetScrollSizes(MM_TEXT,sz);

	CScrollView::OnSize(nType, cx, cy);	
}

void CSpermView::GuoPing()
{
	OnImage();
		
	const int nChooseToAdjust = 0;  // 选取第1帧做为调整图，和结果图
	
	// ReadImageFromDisk(nChooseToAdjust, m_pFile);

	int nW = m_lpBMIH[nChooseToAdjust]->biWidth;
	int nH = m_lpBMIH[nChooseToAdjust]->biHeight;
	int nC = m_lpBMIH[nChooseToAdjust]->biBitCount/8;
	
	if( theView3->m_pwndLiveDetectDlg->m_lpBmInfo &&
		theView3->m_pwndLiveDetectDlg->m_lpBmData )
	{
		DeleteDIBImage(theView3->m_pwndLiveDetectDlg->m_lpBmInfo,
			theView3->m_pwndLiveDetectDlg->m_lpBmData);
	}
	NewDIBImage(theView3->m_pwndLiveDetectDlg->m_lpBmInfo,
		theView3->m_pwndLiveDetectDlg->m_lpBmData, nW, nH, nC);
	
	memcpy(theView3->m_pwndLiveDetectDlg->m_lpBmInfo, m_lpBMIH[nChooseToAdjust], sizeof(BITMAPINFOHEADER));
	memcpy(theView3->m_pwndLiveDetectDlg->m_lpBmData, m_lpImage[nChooseToAdjust], theView3->m_pwndLiveDetectDlg->m_lpBmInfo->biSizeImage);
	
	if( theView3->m_pwndLiveDetectDlg->m_lpOringinImgHead &&
		theView3->m_pwndLiveDetectDlg->m_lpOringinImgData )
	{
		DeleteDIBImage(theView3->m_pwndLiveDetectDlg->m_lpOringinImgHead,
			theView3->m_pwndLiveDetectDlg->m_lpOringinImgData);
	}	
	NewDIBImage(theView3->m_pwndLiveDetectDlg->m_lpOringinImgHead,
		theView3->m_pwndLiveDetectDlg->m_lpOringinImgData, nW, nH, nC);
	
	memcpy(theView3->m_pwndLiveDetectDlg->m_lpOringinImgHead, m_lpBMIH[nChooseToAdjust], sizeof(BITMAPINFOHEADER));
	memcpy(theView3->m_pwndLiveDetectDlg->m_lpOringinImgData, m_lpImage[nChooseToAdjust], theView3->m_pwndLiveDetectDlg->m_lpBmInfo->biSizeImage);
	
	::Rgb2Gray(theView3->m_pwndLiveDetectDlg->m_lpOringinImgHead, theView3->m_pwndLiveDetectDlg->m_lpOringinImgData);
	
	if( m_lpAdjustBmInfo  && m_lpAdjustBmData  ) 
		DeleteDIBImage(m_lpAdjustBmInfo, m_lpAdjustBmData);
	NewDIBImage(m_lpAdjustBmInfo, m_lpAdjustBmData, nW, nH, nC);
	memcpy(m_lpAdjustBmInfo, m_lpBMIH[nChooseToAdjust], sizeof(BITMAPINFOHEADER));
	memcpy(m_lpAdjustBmData, m_lpImage[nChooseToAdjust],
		m_lpBMIH[nChooseToAdjust]->biSizeImage);
	
	if( m_lpTrackResultBmInfo  && m_lpTrackResultBmData  ) 		
		DeleteDIBImage(m_lpTrackResultBmInfo, m_lpTrackResultBmData);	
	NewDIBImage(m_lpTrackResultBmInfo, m_lpTrackResultBmData, nW, nH, nC);	
	
	memcpy(m_lpTrackResultBmInfo, m_lpBMIH[nChooseToAdjust], sizeof(BITMAPINFOHEADER));
	memcpy(m_lpTrackResultBmData, m_lpImage[nChooseToAdjust], m_lpBMIH[nChooseToAdjust]->biSizeImage);
	
	//{ 314 
	if(theView3->m_pwndLiveDetectDlg->m_iplBinaryImage != NULL)
		cvReleaseImage(&theView3->m_pwndLiveDetectDlg->m_iplBinaryImage);
	theView3->m_pwndLiveDetectDlg->m_iplBinaryImage = cvCreateImage(cvSize(m_lpBMIH[nChooseToAdjust]->biWidth,m_lpBMIH[nChooseToAdjust]->biHeight),
		8,m_lpBMIH[nChooseToAdjust]->biBitCount/8);
	
	//} 314
	theView3->m_pwndLiveDetectDlg->m_bIsVideoPlay  = false;
	theView3->m_pwndLiveDetectDlg->m_bControlState = true;
	theView3->m_pwndLiveDetectDlg->m_bModeChange   = false;
	theView3->m_pwndLiveDetectDlg->m_bAutoAnalyse  = true;
	theView3->m_pwndLiveDetectDlg->m_bMoveShow  = true;
	theView3->m_pwndLiveDetectDlg->OnUpdateButtonState();
	
	if( theView3->m_pwndLiveDetectDlg->m_nHeight > 0
		&& theView3->m_pwndLiveDetectDlg->m_nWidth > 0 
		&& theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry )
	{
		for(int i=0; i<theView3->m_pwndLiveDetectDlg->m_nHeight; i++)
		{
			delete [](theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry[i]);
			theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry[i] = NULL;
		}
		delete [](theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry);
		theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry = NULL;
	}
	
	theView3->m_pwndLiveDetectDlg->m_nHeight = m_lpBMIH[nChooseToAdjust]->biHeight;
	theView3->m_pwndLiveDetectDlg->m_nWidth  = m_lpBMIH[nChooseToAdjust]->biWidth;	
	
	if(!theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry)
	{
		theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry =
			new bool*[theView3->m_pwndLiveDetectDlg->m_nHeight];
		for(int i=0; i<theView3->m_pwndLiveDetectDlg->m_nHeight; i++)
		{
			theView3->m_pwndLiveDetectDlg->m_ppIVisitMarkAry[i] = 
				new bool[theView3->m_pwndLiveDetectDlg->m_nWidth];
		}
	}
	
	::SendMessage(theView3->m_pwndLiveDetectDlg->GetSafeHwnd(),
		WM_HSCROLL, 0,
		(LPARAM)( theView3->m_pwndLiveDetectDlg->m_wndSliderGrayValue.GetSafeHwnd() ) );

}

void CSpermView::OnSpermMorset() 
{
	// TODO: Add your command handler code here
	if(m_nPos == -1)
		return;
	CSpermMorphaSet dlg(m_nPos);
	dlg.DoModal();

    Invalidate();
}

void CSpermView::OnSpermNormal()
{
	// TODO: Add your command handler code here
	if(m_nPos == -1)
		return;
	theView3->m_pwndMorphaDetDlg->m_vbIsNormal[m_nPos].IsNormalVector[0] = TRUE;
    Invalidate();
}

void CSpermView::PreviewResult(int reprottype, CString detectno)
{
	crxparm parm;
	parm.ipadd = TEXT( GetConnectIP() );
	parm.database = TEXT("sperm");
	parm.username = TEXT("sa");
	parm.passwd = TEXT("sa");
	CPrinteDlg cpld(reprottype,parm,detectno);
	cpld.DoModal();
}

void CSpermView::OnUpdateFileOpenS(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuItemFileOpen);
}

BOOL CSpermView::ReadImageFromDisk(int i, CFile *pFile)
{
     CDib *pDib = new CDib();
	 BOOL ret = pDib->ReadDibFile(pFile);
	 ::NewDIBImage(m_lpBMIH[i], m_lpImage[i], pDib->m_pInfoHeader->biWidth, pDib->m_pInfoHeader->biHeight, 4);
	 memcpy(m_lpBMIH[i], pDib->m_pInfoHeader, sizeof(BITMAPINFOHEADER));
	 delete pDib, pDib = NULL;
	 return ret;
}

BOOL CSpermView::WriteImageToDisk(int i, CFile *pFile)
{
	CDib *pDib = new CDib(m_lpBMIH[i], m_lpImage[i]);
	BOOL ret = pDib->SaveDibFile(pFile);
	delete pDib, pDib = NULL;
	return ret;
}

void CSpermView::markTheNewSperm(const POINT& pt, COLORTYPE ct) {	

	long h = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqInfo[0]->biHeight;
	long w = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqInfo[0]->biWidth;
	int parr[][2] = {
		{-1,-1}, {-1,0}, {-1,1},
		{0,-1}, {0,0}, {0,1},
		{1,-1}, {1,0}, {1,1},

		{-1,-2}, {0,-2}, {1,-2},
		{-1,2},	{0,2}, {1,2},
		{2,-1},	{2,0}, {2,1},
		{-2,-1}, {-2,0}, {-2,1}
	};

	vector<ColorPoint>vp;
	ColorPoint cp;
	int t=0;
	for (t=0; t<sizeof(parr)/sizeof(parr[0]); t++)
	{	
		int x = pt.x + parr[t][0];
		int y = pt.y + parr[t][1];
		if( x>=0 && x<w && y>=0 && y<h ) {
			cp.pt.x = y;
			cp.pt.y = x;
			cp.ct = ct;
			vp.push_back(cp);
		}
	}
	theView3->m_pwndLiveDetectDlg->m_vp.push_back(vp);
	theView3->m_pwndLiveDetectDlg->FormTrackMap();
}

void CSpermView::OnSpermAddA()
{
	CLiveDetectDLG * pdlg = theView3->m_pwndLiveDetectDlg;
	pdlg->m_nAclass++;
	pdlg->GetSpermMoveParameter();
	pdlg->ShowSpermLiveDetectedResult();
	theView3->UpdateData(FALSE);
	markTheNewSperm(m_pt, REDT);
	CRect rc(mClick_pt.x-5, mClick_pt.y-5, mClick_pt.x+5, mClick_pt.y+5);
	InvalidateRect(&rc);
}

void CSpermView::OnSpermAddB() 
{
	CLiveDetectDLG * pdlg = theView3->m_pwndLiveDetectDlg;
	pdlg->m_nBclass++;
	pdlg->GetSpermMoveParameter();
	pdlg->ShowSpermLiveDetectedResult();
	theView3->UpdateData(FALSE);
	markTheNewSperm(m_pt, GREENT);
	CRect rc(mClick_pt.x-5, mClick_pt.y-5, mClick_pt.x+5, mClick_pt.y+5);
	InvalidateRect(&rc);
}

void CSpermView::OnSpermAddC()
{
	CLiveDetectDLG * pdlg = theView3->m_pwndLiveDetectDlg;
	pdlg->m_nCclass++;
	pdlg->GetSpermMoveParameter();
	pdlg->ShowSpermLiveDetectedResult();
	theView3->UpdateData(FALSE);
	markTheNewSperm(m_pt, BLUET);
	CRect rc(mClick_pt.x-5, mClick_pt.y-5, mClick_pt.x+5, mClick_pt.y+5);
	InvalidateRect(&rc);
}

void CSpermView::OnSpermAddD()
{
	CLiveDetectDLG * pdlg = theView3->m_pwndLiveDetectDlg;
	pdlg->m_nDclass++;
	pdlg->GetSpermMoveParameter();
	pdlg->ShowSpermLiveDetectedResult();
	theView3->UpdateData(FALSE);
	markTheNewSperm(m_pt, YELLOWT);
	CRect rc(mClick_pt.x-5, mClick_pt.y-5, mClick_pt.x+5, mClick_pt.y+5);
	InvalidateRect(&rc);
}

void CSpermView::OnSpermDel()
{
	CLiveDetectDLG * pdlg = theView3->m_pwndLiveDetectDlg;
	int nDelNO = -1;
	int r, max_r = 3;
	int i=0, j=0;
	for(i=0; i<pdlg->m_vp.size(); i++)
	{
		for(j=0; j<pdlg->m_vp[i].size(); j++)
		{
			CPoint p(pdlg->m_vp[i][j].pt);
			swap(p.x, p.y);
			r = dist(p, m_pt) + 0.5;
			if( r < max_r )
			{
				nDelNO = i;
				max_r = r;
			}
		}
	}
	if( nDelNO > 0 ) {
		switch(pdlg->m_vp[nDelNO][0].ct)
		{
		case REDT:
			pdlg->m_nAclass--;
			break;
		case GREENT:
			pdlg->m_nBclass--;
			break;
		case BLUET:
			pdlg->m_nCclass--;
			break;
		case YELLOWT:
			pdlg->m_nDclass--;
			break;
		default:
			break;
		}
		pdlg->GetSpermMoveParameter();
		pdlg->ShowSpermLiveDetectedResult();
		pdlg->m_vp[nDelNO].clear();
		pdlg->FormTrackMap();
		CRect rc(mClick_pt.x-50, mClick_pt.y-50, mClick_pt.x+50, mClick_pt.y+50);
		InvalidateRect(&rc);
	}
}

void CSpermView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if( nIDEvent == mMoveShowTimer ) {
		CDC *pDC = GetDC();
		CRect rc;
		GetClientRect(&rc);
		long cW = rc.Width();
		long cH = rc.Height();
		if( mFrm < FRAME_NUM-1 ) { mFrm++; }
		else { mFrm = 0; }	
		LPBITMAPINFOHEADER lpInfo = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqInfo[mFrm];
		LPBYTE lpData = theView3->m_pwndLiveDetectDlg->m_pMoveImgSeqData[mFrm];
		long nw = lpInfo->biWidth;
		long nh = lpInfo->biHeight;			
		::StretchDIBits(pDC->m_hDC,
						0, 0, cW, cH,
						0, 0, nw, nh,
						lpData, 
						(LPBITMAPINFO)(lpInfo), 
						DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(pDC);
	}

	CScrollView::OnTimer(nIDEvent);
}

