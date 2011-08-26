// View2.cpp : implementation file
//

#include "stdafx.h"
#include "Sperm.h"
#include "View2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView2

IMPLEMENT_DYNCREATE(CView2, CFormView)

CView2::CView2()
	: CFormView(CView2::IDD)
{
	//{{AFX_DATA_INIT(CView2)
	m_sTotalSpermNum = _T("");    //视野中精子总数
	m_sMovementSpermNum = _T(""); //活动精子数目
	m_sActiveSpermNum = _T("");	 //活跃精子数目

	m_sGradeA = _T("");
	m_sGradeB = _T("");
	m_sGradeC = _T("");
	m_sGradeD = _T("");

	m_sLineMoveSpermNum = _T(""); //直线运动精子数
	m_sCurveMoveSpermNum = _T("");//曲线运动精子数

	m_sActiveSpermDensity = _T(""); // 活跃精子密度
	m_sTotalSpermDensity = _T("");  // 总精子密度
	m_sMovementSpermRatio = _T(""); // 活动精子比率，即存活率
	//}}AFX_DATA_INIT
	
	m_nTotalSpermNum    = 0;    //视野中精子总数
	m_nMovementSpermNum = 0;	//活动精子数目
	m_nActiveSpermNum   = 0;	//活跃精子数目

	m_dGradeA = .0;
	m_dGradeB = .0;
	m_dGradeC = .0;
	m_dGradeD = .0;

	m_nLineMoveSpermNum   = 0; //直线运动精子数
	m_nCurveMoveSpermNum  = 0;//曲线运动精子数

	m_dActiveSpermDensity = .0;  // 活跃精子密度
	m_dTotalSpermDensity  = .0;  // 总精子密度
	m_dMovementSpermRatio = .0;	 // 存活率
}

CView2::~CView2()
{
}

void CView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CView2)
	DDX_Text(pDX, IDC_EDIT1, m_sMovementSpermNum);
	DDX_Text(pDX, IDC_EDIT2, m_sTotalSpermNum);
	DDX_Text(pDX, IDC_EDIT3, m_sTotalSpermDensity);

	DDX_Text(pDX, IDC_EDIT4, m_sMovementSpermRatio);

	DDX_Text(pDX, IDC_EDIT5, m_sGradeA);
	DDX_Text(pDX, IDC_EDIT6, m_sGradeB);
	DDX_Text(pDX, IDC_EDIT7, m_sGradeC);
	DDX_Text(pDX, IDC_EDIT8, m_sGradeD);

	DDX_Text(pDX, IDC_EDIT9, m_sLineMoveSpermNum);
	DDX_Text(pDX, IDC_EDIT10, m_sCurveMoveSpermNum);
	DDX_Text(pDX, IDC_EDIT11, m_sActiveSpermNum);
	DDX_Text(pDX, IDC_EDIT12, m_sActiveSpermDensity);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CView2, CFormView)
	//{{AFX_MSG_MAP(CView2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView2 diagnostics

#ifdef _DEBUG
void CView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView2 message handlers
