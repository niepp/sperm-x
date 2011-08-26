/*******************************************************
*                                                      *
*  	SpermParamCalc.h                                   *
*	author: happynp                                    *
*	Email: happynp@gmail.com                           *
*   2009/6/23                                          *
*   ������̬ѧ������ ����������̬ѧ��������            *
*                                                      *
*******************************************************/ 

#if !defined(_SPERM_PARAM_CALC_H)
#define _SPERM_PARAM_CALC_H

// SpermParamCalc.h : header file

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

class  SpermParamCalc
{
	// Construction
public:
	
	// standard constructor
	SpermParamCalc(LPBITMAPINFOHEADER  lpBmInfoSrc, LPBYTE lpBmDataSrc); 
	
	virtual ~SpermParamCalc();
	
	void OnSperMopyParamCalc();
	
	SperMopyParameter  m_SperMopyParameter;  // ��̬����
	
private:
	
	// ������ͼ
	LPBITMAPINFOHEADER m_lpDibIHPro;
	LPBYTE             m_lpDibDataPro;
	
private:
	
	// $��������   
	
    vector<CPoint>m_HeadEdgePoint;      // ͷ���Ӷ���������߽��
    vector<CPoint>m_TailEdgePoint;      // β�������߽��
	vector<CPoint>m_HeadTailBoundary;   // ͷ����β��������
	CPoint m_ProlateTip[2];         // ����˵�
	CPoint m_MinorAxisTip[2];       // ����˵�
    CPoint m_TailTip;               // β�Ͷ˵�
	
private:
	
	// $��������
	
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
