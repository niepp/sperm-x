#ifndef  _SPERMOPY_PARAMETER_
#define  _SPERMOPY_PARAMETER_


/*******************************************************
*                                                      *
*  	SperMopyParameter.h                                *
*	author: NPorz                                      *
*	2007/10/24                                         *
*                                                      *
*   精子形态学分析： 精子形态学参数定义                *
*                                                      *
*******************************************************/

#define  nPARAMETER    14  // 形态指标参数个数

typedef struct tagSperMopyParameter{

	BOOL isDeleted;   //  TRUE  该精子被删除, FALSE  没有删除

// 头部和中段
	double	m_length;    
	double	m_width;
	double	m_area;
	double	m_ellipticity;
	double	m_perfor_area;
	double	m_head_area;
	double	m_perimeter;
	double	m_head_perfor_area;

// 尾部
	double	m_tail_length;
	double	m_tail_width;
	double	m_tail_angle;

	double	m_extension;
	double	m_symmetry;
	double	m_ruga;

	
	int m_headAb;    // 头部异常代码
	int m_perforAb;  // 顶体异常代码
	int m_tailAb;    // 尾部异常代码
	int m_smallpointAb;  // 胞浆小滴异常代码

	tagSperMopyParameter(){
		memset(this,0,sizeof(tagSperMopyParameter));

		m_ellipticity = 0;
		m_head_perfor_area = 0;
		m_extension = 0;
		m_symmetry = 0;
		m_ruga = 0;

		m_headAb = -1;
		m_perforAb = -1;
		m_smallpointAb = -1;
		m_tailAb = -1;

		isDeleted = FALSE;
	}
}SperMopyParameter, *PSperMopyParameter;


struct tagSpermMorphaNum{
	int BigHeadSpermNum;    // 大头异常精子数;
	int SmallHeadSpermNum; // 小头异常精子数；
	int BrushHeadSpermNum;  // 刷状头精子数；
	int ConeHeadSpermNum;  // 锥形头精子数
	int PearHeadSpermNum;  // 梨形头精子数
	int CircleHeadSpermNum;   // 圆形头精子数
	int IrregularHeadSpermNum;  // 无规则头精子数
	int AbnormalPerforpermNum;   // 异常顶体精子数
	int SizeAbnormalpermNum;   //  尺寸异常精子数
	int AngleAbnormalpermNum;  //  角度异常精子数
	int InsertAbnormalpermNum;  // 插入异常精子数
	int ShortTailpermNum;   //  短尾精子数
	int IrregularpermNum;  //  不规则精子数
	int CircleTailSpermNum;  // 环状尾精子数
	int MultiTailSpermNum;   // 多尾精子数
	int NoTailSpermNum;   // 无尾精子数
	int WindSpermNum;    //  弯曲精子数
	int NormalSpermNum;     // 正常精子数;
	int AbnormalSpermNum;   // 异常精子数,为头部异常,颈部异常,尾部异常与程序计算结果.
	int HeadAbnormalSpermNum;   //  头部异常精子数
	int NeckAbnormalSpermNum;   //  颈部异常精子数
	int TailAbnormalSpermNum;   //  尾部异常精子数
	int SmallPointAbSpermNum;  //  胞浆小滴异常精子数

	int	normal_lengthNum;    
	int	normal_widthNum;
	int	normal_areaNum;
	int	normal_ellipticityNum;
	int	normal_perfor_areaNum;
	int	normal_head_areaNum;
	int	normal_perimeterNum;
	int	normal_head_perfor_areaNum;

// 尾部
	int	normal_tail_lengthNum;
	int	normal_tail_widthNum;
	int	normal_tail_angleNum;

	int	normal_extensionNum;
	int	normal_symmetryNum;
	int	normal_rugaNum;

	tagSpermMorphaNum()
	{
		memset(this,0,sizeof(tagSpermMorphaNum));
	}
};

typedef struct tagSpermPartialMorphaNum{

	int nTotalNum;  // 计数
	int nTotalNormalNum;  // 正常计数

	int nHeadAbnormalNum;
	int nBodyAbnormalNum;
	int nTailAbnormalNum;

	double dTotalNormalRatio;
	double dHeadAbnormalRatio;
	double dBodyAbnormalRatio;
	double dTailAbnormalRatio;

	double TZI;
	double SDI;

	void Init()
	{
	 nTotalNum = 0;
	 nTotalNormalNum = 0;

	 nHeadAbnormalNum = 0;
	 nBodyAbnormalNum = 0;
	 nTailAbnormalNum = 0;

	 dTotalNormalRatio = 0.0;
	 dHeadAbnormalRatio = 0.0;
	 dBodyAbnormalRatio = 0.0;
	 dTailAbnormalRatio = 0.0;
	 
	 TZI = 0.0;
	 SDI = 0.0;
	}

}SpermPartialMorphaNum, *pSpermPartialMorphaNum;

#endif   //_SPERMOPY_PARAMETER_
