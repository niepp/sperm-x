#ifndef _IGVFSNAKE_H
#define _IGVFSNAKE_H

#include "SnakeType.h"

#define HEADRG 100
#define PerforRG 200

class IGvfSnake
{
protected:
    double **m_ppf;  // 边缘图
	double m_u;         // GVF场参数( 注意要满足：m_u<=1/4 )
	skVector **m_ppGVF; // GVF场
    void GvfSolver(int);  // 计算GVF场
	double CalculateAverageDist(const vector<skPoint>&vSnake);  // 计算Snake控制点间平均间距

    void GetEdgeImage(); // 计算边缘图
    skVector CalculateTensileForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex);   // 计算编号为NodeIndex的Snake控制点m_vSnake[NodeIndex]的内力拉伸项, sNode是m_vSnake[NodeIndex]的搜索邻域里的点
    skVector CalculateFlexuralForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex);  // 计算编号为NodeIndex的Snake控制点m_vSnake[NodeIndex]的内力弯曲项, sNode是m_vSnake[NodeIndex]的搜索邻域里的点
	skVector CalculateExternalForce(const skPoint&skNode);  // 计算skNode的外力项
	skPoint  Search(const vector<skPoint>&vSnake, int NodeIndex);   // 贪心搜索编号为NodeIndex的Snake控制点的下一步位置。

	vector<skPoint> m_vSnake;    // 轮廓线控制点数组

    /* Snake形变参数---各项权重系数 */
	double m_alpha;      // alpha  elasticity parameter
	double m_beta;       // beta   rigidity parameter
    double m_gama;       // gama  external force weight
 
	/* 插值参数 */
    int  m_nMaxDist;     // 大于我，就在相邻控制点间插值
	int  m_nMinDist;     // 小于我，就合并相邻控制点

	int  m_nMaxIterT;    // 最大迭代次数
	double m_dp;         // 稳定控制点的比例大于这个值就迭代停止

	long m_height;       // 图像区域高
	long m_width;        // 图像区域宽
	
	IplImage* m_ppImageData; // 8位灰度图像数据

	int m_nIterT;  // 当前已迭代次数
    void Interpolate();  // Snake控制点插值
	bool IsInner(int row, int col);
	bool IsInner(const skPoint& sNode);

public:
	IplImage* GetppImageData();

public:
	IGvfSnake(   	
		    long height,       // 图像区域高
			long width,        // 图像区域宽
	        IplImage* ppImageData, // 8位灰度图像数据 
	
		    /* Snake形变参数---各项权重系数 */
			double alpha,      // alpha  elasticity parameter
			double beta,       // beta   rigidity parameter
			double gama,       // gama  external force weight

			double  m_u,       // GVF场参数( 注意要满足：m_u<=1/4 )
			int  nMaxDist,     // 大于我，就在相邻控制点间插值
			int  nMinDist,     // 小于我，就合并相邻控制点
			int  nMaxIterT,    // 最大迭代次数
			double dp          // 稳定控制点的比例大于这个值就迭代停止
		   );

	void SnakeInitialize();   // Snake初始化: 设定初始轮廓，计算图像能量
	void SnakeInitialize(const vector<skPoint>&);  
    bool SnakeDeform(int nIterT);  // Snake变形迭代nIterT次
	void ShowSnakeDeform(IplImage* srcImage);

	virtual~IGvfSnake();

};

inline bool IGvfSnake::IsInner(int row, int col)
{
	if(   row < m_height && row >=0
	   && col < m_width  && col >=0 )
       return true;
	return false;	    
}

inline bool IGvfSnake::IsInner(const skPoint &sNode)
{
	if(   sNode.dy < m_height && sNode.dy >=0
	   && sNode.dx < m_width  && sNode.dx >=0 )
       return true;
	return false;	    
}

inline CvPoint skPoint2CvPoint(const skPoint&p) {
	return cvPoint(p.dx, p.dy);
}

inline skPoint CvPoint2skPoint(const CvPoint&p) {
	return skPoint(p.x, p.y);
}

#endif //_IGVFSNAKE_H