#ifndef _H_CSAMPLEGRABBERCB_NP__
#define _H_CSAMPLEGRABBERCB_NP__


#include "Dshow.h"
#include "amstream.h"
#include "Qedit.h"

class CSampleGrabberCB_np : public ISampleGrabberCB  
{
public:
	long				Width;
	long				Height;
	INT                 mFramePerSecond;  // 帧率
	DWORD				mdwBitRate;  // 位速率

	BITMAPINFOHEADER	bih;

	BOOL			bCaptureFrame;

	CSampleGrabberCB_np() {}

	void SetBitmapHeader()
	{
		memset( &bih, 0, sizeof( bih ) );
        bih.biSize = sizeof( bih );
        bih.biWidth = Width;
        bih.biHeight = Height;
        bih.biPlanes = 1;
        bih.biBitCount = 32;
	}

	STDMETHODIMP_(ULONG) AddRef()  { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }
	
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        CheckPointer(ppv, E_POINTER);
        
        if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) 
        {
            *ppv = (void *) static_cast<ISampleGrabberCB *>(this);
            return NOERROR;
        }
        return E_NOINTERFACE;
    }
	
    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {
        return 0;
    }
	
    STDMETHODIMP BufferCB( double SampleTime, BYTE * pBuffer, long BufferLen );
    
};

#endif //_H_CSAMPLEGRABBERCB_NP__