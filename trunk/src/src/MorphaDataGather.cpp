// MorphaDataGather.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "MorphaDataGather.h"
#include "ImageConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMorphaDataGather dialog
extern _ConnectionPtr theConnection;

CMorphaDataGather::CMorphaDataGather(CWnd* pParent /*=NULL*/)
	: CDialog(CMorphaDataGather::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMorphaDataGather)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CMorphaDataGather::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMorphaDataGather)
	DDX_Control(pDX, IDC_LIST_MORPHADATA, m_List);

	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
	DDX_Text(pDX, IDC_STATIC_AREA, m_area);
	DDX_Text(pDX, IDC_STATIC_WIDTH, m_width);
	DDX_Text(pDX, IDC_STATIC_PERIMETER, m_perimeter);
	DDX_Text(pDX, IDC_STATIC_ELLIPTICITY, m_ellipticity);
	DDX_Text(pDX, IDC_STATIC_EXTENSION, m_extension);
	DDX_Text(pDX, IDC_STATIC_HEAD_AREA, m_head_area);
	DDX_Text(pDX, IDC_STATIC_HEAD_PERFOR_AREA, m_head_perfor_area);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_DEVANGLE, m_tail_angle);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_LENGHT, m_tail_length);
	DDX_Text(pDX, IDC_STATIC_MITOSOMA_WIDTH, m_tail_width);
	DDX_Text(pDX, IDC_STATIC_PERFOR_AREA, m_perfor_area);
	DDX_Text(pDX, IDC_STATIC_SYMMETRY, m_symmetry);
	DDX_Text(pDX, IDC_STATIC_RUGA, m_ruga);
	DDX_Text(pDX, IDC_STATIC_LENGTH, m_length);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMorphaDataGather, CDialog)
	//{{AFX_MSG_MAP(CMorphaDataGather)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MORPHADATA, OnClickListMorphadata)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorphaDataGather message handlers

BOOL CMorphaDataGather::OnInitDialog() 
{
	m_lpData = NULL;
	m_lpInfo = NULL;

	CDialog::OnInitDialog();
	try
	{
		CString strGetN("select count(*) from MorphaSpermData");
		_RecordsetPtr rs=theConnection->Execute((LPCSTR)strGetN,NULL,adCmdText);
		int nTotalRecord = rs->GetCollect((long)0).iVal;
		CString queryinfo;
 		queryinfo.Format("共查询到%d条记录", nTotalRecord);
 		SetDlgItemText(IDC_STATIC_QUERY, queryinfo);

		CString strSQL("select * from MorphaSpermData");
		rs = theConnection->Execute((LPCSTR)strSQL,NULL,adCmdText);
		m_List.SetData(rs);
	}
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMorphaDataGather::OnClickListMorphadata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView = (NM_LISTVIEW*)pNMHDR; 

	LV_COLUMN  lc;
	lc.mask  = LVCF_TEXT ;
	TCHAR pszText[40];
	lc.pszText =  pszText;
	lc.cchTextMax = sizeof(pszText)/sizeof(pszText[0]);
	int nCol = 0;
	while (m_List.GetColumn(nCol,&lc))
	{
		if(CString(lc.pszText) == TEXT("精子编号"))
			break;
		++nCol;
	}
	CString str = m_List.GetItemText(pNMListView->iItem,nCol);
	if(str == "") return;
	CString strSQLinfo;
	strSQLinfo.Format("select * from MorphaSpermData where pID = '%s'", str);

	try
	{
		_RecordsetPtr rs = theConnection->Execute((LPCTSTR)strSQLinfo,NULL,adCmdText);
		_variant_t vt;

		vt = rs->GetCollect("pID");
		CString pID;
		pID = (char*)(_bstr_t)vt;
		bool IsNor;
		vt = rs->GetCollect("IsNormal");
		IsNor = (bool)vt;
		
		TCHAR bStr[2][10] = { "异常", "正常" };
		m_strInfo.Format("第%s号精子，%s", pID, bStr[IsNor]);
		
		vt = rs->GetCollect("m_length");
		m_length = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_width");
		m_width = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_area");
		m_area = (char*)(_bstr_t)vt;
		
		vt = rs->GetCollect("m_ellipticity");
		m_ellipticity = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_perfor_area");
		m_perfor_area = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_head_area");
		m_head_area = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_perimeter");
		m_perimeter = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_head_perfor_area");
		m_head_perfor_area = (char*)(_bstr_t)vt;
		
		vt = rs->GetCollect("m_tail_length");
		m_tail_length = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_tail_width");
		m_tail_width = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_tail_angle");
		m_tail_angle = (char*)(_bstr_t)vt;
		
		vt = rs->GetCollect("m_extension");
		m_extension = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_symmetry");
		m_symmetry = (char*)(_bstr_t)vt;
		vt = rs->GetCollect("m_ruga");
		m_ruga = (char*)(_bstr_t)vt;
		
		ReadImage(pID);
		CWnd *pWnd = GetDlgItem(IDC_STATIC_SHOWIMAGE);
		CRect rc;
		pWnd->GetWindowRect(&rc);
		InvalidateRect(rc);
		UpdateData(FALSE);

	} 
	catch (_com_error& e)
	{
		MessageBox(e.Description());
		return;
	}
	
	*pResult = 0;
}

void CMorphaDataGather::ReadImage(const CString& pID)
{
	//read  data    
	CString strSQL;
	strSQL.Format("select * from MorphaSpermData where pID = '%s'", pID);
	_RecordsetPtr rs;
	rs.CreateInstance("ADODB.Recordset");
	rs->Open((LPCTSTR)strSQL, _variant_t((IDispatch*)theConnection, true),
			 adOpenStatic, adLockOptimistic, adCmdText);
	
	long lDataLength = rs->Fields->GetItem("morphaSpermImage")->ActualSize;
	_variant_t varBLOB = rs->GetFields()->GetItem("morphaSpermImage")->GetChunk(lDataLength);
	
	if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	{	
		LPBYTE pBuffer = new BYTE[lDataLength+1];
		char *pBuf = NULL;
		SafeArrayAccessData(varBLOB.parray, (void **)&pBuf);
		memcpy(pBuffer, pBuf, lDataLength); //复制数据到缓冲区pBuffer
		SafeArrayUnaccessData (varBLOB.parray);
		
		delete m_lpData; m_lpData = NULL;
		delete m_lpInfo; m_lpInfo = NULL;
		Jpg2Bmp(pBuffer, lDataLength, m_lpInfo, m_lpData);
	}
}

void CMorphaDataGather::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CStatic* pShowImage=(CStatic*)GetDlgItem(IDC_STATIC_SHOWIMAGE);
	pShowImage->Invalidate();
	pShowImage->UpdateWindow();
	CRect rc;
	pShowImage->GetWindowRect(rc);
	int w=rc.right-rc.left;
	int h=rc.bottom-rc.top;	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	memDC.SelectObject(&bitmap);
	if(m_lpInfo) {
		::StretchDIBits(memDC.GetSafeHdc(),
			0, 0, rc.Width(), rc.Height(),
			0, 0, m_lpInfo->biWidth, m_lpInfo->biHeight,	
			m_lpData, (LPBITMAPINFO)(m_lpInfo), DIB_RGB_COLORS, SRCCOPY);
	}
	CDC* imgDC = pShowImage->GetDC();
	imgDC->BitBlt(0,0,rc.Width(),rc.Height(),&memDC,0,0,SRCCOPY);
	pShowImage->ReleaseDC(imgDC);
	// Do not call CDialog::OnPaint() for painting messages
}

void CMorphaDataGather::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	delete m_lpData; m_lpData = NULL;
	delete m_lpInfo; m_lpInfo = NULL;
	CDialog::OnClose();
}

void CMorphaDataGather::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
