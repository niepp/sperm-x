//
// CDXGraph.h
//

#ifndef __H_CDXGraph__
#define __H_CDXGraph__

// Filter graph notification to the specified window
#define WM_GRAPHNOTIFY  (WM_USER+20)
#define WM_PLAYOVER WM_USER+300

#include "Dshow.h"
#include "amstream.h"
#include "dxtrans.h"
#include "Qedit.h"
#include "CSampleGrabberCB_np.h"

#define HELPER_RELEASE(x)  if (x) {x->Release(); x = NULL; }

class CSampleGrabberCB_np;
class CDXGraph
{
private:

	LONGLONG			g_rtTotalTime;
	
	IMediaEventEx *		mEvent;
	
	IBasicAudio *		mBasicAudio;
	
	IMediaSeeking *		mSeeking;
    
	DWORD				mObjectTableEntry; 
	
public:

	IMediaControl *		mMediaControl;
	IVideoWindow  *		mVideoWindow;
	IGraphBuilder *     mGraph;  
	//在视频流捕捉的时候，可以方便的生成fiter graph
	ICaptureGraphBuilder2* m_pGraphBuilder2;
	
	CSampleGrabberCB_np*	m_pGrabberCB;

	IBaseFilter     * m_pGrabberFilter;
	ISampleGrabber   *m_pGrabber;
	IBasicVideo *		mBasicVideo;
	IMediaPosition *    pmp;				//数据流的位置查询接口指针
	REFTIME tCurrent;						//视频文件的当前位置时间
	REFTIME tLength;						//视频文件的总时间长度
	REFTIME tRemain;						//视频文件的剩余时间

	CString m_strSourceFile;
	HWND    m_hwnd;

public:
	CDXGraph(CString strSourceFile, HWND hwnd);
	virtual ~CDXGraph();

public:
	int PlayOver();
	IBaseFilter *pMux;
	IConfigAviMux *m_pConfigAviMux;
	IFileSinkFilter *m_pSink;
	IBaseFilter* m_pVCap;

	void DisplayVideoWin();

	virtual bool Create(void);
	virtual void Release(void);
	virtual bool Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	bool SetDisplayWindow(HWND inWindow);
	bool SetNotifyWindow(HWND inWindow);
	bool ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight);
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	bool Run(void);        // Control filter graph
	bool Stop(void);
	bool Pause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	bool SetFullScreen(BOOL inEnabled);
	bool GetFullScreen(void);

	// IMediaSeeking
	bool GetCurrentPosition(double * outPosition);
	bool GetStopPosition(double * outPosition);
	bool SetCurrentPosition(double inPosition);
	bool SetStartStopPosition(double inStart, double inStop);
	bool GetDuration(double * outDuration);
	bool SetPlaybackRate(double inRate);

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);
	long GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	bool RenderFile(const char * inFile);
	bool SnapshotBitmap(const char * outFile);

private:
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	
	bool QueryInterfaces(void);
};

#endif // __H_CDXGraph__