// DetectDLG.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LiveDetectDLG.h"
#include "LiveDetectedPamaSettingDlg.h"
#include "OptionDlg.h"
#include "MainFrm.h"
#include "View3.h"
#include "ImageProcess.h"
#include "ImageConvert.h"
#include "AllFunction.h"
#include "PrinteDlg.h"

#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern _ConnectionPtr theConnection;
extern CView3* theView3;
extern CSpermView* theView1;
extern CString theStrPathApp;

extern CString GetConnectIP();

inline void setBMDataColor(LPBYTE lpSrc, COLORTYPE ct) {
	const BYTE colorv[4][3] = {
		{0,0,255}, // 红
		{0,255,0}, // 绿
		{255,0,0}, // 蓝
		{0,255,255} // 黄
	};
	memcpy(lpSrc, &colorv[ct], 3);
}

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectDLG dialog

CLiveDetectDLG::CLiveDetectDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CLiveDetectDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLiveDetectDLG)
	m_spermszLOW  = 1.2;
	m_spermszHIGH = 14.5;
	m_nGrayValue   = 120;
	m_strSumNum = _T("");
	m_LiveDetectedInfoStr = _T("");
	m_dRoundness = 11.25;
	//}}AFX_DATA_INIT

	m_nLiveDetectedViewSight = 1;

	m_iplBinaryImage   = NULL;
	m_lpOringinImgHead = NULL;
	m_lpOringinImgData = NULL;
	m_lpBmInfo = NULL;
	m_lpBmData = NULL;

	m_ppIVisitMarkAry = NULL;

	m_dDilutionRatio = 1.0; // 稀释比
	m_dDepth = 10.0;
	m_nEnlargeRatio = 20;
	m_dFrequency = 36; // 帧频
	m_nR = 15; // 搜索范围半径

	m_nSpermNumSumInAllFrame = 0;

	m_LIN_Thrshold = 0.6;
	m_CUR_Thrshold = 0.12;
	m_STR_Thrshold = 0.5;
	m_v0 = 10;
	m_v1 = 20;
	m_v2 = 40;

	// 界面控制
	m_bPamaSetting  = true;
	m_bModeChange   = true;
	m_bAutoAnalyse  = false;
	m_bContinueDetected = false;
	m_bFinishDetected   = false;
	m_bMoveShow     = false;

	m_bControlState   = false;

	m_bIsVideoPlay = true; // 视频是否在播放

	m_pVideoPlay = NULL;

	m_nFrameNum = 0;
	m_nFrmCount = 0;
	m_nIntrv = 1;
	m_bCanCap = false;
	
	// 视频播放控制
	m_bVideoPlay   = false;
	m_bVideoStop   = true;
	m_bVideoPause  = true;
	m_bVideoResume = false;
  
	// 定时器ID
	mSliderTimer = 0;
	mNowVideoPlayPos = 0;

	// 原始序列图像
	for(int i=0; i<FRAME_NUM; i++)
	{
		m_pMoveImgSeqData[i] = NULL;
		m_pMoveImgSeqInfo[i] = NULL;
	}
		
	m_bIsUserState = FALSE;

    mCR[A_CLASS] = RGB(255, 0, 0); // 红
	mCR[B_CLASS] = RGB(0, 255, 0); // 绿
	mCR[C_CLASS] = RGB(0, 0, 255); // 蓝
	mCR[D_CLASS] = RGB(0, 255, 255); // 黄

    mColorArr[A_CLASS] = CV_RGB(255, 0, 0);
	mColorArr[B_CLASS] = CV_RGB(0, 255, 0);
	mColorArr[C_CLASS] = CV_RGB(0, 0, 255);
	mColorArr[D_CLASS] = CV_RGB(0, 255, 255);

}

void CLiveDetectDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLiveDetectDLG)
	DDX_Control(pDX, IDC_EDIT_ROUNDNESS, m_wndRoundness);
	DDX_Control(pDX, IDC_SLIDER_GRAPH, mSliderGraph);
	DDX_Control(pDX, IDC_EDIT_SPERMSZ_HIGH, m_wndEditHigh);
	DDX_Control(pDX, IDC_EDIT_SPERMSZ_LOW, m_wndEditLow);
	DDX_Control(pDX, IDC_EDIT_GRAYVALUE, m_wndGrayValue);
	DDX_Control(pDX, IDC_SLIDER_GRAYVALUE, m_wndSliderGrayValue);
	DDX_Text(pDX, IDC_EDIT_SPERMSZ_LOW, m_spermszLOW);
	DDV_MinMaxUInt(pDX, m_spermszLOW, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_SPERMSZ_HIGH, m_spermszHIGH);
	DDV_MinMaxUInt(pDX, m_spermszHIGH, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_GRAYVALUE, m_nGrayValue);
	DDV_MinMaxInt(pDX, m_nGrayValue, 0, 255);
	DDX_Text(pDX, IDC_SUM_NUM, m_strSumNum);
	DDX_Text(pDX, IDC_LD_PROGRESS_INFO, m_LiveDetectedInfoStr);
	DDX_Text(pDX, IDC_EDIT_ROUNDNESS, m_dRoundness);
	DDV_MinMaxInt(pDX, m_dRoundness, 0, 1000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLiveDetectDLG, CDialog)
	//{{AFX_MSG_MAP(CLiveDetectDLG)
	ON_EN_CHANGE(IDC_EDIT_SPERMSZ_LOW, OnChangeEditSpermszLow)
	ON_EN_CHANGE(IDC_EDIT_SPERMSZ_HIGH, OnChangeEditSpermszHigh)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_GRAYVALUE, OnChangeEditGrayvalue)
	ON_EN_KILLFOCUS(IDC_EDIT_SPERMSZ_HIGH, OnKillfocusEditSpermszHigh)
	ON_EN_KILLFOCUS(IDC_EDIT_SPERMSZ_LOW, OnKillfocusEditSpermszLow)
	ON_BN_CLICKED(IDC_BTN_CONTINUEDETECT, OnBtnContinuedetect)
	ON_BN_CLICKED(IDC_BTN_FINISHDETECT, OnBtnFinishdetect)
	ON_BN_CLICKED(IDC_BTN_AUTOANALYSE, OnBtnAutoanalyse)
	ON_BN_CLICKED(IDC_BTN_MODE_CHANGE, OnBtnModeChange)
	ON_BN_CLICKED(IDC_BTN_PAM_SETTING, OnBtnPamSetting)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, OnBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESUME, OnBtnResume)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_ROUNDNESS, OnChangeEditRoundness)

	ON_BN_CLICKED(IDC_BTN_ROUND_LESS, OnBtnRoundLess)
	ON_BN_CLICKED(IDC_BTN_ROUND_BIGGER, OnBtnRoundBigger)
	ON_BN_CLICKED(IDC_BTN_MOVE_SHOW, OnBtnMoveShow)
	//}}AFX_MSG_MAP
	ON_MESSAGE(USER_MESSAGE_AFTERWRITEDATA,AfterWriteData)	

	ON_MESSAGE(USER_MESSAGE_SHOW_CAPTURE_PROGRESS, ShowCaptureProgress)

	ON_MESSAGE(USER_MESSAGE_SHOWLIVEPROGRESS,ShowLiveProgress)
	ON_MESSAGE(USER_MESSAGE_TARGETMARKANDCENTERMARK,TargetMarkAndTargetCenterMark)
	ON_MESSAGE(USER_MSG_AFTERSEQUENCEANALYSE, AfterImageSequenctAnalyse)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectDLG message handlers

//{ My Global functions define here begin

double ppdist(CPoint p1, CPoint p2)
{
	double d = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
	return sqrtf(d);
}

double getdist(const vector<CPoint> &vpt)
{
	double dist = 0.0, maxd = 0.0;
	int i = 0, j = 0, n = vpt.size();
	for(i=0; i<n; i++) 
	{	
		for(j=0; j<n; j++)
		{			
			dist = ppdist(vpt[i], vpt[j]);
			if(dist > maxd)
			{	
				maxd = dist;
			}				
		}
	}
	return maxd;
}

bool MyCheck(double area, double maxarea, double maxdist, double rv)
{ 
	if(area / maxdist >= area / maxarea * rv)
		return true;
	else return false;		
}

void ImgErode(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, int Itr=1)
// 形态学腐蚀运算
{
	IplImage *Img = NULL;
	Img = cvCreateImage(cvSize(lpBmInfo->biWidth, lpBmInfo->biHeight), 8, 1);
	::Dib4ToIplImage1(lpBmInfo, lpBmData, Img);
	
	cvErode(Img, Img, NULL, Itr); // 腐蚀

	IplImage1TODib4(Img, lpBmInfo, lpBmData);
	
	cvReleaseImage(&Img);
}

void ImgDilate(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, int Itr=1)
// 形态学膨胀运算
{	
	IplImage *Img = NULL;
	Img = cvCreateImage(cvSize(lpBmInfo->biWidth, lpBmInfo->biHeight), 8, 1);
	::Dib4ToIplImage1(lpBmInfo, lpBmData, Img);
	
	cvDilate(Img, Img, NULL, Itr); // 膨胀

	IplImage1TODib4(Img, lpBmInfo, lpBmData);
	
	cvReleaseImage(&Img);
}

void ImgClose(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, int Itr=1)
// 形态学闭运算
{
	IplImage *Img = NULL;
	Img = cvCreateImage(cvSize(lpBmInfo->biWidth, lpBmInfo->biHeight), 8, 1);
	::Dib4ToIplImage1(lpBmInfo, lpBmData, Img);
	
	cvErode(Img, Img, NULL, Itr); // 腐蚀
	cvDilate(Img, Img, NULL, Itr); // 膨胀

	IplImage1TODib4(Img, lpBmInfo, lpBmData);
	
	cvReleaseImage(&Img);
}

//} My Global functions define here end

BOOL CLiveDetectDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon,true);
	
	CRect rect,rect8;
	GetWindowRect(rect);
	int cxCur,cxMin;
	((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetColumnInfo(0,cxCur,cxMin);
	((CMainFrame*)GetParent())->m_wndSplitter.GetPane(0,1)->GetDlgItem(IDC_BTN_LIVEDETECTED)->GetWindowRect(rect8);
	MoveWindow(cxCur+10, rect8.bottom, rect.right-rect.left, rect.bottom-rect.top);

	m_wndSliderGrayValue.SetRange(0,255);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_GRAYVALUE))->SetRange32(0,255);
	m_wndSliderGrayValue.SetPos(m_nGrayValue);
	
	OnUpdateButtonState();
	
	m_LiveDetectedInfoStr = _T("");
	m_nLiveDetectedViewSight = 0;
	m_strSumNum = TEXT("");
	UpdateData(FALSE);

 	// 界面控制
	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERAAVI:
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESUME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_GRAPH)->EnableWindow(FALSE);	
		break;
	case CView3::VIDEOAVI:	
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
		GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
		GetDlgItem(IDC_SLIDER_GRAPH)->EnableWindow(TRUE);
		break;
	}
	m_bPamaSetting  = true;
	m_bModeChange   = true;
	m_bAutoAnalyse  = false;
	m_bContinueDetected = false;
	m_bFinishDetected   = false;
	OnUpdateButtonState();
	GetDlgItem(IDC_BTN_MOVE_SHOW)->EnableWindow(FALSE);

	if(theView3->m_eDetectMode == CView3::VIDEOAVI)
	{
		double pos = 0.0, duration = 1.0;
		m_pVideoPlay->m_FilterGraph->GetCurrentPosition(&pos);
		m_pVideoPlay->m_FilterGraph->GetDuration(&duration);
		// Get the new position, and update the slider
		int newPos = int(pos * SLIDER_MAX_LENGTH / duration);
		mSliderGraph.SetRange(0, SLIDER_MAX_LENGTH);
		mSliderGraph.SetPos(newPos);
		mSliderTimer = SetTimer(SLIDER_TIMER, 100, NULL);
	}

	CString fileName = "LiveDetected_Parameter_Data.txt";
    CString sPath(theStrPathApp);
	sPath+= "\\config\\";
	CreateDirectory((LPCTSTR)sPath, NULL);
	
	CString  filePathAndName; 
	filePathAndName = sPath+fileName;
	
	fstream  fOut;
	try{
		fOut.open(filePathAndName, ios::in);
		fOut>>m_dDepth
			>>m_STR_Thrshold
			>>m_LIN_Thrshold
			>>m_CUR_Thrshold
			>>m_v0
			>>m_v1
			>>m_v2
			>>m_nEnlargeRatio;
		fOut.close();
	}
	catch (...) {
		
		MessageBox(_T("文件读入出错."), TEXT("ERROR"), MB_ICONERROR);
	}

	m_SLDTotalResult.ResultInitialize();
	m_nSpermszLOW  = m_spermszLOW*m_nEnlargeRatio;
	m_nSpermszHIGH = m_spermszHIGH*m_nEnlargeRatio;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLiveDetectDLG::OnChangeEditSpermszLow()
{
	if(GetDlgItemInt(IDC_EDIT_SPERMSZ_LOW) > GetDlgItemInt(IDC_EDIT_SPERMSZ_HIGH) )
	{
	}
	else
	{
        //m_nSpermszLOW = GetDlgItemInt(IDC_EDIT_SPERMSZ_LOW);
		if(m_wndEditLow.GetSafeHwnd())
		{
			UpdateData(TRUE);
			::SendMessage(GetSafeHwnd(),WM_HSCROLL,0,(LPARAM)(m_wndSliderGrayValue.GetSafeHwnd()));
		}
	}
}

void CLiveDetectDLG::OnChangeEditSpermszHigh() 
{
	if(GetDlgItemInt(IDC_EDIT_SPERMSZ_HIGH) < GetDlgItemInt(IDC_EDIT_SPERMSZ_LOW))
	{
	}	
	else
	{
        //m_nSpermszHIGH = GetDlgItemInt(IDC_EDIT_SPERMSZ_HIGH);
		if(m_wndEditHigh.GetSafeHwnd())
		{
			UpdateData(TRUE);
			::SendMessage(GetSafeHwnd(),WM_HSCROLL,0,(LPARAM)(m_wndSliderGrayValue.GetSafeHwnd()));	
		}
	}
}

void CLiveDetectDLG::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_GRAYVALUE:
		{
			m_nGrayValue = ((CSliderCtrl*)pScrollBar)->GetPos();
			UpdateData(FALSE);
			
			for(i=0; i<m_lpOringinImgHead->biHeight; i++)
			{
				memset(m_ppIVisitMarkAry[i],false,m_lpOringinImgHead->biWidth*sizeof(bool));
			}

			// m_lpOringinImgHead 是原始图像
			if(m_bIsUserState == FALSE)
			{
				memcpy(m_lpBmInfo,m_lpOringinImgHead,sizeof(BITMAPINFOHEADER));
				memcpy(m_lpBmData,m_lpOringinImgData,m_lpOringinImgHead->biSizeImage);
			}

			UpdateData(TRUE);
			
			m_vSpermEdge.clear();
			for(i=0; i<FRAME_NUM; i++)
				m_vSperm[i].clear();

			m_nSpermszLOW  = m_spermszLOW*0.5*m_nEnlargeRatio;
			m_nSpermszHIGH = m_spermszHIGH*m_nEnlargeRatio;
			m_ddRoundness = m_dRoundness;

			m_nSumNum = OnTargetAdjust(m_lpBmInfo, m_lpBmData, m_ppIVisitMarkAry, 
					 m_nSpermszLOW, m_nSpermszHIGH, m_ddRoundness, m_nGrayValue);
			
			m_strSumNum.Format("共有:%d个精子", m_nSumNum);
			UpdateData(FALSE);
			
			theView1->Invalidate();
			// m_vSpermEdge.clear();
		}

		break;

	case IDC_SLIDER_GRAPH:
		if (m_pVideoPlay && m_pVideoPlay->m_FilterGraph)
		{
			// Calculate the current seeking position
			double duration = 1.0;
			m_pVideoPlay->m_FilterGraph->GetDuration(&duration);
			double pos = duration * mSliderGraph.GetPos() / SLIDER_MAX_LENGTH;
			m_pVideoPlay->m_FilterGraph->SetCurrentPosition(pos);
		}		
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLiveDetectDLG::OnChangeEditGrayvalue()
{
	if(m_wndSliderGrayValue.GetSafeHwnd() == NULL)
		return;
	m_wndSliderGrayValue.SetPos(GetDlgItemInt(IDC_EDIT_GRAYVALUE));
	::SendMessage(GetSafeHwnd(),WM_HSCROLL,0,(LPARAM)(m_wndSliderGrayValue.GetSafeHwnd()));
}

void CLiveDetectDLG::OnChangeEditRoundness()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_wndRoundness.GetSafeHwnd())
	{
		UpdateData(TRUE);	
	}
}

void CLiveDetectDLG::OnBtnRoundLess() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_dRoundness > 0.1) m_dRoundness -= 0.1;
	UpdateData(FALSE);
}

void CLiveDetectDLG::OnBtnRoundBigger() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_dRoundness < 15.1) m_dRoundness += 0.1;
	UpdateData(FALSE);	
}

CLiveDetectDLG::~CLiveDetectDLG()
{
	delete m_pVideoPlay;
	m_pVideoPlay = NULL;

    ClearDataBuffer();

	delete []m_lpBmInfo; m_lpBmInfo = NULL;
	delete []m_lpBmData; m_lpBmData = NULL;

	delete []m_lpOringinImgHead; m_lpOringinImgHead = NULL;
	delete []m_lpOringinImgData; m_lpOringinImgData = NULL;

	cvReleaseImage(&m_iplBinaryImage); 
	
	if(m_ppIVisitMarkAry)
	{
		int i=0;
		for(i=0; i<m_nHeight; i++) 
		{
		   delete []m_ppIVisitMarkAry[i];
		   m_ppIVisitMarkAry[i] = NULL;
		}
		delete []m_ppIVisitMarkAry; m_ppIVisitMarkAry = NULL;
	}
}

int CLiveDetectDLG::OnTargetAdjust(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool **ppIAdjustVisitMarkAry, int iMinArea, int iMaxArea, double dRdness, int thrd)
{
	if(m_bIsUserState == FALSE) ::ThresholdImage(lpBmInfo, lpBmData, thrd);//二值化
	
	// ::ImgDilate(lpBmInfo, lpBmData);
	
	bool res = DibToIplImage(lpBmInfo,lpBmData,m_iplBinaryImage);

	ASSERT(res);

    return	OnTargetAdjustMark(lpBmInfo, lpBmData, ppIAdjustVisitMarkAry, iMaxArea, iMinArea, dRdness);
}

int CLiveDetectDLG::OnTargetAdjustMark(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool **ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness)
{
	CSpermView* pDisView = (CSpermView*)((CMainFrame*)AfxGetMainWnd())->GetView1();
	
    int height = lpBmInfo->biHeight;
	int width  = lpBmInfo->biWidth;
	int lw = WIDTHBYTES(width* 32);
	int i, j;
	LPBYTE lpSrc;
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			lpSrc = lpBmData + lw*i +4*j;
            if( PIEXL(lpSrc) == TARGET && ppIAdjustVisitMarkAry[i][j] == 0 )
			{
                OnTargetAdjustChoose(lpBmInfo, lpBmData, CPoint(i,j), ppIAdjustVisitMarkAry, iMaxSpermSize, iMinSpermSize, dRdness);
			}
		}
	}

	memcpy(pDisView->m_lpAdjustBmData, m_lpOringinImgData, m_lpOringinImgHead->biSizeImage);	

	for(i=0; i<m_vSpermEdge.size(); i++)
	{
		for(j=0; j<m_vSpermEdge[i].size(); j++)
		{
			lpSrc = pDisView->m_lpAdjustBmData + lw*m_vSpermEdge[i][j].x +4*m_vSpermEdge[i][j].y;
			*(lpSrc + 0) =   0;
			*(lpSrc + 1) =   0;
			*(lpSrc + 2) = 255;
		}
	}
	return m_vSpermEdge.size();
}

void CLiveDetectDLG::OnTargetAdjustChoose(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, CPoint p, bool **ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness)
{
    long  Width = lpBmInfo->biWidth;
	long  Height= lpBmInfo->biHeight;
	long  lw = WIDTHBYTES(Width * lpBmInfo->biBitCount); 

	int i;
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1,
					};
	CPoint   q;
	vector<CPoint> vPtRegion;
 	vector<CPoint> vPtQ;
    vPtQ.push_back(p);

	ppIAdjustVisitMarkAry[p.x][p.y] = 1;	 

	while( vPtQ.size() > 0 )
	{
		p = vPtQ.back();
		vPtQ.pop_back();
		vPtRegion.push_back(p);
 
 		for(i=0; i<4; i++)
		{
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
 
 			if( q.x<Height && q.x>=0 &&
				q.y<Width  && q.y>=0 && 
				ppIAdjustVisitMarkAry[q.x][q.y] == 0 &&  //未访问
				PIEXL(lpBmData + q.x*lw+ q.y*4) == TARGET )
			{	
 				ppIAdjustVisitMarkAry[q.x][q.y] = 1; 	
				vPtQ.push_back(q);
			}
 		}
 	}

	// 看面积符合否
	if( vPtRegion.size() <= iMaxSpermSize && vPtRegion.size() >= iMinSpermSize )
	{
		vector<CPoint> Sperm_edge;
	    for(i=0; i<vPtRegion.size(); i++)
		{
            if( IsBoundary(vPtRegion[i], lpBmInfo, lpBmData) )
				Sperm_edge.push_back(vPtRegion[i]);
		}
		// 看形状圆还是扁 -> 圆形度
		double maxdist = getdist(Sperm_edge);
		if( MyCheck( double((int)vPtRegion.size()), double((int)m_nSpermszHIGH), maxdist, dRdness)  )
		{
			m_vSperm[0].push_back(vPtRegion);
			m_vSpermEdge.push_back(Sperm_edge);
		}
	}
	else{
		for(i=0; i<vPtRegion.size(); i++)
		{ // 内部掏空，显示边缘
            *(lpBmData + vPtRegion[i].x*lw + vPtRegion[i].y*4 + 0) = 255;
            *(lpBmData + vPtRegion[i].x*lw + vPtRegion[i].y*4 + 1) = 255;		
            *(lpBmData + vPtRegion[i].x*lw + vPtRegion[i].y*4 + 2) = 255;
            *(lpBmData + vPtRegion[i].x*lw + vPtRegion[i].y*4 + 3) = 0;
		}
	}
}

bool CLiveDetectDLG::IsBoundary(CPoint p, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData)
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
	int lw = WIDTHBYTES(width * lpBmInfo->biBitCount);

	COLORREF  piexlArray[4];

	for(i=0; i<4; i++)
	{
        row = p.x + dir[i][0];
		col = p.y + dir[i][1];
        lpSrc = lpBmData + row*lw + col*4;
		if( row>=0 && row<height && col>=0 && col<width )
			piexlArray[i] = PIEXL(lpBmData + row*lw + col*4);
		else 
			piexlArray[i] = PIEXL(lpBmData+ p.x*lw+ p.y*4);
	}
	for(i=0; i<4; i++)
	   if( piexlArray[i] != PIEXL(lpBmData+ p.x*lw+ p.y*4) )
           return true;
	return false;
}

void CLiveDetectDLG::OnKillfocusEditSpermszHigh() 
{
	if(GetDlgItemInt(IDC_EDIT_SPERMSZ_HIGH) < GetDlgItemInt(IDC_EDIT_SPERMSZ_LOW))
	{
		MessageBox("输入数值应大于等于左边精子的最大直径","数据输入错误",MB_ICONWARNING);
		m_nSpermszHIGH=m_nSpermszLOW;
		UpdateData(FALSE);
	}		
}

void CLiveDetectDLG::OnKillfocusEditSpermszLow() 
{
	if(GetDlgItemInt(IDC_EDIT_SPERMSZ_LOW) > GetDlgItemInt(IDC_EDIT_SPERMSZ_HIGH) )
	{
		MessageBox("输入数值应小于等于右边精子的最大直径","数据输入错误",MB_ICONWARNING);
		m_nSpermszLOW=m_nSpermszHIGH;
		UpdateData(FALSE) ; 
	}	
}

void CLiveDetectDLG::OnTargetMark(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize)
{
    int height = lpBMIH[sn]->biHeight;
	int width  = lpBMIH[sn]->biWidth;
	int lw = WIDTHBYTES(width * 32);
	int i, j;
	LPBYTE lpSrc;
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			lpSrc = lpImage[sn] + lw*i +4*j;
            if( PIEXL(lpSrc) == TARGET && ppIVisitMarkAry[i][j] == 0 )
			{
                markSperm(lpBMIH, lpImage, CPoint(j,i), sn, ppIVisitMarkAry, iMaxSpermSize, iMinSpermSize);
			}
		}
	}
}

void CLiveDetectDLG::markSperm(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], CPoint p, int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize)
{
    long  Width = lpBMIH[sn]->biWidth;
	long  Height= lpBMIH[sn]->biHeight;
	long  lw = WIDTHBYTES(Width * lpBMIH[sn]->biBitCount); 

	int i;
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1,
					};
	CPoint   q;
	vector<CPoint> vPtRegion;
 	vector<CPoint> vPtQ;
    vPtQ.push_back(p);

	ppIVisitMarkAry[p.y][p.x] = 1;	 

	while( vPtQ.size() > 0 )
	{
		p = vPtQ.back();
		vPtQ.pop_back();
		vPtRegion.push_back(p);
 
 		for(i=0; i<4; i++)
		{
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
 
 			if( q.y<Height && q.y>=0 &&
				q.x<Width  && q.x>=0 && 
				ppIVisitMarkAry[q.y][q.x] == 0 &&  // 未访问
				PIEXL(lpImage[sn] + q.y*lw + q.x*4) == TARGET )
			{	
 				ppIVisitMarkAry[q.y][q.x] = 1; 	
				vPtQ.push_back(q);
			}
 		}
 	}

	if( vPtRegion.size() <= iMaxSpermSize && vPtRegion.size() >= iMinSpermSize )
	{
		vector<CPoint> vSpermedge;
		for(i=0; i<vPtRegion.size(); i++)
		{
            if( IsBoundary(CPoint(vPtRegion[i].y, vPtRegion[i].x), lpBMIH[sn], lpImage[sn]) )
				vSpermedge.push_back(vPtRegion[i]);
		}
		// 用周长除以直径
		double maxdist = getdist(vSpermedge);
		if( MyCheck( double((int)vPtRegion.size() - (int)vSpermedge.size()), double((int)m_nSpermszHIGH), maxdist, m_ddRoundness)  )
			m_vSperm[sn].push_back(vPtRegion);
	}
}

void CLiveDetectDLG::OnTargetCenterMark(int i)
{	
	for(int j=0; j<m_vSperm[i].size(); j++)
	{
		// 第i幅图的第j个精子
		CPoint p(0,0);
		int k = 0;
		for(k=0; k<m_vSperm[i][j].size(); k++)
		{					
			p.x += m_vSperm[i][j][k].x;
			p.y += m_vSperm[i][j][k].y;
		}

        //{ 3.25
        m_vSpermCenterPos[i].push_back( CPoint(p.x/k, p.y/k) );
        //} 3.25
	}
}

void CLiveDetectDLG::ClearDataBuffer()
{	
	int i=0;
	for(i=0; i<FRAME_NUM; i++)
	m_vSperm[i].clear();

	for(i=0; i<FRAME_NUM; i++)
		m_vSpermTrack[i].clear(); 

	for(i=0; i<FRAME_NUM; i++)
		m_vSpermCenterPos[i].clear();
	
	m_vSpermPath.clear();     // 原始路径
	m_vSpermAvgPath.clear();  // 平均路径
	m_vSpermEdge.clear(); 
	m_SLDResult.ResultInitialize();

	m_vVSL.clear(); //各个精子目标的直线速度
	m_vVCL.clear(); //每个精子目标的曲线速度
    m_vVAP.clear(); //各个精子目标的平均路径速度
	m_vLIN.clear(); //各个精子目标的直线性
	m_vSTR.clear(); //各个精子目标的前向性
	m_vWOB.clear(); //各个精子目标的摆动性
	m_vALH.clear(); //各个精子目标的侧摆幅度
	m_vBCF.clear(); //各个精子目标的鞭打频率

	delete []m_lpBmInfo; m_lpBmInfo = NULL;
	delete []m_lpBmData; m_lpBmData = NULL;	
	
}

// 自动分析
void CLiveDetectDLG::OnBtnAutoanalyse()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_EDIT_SPERMSZ_LOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_SPERMSZ_LOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SPERMSZ_HIGH)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_SPERMSZ_HIGH)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLIDER_GRAYVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_GRAYVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_GRAYVALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_AUTOANALYSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ROUNDNESS)->EnableWindow(FALSE);

	m_strSumNum = TEXT("");
	UpdateData(FALSE);

	for(int j=0; j<m_vSperm[0].size(); j++)
	{
		if(m_vSperm[0][j].size() <= 0 ) continue;
		// 第0幅图的第j个精子
		CPoint p(0,0);
		int k = 0;
		for(k=0; k<m_vSperm[0][j].size(); k++)
		{					
			p.x += m_vSperm[0][j][k].x;
			p.y += m_vSperm[0][j][k].y;
		}
        m_vSpermCenterPos[0].push_back( CPoint(p.y/k, p.x/k) );
	}

	m_bIsUserState = FALSE;
	for(int i=0; i<FRAME_NUM; i++)
	{
		m_vSperm[i].clear();
	}
	m_vSpermEdge.clear();

	static SequenceAnalyseThdPara thd;
	thd.hWnd = m_hWnd;
	thd.m_lpBMIH = theView1->m_lpBMIH;
	thd.m_lpImage = theView1->m_lpImage;
	thd.iSpermszHIGH = m_nSpermszHIGH;
	thd.iSpermszLOW = m_nSpermszLOW;
	thd.threshvalue = m_nGrayValue;

	AfxBeginThread((AFX_THREADPROC)ImageSequenceAnalyseThread,(LPVOID)&thd);

}

bool IsInSearchRegion(CPoint p, CPoint q, int r)
{
	double dist = (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y);
    return (int)sqrt(dist) <= r;
}

double GetDistance(CPoint p, CPoint q)
{
	double dist = (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y);
    return sqrt(dist);
}

// 相似性度量
double similarity(SpermTrack src, CPoint SpermChoose)
{
	double simly = (1<<30);
	double dist = GetDistance(src.nowPos, SpermChoose);
	return simly - dist;
}

// 两线段是否相交
bool IsTwoLineCrossed(CPoint L1_SPt, CPoint L1_EPt, CPoint L2_SPt, CPoint L2_EPt)
{
	double cx, cy;
	double a1, b1, c1;
	double a2, b2, c2;
	a1 = L1_SPt.y - L1_EPt.y;
	b1 = L1_EPt.x - L1_SPt.x;
	c1 = (L1_SPt.x - L1_EPt.x)*L1_SPt.y - (L1_SPt.y - L1_EPt.y)*L1_SPt.x;

	a2 = L2_SPt.y - L2_EPt.y;
	b2 = L2_EPt.x - L2_SPt.x;
	c2 = (L2_SPt.x - L2_EPt.x)*L2_SPt.y - (L2_SPt.y - L2_EPt.y)*L2_SPt.x;

	if( L1_SPt == L2_SPt || L1_EPt == L2_EPt || L1_SPt == L2_EPt || L1_EPt == L2_SPt )
		return true;

	if( abs(a2*b1 - a1*b2) <= 1.0e-6 )
		return false;
	
	cx = (c2*b1 - c1*b2) / (a2*b1 - a1*b2);
	cy = (c2*a1 - c1*a2) / (a2*b1 - a1*b2);
 
	if( (cx < L1_SPt.x && cx > L1_EPt.x ) ||
		(cx < L1_EPt.x && cx > L1_SPt.x ) ||
		(cx < L2_SPt.x && cx > L2_EPt.x ) ||
		(cx < L2_EPt.x && cx > L2_SPt.x ) )
    return true;
	else return false;
}

// 帧间目标匹配
void CLiveDetectDLG::SpermMoveTrack()
{
	CSpermView* pview1 = (CSpermView*)((CMainFrame*)AfxGetMainWnd())->GetView1();

	int i, j, k;
	int nSpermSum = 0;
	for(i=0; i<FRAME_NUM-1; i++)
		nSpermSum += m_vSpermCenterPos[i].size();

	m_nSpermNumSumInAllFrame = nSpermSum;

#ifdef _MAKE_PREDICT

	int *preIndexI = NULL, *preIndexI1 = NULL;
	preIndexI  = new int[nSpermSum];
	preIndexI1 = new int[nSpermSum];
	memset(preIndexI, -1, sizeof(int)*nSpermSum);
	memset(preIndexI1, -1, sizeof(int)*nSpermSum);

#endif // _MAKE_PREDICT

	for(i=0; i<FRAME_NUM-1; i++)
	{

		const int nSpermNumInFrameI = m_vSpermCenterPos[i].size();
		const int nSpermNumInFrameI1 = m_vSpermCenterPos[i+1].size();

	#ifdef _MAKE_PREDICT	
		swap(preIndexI, preIndexI1);
	#endif // _MAKE_PREDICT

		// 标记某层的精子k是否被匹配好了
		bool  *bSpermMark = NULL;
		bSpermMark = new bool[ nSpermNumInFrameI + nSpermNumInFrameI1 ];
		memset(bSpermMark, false, nSpermNumInFrameI + nSpermNumInFrameI1 );

		for(j=0; j<nSpermNumInFrameI; j++) 
		{
			// 处理第i帧第j个精子
			SpermTrack  st;
			st.nowPos = m_vSpermCenterPos[i][j];
			st.nPred  = 0;

			vector<int> vMatchChoose; // 候选匹配精子,记录下序号
            for(k=0; k<nSpermNumInFrameI1; k++)
            {
                if( IsInSearchRegion(st.nowPos, m_vSpermCenterPos[i+1][k], m_nR) )
                {
					vMatchChoose.push_back(k);
                }
            }

			// 考察各个候选匹配精子，找一个匹配最佳的精子做为“第i帧第j个精子”的下一个。
			double maxSimy = -(1<<30);
			int nBest = -1;
			for(k=0; k<vMatchChoose.size(); k++)
			{
				if( vMatchChoose[k] >= nSpermNumInFrameI + nSpermNumInFrameI1 ) MessageBox("bSpermMark vMatchChoose[k] find here");

				double simy = similarity( st, m_vSpermCenterPos[i+1][vMatchChoose[k]] );
                if( simy > maxSimy && !bSpermMark[vMatchChoose[k]] )
				{
					maxSimy = simy;
				    nBest   = vMatchChoose[k];
				}
			}

			if( nBest != -1 )
			{
				DirVector  dVec(m_vSpermCenterPos[i+1][nBest].x - st.nowPos.x, m_vSpermCenterPos[i+1][nBest].y - st.nowPos.y);
				st.direction = dVec;
				st.velocity  = dVec.Moudle()*m_dFrequency;
				st.nextPos = m_vSpermCenterPos[i+1][nBest];
				st.nextIndex = nBest;
 				bSpermMark[nBest] = true;
			
			#ifdef _MAKE_PREDICT	
				preIndexI1[nBest] = j;	
			#endif // _MAKE_PREDICT

			}
			else{  // 没有可以匹配的

			#ifdef _MAKE_PREDICT	

				if( i>=1 && preIndexI[j] >= 0 && preIndexI[j] < m_vSpermTrack[i-1].size() &&
					m_vSpermTrack[i-1][ preIndexI[j] ].nPred <= MAX_PRED )
				{ // 预测
					st.nPred = m_vSpermTrack[i-1][ preIndexI[j] ].nPred + 1;
					st.nextPos = CPoint( 2 * st.nowPos.x - m_vSpermTrack[i-1][ preIndexI[j] ].nowPos.x,
										 2 * st.nowPos.y - m_vSpermTrack[i-1][ preIndexI[j] ].nowPos.y);
					st.nextIndex = m_vSpermCenterPos[i+1].size();
					bSpermMark[st.nextIndex] = true;
					preIndexI1[st.nextIndex] = j;
					m_vSpermCenterPos[i+1].push_back(st.nextPos);
				}
				else
			#endif // _MAKE_PREDICT	
				{
					st.direction = DirVector(0,0);
					st.velocity  = 0;			
					st.nextPos = CPoint(-1,-1);
					st.nextIndex = -1;		
				}
			}
			m_vSpermTrack[i].push_back(st);	
		}
		delete []bSpermMark;
	}

	#ifdef _MAKE_PREDICT	
		delete []preIndexI; preIndexI = NULL;
		delete []preIndexI1; preIndexI1 = NULL;
	#endif // _MAKE_PREDICT

	for(j=0; j<m_vSpermCenterPos[FRAME_NUM-1].size(); j++) 
	{
		SpermTrack st;
		st.nowPos = m_vSpermCenterPos[FRAME_NUM-1][j];
		st.direction = DirVector(0,0);
		st.velocity  = 0;			
		st.nextPos = CPoint(-1,-1);
		st.nextIndex = -1;
		m_vSpermTrack[FRAME_NUM-1].push_back(st);
	}
}	

// 原始路径
void CLiveDetectDLG::GetSpermPath()
{	
	for(int j=0; j<m_vSpermTrack[0].size(); j++)
	{
		// 路径开始
		SpermPathNode  spNode;
		spNode.nFrame = 0;
		spNode.pos = m_vSpermTrack[0][j].nowPos;   
		
		SpermPath  sp;
		sp.push_back(spNode); // 首节点
		
		// 开始路径跟踪
		int nFrame = 0;
		
		SpermTrack st = m_vSpermTrack[0][j];
		
		while( st.nextIndex != -1 && nFrame < FRAME_NUM-1)
		{			
			int iNextIndex = st.nextIndex;
			st = m_vSpermTrack[++nFrame][iNextIndex]; // 路径上的下一个节点			
			spNode.nFrame = nFrame;
			spNode.pos = st.nowPos;	
			
			if( spNode.pos.x < 0 || spNode.pos.y < 0 ) break;
			
			sp.push_back(spNode); 

		}
		// 路径跟踪结束
		m_vSpermPath.push_back(sp);
	}

}

// 平均路径
void CLiveDetectDLG::GetSpermAvgPath()
{
	int i, j, nPathLength;
	for(i=0; i<m_vSpermPath.size(); ++i)
	{
		// 第i条路径的第j个节点
		SpermPath sp;
		nPathLength = m_vSpermPath[i].size();
		for(j=0; j<nPathLength; j++)
		{
			SpermPathNode spNode;
			if(j == 0 || j == nPathLength-1)
			{		
				spNode.nFrame = m_vSpermPath[i][j].nFrame;
				spNode.pos    = m_vSpermPath[i][j].pos;
				sp.push_back(spNode);
			}
		    else if(j == 1 || j == nPathLength-2)
			{
				spNode.nFrame = m_vSpermPath[i][j].nFrame;
				spNode.pos.x    = ( m_vSpermPath[i][j-1].pos.x
								  + m_vSpermPath[i][j].pos.x 
								  + m_vSpermPath[i][j+1].pos.x)/3;
				spNode.pos.y    = ( m_vSpermPath[i][j-1].pos.y
								  + m_vSpermPath[i][j].pos.y 
								  + m_vSpermPath[i][j+1].pos.y)/3;
				sp.push_back(spNode);
			}
			else
			{
				spNode.nFrame = m_vSpermPath[i][j].nFrame;
				spNode.pos.x    = ( m_vSpermPath[i][j-2].pos.x
								  + m_vSpermPath[i][j-1].pos.x
								  + m_vSpermPath[i][j].pos.x 
								  + m_vSpermPath[i][j+1].pos.x
								  + m_vSpermPath[i][j+2].pos.x)/5;
				spNode.pos.y    = ( m_vSpermPath[i][j-2].pos.y
								  + m_vSpermPath[i][j-1].pos.y
								  + m_vSpermPath[i][j].pos.y 
								  + m_vSpermPath[i][j+1].pos.y
								  + m_vSpermPath[i][j+2].pos.y)/5;
				sp.push_back(spNode);
			}
		}
		m_vSpermAvgPath.push_back(sp);			
	}
}	

// 参数计算
void CLiveDetectDLG::GetSpermMoveParameter()
{
	m_vVSL.clear(); //各个精子目标的直线速度
	m_vVCL.clear(); //每个精子目标的曲线速度
    m_vVAP.clear(); //各个精子目标的平均路径速度
	m_vLIN.clear(); //各个精子目标的直线性
	m_vSTR.clear(); //各个精子目标的前向性
	m_vWOB.clear(); //各个精子目标的摆动性
	m_vALH.clear(); //各个精子目标的侧摆幅度
	m_vBCF.clear(); //各个精子目标的鞭打频率
	m_SLDResult.ResultInitialize();

	int i,j;
    double	dMoveTime = 0.0;
	double  dDistance = 0.0;
	
	// 一个视野体积
	const double mm_px = 8.3; // 每像素多少微米
	double real_um_px = 0.586; // mm_px / (0.15 * m_nEnlargeRatio * m_nEnlargeRatio); // 每像素实际多少微米
	
	m_dVolume = m_dDepth*(mm_px*m_nHeight*0.001 * mm_px*m_nWidth*0.001)/(m_nEnlargeRatio*m_nEnlargeRatio); // 单位：毫升ml

	m_dVolume /= m_dDilutionRatio;

	// <1> 直线速度
	for(i=0; i<m_vSpermPath.size(); ++i)
	{	
		dMoveTime = ( m_vSpermPath[i].size()-1 )/m_dFrequency;
		double dVelocity = 0.0;
		if(m_vSpermPath[i].size() > 1){
			dDistance = GetDistance(m_vSpermPath[i][0].pos, m_vSpermPath[i][ m_vSpermPath[i].size()-1 ].pos);
			dVelocity = dDistance / dMoveTime;
		}
		else dVelocity = 0.0;
		dVelocity *= real_um_px;
		m_vVSL.push_back(dVelocity);
	}

	// <2> 曲线速度
	for(i=0; i<m_vSpermPath.size(); ++i)
	{
		dDistance = 0.0;
		for(j=0; j<m_vSpermPath[i].size()-1; j++)
		{
			dDistance += GetDistance(m_vSpermPath[i][j].pos, m_vSpermPath[i][j+1].pos);
		}
		dMoveTime = ( m_vSpermPath[i].size()-1 )/m_dFrequency;
		double dVelocity = 0.0;
		if(m_vSpermPath[i].size() > 1) {
			dVelocity = dDistance/dMoveTime;
		}
		else dVelocity = 0.0;
		dVelocity *= real_um_px;
		m_vVCL.push_back(dVelocity);
	}

	// <3> 平均路径速度
	for(i=0; i<m_vSpermAvgPath.size(); ++i)
	{
		dDistance = 0.0;
		for(j=0; j<m_vSpermAvgPath[i].size()-1; j++)
		{
			dDistance += GetDistance(m_vSpermAvgPath[i][j].pos, m_vSpermAvgPath[i][j+1].pos);
		}
		dMoveTime = ( m_vSpermAvgPath[i].size()-1 )/m_dFrequency;
		double dVelocity = 0.0;
		if(m_vSpermAvgPath[i].size() > 1) {
			dVelocity = dDistance/dMoveTime;		
		}
		else dVelocity = 0.0;
		dVelocity *= real_um_px;
		m_vVAP.push_back(dVelocity);
	}

	// <4> 直线性
	for(i=0; i<m_vSpermPath.size(); i++)
	{ 
		double dLin = 0.0;
		if( abs(m_vVSL[i]) >= 1e-6 && abs(m_vVCL[i]) >= 1e-6 ) 
		{
			dLin = m_vVSL[i] / m_vVCL[i];			
		}	
		m_vLIN.push_back(dLin);
	}

	// <5> 前向性
	for(i=0; i<m_vSpermPath.size(); i++)
	{
		double dStr = 0.0;
		if( abs(m_vVSL[i]) >= 1e-6 && abs(m_vVAP[i]) >= 1e-6 )
		{
			dStr = m_vVSL[i] / m_vVAP[i];			
		}
		m_vSTR.push_back(dStr);
	}

	// <6> 摆动性
	for(i=0; i<m_vSpermPath.size(); i++)
	{
		double dWob = 0.0;
		if( abs(m_vVAP[i]) >= 1e-6 && abs(m_vVCL[i]) >= 1e-6 )
		{
			dWob = m_vVAP[i] / m_vVCL[i];			
		}
		m_vWOB.push_back(dWob);
	}

	// <7> 侧摆幅度
	for(i=0; i<m_vSpermPath.size(); ++i)
	{
		double dmax = 0.0;
		double dSideRange = 0.0;
		for(j=0; j<m_vSpermPath[i].size(); j++)
		{
			dSideRange = 2 * GetDistance(m_vSpermPath[i][j].pos, m_vSpermAvgPath[i][j].pos);
			if( dSideRange > dmax )
				dmax = dSideRange;
		}
		if( m_vSpermPath[i].size() > 2 ) {
			dSideRange = dmax;			
		}
		else{
			dSideRange = 0.0;
		}
		dSideRange *= real_um_px;
		m_vALH.push_back(dSideRange);
	}

	// <8> 鞭打频率
	int nBeatCross;
	for(i=0; i<m_vSpermPath.size(); ++i)
	{
		nBeatCross = 0;
		CPoint L1_SPt, L1_EPt, L2_SPt, L2_EPt;
		for(j=0; j<m_vSpermPath[i].size()-1; j++)
		{
			L1_SPt = m_vSpermPath[i][j].pos;
			L1_EPt = m_vSpermPath[i][j+1].pos;
			L2_SPt = m_vSpermAvgPath[i][j].pos;
			L2_EPt = m_vSpermAvgPath[i][j+1].pos;
			if(L1_SPt == L1_EPt || L2_SPt == L2_EPt ) continue;
			nBeatCross += (int)IsTwoLineCrossed(L1_SPt, L1_EPt, L2_SPt, L2_EPt);
		}
		double dBcf = 0.0;
		double dMoveTime = ( m_vSpermPath[i].size()-1 ) / m_dFrequency;
		if(m_vSpermPath[i].size() > 1) {
			dBcf = nBeatCross / dMoveTime;
		}
		else 
			dBcf = 1.0*m_dFrequency;
		m_vBCF.push_back(dBcf);
	}
	
	// 运动等级分类计数
	int nGradeA    = 0;
	int nGradeB    = 0;
	int nGradeC    = 0;
	int nGradeD    = 0;
	
	char classtype;
	m_vSpermClass.clear();

	for(i=0; i<m_vVAP.size(); i++)
	{
		if(m_vVAP[i] < m_v0)
		{	nGradeD++; classtype = D_CLASS; }
		else if(m_vVAP[i] < m_v1)
		{	nGradeC++; classtype = C_CLASS; }
		else if(m_vVAP[i] < m_v2)
		{	nGradeB++; classtype = B_CLASS; }
		else{
			if(m_vSTR[i] > m_STR_Thrshold) 
			{	
				// 活跃精子个数（个）,是各视野内观察到的STR大于某个阈值的A级精子个数之和。
				m_SLDResult.nActiveSpermNum++;  
			}
			nGradeA++;
			classtype = A_CLASS;
		}
		m_vSpermClass.push_back(classtype);
	}

	m_SLDResult.iGradeA = nGradeA + m_nAclass;
	m_SLDResult.iGradeB = nGradeB + m_nBclass;
	m_SLDResult.iGradeC = nGradeC + m_nCclass;
	m_SLDResult.iGradeD = nGradeD + m_nDclass;

	if( m_vVAP.size() > 0 )
	{
		m_SLDResult.dGradeA = nGradeA/double(m_vVAP.size());
		m_SLDResult.dGradeB = nGradeB/double(m_vVAP.size());
		m_SLDResult.dGradeC = nGradeC/double(m_vVAP.size());
		m_SLDResult.dGradeD = nGradeD/double(m_vVAP.size());
	}
	else{
		m_SLDResult.dGradeA = .0;
		m_SLDResult.dGradeB = .0;
		m_SLDResult.dGradeC = .0;
		m_SLDResult.dGradeD = .0;
	}

	m_SLDResult.dGradeADensity = m_SLDResult.iGradeA / m_dVolume;
	m_SLDResult.dGradeBDensity = m_SLDResult.iGradeB / m_dVolume;
	m_SLDResult.dGradeCDensity = m_SLDResult.iGradeC / m_dVolume;
	m_SLDResult.dGradeDDensity = m_SLDResult.iGradeD / m_dVolume;

	// 统计平均值数据
	for(i=0; i<m_vSpermPath.size(); i++)
	{
		m_SLDResult.dVsl += m_vVSL[i];
		m_SLDResult.dVcl += m_vVCL[i];
		m_SLDResult.dVap += m_vVAP[i];
		m_SLDResult.dLin += m_vLIN[i];
		m_SLDResult.dStr += m_vSTR[i];
		m_SLDResult.dWob += m_vWOB[i];
		m_SLDResult.dAlh += m_vALH[i];
		m_SLDResult.dBcf += m_vBCF[i];
	}

	if( m_vSpermPath.size() > 0 )
	{
		m_SLDResult.dVsl /= m_vSpermPath.size();
		m_SLDResult.dVcl /= m_vSpermPath.size();
		m_SLDResult.dVap /= m_vSpermPath.size();
		m_SLDResult.dLin /= m_vSpermPath.size();
		m_SLDResult.dStr /= m_vSpermPath.size();
		m_SLDResult.dWob /= m_vSpermPath.size();
		m_SLDResult.dAlh /= m_vSpermPath.size();
		m_SLDResult.dBcf /= m_vSpermPath.size();

		// 计算标准差
		double df_vsl = 0.0, df_vcl = 0.0, df_vap = 0.0, df_alh = 0.0;
		for(i=0; i<m_vSpermPath.size(); i++)
		{
			df_vsl += SQUARE( m_vVSL[i] - m_SLDResult.dVsl );
			df_vcl += SQUARE( m_vVCL[i] - m_SLDResult.dVcl );
			df_vap += SQUARE( m_vVAP[i] - m_SLDResult.dVap );
			df_alh += SQUARE( m_vALH[i] - m_SLDResult.dAlh );	
		}

		if( m_vSpermPath.size() > 1 )
		{
			m_SLDResult.sd_vsl = sqrtf( df_vsl / ( m_vSpermPath.size() - 1 ) );
			m_SLDResult.sd_vcl = sqrtf( df_vcl / ( m_vSpermPath.size() - 1 ) );
			m_SLDResult.sd_vap = sqrtf( df_vap / ( m_vSpermPath.size() - 1 ) );
			m_SLDResult.sd_alh = sqrtf( df_alh / ( m_vSpermPath.size() - 1 ) );
		}
		else{
			m_SLDResult.sd_vsl = 0;
			m_SLDResult.sd_vcl = 0;
			m_SLDResult.sd_vap = 0;
			m_SLDResult.sd_alh = 0;
		}
	}
	else{
		m_SLDResult.dVsl  = .0;
		m_SLDResult.dVcl  = .0;
		m_SLDResult.dVap  = .0;
		m_SLDResult.dLin  = .0;
		m_SLDResult.dStr  = .0;
		m_SLDResult.dWob  = .0;
		m_SLDResult.dAlh  = .0;
		m_SLDResult.dBcf  = .0;
	}

	// 统计意义上的诊断参数
	// 取精子在精子计数板上分布的平均
	// 目标精子总数

	m_SLDResult.nTotalSpermNum = m_nSpermNumSumInAllFrame / FRAME_NUM + m_nAclass + m_nBclass + m_nCclass + m_nDclass;

	// 活动精子数目
	m_SLDResult.nMovementSpermNum = nGradeA + m_nAclass 
								  + nGradeB + m_nBclass
								  + nGradeC + m_nCclass;

	// 直线运动精子数，曲线运动精子数
	for(i=0; i<m_vLIN.size(); ++i)
	{
		if(m_vLIN[i] > m_LIN_Thrshold)
			m_SLDResult.nLineMoveSpermNum++;
		if(m_vLIN[i] < m_CUR_Thrshold)
			m_SLDResult.nCurveMoveSpermNum++;
	}

	//前向精子总数,a+b级
	m_SLDResult.nFrontSpermNum = nGradeA + m_nAclass 
							   + nGradeB + m_nBclass;   

	//各精子数目所占比率
	if( m_SLDResult.nTotalSpermNum > 0 )
	{
		m_SLDResult.dTotalSpermRatio = 1;
		m_SLDResult.dMovementSpermRatio = m_SLDResult.nMovementSpermNum/double(m_SLDResult.nTotalSpermNum); 
		m_SLDResult.dActiveSpermRatio   = m_SLDResult.nActiveSpermNum/double(m_SLDResult.nTotalSpermNum);    
		m_SLDResult.dLineMoveSpermRatio = m_SLDResult.nLineMoveSpermNum/double(m_SLDResult.nTotalSpermNum); 
		m_SLDResult.dCurveMoveSpermRatio= m_SLDResult.nCurveMoveSpermNum/double(m_SLDResult.nTotalSpermNum); 
		m_SLDResult.dFrontSpermRatio    = m_SLDResult.nFrontSpermNum/double(m_SLDResult.nTotalSpermNum); 
	}
	else{
		m_SLDResult.dTotalSpermRatio     = .0;
		m_SLDResult.dMovementSpermRatio  = .0;
		m_SLDResult.dActiveSpermRatio    = .0;
		m_SLDResult.dLineMoveSpermRatio  = .0;
		m_SLDResult.dCurveMoveSpermRatio = .0;
		m_SLDResult.dFrontSpermRatio     = .0;
	}

	// 速度分布统计
		
	for(i=0; i<m_vVCL.size(); i++)
	{
		if(m_vVCL[i] <= 10)
			m_SLDResult.m_vclgrd.vclgrade[1]++;
		else if(m_vVCL[i] <= 20)
			m_SLDResult.m_vclgrd.vclgrade[2]++;
		else if(m_vVCL[i] <= 30)
			m_SLDResult.m_vclgrd.vclgrade[3]++;
		else if(m_vVCL[i] <= 40)
			m_SLDResult.m_vclgrd.vclgrade[4]++;
		else if(m_vVCL[i] <= 50)
			m_SLDResult.m_vclgrd.vclgrade[5]++;
		else if(m_vVCL[i] <= 60)
			m_SLDResult.m_vclgrd.vclgrade[6]++;
		else
			m_SLDResult.m_vclgrd.vclgrade[7]++;
	}

	for(i=0; i<m_vVSL.size(); i++)
	{
		if(m_vVSL[i] <= 10)
			m_SLDResult.m_vslgrd.vslgrade[1]++;
		else if(m_vVSL[i] <= 20)
			m_SLDResult.m_vslgrd.vslgrade[2]++;
		else if(m_vVSL[i] <= 30)
			m_SLDResult.m_vslgrd.vslgrade[3]++;
		else if(m_vVSL[i] <= 40)
			m_SLDResult.m_vslgrd.vslgrade[4]++;
		else if(m_vVSL[i] <= 50)
			m_SLDResult.m_vslgrd.vslgrade[5]++;
		else if(m_vVSL[i] <= 60)
			m_SLDResult.m_vslgrd.vslgrade[6]++;
		else
			m_SLDResult.m_vslgrd.vslgrade[7]++;
	}

	for(i=0; i<m_vVAP.size(); i++)
	{
		if(m_vVAP[i] <= 10)
			m_SLDResult.m_vapgrd.vapgrade[1]++;
		else if(m_vVAP[i] <= 20)
			m_SLDResult.m_vapgrd.vapgrade[2]++;
		else if(m_vVAP[i] <= 30)
			m_SLDResult.m_vapgrd.vapgrade[3]++;
		else if(m_vVAP[i] <= 40)
			m_SLDResult.m_vapgrd.vapgrade[4]++;
		else if(m_vVAP[i] <= 50)
			m_SLDResult.m_vapgrd.vapgrade[5]++;
		else if(m_vVAP[i] <= 60)
			m_SLDResult.m_vapgrd.vapgrade[6]++;
		else
			m_SLDResult.m_vapgrd.vapgrade[7]++;
	}

	// 总精子密度
	m_SLDResult.dTotalSpermDensity = m_SLDResult.nTotalSpermNum/m_dVolume;  

	// 活跃精子密度
	m_SLDResult.dActiveSpermDensity = m_SLDResult.nActiveSpermNum/m_dVolume;

	// 活动精子密度
	m_SLDResult.dMovementSpermDensity = m_SLDResult.nMovementSpermNum/m_dVolume;

	// 前向精子密度
	m_SLDResult.dFrontSpermDensity = m_SLDResult.nFrontSpermNum/m_dVolume; 
	
	// 直线精子密度
	m_SLDResult.dLineSpermDensity  = m_SLDResult.nLineMoveSpermNum/m_dVolume;   
	
	// 曲线精子密度
	m_SLDResult.dCurveSpermDensity = m_SLDResult.nCurveMoveSpermNum/m_dVolume;  

}

void CLiveDetectDLG::ShowSpermLiveDetectedResult()
{
	theView3->m_sTotalSpermNum.Format("%d个", m_SLDResult.nTotalSpermNum);    
	theView3->m_sMovementSpermNum.Format("%d个", m_SLDResult.nMovementSpermNum); 
	theView3->m_sActiveSpermNum.Format("%d个", m_SLDResult.nActiveSpermNum);	 

	theView3->m_sGradeA.Format("%.2f%%", m_SLDResult.dGradeA*100);
	theView3->m_sGradeB.Format("%.2f%%", m_SLDResult.dGradeB*100);
	theView3->m_sGradeC.Format("%.2f%%", m_SLDResult.dGradeC*100);
	theView3->m_sGradeD.Format("%.2f%%", m_SLDResult.dGradeD*100);

	theView3->m_sLineMoveSpermNum.Format("%d个", m_SLDResult.nLineMoveSpermNum); 
	theView3->m_sCurveMoveSpermNum.Format("%d个", m_SLDResult.nCurveMoveSpermNum);

	theView3->m_sActiveSpermDensity.Format("%.3f(百万个/ml)", m_SLDResult.dActiveSpermDensity/1000000); 
	theView3->m_sTotalSpermDensity.Format("%.3f(百万个/ml)", m_SLDResult.dTotalSpermDensity/1000000);  
	theView3->m_sMovementSpermRatio.Format("%.2f%%", m_SLDResult.dMovementSpermRatio*100); // 存活率
	
	theView3->UpdateData(FALSE);
	theView3->Invalidate();

}

void CLiveDetectDLG::ClearView3()  // 清空View3的显示结果
{
	theView3->m_sTotalSpermNum = _T(""); 
	theView3->m_sMovementSpermNum = _T("");
	theView3->m_sActiveSpermNum = _T("");

	theView3->m_sGradeA = _T("");
	theView3->m_sGradeB = _T("");
	theView3->m_sGradeC = _T("");
	theView3->m_sGradeD = _T("");

	theView3->m_sLineMoveSpermNum  = _T("");
	theView3->m_sCurveMoveSpermNum = _T("");

	theView3->m_sActiveSpermDensity = _T("");
	theView3->m_sTotalSpermDensity  = _T("");
	theView3->m_sMovementSpermRatio = _T("");
	theView3->UpdateData(FALSE);
	theView3->Invalidate();

}

// 继续检测
void CLiveDetectDLG::OnBtnContinuedetect() 
{
	GetDlgItem(IDC_BTN_MOVE_SHOW)->EnableWindow(FALSE);
	if(theView1->mMoveShowTimer) {
		theView1->mFrm = 0;
		theView1->KillTimer(theView1->mMoveShowTimer); 
	}

	// 保存上次检测的结果
	m_SLDTotalResult.SaveLiveDetectedResult(m_SLDResult);
	
	// 清空各个数据存储区
	ClearDataBuffer();

	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CSpermView* pView1=(CSpermView*)pFrame->GetView1();

	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERAAVI:
		pView1->m_eViewShowMode = pView1->VIDEO_PLAY;
		theView3->m_pwndLiveDetectDlg->m_cap.Init(theView1->m_nDeviceID,theView1->m_hWnd);
		break;
	case CView3::VIDEOAVI:
		pView1->m_eViewShowMode = pView1->VIDEO_PLAY;
		m_strSumNum = TEXT("");

		OnBtnResume();
		{
			GetDlgItem(IDC_BTN_PLAY)->EnableWindow(false);
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(true);
			GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(true);
			GetDlgItem(IDC_BTN_RESUME)->EnableWindow(false);
			GetDlgItem(IDC_SLIDER_GRAPH)->EnableWindow(true);
			Invalidate();
		}
		break;
	}

	pView1->Invalidate();

	m_bFinishDetected = false;
	m_bContinueDetected = false;
	m_bModeChange = true;
	m_bMoveShow = false;

	m_bIsUserState = FALSE;

	OnUpdateButtonState();
	
	// 更新当前状态信息
	m_LiveDetectedInfoStr = _T("");
	UpdateData(FALSE);

	// 更新View3
	ClearView3();

}

void CLiveDetectDLG::OnBtnModeChange() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_BTN_MODE_CHANGE)->EnableWindow(FALSE);
	// 捕获视频序列帧并切换到第一帧显示状态
	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERAAVI:  // 摄像机
		m_bCanCap = true;
		m_nFrameNum = FRAME_NUM * m_nIntrv;
		m_nFrmCount = FRAME_NUM;
		break;
	case CView3::VIDEOAVI:   // 视频文件
		if(IsVideoStopped() == true)
		{
			if(MessageBox(_T("已播放完毕, 从头再播放?"),TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION) == IDNO )
			{ GetDlgItem(IDC_BTN_MODE_CHANGE)->EnableWindow(TRUE); return; }
			else {
				OnBtnPlay();
				GetDlgItem(IDC_BTN_MODE_CHANGE)->EnableWindow(TRUE);
			}
		}
		else{	
			m_bCanCap = true;
			m_nFrameNum = FRAME_NUM * m_nIntrv;
			m_nFrmCount = FRAME_NUM;
			GetDlgItem(IDC_BTN_PLAY)->EnableWindow(false);
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(false);
			GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_RESUME)->EnableWindow(false);
			GetDlgItem(IDC_SLIDER_GRAPH)->EnableWindow(false);
			OnBtnResume();
		}
		break;
	}
	m_nAclass = 0;
	m_nBclass = 0;
	m_nCclass = 0;
	m_nDclass = 0;
	theView1->m_bMenuItemFileOpen = false; 
}

void CLiveDetectDLG::OnBtnPamSetting() 
{
	// TODO: Add your control notification handler code here
	CLiveDetectedPamaSettingDlg liveDetectPamaSettingDlg;
	if( liveDetectPamaSettingDlg.DoModal() == IDOK )
	{
		m_dDepth = liveDetectPamaSettingDlg.m_dDepth;
		m_nEnlargeRatio = liveDetectPamaSettingDlg.m_nEnlargeRatio;
		m_LIN_Thrshold = liveDetectPamaSettingDlg.m_dLinThr;
		m_CUR_Thrshold = liveDetectPamaSettingDlg.m_dCurThr;
		m_STR_Thrshold = liveDetectPamaSettingDlg.m_dStrThrd;
		m_v0 = liveDetectPamaSettingDlg.m_dV0Thr;
		m_v1 = liveDetectPamaSettingDlg.m_dV1Thr;
		m_v2 = liveDetectPamaSettingDlg.m_dV2Thr;

		m_nSpermszLOW  = m_spermszLOW*m_nEnlargeRatio;
		m_nSpermszHIGH = m_spermszHIGH*m_nEnlargeRatio;
	}
}

void CLiveDetectDLG::OnBtnFinishdetect() 
{
	GetDlgItem(IDC_BTN_MOVE_SHOW)->EnableWindow(FALSE);
	if(theView1->mMoveShowTimer) {
		theView1->mFrm = 0;
		theView1->KillTimer(theView1->mMoveShowTimer);
	}

	// 保存上次检测的结果
	m_SLDTotalResult.SaveLiveDetectedResult(m_SLDResult);
	m_SLDTotalResult.lpSLDResultBmData = theView1->m_lpTrackResultBmData;
	m_SLDTotalResult.lpSLDResultBmInfo = theView1->m_lpTrackResultBmInfo;
	// 清空各个数据存储区
	ClearDataBuffer();
	long k=0;
	for(k=0; k<FRAME_NUM; k++) {
		DeleteDIBImage(theView1->m_lpBMIH[k], theView1->m_lpImage[k]);
	}
	
	if(MessageBox(_T("保存检测的结果?"),TEXT("提示"), MB_YESNO  | MB_ICONEXCLAMATION) == IDNO )
	{
		CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
		CView3* pView3=(CView3*)pMainFrm->GetView3();
		
		// 清空View3的显示结果
		ClearView3();
		
		// 清空CLiveDetectDLG的一些可以清空的数据
		ClearDataBuffer();
		for(k=0; k<FRAME_NUM; k++) {	
			DeleteDIBImage(theView1->m_lpBMIH[k], theView1->m_lpImage[k]);
		}

		delete []m_lpBmInfo; m_lpBmInfo = NULL;
		delete []m_lpBmData; m_lpBmData = NULL;
		
		delete []m_lpOringinImgHead; m_lpOringinImgHead = NULL;
		delete []m_lpOringinImgData; m_lpOringinImgData = NULL;
		
		cvReleaseImage(&m_iplBinaryImage);
		
		if(m_ppIVisitMarkAry)
		{
			int i=0;
			for(i=0; i<m_nHeight; i++) 
			{
				delete []m_ppIVisitMarkAry[i];
				m_ppIVisitMarkAry[i] = NULL;
			}
			delete []m_ppIVisitMarkAry; m_ppIVisitMarkAry = NULL;
		}
		
		m_bIsUserState = FALSE;
		
		switch(theView3->m_eDetectMode)
		{
		case CView3::CAMERAAVI:
			m_cap.Init(theView1->m_nDeviceID,theView1->GetSafeHwnd());
			theView3->m_eDetectMode =(CView3::DETECTMODE)((int)theView3->m_eDetectMode
																^ (int)CView3::AVI );
			break;
		case CView3::VIDEOAVI:
			OnBtnResume();
			theView1->m_eViewShowMode = CSpermView::VIDEO_PLAY;
			break;
		}
		
		// 清空View3的显示结果
		theView3->m_strDetectNO	 = _T("");
		theView3->m_strPatientName = _T("");
		theView3->UpdateData(FALSE);
		DestroyWindow();
		return;
	}
	
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView3=(CView3*)pMainFrm->GetView3();
	CString strSQLLiveResult, strSQLSpermStatics, strSQLMorPara, strSQLVelDistribute, strSQLcmpwithWHO;
	strSQLLiveResult.Format("insert into livenessresult values(\
		'%s',%d,%d,%lf,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%d,%d,%d)",
		pView3->m_strDetectNO,
		m_SLDTotalResult.nMovementSpermNum,
		m_SLDTotalResult.nTotalSpermNum,
		m_SLDTotalResult.dTotalSpermDensity,
		m_SLDTotalResult.nLineMoveSpermNum,
		m_SLDTotalResult.nCurveMoveSpermNum,
		m_SLDTotalResult.nActiveSpermNum,
		m_SLDTotalResult.dActiveSpermRatio,
		m_SLDTotalResult.dActiveSpermDensity/(1.0e6),
		m_SLDTotalResult.dGradeA,
		m_SLDTotalResult.dGradeB,
		m_SLDTotalResult.dGradeC,
		m_SLDTotalResult.dGradeD,
		m_SLDTotalResult.dGradeADensity/(1.0e6),
		m_SLDTotalResult.dGradeBDensity/(1.0e6),
		m_SLDTotalResult.dGradeCDensity/(1.0e6),
		m_SLDTotalResult.dGradeDDensity/(1.0e6),
		m_SLDTotalResult.iGradeA,
		m_SLDTotalResult.iGradeB,
		m_SLDTotalResult.iGradeC,
		m_SLDTotalResult.iGradeD
		);

	strSQLSpermStatics.Format("insert into spermnumstatics values(\
		'%s',%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)",
		pView3->m_strDetectNO,
		m_SLDTotalResult.nTotalSpermNum,
		m_SLDTotalResult.nActiveSpermNum,
		m_SLDTotalResult.nMovementSpermNum,
		m_SLDTotalResult.nFrontSpermNum,
		m_SLDTotalResult.nLineMoveSpermNum,
		m_SLDTotalResult.nCurveMoveSpermNum,
		m_SLDTotalResult.dTotalSpermRatio,
		m_SLDTotalResult.dActiveSpermRatio,
		m_SLDTotalResult.dMovementSpermRatio,
		m_SLDTotalResult.dFrontSpermRatio,
		m_SLDTotalResult.dLineMoveSpermRatio,
		m_SLDTotalResult.dCurveMoveSpermRatio,
		m_SLDTotalResult.dTotalSpermDensity/(1.0e6),
		m_SLDTotalResult.dActiveSpermDensity/(1.0e6),
		m_SLDTotalResult.dMovementSpermDensity/(1.0e6),
		m_SLDTotalResult.dFrontSpermDensity/(1.0e6),
		m_SLDTotalResult.dLineSpermDensity/(1.0e6),
		m_SLDTotalResult.dCurveSpermDensity/(1.0e6) );

	strSQLMorPara.Format("insert into spermmovitypara values(\
		'%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf, %lf, %lf, %lf, %lf)", 
										    pView3->m_strDetectNO,
											m_SLDTotalResult.dVap,
											m_SLDTotalResult.dVcl,
											m_SLDTotalResult.dVsl,
											m_SLDTotalResult.dWob,
											m_SLDTotalResult.dStr,
											m_SLDTotalResult.dLin,
											m_SLDTotalResult.dAlh,
											m_SLDTotalResult.dBcf,
											m_SLDTotalResult.sd_vsl,
											m_SLDTotalResult.sd_vcl,
											m_SLDTotalResult.sd_vap,
											m_SLDTotalResult.sd_alh
											);

	//{ 2008_8_26 added by happynp
	double nvcl = 0, nvsl = 0, nvap = 0;
	for(int i=1; i<=7; i++)
	{
		nvcl += m_SLDTotalResult.m_vclgrd.vclgrade[i];
		nvsl += m_SLDTotalResult.m_vslgrd.vslgrade[i];
		nvap += m_SLDTotalResult.m_vapgrd.vapgrade[i];
	}

	strSQLVelDistribute.Format("insert into velDistribute values(\
		'%s', %lf, %lf, %lf, %lf, %lf, %lf, %lf, \
		      %lf, %lf, %lf, %lf, %lf, %lf, %lf, \
			  %lf, %lf, %lf, %lf, %lf, %lf, %lf)", \
			  pView3->m_strDetectNO, 
			  m_SLDTotalResult.m_vclgrd.vclgrade[1] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[2] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[3] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[4] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[5] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[6] / nvcl,
			  m_SLDTotalResult.m_vclgrd.vclgrade[7] / nvcl,

			  m_SLDTotalResult.m_vslgrd.vslgrade[1] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[2] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[3] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[4] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[5] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[6] / nvsl,
			  m_SLDTotalResult.m_vslgrd.vslgrade[7] / nvsl,

			  m_SLDTotalResult.m_vapgrd.vapgrade[1] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[2] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[3] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[4] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[5] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[6] / nvap,
			  m_SLDTotalResult.m_vapgrd.vapgrade[7] / nvap );
	//} 2008_8_26 added by happynp

	strSQLcmpwithWHO.Format("insert into cmpwithwho values(\
		'%s',%lf,%lf,%lf,%lf,%lf,%lf)",pView3->m_strDetectNO,
		m_SLDTotalResult.dTotalSpermDensity/(1.0e6),12345.0,
		m_SLDTotalResult.dFrontSpermDensity/(1.0e6),12345.0,
		m_SLDTotalResult.dGradeADensity/(1.0e6),12345.0
		);

	CString strSQLDelLivenessresult, strSQLDelSpermmovitypara, strSQLDelSpermnumstatics, strSQLDelVelDistribute, strSQLDelCmpwithWHO, strSQLDelImage;
	strSQLDelLivenessresult.Format("delete from livenessresult where pDetectNO = '%s'", pView3->m_strDetectNO);
	strSQLDelSpermmovitypara.Format("delete from spermmovitypara where pDetectNO = '%s'", pView3->m_strDetectNO);
	strSQLDelSpermnumstatics.Format("delete from spermnumstatics where pDetectNO = '%s'", pView3->m_strDetectNO);
	strSQLDelVelDistribute.Format("delete from velDistribute where pDetectNO = '%s'", pView3->m_strDetectNO);
	strSQLDelCmpwithWHO.Format("delete from cmpwithWHO where pDetectNO = '%s'", pView3->m_strDetectNO);
	strSQLDelImage.Format("delete from image_table where pDetectNO = '%s'", pView3->m_strDetectNO);
	
	try{
		theConnection->Execute((LPCTSTR)strSQLDelLivenessresult, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLDelSpermmovitypara, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLDelSpermnumstatics, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLDelVelDistribute, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLDelCmpwithWHO, NULL, adCmdText);
		theConnection->Execute((LPCTSTR)strSQLDelImage, NULL, adCmdText);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}

	static WRDataThdPara para;
	para.SLDTotalResult      = m_SLDTotalResult;
	para.strSQLLiveResult    = strSQLLiveResult;
	para.strSQLMorPara       = strSQLMorPara;
	para.strSQLSpermStatics  = strSQLSpermStatics;
	para.strSQLVelDistribute = strSQLVelDistribute;
	para.strSQLcmpwithWHO    = strSQLcmpwithWHO;
	para.hWnd                = m_hWnd;

	// 
	m_LiveDetectedInfoStr = _T("正在写数据库...");
	UpdateData(FALSE);
	AfxBeginThread((AFX_THREADPROC)WriteDataThread,(LPVOID)&para);

}

void CLiveDetectDLG::WriteImage(const CString &strDetectNO, LPBITMAPINFOHEADER lpHead, LPBYTE lpData)
{	
	try
	{
		// lpHead, lpData -> pJpgBuf
		BYTE* pJpgBuf = NULL;
		DWORD nSize;
		Bmp2Jpg(lpHead, lpData, pJpgBuf, nSize);
		
		//
		CString strSelect;
		strSelect.Format("select * from image_table where pDetectNo = 'noExist' ");
		_RecordsetPtr rs("adodb.recordset");
		rs->PutCursorLocation(adUseClient);
		rs->Open((LPCTSTR)strSelect,_variant_t(theConnection.GetInterfacePtr(),true),
			adOpenStatic, adLockOptimistic, adCmdText);
		rs->AddNew("pdetectno", (LPCTSTR)strDetectNO);

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
		rs->Fields->GetItem("pimage")->AppendChunk(varChunk);
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

void CLiveDetectDLG::GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER& lpHead, LPBYTE& lpData)
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

void CLiveDetectDLG::OnUpdateButtonState()
{
	// 按钮项
	CButton *btn;
	btn = (CButton*)GetDlgItem(IDC_BTN_PAM_SETTING);
	btn->EnableWindow(m_bPamaSetting);

	btn = (CButton*)GetDlgItem(IDC_BTN_MODE_CHANGE);
	btn->EnableWindow(m_bModeChange);

	btn = (CButton*)GetDlgItem(IDC_BTN_AUTOANALYSE);
	btn->EnableWindow(m_bAutoAnalyse);

	btn = (CButton*)GetDlgItem(IDC_BTN_CONTINUEDETECT);
	btn->EnableWindow(m_bContinueDetected);

	btn = (CButton*)GetDlgItem(IDC_BTN_FINISHDETECT);
	btn->EnableWindow(m_bFinishDetected);

	EnableAdjustControl();

	Invalidate();
}

void CLiveDetectDLG::EnableAdjustControl()
{
	// 
	CWnd  *pWnd;
	pWnd = GetDlgItem(IDC_SPIN_SPERMSZ_HIGH);
	pWnd->EnableWindow(m_bControlState);	

	pWnd = GetDlgItem(IDC_SPIN_SPERMSZ_LOW); 
	pWnd->EnableWindow(m_bControlState);

	pWnd = GetDlgItem(IDC_EDIT_SPERMSZ_HIGH);
	pWnd->EnableWindow(m_bControlState);

	pWnd = GetDlgItem(IDC_EDIT_SPERMSZ_LOW);
	pWnd->EnableWindow(m_bControlState);

	pWnd = GetDlgItem(IDC_SPIN_GRAYVALUE); 
	pWnd->EnableWindow(m_bControlState);
	
	pWnd = GetDlgItem(IDC_EDIT_GRAYVALUE);  
	pWnd->EnableWindow(m_bControlState);
	
	pWnd = GetDlgItem(IDC_SLIDER_GRAYVALUE);
	pWnd->EnableWindow(m_bControlState);

	pWnd = GetDlgItem(IDC_EDIT_ROUNDNESS);
	pWnd->EnableWindow(m_bControlState);

	pWnd = GetDlgItem(IDC_BTN_ROUND_LESS);
	pWnd->EnableWindow(m_bControlState);
		
	pWnd = GetDlgItem(IDC_BTN_ROUND_BIGGER);
	pWnd->EnableWindow(m_bControlState);
}

void CLiveDetectDLG::OnOK() 
{
	OnBtnFinishdetect();
	CDialog::OnOK();
}

void CLiveDetectDLG::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if( theView3->m_eDetectMode == CView3::CAMERAAVI)
		theView3->m_eDetectMode = CView3::CAMERA;
	if(m_bFinishDetected)
	{	
		if(MessageBox(_T("检测完成?"),TEXT("提示"),MB_YESNO  | MB_ICONEXCLAMATION) ==  IDNO  )
		{		
			m_nYesNo = IDNO;
			return;
		}
		m_nYesNo = IDYES;
		OnBtnFinishdetect();
	}
	else{

		// 清空CLiveDetectDLG的一些可以清空的数据
		ClearDataBuffer();
		
		delete []m_lpBmInfo; m_lpBmInfo = NULL;
		delete []m_lpBmData; m_lpBmData = NULL;
		
		delete []m_lpOringinImgHead; m_lpOringinImgHead = NULL;
		delete []m_lpOringinImgData; m_lpOringinImgData = NULL;

		DeleteDIBImage(theView1->m_lpAdjustBmInfo, theView1->m_lpAdjustBmData);
		DeleteDIBImage(theView1->m_lpTrackResultBmInfo, theView1->m_lpTrackResultBmData);	
		
		cvReleaseImage(&m_iplBinaryImage);

 		for(int i=0; i<FRAME_NUM; i++)
 		{ DeleteDIBImage(m_pMoveImgSeqInfo[i], m_pMoveImgSeqData[i]); }

		if(m_ppIVisitMarkAry)
		{
			int i=0;
			for(i=0; i<m_nHeight; i++) 
			{
				delete []m_ppIVisitMarkAry[i];
				m_ppIVisitMarkAry[i] = NULL;
			}
			delete []m_ppIVisitMarkAry;
			m_ppIVisitMarkAry = NULL;
		}
		
		if(m_pVideoPlay) m_pVideoPlay->OnVideoResume();
		DestroyWindow();
	}
}

UINT CLiveDetectDLG::WriteDataThread(LPVOID para)
{	
	WRDataThdPara* p=(WRDataThdPara*)para;
	const CString& strSQLLiveResult    = p->strSQLLiveResult;
	const CString& strSQLSpermStatics  = p->strSQLSpermStatics;
	const CString& strSQLMorPara       = p->strSQLMorPara;
	const CString& strSQLVelDistribute = p->strSQLVelDistribute;
	const CString& strSQLcmpwithWHO    = p->strSQLcmpwithWHO;
	SpermLiveDetectResult& m_SLDTotalResult = p->SLDTotalResult;
	try
	{
		theConnection->Execute((LPCTSTR)strSQLLiveResult,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSQLSpermStatics,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSQLMorPara,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSQLVelDistribute,NULL,adCmdText);
		theConnection->Execute((LPCTSTR)strSQLcmpwithWHO,NULL,adCmdText);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 1;
	}

	WriteImage(m_SLDTotalResult.strDetedNO,
		m_SLDTotalResult.lpSLDResultBmInfo, 
		m_SLDTotalResult.lpSLDResultBmData);

	m_SLDTotalResult.ResultInitialize();
	::PostMessage(p->hWnd,USER_MESSAGE_AFTERWRITEDATA,0,0);
	return 0;
}

LRESULT CLiveDetectDLG::AfterWriteData(WPARAM w, LPARAM l)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetMainWnd();
	CView3* pView3=(CView3*)pMainFrm->GetView3();

	// 清空View3的显示结果
	ClearView3();

	// 清空CLiveDetectDLG的一些可以清空的数据
    ClearDataBuffer();

	delete []m_lpBmInfo; m_lpBmInfo = NULL;
	delete []m_lpBmData; m_lpBmData = NULL;

	delete []m_lpOringinImgHead; m_lpOringinImgHead = NULL;
	delete []m_lpOringinImgData; m_lpOringinImgData = NULL;

	cvReleaseImage(&m_iplBinaryImage);
	
	if(m_ppIVisitMarkAry)
	{
		int i=0;
		for(i=0; i<m_nHeight; i++) 
		{
		   delete []m_ppIVisitMarkAry[i];
		   m_ppIVisitMarkAry[i] = NULL;
		}
		delete []m_ppIVisitMarkAry; m_ppIVisitMarkAry = NULL;
	}
	
	m_bIsUserState = FALSE;

	switch(theView3->m_eDetectMode)
	{
	case CView3::CAMERAAVI:
		m_cap.Init(theView1->m_nDeviceID,theView1->GetSafeHwnd());
		
		theView3->m_eDetectMode =(CView3::DETECTMODE)((int)theView3->m_eDetectMode
																^ (int)CView3::AVI );
		break;
	case CView3::VIDEOAVI:
		OnBtnResume();
		theView1->m_eViewShowMode = CSpermView::VIDEO_PLAY;
		break;
	}
	
	if(MessageBox("打印报告?", "打印报告", MB_OKCANCEL | MB_ICONEXCLAMATION )==IDOK)
	{
		CMainFrame *pfrm = 	(CMainFrame*) AfxGetMainWnd();
		CLiveDataRetriveDlg *(&pDlg) = pfrm->m_pWndLivedlg;
		
		if (pDlg == NULL) return S_FALSE;
		
		pfrm->m_nQueryPage = 1;
		
		if( pDlg->m_hWnd == NULL )
			pDlg->Create(IDD_DIALOG_LIVE_RETRIVE);
		pDlg->ShowWindow(SW_HIDE);
		pDlg->UpdateWindow();

		CString strSQLinfo;
		strSQLinfo.Format("select distinct * from basicinfo,spermchait \
	   			   where basicinfo.pdetectno = spermchait.pdetectno and \
				   basicinfo.pdetectno='%s'", theView3->m_strDetectNO);

		pDlg->ReadRecordFromDatabase(strSQLinfo);		
		pDlg->OnBtnPrint();
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
	return S_OK;
}

UINT CLiveDetectDLG::InitCameraThread(LPVOID param)
{
	InitCameraThdPara *pInitCameraThdPara = (InitCameraThdPara*)param;
	CCaptureVideo *pCap = pInitCameraThdPara->pCap;
	HWND hWnd = pInitCameraThdPara->hWnd;
	int  nDectNO = pInitCameraThdPara->nDectNO;
	pCap->Init(nDectNO, hWnd);
	return 0;
}

LRESULT CLiveDetectDLG::ShowLiveProgress(WPARAM w, LPARAM l)
{
	CString* pStr=(CString*)w;
	m_LiveDetectedInfoStr = *pStr;
	UpdateData(FALSE);
	return S_OK;
}

UINT CLiveDetectDLG::ImageSequenceAnalyseThread(LPVOID param)
{
    SequenceAnalyseThdPara* p=(SequenceAnalyseThdPara*)param;
	int height = p->m_lpBMIH[0]->biHeight;
	int width  = p->m_lpBMIH[0]->biWidth;
    int **ppIVisitMarkAry;
	ppIVisitMarkAry = new int*[height];
	int j = 0;
    for(j=0; j<height; j++)
        ppIVisitMarkAry[j] = new int[width];
	// { add 2009_4_1
	long lImageSize = p->m_lpBMIH[0]->biSizeImage;
	LPBYTE lpBuf = new BYTE[lImageSize];
	// } add 2009_4_1

	static CString info;
	static TargetMarkAndCenterThdPara thd;
	int i = 0;
	for(i=0; i<FRAME_NUM; i++)
	{
		info.Format("正在处理第(%d/%d)个序列帧", i+1, FRAME_NUM);

		::SendMessage(p->hWnd,USER_MESSAGE_SHOWLIVEPROGRESS,(WPARAM)&info,0);

		memcpy(lpBuf, p->m_lpImage[i], lImageSize);
		::ThresholdImage(p->m_lpBMIH[i], p->m_lpImage[i], p->threshvalue); //二值化
		
	    ::ImgDilate(p->m_lpBMIH[i], p->m_lpImage[i]);
		::ImgErode(p->m_lpBMIH[i], p->m_lpImage[i]);

        for(int j=0; j<height; j++)
			memset(ppIVisitMarkAry[j], 0, sizeof(int)*width);
		thd.i = i;
		thd.ppIVisitMarkAry = ppIVisitMarkAry;
		thd.iSpermszHIGH = p->iSpermszHIGH;
		thd.iSpermszLOW = p->iSpermszLOW;

		::SendMessage(p->hWnd,USER_MESSAGE_TARGETMARKANDCENTERMARK,(WPARAM)&thd,0);

		memcpy(p->m_lpImage[i], lpBuf, lImageSize);

	}   
		
	delete []lpBuf; lpBuf = NULL;

	info = _T("分析完毕");
	::SendMessage(p->hWnd,USER_MESSAGE_SHOWLIVEPROGRESS,(WPARAM)&info,0);

    for(j=0; j<height; j++)          
		delete []ppIVisitMarkAry[j], ppIVisitMarkAry[j] = NULL;
	delete []ppIVisitMarkAry, ppIVisitMarkAry = NULL;

	::SendMessage(p->hWnd, USER_MSG_AFTERSEQUENCEANALYSE,0,0);

	return 0;
}

LRESULT CLiveDetectDLG::TargetMarkAndTargetCenterMark(WPARAM w,LPARAM l)
{
	TargetMarkAndCenterThdPara* p = (TargetMarkAndCenterThdPara*)w;
	OnTargetMark(theView1->m_lpBMIH, 
		         theView1->m_lpImage,
				 p->i, 
				 p->ppIVisitMarkAry,
				 p->iSpermszHIGH,
				 p->iSpermszLOW
				 );  // 标记目标
    if(p->i>0) OnTargetCenterMark(p->i);
	return S_OK;
}

LRESULT CLiveDetectDLG::AfterImageSequenctAnalyse(WPARAM w, LPARAM l)
{
	// 轨迹跟踪
	SpermMoveTrack();
	GetSpermPath();  
	GetSpermAvgPath();
	GetSpermMoveParameter();
	ShowSpermLiveDetectedResult();
	FormMoveFrame();
	FormTrackMap(); // 形成运动回放的序列图像

	// Update Showing Data of this Dialog
	CString str;
	str.Format("目前一共检测了%d个视野", ++m_nLiveDetectedViewSight);
	GetDlgItem(IDC_STATIC_DETECTNUM)->SetWindowText(str);
	UpdateData(FALSE);

	// 更新按钮状态
	m_bAutoAnalyse = false;
	m_bContinueDetected = true;
	m_bFinishDetected = true;
	m_bControlState = false;
	m_bMoveShow = true;
	OnUpdateButtonState();

	m_bIsUserState = TRUE;
	
	theView1->m_bMenuItemFileOpen = true;
	theView1->Invalidate();

	GetDlgItem(IDC_BTN_MOVE_SHOW)->EnableWindow(TRUE);
	return S_OK;
}

void CLiveDetectDLG::OnBtnPlay() 
{
	// TODO: Add your control notification handler code here
	m_pVideoPlay->OnVideoPlay();

	// Start a timer
	if (mSliderTimer == 0)
	{
		mSliderTimer = SetTimer(SLIDER_TIMER, 100, NULL);
	}
	mSliderGraph.SetPos(0);

	m_bVideoPlay   = false;
	m_bVideoResume = false;
	m_bVideoPause  = true;
	m_bVideoStop   = true;
	GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
	GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
	Invalidate();

}

void CLiveDetectDLG::OnBtnPause() 
{
	// TODO: Add your control notification handler code here
	m_pVideoPlay->OnVideoPause();

	// Stop the timer
	if (mSliderTimer)
	{
		KillTimer(mSliderTimer);
		mSliderTimer = 0;
	}
	
	m_bVideoPlay   = true;
	m_bVideoPause  = false;
	m_bVideoResume = true;
	m_bVideoStop   = true;
	GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
	GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
	Invalidate();

}

void CLiveDetectDLG::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	m_pVideoPlay->OnVideoStop();

	// Stop the timer
	if (mSliderTimer)
	{
		KillTimer(mSliderTimer);
		mSliderTimer = 0;
	}
	mSliderGraph.SetPos(SLIDER_MAX_LENGTH);

	m_bVideoPlay   = true;
	m_bVideoPause  = false;
	m_bVideoResume = false;
	m_bVideoStop   = false;
	GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
	GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
	Invalidate();

}

void CLiveDetectDLG::OnBtnResume() 
{
	// TODO: Add your control notification handler code here
	m_pVideoPlay->OnVideoResume();	

	// Start a timer
	if (mSliderTimer == 0)
	{
		mSliderTimer = SetTimer(SLIDER_TIMER, 100, NULL);
	}	
	
	m_bVideoPlay   = false;
	m_bVideoPause  = true;
	m_bVideoResume = false;
	m_bVideoStop   = true;
	GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
	GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
	Invalidate();

}

void CLiveDetectDLG::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == mSliderTimer && m_pVideoPlay && m_pVideoPlay->m_FilterGraph)
	{
		double pos = 0.0, duration = 1.0;
		m_pVideoPlay->m_FilterGraph->GetCurrentPosition(&pos);
		m_pVideoPlay->m_FilterGraph->GetDuration(&duration);
		// Get the new position, and update the slider
		int newPos = int(pos * SLIDER_MAX_LENGTH / duration);
		if (mSliderGraph.GetPos() != newPos)
		{
			mSliderGraph.SetPos(newPos);
		}

		if(newPos == SLIDER_MAX_LENGTH)
		{
			m_bVideoPlay   = true;
			m_bVideoPause  = false;
			m_bVideoResume = false;
			m_bVideoStop   = false;
			GetDlgItem(IDC_BTN_PLAY)->EnableWindow(m_bVideoPlay);
			GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bVideoStop);
			GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(m_bVideoPause);
			GetDlgItem(IDC_BTN_RESUME)->EnableWindow(m_bVideoResume);
			Invalidate();
			KillTimer(mSliderTimer);
			mSliderTimer = 0;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

bool CLiveDetectDLG::IsVideoStopped()
{
	if(m_pVideoPlay && m_pVideoPlay->m_FilterGraph)
	{
		double pos = 0.0, duration = 1.0;
		m_pVideoPlay->m_FilterGraph->GetCurrentPosition(&pos);
		m_pVideoPlay->m_FilterGraph->GetDuration(&duration);

		if (pos >= int(duration-1))
			return true;
		else 
			return false;
	}
	return false;
}

LRESULT CLiveDetectDLG::ShowCaptureProgress(WPARAM w, LPARAM l)
{
	CString* pStr = (CString*)w;
	m_LiveDetectedInfoStr = *pStr;
	UpdateData(FALSE);
	return S_OK;
}

void CLiveDetectDLG::GetColorPointFromRect(IplImage *Img, const CRect &rc, COLORTYPE ct, vector<ColorPoint>&vp, bool **marr)
{
	long l = rc.left;
	long t = rc.top;
	long r = rc.right;
	long b = rc.bottom;

	long h = Img->height;
	long w = Img->width;
	long lw = Img->widthStep;

	l = max(l, 0);
    t = max(t, 0);
	r = min(r, Img->width-1);
	b = min(b, Img->height-1);

	long i, j;
	for(i=t; i<=b && i<h; i++)
	{
		for(j=l; j<=r && j<w; j++)
		{
			char* lpsrc = Img->imageData + i*lw + j*4;
			if( !marr[i][j] && PIEXL(lpsrc) == mCR[ct] ) {
				ColorPoint cp;
				cp.pt.x = i;
				cp.pt.y = j;
				cp.ct = ct;
				vp.push_back(cp);
				marr[i][j] = true;
			}
		}
	}
}

void CLiveDetectDLG::FormMoveFrame()
{
	int i=0, j=0;
	for(i=0; i<m_vp.size(); i++) {
		m_vp[i].clear();
	}
	m_vp.clear();

	int channel = m_lpBmInfo->biBitCount/8;
	IplImage* Img;
	Img = cvCreateImage(cvSize(m_lpBmInfo->biWidth, m_lpBmInfo->biHeight), 8, channel);
	
	for(i=0;i<Img->height;++i) {
		for(j=0;j<Img->width;++j)
		{
			memcpy(&Img->imageData[i*Img->widthStep+j*channel],
				   &m_lpOringinImgData[i*Img->widthStep + j*channel], channel);
		}
	}
	
	bool **markarr;
	markarr = new bool*[Img->height];
	for(i=0;i<Img->height;++i)
	{
		markarr[i] = new bool[Img->width];
		memset(markarr[i], false, sizeof(bool)*Img->width);
	}	

	COLORTYPE ct;
	const int r = 2;
	const int nIntv = 2;
	for(i=0; i<m_vSpermPath.size(); ++i)
	{
		vector<ColorPoint>vp;
		m_vp.push_back(vp);
		ct = (COLORTYPE)(int)m_vSpermClass[i];
		if( m_vSpermClass[i] == D_CLASS ) {
			CvPoint p0 = cvPoint(m_vSpermPath[i][0].pos.x, m_vSpermPath[i][0].pos.y);
			CvPoint p1 = cvPoint(m_vSpermPath[i][0].pos.x-r, m_vSpermPath[i][0].pos.y-r);
			CvPoint p2 = cvPoint(m_vSpermPath[i][0].pos.x+r, m_vSpermPath[i][0].pos.y+r);
			cvCircle(Img, p0, 1, mColorArr[ct]);
			cvLine(Img, p1, p2, mColorArr[ct], 1);
			long l = min(p1.x, p2.x);
			long t = min(p1.y, p2.y);
			long r = max(p1.x, p2.x);
			long b = max(p1.y, p2.y);
			cvLine(Img, cvPoint(l,b), cvPoint(r,t), mColorArr[ct], 1);
			GetColorPointFromRect(Img, CRect(l, t, r, b), ct, m_vp[i], markarr);
		}
		else {	
			for(j=0; j+nIntv<m_vSpermPath[i].size(); j+=nIntv)
			{
				CvPoint p1 = cvPoint(m_vSpermPath[i][j].pos.x, m_vSpermPath[i][j].pos.y);
				CvPoint p2 = cvPoint(m_vSpermPath[i][j+nIntv].pos.x, m_vSpermPath[i][j+nIntv].pos.y);
				cvLine(Img, p1, p2, mColorArr[ct]);
				long l = min(p1.x, p2.x); 
				long t = min(p1.y, p2.y); 
				long r = max(p1.x, p2.x); 
				long b = max(p1.y, p2.y); 
				GetColorPointFromRect(Img, CRect(l, t, r, b), ct, m_vp[i], markarr);
			}
			
			CvPoint p1 = cvPoint(m_vSpermPath[i][j].pos.x, m_vSpermPath[i][j].pos.y);
			CvPoint p2 = cvPoint(m_vSpermPath[i][m_vSpermPath[i].size()-1].pos.x, m_vSpermPath[i][m_vSpermPath[i].size()-1].pos.y);
			cvLine(Img, p1, p2, mColorArr[ct]);
			long l = min(p1.x, p2.x);
			long t = min(p1.y, p2.y);
			long r = max(p1.x, p2.x);
			long b = max(p1.y, p2.y);
			GetColorPointFromRect(Img, CRect(l, t, r, b), ct, m_vp[i], markarr);
		}
	}
	
	for(i=0;i<Img->height;++i)
	{
		delete []markarr[i]; 
		markarr[i] = NULL;
	}
	delete []markarr; markarr = NULL;
	cvReleaseImage(&Img);
}

void CLiveDetectDLG::FormTrackMap()
{
	long h = theView1->m_lpTrackResultBmInfo->biHeight;
	long w = theView1->m_lpTrackResultBmInfo->biWidth;
	long lw = WIDTHBYTES(w*32);  // 每行多少字节
	long ImageSize = lw*h;
	long i, j, k;
	long r, c;
	COLORTYPE t;

	for(k=0; k< FRAME_NUM; k++)
	{
		memcpy(m_pMoveImgSeqInfo[k], theView1->m_lpBMIH[k], sizeof(BITMAPINFOHEADER));
		memcpy(m_pMoveImgSeqData[k], theView1->m_lpImage[k], ImageSize);
	}
	memcpy(theView1->m_lpTrackResultBmInfo, theView1->m_lpBMIH[0], sizeof(BITMAPINFOHEADER));
	memcpy(theView1->m_lpTrackResultBmData, theView1->m_lpImage[0], ImageSize);

	for(i=0; i<m_vp.size(); ++i)
	{
		for(j=0; j<m_vp[i].size(); j++)
		{		
			r = m_vp[i][j].pt.x;
			c = m_vp[i][j].pt.y;
			t = m_vp[i][j].ct;		
			for(k=0; k< FRAME_NUM; k++)
			{
				setBMDataColor(&m_pMoveImgSeqData[k][r*lw + c*4], t);
			}
			setBMDataColor(theView1->m_lpTrackResultBmData + r*lw + c*4, t);
		}
	}
	theView1->m_eViewShowMode = theView1->VIDEO_RESULT;
}

void CLiveDetectDLG::OnBtnMoveShow()
{
	// TODO: Add your control notification handler code here
	if( theView1->mFrm == 0 )
		theView1->mMoveShowTimer = theView1->SetTimer(MOVE_SHOW_TIMER, 50, NULL);
}
