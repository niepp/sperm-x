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
	BOOL isDeleted;   //  TRUE  �þ��ӱ�ɾ��,FALSE  û��ɾ��
	POINT  SpermCenterPos;
	vector<POINT>Region;
	tagSpermRegion()
	{
		isDeleted = FALSE;
	}
}SpermRegion;

// ������������ͼ��
typedef struct tagSpermImage{    
	BOOL isDeleted;   //  TRUE  �þ��ӱ�ɾ��,FALSE  û��ɾ��
    LPBITMAPINFOHEADER  m_lpBmpInfo;
    LPBYTE              m_lpBmpData;
	tagSpermImage()
	{
		isDeleted = FALSE;
		m_lpBmpInfo = NULL;
		m_lpBmpData = NULL;
	}
}SpermImage;

// ���Ӹ�����̬�����������
typedef struct tagIsSperNormal{ 
	BOOL isDeleted;   //  TRUE  �þ��ӱ�ɾ��,FALSE  û��ɾ��
	// FALSE: �쳣;
	// TRUE : ����;
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

public:   //  ������̬ѧ����
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
	int m_nYesNo;  //  �����Ƿ�ر�
	int m_nR;
	int m_nVideoFiled;
	BOOL m_BFinished;
	LPBITMAPINFOHEADER m_lpBMMorphaInfo;
	LPBYTE             m_lpBMMorphaData;
	int m_nSelNO;    //  ��ǰѡ��ľ��ӱ��
	IplImage* m_iplMorphaGrayImage;             //   ������̬ѧ����ͼ��,��ͨ���Ҷ�ͼ��ֵ���             
	IplImage* m_iplMorphaColorImg;              //   ������̬ѧ����ͼ��,��ɫͼ
	IplImage* m_iplTempImg;                     //   �м���ͼ
	IplImage* m_rImg;							//   ��������ƴ��ͼ
	
	CWraper* m_pSigSpermRecog;  // ����������̬ѧ����
	int   m_nMaxArea; 
     	// ����ϸ��ͼ�������ֵ
	int   m_nEdgeThick;
       	// ����ϸ�����ĵ���ͼ��߽����С�����ֵ����Ϊ�ǲ���ȫ���ڵ�ǰͼ������ 

	vector<SpermRegion> m_vSpermRegion;
	   // �������ӵķָ������洢����λ�õ�, ���������ӵ�����λ��
	SperMopyParameter  m_stdInf_SperMopyParameter;  // ��̬������׼����ֵ
    SperMopyParameter  m_stdSup_SperMopyParameter;  // ��̬������׼����ֵ
	vector<SpermImage> m_vRSpermPaddedImage;         // ��¼�������ӷ�����������ͼ��,Сͼ��ʾ
	vector<SpermImage> m_vRSpermImage;               // ��¼�������ӷ�����������ͼ��,��ͼ��ʾ
	vector<SpermImage> m_vRSpermEdgeImage;           // ��¼�������ӷ�������ı�Եͼ��
	vector<SpermImage> m_vOSpermImage;               // ��¼��������ԭʼ��ͼ��
    vector<SperMopyParameter>m_vSperMopyParameter;  // ��¼�������ӵ���̬����
  
	vector<IsSperNormal> m_vbIsNormal;               // ���ݲ������������

	tagSpermMorphaNum    m_MhNum;               //  ��¼��ǰ���ĸ��־��ӵĸ���
	SpermPartialMorphaNum m_PtAmRes;   // ���
	
	// �������ͼ---Ϳɫ��ʾ
    LPBITMAPINFOHEADER  m_lpResBmInfoSrc;
	LPBYTE              m_lpResBmDataSrc;
	// �������ͼ---��Ե�����ʾ
    LPBITMAPINFOHEADER  m_lpResEdgeBmInfoSrc;
	LPBYTE              m_lpResEdgeBmDataSrc;

	CSpermMorphaSet m_wndMorphaSet;

	list<int> m_lsOperateIndex;          //  ����ɾ����������¼
	list<int>::iterator m_itListIndex;   //  ָ��ǰ����һ��

	UIControl m_UICtrl;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MORPHADETECTDLG_H__F89F1D29_CDE9_4F00_AC06_3BBD1DA2BAAC__INCLUDED_)
