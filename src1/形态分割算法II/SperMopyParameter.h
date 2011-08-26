#ifndef  _SPERMOPY_PARAMETER_
#define  _SPERMOPY_PARAMETER_


/*******************************************************
*                                                      *
*  	SperMopyParameter.h                                *
*	author: NPorz                                      *
*	2007/10/24                                         *
*                                                      *
*   ������̬ѧ������ ������̬ѧ��������                *
*                                                      *
*******************************************************/

#define  nPARAMETER    14  // ��ָ̬���������

typedef struct tagSperMopyParameter{

	BOOL isDeleted;   //  TRUE  �þ��ӱ�ɾ��, FALSE  û��ɾ��

// ͷ�����ж�
	double	m_length;    
	double	m_width;
	double	m_area;
	double	m_ellipticity;
	double	m_perfor_area;
	double	m_head_area;
	double	m_perimeter;
	double	m_head_perfor_area;

// β��
	double	m_tail_length;
	double	m_tail_width;
	double	m_tail_angle;

	double	m_extension;
	double	m_symmetry;
	double	m_ruga;

	
	int m_headAb;    // ͷ���쳣����
	int m_perforAb;  // �����쳣����
	int m_tailAb;    // β���쳣����
	int m_smallpointAb;  // ����С���쳣����

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
	int BigHeadSpermNum;    // ��ͷ�쳣������;
	int SmallHeadSpermNum; // Сͷ�쳣��������
	int BrushHeadSpermNum;  // ˢ״ͷ��������
	int ConeHeadSpermNum;  // ׶��ͷ������
	int PearHeadSpermNum;  // ����ͷ������
	int CircleHeadSpermNum;   // Բ��ͷ������
	int IrregularHeadSpermNum;  // �޹���ͷ������
	int AbnormalPerforpermNum;   // �쳣���徫����
	int SizeAbnormalpermNum;   //  �ߴ��쳣������
	int AngleAbnormalpermNum;  //  �Ƕ��쳣������
	int InsertAbnormalpermNum;  // �����쳣������
	int ShortTailpermNum;   //  ��β������
	int IrregularpermNum;  //  ����������
	int CircleTailSpermNum;  // ��״β������
	int MultiTailSpermNum;   // ��β������
	int NoTailSpermNum;   // ��β������
	int WindSpermNum;    //  ����������
	int NormalSpermNum;     // ����������;
	int AbnormalSpermNum;   // �쳣������,Ϊͷ���쳣,�����쳣,β���쳣����������.
	int HeadAbnormalSpermNum;   //  ͷ���쳣������
	int NeckAbnormalSpermNum;   //  �����쳣������
	int TailAbnormalSpermNum;   //  β���쳣������
	int SmallPointAbSpermNum;  //  ����С���쳣������

	int	normal_lengthNum;    
	int	normal_widthNum;
	int	normal_areaNum;
	int	normal_ellipticityNum;
	int	normal_perfor_areaNum;
	int	normal_head_areaNum;
	int	normal_perimeterNum;
	int	normal_head_perfor_areaNum;

// β��
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

	int nTotalNum;  // ����
	int nTotalNormalNum;  // ��������

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
