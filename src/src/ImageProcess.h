#ifndef _IMAGE_PROCESS
#define _IMAGE_PROCESS

void Rgb2Gray(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);

void ThresholdImage(LPBITMAPINFOHEADER lpBMIHPro, LPBYTE lpImagePro, int thrd);
int  OnImageOtsu(LPBITMAPINFOHEADER lpBMIHPro,LPBYTE lpImagePro);

void MediaFilter(LPBITMAPINFOHEADER m_lpBMIHPro, LPBYTE m_lpImagePro);
void MeanFilter(LPBITMAPINFOHEADER m_lpBMIHPro, LPBYTE m_lpImagePro);
bool NewDIBImage(LPBITMAPINFOHEADER& head,LPBYTE& data,int w,int h,int chanels);
bool DeleteDIBImage(LPBITMAPINFOHEADER& head,LPBYTE& data);

void ClipBmp(LPBITMAPINFOHEADER& head,LPBYTE& data, int cw, int ch);

void showImage(const char*str, IplImage *SrcImg, const int t=0);

#endif


#ifndef WIDTHBYTES
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#endif