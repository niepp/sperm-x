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

// Ԥ�����
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
	CPoint		nowPos;     // ��ǰ֡����λ��
	CPoint		nextPos;    // ��һ֡����λ��
	size_t     nextIndex;   // ��һ֡����������֡�е�������
	double		velocity;	// �ٶ�
	DirVector   direction;	// ����
	size_t      nPred;      // Ԥ��֡��
	SpermTrack(){}
}SpermTrack;

typedef struct SpermPathNode{
	int     nFrame;  // �ڵ�����֡���
	CPoint  pos;     // �ڵ���֡��λ��
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
	//���ӵȼ����࣬������Ŀ
	int iGradeA;
	int iGradeB;
	int iGradeC;
	int iGradeD;

	//���ӵȼ�����;������ռ����
	double dGradeA;
	double dGradeB;
	double dGradeC;
	double dGradeD;

	//���ӵȼ�����;�����ܶ�
	double dGradeADensity;
	double dGradeBDensity;
	double dGradeCDensity;
	double dGradeDDensity;

	int nTotalSpermNum;    //��Ұ�о�������
	int nMovementSpermNum; //�������Ŀ
	int nActiveSpermNum;   //��Ծ������Ŀ,���Ǹ���Ұ�ڹ۲쵽��STR����ĳ����ֵ��A�����Ӹ���֮�͡�
	int nLineMoveSpermNum; //ֱ���˶�������
	int nCurveMoveSpermNum;//�����˶�������
	int nFrontSpermNum;    //ǰ��������,a+b��

	double dTotalSpermRatio;    //��Ұ�о�������,Ϊ1
	double dMovementSpermRatio; //�������Ŀ��ռ����
	double dActiveSpermRatio;	//��Ծ������Ŀ��ռ����
	double dLineMoveSpermRatio; //ֱ���˶�������ռ����
	double dCurveMoveSpermRatio;//�����˶�������ռ����
	double dFrontSpermRatio;    //ǰ����(a+b)��ռ���ʡ�

	double dTotalSpermDensity;  // �ܾ����ܶ�
	double dActiveSpermDensity; // ��Ծ�����ܶ�
	double dMovementSpermDensity; // ������ܶ�
	double dFrontSpermDensity;  // ǰ�����ܶ�
	double dLineSpermDensity;   // ֱ�߾����ܶ�
	double dCurveSpermDensity;  // ���߾����ܶ�


	//��������Ŀ���ƽ���˶�����
	double dVsl;
	double dVcl;
	double dVap;
	double dLin;
	double dStr;
	double dWob;
	double dAlh;
	double dBcf;

	// ��׼��
	double sd_vsl;
	double sd_vcl;
	double sd_vap;
	double sd_alh;

	//{ 2008_8_26
	vclGrade<int> m_vclgrd;
	vslGrade<int> m_vslgrd;
	vapGrade<int> m_vapgrd;
	//} 2008_8_26

	// ���ͼ��
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

		dTotalSpermRatio    = 0; //��Ұ�о�������,Ϊ1
		dMovementSpermRatio = 0; //�������Ŀ��ռ����
		dActiveSpermRatio   = 0; //��Ծ������Ŀ����ռ����
		dLineMoveSpermRatio = 0; //ֱ���˶�������ռ����
		dCurveMoveSpermRatio= 0; //�����˶�������ռ����
		dFrontSpermRatio    = 0; //ǰ����(a+b)��ռ���ʡ�
		
		dTotalSpermDensity = .0; // �ܾ����ܶ�
		dActiveSpermDensity= .0; // ��Ծ�����ܶ�
		dMovementSpermDensity = .0; // ������ܶ�
		dFrontSpermDensity = .0; // ǰ�����ܶ�
		dLineSpermDensity  = .0; // ֱ�߾����ܶ�
		dCurveSpermDensity = .0; // ���߾����ܶ�

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
		
	// �����ϴμ��Ľ��
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

		//������ռ����
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

		//���ӵȼ�����;�����ܶ�
		dGradeADensity = (dGradeADensity + sldResult.dGradeADensity)/2.0;
		dGradeBDensity = (dGradeBDensity + sldResult.dGradeBDensity)/2.0;
		dGradeCDensity = (dGradeCDensity + sldResult.dGradeCDensity)/2.0;
		dGradeDDensity = (dGradeDDensity + sldResult.dGradeDDensity)/2.0;

		//��������Ŀ��ռ����
		if( nTotalSpermNum + sldResult.nTotalSpermNum > 0 )
		{
			dTotalSpermRatio = 1;   // Ϊ1
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

			//��������Ŀ�ܶ�
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

			//��������Ŀ�ܶ�
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
	HWND hWnd;          //  ���ڸ���CLiveDetectDLG
	LPBYTE* m_lpImage;			// ����֡������ָ��,CSpermview��
	LPBITMAPINFOHEADER* m_lpBMIH; // ����֡��Ϣͷ��ָ��,CSpermview��
	int iSpermszHIGH;
	int iSpermszLOW;
	int threshvalue;    //  �ָ���ֵ
};

struct TargetMarkAndCenterThdPara{

	int i;   // �ڼ�������֡
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

	// �ֹ�����

	void OnTargetAdjustChoose(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, CPoint p, bool** ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness);
	int  OnTargetAdjustMark(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool** ppIAdjustVisitMarkAry, int iMaxSpermSize, int iMinSpermSize, double dRdness);
	int  OnTargetAdjust(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData, bool **ppIAdjustVisitMarkAry, int iMinArea, int iMaxArea, double dRdness, int thrd);
	bool IsBoundary(CPoint p, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);
	int m_nSumNum;

	// ����֡����
	vector<vector<CPoint> >m_vSperm[FRAME_NUM];
	vector<CPoint>m_vSpermCenterPos[FRAME_NUM];   // ��¼��������λ��

	void markSperm(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], CPoint p, int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize);
	void OnTargetMark(LPBITMAPINFOHEADER lpBMIH[], LPBYTE lpImage[], int sn, int **ppIVisitMarkAry, int iMaxSpermSize, int iMinSpermSize);	
	void OnTargetCenterMark(int i);
	void OnTargetOverlay(int x);

	static void WriteImage(const CString &strDetectNO, LPBITMAPINFOHEADER lpHead, LPBYTE lpData);
	void GetImage(const CString &strDetectNO, LPBITMAPINFOHEADER& lpHead, LPBYTE& lpData);

	// ·������
	//{ 3.25
	
	double m_dDilutionRatio; // ϡ�ͱ�
	double m_dDepth;  
	int m_nEnlargeRatio; // ��΢���Ŵ���
	int m_nR; // ������Χ�뾶
	double m_dFrequency; // ֡Ƶ
	double m_dVolume; // ���

	
	// ������ֵ����
	double m_LIN_Thrshold;
	double m_CUR_Thrshold;
	double m_STR_Thrshold;
	double m_v0;
	double m_v1;
	double m_v2;

	vector<SpermTrack> m_vSpermTrack[FRAME_NUM]; 
	
    void SpermMoveTrack();		// ֡��Ŀ��ƥ��
	void GetSpermPath();		// ԭʼ·��
    void GetSpermAvgPath();	    // ƽ��·��
	void GetSpermMoveParameter();	// ��������
	void ShowSpermLiveDetectedResult(); // ��ʾ�������
	void ClearDataBuffer();		// ��ո������ݴ洢��

	void ClearView3();  // ���View3����ʾ���

	// { add by happynp@gmail.com 2009_3_31
	LPBYTE m_pMoveImgSeqData[FRAME_NUM];  // ��Ƶ�ط�����ͼ��
	LPBITMAPINFOHEADER m_pMoveImgSeqInfo[FRAME_NUM];
	void FormMoveFrame();
	void GetColorPointFromRect(IplImage *Img, const CRect &rc, COLORTYPE ct, vector<ColorPoint>&vp, bool **marr);
	void FormTrackMap(); // �γ���Ƶ�ط�����ͼ��
	vector<vector<ColorPoint> >m_vp; // ��¼ÿ֡ͼ����ÿ�����ӵĹ켣������ɫ,�����γ���Ƶ�ط�����ͼ�񣬱������ӣ�ɾ������
	vector<char>m_vSpermClass; // ��¼ÿ�����ӵ�ABCD������
	DWORD mCR[4]; // ��ɫ��
	CvScalar mColorArr[4]; // ��ɫ��
	// } add by happynp@gmail.com 2009_3_31

	//} 3.25

	//{ 3.26

	vector<SpermPath> m_vSpermPath;     // ԭʼ·��
	vector<SpermPath> m_vSpermAvgPath;  // ƽ��·��
	SpermLiveDetectResult  m_SLDResult; // �������
	SpermLiveDetectResult  m_SLDTotalResult; // ������Ұ�������ۼӽ��
	int m_nSpermNumSumInAllFrame;

	bool m_bIsVideoPlay;   // ��Ƶ�Ƿ��ڲ���

//{ 3.28
	vector<double> m_vVSL; //��������Ŀ���ֱ���ٶ�
	vector<double> m_vVCL; //ÿ������Ŀ��������ٶ�
	vector<double> m_vVAP; //��������Ŀ���ƽ��·���ٶ�
	vector<double> m_vLIN; //��������Ŀ���ֱ����
	vector<double> m_vSTR; //��������Ŀ���ǰ����
	vector<double> m_vWOB; //��������Ŀ��İڶ���
	vector<double> m_vALH; //��������Ŀ��Ĳ�ڷ���
	vector<double> m_vBCF; //��������Ŀ��ı޴�Ƶ��
//} 3.28

	//} 3.26

	~CLiveDetectDLG();

	// ԭͼ
	LPBITMAPINFOHEADER m_lpOringinImgHead;
	LPBYTE             m_lpOringinImgData;

	// ��ֵͼ
	IplImage* m_iplBinaryImage;
	
	// �������
	bool ** m_ppIVisitMarkAry;

	//{ 3.13
	vector<vector<CPoint> > m_vSpermEdge; 
	LPBITMAPINFOHEADER m_lpBmInfo;
	LPBYTE m_lpBmData;
	//} 3.13 
	int m_nHeight;  // ͼ���
	int m_nWidth;	// ͼ���

	int m_nLiveDetectedViewSight; // ����˶��ٸ���Ұ��

	// �������
	// ��ť��
	bool m_bPamaSetting;
	bool m_bModeChange;
	bool m_bAutoAnalyse;
	bool m_bContinueDetected;
	bool m_bFinishDetected;
	bool m_bMoveShow;

	// �����ؼ���
	bool m_bControlState;

	int m_nYesNo;  //  �����Ƿ�ر�

	CCaptureVideo m_cap; // ��������ͷ��Ƶ����
	CMyVideoPlay *m_pVideoPlay;  // ���𲥷���Ƶ�ļ�
	int m_nFrameNum;	
	int m_nFrmCount;
	int m_nIntrv;   // ��Ƶ�ɼ����
	bool m_bCanCap;

	//
	// ��ť����
	bool m_bVideoPlay;
	bool m_bVideoStop;
	bool m_bVideoPause;
	bool m_bVideoResume;

	UINT mSliderTimer;   // ��ʱ��ID
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
