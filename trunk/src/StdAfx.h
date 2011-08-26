// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#define _WIN32_WINNT 0x0500
#if !defined(AFX_STDAFX_H__9392317E_60DF_4B0B_8A20_E8ADF2AB1CCC__INCLUDED_)
#define AFX_STDAFX_H__9392317E_60DF_4B0B_8A20_E8ADF2AB1CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma  warning(disable : 4786)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common 

#include "afxdb.h"
#include "objbase.h"

#endif // _AFX_NO_AFXCMN_SUPPORT

#define  FRAME_NUM  30

//#include "Streams.h"
#include "dxtrans.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

#include "ImageProcess.h"
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <list>
using namespace std;

#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"../lib/cv.lib")
#pragma comment(lib,"../lib/highgui.lib")
#pragma comment(lib,"../lib/cxcore.lib")

#import "../lib/msado15.dll"  no_namespace rename("EOF","EndOfFile")
#import "../lib/craxdrt.dll"  no_namespace 

#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CSpermModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CSpermModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9392317E_60DF_4B0B_8A20_E8ADF2AB1CCC__INCLUDED_)
