// MorphaParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MorphaParameterDlg.h"
#include "View3.h"
#include "AllFunction.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMorphaParameterDlg dialog

extern CView3* theView3;
extern CString theStrPathApp;

extern int gThreshAdjustPart;
extern int gThreshAdjust;
extern double gGrowStep;

CMorphaParameterDlg::CMorphaParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMorphaParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMorphaParameterDlg)
	m_nMicroscope = 100;
	m_inf_length = _T("");
	m_inf_area = _T("");
	m_inf_ellipticity = _T("");
	m_inf_perfor_area = _T("");
	m_inf_head_area = _T("");
	m_inf_perimeter = _T("");
	m_inf_width = _T("");
	m_inf_tail_length = _T("");
	m_inf_tail_width = _T("");
	m_inf_tail_angle = _T("");
	m_inf_head_perfor_area = _T("");
	m_inf_extension = _T("");
	m_inf_symmetry = _T("");
	m_inf_ruga = _T("");
	m_sup_length = _T("");
	m_sup_area = _T("");
	m_sup_ellipticity = _T("");
	m_sup_perfor_area = _T("");
	m_sup_head_area = _T("");
	m_sup_perimeter = _T("");
	m_sup_width = _T("");
	m_sup_tail_length = _T("");
	m_sup_tail_width = _T("");
	m_sup_tail_angle = _T("");
	m_sup_head_perfor_area = _T("");
	m_sup_extension = _T("");
	m_sup_symmetry = _T("");
	m_sup_ruga = _T("");
	m_strGrowStep = _T("");
	m_strThreshAdjust = _T("");
	m_strThreshAdjustPart = _T("");
	//}}AFX_DATA_INIT

	m_ThreshAdjustPart = 35;
	m_ThreshAdjust = 25;
	m_GrowStep = 0.85;

}


void CMorphaParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMorphaParameterDlg)
	DDX_Control(pDX, IDC_SLIDER_THRESH_ADJUST_PART, m_sliderThreshAdjustPart);
	DDX_Control(pDX, IDC_SLIDER_THRESH_ADJUST, m_sliderThreshAdjust);
	DDX_Control(pDX, IDC_SLIDER_GROW_STEP, m_sliderGrowStep);
	DDX_Text(pDX, IDC_MICROSCOPE, m_nMicroscope);
		DDX_Control(pDX, IDC_COMBOX, m_ComCtrl);
	DDX_Text(pDX, IDC_INF_LENGHT, m_inf_length);
	DDX_Text(pDX, IDC_INF_AREA, m_inf_area);
	DDX_Text(pDX, IDC_INF_ELLIPTICITY, m_inf_ellipticity);
	DDX_Text(pDX, IDC_INF_PERFOR_AREA, m_inf_perfor_area);
	DDX_Text(pDX, IDC_INF_HEAD_AREA, m_inf_head_area);
	DDX_Text(pDX, IDC_INF_PERIMETER, m_inf_perimeter);
	DDX_Text(pDX, IDC_INF_WIDTH, m_inf_width);
	DDX_Text(pDX, IDC_INF_MITOSOMA_LENGHT, m_inf_tail_length);
	DDX_Text(pDX, IDC_INF_MITOSOMA_WIDTH, m_inf_tail_width);
	DDX_Text(pDX, IDC_INF_MITOSOMA_DEVANGLE, m_inf_tail_angle);
	DDX_Text(pDX, IDC_INF_HEAD_PERFOR_AREA, m_inf_head_perfor_area);
	DDX_Text(pDX, IDC_INF_EXTENSION, m_inf_extension);
	DDX_Text(pDX, IDC_INF_SYMMETRY, m_inf_symmetry);
	DDX_Text(pDX, IDC_INF_RUGA, m_inf_ruga);
	DDX_Text(pDX, IDC_SUP_LENGHT, m_sup_length);
	DDX_Text(pDX, IDC_SUP_AREA, m_sup_area);
	DDX_Text(pDX, IDC_SUP_ELLIPTICITY, m_sup_ellipticity);
	DDX_Text(pDX, IDC_SUP_PERFOR_AREA, m_sup_perfor_area);
	DDX_Text(pDX, IDC_SUP_HEAD_AREA, m_sup_head_area);
	DDX_Text(pDX, IDC_SUP_PERIMETER, m_sup_perimeter);
	DDX_Text(pDX, IDC_SUP_WIDTH, m_sup_width);
	DDX_Text(pDX, IDC_SUP_MITOSOMA_LENGHT, m_sup_tail_length);
	DDX_Text(pDX, IDC_SUP_MITOSOMA_WIDTH, m_sup_tail_width);
	DDX_Text(pDX, IDC_SUP_MITOSOMA_DEVANGLE, m_sup_tail_angle);
	DDX_Text(pDX, IDC_SUP_HEAD_PERFOR_AREA, m_sup_head_perfor_area);
	DDX_Text(pDX, IDC_SUP_EXTENSION, m_sup_extension);
	DDX_Text(pDX, IDC_SUP_SYMMETRY, m_sup_symmetry);
	DDX_Text(pDX, IDC_SUP_RUGA, m_sup_ruga);
	DDX_Text(pDX, IDC_STATIC_GROW_STEP, m_strGrowStep);
	DDX_Text(pDX, IDC_STATIC_THRESH_ADJUST, m_strThreshAdjust);
	DDX_Text(pDX, IDC_STATIC_THRESH_ADJUST_PART, m_strThreshAdjustPart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMorphaParameterDlg, CDialog)
	//{{AFX_MSG_MAP(CMorphaParameterDlg)
	ON_CBN_SELCHANGE(IDC_COMBOX, OnSelchangeCombox)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorphaParameterDlg message handlers

void CMorphaParameterDlg::OnSelchangeCombox() 
{
	CString fileName;
	int nCursel = m_ComCtrl.GetCurSel();
	switch(nCursel) {
	case 0:	
		fileName = "WHO_Parameter_Data.txt";
		break;
	case 1:
		fileName = "UserDefined_Parameter_Data.txt";
		break;
	}
    ReadAndSetParameterData(fileName);	
}

void CMorphaParameterDlg::ReadAndSetParameterData(CString fileName)
{
    CString sPath( theStrPathApp );
	sPath+= "\\data\\";
	CreateDirectory((LPCTSTR)sPath, NULL);

	CString  filePathAndName; 
	filePathAndName = sPath+fileName;

	fstream  fin;
	fin.open(filePathAndName, ios::in);

	 double _inf_length;
	 double _inf_width;
	 double _inf_area;

	 double _inf_ellipticity;
	 double _inf_perfor_area;
	 double _inf_head_area;
	 double _inf_perimeter;
	 
	 double _inf_tail_length;
	 double _inf_tail_width;
	 double _inf_tail_angle;

	 double _inf_head_perfor_area;	
	 double _inf_extension;
	 double _inf_symmetry;
	 double _inf_ruga;
			
	 double _sup_length;
	 double _sup_width;
	 double _sup_area;

	 double _sup_ellipticity;
	 double _sup_perfor_area;
	 double _sup_head_area;
	 double _sup_perimeter;
	 
	 double _sup_tail_length;
	 double _sup_tail_width;
	 double _sup_tail_angle;

	 double _sup_head_perfor_area;	
	 double _sup_extension;
	 double _sup_symmetry;
	 double _sup_ruga;
	 int   nMicroscope;

      fin>>_inf_length
		 >>_inf_width
	     >>_inf_area

		 >>_inf_ellipticity
		 >>_inf_perfor_area
		 >>_inf_head_area
		 >>_inf_perimeter
		 
		 >>_inf_tail_length
		 >>_inf_tail_width
		 >>_inf_tail_angle

		 >>_inf_head_perfor_area	
		 >>_inf_extension
		 >>_inf_symmetry
		 >>_inf_ruga
		 
		 >>_sup_length
		 >>_sup_width
		 >>_sup_area

		 >>_sup_ellipticity
		 >>_sup_perfor_area
		 >>_sup_head_area
		 >>_sup_perimeter
		 
		 >>_sup_tail_length
		 >>_sup_tail_width
		 >>_sup_tail_angle

		 >>_sup_head_perfor_area	
		 >>_sup_extension
		 >>_sup_symmetry
		 >>_sup_ruga
		 >>nMicroscope
		 >>m_ThreshAdjustPart
		 >>m_ThreshAdjust
		 >>m_GrowStep;

	fin.close();

	CString cs;
	cs.Format("%.2f", _inf_length);
	m_inf_length = cs;

	cs.Format("%.2f", _inf_width);
	m_inf_width       = cs;

	cs.Format("%.2f", _inf_area);
	m_inf_area   = cs;

	cs.Format("%.2f", _inf_perimeter);
	m_inf_perimeter   = cs;
	
	cs.Format("%.2lf", _inf_ellipticity);
	m_inf_ellipticity = cs;

	cs.Format("%.2f", _inf_perfor_area);
	m_inf_perfor_area = cs;

	cs.Format("%.2f", _inf_head_area);
	m_inf_head_area   = cs;

	cs.Format("%.2f", _inf_head_perfor_area);
	m_inf_head_perfor_area  = cs;

	cs.Format("%.2lf", _inf_tail_length);
	m_inf_tail_length = cs;

	cs.Format("%.2lf", _inf_tail_width);
	m_inf_tail_width  = cs;

	cs.Format("%d", (int)_inf_tail_angle);
	m_inf_tail_angle  = cs;
		
	cs.Format("%.2f", _inf_ruga);
	m_inf_ruga  = cs;
		
	cs.Format("%.2f", _inf_extension);
	m_inf_extension  = cs;
		
	cs.Format("%.2f", _inf_symmetry);
	m_inf_symmetry   = cs;

	cs.Format("%.2f", _sup_length);
	m_sup_length = cs;

	cs.Format("%.2f", _sup_width);
	m_sup_width       = cs;

	cs.Format("%.2f", _sup_area);
	m_sup_area   = cs;

	cs.Format("%.2f", _sup_perimeter);
	m_sup_perimeter   = cs;
	
	cs.Format("%.2lf", _sup_ellipticity);
	m_sup_ellipticity = cs;

	cs.Format("%.2f", _sup_perfor_area);
	m_sup_perfor_area = cs;

	cs.Format("%.2f", _sup_head_area);
	m_sup_head_area   = cs;

	cs.Format("%.2f", _sup_head_perfor_area);
	m_sup_head_perfor_area  = cs;

	cs.Format("%.2f", _sup_tail_length);
	m_sup_tail_length = cs;

	cs.Format("%.2f", _sup_tail_width);
	m_sup_tail_width  = cs;

	cs.Format("%.2f", _sup_tail_angle);
	m_sup_tail_angle  = cs;
		
	cs.Format("%.2f", _sup_ruga);
	m_sup_ruga  = cs;
		
	cs.Format("%.2f", _sup_extension);
	m_sup_extension  = cs;
		
	cs.Format("%.2f", _sup_symmetry);
	m_sup_symmetry   = cs;

	m_nMicroscope = nMicroscope;

	m_sliderThreshAdjustPart.SetPos(m_ThreshAdjustPart);
	m_strThreshAdjustPart.Format("%d", m_ThreshAdjustPart);
	m_sliderThreshAdjust.SetPos(m_ThreshAdjust);
	m_strThreshAdjust.Format("%d", m_ThreshAdjust);
	m_sliderGrowStep.SetPos(m_GrowStep*100);
	m_strGrowStep.Format("%d", int(m_GrowStep*100));

	UpdateWindow();
    UpdateData(FALSE);

}


BOOL CMorphaParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ReadAndSetParameterData("UserDefined_Parameter_Data.txt");
	m_ComCtrl.SetCurSel(1);

	m_sliderThreshAdjustPart.SetRange(-50,50);
	m_sliderThreshAdjustPart.SetPos(m_ThreshAdjustPart);
	m_strThreshAdjustPart.Format("%d", m_ThreshAdjustPart);

	m_sliderThreshAdjust.SetRange(-100,0);
	m_sliderThreshAdjust.SetPos(m_ThreshAdjust);
	m_strThreshAdjust.Format("%d", m_ThreshAdjust);

	m_sliderGrowStep.SetRange(0,100);
	m_sliderGrowStep.SetPos(m_GrowStep*100);
	m_strGrowStep.Format("%d", int(m_GrowStep*100));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMorphaParameterDlg::OnOK() 
{
	// TODO: Add extra validation here
	 double _inf_length;
	 double _inf_area;
	 double _inf_ellipticity;
	 double _inf_perfor_area;
	 double _inf_head_area;
	 double _inf_perimeter;
	 double _inf_width;
	 double _inf_tail_length;
	 double _inf_tail_width;
	 double _inf_tail_angle;
	 double _inf_head_perfor_area;	
	 double _inf_extension;
	 double _inf_symmetry;
	 double _inf_ruga;
			
	 double _sup_length;
	 double _sup_area;
	 double _sup_ellipticity;
	 double _sup_perfor_area;
	 double _sup_head_area;
	 double _sup_perimeter;
	 double _sup_width;
	 double _sup_tail_length;
	 double _sup_tail_width;
	 double _sup_tail_angle;
	 double _sup_head_perfor_area;	
	 double _sup_extension;
	 double _sup_symmetry;
	 double _sup_ruga;
	 int    nMicroscope;

     UpdateData(TRUE);
     _inf_length = atof((LPCTSTR)m_inf_length); 
	 _inf_width = atof((LPCTSTR)m_inf_width);
	 _inf_area   = atof((LPCTSTR)m_inf_area);

	 _inf_ellipticity    = atof((LPCTSTR)m_inf_ellipticity);
	 _inf_perfor_area    = atof((LPCTSTR)m_inf_perfor_area);
	 _inf_head_area      = atof((LPCTSTR)m_inf_head_area);
	 _inf_perimeter      = atof((LPCTSTR)m_inf_perimeter);

	 _inf_tail_length       = atof((LPCTSTR)m_inf_tail_length);
	 _inf_tail_width        = atof((LPCTSTR)m_inf_tail_width);
	 _inf_tail_angle        = atof((LPCTSTR)m_inf_tail_angle);

	 _inf_head_perfor_area  = atof((LPCTSTR)m_inf_head_perfor_area);	 
	 _inf_extension      = atof((LPCTSTR)m_inf_extension);
	 _inf_symmetry       = atof((LPCTSTR)m_inf_symmetry);
	 _inf_ruga           = atof((LPCTSTR)m_inf_ruga);
			
     _sup_length = atof((LPCTSTR)m_sup_length); 
	 _sup_width = atof((LPCTSTR)m_sup_width);
	 _sup_area   = atof((LPCTSTR)m_sup_area);

	 _sup_ellipticity    = atof((LPCTSTR)m_sup_ellipticity);
	 _sup_perfor_area    = atof((LPCTSTR)m_sup_perfor_area);
	 _sup_head_area      = atof((LPCTSTR)m_sup_head_area);
	 _sup_perimeter      = atof((LPCTSTR)m_sup_perimeter);

	 _sup_tail_length       = atof((LPCTSTR)m_sup_tail_length);
	 _sup_tail_width        = atof((LPCTSTR)m_sup_tail_width);
	 _sup_tail_angle        = atof((LPCTSTR)m_sup_tail_angle);

	 _sup_head_perfor_area  = atof((LPCTSTR)m_sup_head_perfor_area);
	 _sup_extension      = atof((LPCTSTR)m_sup_extension);
	 _sup_symmetry       = atof((LPCTSTR)m_sup_symmetry);
	 _sup_ruga           = atof((LPCTSTR)m_sup_ruga);
	 nMicroscope         = GetDlgItemInt(IDC_MICROSCOPE);

	
	CString fileName;
	int nCursel = m_ComCtrl.GetCurSel();
	switch(nCursel) {
	case 0:	
		fileName = "WHO_Parameter_Data.txt";
		break;
	case 1:
		fileName = "UserDefined_Parameter_Data.txt";
		break;
	}

    CString sPath(theStrPathApp);
	sPath+= "\\data\\";
	CreateDirectory((LPCTSTR)sPath, NULL);

	CString  filePathAndName; 
	filePathAndName = sPath+fileName;

	fstream  fOut;
	fOut.open(filePathAndName, ios::out|ios::trunc);

     fOut<<_inf_length
		 <<"\t"<<_inf_width
	     <<"\t"<<_inf_area

		 <<"\t"<<_inf_ellipticity
		 <<"\t"<<_inf_perfor_area
		 <<"\t"<<_inf_head_area
		 <<"\t"<<_inf_perimeter
		 
		 <<"\t"<<_inf_tail_length
		 <<"\t"<<_inf_tail_width
		 <<"\t"<<_inf_tail_angle

		 <<"\t"<<_inf_head_perfor_area
		 <<"\t"<<_inf_extension
		 <<"\t"<<_inf_symmetry
		 <<"\t"<<_inf_ruga	
		 
		 <<"\n\n"<<_sup_length
		 <<"\t"<<_sup_width
		 <<"\t"<<_sup_area

		 <<"\t"<<_sup_ellipticity
		 <<"\t"<<_sup_perfor_area
		 <<"\t"<<_sup_head_area
		 <<"\t"<<_sup_perimeter
		 
		 <<"\t"<<_sup_tail_length
		 <<"\t"<<_sup_tail_width
		 <<"\t"<<_sup_tail_angle

		 <<"\t"<<_sup_head_perfor_area
		 <<"\t"<<_sup_extension
		 <<"\t"<<_sup_symmetry
		 <<"\t"<<_sup_ruga
		 <<"\n\n"<<nMicroscope
		 <<"\n\n"<<m_ThreshAdjustPart
		 <<"\n\n"<<m_ThreshAdjust
		 <<"\n\n"<<m_GrowStep;

	fOut.close();
	theView3->m_pwndMorphaDetDlg->InitializeStd(filePathAndName);
	theView3->m_pwndMorphaDetDlg->m_nMicroScope = m_nMicroscope;
	CDialog::OnOK();
}

void CMorphaParameterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_THRESH_ADJUST_PART:
		m_ThreshAdjustPart = ((CSliderCtrl*)pScrollBar)->GetPos();
		m_strThreshAdjustPart.Format("%d", m_ThreshAdjustPart);
		gThreshAdjustPart = m_ThreshAdjustPart;
		UpdateData(FALSE);
		break;

	case IDC_SLIDER_THRESH_ADJUST:
		m_ThreshAdjust = ((CSliderCtrl*)pScrollBar)->GetPos();
		m_strThreshAdjust.Format("%d", m_ThreshAdjust);
		gThreshAdjust = m_ThreshAdjust;
		UpdateData(FALSE);
		break;

	case IDC_SLIDER_GROW_STEP:
		int tmp = (((CSliderCtrl*)pScrollBar)->GetPos());
		m_strGrowStep.Format("%d", tmp);
		m_GrowStep = tmp / double(100);
		gGrowStep = m_GrowStep;
		UpdateData(FALSE);
		break;
	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
