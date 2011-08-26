/*******************************************************
*                                                      *
*  	SingleSpermRecognition.h                           *
*	author: NPhard                                     *
*	Email: happynp@gmail.com                           *
*   2007/10/24                                         *
*   精子形态学分析： 单个精子形态学分割计算            *
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
  	    
	 SperMopyParameter  m_SperMopyParameter;  // 形态参数

	// 当前处理图像
	  LPBITMAPINFOHEADER m_lpDibIHPro;
      LPBYTE             m_lpDibDataPro;

private:
	
	// 保存原始位图灰度化结果
	LPBITMAPINFOHEADER m_smSpermBIHPro;
	LPBYTE             m_smSpermBDPro;
     
	vector<POINT>m_vRegionPoints; // 目标区域

	CPoint     m_CenterPos;   // 矩形区域中心
	BOOL       **m_visit;     // 访问标记数组

private:

// $1-----形态分割

    // 保存整体分割后二值化图像
	LPBITMAPINFOHEADER m_lpThrSegSavedIHPro;  
	LPBYTE             m_lpThrSegSavedDataPro; 
   
    int   m_iThreshold;   // 整体分割阈值
    int   m_iThresholdFH; // 顶体和头部分割阈值

// $2-----参数计算   

    vector<CPoint>m_HeadEdgePoint;      // 头部加顶体的轮廓边界点
    vector<CPoint>m_TailEdgePoint;      // 尾部轮廓边界点
	vector<CPoint>m_HeadTailBoundary;   // 头部和尾部交界线
	CPoint m_ProlateTip[2];         // 长轴端点
	CPoint m_MinorAxisTip[2];       // 短轴端点
    CPoint m_TailTip;               // 尾巴端点

private:

// $1-----形态分割

	 void OnGray();      // 灰度化
     void MediaFilter(); // 中值滤波  
	 void MeanFilter();  // 均值滤波(高斯平滑)
	 void SaveGrayDib(); // 保存灰度化图像

//   void OnOtsu();        // 目标整体分割
	 void OnSegment();
	 void SaveSegDib();    // 保存整体分割后二值化图像

	 void OnErosion();
	 void OnExpand();
	 
	 void OnTagOtsu();     // 顶体和头部分割
     void OnTagThreshold();

	 void OnExpandHead();  
	 void OnErosionHead();

	 void OnMarkTail();    // 标记尾部

     void CopySperm(POINT p, LPBYTE lpData);  // 拷贝目标，去掉杂质
	 void OnCopySperm();

	 void OnExpandTail();
 
	 int  GetRegionArea(POINT p);
	 void SearchHole(vector<POINT>&);
	 void OnFillHole(vector<POINT>&);

	 void CopyHead(POINT p, LPBYTE& lpTemp);  // 拷贝头部  
	 POINT GetSeed(CPoint p);
	 void CopyTail(POINT p, LPBYTE& lpTemp);  // 拷贝尾部
     void HandleTailCross();       // 处理尾部交叉情况

	 void FilterConglutinate();    // 过滤粘连物
	 void SpermThining();          // 单层细化

// $2-----参数计算

     void OnGetHeadEdge();   // 轮廓提取

	 double GetLength();
	 double GetWidth();
	 double GetArea();
	 double GetPerforArea();
	 double	GetHeadArea();
	 double GetPerimeter();
	 
     double GetTailLength();
	 double GetTailWidth();
	 double GetTailAngle();

	 void OnMarkEllipse();   // 标记椭圆轮廓
	 void OnMarkProlate();   // 标记长轴
	 void OnMarkMinorAxis(); // 标记短轴 
     void OnMarkTailAxis();  // 标记尾轴

	 double distance(CPoint p, CPoint q); 

	 BOOL IsAllPiexlsInLineP_QAreGreen(POINT p, POINT q);
};

#endif // !defined(_CSINGLE_SPERM_RECOGNTION_H)
