
#ifndef  _IMAGE_CONVERT
#define  _IMAGE_CONVERT
bool IplImageTODib(IplImage* img,LPBITMAPINFOHEADER head,LPBYTE data);
bool IplImage1TODib4(IplImage* Img, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);
bool IplImage1TODib3(IplImage* Img, LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData);
bool DibToIplImage(LPBITMAPINFOHEADER head,LPBYTE data, IplImage* img);
bool Dib4ToIplImage1(LPBITMAPINFOHEADER head,LPBYTE data, IplImage* img);
bool Dib3ToIplImage1(LPBITMAPINFOHEADER head,LPBYTE data, IplImage* img);
bool Bmp2Jpg(LPBITMAPINFOHEADER head,LPBYTE data, BYTE* &lpJpg, DWORD &nSize);
bool Jpg2Bmp(BYTE* lpJpgBuf, DWORD nBufSize, LPBITMAPINFOHEADER &head, LPBYTE &data);
#endif


#ifndef WIDTHBYTES
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#endif
