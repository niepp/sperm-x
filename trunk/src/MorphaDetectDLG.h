 #if !defined(AFX_MORPHADETECTDLG_H__F89F1D29_CDE9_4F00_AC06_3BBD1DA2BAAC__INCLUDED_)
#define AFX_MORPHADETECTDLG_H__F89F1D29_CDE9_4F00_AC06_3BBD1DA2BAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MorphaDetectDLG.h : header file
//

#include "SperMopyParameter.h"
#include "Wraper.h"
#include "MainFrm.h"
#include "SpermView.h"
#include "SpermMorphaSet.h"

/////////////////////////////////////////////////////////////////////////////
// CMorphaDetectDLG dialog
typedef struct tagSpermRegion{
	BOOL isDeleted;   //  TRUE  该精子被删除,FALSE  没有删除
	POINT  SpermCenterPos;
	vector<POINT>Region;
	tagSpermRegion()
	{
		isDeleted = FALSE;
	}
}SpermRegion;

// 单个精子区域图像
typedef struct tagSpermImage{    
	BOOL isDeleted;   //  TRUE  该精子被删除,FALSE  没有删除
    LPBITMAPINFOHEADER  m_lpBmpInfo;
    LPBYTE              m_lpBmpData;
	tagSpermImage()
	{
		isDeleted = FALSE;
		m_lpBmpInfo = NULL;
		m_lpBmpData = NULL;
	}
}SpermImage;

// 精子各个形态参数正常与否
typedef struct tagIsSperNormal{ 
	BOOL isDeleted;   //  TRUE  该精子被删除,FALSE  没有删除
	// FALSE: 异常;
	// TRUE : 正常;
	BOOL IsNormalVector[1+nPARAMETER];
	tagIsSperNormal()
	{
		isDeleted = FALSE;
		memset(IsNormalVector,FALSE,sizeof(IsNormalVector));
	}
}IsSperNormal;

class CMorphaDetectDLG;

struct ThreadInfo{
	CMorphaDetectDLG* pDlg;
	CSpermView * pView1;
};
struct UIControl
{
	BOOL m_BContinueDetect;
	BOOL m_BRestoreCamera;
	BOOL m_BFinishDetect;
};

class CMorphaDetectDLG : public CDialog
{
// Construction
public:
	void InitStr();
	CMorphaDetectDLG(CWnd* pParent = NULL);   // standard constructor
    CString IntTOCString(int n);
	void WriteImageToDB(const CString &strpID, const CString &fieldname, LPBITMAPINFOHEADER lpHead, LPBYTE lpData);

// Dialog Data
	//{{AFX_DATA(CMorphaDetectDLG)
	enum { IDD = IDD_DIALOG_MORPHADETECT };
	CComboBox	m_wndCbxShowOpt;
	CButton	m_wndChkRectangle;
	CString	m_strArea;
	CString	m_strWidth;
	CString	m_strPerimeter;
	CString	m_strEllipticity;
	CString	m_strExtension;
	CString	m_strHeadArea;
	CString	m_strHeadPerforArea;
	CString	m_strMitosmaDevangle;
	CString	m_strMitsomaLength;
	CString	m_strMitosomaWidth;
	CString	m_strPerforArea;
	CString	m_strSymmetry;
	CString	m_strInfo;
	CString	m_strIsNormal;
	CString	m_strRuga;
	CString	m_strLength;
	//}}AFX_DATA

	int  m_nMicroScope;

	// add by happynp 2009_3_11
    BOOL m_bIsDataGather;
	// 2009_3_11

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMorphaDetectDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CMorphaDetectDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMorphadetectContinue();
	afx_msg void OnPaint();
	afx_msg void OnCheckRectangle();
	afx_msg void OnSelchangeComboShowopt();
	afx_msg void OnButtonMorphadetectFinish();
	afx_msg void OnClose();
	afx_msg void OnButtonDeletesperm();
	afx_msg void OnButtonMorphadetectRestore();
	virtual void OnCancel();
	afx_msg void OnButtonMorphadetectParameter();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	~CMorphaDetectDLG();

public:   //  精子形态学分析
	SperMopyParameter PiexlValueToReallity(SperMopyParameter &smp);
	void GetSpermPartialAbnormalNum();
	void GetNumOfEachKindsOfAbnormalSperm();

	void WriteMorphaSpermData(int nSelNO, bool IsNormal);
	void WriteMorphaSpermInfo();
	void SetUI(const UIControl& ui);
	void WriteImage(const CString &strDetectNO,const CString& fieldname, LPBITMAPINFOHEADER lpHead, LPBYTE lpData);
	void AddSperm(int selNO);
	void DeleteSperm(int selNO);
	void GetAllKindsSpermNum();
	void ShowSperm(int iPos);
    void clearworkspace();
	void InitializeStd(CString filename);
	void FormResultImage();
	void DiagnoseAllSperm();
	IsSperNormal IsNormal(SperMopyParameter tSperMopyParameter);
	void SperMopyAnalyse();
	void AutoSeg();
	void ImagePreProcess();
	static UINT ThreadHandle_AutoAnalyse(LPVOID lpParam);
    bool IsBoundary(CPoint p, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);
	int m_nYesNo;  //  窗口是否关闭
	int m_nR;
	int m_nVideoFiled;
	BOOL m_BFinished;
	LPBITMAPINFOHEADER m_lpBMMorphaInfo;
	LPBYTE             m_lpBMMorphaData;
	int m_nSelNO;    //  当前选择的精子编号
	IplImage* m_iplMorphaGrayImage;             //   整幅形态学分析图像,三通道灰度图，值相等             
	IplImage* m_iplMorphaColorImg;              //   整幅形态学分析图像,彩色图
	IplImage* m_iplTempImg;                     //   中间结果图
	IplImage* m_rImg;							//   单个精子拼接图
	
	CWraper* m_pSigSpermRecog;  // 单个精子形态学分析
	int   m_nMaxArea; 
     	// 精子细胞图像面积阈值
	int   m_nEdgeThick;
       	// 精子细胞中心点离图像边界距离小于这个值就认为是不完全落在当前图像里面 

	vector<SpermRegion> m_vSpermRegion;
	   // 各个精子的分割结果，存储像素位置点, 及各个精子的中心位置
	SperMopyParameter  m_stdInf_SperMopyParameter;  // 形态参数标准下限值
    SperMopyParameter  m_stdSup_SperMopyParameter;  // 形态参数标准上限值
	vector<SpermImage> m_vRSpermPaddedImage;         // 记录各个精子分析结果的填充图像,小图显示
	vector<SpermImage> m_vRSpermImage;               // 记录各个精子分析结果的填充图像,大图显示
	vector<SpermImage> m_vRSpermEdgeImage;           // 记录各个精子分析结果的边缘图像
	vector<SpermImage> m_vOSpermImage;               // 记录各个精子原始的图像
    vector<SperMopyParameter>m_vSperMopyParameter;  // 记录各个精子的形态参数
  
	vector<IsSperNormal> m_vbIsNormal;               // 根据参数所作的诊断

	tagSpermMorphaNum    m_MhNum;               //  记录当前检测的各种精子的个数
	SpermPartialMorphaNum m_PtAmRes;   // 结果
	
	// 分析结果图---涂色显示
    LPBITMAPINFOHEADER  m_lpResBmInfoSrc;
	LPBYTE              m_lpResBmDataSrc;
	// 分析结果图---边缘标记显示
    LPBITMAPINFOHEADER  m_lpResEdgeBmInfoSrc;
	LPBYTE              m_lpResEdgeBmDataSrc;

	CSpermMorphaSet m_wndMorphaSet;

	list<int> m_lsOperateIndex;          //  保存删除精子数记录
	list<int>::iterator m_itListIndex;   //  指向当前的下一个

	UIControl m_UICtrl;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHADETECTDLG_H__F89F1D29_CDE9_4F00_AC06_3BBD1DA2BAAC__INCLUDED_)
