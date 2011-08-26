#include "StdAfx.h"
#include "ImageConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool IplImageTODib(IplImage* img,LPBITMAPINFOHEADER head,LPBYTE data)
{
	try
	{
		head->biBitCount = 8 * img->nChannels;
		head->biHeight   = img->height;
		head->biWidth    = img->width;
		head->biSizeImage= WIDTHBYTES(head->biBitCount*head->biWidth)*img->height;
		head->biPlanes = 1;
		head->biXPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSX);
		head->biYPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSY);
		head->biCompression=0;
		head->biClrUsed=0;
		head->biClrImportant=0;
		head->biSize  = sizeof(BITMAPINFOHEADER);
		int i,j,k;
		for(i=0;i<img->height;++i)
			for(j=0;j<img->width;++j)
				for(k=0;k<img->nChannels;++k)
				{
					data[(img->height-1-i)*WIDTHBYTES(head->biBitCount*head->biWidth)+img->nChannels*j+k]
						= img->imageData[i*img->widthStep+img->nChannels*j+k];
				}
		return true;	
	}
	catch (...) {
		ASSERT(NULL);
		return false;	
	}
}

bool IplImage1TODib4(IplImage* Img, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData)
{
	try
	{
		lpBmInfo->biBitCount = 32;
		lpBmInfo->biHeight   = Img->height;
		lpBmInfo->biWidth    = Img->width;
		lpBmInfo->biSizeImage= WIDTHBYTES(lpBmInfo->biBitCount * lpBmInfo->biWidth) * Img->height;
		lpBmInfo->biPlanes = 1;
		lpBmInfo->biXPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSX);
		lpBmInfo->biYPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSY);
		lpBmInfo->biCompression=0;
		lpBmInfo->biClrUsed=0;
		lpBmInfo->biClrImportant=0;
		lpBmInfo->biSize  = sizeof(BITMAPINFOHEADER);

		int i,j,k;		
		int lw = WIDTHBYTES(lpBmInfo->biBitCount * lpBmInfo->biWidth);
		for(i=0; i<Img->height; ++i)
			for(j=0; j<Img->width; ++j)
				for(k=0; k<4; ++k)
				{
					lpBmData[(Img->height-1-i)*lw + 4*j + k]
						= Img->imageData[ i*Img->widthStep + j];
				}
		return true;	
	}
	catch (...) {
		ASSERT(NULL);
		return false;	
	}
}

bool IplImage1TODib3(IplImage* Img, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData)
{
	try
	{
		lpBmInfo->biBitCount = 24;
		lpBmInfo->biHeight   = Img->height;
		lpBmInfo->biWidth    = Img->width;
		lpBmInfo->biSizeImage= WIDTHBYTES(lpBmInfo->biBitCount * lpBmInfo->biWidth) * Img->height;
		lpBmInfo->biPlanes = 1;
		lpBmInfo->biXPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSX);
		lpBmInfo->biYPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSY);
		lpBmInfo->biCompression=0;
		lpBmInfo->biClrUsed=0;
		lpBmInfo->biClrImportant=0;
		lpBmInfo->biSize  = sizeof(BITMAPINFOHEADER);

		int i,j,k;		
		int lw = WIDTHBYTES(lpBmInfo->biBitCount * lpBmInfo->biWidth);
		for(i=0; i<Img->height; ++i)
			for(j=0; j<Img->width; ++j)
				for(k=0; k<3; ++k)
				{
					lpBmData[(Img->height-1-i)*lw + 3*j + k]
						= Img->imageData[ i*Img->widthStep + j];
				}
		return true;	
	}
	catch (...) {
		ASSERT(NULL);
		return false;	
	}
}

bool DibToIplImage(LPBITMAPINFOHEADER head,LPBYTE data,IplImage* img)
{
	try
	{
		int i,j,k;
		int channels=head->biBitCount/8;
		for(i=0;i<head->biHeight;++i)
			for(j=0;j<head->biWidth;++j)
				for(k=0;k<channels;++k)
				{
					img->imageData[(head->biHeight-1-i)*img->widthStep+j*channels+k]
						= data[i*WIDTHBYTES(head->biBitCount*head->biWidth)+channels*j+k];
				}
		return true;	
	}
	catch (...)
	{
		ASSERT(NULL);
		return false;	
	}
}

bool Dib4ToIplImage1(LPBITMAPINFOHEADER head,LPBYTE data, IplImage* img)
{
	try
	{
		int i,j;
		int channels = 1;
		int lw = WIDTHBYTES(head->biBitCount*head->biWidth);
		for(i=0;i<head->biHeight;++i)
			for(j=0;j<head->biWidth;++j)
					img->imageData[(head->biHeight-1-i)*img->widthStep + j + 0]
						= data[i*lw + j*4 + 0];
		return true;	
	}
	catch (...)
	{
		ASSERT(NULL);
		return false;	
	}
}

bool Dib3ToIplImage1(LPBITMAPINFOHEADER head,LPBYTE data, IplImage* img)
{
	try
	{
		int i,j;
		int channels = 1;
		int lw = WIDTHBYTES(head->biBitCount*head->biWidth);
		for(i=0;i<head->biHeight;++i)
			for(j=0;j<head->biWidth;++j)
					img->imageData[(head->biHeight-1-i)*img->widthStep + j + 0]
						= data[i*lw + j*3 + 0];
		return true;	
	}
	catch (...)
	{
		ASSERT(NULL);
		return false;	
	}
}

bool Bmp2Jpg(LPBITMAPINFOHEADER head,LPBYTE data, BYTE* &lpJpgBuf, DWORD &nSize)
{
	try{
		int width    = head->biWidth;
		int height   = head->biHeight;
		int nchannels= head->biBitCount / 8;
		IplImage *Img = NULL;
		Img = cvCreateImage(cvSize(width, height), 8, 3);
		int i,j;
		int lw = WIDTHBYTES(head->biBitCount*head->biWidth);
		for(i=0;i<height;++i)
			for(j=0;j<width;++j)
			{
				Img->imageData[(height-1-i)*Img->widthStep+j*3+0] = data[i*lw+nchannels*j+0];
				Img->imageData[(height-1-i)*Img->widthStep+j*3+1] = data[i*lw+nchannels*j+1];
				Img->imageData[(height-1-i)*Img->widthStep+j*3+2] = data[i*lw+nchannels*j+2];
			}
		//取得运行程序的绝对路径 
		CString  strPath(__argv[0]);
		strPath  =  strPath.Left(strPath.ReverseFind('\\')); 
		CString sFile(strPath+"\\TempJpgFile.jpg");
		cvSaveImage(sFile, Img);

		try{
			
			CFile  jpgFile(sFile, CFile::modeRead);
			nSize = jpgFile.GetLength();	
			
			lpJpgBuf = new BYTE[nSize];
			jpgFile.Read(lpJpgBuf, nSize);
			jpgFile.Close();
		}
		catch( CFileException e) {
			CString ErrorInfoStr;
			ErrorInfoStr.Format("文件"+e.m_strFileName+"读入失败！");
			AfxMessageBox(ErrorInfoStr);
		}
		CFile::Remove( sFile );			
		return true;
	}
	catch(...) 
	{
		ASSERT(NULL);
		return false;
	}
}

bool Jpg2Bmp(BYTE* lpJpgBuf, DWORD nBufSize, LPBITMAPINFOHEADER &head, LPBYTE &data)
{
	try{
		//取得运行程序的绝对路径 
		CString  strPath(__argv[0]);
		strPath  =  strPath.Left(strPath.ReverseFind('\\')); 
		CString sFile(strPath+"\\TempJpgFile.jpg");

		try{	
			CFile  jpgFile(sFile, CFile::modeWrite|CFile::modeCreate);	
			jpgFile.Write(lpJpgBuf, nBufSize);
			jpgFile.Flush();
			jpgFile.Close();
			IplImage *Img = NULL;
			Img = cvLoadImage(sFile, CV_LOAD_IMAGE_ANYCOLOR);
			
			int width    = Img->width;
			int height   = Img->height;
			int nchannels= Img->nChannels;

			head = new BITMAPINFOHEADER;
			head->biBitCount = nchannels * 8;
			head->biHeight   = height;
			head->biWidth    = width;
			head->biSizeImage= WIDTHBYTES(head->biBitCount*width)*height;
			head->biPlanes = 1;
			head->biXPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSX);
			head->biYPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSY);
			head->biCompression=0;
			head->biClrUsed=0;
			head->biClrImportant=0;
			head->biSize  = sizeof(BITMAPINFOHEADER);
			
			data = new BYTE[head->biSizeImage];
			
			int i,j;
			int lw = WIDTHBYTES(head->biBitCount*head->biWidth);
			for(i=0;i<Img->height;++i)
				for(j=0;j<Img->width;++j)
				{
					data[(Img->height-1-i)*lw+nchannels*j+0] = Img->imageData[i*Img->widthStep+nchannels*j+0];
					data[(Img->height-1-i)*lw+nchannels*j+1] = Img->imageData[i*Img->widthStep+nchannels*j+1];
					data[(Img->height-1-i)*lw+nchannels*j+2] = Img->imageData[i*Img->widthStep+nchannels*j+2];
				}
				
				
				CFile::Remove(sFile);
				return true;
			}
			catch(CFileException e)
			{
				CString ErrorInfoStr;
				ErrorInfoStr.Format("文件"+e.m_strFileName+"写入失败！");
				AfxMessageBox(ErrorInfoStr);
				return false;
			}


	}
	catch(...) 
	{
		ASSERT(NULL);
		return false;
	}
}