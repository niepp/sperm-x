// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "crystalreportviewer10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrystalReportViewer10

IMPLEMENT_DYNCREATE(CCrystalReportViewer10, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CCrystalReportViewer10 properties

/////////////////////////////////////////////////////////////////////////////
// CCrystalReportViewer10 operations

LPUNKNOWN CCrystalReportViewer10::GetReportSource()
{
	LPUNKNOWN result;
	InvokeHelper(0xfa, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetReportSource(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xfa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CCrystalReportViewer10::GetDisplayGroupTree()
{
	BOOL result;
	InvokeHelper(0xfb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetDisplayGroupTree(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetDisplayToolbar()
{
	BOOL result;
	InvokeHelper(0xfc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetDisplayToolbar(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableGroupTree()
{
	BOOL result;
	InvokeHelper(0xfd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableGroupTree(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableNavigationControls()
{
	BOOL result;
	InvokeHelper(0xfe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableNavigationControls(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableStopButton()
{
	BOOL result;
	InvokeHelper(0xff, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableStopButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xff, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnablePrintButton()
{
	BOOL result;
	InvokeHelper(0x100, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnablePrintButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableZoomControl()
{
	BOOL result;
	InvokeHelper(0x101, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableZoomControl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x101, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableCloseButton()
{
	BOOL result;
	InvokeHelper(0x102, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableCloseButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableProgressControl()
{
	BOOL result;
	InvokeHelper(0x103, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableProgressControl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x103, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableSearchControl()
{
	BOOL result;
	InvokeHelper(0x104, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableSearchControl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableRefreshButton()
{
	BOOL result;
	InvokeHelper(0x105, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableRefreshButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableDrillDown()
{
	BOOL result;
	InvokeHelper(0x106, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableDrillDown(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableAnimationCtrl()
{
	BOOL result;
	InvokeHelper(0x107, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableAnimationCtrl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x107, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableSelectExpertButton()
{
	BOOL result;
	InvokeHelper(0x108, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableSelectExpertButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x108, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CCrystalReportViewer10::ViewReport()
{
	InvokeHelper(0x109, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CCrystalReportViewer10::GetEnableToolbar()
{
	BOOL result;
	InvokeHelper(0x154, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableToolbar(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x154, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetDisplayBorder()
{
	BOOL result;
	InvokeHelper(0x155, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetDisplayBorder(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x155, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetDisplayTabs()
{
	BOOL result;
	InvokeHelper(0x156, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetDisplayTabs(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x156, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetDisplayBackgroundEdge()
{
	BOOL result;
	InvokeHelper(0x157, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetDisplayBackgroundEdge(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x157, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH CCrystalReportViewer10::GetTrackCursorInfo()
{
	LPDISPATCH result;
	InvokeHelper(0x159, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short CCrystalReportViewer10::GetActiveViewIndex()
{
	short result;
	InvokeHelper(0x15a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short CCrystalReportViewer10::GetViewCount()
{
	short result;
	InvokeHelper(0x15b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::ActivateView(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x15c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

void CCrystalReportViewer10::AddView(const VARIANT& GroupPath)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x15d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &GroupPath);
}

void CCrystalReportViewer10::CloseView(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x15e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

VARIANT CCrystalReportViewer10::GetViewPath(short Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x15f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index);
	return result;
}

void CCrystalReportViewer10::PrintReport()
{
	InvokeHelper(0x160, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::Refresh()
{
	InvokeHelper(0x161, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::SearchForText(LPCTSTR Text)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x162, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text);
}

void CCrystalReportViewer10::ShowFirstPage()
{
	InvokeHelper(0x163, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::ShowNextPage()
{
	InvokeHelper(0x164, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::ShowPreviousPage()
{
	InvokeHelper(0x165, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::ShowLastPage()
{
	InvokeHelper(0x166, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCrystalReportViewer10::ShowNthPage(short PageNumber)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x167, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PageNumber);
}

void CCrystalReportViewer10::Zoom(short ZoomLevel)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x168, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ZoomLevel);
}

long CCrystalReportViewer10::GetCurrentPageNumber()
{
	long result;
	InvokeHelper(0x169, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::ShowGroup(const VARIANT& GroupPath)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x16a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &GroupPath);
}

BOOL CCrystalReportViewer10::GetIsBusy()
{
	BOOL result;
	InvokeHelper(0x16b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CCrystalReportViewer10::GetEnablePopupMenu()
{
	BOOL result;
	InvokeHelper(0x16c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnablePopupMenu(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x16c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableExportButton()
{
	BOOL result;
	InvokeHelper(0x16d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableExportButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x16d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CCrystalReportViewer10::GetEnableSearchExpertButton()
{
	BOOL result;
	InvokeHelper(0x16f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableSearchExpertButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x16f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CCrystalReportViewer10::SearchByFormula(LPCTSTR formula)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x170, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 formula);
}

CString CCrystalReportViewer10::GetViewName(BSTR* pTabName)
{
	CString result;
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x171, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		pTabName);
	return result;
}

BOOL CCrystalReportViewer10::GetEnableHelpButton()
{
	BOOL result;
	InvokeHelper(0x172, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetEnableHelpButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x172, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

VARIANT CCrystalReportViewer10::GetGroup()
{
	VARIANT result;
	InvokeHelper(0x190, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::GetLastPageNumber(long* pageN, BOOL* lastPageKnown)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PBOOL;
	InvokeHelper(0x191, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pageN, lastPageKnown);
}

void CCrystalReportViewer10::RefreshEx(BOOL refreshServerData)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x192, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 refreshServerData);
}

BOOL CCrystalReportViewer10::GetLaunchHTTPHyperlinksInNewBrowser()
{
	BOOL result;
	InvokeHelper(0x1f4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CCrystalReportViewer10::SetLaunchHTTPHyperlinksInNewBrowser(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}
