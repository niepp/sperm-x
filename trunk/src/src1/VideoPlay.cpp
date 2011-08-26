// VideoPlay.cpp: implementation of the CMyVideoPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sperm.h"
#include "VideoPlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyVideoPlay::CMyVideoPlay()
{
	m_hDisplayWnd = NULL;
	m_hNotifyWnd  = NULL;
	m_strSourceFile = _T("");
	m_FilterGraph = NULL;

}

CMyVideoPlay::~CMyVideoPlay()
{
	DestroyGraph();
}

void CMyVideoPlay::Init(HWND hDisplayWnd, HWND hNotifyWnd,CString str)
{
	m_hDisplayWnd = hDisplayWnd;
	m_hNotifyWnd  = hNotifyWnd;
	m_strSourceFile = str;
    CreateGraph();
}

VideoInfo CMyVideoPlay::GetVideoInfo()
{
	VideoInfo  vif;
	vif.mHeight = m_FilterGraph->m_pGrabberCB->Height;
	vif.mWidth  = m_FilterGraph->m_pGrabberCB->Width;
	vif.mFramePerSecond = m_FilterGraph->m_pGrabberCB->mFramePerSecond;
	return vif;
}

void CMyVideoPlay::OnVideoPlay() // 从头播放
{
	if (m_FilterGraph)
	{
		m_FilterGraph->SetCurrentPosition(0);
		m_FilterGraph->SetDisplayWindow(m_hDisplayWnd);
		m_FilterGraph->Run();
	}
}

void CMyVideoPlay::OnVideoStop()   // 停止
{
	if (m_FilterGraph)
	{
		double outPosition = 0;
		m_FilterGraph->GetStopPosition(&outPosition);
		m_FilterGraph->SetCurrentPosition(outPosition);
		m_FilterGraph->Stop();
		m_FilterGraph->SetDisplayWindow(NULL);
	}
}

void CMyVideoPlay::OnVideoPause()  // 暂停
{
	if (m_FilterGraph)
	{
		m_FilterGraph->Pause();
		m_FilterGraph->SetDisplayWindow(NULL);
	}
}

void CMyVideoPlay::OnVideoResume() // 从暂停处开始播放
{
	if(m_FilterGraph)
	{
		m_FilterGraph->SetDisplayWindow(m_hDisplayWnd);
		m_FilterGraph->Run();	
	}
}

void CMyVideoPlay::CreateGraph()
{
	DestroyGraph();
	m_FilterGraph = new CDXGraph(m_strSourceFile, m_hDisplayWnd);
	if (m_FilterGraph->Create())
	{
		// Render the source clip
		m_FilterGraph->DisplayVideoWin();
		// Set video window and notification window
		m_FilterGraph->SetDisplayWindow(m_hDisplayWnd);
		m_FilterGraph->SetNotifyWindow(m_hNotifyWnd);
		// Show the first frame
		m_FilterGraph->Pause();
	}
}

void CMyVideoPlay::DestroyGraph()
{
	if (m_FilterGraph)
	{
		// Stop the filter graph first
		m_FilterGraph->Stop();
		m_FilterGraph->SetNotifyWindow(NULL);
		delete m_FilterGraph;
		m_FilterGraph = NULL;
	}
}
