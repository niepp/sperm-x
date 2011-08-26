// MorphaDetectDLG.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MorphaDetectDLG.h"
#include "MainFrm.h"
#include "SpermView.h"
#include "AllFunction.h"
#include "ImageProcess.h"
#include "ImageConvert.h"
#include "View3.h"
#include "fstream"
#include "OptionDlg.h"
#include "afxmt.h"
#include "MorphaParameterDlg.h"
#include "highgui.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpermApp theApp;
CSpermView*  theView1;
CView3* theView3;
extern _ConnectionPtr theConnection;
CEvent grabimage;
extern CString theStrPathApp;

extern int gThreshAdjustPart;
extern int gThreshAdjust;
extern double gGrowStep;

extern int aaa;

void CMorphaDetectDLG::InitStr()
{
	m_strArea = _T("");
	m_strWidth = _T("");
	m_strPerimeter = _T("");
	m_strEllipticity = _T("");
	m_strExtension = _T("");
	m_strHeadArea = _T("");
	m_strHeadPerforArea = _T("");
	m_strMitosmaDevangle = _T("");
	m_strMitsomaLength = _T("");
	m_strMitosomaWidth = _T("");
	m_strPerforArea = _T("");
	m_strSymmetry = _T("");
	m_strInfo = _T("");
	m_strIsNormal = _T("");
	m_strRuga = _T("");
	m_strLength = _T("");
}

CMorphaDetectDLG::CMorphaDetectDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CMorphaDetectDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMorphaDetectDLG)
	InitStr();
	//}}AFX_DATA_INIT
	m_pSigSpermRecog = NULL;                        // 单个精子形态学分析

	m_iplMorphaColorImg = NULL;
	m_iplTempImg = NULL;
	m_rImg = NULL;

	m_nVideoFiled = 0;
	m_lpBMMorphaData     = NULL;
	m_lpBMMorphaInfo     = NULL; 

	m_iplMorphaGrayImage = NULL;

	m_nMaxArea = 20000;
	m_nEdgeThick = 18;
	m_nR = 75;
    
	m_lpResBmInfoSrc = NULL;
	m_lpResBmDataSrc = NULL;

	m_lpResEdgeBmInfoSrc = NULL;
	m_lpResEdgeBmDataSrc = NULL;

	m_lpResBmInfoSrc = NULL;
	m_lpResBmDataSrc = NULL;

	m_nSelNO= -1;

	m_itListIndex = m_lsOperateIndex.end();

	m_BFinished = FALSE;

	 m_UICtrl.m_BContinueDetect = FALSE;
	 m_UICtrl.m_BRestoreCamera  = FALSE;
	 m_UICtrl.m_BFinishDetect   = FALSE;
	
	 m_nMicroScope = 100;

     m_bIsDataGather = TRUE;

}


void CMorphaDetectDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMorphaDetectDLG)
	DDX_Control(pDX, IDC_COMBO_SHOWOPT, m_wndCbxShowOpt);
	DDX_Control(pDX, IDC_CHECK_RECTANGLE, m_wndChkRectangle);
	DDX_Text(pDX, IDC_STATIC_AREA, m_strArea);
	DDX_Text(pDX, IDC_STATIC_WIDTH, m_strWidth);
	DDX_Text(pDX, IDC_STATIC_PERIMETER, m_strPerimeter);
	DDX_Text(pDX, IDC_STATIC_ELLIPTICITY, m_strEllipticity);
	DDX_Text(pDX, IDC_STATIC_EXTENSION, m_strExtension);
	DDX_Text(pDX, IDC_STATIC_HEAD_AREA, m_strHeadArea);
	DDX_Text(pDX, IDC_STATIC_HEAD_PERFOR_AREA, m_strHeadPerforArea);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_DEVANGLE, m_strMitosmaDevangle);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_LENGHT, m_strMitsomaLength);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_WIDTH, m_strMitosomaWidth);
	DDX_Text(pDX, IDC_STATIC_PERFOR_AREA, m_strPerforArea);
	DDX_Text(pDX, IDC_STATIC_SYMMETRY, m_strSymmetry);
	DDX_Text(pDX, IDC_STATIC_STRINFO, m_strInfo);
	DDX_Text(pDX, IDC_STATIC_ISNORMAL, m_strIsNormal);
	DDX_Text(pDX, IDC_STATIC_RUGA, m_strRuga);
	DDX_Text(pDX, IDC_STATIC_LENGTH, m_strLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMorphaDetectDLG, CDialog)
	//{{AFX_MSG_MAP(CMorphaDetectDLG)
	ON_BN_CLICKED(IDC_BUTTON_MORPHADETECT_CONTINUE, OnButtonMorphadetectContinue)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_RECTANGLE, OnCheckRectangle)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOWOPT, OnSelchangeComboShowopt)
	ON_BN_CLICKED(IDC_BUTTON_MORPHADETECT_FINISH, OnButtonMorphadetectFinish)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DELETESPERM, OnButtonDeletesperm)
	ON_BN_CLICKED(IDC_BUTTON_MORPHADETECT_RESTORE, OnButtonMorphadetectRestore)
	ON_BN_CLICKED(IDC_BUTTON_MORPHADETECT_PARAMETER, OnButtonMorphadetectParameter)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorphaDetectDLG message handlers

BOOL CMorphaDetectDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndChkRectangle.SetCheck(0);
	m_wndCbxShowOpt.SetCurSel(1);
	
	GetDlgItem(IDC_BUTTON_MORPHADETECT_FINISH)->EnableWindow(FALSE);
	switch(theView3->m_eDetectMode)
	{
	case CView3::MORPHAIMAGE:
	case CView3::CAMERABMP:
	case CView3::VIDEOBMP:
		GetDlgItem(IDC_BUTTON_MORPHADETECT_RESTORE)->EnableWindow(FALSE);
		break;
	}

	GetDlgItem(IDC_BUTTON_DELETESPERM)->EnableWindow(m_nSelNO!=-1);	
	GetDlgItem(IDC_BUTTON_MORPHADETECT_CONTINUE)->SetFocus();

	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon,true);

	CRect rect,rect8;
	GetWindowRect(rect);
	int cxCur,cxMin;
	((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetColumnInfo(0,cxCur,cxMin);
	((CMainFrame*)GetParent())->m_wndSplitter.GetPane(0,1)->GetDlgItem(IDC_BUTTON_MORPHADETECT)->GetWindowRect(rect8);
	MoveWindow(cxCur+10,rect8.bottom,rect.right-rect.left,rect.bottom-rect.top);

    CString sPath(theStrPathApp), fileName = "UserDefined_Parameter_Data.txt";
	sPath+= "\\data\\";
	CreateDirectory((LPCTSTR)sPath, NULL);

	CString  filePathAndName; 
	filePathAndName = sPath+fileName;	
    InitializeStd(filePathAndName);

	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView3=(CView3*)pMainFrm->GetView3();
	SetWindowText("精子形态学检测_"+pView3->m_strDetectNO);

	m_PtAmRes.Init();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

CMorphaDetectDLG::~CMorphaDetectDLG()
{
	clearworkspace();
}

void CMorphaDetectDLG::OnButtonMorphadetectContinue() 
{
	if( m_BFinished)
	{
		switch(theView3->m_eDetectMode)
		{
		case CView3::MORPHAIMAGE:
			MessageBox("请继续打开形态学图片");
			return;
			break;
		case CView3::CAMERABMP:
			MessageBox("请首先恢复摄像机");
			return;
			break;
		}
	}

	GetDlgItem(IDC_BUTTON_MORPHADETECT_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MORPHADETECT_PARAMETER)->EnableWindow(FALSE);
	clearworkspace();


	// 读取图像
	CMainFrame* pMianFrm=(CMainFrame*)AfxGetMainWnd();
	CSpermView* pView=(CSpermView*)pMianFrm->GetView1();
	IplImage* corlorimg;
	CString szFileName;

	switch(theView3->m_eDetectMode)
	{
	case CView3::MORPHAIMAGE:
		corlorimg=pView->m_imgMorphaImg.GetImage();
		break;
	case CView3::CAMERABMP:	
		theView3->m_pwndLiveDetectDlg->m_cap.GrabOneFrame(TRUE);
		WaitForSingleObject(grabimage.m_hObject,INFINITE);
		theView3->m_pwndLiveDetectDlg->m_cap.Stop();

		// 取得运行程序的绝对路径     
		szFileName = theStrPathApp;
		
		szFileName += "\\donaldo.bmp";		
		corlorimg = cvLoadImage((LPCTSTR)szFileName, CV_LOAD_IMAGE_UNCHANGED);
		theView1->m_eViewShowMode = CSpermView::MORPHA_IMAGE;
		theView1->m_imgMorphaImg.CopyOf(corlorimg);
		theView1->Invalidate();
		break;
	}
	m_iplMorphaColorImg = cvCreateImage(cvGetSize(corlorimg),8,3);  // 0312
	m_iplTempImg  = cvCreateImage(cvGetSize(corlorimg),8,3);     // 0312
	m_iplMorphaGrayImage = cvCreateImage(cvGetSize(corlorimg),8,3);  // 0312
	cvCopyImage(corlorimg,m_iplMorphaColorImg);      // 0312
	
	int i,j;
	for(i=0;i<m_iplMorphaGrayImage->height;++i)
		for(j=0;j<m_iplMorphaGrayImage->width;++j)
		{
			unsigned char b=(unsigned char)corlorimg->imageData[i*corlorimg->widthStep+3*j+0];
			unsigned char g=(unsigned char)corlorimg->imageData[i*corlorimg->widthStep+3*j+1];
			unsigned char r=(unsigned char)corlorimg->imageData[i*corlorimg->widthStep+3*j+2];
			m_iplMorphaGrayImage->imageData[i*m_iplMorphaGrayImage->widthStep+j*3+0] =
			m_iplMorphaGrayImage->imageData[i*m_iplMorphaGrayImage->widthStep+j*3+1] =
			m_iplMorphaGrayImage->imageData[i*m_iplMorphaGrayImage->widthStep+j*3+2] 
				= (unsigned char)(0.114*b+0.587*g+0.299*r); 
		}  
	
    m_nSelNO = -1;
	
	static ThreadInfo ti;
	ti.pDlg=this;
	ti.pView1=(CSpermView*)((CMainFrame*)AfxGetMainWnd())->GetView1();
	
	AfxBeginThread((AFX_THREADPROC)ThreadHandle_AutoAnalyse,&ti);

	CString text;	
	GetDlgItemText(IDC_BUTTON_MORPHADETECT_CONTINUE,text);
	if(text == _T("开始检测"))
		SetDlgItemText(IDC_BUTTON_MORPHADETECT_CONTINUE,_T("继续检测"));
}


UINT CMorphaDetectDLG::ThreadHandle_AutoAnalyse(LPVOID lpParam)
{
	//CMorphaDetectDLG* pDlg=(CMorphaDetectDLG*)CWnd::FromHandle(((ThreadInfo*)lpParam)->hMorDlg);
	CMorphaDetectDLG* pDlg=((ThreadInfo*)lpParam)->pDlg;
		
	pDlg->ImagePreProcess();
	pDlg->AutoSeg();
	pDlg->SperMopyAnalyse();
	pDlg->DiagnoseAllSperm();
	pDlg->FormResultImage();
	
	DWORD dw=0;
	CPoint pt=(CPoint)pDlg->m_vSpermRegion[0].SpermCenterPos - theView1->GetScrollPosition();
	CPoint pt1=(CPoint)pDlg->m_vSpermRegion[0].SpermCenterPos;
	CPoint pt2=theView1->GetScrollPosition();
	dw |= pt.y;
	dw |= (pt.x<<16);
	theView1->SendMessage(WM_LBUTTONDOWN,0,(LPARAM)dw);

	pDlg->GetAllKindsSpermNum();
	pDlg->GetNumOfEachKindsOfAbnormalSperm();

	CString str;
	str.Format("目前一共检测了%d个视野,%d个精子,其中%d个正常,%d个异常.",
		pDlg->m_nVideoFiled,pDlg->m_MhNum.AbnormalSpermNum+pDlg->m_MhNum.NormalSpermNum,
		pDlg->m_MhNum.NormalSpermNum,pDlg->m_MhNum.AbnormalSpermNum);
	pDlg->GetDlgItem(IDC_DETECT_INFO)->SetWindowText(str);
	pDlg->GetDlgItem(IDC_BUTTON_MORPHADETECT_FINISH)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_MORPHADETECT_CONTINUE)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_MORPHADETECT_PARAMETER)->EnableWindow(TRUE);
	// 0415
	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERABMP:
		pDlg->GetDlgItem(IDC_BUTTON_MORPHADETECT_RESTORE)->EnableWindow(TRUE);
		break;
	}
	// 0415
	theView1->m_eViewShowMode = CSpermView::MORPHA_RESULT;
	theView1->Invalidate();
	theView3->m_pwndMorphaDetDlg->m_BFinished = TRUE;
	return 0;
}

void CMorphaDetectDLG::ImagePreProcess()
{
	cvCopyImage(m_iplMorphaColorImg,m_iplTempImg);
	cvSmooth(m_iplTempImg,m_iplTempImg,CV_MEDIAN);
	cvSmooth(m_iplTempImg,m_iplTempImg,CV_GAUSSIAN);
}

void CMorphaDetectDLG::AutoSeg()
{
    int i,j;
	IplImage* srcImage;
	IplImage* dstImage;
	srcImage = cvCreateImage(cvGetSize(m_iplMorphaColorImg),8,1);   
	dstImage = cvCreateImage(cvGetSize(m_iplMorphaColorImg),8,1);   

	NewDIBImage(m_lpBMMorphaInfo,m_lpBMMorphaData,srcImage->width,srcImage->height,3);

	int lw = WIDTHBYTES(m_lpBMMorphaInfo->biWidth * m_lpBMMorphaInfo->biBitCount);

	for(i=0;i<m_iplTempImg->height;++i)
		for(j=0;j<m_iplTempImg->width;++j)
		{
			unsigned char b=(unsigned char)m_iplTempImg->imageData[i*m_iplTempImg->widthStep+3*j+0];
			unsigned char g=(unsigned char)m_iplTempImg->imageData[i*m_iplTempImg->widthStep+3*j+1];
			unsigned char r=(unsigned char)m_iplTempImg->imageData[i*m_iplTempImg->widthStep+3*j+2];
			unsigned char gray = (unsigned char)(0.114*b+0.587*g+0.299*r);
			srcImage->imageData[i*srcImage->widthStep+j] = gray;

			m_lpBMMorphaData[(m_iplTempImg->height-1-i)*lw + j*3 + 0] = gray;
			m_lpBMMorphaData[(m_iplTempImg->height-1-i)*lw + j*3 + 1] = gray;
			m_lpBMMorphaData[(m_iplTempImg->height-1-i)*lw + j*3 + 2] = gray;
		}

	SetDlgItemText(IDC_ANALYSEPROGRESS,"正在搜索精子，请稍候...");
	vector<REGION> vSpermResult;
	::ImageSeg(srcImage, dstImage, vSpermResult);


	int n = vSpermResult.size();
	int sz=m_vSpermRegion.size();
	m_vSpermRegion.reserve(n);
	
	for(i=0; i<n; ++i)
	{
        if(vSpermResult[i].region.size() <=0 ||
		   vSpermResult[i].region.size() >= m_nMaxArea || 
		   !( (vSpermResult[i].centerpoint.x >= m_nEdgeThick) &&
		      (vSpermResult[i].centerpoint.x <= srcImage->width - m_nEdgeThick) &&
			  (vSpermResult[i].centerpoint.y >= m_nEdgeThick) &&
		      (vSpermResult[i].centerpoint.y <= srcImage->height - m_nEdgeThick) ) 
		  )
        {   continue;  } 

		int m = vSpermResult[i].region.size();
		SpermRegion  sr;
		CPoint p;
		p.x = vSpermResult[i].centerpoint.y;
		p.y = vSpermResult[i].centerpoint.x;
		sr.SpermCenterPos = p;
		sr.Region.reserve(m);
		for(int j=0; j<m; ++j)
		{	
			p.x = vSpermResult[i].region[j].y;
            p.y = vSpermResult[i].region[j].x;
			sr.Region.push_back(p);
		}	
		m_vSpermRegion.push_back(sr);
	}
	
    cvReleaseImage(&srcImage);
    cvReleaseImage(&dstImage);
}

void CMorphaDetectDLG::SperMopyAnalyse()
{
	int spermnum=m_vSpermRegion.size();
	int i,j;
	int  width, height, Oldlw, Newlw;
	long imgSize;
 	width  = 2*m_nR; 
	height = 2*m_nR;
    Oldlw     = WIDTHBYTES(m_lpBMMorphaInfo->biWidth * m_lpBMMorphaInfo->biBitCount);
    Newlw     = WIDTHBYTES(width * m_lpBMMorphaInfo->biBitCount);
	imgSize = Newlw*height;
	for(i=0;i<spermnum;++i)
	{
		CString info;
		info.Format("正在分析第(%d / %d)个精子。",i+1,spermnum);
		SetDlgItemText(IDC_ANALYSEPROGRESS,info);
        LPBITMAPINFOHEADER lpBmpInfo;
		LPBYTE             lpBmpData;

		lpBmpInfo = (LPBITMAPINFOHEADER)new unsigned char[sizeof(BITMAPINFOHEADER)];
        memcpy(lpBmpInfo, m_lpBMMorphaInfo, sizeof(BITMAPINFOHEADER));
        lpBmpInfo->biHeight = height;
		lpBmpInfo->biWidth  = width;
		lpBmpInfo->biSizeImage = imgSize;

        lpBmpData = (LPBYTE)new unsigned char[lpBmpInfo->biSizeImage];
		memset(lpBmpData, 255, lpBmpInfo->biSizeImage);

		long heightOffset = m_lpBMMorphaInfo->biHeight - (m_vSpermRegion[i].SpermCenterPos.x + m_nR);
	    long widthOffset  = m_vSpermRegion[i].SpermCenterPos.y - m_nR;

		SpermImage OSpermImage;
		NewDIBImage(OSpermImage.m_lpBmpInfo,OSpermImage.m_lpBmpData,
			      lpBmpInfo->biWidth,lpBmpInfo->biHeight,lpBmpInfo->biBitCount/8 );	 
		for(j=1; j<=height; j++)
		{
			int k;
			for(k=1; k<=width; k++)
			{
				 int jj = j-1+heightOffset;
 				 int ii = k-1+widthOffset;

                 if( ii<0 || ii >= m_lpBMMorphaInfo->biWidth ||
			         jj<0 || jj >= m_lpBMMorphaInfo->biHeight )
				   continue;
				 int ch;
				*(lpBmpData + (j-1)*Newlw + (k-1)*3 + 0) = *(m_lpBMMorphaData + jj*Oldlw + ii*3 + 0);
				*(lpBmpData + (j-1)*Newlw + (k-1)*3 + 1) = *(m_lpBMMorphaData + jj*Oldlw + ii*3 + 1);
				*(lpBmpData + (j-1)*Newlw + (k-1)*3 + 2) = *(m_lpBMMorphaData + jj*Oldlw + ii*3 + 2);

                for(ch=0;ch<m_iplMorphaColorImg->nChannels;++ch)
					OSpermImage.m_lpBmpData[(j-1)*Newlw + (k-1)*m_iplMorphaColorImg->nChannels+ch] 
					   = m_iplMorphaColorImg->imageData[(m_iplMorphaColorImg->height-1-jj)*m_iplMorphaColorImg->widthStep+
					                                    ii*m_iplMorphaColorImg->nChannels+ ch ];
			}
		}
		m_vOSpermImage.push_back(OSpermImage);
        vector<POINT> RegionPoints;
		CPoint p;
		int m = m_vSpermRegion[i].Region.size();

		for(j=0; j<m; j++)
		{									
			p.x = m_lpBMMorphaInfo->biHeight - m_vSpermRegion[i].Region[j].x - heightOffset;
			p.y = m_vSpermRegion[i].Region[j].y - widthOffset;
			if( p.x>=0 && p.x< height && p.y>=0 && p.y<width )		
				RegionPoints.push_back(p);			
		}	    
		
	//	if(i==5) aaa = 1;

		m_pSigSpermRecog = new CWraper(lpBmpInfo,lpBmpData,RegionPoints,m_nR);
        m_pSigSpermRecog->OnSperMopySeg();
		m_pSigSpermRecog->OnSperMopyParamCalc();
			
	    memcpy(lpBmpInfo, m_pSigSpermRecog->m_lpDibIHPro, sizeof(BITMAPINFOHEADER));
		memcpy(lpBmpData, m_pSigSpermRecog->m_lpDibDataPro, m_pSigSpermRecog->m_lpDibIHPro->biSizeImage);
 
		SpermImage RSpermImage;
        RSpermImage.m_lpBmpInfo = lpBmpInfo;
        RSpermImage.m_lpBmpData = lpBmpData;
		m_vRSpermImage.push_back(RSpermImage);

		SpermImage RSpermPaddedImage;
		NewDIBImage(RSpermPaddedImage.m_lpBmpInfo,RSpermPaddedImage.m_lpBmpData,
			  lpBmpInfo->biWidth,lpBmpInfo->biHeight,lpBmpInfo->biBitCount/8);
        memcpy(RSpermPaddedImage.m_lpBmpData,lpBmpData,lpBmpInfo->biSizeImage);
		for(j=1; j<=height; j++)
		{
			int k;
			for(k=1; k<=width; k++)
			{
				 int jj = j-1+heightOffset;
 				 int ii = k-1+widthOffset;

                 if( ii<0 || ii >= m_lpBMMorphaInfo->biWidth ||
			         jj<0 || jj >= m_lpBMMorphaInfo->biHeight )
				   continue;
				 int ch;
			   if( PIEXL(lpBmpData + (j-1)*Newlw + (k-1)*3) == RGB(255,255,255))
                for(ch=0;ch<m_iplMorphaColorImg->nChannels;++ch)
					RSpermPaddedImage.m_lpBmpData[(j-1)*Newlw + (k-1)*m_iplMorphaColorImg->nChannels+ch] 
					   = m_iplMorphaColorImg->imageData[(m_iplMorphaColorImg->height-1-jj)*m_iplMorphaColorImg->widthStep+
					                                    ii*m_iplMorphaColorImg->nChannels+ ch ];
			}
		}		
		m_vRSpermPaddedImage.push_back(RSpermPaddedImage);
		{
			SpermImage si;
			NewDIBImage(si.m_lpBmpInfo,si.m_lpBmpData,width,height,3);
			memcpy(si.m_lpBmpData, RSpermPaddedImage.m_lpBmpData, si.m_lpBmpInfo->biSizeImage);
			int k;
			COLORREF  piexlArray[8];
			for(int ii=1; ii<height-1; ii++)
			{
				for(int jj=1; jj<width-1; jj++)
				{		
					piexlArray[0] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii-1)*Newlw + (jj-1)*3);
					piexlArray[1] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii-1)*Newlw + (jj-0)*3);
					piexlArray[2] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii-1)*Newlw + (jj+1)*3);
					piexlArray[3] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii-0)*Newlw + (jj-1)*3);
					piexlArray[4] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii-0)*Newlw + (jj+1)*3);
					piexlArray[5] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii+1)*Newlw + (jj-1)*3);
					piexlArray[6] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii+1)*Newlw + (jj-0)*3);
					piexlArray[7] = PIEXL(RSpermPaddedImage.m_lpBmpData+ (ii+1)*Newlw + (jj+1)*3);
					BOOL  state = TRUE;
					for(k=0; k<8-1; k++)
						if( piexlArray[k] != piexlArray[k+1])
							state = FALSE;
						if( !(piexlArray[k] == RED_PIEXL || 
							piexlArray[k] == GREEN_PIEXL || 
							piexlArray[k] == BLUE_PIEXL  ) )
							state = FALSE;
						
						if( state )
						{
							*(si.m_lpBmpData + ii*Newlw + jj*3 + 0) = *(OSpermImage.m_lpBmpData+ ii*Newlw + jj*3 + 0);
							*(si.m_lpBmpData + ii*Newlw + jj*3 + 1) = *(OSpermImage.m_lpBmpData+ ii*Newlw + jj*3 + 1);
							*(si.m_lpBmpData + ii*Newlw + jj*3 + 2) = *(OSpermImage.m_lpBmpData+ ii*Newlw + jj*3 + 2);			               
						}
				}    
			}
			m_vRSpermEdgeImage.push_back(si);
		}
		SperMopyParameter smp = PiexlValueToReallity(m_pSigSpermRecog->m_SperMopyParameter);
		m_vSperMopyParameter.push_back(smp);
		// 诊断正常与否
		IsSperNormal bIsNor; 
	    bIsNor  = IsNormal(smp);
		m_vbIsNormal.push_back(bIsNor);

		delete  m_pSigSpermRecog;
		m_pSigSpermRecog = NULL;	
	}
	CString info;
	info.Format("分析完成。");
	SetDlgItemText(IDC_ANALYSEPROGRESS,info);

}

SperMopyParameter CMorphaDetectDLG::PiexlValueToReallity(SperMopyParameter &smp)
{
	const double DPI = 96;
	const double mm_px = 1/DPI*25.4; // 每像素多少毫米
	double xishu = mm_px*1000.0/(m_nMicroScope); // 换算成微米

    SperMopyParameter smpReallity;
	smpReallity.m_length = smp.m_length*xishu, 
	smpReallity.m_width	= smp.m_width*xishu,
	smpReallity.m_area =	smp.m_area*xishu*xishu,
	smpReallity.m_ellipticity =	smp.m_ellipticity,
	smpReallity.m_perfor_area =	smp.m_perfor_area*xishu*xishu,
	smpReallity.m_head_area	= smp.m_head_area*xishu*xishu,
	smpReallity.m_perimeter = smp.m_perimeter*xishu,
	smpReallity.m_head_perfor_area = smp.m_head_perfor_area,
	smpReallity.m_tail_length = smp.m_tail_length*xishu,
	smpReallity.m_tail_width = smp.m_tail_width*xishu,
	smpReallity.m_tail_angle =	smp.m_tail_angle,
	smpReallity.m_extension =	smp.m_extension,
	smpReallity.m_symmetry = smp.m_symmetry,
	smpReallity.m_ruga = smp.m_ruga;
	return smpReallity;
}

// 对所有精子根据设定的形态学标准进行诊断
void CMorphaDetectDLG::DiagnoseAllSperm()
{
    int i, n;
	n = m_vSpermRegion.size();
	for(i=0; i<n; i++)
	{
		m_vbIsNormal[i] = IsNormal(m_vSperMopyParameter[i]);
		
		m_MhNum.normal_lengthNum += m_vbIsNormal[i].IsNormalVector[1];
		m_MhNum.normal_widthNum +=  m_vbIsNormal[i].IsNormalVector[2];
		m_MhNum.normal_areaNum += m_vbIsNormal[i].IsNormalVector[3];
		m_MhNum.normal_ellipticityNum += m_vbIsNormal[i].IsNormalVector[4];
		m_MhNum.normal_perfor_areaNum += m_vbIsNormal[i].IsNormalVector[5];
		m_MhNum.normal_head_areaNum += m_vbIsNormal[i].IsNormalVector[6];
		m_MhNum.normal_perimeterNum += m_vbIsNormal[i].IsNormalVector[7];
		m_MhNum.normal_head_perfor_areaNum += m_vbIsNormal[i].IsNormalVector[8];

// 尾部
		m_MhNum.normal_tail_lengthNum += m_vbIsNormal[i].IsNormalVector[9];
		m_MhNum.normal_tail_widthNum += m_vbIsNormal[i].IsNormalVector[10];
		m_MhNum.normal_tail_angleNum += m_vbIsNormal[i].IsNormalVector[11];

		m_MhNum.normal_extensionNum += m_vbIsNormal[i].IsNormalVector[12];
		m_MhNum.normal_symmetryNum += m_vbIsNormal[i].IsNormalVector[13];
		m_MhNum.normal_rugaNum += m_vbIsNormal[i].IsNormalVector[14];

	}
	
}

void CMorphaDetectDLG::GetNumOfEachKindsOfAbnormalSperm()
{
	int i, n;
	n = m_vSpermRegion.size();
	for(i=0; i<n; i++)
	{
		if(m_vbIsNormal[i].isDeleted) continue;

		if( (!m_vbIsNormal[i].IsNormalVector[4] ||
			 !m_vbIsNormal[i].IsNormalVector[5] ||
			 !m_vbIsNormal[i].IsNormalVector[8] )
			) // 头
			m_PtAmRes.nHeadAbnormalNum++;

		if( ( !m_vbIsNormal[i].IsNormalVector[1] ||
			!m_vbIsNormal[i].IsNormalVector[2] ||
			!m_vbIsNormal[i].IsNormalVector[3] ||
			!m_vbIsNormal[i].IsNormalVector[4] ||
			!m_vbIsNormal[i].IsNormalVector[6] ||
			!m_vbIsNormal[i].IsNormalVector[7] ||
			!m_vbIsNormal[i].IsNormalVector[8] ||
			!m_vbIsNormal[i].IsNormalVector[12] ||
			!m_vbIsNormal[i].IsNormalVector[13] ||
			!m_vbIsNormal[i].IsNormalVector[14] )
			) // 体
			m_PtAmRes.nBodyAbnormalNum++;
			
		if( ( !m_vbIsNormal[i].IsNormalVector[9] ||
			!m_vbIsNormal[i].IsNormalVector[10] ||
			!m_vbIsNormal[i].IsNormalVector[11] )
			) // 尾
			m_PtAmRes.nTailAbnormalNum++;
	}
}

IsSperNormal CMorphaDetectDLG::IsNormal(SperMopyParameter tSperMopyParameter)
{
	  IsSperNormal  bRe;
	  BOOL          bFinalRe;
	  int i, n;
	  double  Inf_Parameter[] = {
							 m_stdInf_SperMopyParameter.m_length,
							 m_stdInf_SperMopyParameter.m_width,
							 m_stdInf_SperMopyParameter.m_area,
							 
							 m_stdInf_SperMopyParameter.m_ellipticity,
							 m_stdInf_SperMopyParameter.m_perfor_area,
							 m_stdInf_SperMopyParameter.m_head_area,
							 m_stdInf_SperMopyParameter.m_perimeter,				

							 m_stdInf_SperMopyParameter.m_tail_length,
							 m_stdInf_SperMopyParameter.m_tail_width,
							 m_stdInf_SperMopyParameter.m_tail_angle,
							 
							 m_stdInf_SperMopyParameter.m_head_perfor_area,	
							 m_stdInf_SperMopyParameter.m_extension, 
							 m_stdInf_SperMopyParameter.m_symmetry,
							 m_stdInf_SperMopyParameter.m_ruga
	  };

	  double  Sup_Parameter[] = {
							 m_stdSup_SperMopyParameter.m_length,
							 m_stdSup_SperMopyParameter.m_width,
							 m_stdSup_SperMopyParameter.m_area,
							 
							 m_stdSup_SperMopyParameter.m_ellipticity,
							 m_stdSup_SperMopyParameter.m_perfor_area,
							 m_stdSup_SperMopyParameter.m_head_area,
							 m_stdSup_SperMopyParameter.m_perimeter,
							
							 m_stdSup_SperMopyParameter.m_tail_length,
							 m_stdSup_SperMopyParameter.m_tail_width,
							 m_stdSup_SperMopyParameter.m_tail_angle,
 
							 m_stdSup_SperMopyParameter.m_head_perfor_area,	
							 m_stdSup_SperMopyParameter.m_extension,
							 m_stdSup_SperMopyParameter.m_symmetry,
							 m_stdSup_SperMopyParameter.m_ruga
	  };

	  double  Parameter[] = {
							 tSperMopyParameter.m_length,
							 tSperMopyParameter.m_width,
							 tSperMopyParameter.m_area,
							 
							 tSperMopyParameter.m_ellipticity,
							 tSperMopyParameter.m_perfor_area,
							 tSperMopyParameter.m_head_area,
							 tSperMopyParameter.m_perimeter,						

							 tSperMopyParameter.m_tail_length,
							 tSperMopyParameter.m_tail_width,
							 tSperMopyParameter.m_tail_angle,
 
							 tSperMopyParameter.m_head_perfor_area,	
							 tSperMopyParameter.m_extension,
							 tSperMopyParameter.m_symmetry,
							 tSperMopyParameter.m_ruga
	  };

     // FALSE: 异常;
	 // TRUE : 正常;
     bFinalRe = TRUE; 
	 n = 1+nPARAMETER;	  
	 
	 for(i=0; i<n-1; i++) 
	 {
		 bRe.IsNormalVector[i+1] = (Parameter[i] <= Sup_Parameter[i] &&
			                        Parameter[i] >= Inf_Parameter[i]  ) ? TRUE : FALSE;
		 bFinalRe *= bRe.IsNormalVector[i+1];
	 }

	 bRe.IsNormalVector[0] = bFinalRe;  // bool数组第一项存放总的判断结果 
     return bRe;   
}

bool CMorphaDetectDLG::IsBoundary(CPoint p, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData)
{
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1,
					};
	int i, row, col;
	LPBYTE lpSrc;
	int width  = lpBmInfo->biWidth;
	int height = lpBmInfo->biHeight;
	int channel = lpBmInfo->biBitCount / 8 ;
	int lw = WIDTHBYTES(width * lpBmInfo->biBitCount);

	COLORREF  piexlArray[4];

	for(i=0; i<4; i++)
	{
        row = p.y + dir[i][0];
		col = p.x + dir[i][1];
        lpSrc = lpBmData + row*lw + col*channel;
		if( row>=0 && row<height && col>=0 && col<width )
			piexlArray[i] = PIEXL(lpBmData + row*lw + col*channel);
		else 
			piexlArray[i] = PIEXL(lpBmData+ p.y*lw+ p.x*channel);
	}
	for(i=0; i<4; i++)
	   if( piexlArray[i] != PIEXL(lpBmData+ p.y*lw+ p.x*channel) )
           return true;
	return false;
}

void CMorphaDetectDLG::FormResultImage()
{
	int i, j, k, n;
	long ii, jj;

	if(m_lpResBmInfoSrc != NULL) delete m_lpResBmInfoSrc;
	m_lpResBmInfoSrc = new BITMAPINFOHEADER;
	m_lpResBmInfoSrc->biBitCount = 24;
	m_lpResBmInfoSrc->biHeight   = m_iplMorphaColorImg->height;
	m_lpResBmInfoSrc->biWidth    = m_iplMorphaColorImg->width;
	m_lpResBmInfoSrc->biSizeImage =
		                   DIBWIDTHBYTES(*m_lpResBmInfoSrc)*m_iplMorphaColorImg->height; // 0312
	m_lpResBmInfoSrc->biPlanes = 1;
	m_lpResBmInfoSrc->biXPelsPerMeter = 0;
	m_lpResBmInfoSrc->biYPelsPerMeter = 0;
	m_lpResBmInfoSrc->biCompression=0;
	m_lpResBmInfoSrc->biClrUsed=0;
	m_lpResBmInfoSrc->biClrImportant=0;
	m_lpResBmInfoSrc->biSize  = sizeof(BITMAPINFOHEADER);
	//{ 0312
	int widthstep=DIBWIDTHBYTES(*m_lpResBmInfoSrc);   // 0312
	//} 0312

	if(m_lpResBmDataSrc != NULL) delete []m_lpResBmDataSrc;
	m_lpResBmDataSrc = new BYTE [widthstep*m_iplMorphaColorImg->height ];

	//{  0312  // 初始化m_lpResBmDataSrc为整张图像
	for(i=0;i<m_iplMorphaColorImg->height;++i)
		for(j=0;j<m_iplMorphaColorImg->width;++j)
		{
				memcpy(&m_lpResBmDataSrc[(m_lpResBmInfoSrc->biHeight-1-i)*widthstep+j*3 + 0],
				       &m_iplMorphaColorImg->imageData[i*m_iplMorphaColorImg->widthStep+j*3],3);   // 0312
		}
	//}  0312

	if(m_lpResEdgeBmInfoSrc != NULL) 
	{
		delete m_lpResEdgeBmInfoSrc; 
		m_lpResEdgeBmInfoSrc = NULL;
	}
	m_lpResEdgeBmInfoSrc = new BITMAPINFOHEADER;
	memcpy(m_lpResEdgeBmInfoSrc,m_lpResBmInfoSrc,sizeof(BITMAPINFOHEADER));

	if(m_lpResEdgeBmDataSrc != NULL) delete []m_lpResEdgeBmDataSrc;
	m_lpResEdgeBmDataSrc = new BYTE [widthstep*m_iplMorphaColorImg->height ];
	//{ 0312
	memcpy(m_lpResEdgeBmDataSrc,m_lpResBmDataSrc,widthstep*m_iplMorphaColorImg->height);
	//} 0312
	int  width, height, NewWidth, NewHeight, Oldlw, Newlw;
	long heightOffset, widthOffset;
	width = m_lpResBmInfoSrc->biWidth;
	height= m_lpResBmInfoSrc->biHeight;
 	NewWidth  = 2*m_nR;
	NewHeight = 2*m_nR;
    Oldlw     = WIDTHBYTES(m_lpResBmInfoSrc->biWidth * m_lpResBmInfoSrc->biBitCount);
    Newlw     = WIDTHBYTES(NewWidth * 24);

	//
	if(m_vOSpermImage.size()>0)
	{
		const int nSpermNum = 2*m_vOSpermImage.size();
		const int nC = 6;		// 一行nC个图
		int nW = nC*NewWidth;
		int nH = nSpermNum%nC ? (nSpermNum/nC+1)*NewHeight : (nSpermNum/nC)*NewHeight;
		int nWideStep = WIDTHBYTES(nW*24);
		m_rImg = cvCreateImage(cvSize(nW,nH),8,3);
		
		for(i=0; i<nH; ++i)
		{	
			for(j=0; j<nW; ++j)
			{
				m_rImg->imageData[i*m_rImg->widthStep+j*3+0] = (unsigned char)204;
				m_rImg->imageData[i*m_rImg->widthStep+j*3+1] = (unsigned char)232;
				m_rImg->imageData[i*m_rImg->widthStep+j*3+2] = (unsigned char)207;
			}
		}	
		for(k=0; k<nSpermNum; k++)
		{
			if(find(m_lsOperateIndex.begin(),m_itListIndex, k/2) != m_itListIndex) continue;
			int ni,nj;
			ni = k%nC;
			nj = k/nC;
			char* lpSrc;
			for(j=0; j<NewHeight; j++)
			{
				for(i=0; i<NewWidth; i++)
				{
					lpSrc = m_rImg->imageData + (nj*NewHeight+j)*nWideStep + (ni*NewWidth+i)*3;
					if( k%2 == 0 ) 
					{
						lpSrc[0] = *(m_vOSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 0);
						lpSrc[1] = *(m_vOSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 1);
						lpSrc[2] = *(m_vOSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 2);
					}
					else
					{
						lpSrc[0] = *(m_vRSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 0);
						lpSrc[1] = *(m_vRSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 1);
						lpSrc[2] = *(m_vRSpermImage[k/2].m_lpBmpData + j*Newlw + i*3 + 2);
					}
				}
			}
			CvPoint pt1 = cvPoint(ni*NewWidth+2, nj*NewHeight+2);
			CvPoint pt2 = cvPoint((ni+1)*NewWidth-2, (nj+1)*NewHeight-2);
			if(m_vbIsNormal[k/2].IsNormalVector[0] == TRUE)  // 正常的用蓝色标记，异常的用红色标记
				cvRectangle(m_rImg, pt1, pt2, CV_RGB(0,0,255), 2);
			else
				cvRectangle(m_rImg, pt1, pt2, CV_RGB(255,0,0), 2);
		}
		
		CvSize imgsz=cvGetSize(m_rImg);
		imgsz.width = imgsz.width*2/3;
		imgsz.height = imgsz.height*2/3;
		IplImage* resimage=cvCreateImage(imgsz,8,3);
		cvResize(m_rImg,resimage);
		cvReleaseImage(&m_rImg);m_rImg=cvCreateImage(imgsz,8,3);
		cvCopyImage(resimage,m_rImg);
		cvReleaseImage(&resimage);
	}

	//

	/* ------ 形成涂色结果图 ------ */
	LPBYTE  lpSrc;
	n = m_vSpermRegion.size();
	for(k=0; k<n; k++)
	{		
		if(find(m_lsOperateIndex.begin(),m_itListIndex,k) != m_itListIndex) continue;
	   heightOffset = m_lpResBmInfoSrc->biHeight - (m_vSpermRegion[k].SpermCenterPos.x + m_nR);
	   widthOffset  = m_vSpermRegion[k].SpermCenterPos.y - m_nR;

		for(j=1; j<=NewHeight; j++)
		{
			for(i=1; i<=NewWidth; i++)
			{
				 jj = j-1+heightOffset;
 				 ii = i-1+widthOffset;
             
				 lpSrc = m_vRSpermImage[k].m_lpBmpData + (j-1)*Newlw + (i-1)*3;

                 if( ii>=0 && ii < m_lpResBmInfoSrc->biWidth  &&
			         jj>=0 && jj < m_lpResBmInfoSrc->biHeight &&
					 PIEXL(lpSrc) != WHITE_PIEXL/* 透明背景色 */ )
				 {
					 *(m_lpResBmDataSrc+ jj*Oldlw+ ii*3 + 0) = *(lpSrc + 0);
					 *(m_lpResBmDataSrc+ jj*Oldlw+ ii*3 + 1) = *(lpSrc + 1);
					 *(m_lpResBmDataSrc+ jj*Oldlw+ ii*3 + 2) = *(lpSrc + 2);	
				 }				 
			}
		}
	}

	/* ------ 形成边缘标记结果图 ------ */
	unsigned char* lpTemp = new unsigned char[m_lpResBmInfoSrc->biSizeImage];
	memcpy(lpTemp, m_lpResBmDataSrc, m_lpResBmInfoSrc->biSizeImage);

	COLORREF  piexlArray[8];
    for(i=1; i<height-1; i++)
	{
		for(j=1; j<width-1; j++)
		{		
           	piexlArray[0] = PIEXL(m_lpResBmDataSrc+ (i-1)*Oldlw + (j-1)*3);
			piexlArray[1] = PIEXL(m_lpResBmDataSrc+ (i-1)*Oldlw + (j-0)*3);
			piexlArray[2] = PIEXL(m_lpResBmDataSrc+ (i-1)*Oldlw + (j+1)*3);
			piexlArray[3] = PIEXL(m_lpResBmDataSrc+ (i-0)*Oldlw + (j-1)*3);
			piexlArray[4] = PIEXL(m_lpResBmDataSrc+ (i-0)*Oldlw + (j+1)*3);
			piexlArray[5] = PIEXL(m_lpResBmDataSrc+ (i+1)*Oldlw + (j-1)*3);
			piexlArray[6] = PIEXL(m_lpResBmDataSrc+ (i+1)*Oldlw + (j-0)*3);
			piexlArray[7] = PIEXL(m_lpResBmDataSrc+ (i+1)*Oldlw + (j+1)*3);

			BOOL  state = TRUE;
			for(k=0; k<8-1; k++)
				if( piexlArray[k] != piexlArray[k+1])
					state = FALSE;
            if( !(piexlArray[k] == RED_PIEXL   || 
				  piexlArray[k] == GREEN_PIEXL || 
				  piexlArray[k] == BLUE_PIEXL  ) )
				  state = FALSE;
			 				
            if( state )
            {
                *(lpTemp + i*Oldlw + j*3 + 0) = *(m_lpResEdgeBmDataSrc+ i*Oldlw + j*3 + 0);
                *(lpTemp + i*Oldlw + j*3 + 1) = *(m_lpResEdgeBmDataSrc+ i*Oldlw + j*3 + 1);
                *(lpTemp + i*Oldlw + j*3 + 2) = *(m_lpResEdgeBmDataSrc+ i*Oldlw + j*3 + 2);			               
			}
		}    
	}
	memcpy(m_lpResEdgeBmDataSrc, lpTemp, m_lpResEdgeBmInfoSrc->biSizeImage);
	
	delete []lpTemp;
	lpTemp = NULL;

	theView1->m_bAnalyseFinished = TRUE;
}

void CMorphaDetectDLG::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CStatic* pShowImage=(CStatic*)GetDlgItem(IDC_STATIC_SHOWIMAGE);
	pShowImage->Invalidate();
	pShowImage->UpdateWindow();
	CRect rc;
	pShowImage->GetWindowRect(rc);
	int w=rc.right-rc.left;
	int h=rc.bottom-rc.top;	
	CPoint sp=theView1->GetScrollPosition();
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	memDC.SelectObject(&bitmap);
	GetDlgItem(IDC_BUTTON_DELETESPERM)->EnableWindow(m_nSelNO!=-1);
	switch( m_wndCbxShowOpt.GetCurSel())
	{
	case ORIGINIMAGE: // 原图
		if(m_nSelNO != -1 && m_vOSpermImage[m_nSelNO].isDeleted == FALSE)
		{	
			StretchDIBits(memDC.m_hDC,0,0,w,h,
				0,0,2*m_nR,2*m_nR,m_vOSpermImage[m_nSelNO].m_lpBmpData
				    ,(BITMAPINFO*)m_vOSpermImage[m_nSelNO].m_lpBmpInfo
					,DIB_RGB_COLORS,SRCCOPY);
		}	
	break;
	case PADDEDIMAGE: // 填充色结果图
		if(m_nSelNO != -1 && m_vRSpermPaddedImage[m_nSelNO].isDeleted == FALSE)
		{	
			StretchDIBits(memDC.m_hDC,0,0,w,h,
				0,0,2*m_nR,2*m_nR,m_vRSpermPaddedImage[m_nSelNO].m_lpBmpData,
				(BITMAPINFO*)m_vRSpermPaddedImage[m_nSelNO].m_lpBmpInfo,
				DIB_RGB_COLORS,SRCCOPY);
		}
		break;   
	case EDGEIMAGE: // 边缘结果图
		if(m_nSelNO != -1 && m_vRSpermEdgeImage[m_nSelNO].isDeleted == FALSE)
		{	
			StretchDIBits(memDC.m_hDC,0,0,w,h,
				0,0,2*m_nR,2*m_nR,m_vRSpermEdgeImage[m_nSelNO].m_lpBmpData,
				   (BITMAPINFO*)m_vRSpermEdgeImage[m_nSelNO].m_lpBmpInfo
				   ,DIB_RGB_COLORS,SRCCOPY
				 );
		}	
		break;
	}
	CDC* imgDC=pShowImage->GetDC();
	imgDC->BitBlt(0,0,rc.Width(),rc.Height(),&memDC,0,0,SRCCOPY);
	pShowImage->ReleaseDC(imgDC);
}

void CMorphaDetectDLG::OnCheckRectangle() 
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CSpermView* pView=(CSpermView*)pMainFrm->GetView1();
	pView->Invalidate();
}

void CMorphaDetectDLG::OnSelchangeComboShowopt() 
{
	// TODO: Add your control notification handler code here
	CRect rc1,rc2;
	GetWindowRect(rc1);
	GetDlgItem(IDC_STATIC_SHOWIMAGE)->GetWindowRect(rc2);
	InvalidateRect(CRect(rc2.left-rc1.left,rc2.right-rc1.left,
		                 rc2.top-rc1.top,rc2.bottom-rc1.top));
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CSpermView* pView=(CSpermView*)pMainFrm->GetView1();
	pView->Invalidate();
}

void CMorphaDetectDLG::InitializeStd(CString filename)
{
	 double _inf_length;
	 double _inf_area;
	 double _inf_ellipticity;
	 double _inf_perfor_area;
	 double _inf_head_area;
	 double _inf_perimeter;
	 double _inf_width;
	 double _inf_tail_length;
	 double _inf_tail_width;
	 double _inf_tail_angle;
	 double _inf_head_perfor_area;	
	 double _inf_extension;
	 double _inf_symmetry;
	 double _inf_ruga;
			
	 double _sup_length;
	 double _sup_area;
	 double _sup_ellipticity;
	 double _sup_perfor_area;
	 double _sup_head_area;
	 double _sup_perimeter;
	 double _sup_width;
	 double _sup_tail_length;
	 double _sup_tail_width;
	 double _sup_tail_angle;
	 double _sup_head_perfor_area;	
	 double _sup_extension;
	 double _sup_symmetry;
	 double _sup_ruga;
     int _ThreshAdjustPart;
	 int _ThreshAdjust;
	 double _GrowStep;

	 fstream  fin;
	 fin.open((LPCTSTR)filename, ios::in); 
	  fin>>_inf_length
		 >>_inf_width
	     >>_inf_area

		 >>_inf_ellipticity
		 >>_inf_perfor_area
		 >>_inf_head_area
		 >>_inf_perimeter	
		 
		 >>_inf_tail_length
		 >>_inf_tail_width
		 >>_inf_tail_angle

		 >>_inf_head_perfor_area	
		 >>_inf_extension
		 >>_inf_symmetry
		 >>_inf_ruga
		 
		 >>_sup_length
		 >>_sup_width
		 >>_sup_area

		 >>_sup_ellipticity
		 >>_sup_perfor_area
		 >>_sup_head_area
		 >>_sup_perimeter
		 
		 >>_sup_tail_length
		 >>_sup_tail_width
		 >>_sup_tail_angle

		 >>_sup_head_perfor_area	
		 >>_sup_extension
		 >>_sup_symmetry
		 >>_sup_ruga
	     >>m_nMicroScope
		 >>_ThreshAdjustPart
		 >>_ThreshAdjust
		 >>_GrowStep;
	fin.close();

   // 更新参数标准
 	m_stdInf_SperMopyParameter.m_length           = _inf_length;
	m_stdInf_SperMopyParameter.m_width            = _inf_width;
	m_stdInf_SperMopyParameter.m_area             = _inf_area;

	m_stdInf_SperMopyParameter.m_ellipticity      = _inf_ellipticity;
	m_stdInf_SperMopyParameter.m_perfor_area      = _inf_perfor_area;
	m_stdInf_SperMopyParameter.m_head_area        = _inf_head_area;
	m_stdInf_SperMopyParameter.m_perimeter        = _inf_perimeter;

	m_stdInf_SperMopyParameter.m_tail_length      = _inf_tail_length;
	m_stdInf_SperMopyParameter.m_tail_width       = _inf_tail_width;
	m_stdInf_SperMopyParameter.m_tail_angle       = _inf_tail_angle;

	m_stdInf_SperMopyParameter.m_head_perfor_area = _inf_head_perfor_area;
	m_stdInf_SperMopyParameter.m_extension        = _inf_extension;
	m_stdInf_SperMopyParameter.m_symmetry         = _inf_symmetry;
	m_stdInf_SperMopyParameter.m_ruga             = _inf_ruga;
	
	m_stdSup_SperMopyParameter.m_length           = _sup_length;
	m_stdSup_SperMopyParameter.m_width            = _sup_width;
	m_stdSup_SperMopyParameter.m_area             = _sup_area;

	m_stdSup_SperMopyParameter.m_ellipticity      = _sup_ellipticity;
	m_stdSup_SperMopyParameter.m_perfor_area      = _sup_perfor_area;
	m_stdSup_SperMopyParameter.m_head_area        = _sup_head_area;
	m_stdSup_SperMopyParameter.m_perimeter        = _sup_perimeter;

	m_stdSup_SperMopyParameter.m_tail_length      = _sup_tail_length;
	m_stdSup_SperMopyParameter.m_tail_width       = _sup_tail_width;
	m_stdSup_SperMopyParameter.m_tail_angle       = _sup_tail_angle;

	m_stdSup_SperMopyParameter.m_head_perfor_area = _sup_head_perfor_area;
	m_stdSup_SperMopyParameter.m_extension        = _sup_extension;
	m_stdSup_SperMopyParameter.m_symmetry         = _sup_symmetry;
	m_stdSup_SperMopyParameter.m_ruga             = _sup_ruga;

	gThreshAdjustPart = _ThreshAdjustPart;
	gThreshAdjust = _ThreshAdjust;
	gGrowStep = _GrowStep;

}

void CMorphaDetectDLG::clearworkspace()
{
	int i;
	if(m_iplMorphaColorImg) cvReleaseImage(&m_iplMorphaColorImg);
	if(m_iplMorphaGrayImage) cvReleaseImage(&m_iplMorphaGrayImage);
	if(m_iplTempImg) cvReleaseImage(&m_iplTempImg);
	if(m_rImg) cvReleaseImage(&m_rImg);

	if(m_lpBMMorphaInfo) DeleteDIBImage(m_lpBMMorphaInfo,m_lpBMMorphaData);
	if(m_lpResBmInfoSrc) DeleteDIBImage(m_lpResBmInfoSrc,m_lpResBmDataSrc);
	if(m_lpResEdgeBmInfoSrc) DeleteDIBImage(m_lpResEdgeBmInfoSrc,m_lpResEdgeBmDataSrc);

	m_vbIsNormal.clear();
	m_vSperMopyParameter.clear();

	for(i=0;i<m_vRSpermEdgeImage.size();++i)
	{
		DeleteDIBImage(m_vRSpermEdgeImage[i].m_lpBmpInfo, 
			m_vRSpermEdgeImage[i].m_lpBmpData);
	}
	m_vRSpermEdgeImage.clear();

	for(i=0;i<m_vRSpermImage.size();++i)
	{
		DeleteDIBImage(m_vRSpermImage[i].m_lpBmpInfo, 
			m_vRSpermImage[i].m_lpBmpData);
	}
	m_vRSpermImage.clear();

	for(i=0;i<m_vRSpermPaddedImage.size();++i)
	{
		DeleteDIBImage(m_vRSpermPaddedImage[i].m_lpBmpInfo,
			m_vRSpermPaddedImage[i].m_lpBmpData);
	}
	m_vRSpermPaddedImage.clear();

	for(i=0;i<m_vOSpermImage.size();++i)
	{
		DeleteDIBImage(m_vOSpermImage[i].m_lpBmpInfo,
			m_vOSpermImage[i].m_lpBmpData);
	}
	m_vOSpermImage.clear();

	for(i=0;i<m_vSpermRegion.size();++i)
		m_vSpermRegion[i].Region.clear();
	m_vSpermRegion.clear();

	m_lsOperateIndex.clear();
	m_itListIndex = m_lsOperateIndex.begin();
	
	m_BFinished = FALSE;

}

void CMorphaDetectDLG::ShowSperm(int iPos)
{
	m_nSelNO = iPos;
	Invalidate();
	theView1->Invalidate();
}

void CMorphaDetectDLG::GetAllKindsSpermNum()
{
	int i;
	for(i=0;i<m_vbIsNormal.size();++i)
		if(m_vbIsNormal[i].IsNormalVector[0] == TRUE )
			++m_MhNum.NormalSpermNum;
		else
			++m_MhNum.AbnormalSpermNum;
}

CString CMorphaDetectDLG::IntTOCString(int n)
{
	CString str;
	str.Format("%d",n);
	return str;
}

void CMorphaDetectDLG::GetSpermPartialAbnormalNum()
{
	m_PtAmRes.nTotalNum = m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum;
	m_PtAmRes.nTotalNormalNum = m_MhNum.NormalSpermNum;

	const int num = m_PtAmRes.nTotalNum;
	
	if( num > 0 ) {
		m_PtAmRes.dTotalNormalRatio  = m_PtAmRes.nTotalNormalNum / double(num);
		m_PtAmRes.dHeadAbnormalRatio = m_PtAmRes.nHeadAbnormalNum / double(num);
		m_PtAmRes.dBodyAbnormalRatio = m_PtAmRes.nBodyAbnormalNum / double(num);
		m_PtAmRes.dTailAbnormalRatio = m_PtAmRes.nTailAbnormalNum / double(num);
		int defectNum = ( m_PtAmRes.nHeadAbnormalNum 
						+ m_PtAmRes.nBodyAbnormalNum
						+ m_PtAmRes.nTailAbnormalNum );
		if( num <= m_PtAmRes.nTotalNormalNum ) m_PtAmRes.TZI = 0.0;
		m_PtAmRes.TZI = defectNum / double( num - m_PtAmRes.nTotalNormalNum );
		m_PtAmRes.SDI = defectNum / double(num);
	}
	else{
		m_PtAmRes.dTotalNormalRatio  = 0.0;
		m_PtAmRes.dHeadAbnormalRatio = 0.0;
		m_PtAmRes.dBodyAbnormalRatio = 0.0;
		m_PtAmRes.dTailAbnormalRatio = 0.0;	
		m_PtAmRes.TZI = 0.0;
		m_PtAmRes.SDI = 0.0;
	}
}

void CMorphaDetectDLG::OnButtonMorphadetectFinish() 
{
	if(MessageBox(_T("保存检测的结果?"),TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION) == IDNO )
	{
		theView1->m_imgMorphaImg.Destroy();
	    memset(&m_MhNum,0,sizeof(m_MhNum));
		m_lsOperateIndex.clear();
		m_itListIndex= m_lsOperateIndex.end();
		m_nVideoFiled = 0;
		m_BFinished  = FALSE;

		theView1->SetScrollSizes(MM_TEXT,CSize(100,100));
	
		switch(theView3->m_eDetectMode)
		{
		case CView3::MORPHAIMAGE:
			theView1->m_eViewShowMode = CSpermView::INITIAL;
			break;
		case CView3::CAMERABMP:
			if(!theView3->m_pwndLiveDetectDlg->m_cap.IsOpen())
				theView3->m_pwndLiveDetectDlg->m_cap.Init(theView1->m_nDeviceID,theView1->GetSafeHwnd());
			theView3->m_eDetectMode =(CView3::DETECTMODE)((int)theView3->m_eDetectMode
				                                       ^ (int)CView3::BMP );
			break;
		}
		theView1->Invalidate();
		DestroyWindow();
		return;
	}

	CString strDetectNO=theView3->m_strDetectNO;

	CString Table_Arr[6] = {
		"morpharesult",
		"morphaimage",
		"MorphaPartialRes",
		"morpharesultratio",
		"resultkind"
	};
	int k = 0;
	CString strSqlDel;
	try{
		for(k=0; k<5; k++)
		{
			strSqlDel.Format("delete from %s where pDetectNO = '%s'", Table_Arr[k], strDetectNO);
			theConnection->Execute((LPCTSTR)strSqlDel,NULL,adCmdText);
		}
		strSqlDel.Format("delete from %s where pID like '%s%%'", "MorphaSpermInfo", strDetectNO);
		theConnection->Execute((LPCTSTR)strSqlDel,NULL,adCmdText);
	}
	catch (_com_error&e) {
		AfxMessageBox(e.Description());
	}

	CString strIns;
	strIns.Format("insert into morpharesult values('%s',\
					%d,%d,%d,%d,%d,%d,%d,%d,\
					%d,%d,%d,%d,%d,%d,%d,%d,\
					%d,%d,%d,%d,%d,%d,%d,%d,\
					%d,%d,%d,%d,%d,%d,%d,%d,\
					%d,%d,%d,%d,%d)"
	     ,strDetectNO
		,(m_MhNum.NormalSpermNum)
		,(m_MhNum.AbnormalSpermNum)
		,(m_MhNum.HeadAbnormalSpermNum)
		,(m_MhNum.NeckAbnormalSpermNum)
		,(m_MhNum.TailAbnormalSpermNum)
		,(m_MhNum.BigHeadSpermNum)
		,(m_MhNum.SmallHeadSpermNum)
		,(m_MhNum.BrushHeadSpermNum)
		,(m_MhNum.ConeHeadSpermNum)
		,(m_MhNum.CircleHeadSpermNum)
		,(m_MhNum.PearHeadSpermNum)
		,(m_MhNum.IrregularHeadSpermNum)
		,(m_MhNum.SizeAbnormalpermNum)
		,(m_MhNum.AngleAbnormalpermNum)
		,(m_MhNum.InsertAbnormalpermNum)
		,(m_MhNum.ShortTailpermNum)
		,(m_MhNum.IrregularpermNum)
		,(m_MhNum.CircleTailSpermNum)
		,(m_MhNum.MultiTailSpermNum)
		,(m_MhNum.NoTailSpermNum)
		,(m_MhNum.WindSpermNum)
		,(m_MhNum.AbnormalPerforpermNum)
		,(m_MhNum.SmallPointAbSpermNum)
		,(m_MhNum.normal_lengthNum)
		,(m_MhNum.normal_widthNum)
		,(m_MhNum.normal_areaNum)
		,(m_MhNum.normal_ellipticityNum)
		,(m_MhNum.normal_perfor_areaNum)
		,(m_MhNum.normal_head_areaNum)
		,(m_MhNum.normal_perimeterNum)
		,(m_MhNum.normal_head_perfor_areaNum)
		,(m_MhNum.normal_tail_lengthNum)
		,(m_MhNum.normal_tail_widthNum)
		,(m_MhNum.normal_tail_angleNum)
		,(m_MhNum.normal_extensionNum)
		,(m_MhNum.normal_symmetryNum)
		,(m_MhNum.normal_rugaNum)
		);

	WriteMorphaSpermInfo();

	WriteImage(strDetectNO, "padimage",  m_lpResBmInfoSrc, m_lpResBmDataSrc);
	WriteImage(strDetectNO, "edgeimage", m_lpResEdgeBmInfoSrc,m_lpResEdgeBmDataSrc);

	if( m_iplMorphaColorImg )
	{
		LPBITMAPINFOHEADER head = new BITMAPINFOHEADER;
		memset(head, 0, sizeof(BITMAPINFOHEADER));
		LPBYTE  data = new BYTE[m_iplMorphaColorImg->imageSize];
		memset(data, 0, m_iplMorphaColorImg->imageSize);
		IplImageTODib(m_iplMorphaColorImg, head, data);
		WriteImage(strDetectNO, "FisrtImage", head, data);
		DeleteDIBImage(head, data);
	}

	if( m_rImg )
	{
		LPBITMAPINFOHEADER head = new BITMAPINFOHEADER;
		memset(head, 0, sizeof(BITMAPINFOHEADER));
		LPBYTE       data = new BYTE[m_rImg->imageSize];
		memset(data, 0, m_rImg->imageSize);
		IplImageTODib(m_rImg, head, data);
		WriteImage(strDetectNO, "singleResultImage", head, data);
		DeleteDIBImage(head, data);
		cvReleaseImage(&m_rImg);
	}
 
	CString strInsMorResRatio;
	if( m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum != 0 )
	strInsMorResRatio.Format("insert morpharesultratio values('%s',%lf,%lf,%lf,%lf,%lf,\
		%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)",theView3->m_strDetectNO,
		m_MhNum.normal_lengthNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_widthNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_areaNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_ellipticityNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_perfor_areaNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_head_areaNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_perimeterNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_head_perfor_areaNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_tail_lengthNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_tail_widthNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_tail_angleNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_extensionNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_symmetryNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum),
		m_MhNum.normal_rugaNum*1.0/(m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum)
		);
	else
		strInsMorResRatio.Format("insert morpharesultratio values('%s',0.0,0.0,0.0,0.0,0.0,\
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0)", strDetectNO);

	GetSpermPartialAbnormalNum();

	CString strSQLRes;
	strSQLRes.Format("insert MorphaPartialRes values('%s', %d, %d, %d, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf)" , \
		strDetectNO,
		m_PtAmRes.nTotalNum,
		m_PtAmRes.nTotalNormalNum,
		m_PtAmRes.nHeadAbnormalNum,
		m_PtAmRes.nBodyAbnormalNum,
		m_PtAmRes.nTailAbnormalNum,
		
		m_PtAmRes.dTotalNormalRatio,
		m_PtAmRes.dHeadAbnormalRatio,
		m_PtAmRes.dBodyAbnormalRatio,
		m_PtAmRes.dTailAbnormalRatio,
		m_PtAmRes.TZI,
		m_PtAmRes.SDI);

	try
	{
		theConnection->Execute((LPCTSTR)strIns,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strInsMorResRatio,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSQLRes,NULL,adCmdText);

		CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
		CView3* pView3=(CView3*)pMainFrm->GetView3();
		CSpermView* pSpermView=(CSpermView*)pMainFrm->GetView1();

		pSpermView->m_imgMorphaImg.Destroy();
	    memset(&m_MhNum,0,sizeof(m_MhNum));
		m_lsOperateIndex.clear();
		m_itListIndex= m_lsOperateIndex.end();
		m_nVideoFiled = 0;
		m_BFinished  = FALSE;
		// 0415
		theView1->SetScrollSizes(MM_TEXT,CSize(100,100));
		// 0415
		switch(theView3->m_eDetectMode)
		{
		case CView3::MORPHAIMAGE:
			theView1->m_eViewShowMode = CSpermView::INITIAL;
			break;
		case CView3::CAMERABMP:
			if(!theView3->m_pwndLiveDetectDlg->m_cap.IsOpen())
				theView3->m_pwndLiveDetectDlg->m_cap.Init(theView1->m_nDeviceID,theView1->GetSafeHwnd());
			theView3->m_eDetectMode =(CView3::DETECTMODE)((int)theView3->m_eDetectMode
				                                       ^ (int)CView3::BMP );
			break;
		}
		
		// 0415
		theView1->Invalidate();
		// 0415

		// { 2008_9_4
		if(MessageBox("打印报告?", "打印报告", MB_OKCANCEL| MB_ICONEXCLAMATION )==IDOK)
		{
			CMainFrame *pfrm = (CMainFrame*)AfxGetMainWnd();
			CMorphaDataRetriveDlg *(&pDlg) = pfrm->m_pWndMorphadlg;
			
			if(pDlg==NULL) return;

			pfrm->m_nQueryPage = 0;

			if( pDlg->m_hWnd == NULL )
				pDlg->Create(IDD_DIALOG_MORPHA_RETRIVE);
			pDlg->ShowWindow(SW_HIDE);

			CString strSQLinfo("select distinct * from basicinfo,spermchait\
								where basicinfo.pdetectno=spermchait.pdetectno and \
								basicinfo.pdetectno='");
			strSQLinfo = strSQLinfo + theView3->m_strDetectNO + CString("'");
			pDlg->ReadRecordFromDatabase(strSQLinfo);
			pDlg->OnPrint();

			pDlg->DestroyWindow();	

		}
		else {
			// do nothing.
		}	
		// 清空View3的显示结果
		theView3->m_strDetectNO	 = _T("");
		theView3->m_strPatientName = _T("");
		theView3->UpdateData(FALSE);
		DestroyWindow();
        // } 2008_9_4

	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
	}
}

void CMorphaDetectDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if( theView3->m_eDetectMode == CView3::CAMERABMP)
		theView3->m_eDetectMode = CView3::CAMERA;

	if(m_BFinished)
	{
		if(MessageBox(_T("检测完成?"), TEXT("提示"), MB_YESNO | MB_ICONEXCLAMATION) == IDNO )
		{
			m_nYesNo = IDNO;
			return;
		}
		else {
			m_nYesNo = IDYES;		
			OnButtonMorphadetectFinish();
			clearworkspace();
		}
	}
	else {
		clearworkspace();
		DestroyWindow();
	}

}

void CMorphaDetectDLG::OnButtonDeletesperm() 
{
	if(m_nSelNO == -1)
		return ;
	m_lsOperateIndex.erase(m_itListIndex,m_lsOperateIndex.end()); 
	                   // 删除后面将要被覆盖的操作,包括m_itListIndex所指向的
	m_lsOperateIndex.push_back(m_nSelNO); // 记录当前操作序号
	m_itListIndex = m_lsOperateIndex.end(); //  更新当前操作序号
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CSpermView* pSpermView=(CSpermView*)pMainFrm->GetView1();
	DeleteSperm(m_nSelNO);
	m_nSelNO = -1;
	GetDlgItem(IDC_BUTTON_DELETESPERM)->EnableWindow(m_nSelNO!=-1);
	FormResultImage();

	CString str;
	str.Format("目前一共检测了%d个视野,%d个精子,其中%d个正常,%d个异常.",
		m_nVideoFiled,m_MhNum.AbnormalSpermNum+m_MhNum.NormalSpermNum,
		m_MhNum.NormalSpermNum,m_MhNum.AbnormalSpermNum);
	GetDlgItem(IDC_DETECT_INFO)->SetWindowText(str);
	CRect rect;
	theView1->GetClientRect(rect);
	CPoint sp=theView1->GetScrollPosition();
	rect.bottom+=sp.y;
	rect.right+=sp.x;

	CDC* pDC=pSpermView->GetDC();
	pDC->SetViewportOrg(-sp);

	pSpermView->DrawMemDCImage(pDC,rect);
	pSpermView->ReleaseDC(pDC);
}

void CMorphaDetectDLG::DeleteSperm(int selNO)
{
	//  设置删除标志，表明该精子是否删除
	m_vbIsNormal[selNO].isDeleted = TRUE;
	m_vOSpermImage[selNO].isDeleted = TRUE;
	m_vRSpermEdgeImage[selNO].isDeleted = TRUE;
	m_vRSpermImage[selNO].isDeleted = TRUE;
	m_vRSpermPaddedImage[selNO].isDeleted = TRUE;
	m_vSperMopyParameter[selNO].isDeleted = TRUE;
	m_vSpermRegion[selNO].isDeleted = TRUE;

	//  删除精子后修改数据
	if(m_vbIsNormal[selNO].IsNormalVector[0] == TRUE)
		--m_MhNum.NormalSpermNum;
	else
		--m_MhNum.AbnormalSpermNum;

	m_MhNum.normal_lengthNum -= m_vbIsNormal[selNO].IsNormalVector[1];
	m_MhNum.normal_widthNum -=  m_vbIsNormal[selNO].IsNormalVector[2];
	m_MhNum.normal_areaNum -= m_vbIsNormal[selNO].IsNormalVector[3];
	m_MhNum.normal_ellipticityNum -= m_vbIsNormal[selNO].IsNormalVector[4];
	m_MhNum.normal_perfor_areaNum -= m_vbIsNormal[selNO].IsNormalVector[5];
	m_MhNum.normal_head_areaNum -= m_vbIsNormal[selNO].IsNormalVector[6];
	m_MhNum.normal_perimeterNum -= m_vbIsNormal[selNO].IsNormalVector[7];
	m_MhNum.normal_head_perfor_areaNum -= m_vbIsNormal[selNO].IsNormalVector[8];

// 尾部
	m_MhNum.normal_tail_lengthNum -= m_vbIsNormal[selNO].IsNormalVector[9];
	m_MhNum.normal_tail_widthNum -= m_vbIsNormal[selNO].IsNormalVector[10];
	m_MhNum.normal_tail_angleNum -= m_vbIsNormal[selNO].IsNormalVector[11];

	m_MhNum.normal_extensionNum -= m_vbIsNormal[selNO].IsNormalVector[12];
	m_MhNum.normal_symmetryNum -= m_vbIsNormal[selNO].IsNormalVector[13];
	m_MhNum.normal_rugaNum -= m_vbIsNormal[selNO].IsNormalVector[14];


		if( (!m_vbIsNormal[selNO].IsNormalVector[4] ||
			 !m_vbIsNormal[selNO].IsNormalVector[5] ||
			 !m_vbIsNormal[selNO].IsNormalVector[8] )
			) // 头
			m_PtAmRes.nHeadAbnormalNum--;

		if( ( !m_vbIsNormal[selNO].IsNormalVector[1] ||
			!m_vbIsNormal[selNO].IsNormalVector[2] ||
			!m_vbIsNormal[selNO].IsNormalVector[3] ||
			!m_vbIsNormal[selNO].IsNormalVector[4] ||
			!m_vbIsNormal[selNO].IsNormalVector[6] ||
			!m_vbIsNormal[selNO].IsNormalVector[7] ||
			!m_vbIsNormal[selNO].IsNormalVector[8] ||
			!m_vbIsNormal[selNO].IsNormalVector[12] ||
			!m_vbIsNormal[selNO].IsNormalVector[13] ||
			!m_vbIsNormal[selNO].IsNormalVector[14] )
			) // 体
			m_PtAmRes.nBodyAbnormalNum--;
			
		if( ( !m_vbIsNormal[selNO].IsNormalVector[9] ||
			!m_vbIsNormal[selNO].IsNormalVector[10] ||
			!m_vbIsNormal[selNO].IsNormalVector[11] )
			) // 尾
			m_PtAmRes.nTailAbnormalNum--;	


	SperMopyParameter& t=m_vSperMopyParameter[selNO];
	switch(t.m_tailAb)
	{
	case 0:
		--m_MhNum.SizeAbnormalpermNum;
		break;
	case 1:
		--m_MhNum.AngleAbnormalpermNum;
		break;
	case 2:
		--m_MhNum.InsertAbnormalpermNum;
		break;
	case 3:
		--m_MhNum.IrregularpermNum;
		break;
	case 4:
		--m_MhNum.ShortTailpermNum;
		break;
	case 5:
		--m_MhNum.CircleTailSpermNum;
		break;
	case 6:
		--m_MhNum.MultiTailSpermNum;
		break;
	case 7:
		--m_MhNum.NoTailSpermNum;
		break;
	case 8:
		--m_MhNum.WindSpermNum;
		break;
	default:
		break;
	}
	switch(t.m_headAb)
	{
	case 0:
		--m_MhNum.BigHeadSpermNum;
		break;
	case 1:
		--m_MhNum.SmallHeadSpermNum;
		break;
	case 2:
		--m_MhNum.CircleHeadSpermNum;
		break;
	case 3:
		--m_MhNum.ConeHeadSpermNum;
		break;
	case 4:
		--m_MhNum.PearHeadSpermNum;
		break;
	case 5:
		--m_MhNum.BrushHeadSpermNum;
		break;
	case 6:
		--m_MhNum.IrregularHeadSpermNum;
		break;
	default:
		break;
	}
	switch(t.m_perforAb)
	{
	case 0:
		--m_MhNum.AbnormalPerforpermNum;
		break;
	default:
		break;
	}
	switch(t.m_smallpointAb)
	{
	case 0:
		--m_MhNum.SmallPointAbSpermNum;
		break;
	default:
		break;
	}
}

void CMorphaDetectDLG::AddSperm(int selNO)
{

	//  设置删除标志，表明该精子是否删除
	m_vbIsNormal[selNO].isDeleted = FALSE;
	m_vOSpermImage[selNO].isDeleted = FALSE;
	m_vRSpermEdgeImage[selNO].isDeleted = FALSE;
	m_vRSpermImage[selNO].isDeleted = FALSE;
	m_vRSpermPaddedImage[selNO].isDeleted = FALSE;
	m_vSperMopyParameter[selNO].isDeleted = FALSE;
	m_vSpermRegion[selNO].isDeleted = FALSE;


	m_MhNum.normal_lengthNum += m_vbIsNormal[selNO].IsNormalVector[1];
	m_MhNum.normal_widthNum +=  m_vbIsNormal[selNO].IsNormalVector[2];
	m_MhNum.normal_areaNum += m_vbIsNormal[selNO].IsNormalVector[3];
	m_MhNum.normal_ellipticityNum += m_vbIsNormal[selNO].IsNormalVector[4];
	m_MhNum.normal_perfor_areaNum += m_vbIsNormal[selNO].IsNormalVector[5];
	m_MhNum.normal_head_areaNum += m_vbIsNormal[selNO].IsNormalVector[6];
	m_MhNum.normal_perimeterNum += m_vbIsNormal[selNO].IsNormalVector[7];
	m_MhNum.normal_head_perfor_areaNum += m_vbIsNormal[selNO].IsNormalVector[8];

// 尾部
	m_MhNum.normal_tail_lengthNum += m_vbIsNormal[selNO].IsNormalVector[9];
	m_MhNum.normal_tail_widthNum += m_vbIsNormal[selNO].IsNormalVector[10];
	m_MhNum.normal_tail_angleNum += m_vbIsNormal[selNO].IsNormalVector[11];

	m_MhNum.normal_extensionNum += m_vbIsNormal[selNO].IsNormalVector[12];
	m_MhNum.normal_symmetryNum += m_vbIsNormal[selNO].IsNormalVector[13];
	m_MhNum.normal_rugaNum += m_vbIsNormal[selNO].IsNormalVector[14];


		if( (!m_vbIsNormal[selNO].IsNormalVector[4] ||
			 !m_vbIsNormal[selNO].IsNormalVector[5] ||
			 !m_vbIsNormal[selNO].IsNormalVector[8] )
			) // 头
			m_PtAmRes.nHeadAbnormalNum++;

		if( ( !m_vbIsNormal[selNO].IsNormalVector[1] ||
			!m_vbIsNormal[selNO].IsNormalVector[2] ||
			!m_vbIsNormal[selNO].IsNormalVector[3] ||
			!m_vbIsNormal[selNO].IsNormalVector[4] ||
			!m_vbIsNormal[selNO].IsNormalVector[6] ||
			!m_vbIsNormal[selNO].IsNormalVector[7] ||
			!m_vbIsNormal[selNO].IsNormalVector[8] ||
			!m_vbIsNormal[selNO].IsNormalVector[12] ||
			!m_vbIsNormal[selNO].IsNormalVector[13] ||
			!m_vbIsNormal[selNO].IsNormalVector[14] )
			) // 体
			m_PtAmRes.nBodyAbnormalNum++;
			
		if( ( !m_vbIsNormal[selNO].IsNormalVector[9] ||
			!m_vbIsNormal[selNO].IsNormalVector[10] ||
			!m_vbIsNormal[selNO].IsNormalVector[11] )
			) // 尾
			m_PtAmRes.nTailAbnormalNum++;

	//  增加精子后修改数据
	if(m_vbIsNormal[selNO].IsNormalVector[0] == TRUE)
		++m_MhNum.NormalSpermNum;
	else
		++m_MhNum.AbnormalSpermNum;

	SperMopyParameter& t=m_vSperMopyParameter[selNO];
	switch(t.m_tailAb)
	{
	case 0:
		++m_MhNum.SizeAbnormalpermNum;
		break;
	case 1:
		++m_MhNum.AngleAbnormalpermNum;
		break;
	case 2:
		++m_MhNum.InsertAbnormalpermNum;
		break;
	case 3:
		++m_MhNum.IrregularpermNum;
		break;
	case 4:
		++m_MhNum.ShortTailpermNum;
		break;
	case 5:
		++m_MhNum.CircleTailSpermNum;
		break;
	case 6:
		++m_MhNum.MultiTailSpermNum;
		break;
	case 7:
		++m_MhNum.NoTailSpermNum;
		break;
	case 8:
		++m_MhNum.WindSpermNum;
		break;
	default:
		break;
	}
	switch(t.m_headAb)
	{
	case 0:
		++m_MhNum.BigHeadSpermNum;
		break;
	case 1:
		++m_MhNum.SmallHeadSpermNum;
		break;
	case 2:
		++m_MhNum.CircleHeadSpermNum;
		break;
	case 3:
		++m_MhNum.ConeHeadSpermNum;
		break;
	case 4:
		++m_MhNum.PearHeadSpermNum;
		break;
	case 5:
		++m_MhNum.BrushHeadSpermNum;
		break;
	case 6:
		++m_MhNum.IrregularHeadSpermNum;
		break;
	default:
		break;
	}
	switch(t.m_perforAb)
	{
	case 0:
		++m_MhNum.AbnormalPerforpermNum;
		break;
	default:
		break;
	}
	switch(t.m_smallpointAb)
	{
	case 0:
		++m_MhNum.SmallPointAbSpermNum;
		break;
	default:
		break;
	}

}

void CMorphaDetectDLG::WriteImage(const CString &strDetectNO, const CString &fieldname, LPBITMAPINFOHEADER lpHead, LPBYTE lpData)
{
	try
	{
		// lpHead, lpData -> pJpgBuf
		BYTE* pJpgBuf = NULL;
		DWORD nSize;
		Bmp2Jpg(lpHead, lpData, pJpgBuf, nSize);

		CString strSelect;
		strSelect.Format("select * from morphaimage where pDetectNo = '%s'",strDetectNO);
		_RecordsetPtr rs("adodb.recordset");
		rs->PutCursorLocation(adUseClient);
		rs->Open((LPCTSTR)strSelect,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);
		if(rs->GetRecordCount() == 0)
			rs->AddNew("pdetectno",(LPCTSTR)strDetectNO);
		else
			rs->MoveFirst();

		_variant_t varChunk;
		SAFEARRAY *psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nSize;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);

		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = psa;

		//  写Jpg图像数据(二进制格式)
		char *pSafeArrayBuf = NULL;
		SafeArrayAccessData(varChunk.parray,(void **)&pSafeArrayBuf);
		memcpy(pSafeArrayBuf, pJpgBuf, nSize);
		SafeArrayUnaccessData(varChunk.parray);	

		delete []pJpgBuf;
		pJpgBuf = NULL;

        //  写入数据库
		rs->Fields->GetItem(LPCTSTR(fieldname))->AppendChunk(varChunk);
		rs->Update();
		::VariantClear(&varChunk);
		::SafeArrayDestroyData(psa);
	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return;
	}
}

void CMorphaDetectDLG::WriteImageToDB(const CString &strpID, const CString &fieldname, LPBITMAPINFOHEADER lpHead, LPBYTE lpData)
{
	try
	{
		// lpHead, lpData -> pJpgBuf
		BYTE* pJpgBuf = NULL;
		DWORD nSize;
		Bmp2Jpg(lpHead, lpData, pJpgBuf, nSize);

		CString strSelect;
		strSelect.Format("select morphaSpermImage from morphaspermData where pID = '%s'", strpID);
		_RecordsetPtr rs("adodb.recordset");
		rs->PutCursorLocation(adUseClient);
		rs->Open((LPCTSTR)strSelect, _variant_t(theConnection.GetInterfacePtr(), true),
				 adOpenStatic, adLockOptimistic, adCmdText);

		_variant_t varChunk;
		SAFEARRAY *psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nSize;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);

		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = psa;

		//  写Jpg图像数据(二进制格式)
		char *pSafeArrayBuf = NULL;
		SafeArrayAccessData(varChunk.parray,(void **)&pSafeArrayBuf);
		memcpy(pSafeArrayBuf, pJpgBuf, nSize);
		SafeArrayUnaccessData(varChunk.parray);

		delete []pJpgBuf;
		pJpgBuf = NULL;

        //  写入数据库
		rs->Fields->GetItem(LPCTSTR(fieldname))->AppendChunk(varChunk);
		rs->Update();
		::VariantClear(&varChunk);
		::SafeArrayDestroyData(psa);
	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return;
	}
}

void CMorphaDetectDLG::OnButtonMorphadetectRestore() 
{
	theView3->m_pwndLiveDetectDlg->m_cap.Init(theView1->m_nDeviceID,theView1->GetSafeHwnd());
	// 0415
	GetDlgItem(IDC_BUTTON_MORPHADETECT_RESTORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MORPHADETECT_CONTINUE)->EnableWindow(TRUE);
	// 0415
	theView1->SetScrollSizes(MM_TEXT, CSize(100,100));
	m_lsOperateIndex.clear();
	m_BFinished = FALSE;
	++m_nVideoFiled;
}

void CMorphaDetectDLG::OnCancel() 
{
	SendMessage(WM_CLOSE);
}

void CMorphaDetectDLG::SetUI(const UIControl &ui)
{
	GetDlgItem(IDC_BUTTON_MORPHADETECT_CONTINUE)->EnableWindow(ui.m_BContinueDetect);
	GetDlgItem(IDC_BUTTON_MORPHADETECT_FINISH)->EnableWindow(ui.m_BFinishDetect);
	GetDlgItem(IDC_BUTTON_MORPHADETECT_RESTORE)->EnableWindow(ui.m_BRestoreCamera);
}

void CMorphaDetectDLG::WriteMorphaSpermInfo()
{
	int i;
	for(i=0; i<m_vSpermRegion.size(); ++i)
	{
		if(m_vSpermRegion[i].isDeleted) continue;
		CString strSQL, pID;
		SperMopyParameter smp = m_vSperMopyParameter[i];
		pID = theView3->m_strDetectNO+"_"+IntTOCString(i+1);
		strSQL.Format("insert into morphasperminfo values \
			('%s',%d,%d,%d,%.2lf,%.2lf,%.2lf,%.3f,%.2lf,%.2lf,%.2lf,%.3f, %.2lf, %.2lf, %.2lf, %.3f, %.3f, %.3f)",
			pID,
			m_vbIsNormal[i].IsNormalVector[0],
			m_vSpermRegion[i].SpermCenterPos.x,
			m_vSpermRegion[i].SpermCenterPos.y,
			m_vSperMopyParameter[i].m_length, // 换算成微米
			m_vSperMopyParameter[i].m_width,
			m_vSperMopyParameter[i].m_area,
			m_vSperMopyParameter[i].m_ellipticity,
			m_vSperMopyParameter[i].m_perfor_area,
			m_vSperMopyParameter[i].m_head_area,
			m_vSperMopyParameter[i].m_perimeter,
			m_vSperMopyParameter[i].m_head_perfor_area,
			m_vSperMopyParameter[i].m_tail_length,
			m_vSperMopyParameter[i].m_tail_width,
			m_vSperMopyParameter[i].m_tail_angle,
			m_vSperMopyParameter[i].m_extension,
			m_vSperMopyParameter[i].m_symmetry,
			m_vSperMopyParameter[i].m_ruga);
		try
		{
			theConnection->Execute((LPCTSTR)strSQL,NULL,adCmdText);
		}
		catch (_com_error& e)
		{
			MessageBox(e.Description());
			return;
		}
	}
}

void CMorphaDetectDLG::WriteMorphaSpermData(int nSelNO, bool IsNormal)
{
	if(m_vSpermRegion[nSelNO].isDeleted) return;

	CString strSqlDel;
	strSqlDel.Format("delete from morphaspermData where pID like '%s%%'", theView3->m_strDetectNO);
	try{
		theConnection->Execute((LPCTSTR)strSqlDel,NULL,adCmdText);
	}
	catch(_com_error& e) {
		AfxMessageBox(e.Description());
	}
	
	CString strSQL, pID;
	SperMopyParameter smp = m_vSperMopyParameter[nSelNO];
	pID = theView3->m_strDetectNO+"_"+IntTOCString(nSelNO+1);
	strSQL.Format("insert into morphaspermData values \
		('%s',%d, %.2lf, %.2lf, %.2lf, %.3f, %.2lf, %.2lf, %.2lf, %.3f, %.2lf, %.2lf, %.2lf, %.3f, %.3f, %.3f, '%s')",
		pID,
		IsNormal,
		m_vSperMopyParameter[nSelNO].m_length, // 换算成微米
		m_vSperMopyParameter[nSelNO].m_width,
		m_vSperMopyParameter[nSelNO].m_area,
		m_vSperMopyParameter[nSelNO].m_ellipticity,
		m_vSperMopyParameter[nSelNO].m_perfor_area,
		m_vSperMopyParameter[nSelNO].m_head_area,
		m_vSperMopyParameter[nSelNO].m_perimeter,
		m_vSperMopyParameter[nSelNO].m_head_perfor_area,
		m_vSperMopyParameter[nSelNO].m_tail_length,
		m_vSperMopyParameter[nSelNO].m_tail_width,
		m_vSperMopyParameter[nSelNO].m_tail_angle,
		m_vSperMopyParameter[nSelNO].m_extension,
		m_vSperMopyParameter[nSelNO].m_symmetry,
		m_vSperMopyParameter[nSelNO].m_ruga,
		"");
	try
	{
		theConnection->Execute((LPCTSTR)strSQL,NULL,adCmdText);
		WriteImageToDB(pID, "morphaSpermImage",  m_vOSpermImage[nSelNO].m_lpBmpInfo, m_vOSpermImage[nSelNO].m_lpBmpData);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
}

void CMorphaDetectDLG::OnButtonMorphadetectParameter() 
{
	// TODO: Add your control notification handler code here
	CMorphaParameterDlg dlg(this);
	dlg.DoModal();
}

HBRUSH CMorphaDetectDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{   
		if(m_nSelNO != -1)
		{
			COLORREF color;
			UINT IDArray[] = {
			 IDC_STATIC_ISNORMAL,

			 IDC_STATIC_LENGTH,
			 IDC_STATIC_WIDTH,
			 IDC_STATIC_AREA,

			 IDC_STATIC_ELLIPTICITY,				
			 IDC_STATIC_PERFOR_AREA,				
			 IDC_STATIC_HEAD_AREA,
			 IDC_STATIC_PERIMETER,
			 	
			 IDC_STATIC_MITOSOMA_LENGHT,			
			 IDC_STATIC_MITOSOMA_WIDTH,
			 IDC_STATIC_MITOSOMA_DEVANGLE,
			 
			 IDC_STATIC_HEAD_PERFOR_AREA,	
			 IDC_STATIC_EXTENSION,
			 IDC_STATIC_SYMMETRY,
			 IDC_STATIC_RUGA
			};
			for(int k=0; k<sizeof(IDArray)/sizeof(UINT); k++)
			{
				if( pWnd->GetDlgCtrlID() == IDArray[k] )
				{
					color = m_vbIsNormal[m_nSelNO].IsNormalVector[k] == TRUE ? BLUE_PIEXL : RED_PIEXL;
					pDC->SetTextColor(color);
				}		
			}
		}
	}   
    
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
