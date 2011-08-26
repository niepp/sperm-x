#if !defined(AFX_DETECTDLG_H__FEB8B355_A218_4F7F_A5B4_760F5D9EA5D7__INCLUDED_)
#define AFX_DETECTDLG_H__FEB8B355_A218_4F7F_A5B4_760F5D9EA5D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetectDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLiveDetectDLG dialog

//{ 3.25

#include "StdAfx.h"
#include "resource.h"
#include "CaptureVideo.h"
#include "VideoPlay.h"

#define  SLIDER_TIMER  100
#define  SLIDER_MAX_LENGTH  1000
#define  MAX_PRED 1
#define  GRADE_NUM  7

#define  TARGET WHITE_PIEXL
#define  BACKGD BLACK_PIEXL

#define  SQUARE(x) ((x)*(x))

#define  A_CLASS 0
#define  B_CLASS 1
#define  C_CLASS 2
#define  D_CLASS 3

// 预编译宏
#define _MAKE_PREDICT

// #define _SAVE_IMAGE_FRAME

typedef struct DirVector
{
	double dx;
	double dy;
	DirVector(){}
	DirVector(double x, double y) : dx(x), dy(y) {}
	double Moudle(){return sqrtf(dx*dx+dy*dy);}
}DirVector;

typedef struct SpermTrack 
{
	CPoint		nowPos;     // 当前帧精子位置
	CPoint		nextPos;    // 下一帧精子位置
	size_t     nextIndex;   // 下一帧精子在所在帧中的索引号
	double		velocity;	// 速度
	DirVector   direction;	// 方向
	size_t      nPred;      // 预测帧数
	SpermTrack(){}
}SpermTrack;

typedef struct SpermPathNode{
	int     nFrame;  // 节点所在帧序号
	CPoint  pos;     // 节点在帧中位置
}SpermPathNode;

typedef vector<SpermPathNode> SpermPath; 

template<typename T>
struct vclGrade{
    T vclgrade[GRADE_NUM + 1];
};

template<typename T>
struct vslGrade{
    T vslgrade[GRADE_NUM + 1];
};

template<typename T>
struct vapGrade{
    T vapgrade[GRADE_NUM + 1];
};

enum COLORTYPE{ REDT=0, GREENT, BLUET, YELLOWT };
typedef struct ColorPoint{
	CPoint pt;
	COLORTYPE ct;
}ColorPoint;

typedef struct SpermLiveDetectResult 
{	
	CString strDetedNO;
	//精子等级分类，各级数目
	int iGradeA;
	int iGradeB;
	int iGradeC;
	int iGradeD;

	//精子等级分类;各级所占比率
	double dGradeA;
	double dGradeB;
	double dGradeC;
	double dGradeD;

	//精子等级分类;各级密度
	double dGradeADensity;
	double dGradeBDensity;
	double dGradeCDensity;
	double dGradeDDensity;

	int nTotalSpermNum;    //视野中精子总数
	int nMovementSpermNum; //活动精子数目
	int nActiveSpermNum;   //活跃精子数目,它是各视野内观察到的STR大于某个阈值的A级精子个数之和。
	int nLineMoveSpermNum; //直线运动精子数
	int nCurveMoveSpermNum;//曲线运动精子数
	int nFrontSpermNum;    //前向精子总数,a+b级

	double dTotalSpermRatio;    //视野中精子总数,为1
	double dMovementSpermRatio; //活动精子数目所占比率
	double dActiveSpermRatio;	//活跃精子数目所占比率
	double dLineMoveSpermRatio; //直线运动精子所占比率
	double dCurveMoveSpermRatio;//曲线运动精子所占比率
	double dFrontSpermRatio;    //前向精子(a+b)所占比率。

	double dTotalSpermDensity;  // 总精子密度
	double dActiveSpermDensity; // 活跃精子密度
	double dMovementSpermDensity; // 活动精子密度
	double dFrontSpermDensity;  // 前向精子密度
	double dLineSpermDensity;   // 直线精子密度
	double dCurveSpermDensity;  // 曲线精子密度


	//各个精子目标的平均运动参数
	double dVsl;
	double dVcl;
	double dVap;
	double dLin;
	double dStr;
	double dWob;
	double dAlh;
	double dBcf;

	// 标准差
	double sd_vsl;
	double sd_vcl;
	double sd_vap;
	double sd_alh;

	//{ 2008_8_26
	vclGrade<int> m_vclgrd;
	vslGrade<int> m_vslgrd;
	vapGrade<int> m_vapgrd;
	//} 2008_8_26

	// 结果图像
    LPBITMAPINFOHEADER lpSLDResultBmInfo;
	LPBYTE	lpSLDResultBmData;

	void ResultInitialize()
	{
		//strDetedNO = _T("");
		iGradeA= 0;
		iGradeB= 0;
		iGradeC= 0;
		iGradeD= 0;
		
		dGradeA    = .0;
		dGradeB    = .0;
		dGradeC    = .0;
		dGradeD    = .0;

		dGradeADensity =.0;
		dGradeBDensity =.0;
		dGradeCDensity =.0;
		dGradeDDensity =.0;
		
		nTotalSpermNum	   = 0;  
		nMovementSpermNum  = 0;  
		nActiveSpermNum	   = 0;

		nLineMoveSpermNum  = 0;
		nCurveMoveSpermNum = 0;
		nFrontSpermNum     = 0; 

		dTotalSpermRatio    = 0; //视野中精子总数,为1
		dMovementSpermRatio = 0; //活动精子数目所占比率
		dActiveSpermRatio   = 0; //活跃精子数目；所占比率
		dLineMoveSpermRatio = 0; //直线运动精子所占比率
		dCurveMoveSpermRatio= 0; //曲线运动精子所占比率
		dFrontSpermRatio    = 0; //前向精子(a+b)所占比率。
		
		dTotalSpermDensity = .0; // 总精子密度
		dActiveSpermDensity= .0; // 活跃精子密度
		dMovementSpermDensity = .0; // 活动精子密度
		dFrontSpermDensity = .0; // 前向精子密度
		dLineSpermDensity  = .0; // 直线精子密度
		dCurveSpermDensity = .0; // 曲线精子密度

		dVsl = .0;
		dVcl = .0;
		dVap = .0;
		dLin = .0;
		dStr = .0;
		dWob = .0;
		dAlh = .0;
		dBcf = .0; 

		sd_vsl = .0;
		sd_vcl = .0;
		sd_vap = .0;
		sd_alh = .0;

	    memset(m_vclgrd.vclgrade, 0, sizeof(m_vclgrd.vclgrade));
	    memset(m_vslgrd.vslgrade, 0, sizeof(m_vslgrd.vslgrade));
	    memset(m_vapgrd.vapgrade, 0, sizeof(m_vapgrd.vapgrade));

	    lpSLDResultBmInfo = NULL;
		lpSLDResultBmData = NULL;
	}
	SpermLiveDetectResult()
	{
		ResultInitialize();
	}
		
	// 保存上次检测的结果
	void SaveLiveDetectedResult(const SpermLiveDetectResult& sldResult)
	{
		strDetedNO = sldResult.strDetedNO;
		iGradeA += sldResult.iGradeA;
		iGradeB += sldResult.iGradeB;
		iGradeC += sldResult.iGradeC;
		iGradeD += sldResult.iGradeD; 

        for (int i=0; i<GRADE_NUM; i++)
        {
			m_vclgrd.vclgrade[i] += sldResult.m_vclgrd.vclgrade[i];
			m_vslgrd.vslgrade[i] += sldResult.m_vslgrd.vslgrade[i];
			m_vapgrd.vapgrade[i] += sldResult.m_vapgrd.vapgrade[i];
        }

		//各级所占比率
		if( nTotalSpermNum + sldResult.nTotalSpermNum > 0 )
		{
			dGradeA    = (dGradeA*nTotalSpermNum + sldResult.dGradeA*sldResult.nTotalSpermNum) \
						 /(nTotalSpermNum + sldResult.nTotalSpermNum);
			dGradeB    = (dGradeB*nTotalSpermNum + sldResult.dGradeB*sldResult.nTotalSpermNum) \
						 /(nTotalSpermNum + sldResult.nTotalSpermNum);
			dGradeC    = (dGradeC*nTotalSpermNum + sldResult.dGradeC*sldResult.nTotalSpermNum) \
						 /(nTotalSpermNum + sldResult.nTotalSpermNum);
			dGradeD    = (dGradeD*nTotalSpermNum + sldResult.dGradeD*sldResult.nTotalSpermNum) \
						 /(nTotalSpermNum + sldResult.nTotalSpermNum);
		}
		else{
			dGradeA    = .0;
			dGradeB    = .0;
			dGradeC    = .0;
			dGradeD    = .0;
		}

		//精子等级分类;各级密度
		dGradeADensity = (dGradeADensity + sldResult.dGradeADensity)/2.0;
		dGradeBDensity = (dGradeBDensity + sldResult.dGradeBDensity)/2.0;
		dGradeCDensity = (dGradeCDensity + sldResult.dGradeCDensity)/2.0;
		dGradeDDensity = (dGradeDDensity + sldResult.dGradeDDensity)/2.0;

		//各精子数目所占比率
		if( nTotalSpermNum + sldResult.nTotalSpermNum > 0 )
		{
			dTotalSpermRatio = 1;   // 为1
			dMovementSpermRatio = (dMovementSpermRatio*nTotalSpermNum + sldResult.dMovementSpermRatio*sldResult.nTotalSpermNum) \
								  /(nTotalSpermNum + sldResult.nTotalSpermNum); 
			dActiveSpermRatio = (dActiveSpermRatio*nTotalSpermNum + sldResult.dActiveSpermRatio*sldResult.nTotalSpermNum) \
								  /(nTotalSpermNum + sldResult.nTotalSpermNum); 	 
			dLineMoveSpermRatio = (dLineMoveSpermRatio*nTotalSpermNum + sldResult.dLineMoveSpermRatio*sldResult.nTotalSpermNum) \
								  /(nTotalSpermNum + sldResult.nTotalSpermNum);  
			dCurveMoveSpermRatio = (dCurveMoveSpermRatio*nTotalSpermNum + sldResult.dCurveMoveSpermRatio*sldResult.nTotalSpermNum) \
								  /(nTotalSpermNum + sldResult.nTotalSpermNum); 
			dFrontSpermRatio = (dFrontSpermRatio*nTotalSpermNum + sldResult.dFrontSpermRatio*sldResult.nTotalSpermNum) \
								  /(nTotalSpermNum + sldResult.nTotalSpermNum);     

			//各精子数目密度
			dTotalSpermDensity  = (dTotalSpermDensity*nTotalSpermNum + sldResult.dTotalSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dActiveSpermDensity = (dActiveSpermDensity*nTotalSpermNum + sldResult.dActiveSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dMovementSpermDensity = (dMovementSpermDensity*nTotalSpermNum + sldResult.dMovementSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			
			dFrontSpermDensity = (dFrontSpermDensity*nTotalSpermNum + sldResult.dFrontSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dLineSpermDensity = (dLineSpermDensity*nTotalSpermNum + sldResult.dLineSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);   
			dCurveSpermDensity = (dCurveSpermDensity*nTotalSpermNum + sldResult.dCurveSpermDensity*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);

			dVsl = (dVsl*nTotalSpermNum + sldResult.dVsl*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dVcl = (dVcl*nTotalSpermNum + sldResult.dVcl*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dVap = (dVap*nTotalSpermNum + sldResult.dVap*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dLin = (dLin*nTotalSpermNum + sldResult.dLin*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dStr = (dStr*nTotalSpermNum + sldResult.dStr*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dWob = (dWob*nTotalSpermNum + sldResult.dWob*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dAlh = (dAlh*nTotalSpermNum + sldResult.dAlh*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);
			dBcf = (dBcf*nTotalSpermNum + sldResult.dBcf*sldResult.nTotalSpermNum) \
									/(nTotalSpermNum + sldResult.nTotalSpermNum);

			sd_vsl = sldResult.sd_vsl;
			sd_vcl = sldResult.sd_vcl;
			sd_vap = sldResult.sd_vap;
			sd_alh = sldResult.sd_alh;
		}
		else{
			dTotalSpermRatio     = .0;
			dMovementSpermRatio  = .0; 
			dActiveSpermRatio    = .0; 	 
			dLineMoveSpermRatio  = .0;  
			dCurveMoveSpermRatio = .0; 
			dFrontSpermRatio     = .0;     

			//各精子数目密度
			dTotalSpermDensity    = .0;
			dActiveSpermDensity   = .0;
			dMovementSpermDensity = .0;
			
			dFrontSpermDensity = .0;
			dLineSpermDensity  = .0;   
			dCurveSpermDensity = .0;

			dVsl = .0;
			dVcl = .0;
			dVap = .0;
			dLin = .0;
			dStr = .0;
			dWob = .0;
			dAlh = .0;
			dBcf = .0; 
		}
		
		nTotalSpermNum	   += sldResult.nTotalSpermNum;
		nMovementSpermNum  += sldResult.nMovementSpermNum;
		nActiveSpermNum	   += sldResult.nActiveSpermNum;
		nLineMoveSpermNum  += sldResult.nLineMoveSpermNum;
		nCurveMoveSpermNum += sldResult.nCurveMoveSpermNum;
	    nFrontSpermNum	   += sldResult.nFrontSpermNum;

		lpSLDResultBmInfo = sldResult.lpSLDResultBmInfo;
	    lpSLDResultBmData = sldResult.lpSLDResultBmData;

	}

}SpermLiveDetectResult;

struct WRDataThdPara{
		CString strSQLLiveResult;
		CString strSQLSpermStatics;
		CString strSQLMorPara;
		CString strSQLVelDistribute;
		CString strSQLcmpwithWHO;
		SpermLiveDetectResult SLDTotalResult;
		HWND hWnd;
};

struct SequenceAnalyseThdPara{
	HWND hWnd;          //  用于更新CLiveDetectDLG
	LPBYTE* m_lpImage;			// 序列帧数据区指针,CSpermview中
	LPBITMAPINFOHEADER* m_lpBMIH; // 序列帧信息头的指针,CSpermview中
	int iSpermszHIGH;
	int iSpermszLOW;
	int threshvalue;    //  分割阈值
};

struct TargetMarkAndCenterThdPara{

	int i;   // 第几个序列帧
	int** ppIVisitMarkAry;
	int iSpermszHIGH;
	int iSpermszLOW;
};

struct InitCameraThdPara{
	HWND hWnd;
	CCaptureVideo * pCap;
	int  nDectNO;
};
//} 3.25

class CLiveDetectDLG : public CDialog
{
// Construction
public:
	CLiveDetectDLG(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CLiveDetectDLG)
	enum { IDD = IDD_DIALOG_LIVEDETECT };
	CEdit	m_wndRoundness;
	CSliderCtrl	mSliderGraph;
	CEdit	m_wndEditHigh;
	CEdit	m_wndEditLow;
	CEdit	m_wndGrayValue;
	CSliderCtrl	m_wndSliderGrayValue;
	double 	m_spermszLOW;
	double	m_spermszHIGH;
	int		m_nGrayValue;
	CString	m_strSumNum;
	CString	m_LiveDetectedInfoStr;
	double	m_dRoundness;
	//}}AFX_DATA
	UINT	m_nSpermszLOW;
	UINT	m_nSpermszHIGH;
	double  m_ddRoundness;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLiveDetectDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:

	public:

	// Generated message map functions
	//{{AFX_MSG(CLiveDetectDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditSpermszLow();
	afx_msg void OnChangeEditSpermszHigh();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditGrayvalue();
	afx_msg void OnKillfocusEditSpermszHigh();
	afx_msg void OnKillfocusEditSpermszLow();
	afx_msg void OnBtnContinuedetect();
	afx_msg void OnBtnFinishdetect();
	afx_msg void OnBtnAutoanalyse();
	afx_msg void OnBtnModeChange();
	afx_msg void OnBtnPamSetting();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnPause();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnResume();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnMoveshow();
	afx_msg void OnChangeEditRoundness();

	afx_msg void OnBtnRoundLess();
	afx_msg void OnBtnRoundBigger();
	afx_msg void OnBtnMoveShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	LRESULT AfterImageSequenctAnalyse(WPARAM w,LPARAM l);
	LRESULT TargetMarkAndTargetCenterMark(WPARAM w,LPARAM l);
	static UINT ImageSequenceAnalyseThread(LPVOID param);
	LRESULT ShowLiveProgress(WPARAM w,LPARAM l);
	LRESULT ShowCaptureProgress(WPARAM w, LPARAM l);
	LRESULT AfterWriteData(WPARAM w, LPARAM l);
	static UINT WriteDataThread(LPVOID para);
	static UINT InitCameraThread(LPVOID param);

	// 手工调整

	void OnTargetAdjustChoose(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, CPoint p, bool** ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness);
	int  OnTargetAdjustMark(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool** ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness);
	int  OnTargetAdjust(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool **ppIAdjustVisitMarkAry, int iMinArea, int iMaxArea, double dRdness, int thrd);
	bool IsBoundary(CPoint p, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);
	int m_nSumNum;

	// 序列帧处理
	vector<vector<CPoint> >m_vSperm[FRAME_NUM];
	vector<CPoint>m_vSpermCenterPos[FRAME_NUM];   // 记录精子质心位置

	void markSperm(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], CPoint p, int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize);
	void OnTargetMark(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize);	
	void OnTargetCenterMark(int i);
	void OnTargetOverlay(int x);

	static void WriteImage(const CString &strDetectNO, LPBITMAPINFOHEADER lpHead, LPBYTE lpData);
	void GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER& lpHead, LPBYTE& lpData);

	// 路径跟踪
	//{ 3.25
	
	double m_dDilutionRatio; // 稀释比
	double m_dDepth;  
	int m_nEnlargeRatio; // 显微镜放大倍数
	int m_nR; // 搜索范围半径
	double m_dFrequency; // 帧频
	double m_dVolume; // 体积

	
	// 各个阈值参数
	double m_LIN_Thrshold;
	double m_CUR_Thrshold;
	double m_STR_Thrshold;
	double m_v0;
	double m_v1;
	double m_v2;

	vector<SpermTrack> m_vSpermTrack[FRAME_NUM]; 
	
    void SpermMoveTrack();		// 帧间目标匹配
	void GetSpermPath();		// 原始路径
    void GetSpermAvgPath();	    // 平均路径
	void GetSpermMoveParameter();	// 参数计算
	void ShowSpermLiveDetectedResult(); // 显示分析结果
	void ClearDataBuffer();		// 清空各个数据存储区

	void ClearView3();  // 清空View3的显示结果

	// { add by happynp@gmail.com 2009_3_31
	LPBYTE m_pMoveImgSeqData[FRAME_NUM];  // 视频回放序列图像
	LPBITMAPINFOHEADER m_pMoveImgSeqInfo[FRAME_NUM];
	void FormMoveFrame();
	void GetColorPointFromRect(IplImage *Img, const CRect &rc, COLORTYPE ct, vector<ColorPoint>&vp, bool **marr);
	void FormTrackMap(); // 形成视频回放序列图像
	vector<vector<ColorPoint> >m_vp; // 记录每帧图像中每个精子的轨迹及其颜色,用于形成视频回放序列图像，便于增加，删除精子
	vector<char>m_vSpermClass; // 记录每个精子的ABCD级分类
	DWORD mCR[4]; // 颜色表
	CvScalar mColorArr[4]; // 颜色表
	// } add by happynp@gmail.com 2009_3_31

	//} 3.25

	//{ 3.26

	vector<SpermPath> m_vSpermPath;     // 原始路径
	vector<SpermPath> m_vSpermAvgPath;  // 平均路径
	SpermLiveDetectResult  m_SLDResult; // 分析结果
	SpermLiveDetectResult  m_SLDTotalResult; // 各个视野分析的累加结果
	int m_nSpermNumSumInAllFrame;

	bool m_bIsVideoPlay;   // 视频是否在播放

//{ 3.28
	vector<double> m_vVSL; //各个精子目标的直线速度
	vector<double> m_vVCL; //每个精子目标的曲线速度
	vector<double> m_vVAP; //各个精子目标的平均路径速度
	vector<double> m_vLIN; //各个精子目标的直线性
	vector<double> m_vSTR; //各个精子目标的前向性
	vector<double> m_vWOB; //各个精子目标的摆动性
	vector<double> m_vALH; //各个精子目标的侧摆幅度
	vector<double> m_vBCF; //各个精子目标的鞭打频率
//} 3.28

	//} 3.26

	~CLiveDetectDLG();

	// 原图
	LPBITMAPINFOHEADER m_lpOringinImgHead;
	LPBYTE             m_lpOringinImgData;

	// 二值图
	IplImage* m_iplBinaryImage;
	
	// 标记数组
	bool ** m_ppIVisitMarkAry;

	//{ 3.13
	vector<vector<CPoint> > m_vSpermEdge; 
	LPBITMAPINFOHEADER m_lpBmInfo;
	LPBYTE m_lpBmData;
	//} 3.13 
	int m_nHeight;  // 图像高
	int m_nWidth;	// 图像宽

	int m_nLiveDetectedViewSight; // 检测了多少个视野区

	// 界面控制
	// 按钮项
	bool m_bPamaSetting;
	bool m_bModeChange;
	bool m_bAutoAnalyse;
	bool m_bContinueDetected;
	bool m_bFinishDetected;
	bool m_bMoveShow;

	// 调整控件项
	bool m_bControlState;

	int m_nYesNo;  //  窗口是否关闭

	CCaptureVideo m_cap; // 负责摄像头视频捕获
	CMyVideoPlay *m_pVideoPlay;  // 负责播放视频文件
	int m_nFrameNum;	
	int m_nFrmCount;
	int m_nIntrv;   // 视频采集间隔
	bool m_bCanCap;

	//
	// 按钮控制
	bool m_bVideoPlay;
	bool m_bVideoStop;
	bool m_bVideoPause;
	bool m_bVideoResume;

	UINT mSliderTimer;   // 定时器ID
    int  mNowVideoPlayPos;
	bool IsVideoStopped();

	// add by happynp@gmail.com 2008_9_3
    BOOL m_bIsUserState;
	// 2008_9_3

	void EnableAdjustControl();
	void OnUpdateButtonState();

	// { add by happynp@gmail.com 2009_3_13
    int m_nAclass;
	int m_nBclass;
	int m_nCclass;
	int m_nDclass;
	// } 2009_3_13

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETECTDLG_H__FEB8B355_A218_4F7F_A5B4_760F5D9EA5D7__INCLUDED_)
