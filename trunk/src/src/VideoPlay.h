// VideoPlay.h: interface for the CMyVideoPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOPLAY_H__719F3572_FBFE_48A2_A80B_DE717947ABCC__INCLUDED_)
#define AFX_VIDEOPLAY_H__719F3572_FBFE_48A2_A80B_DE717947ABCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDXGraph.h"

typedef struct VideoInfo{
    int mHeight;
	int mWidth;
	int mFramePerSecond;
}VideoInfo;

class CMyVideoPlay {  

private:
	CString   m_strSourceFile;
	HWND      m_hDisplayWnd;
	HWND      m_hNotifyWnd;

public:	
	CDXGraph *m_FilterGraph;

	CMyVideoPlay();
	virtual ~CMyVideoPlay();
	VideoInfo GetVideoInfo();
	void Init(HWND hDisplayWnd, HWND hNotifyWnd, CString str);
	void CreateGraph();
	void DestroyGraph();

	void OnVideoPlay();
	void OnVideoStop();
	void OnVideoPause();
	void OnVideoResume();
};

#endif // !defined(AFX_VIDEOPLAY_H__719F3572_FBFE_48A2_A80B_DE717947ABCC__INCLUDED_)
