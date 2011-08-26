#include "StdAfx.h"
#include "CSampleGrabberCB_np.h"
#include "SpermView.h"
#include "View3.h"
#include "ImageProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern 	CSpermView* theView1;
extern  CView3* theView3;

STDMETHODIMP CSampleGrabberCB_np::BufferCB( double SampleTime, BYTE * pBuffer, long BufferLen )
{
	int &nFrameNum = theView3->m_pwndLiveDetectDlg->m_nFrameNum;
	int &nFrameCount = theView3->m_pwndLiveDetectDlg->m_nFrmCount;
	if( theView3->m_pwndLiveDetectDlg->m_bCanCap && (nFrameNum--) % (theView3->m_pwndLiveDetectDlg->m_nIntrv) == 0)
	{	
		BITMAPINFOHEADER	bi; //��Ϣͷ
		memset(&bi, 0, sizeof(BITMAPINFOHEADER));
		bi.biSize   = sizeof(BITMAPINFOHEADER);
		bi.biWidth  = Width;
		bi.biHeight = Height;
		
		bi.biPlanes	= 1;
		bi.biBitCount  = 32;
		bi.biSizeImage = BufferLen;

		// ���ͷ�ԭ���Ŀռ�	
		if(theView1->m_lpBMIH[FRAME_NUM - nFrameCount] != NULL)
		{
			delete []theView1->m_lpBMIH[FRAME_NUM - nFrameCount];
			theView1->m_lpBMIH[FRAME_NUM - nFrameCount] = NULL;
		}
		if( theView1->m_lpImage[FRAME_NUM - nFrameCount] != NULL )
		{
			delete []theView1->m_lpImage[FRAME_NUM - nFrameCount];
			theView1->m_lpImage[FRAME_NUM - nFrameCount] = NULL;
		}	

		if( theView1->m_lpBMIH[FRAME_NUM - nFrameCount] == NULL )
			theView1->m_lpBMIH[FRAME_NUM - nFrameCount] = new BITMAPINFOHEADER;
		memcpy(theView1->m_lpBMIH[FRAME_NUM - nFrameCount], &bi, sizeof(BITMAPINFOHEADER));

		if( theView1->m_lpImage[FRAME_NUM - nFrameCount] == NULL )
			theView1->m_lpImage[FRAME_NUM - nFrameCount] = new unsigned char[BufferLen];
		memcpy(theView1->m_lpImage[FRAME_NUM - nFrameCount], pBuffer, BufferLen);

		LPBITMAPINFOHEADER& head = theView1->m_lpBMIH[FRAME_NUM - nFrameCount];
		LPBYTE& data = theView1->m_lpImage[FRAME_NUM - nFrameCount];

		nFrameCount --;
		
		CString  strInfo;
		if( nFrameCount == 0 )
			strInfo.Format("��Ƶ������ϣ�");
		else
			strInfo.Format("���ڲ�����Ƶ��(%d/%d)֡", FRAME_NUM - nFrameCount, FRAME_NUM );
				
		::SendMessage(theView3->m_pwndLiveDetectDlg->m_hWnd,USER_MESSAGE_SHOW_CAPTURE_PROGRESS,(WPARAM)&strInfo,0);

		if (nFrameCount == 0) {
			theView3->m_pwndLiveDetectDlg->m_bCanCap = false;
			theView1->PostMessage(USER_MM_CAP_FRM_HAS_BEEN_FINISHED);
		}		
	}
	return 0;
}
