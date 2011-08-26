//DShowUtilities.cpp--------------------------------------------------------


#include "stdafx.h"
#include "DShowUtilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*-----------------------------------------------------------------------------------
The following function creates a filter with a specified class identifier (CLSID)
and adds it to the filter graph

USE Example: Add AVI Mux filter to the graph

IBaseFilter *pMux;
hr = AddFilterByCLSID(pGraph, CLSID_AviDest, L"AVI Mux", &pMux); 
if (SUCCEEDED(hr))
{
    //...............
   pMux->Release();
}
----------------------------------------------------------------------------------*/
HRESULT AddFilterByCLSID(
    IGraphBuilder *pGraph,  // Pointer to the Filter Graph Manager.
    const GUID& clsid,      // CLSID of the filter to create.
    LPCWSTR wszName,        // A name for the filter.
    IBaseFilter **ppF)      // Receives a pointer to the filter.
{
    if (!pGraph || ! ppF) return E_POINTER;
    *ppF = 0;
    IBaseFilter *pF = 0;
    HRESULT hr = CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, reinterpret_cast<void**>(&pF));
    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter(pF, wszName);
        if (SUCCEEDED(hr))
            *ppF = pF;
        else
            pF->Release();
    }
    return hr;
}

/*----------------------------------------------------------------------------------
This function find an unconnected pin on a filter

The following function searches on a filter for an unconnected pin, 
either input or output. It returns the first matching pin. 
Finding unconnected pins is useful when you are connecting filters.

USE Example:

IPin *pOut = NULL;
HRESULT hr = GetUnconnectedPin(pFilter, PINDIR_OUTPUT, &pOut);
if (SUCCEEDED(hr))
{
    //...........
    pOut->Release();
}

-----------------------------------------------------------------------------------*/
HRESULT GetUnconnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin)           // Receives a pointer to the pin.
{
    *ppPin = 0;
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;
            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))  // Already connected, not the pin we want.
            {
                pTmp->Release();
            }
            else  // Unconnected, this is the pin we want.
            {
                pEnum->Release();
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }
    pEnum->Release();
    // Did not find a matching pin.
    return E_FAIL;
}

/*--------------------------------------------------------------------------------
The following function connects an output pin from one filter to 
the first available input pin on another filter.
里面调用了前面定义的函数GetUnconnectedPin

---------------------------------------------------------------------------------*/

HRESULT ConnectFilters(
    IGraphBuilder *pGraph, // Filter Graph Manager.
    IPin *pOut,            // Output pin on the upstream filter.
    IBaseFilter *pDest)    // Downstream filter.
{
    if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
    {
        return E_POINTER;
    }
#ifdef debug
        PIN_DIRECTION PinDir;
        pOut->QueryDirection(&PinDir);
        _ASSERTE(PinDir == PINDIR_OUTPUT);
#endif

    // Find an input pin on the downstream filter.
    IPin *pIn = 0;
    HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);//调用了前面定义的函数
    if (FAILED(hr))
    {
        return hr;
    }
    // Try to connect them.
    hr = pGraph->Connect(pOut, pIn);
    pIn->Release();
    return hr;
}


/*---------------------------------------------------------------------------------
Here is an overloaded version of the same function. The second parameter is 
a pointer to a filter, rather than a pin.
The function connects the first filter to the second filter


USE Example:
The following example use this example to connect AVI Mux filter to File Writer 
filter

IBaseFilter *pMux, *pWrite;
hr = AddFilterByCLSID(pGraph, CLSID_AviDest, L"AVI Mux", &pMux);
if (SUCCEEDED(hr))
{
    hr = AddFilterByCLSID(pGraph, CLSID_FileWriter, L"File Writer", &pWrite);
    if (SUCCEEDED(hr))
    {
        hr = ConnectFilters(pGraph, pMux, pWrite);
        //Use IFileSinkFilter to set the file name (not shown). 
       pWrite->Release();
    }
    pMux->Release();
}

这里面用了前面定义的那个ConnectFilters函数和GetUnconnectedPin函数
----------------------------------------------------------------------------------*/
HRESULT ConnectFilters(
    IGraphBuilder *pGraph, 
    IBaseFilter *pSrc, 
    IBaseFilter *pDest)
{
    if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
    {
        return E_POINTER;
    }

    // Find an output pin on the first filter.
    IPin *pOut = 0;
    HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);//用了前面定义的函数
    if (FAILED(hr)) 
    {
        return hr;
    }
    hr = ConnectFilters(pGraph, pOut, pDest);//用了前面定义的函数
    pOut->Release();
    return hr;
}


/*---------------------------------------------------------------------------------
判断pin是否有此mediaType,stolen from sdk amcap
--------------------------------------------------------------------------------*/
BOOL HasMediaType(IPin* pPin,  REFGUID majorType )
{
    if( !pPin )
    {
        return FALSE;
    }

    IEnumMediaTypes* pMediaTypes;
    HRESULT hr = pPin->EnumMediaTypes( &pMediaTypes );
    if( FAILED( hr ) )
    {
        return FALSE;
    }

    hr = pMediaTypes->Reset();
    if( FAILED( hr ) )
    {
        return FALSE;
    }

    ULONG           fetched;
    AM_MEDIA_TYPE   *mediaType;

    while( S_OK == pMediaTypes->Next( 1, &mediaType, &fetched ) )
    {
        if( IsEqualGUID( mediaType->majortype, majorType ) )
        {
       //     DeleteMediaType( mediaType );
            return TRUE;
        }
//        DeleteMediaType( mediaType );
    }

    return FALSE;

}

/*--------------------------------------------------------------------------------
判断是否是video pin,stolen from sdk amcap
此函数用到上面的函数HasMediaType
--------------------------------------------------------------------------------*/
BOOL IsVideoPin( IPin* pPin )
{
    return HasMediaType( pPin, MEDIATYPE_Video );//上面的函数
}


/*---------------------------------------------------------------------------------
 Tear down everything downstream of a given filter
将一个GraphBuilder中的给定的filter下的所有filter都删除，通过递归调用实现
stolen from amcap
----------------------------------------------------------------------------------*/
void NukeDownstream(IBaseFilter *pf,IGraphBuilder *pFg)
{
    IPin *pP=0, *pTo=0;
    ULONG u;
    IEnumPins *pins = NULL;
    PIN_INFO pininfo;

    if (!pf)
        return;

    HRESULT hr = pf->EnumPins(&pins);
    pins->Reset();//通过这个函数调用，获得最新的数据？

    while(hr == NOERROR)
    {
        hr = pins->Next(1, &pP, &u);//找到filter上的pin
        if(hr == S_OK && pP)
        {
            pP->ConnectedTo(&pTo);//找到与这个pin相连的另一个filter上的pin
            if(pTo)
            {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr == NOERROR)
                {
                    if(pininfo.dir == PINDIR_INPUT)//只寻找input的pin
                    {
                        NukeDownstream(pininfo.pFilter,pFg);//递归调用
                        pFg->Disconnect(pTo);//要将pin的两端同时移除
                        pFg->Disconnect(pP);
                        pFg->RemoveFilter(pininfo.pFilter);//在递归调用中，
         //最先删除的是只有input pin的filter
                    }
                    pininfo.pFilter->Release();
                }
                pTo->Release();
            }
            pP->Release();
        }
    }//end while

    if(pins)
        pins->Release();
}

/*---------------------------------------------------------------------------------
 Tear down everything downstream of the capture filters, so we can build
 a different capture graph.  Notice that we never destroy the capture filters
 and WDM filters upstream of them, because then all the capture settings
 we've set would be lost.
将capture filter后面的filter删除，这样就可以建立一个新的capture graph。
不删除capture filter的原因在于如果删除，那么，我们原先的设置就白设了
stolen from sdk amcap
---------------------------------------------------------------------------------*/
void TearDownGraph(IGraphBuilder *pFg,IVideoWindow *pVW,IBaseFilter *pVCap)
{
    if(pVW)//IVideoWindow停止视频显示
    {
        // stop drawing in our window, or we may get wierd repaint effects
        pVW->put_Owner(NULL);
        pVW->put_Visible(OAFALSE);
        pVW->Release();
        pVW = NULL;
    }

    // destroy the graph downstream of our capture filters
    if(pVCap)
        NukeDownstream(pVCap,pFg);//这个函数 Tear down everything downstream of a given filter
                              //但是，这个filter却是会保留的
 //然后应该是对一些filter的release
}

