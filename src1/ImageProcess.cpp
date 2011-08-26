#include "stdafx.h"
#include "ImageProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void MediaFilter(LPBITMAPINFOHEADER m_lpBMIHPro, LPBYTE m_lpImagePro)
{
	long lw=WIDTHBYTES((m_lpBMIHPro->biWidth)*32);//每行的字节数
	unsigned char a[9];
	int i,j;
	int m,n;
	int k;
    unsigned char* p_temp=new unsigned char[m_lpBMIHPro->biSizeImage];
	memcpy(p_temp,m_lpImagePro,m_lpBMIHPro->biSizeImage);
	for(j=1;j<m_lpBMIHPro->biHeight-1;j++)
		for(i=1;i<m_lpBMIHPro->biWidth-1;i++)
		{
			a[0]=*(m_lpImagePro+(j-1)*lw+(i-1)*4);
			a[1]=*(m_lpImagePro+(j-1)*lw+i*4);
			a[2]=*(m_lpImagePro+(j-1)*lw+(i+1)*4);
			a[3]=*(m_lpImagePro+j*lw+(i-1)*4);
			a[4]=*(m_lpImagePro+j*lw+i*4);
			a[5]=*(m_lpImagePro+j*lw+(i+1)*4);
			a[6]=*(m_lpImagePro+(j+1)*lw+(i-1)*4);
			a[7]=*(m_lpImagePro+(j+1)*lw+i*4);
			a[8]=*(m_lpImagePro+(j+1)*lw+(i+1)*4);
			
			for(m=0;m<9;m++)
				for(n=m+1;n<9;n++)
				{
					if(a[m]>a[n])
					{
						k=a[m];
						a[m]=a[n];
						a[n]=k;
					}
				}
				*(p_temp+j*lw+i*4)=a[4];
				*(p_temp+j*lw+i*4+1)=a[4];
				*(p_temp+j*lw+i*4+2)=a[4];
				*(p_temp+j*lw+i*4+3)=0;
		}
		memcpy(m_lpImagePro,p_temp,m_lpBMIHPro->biSizeImage);
		delete []p_temp;		

}

void MeanFilter(LPBITMAPINFOHEADER m_lpBMIHPro, LPBYTE m_lpImagePro)
{
	
	unsigned char a[9];
	int b[9]={1,2,1,
			  2,4,2,
			  1,2,1};//定义模板
	int i,j;
	int k;
	int temp_value;
	int lw = WIDTHBYTES((m_lpBMIHPro->biWidth)*32);
    unsigned char* p_temp=new unsigned char[m_lpBMIHPro->biSizeImage];
	memcpy(p_temp,m_lpImagePro,m_lpBMIHPro->biSizeImage);
	for(j=1;j<m_lpBMIHPro->biHeight-1;j++)
		for(i=1;i<m_lpBMIHPro->biWidth-1;i++)
		{
			temp_value = 0;
			a[0]=*(m_lpImagePro+(j-1)*lw+(i-1)*4);
			a[1]=*(m_lpImagePro+(j-1)*lw+i*4);
			a[2]=*(m_lpImagePro+(j-1)*lw+(i+1)*4);
			a[3]=*(m_lpImagePro+j*lw+(i-1)*4);
			a[4]=*(m_lpImagePro+j*lw+i*4);
			a[5]=*(m_lpImagePro+j*lw+(i+1)*4);
			a[6]=*(m_lpImagePro+(j+1)*lw+(i-1)*4);
			a[7]=*(m_lpImagePro+(j+1)*lw+i*4);
			a[8]=*(m_lpImagePro+(j+1)*lw+(i+1)*4);
			
			for(k=0; k<9; k++)
				temp_value += a[k]*b[k];

			temp_value = temp_value / 16;
			*(p_temp+j*lw+i*4)=temp_value;
			*(p_temp+j*lw+i*4+1)=temp_value;
			*(p_temp+j*lw+i*4+2)=temp_value;
			*(p_temp+j*lw+i*4+3)=0;
		}
		memcpy(m_lpImagePro,p_temp,m_lpBMIHPro->biSizeImage);
		delete []p_temp;
}

void Rgb2Gray(LPBITMAPINFOHEADER lpBmInfo, LPBYTE lpBmData)
{
    LPBYTE lpSrc;
	long lw = WIDTHBYTES(lpBmInfo->biWidth*lpBmInfo->biBitCount);//每行的字节数
	for(long i=0; i<lpBmInfo->biHeight; i++)
	{
		for(long j=0; j<lpBmInfo->biWidth; j++)
		{
			lpSrc = lpBmData + i*lw + j*4;
			unsigned char gray = (0.114*(lpSrc[0])
								 +0.587*(lpSrc[1])
								 +0.299*(lpSrc[2]) + 0.5);
			lpSrc[0] = gray;
			lpSrc[1] = gray;
			lpSrc[2] = gray;
			lpSrc[3] = 0;
		}
	}
}

void ThresholdImage(LPBITMAPINFOHEADER lpBMIHPro, LPBYTE lpImagePro, int thrd)
{
	//利用自动求得的阈值来二值化图像
	unsigned char* pData = new unsigned char[lpBMIHPro->biSizeImage];
	long lw=WIDTHBYTES((lpBMIHPro->biWidth)*32); //每行的字节数

	for(int j=1;j<=lpBMIHPro->biHeight;j++)
	{
		for(int i=1;i<=lpBMIHPro->biWidth;i++)
		{
			long jj=(j-1)*lw;
			long ii=4*(i-1);
			if(*(lpImagePro+jj+ii)>thrd)
			{
				*(pData+jj+ii) = 255;
				*(pData+jj+ii+1) = 255;
				*(pData+jj+ii+2) = 255;
				*(pData+jj+ii+3) = 0;
			}
			else
			{
				*(pData+jj+ii) = 0;
				*(pData+jj+ii+1) = 0;
				*(pData+jj+ii+2) = 0;
				*(pData+jj+ii+3) = 0;
			}
		}
	}	
	memcpy(lpImagePro,pData,lpBMIHPro->biSizeImage);//象素信息
	delete []pData;
}

int OnImageOtsu(LPBITMAPINFOHEADER lpBMIHPro, LPBYTE lpImagePro)
{
	// start=clock();
	BYTE *lpSrc;
	int thresholdValue=1; // 阈值
	int ihist[256];         // 图像直方图，256个点
	int i,j,k;       // various counters
	int n,n1,n2;
	double m,m1,m2,sum,csum,fmax,sb;
	long lw=WIDTHBYTES((lpBMIHPro->biWidth)*32);//每行的字节数
	// 对直方图置零...
	memset(ihist,0,sizeof(ihist));
	// 生成直方图
	for (i=0;i<lpBMIHPro->biHeight;i++)
		for (j=0;j<lpBMIHPro->biWidth;j++) 
		{
			lpSrc=(unsigned char*)lpImagePro+lw*i+j*4;
			// 计数加1
			ihist[*(lpSrc)]++;
		}
		// set up everything
		sum=csum=0.0;
		n=0;
		for(k=0;k<=255;k++) 
		{
			sum+=(double)k*(double)ihist[k];   /* x*f(x) 质量矩*/
			n+=ihist[k];                           /* f(x)   质量   */
		}
		m=sum/n;
		// do the otsu global thresholding method
		fmax=-1.0;
		n1=0;
		for(k=0;k<255;k++) 
		{
			n1+=ihist[k];
			if(!n1) 
				continue;
			n2=n-n1;
			if(n2==0) 
				break;
			csum+=(double)k*ihist[k];
			m1=csum/n1;
			m2=(sum-csum) / n2;
			sb=(double)n1*(m1-m)*(m1-m)+(double)n2*(m2-m)*(m2-m);
			if(sb>fmax) 
			{
				fmax=sb;
				thresholdValue=k;
			}
		}
		// at this point we have our thresholding value
		return thresholdValue;
}


bool NewDIBImage(LPBITMAPINFOHEADER& head,LPBYTE& data,int w,int h,int chanels)
{
	try
	{
		head=new BITMAPINFOHEADER;
		ASSERT(head != NULL);
		head->biBitCount = 8 * chanels;
		head->biHeight   = h;
		head->biWidth    = w;
		head->biSizeImage= WIDTHBYTES(head->biBitCount*w)*h;
		head->biPlanes = 1;
		head->biXPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSX);
		head->biYPelsPerMeter = 0; //GetDeviceCaps(GetDC(GetDesktopWindow()),LOGPIXELSY);
		head->biCompression=0;
		head->biClrUsed=0;
		head->biClrImportant=0;
		head->biSize  = sizeof(BITMAPINFOHEADER);
		data = new BYTE[head->biSizeImage];
		return true;
	}
	catch (...)
	{
		ASSERT(NULL);
		return false;
	}

}
bool DeleteDIBImage(LPBITMAPINFOHEADER& head,LPBYTE& data)
{
	try
	{
		delete []head;
		delete []data;
		head = NULL;
		data = NULL;
		return true;
	}
	catch (...)
	{
		ASSERT(NULL);
		return false;
	}
}

void ClipBmp(LPBITMAPINFOHEADER& head,LPBYTE& data, int cw, int ch)
{
	long w = head->biWidth;
	long h = head->biHeight;
	long l = WIDTHBYTES(w*head->biBitCount);
	long nw = w - 2*cw;
	long nh = h - 2*ch;
	long nl = WIDTHBYTES(nw*head->biBitCount);
	head->biHeight = nh;
	head->biWidth  = nw;
	head->biSizeImage = nl*nh;

	LPBYTE buf = new BYTE[head->biSizeImage];
	int i;
	for(i=0; i<h; i++)
	{
		memcpy(buf+i*nl, data+i*l+cw*4, nw*4);
	}
	delete []data;
	data = buf;
}

void showImage(const char*str, IplImage *SrcImg, const int t)
{
    cvNamedWindow(str);
	cvShowImage(str, SrcImg);
	cvWaitKey(t);
	cvDestroyWindow(str);
}
