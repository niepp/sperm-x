// LoginThread.cpp : implementation file
//

#include "stdafx.h"
#include "sperm.h"
#include "LoginThread.h"
#include "afxmt.h"
#include "AllFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpermApp theApp;
extern _ConnectionPtr theConnection;
/////////////////////////////////////////////////////////////////////////////
// CLoginThread

IMPLEMENT_DYNCREATE(CLoginThread, CWinThread)
CLoginThread::CLoginThread()
{	
}

CLoginThread::~CLoginThread()
{
}

BOOL CLoginThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	::CoInitialize(NULL);
	m_Connection.CreateInstance("adodb.connection");
	return TRUE;
}

int CLoginThread::ExitInstance()
{
	if(m_Connection->GetState() == adStateOpen)
		m_Connection->Close();
	m_Connection.Release();
	CoUninitialize();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CLoginThread, CWinThread)
	//{{AFX_MSG_MAP(CLoginThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_THDMSG_GETLOGININFO,GetLoginInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginThread message handlers

void CLoginThread::GetLoginInfo(WPARAM w, LPARAM l)
{
	m_ThdInfo = (ConnectInfoThd*)w;
	ConnectInfoThd* pCon=m_ThdInfo;
	CString& m_strServerName=pCon->m_strServerName;
	CString& m_strUsername = pCon->m_strUsername;
	CString& m_password = pCon->m_password;
	CString str;
	try
	{
		// 连接数据源	
		//str="Provider=SQLOLEDB.1;Persist Security Info=False;Data Source=";

		str = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=../data/data.mdb";
		m_Connection->Open(str.GetString(),"","",adModeUnknown);
		m_Connection->PutCursorLocation(adUseClient);

		theConnection->Open(str.GetString(),"","",adModeUnknown);
		theConnection->PutCursorLocation(adUseClient);

		//_bstr_t sqlUseDatabase("use master");
		//m_Connection->Execute(sqlUseDatabase,NULL,adCmdText);

		//   创建保存用户名密码的表
		if(!IsTableExist(m_Connection,TEXT("username")))
		{
			CString strSQLuser("create table username\
			(username varchar(20) primary key,\
			 password varchar(20))");
			m_Connection->Execute((LPCTSTR)strSQLuser,NULL,adCmdText);
		}	
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);	
		PostQuitMessage(0);
	}	
	try
	{	_RecordsetPtr ptr("adodb.recordset");
		//  创建数据库
		//_bstr_t sqlDBEXIST("select 1 from master..sysdatabases where name = 'sperm'");
		//_RecordsetPtr ptr("adodb.recordset");
		//ptr->Open(sqlDBEXIST,m_Connection.GetInterfacePtr(),
		//	adOpenStatic,adLockOptimistic,adCmdUnknown);
		//if(ptr->GetRecordCount() == 0 )  // 没有数据库就创建它
		//{
		//	_bstr_t strCreateDatabase("create database sperm");
		//	m_Connection->Execute(strCreateDatabase,NULL,adCmdText);
		//}

		//  确认用户合法性
		_bstr_t sqlIllegalUser("select * from username where username='");
		sqlIllegalUser += (LPCTSTR)m_strUsername+_bstr_t("' and ") + 
			_bstr_t("password='")+(LPCTSTR)m_password+_bstr_t("'");
		ptr = m_Connection->Execute(sqlIllegalUser,NULL,adCmdText);
		if( ptr->GetRecordCount() == 0)
		{
			AfxMessageBox("用户名或密码错误,请重新输入");
			::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);	
			PostQuitMessage(0);
		}
		_variant_t vt=ptr->GetCollect("password");
		CString pw=(char*)(_bstr_t)vt;
		if( pw != m_password)
		{
			AfxMessageBox("用户名或密码错误,请重新输入");
			::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);
			PostQuitMessage(0);
		}	

		//theConnection->Open((LPCSTR)str,(LPCSTR)"sa"
		//		,(LPCSTR)"sa",adConnectUnspecified);
		//_bstr_t sqlUseDatabase("use sperm");
		//theConnection->Execute(sqlUseDatabase,NULL,adCmdText);
		//m_Connection->Execute(sqlUseDatabase,NULL,adCmdText);
	}
	catch (_com_error& e) 
	{ 
		AfxMessageBox(e.Description());
		::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);	
		PostQuitMessage(0);
	}
	catch (...)
	{
		AfxMessageBox("登陆错误.");
		::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);
		PostQuitMessage(0);
	}

	//  创建表
		_bstr_t strSQLbasicinfo("create table basicinfo \
			( \
			  pDetectNO    varchar(20) primary key,\
			  pCaseNO      varchar(20) , \
			  pName        varchar(20),\
			  pAge         int,\
			  pDetectDateTime  datetime,\
			  pDetectDoctor  varchar(20) ,\
			  pDeliverDoctor  varchar(20) ,\
			  pMemory        varchar(20),  \
			  pSampleNO    varchar(20)  \
			)");
		_bstr_t strSQLspermchait("create table spermchait \
		    ( \
			  pDetectNO    varchar(20) primary key,\
			  pSpermGetDateTime datetime,\
			  pDaysOfAbstinency    int,\
			  pSpermMethod   varchar(20),\
			  pSpermVolume   Decimal(5,1),\
			  pLiquifyTime   int,\
			  pShape         varchar(20),\
			  pDilutionRatio decimal(20,3), \
			  pPH            decimal(10,1),\
			  pThickness     varchar(20),\
			  pSmell         varchar(20),\
			  pColor         varchar(20),\
			  pCohesion      varchar(20),\
			  pLiquifyState  varchar(20),\
			  pRoomTempera   int\
			 )");
		 _bstr_t strSQLlivenessresult("create table livenessresult \
		    ( \
		      pDetectNO    varchar(20) primary key,\
			  pMoveSperm   int,\
			  pDetectedSperm int,\
			  pSpermDensity  decimal(20,3),\
			  pStraightSpermNum int,\
			  pCurveSpermNum  int,\
			  pActiveSpermNum int,\
			  pActiveSpermRatio decimal(20,3),\
			  pActiveSpermDensity decimal(20,3),\
			  pLevelARatio     decimal(20,3),\
			  pLevelBRatio     decimal(20,3),\
			  pLevelCRatio     decimal(20,3),\
			  pLevelDRatio     decimal(20,3),\
			  pLevelADensity     decimal(20,3),\
			  pLevelBDensity     decimal(20,3),\
			  pLevelCDensity     decimal(20,3),\
			  pLevelDDensity     decimal(20,3),\
			  pLevelANum     int,\
			  pLevelBNum     int,\
			  pLevelCNum     int,\
			  pLevelDNum     int\
			)");
		 _bstr_t strSQLspermnumstatics("create table spermnumstatics\
			 ( \
			  pDetectNO    varchar(20) primary key,\
			  pSpermTotalNum     int,\
			  pActiveSpermNum    int,\
			  pMoveSpermNum      int,\
			  pFrontSpermNum     int,\
			  pStraightSpermNum  int,\
			  pCurveSpermNum     int,\
			  pSpermTotalRatio   decimal(20,3),\
			  pActiveSpermRatio  decimal(20,3),\
			  pMoveSpermRatio      decimal(20,3),\
			  pFrontSpermRatio   decimal(20,3),\
			  pStraightSpermRatio  decimal(20,3),\
			  pCurveSpermRatio    decimal(20,3),\
			  pSpermTotalDensity   decimal(20,3),\
			  pActiveSpermDensity  decimal(20,3),\
			  pMoveSpermDensity      decimal(20,3),\
			  pFrontSpermDensity   decimal(20,3),\
			  pStraightSpermDensity  decimal(20,3),\
			  pCurveSpermDensity    decimal(20,3)\
			 )");
		 _bstr_t strSQLcmpwithWHO("create table cmpwithwho\
			(\
			  pDetectNO    varchar(20) primary key,\
			  pTotalDensityFact  decimal(20,3),\
			  pTotalDensityStandard   decimal(20,3),\
			  pMovityFact        decimal(20,3),\
			  pMovityStandard         decimal(20,3),\
			  pFastSpermFact     decimal(20,3),\
			  pFastSpermStandard      decimal(20,3)\
			  )");
		 _bstr_t strSQLspermmovitypara("create table spermmovitypara\
	         ( \
			  pDetectNO    varchar(20) primary key,\
			  pVAP    decimal(20,3),\
			  pVCL    decimal(20,3),\
			  pVSL    decimal(20,3),\
			  pWOB    decimal(20,3),\
			  pSTR    decimal(20,3),\
			  pLIN    decimal(20,3),\
			  pALN    decimal(20,3),\
			  pBCF    decimal(20,3),\
			  sd_vsl  decimal(20,3),\
			  sd_vcl  decimal(20,3),\
			  sd_vap  decimal(20,3),\
			  sd_alh  decimal(20,3)\
			)");
		 _bstr_t strSQLresultkind("create table resultkind\
		      ( \
			  pDetectNO    varchar(20) primary key,\
			  pActSpermCell  int,\
			  pActSpermDensity  decimal(20,3),\
			  pActSpermRatio    decimal(20,3),\
			  pSportsBCell  int,\
			  pSportsBDensity  decimal(20,3),\
			  pSportsBRatio    decimal(20,3),\
			  p16ptsCell  int,\
			  p16ptsDensity  decimal(20,3),\
			  p16ptsRatio    decimal(20,3)\
			)");  
		 _bstr_t strSQLimage("create table image_table \
	      (  \
		  pDetectNO    varchar(20)  primary key ,\
		  pImage       image\
		  )");

    		 _bstr_t strSQLMorphaResult("create table morpharesult\
				( \
				    pDetectNO varchar(20)  primary key,\
					pNormalNum   int  , \
					pAbnormalNum  int  , \
					pHeadAbnormalNum int  ,\
					pNeckAbnormalNum int  ,\
					pTailAbnormalNum int  ,\
					pBigHead int  ,\
					pSmallHead int  ,\
					pBrushHead int  ,\
					pConeHead int  ,\
					pCirHead int  ,\
					pPearHead int  ,\
					pNOShapeHead int  ,\
					pSizeAb int  ,\
					pAngleAb int  ,\
					pInsAb int  ,\
					pShortTail int  ,\
					pIrregular int  ,\
					pCirTail int  ,\
					pMulTail int  ,\
					pNoTail int  ,\
					pWind   int  ,\
					pPerforAb int  ,\
					psmallptab int  ,\
					normal_lengthNum  int     ,\
					normal_widthNum int    ,\
					normal_areaNum int    ,\
					normal_ellipticityNum int    ,\
					normal_perfor_areaNum int    ,\
					normal_head_areaNum int    ,\
					normal_perimeterNum int    ,\
					normal_head_perfor_areaNum int    ,\
					normal_tail_lengthNum int    ,\
					normal_tail_widthNum int    ,\
					normal_tail_angleNum int    ,\
					normal_extensionNum int    ,\
					normal_symmetryNum int    ,\
					normal_rugaNum int    \
			     )");
	 _bstr_t strMorphaImage("create table morphaimage(pdetectno varchar(20) primary key,\
		 fisrtimage image, padimage image, edgeimage image, singleresultimage image);");
	 _bstr_t strSQLMorphaResultRatio("create table morpharesultratio(\
		 pdetectno varchar(20) primary key,\
		 normal_lengthRatio decimal(10,3)  ,\
		 normal_widthRatio decimal(10,3)  ,\
		 normal_areaRatio decimal(10,3)  ,\
		 normal_ellipticityRatio decimal(10,3)  ,\
		 normal_perfor_areaRatio decimal(10,3)  ,\
		 normal_head_areaRatio decimal(10,3)  ,\
		 normal_perimeterRatio decimal(10,3)  ,\
		 normal_head_perfor_areaRatio decimal(10,3)  ,\
		 normal_tail_lengthRatio decimal(10,3)  ,\
		 normal_tail_widthRatio decimal(10,3)  ,\
		 normal_tail_angleRatio decimal(10,3)  ,\
		 normal_extensionRatio decimal(10,3)  ,\
		 normal_symmetryRatio decimal(10,3)  ,\
		 normal_rugaRatio decimal(10,3)  \
		 )");
	 _bstr_t strSQLMorphaSpermInfo("create table morphasperminfo(\
		 pID varchar(20) primary key,\
		 IsNormal   bit,\
		 x   int  ,\
		 y   int   ,\
		 m_length decimal(20 ,3) 	  ,\
		 m_width decimal(20 ,3)	  ,\
		 m_area decimal(20 ,3)	  ,\
		 m_ellipticity  decimal(20,3)  ,\
		 m_perfor_area decimal(20 ,3)  ,\
		 m_head_area decimal(20 ,3)	  ,\
		 m_perimeter decimal(20 ,3)	  ,\
		 m_head_perfor_area decimal(20 ,3)	  ,\
		 m_tail_length decimal(20 ,3)	  ,\
		 m_tail_width decimal(20 ,3)	  ,\
		 m_tail_angle decimal(20 ,3)	  ,\
		 m_extension decimal(20 ,3)	  ,\
		 m_symmetry decimal(20 ,3)	  ,\
		 m_ruga decimal(20,3)\
		 )");
	 
	 //{ 2009_3_11  create a new table for morpha data gather, add by happynp 
	 _bstr_t strSQLMorphaSpermData("create table morphaspermdata(\
		 pID varchar(20) primary key,\
		 IsNormal   bit,\
		 m_length int 	  ,\
		 m_width int	  ,\
		 m_area decimal(20 ,3)	  ,\
		 m_ellipticity  decimal(20,3)  ,\
		 m_perfor_area decimal(20 ,3)  ,\
		 m_head_area decimal(20 ,3)	  ,\
		 m_perimeter decimal(20 ,3)	  ,\
		 m_head_perfor_area decimal(20 ,3)	  ,\
		 m_tail_length decimal(20 ,3)	  ,\
		 m_tail_width decimal(20 ,3)	  ,\
		 m_tail_angle decimal(20 ,3)	  ,\
		 m_extension decimal(20 ,3)	  ,\
		 m_symmetry decimal(20 ,3)	  ,\
		 m_ruga decimal(20,3)  ,\
		 morphaSpermImage image\
		 )");
	 //} 2009_3_11

	 _bstr_t strSQLDoctorList("create table doctor(\
		 strSendDoctor varchar(20),\
		 strDetectDoctor varchar(20))");

	 //{ 2008_8_26  create a new table, add by happynp  
	 _bstr_t strSQLVelDistribute("create table veldistribute( \
		 pdetectno varchar(20) primary key, \
		 vcl10 decimal(10,3)  , \
		 vcl20 decimal(10,3)  , \
		 vcl30 decimal(10,3)  , \
		 vcl40 decimal(10,3)  , \
		 vcl50 decimal(10,3)  , \
		 vcl60 decimal(10,3)  , \
		 vclmax decimal(10,3)  , \
		 vsl10 decimal(10,3)  , \
		 vsl20 decimal(10,3)  , \
		 vsl30 decimal(10,3)  , \
		 vsl40 decimal(10,3)  , \
		 vsl50 decimal(10,3)  , \
		 vsl60 decimal(10,3)  , \
		 vslmax decimal(10,3)  , \
		 vap10 decimal(10,3)  , \
		 vap20 decimal(10,3)  , \
		 vap30 decimal(10,3)  , \
		 vap40 decimal(10,3)  , \
		 vap50 decimal(10,3)  , \
		 vap60 decimal(10,3)  , \
		 vapmax decimal(10,3)\
		)");
     //} 2008_8_26

	 //{ 2008_9_1  create a new table, add by happynp  
	 _bstr_t strSQLMorphaPartialRes("create table morphapartialres( \
		 pDetectNO varchar(20) primary key, \
		 nTotalNum decimal(10,3)  , \
		 nTotalNormalNum decimal(10,3)  , \
		 nHeadAbnormalNum decimal(10,3)  , \
		 nBodyAbnormalNum decimal(10,3)  , \
		 nTailAbnormalNum decimal(10,3)  , \
		 dTotalNormalRatio decimal(10,3)  , \
		 dHeadAbnormalRatio decimal(10,3)  , \
		 dBodyAbnormalRatio decimal(10,3)  , \
		 dTailAbnormalRatio decimal(10,3)  , \
		 TZI decimal(10,3)  , \
		 SDI decimal(10,3)\
		 )");
     //} 2008_9_1

	 //{ 2009_3_23  create a new table, add by happynp
	 _bstr_t strSQLFResult("create table fresult( \
							 pDetectNO varchar(20) primary key, \
							 BeforeNormalSpermNumbers decimal(5,2), \
							 AfterNormalSpermNumbers decimal(5,2))");

     //} 2008_3_23
	 try
	 {
		 if(!IsTableExist(m_Connection, TEXT("basicinfo")))
			 m_Connection->Execute(strSQLbasicinfo,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("spermchait")))
			 m_Connection->Execute(strSQLspermchait,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("livenessresult")))
			 m_Connection->Execute(strSQLlivenessresult,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("spermnumstatics")))
			 m_Connection->Execute(strSQLspermnumstatics,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("cmpwithwho")))
			 m_Connection->Execute(strSQLcmpwithWHO,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("spermmovitypara")))
			 m_Connection->Execute(strSQLspermmovitypara,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("resultkind")))
			 m_Connection->Execute(strSQLresultkind,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("image_table")))
			 m_Connection->Execute(strSQLimage,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("morpharesult")))
			 m_Connection->Execute(strSQLMorphaResult,NULL,adCmdText);	
		 if(!IsTableExist(m_Connection, TEXT("morphaimage")))
			 m_Connection->Execute(strMorphaImage,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("morpharesultratio")))
			 m_Connection->Execute(strSQLMorphaResultRatio,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("morphasperminfo")))
			 m_Connection->Execute(strSQLMorphaSpermInfo,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("morphaspermdata")))
			 m_Connection->Execute(strSQLMorphaSpermData,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("doctor")))
			 m_Connection->Execute(strSQLDoctorList,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("veldistribute")))
			 m_Connection->Execute(strSQLVelDistribute,NULL,adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("morphapartialres")))
			 m_Connection->Execute(strSQLMorphaPartialRes, NULL, adCmdText);
		 if(!IsTableExist(m_Connection, TEXT("fresult")))
			 m_Connection->Execute(strSQLFResult, NULL, adCmdText);
	 }
	 catch (_com_error& e)
	 {
		 AfxMessageBox(e.Description());
		 ::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);	
		 PostQuitMessage(0);
	 }
	 catch (...)
	 {
		 AfxMessageBox("发生错误.");
		 ::SendMessage(pCon->hwnd,USER_MSG_SETBUTTONSTATE,1,0);	
		 PostQuitMessage(0);
	 }
	 ::SendMessage(pCon->hwnd,USER_MSG_AFTERCONNECTDATABASE,0,0);
	 PostQuitMessage(0);
}

