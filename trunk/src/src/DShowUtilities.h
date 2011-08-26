#ifndef DShowUtilities_H_
#define DShowUtilities_H_

#include <DShow.h>

#pragma comment(lib,"Strmiids.lib")


//这只是一个很小的函数而已，只是一些例子代码中会用到

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; } 
#endif

HRESULT AddFilterByCLSID(
						 IGraphBuilder *pGraph,  // Pointer to the Filter Graph Manager.
						 const GUID& clsid,      // CLSID of the filter to create.
						 LPCWSTR wszName,        // A name for the filter.
						 IBaseFilter **ppF);      // Receives a pointer to the filter.

HRESULT GetUnconnectedPin(
						  IBaseFilter *pFilter,   // Pointer to the filter.
						  PIN_DIRECTION PinDir,   // Direction of the pin to find.
						  IPin **ppPin);           // Receives a pointer to the pin.

HRESULT ConnectFilters(
					   IGraphBuilder *pGraph, // Filter Graph Manager.
					   IPin *pOut,            // Output pin on the upstream filter.
					   IBaseFilter *pDest);    // Downstream filter.

HRESULT ConnectFilters(
					   IGraphBuilder *pGraph, 
					   IBaseFilter *pSrc, 
					   IBaseFilter *pDest);

//判断pin是否有此mediaType,stolen from sdk amcap
BOOL HasMediaType(IPin* pPin,  REFGUID majorType );

//stolen from sdk amcap,其中用到上面的函数HasMediaType
BOOL IsVideoPin( IPin* pPin );

// Tear down everything downstream of a given filter
//将一个给定的filter下的所有filter都删除，通过递归调用实现
//stolen from sdk amcap
void NukeDownstream(IBaseFilter *pf,IGraphBuilder *pFg);

// Tear down everything downstream of the capture filters, so we can build
// a different capture graph.  Notice that we never destroy the capture filters
// and WDM filters upstream of them, because then all the capture settings
// we've set would be lost.
//将capture filter后面的filter删除，这样就可以建立一个新的capture graph。
//不删除capture filter的原因在于如果删除，那么，我们原先的设置就白设了
//stolen from sdk amcap
void TearDownGraph(IGraphBuilder *pFg,IVideoWindow *pVW,IBaseFilter *pVCap);

#endif


