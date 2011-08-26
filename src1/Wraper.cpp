#include "stdafx.h"
#include "Wraper.h"
#include "ImageConvert.h"

CWraper::CWraper(LPBITMAPINFOHEADER  lpBmInfoSrc, LPBYTE lpBmDataSrc, vector<POINT>& RegionPoints, int r)
{
	m_lpDibIHPro = new BITMAPINFOHEADER;
	memcpy(m_lpDibIHPro, lpBmInfoSrc, sizeof(BITMAPINFOHEADER));
	m_lpDibDataPro = new BYTE[lpBmInfoSrc->biSizeImage];
	memcpy(m_lpDibDataPro, lpBmDataSrc, lpBmInfoSrc->biSizeImage);
	m_RegionPoints = RegionPoints;
}
	
CWraper::~CWraper()
{
	delete []m_lpDibIHPro; m_lpDibIHPro = NULL;
	delete []m_lpDibDataPro; m_lpDibDataPro = NULL;
}
	
bool CWraper::OnSperMopySeg()
{
	IplImage *Img = cvCreateImage(cvSize(m_lpDibIHPro->biWidth, m_lpDibIHPro->biHeight), IPL_DEPTH_8U, 1);
	Dib3ToIplImage1(m_lpDibIHPro, m_lpDibDataPro, Img);
	SinSpmReco ssr(Img, m_RegionPoints);
	IplImage *ResImg = ssr.SegSperm();

	memset(m_lpDibDataPro, 255, m_lpDibIHPro->biSizeImage);
	if(ResImg==NULL) {cvReleaseImage(&Img);return false;}
	int i,j;
	int width = Img->width;
	int height= Img->height;
	int bytewidth =  WIDTHBYTES(width * m_lpDibIHPro->biBitCount);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if( CV_IMAGE_ELEM(ResImg, uchar, i, j) == (uchar)(PerforRG) )
			{
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 0] = 0;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 1] = 0;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 2] = 255;
			}
			else if( CV_IMAGE_ELEM(ResImg, uchar, i, j) == (uchar)(HEADRG) )
			{
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 0] = 255;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 1] = 0;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 2] = 0;
			}
			else if( CV_IMAGE_ELEM(ResImg, uchar, i, j) == (uchar)(0) )
			{
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 0] = 0;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 1] = 255;
				m_lpDibDataPro[(height-1-i)*bytewidth + 3*j + 2] = 0;
			}
		}
	}
	cvReleaseImage(&Img);
	return true;
}

void CWraper::OnSperMopyParamCalc()
{
	SpermParamCalc spc(m_lpDibIHPro, m_lpDibDataPro);
	spc.OnSperMopyParamCalc();
	m_SperMopyParameter = spc.m_SperMopyParameter;
}