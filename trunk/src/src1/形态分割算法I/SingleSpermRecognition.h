/*******************************************************
*                                                      *
*  	SingleSpermRecognition.h                           *
*	author: NPhard                                     *
*	Email: happynp@gmail.com                           *
*   2007/10/24                                         *
*   ������̬ѧ������ ����������̬ѧ�ָ����            *
*                                                      *
*******************************************************/ 

#if !defined(_CSINGLE_SPERM_RECOGNTION_H)
#define _CSINGLE_SPERM_RECOGNTION_H

// SingleSpermRecognition.h : header file

#include "SperMopyParameter.h"
#include "AllDataStructure.h"

#ifndef  WIDTHBYTES
#define  WIDTHBYTES(bits)  (((bits) + 31) / 32 * 4)
#endif
#define  PIEXL(lpSrc)         RGB(*(lpSrc+2), *(lpSrc+1), *(lpSrc+0))
#define  RED_PIEXL            RGB(255,0,0)
#define  GREEN_PIEXL          RGB(0,255,0)
#define  BLUE_PIEXL           RGB(0,0,255)
#define  YELLOW_PIXEL         RGB(0,255,255)
#define  BLACK_PIEXL          RGB(0,0,0)
#define  WHITE_PIEXL          RGB(255,255,255)

#define  GET_RED(rgb)        ((BYTE)(rgb&0xFF))
#define  GET_GREEN(rgb)      ((BYTE)((rgb&0xFF00)>>8))
#define  GET_BLUE(rgb)       ((BYTE)((rgb&0xFF0000)>>16))

#define  PI                   (2.0*acos(0))

typedef struct tagHole{
	POINT holeSeed;
	int   area;
}Hole;

class  CSingleSpermRecognition
{
// Construction
public:
 
	// standard constructor
	 CSingleSpermRecognition(LPBITMAPINFOHEADER  lpBmInfoSrc, LPBYTE lpBmDataSrc, vector<POINT>& RegionPoints, int r); 

     virtual ~CSingleSpermRecognition();

	 void OnSperMopySeg();
	 void OnSperMopyParamCalc();
  	    
	 SperMopyParameter  m_SperMopyParameter;  // ��̬����

	// ��ǰ����ͼ��
	  LPBITMAPINFOHEADER m_lpDibIHPro;
      LPBYTE             m_lpDibDataPro;

private:
	
	// ����ԭʼλͼ�ҶȻ����
	LPBITMAPINFOHEADER m_smSpermBIHPro;
	LPBYTE             m_smSpermBDPro;
     
	vector<POINT>m_vRegionPoints; // Ŀ������

	CPoint     m_CenterPos;   // ������������
	BOOL       **m_visit;     // ���ʱ������

private:

// $1-----��̬�ָ�

    // ��������ָ���ֵ��ͼ��
	LPBITMAPINFOHEADER m_lpThrSegSavedIHPro;  
	LPBYTE             m_lpThrSegSavedDataPro; 
   
    int   m_iThreshold;   // ����ָ���ֵ
    int   m_iThresholdFH; // �����ͷ���ָ���ֵ

// $2-----��������   

    vector<CPoint>m_HeadEdgePoint;      // ͷ���Ӷ���������߽��
    vector<CPoint>m_TailEdgePoint;      // β�������߽��
	vector<CPoint>m_HeadTailBoundary;   // ͷ����β��������
	CPoint m_ProlateTip[2];         // ����˵�
	CPoint m_MinorAxisTip[2];       // ����˵�
    CPoint m_TailTip;               // β�Ͷ˵�

private:

// $1-----��̬�ָ�

	 void OnGray();      // �ҶȻ�
     void MediaFilter(); // ��ֵ�˲�  
	 void MeanFilter();  // ��ֵ�˲�(��˹ƽ��)
	 void SaveGrayDib(); // ����ҶȻ�ͼ��

//   void OnOtsu();        // Ŀ������ָ�
	 void OnSegment();
	 void SaveSegDib();    // ��������ָ���ֵ��ͼ��

	 void OnErosion();
	 void OnExpand();
	 
	 void OnTagOtsu();     // �����ͷ���ָ�
     void OnTagThreshold();

	 void OnExpandHead();  
	 void OnErosionHead();

	 void OnMarkTail();    // ���β��

     void CopySperm(POINT p, LPBYTE lpData);  // ����Ŀ�꣬ȥ������
	 void OnCopySperm();

	 void OnExpandTail();
 
	 int  GetRegionArea(POINT p);
	 void SearchHole(vector<POINT>&);
	 void OnFillHole(vector<POINT>&);

	 void CopyHead(POINT p, LPBYTE& lpTemp);  // ����ͷ��  
	 POINT GetSeed(CPoint p);
	 void CopyTail(POINT p, LPBYTE& lpTemp);  // ����β��
     void HandleTailCross();       // ����β���������

	 void FilterConglutinate();    // ����ճ����
	 void SpermThining();          // ����ϸ��

// $2-----��������

     void OnGetHeadEdge();   // ������ȡ

	 double GetLength();
	 double GetWidth();
	 double GetArea();
	 double GetPerforArea();
	 double	GetHeadArea();
	 double GetPerimeter();
	 
     double GetTailLength();
	 double GetTailWidth();
	 double GetTailAngle();

	 void OnMarkEllipse();   // �����Բ����
	 void OnMarkProlate();   // ��ǳ���
	 void OnMarkMinorAxis(); // ��Ƕ��� 
     void OnMarkTailAxis();  // ���β��

	 double distance(CPoint p, CPoint q); 

	 BOOL IsAllPiexlsInLineP_QAreGreen(POINT p, POINT q);
};

#endif // !defined(_CSINGLE_SPERM_RECOGNTION_H)
