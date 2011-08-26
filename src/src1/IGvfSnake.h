#ifndef _IGVFSNAKE_H
#define _IGVFSNAKE_H

#include "SnakeType.h"

#define HEADRG 100
#define PerforRG 200

class IGvfSnake
{
protected:
    double **m_ppf;  // ��Եͼ
	double m_u;         // GVF������( ע��Ҫ���㣺m_u<=1/4 )
	skVector **m_ppGVF; // GVF��
    void GvfSolver(int);  // ����GVF��
	double CalculateAverageDist(const vector<skPoint>&vSnake);  // ����Snake���Ƶ��ƽ�����

    void GetEdgeImage(); // �����Եͼ
    skVector CalculateTensileForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex);   // ������ΪNodeIndex��Snake���Ƶ�m_vSnake[NodeIndex]������������, sNode��m_vSnake[NodeIndex]������������ĵ�
    skVector CalculateFlexuralForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex);  // ������ΪNodeIndex��Snake���Ƶ�m_vSnake[NodeIndex]������������, sNode��m_vSnake[NodeIndex]������������ĵ�
	skVector CalculateExternalForce(const skPoint&skNode);  // ����skNode��������
	skPoint  Search(const vector<skPoint>&vSnake, int NodeIndex);   // ̰���������ΪNodeIndex��Snake���Ƶ����һ��λ�á�

	vector<skPoint> m_vSnake;    // �����߿��Ƶ�����

    /* Snake�α����---����Ȩ��ϵ�� */
	double m_alpha;      // alpha  elasticity parameter
	double m_beta;       // beta   rigidity parameter
    double m_gama;       // gama  external force weight
 
	/* ��ֵ���� */
    int  m_nMaxDist;     // �����ң��������ڿ��Ƶ���ֵ
	int  m_nMinDist;     // С���ң��ͺϲ����ڿ��Ƶ�

	int  m_nMaxIterT;    // ����������
	double m_dp;         // �ȶ����Ƶ�ı����������ֵ�͵���ֹͣ

	long m_height;       // ͼ�������
	long m_width;        // ͼ�������
	
	IplImage* m_ppImageData; // 8λ�Ҷ�ͼ������

	int m_nIterT;  // ��ǰ�ѵ�������
    void Interpolate();  // Snake���Ƶ��ֵ
	bool IsInner(int row, int col);
	bool IsInner(const skPoint& sNode);

public:
	IplImage* GetppImageData();

public:
	IGvfSnake(   	
		    long height,       // ͼ�������
			long width,        // ͼ�������
	        IplImage* ppImageData, // 8λ�Ҷ�ͼ������ 
	
		    /* Snake�α����---����Ȩ��ϵ�� */
			double alpha,      // alpha  elasticity parameter
			double beta,       // beta   rigidity parameter
			double gama,       // gama  external force weight

			double  m_u,       // GVF������( ע��Ҫ���㣺m_u<=1/4 )
			int  nMaxDist,     // �����ң��������ڿ��Ƶ���ֵ
			int  nMinDist,     // С���ң��ͺϲ����ڿ��Ƶ�
			int  nMaxIterT,    // ����������
			double dp          // �ȶ����Ƶ�ı����������ֵ�͵���ֹͣ
		   );

	void SnakeInitialize();   // Snake��ʼ��: �趨��ʼ����������ͼ������
	void SnakeInitialize(const vector<skPoint>&);  
    bool SnakeDeform(int nIterT);  // Snake���ε���nIterT��
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