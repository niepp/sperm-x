#ifndef DShowUtilities_H_
#define DShowUtilities_H_

#include <DShow.h>

#pragma comment(lib,"Strmiids.lib")


//��ֻ��һ����С�ĺ������ѣ�ֻ��һЩ���Ӵ����л��õ�

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

//�ж�pin�Ƿ��д�mediaType,stolen from sdk amcap
BOOL HasMediaType(IPin* pPin,  REFGUID majorType );

//stolen from sdk amcap,�����õ�����ĺ���HasMediaType
BOOL IsVideoPin( IPin* pPin );

// Tear down everything downstream of a given filter
//��һ��������filter�µ�����filter��ɾ����ͨ���ݹ����ʵ��
//stolen from sdk amcap
void NukeDownstream(IBaseFilter *pf,IGraphBuilder *pFg);

// Tear down everything downstream of the capture filters, so we can build
// a different capture graph.  Notice that we never destroy the capture filters
// and WDM filters upstream of them, because then all the capture settings
// we've set would be lost.
//��capture filter�����filterɾ���������Ϳ��Խ���һ���µ�capture graph��
//��ɾ��capture filter��ԭ���������ɾ������ô������ԭ�ȵ����þͰ�����
//stolen from sdk amcap
void TearDownGraph(IGraphBuilder *pFg,IVideoWindow *pVW,IBaseFilter *pVCap);

#endif


