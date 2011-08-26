// CSingleSpermRecognition.cpp : implementation file
//
/*******************************************************
*                                                      *
*  	SingleSpermRecognition.cpp                         *
*	author: NPhard                                     *
*	Email: happy_np@163.com                            *
*   2007/10/24                                         *
*   精子形态学分析： 单个精子形态学分割计算            *
*                                                      *
*******************************************************/

#include "stdafx.h"

#include "SingleSpermRecognition.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int gThreshAdjustPart = 35;

/////////////////////////////////////////////////////////////////////////////
// CSingleSpermRecognition dialog

CSingleSpermRecognition::CSingleSpermRecognition(
							LPBITMAPINFOHEADER  lpBmInfoSrc, 
							LPBYTE lpBmDataSrc, 
							vector<POINT>& RegionPoints,
							int r)
{

    m_lpDibIHPro     = NULL;
    m_lpDibDataPro   = NULL;

    m_lpThrSegSavedIHPro   = NULL;
    m_lpThrSegSavedDataPro = NULL;

	m_lpDibIHPro   = NULL;
	m_lpDibDataPro = NULL;
    m_smSpermBIHPro = NULL;
	m_smSpermBDPro  = NULL;

    m_smSpermBIHPro = (LPBITMAPINFOHEADER)new unsigned char[sizeof(BITMAPINFOHEADER)];
    memcpy(m_smSpermBIHPro, lpBmInfoSrc, sizeof(BITMAPINFOHEADER));
	m_smSpermBDPro  = (LPBYTE)new unsigned char[lpBmInfoSrc->biSizeImage];
	memcpy(m_smSpermBDPro, lpBmDataSrc, lpBmInfoSrc->biSizeImage);

	m_vRegionPoints = RegionPoints;

	m_CenterPos = CPoint(r,r);
	m_visit = new BOOL*[2*r];
	for(int k=0; k<2*r; k++)
	{
		m_visit[k] = new BOOL[2*r];
		memset(m_visit[k], FALSE, sizeof(BOOL)*2*r );
	}
    m_ProlateTip[0] = CPoint(-1,-1);
	m_ProlateTip[1] = CPoint(-1,-1);
		
	m_MinorAxisTip[0] = CPoint(-1,-1);
	m_MinorAxisTip[1] = CPoint(-1,-1);
    m_TailTip = CPoint(-1,-1);          

}

CSingleSpermRecognition::~CSingleSpermRecognition()
{	
	delete []m_lpThrSegSavedIHPro;
	m_lpThrSegSavedIHPro = NULL;
		
	delete []m_lpThrSegSavedDataPro;
	m_lpThrSegSavedDataPro = NULL;

	delete []m_lpDibIHPro;
	m_lpDibIHPro = NULL;

	delete []m_lpDibDataPro;
	m_lpDibDataPro = NULL;

	delete []m_smSpermBIHPro;
    m_smSpermBIHPro = NULL;
  
	delete []m_smSpermBDPro;
	m_smSpermBDPro = NULL;

	for(int k=0; k<2*m_CenterPos.y; k++)
		delete []m_visit[k];
	delete []m_visit;

}

/////////////////////////////////////////////////////////////////////////////

void CSingleSpermRecognition::OnGray() 
{	

	long  Width = m_smSpermBIHPro->biWidth;
	long  Height= m_smSpermBIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_smSpermBIHPro->biBitCount); 
	int i, j;
	LPBYTE lpSrc;
    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
		   lpSrc = m_smSpermBDPro+ i*ByteWidth + j*3; 
           unsigned char grayValue = (unsigned char)(0.299*lpSrc[0] + 0.587*lpSrc[1] + 0.114*lpSrc[2]);			
		   *(lpSrc+ 0) = grayValue;
		   *(lpSrc+ 1) = grayValue;
		   *(lpSrc+ 2) = grayValue;
		}    
	}

}

void CSingleSpermRecognition::MeanFilter() // 均值滤波(高斯平滑)
{
	unsigned char a[9];
	int b[9] = {1,2,1,2,4,2,1,2,1}; //定义高斯模板
	int i,j;
	int k;
	int temp_value;

	long  Width = m_smSpermBIHPro->biWidth;
	long  Height= m_smSpermBIHPro->biHeight;
	long  lw = WIDTHBYTES(Width * m_smSpermBIHPro->biBitCount); 

    unsigned char* p_temp=new unsigned char[m_smSpermBIHPro->biSizeImage];
	memcpy(p_temp, m_smSpermBDPro, m_smSpermBIHPro->biSizeImage);

	for(j=1; j<Height; j++)
		for(i=1; i<Width; i++)
		{
			temp_value = 0;
			a[0] = *(m_smSpermBDPro+ (j-1)*lw+ (i-1)*3);
			a[1] = *(m_smSpermBDPro+ (j-1)*lw+ (i-0)*3);
			a[2] = *(m_smSpermBDPro+ (j-1)*lw+ (i+1)*3);
			a[3] = *(m_smSpermBDPro+ (j-0)*lw+ (i-1)*3);
			a[4] = *(m_smSpermBDPro+ (j-0)*lw+ (i-0)*3);
			a[5] = *(m_smSpermBDPro+ (j-0)*lw+ (i+1)*3);
			a[6] = *(m_smSpermBDPro+ (j+1)*lw+ (i-1)*3);
			a[7] = *(m_smSpermBDPro+ (j+1)*lw+ (i-0)*3);
			a[8] = *(m_smSpermBDPro+ (j+1)*lw+ (i+1)*3);
			
			for(k=0; k<9; k++)
				temp_value += a[k]*b[k];

			temp_value = temp_value /16;
			*(p_temp+(j-1)*lw+(i-1)*3+0) = temp_value;
			*(p_temp+(j-1)*lw+(i-1)*3+1) = temp_value;
			*(p_temp+(j-1)*lw+(i-1)*3+2) = temp_value;

		}
		memcpy(m_smSpermBDPro, p_temp, m_smSpermBIHPro->biSizeImage);
		delete []p_temp;
}

void CSingleSpermRecognition::MediaFilter() // 中值滤波
{

	long  Width = m_smSpermBIHPro->biWidth;
	long  Height= m_smSpermBIHPro->biHeight;
	long  lw = WIDTHBYTES(Width * m_smSpermBIHPro->biBitCount); //每行的字节数

	unsigned char a[9];
	int i, j;
	int m, n;
	int k;

    unsigned char* p_temp = new unsigned char[m_smSpermBIHPro->biSizeImage];
	memcpy(p_temp, m_smSpermBDPro, m_smSpermBIHPro->biSizeImage);

	for(j=1; j<Height; j++)
	{
		for(i=1; i<Width; i++)
		{
			a[0]=*(m_smSpermBDPro+ (j-1)*lw+ (i-1)*3);
			a[1]=*(m_smSpermBDPro+ (j-1)*lw+ (i-0)*3);
			a[2]=*(m_smSpermBDPro+ (j-1)*lw+ (i+1)*3);
			a[3]=*(m_smSpermBDPro+ (j-0)*lw+ (i-1)*3);
			a[4]=*(m_smSpermBDPro+ (j-0)*lw+ (i-0)*3);
			a[5]=*(m_smSpermBDPro+ (j-0)*lw+ (i+1)*3);
			a[6]=*(m_smSpermBDPro+ (j+1)*lw+ (i-1)*3);
			a[7]=*(m_smSpermBDPro+ (j+1)*lw+ (i-0)*3);
			a[8]=*(m_smSpermBDPro+ (j+1)*lw+ (i+1)*3);
			
			for(m=0; m<9; m++)
			{
				for(n=m+1; n<9; n++)
				{
					if(a[m]>a[n])
					{
					     k = a[m];
					  a[m] = a[n];
					  a[n] = k;
					}
				}
			}

			*(p_temp+(j-1)*lw+(i-1)*3+0) = a[4];
			*(p_temp+(j-1)*lw+(i-1)*3+1) = a[4];
			*(p_temp+(j-1)*lw+(i-1)*3+2) = a[4];

		}
	}

	memcpy(m_smSpermBDPro, p_temp, m_smSpermBIHPro->biSizeImage);
	delete []p_temp;
		
}

void CSingleSpermRecognition::SaveGrayDib()
{

	// 保存结果
	if(m_lpDibIHPro == NULL)
	{
		m_lpDibIHPro   = (LPBITMAPINFOHEADER)new unsigned char[sizeof(BITMAPINFOHEADER)];
    	memcpy(m_lpDibIHPro, m_smSpermBIHPro, sizeof(BITMAPINFOHEADER));
	    m_lpDibDataPro = new unsigned char[m_smSpermBIHPro->biSizeImage];
	}
    memcpy(m_lpDibIHPro, m_smSpermBIHPro, sizeof(BITMAPINFOHEADER));
    memcpy(m_lpDibDataPro, m_smSpermBDPro, m_smSpermBIHPro->biSizeImage);

}

//void CSingleSpermRecognition::OnOtsu() 
//{
//	LPBYTE lpSrc;
//	int   thresholdValue = 1; // 阈值
//	int   ihist[256];         // 图像直方图，256个点
//	int   i, j, k;              // various counters
//	int   n, n1, n2;
//	double m, m1, m2, sum, csum, fmax, sb;
//
//	long  Width = m_lpDibIHPro->biWidth;
//	long  Height= m_lpDibIHPro->biHeight;
//	long  lw = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount);  //每行的字节数
//
//	// 对直方图置零...
//	memset(ihist, 0, sizeof(ihist));
//
//	// 生成直方图
//	for (i=0; i<Height; i++)
//	{
//		for (j=0; j<Width; j++) 
//		{
//			lpSrc = (unsigned char*)m_lpDibDataPro+ lw*i+ j*3;
//
//			// 计数加1
//			ihist[*(lpSrc)]++;
//		}
//	}
//
//	// set up everything
//	sum  = 0.0;
//	csum = 0.0;
//	   n = 0;
//	for(k=0; k<=255; k++) 
//	{
//		sum += (double)k*(double)ihist[k];       /* x*f(x) 质量矩 */
//		  n += ihist[k];                         /* f(x)   质量   */
//	}
//	m = sum/n;
//
//	// do the otsu global thresholding method
//	fmax = -1.0;
//	  n1 = 0;
//	for(k=0; k<255; k++) 
//	{
//		n1 += ihist[k];
//		if(!n1) 
//			continue;
//		n2 = n-n1;
//		if(n2 == 0) 
//			break;
//		csum += (double)k*ihist[k];
//		m1 = csum/n1;
//		m2 = (sum-csum) / n2;
//
//		//	sb=(double)n1*(double)n2*(m1-m2)*(m1-m2);
//		sb = (double)n1*(m1-m)*(m1-m) + (double)n2*(m2-m)*(m2-m);
//
//		/* bbg: note: can be optimized. */
//		if(sb > fmax) 
//		{
//			fmax = sb;
//			thresholdValue = k;
//		}
//	}
//
//	// at this point we have our thresholding value
//	m_iThreshold = thresholdValue;//保存阈值
//
//}

// 二值化
void CSingleSpermRecognition::OnSegment() 
{
	int   i,n;
 	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 
    LPBYTE  lpSrc;

	memset(m_lpDibDataPro, 255, m_lpDibIHPro->biSizeImage);

	n = m_vRegionPoints.size();
    int col, row;
	for(i=0; i<n; ++i)	
	{       
	    row = m_vRegionPoints[i].x;
		col = m_vRegionPoints[i].y; 
		lpSrc = m_lpDibDataPro+ row*ByteWidth + col*3;
		*(lpSrc+0) = 0;
		*(lpSrc+1) = 0;
		*(lpSrc+2) = 0;		    
	}

}

void CSingleSpermRecognition::SaveSegDib()
{

	// 保存结果
	m_lpThrSegSavedIHPro   = (LPBITMAPINFOHEADER)new unsigned char[sizeof(BITMAPINFOHEADER)];
	memcpy(m_lpThrSegSavedIHPro, m_lpDibIHPro, sizeof(BITMAPINFOHEADER));
	m_lpThrSegSavedDataPro = new unsigned char[m_lpThrSegSavedIHPro->biSizeImage];
    memcpy(m_lpThrSegSavedDataPro, m_lpDibDataPro, m_lpThrSegSavedIHPro->biSizeImage);

}

void CSingleSpermRecognition::OnErosion() // 腐蚀运算
{
	// TODO: Add your command handler code here
	
    // 指向源图像的指针
	unsigned char* lpSrc;

	// 指向缓存图像的指针
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
    
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);
	
	// 长，宽，每行字节数  
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	// 腐蚀结构模板
    int structure[12][12] = { 0,0,1,1,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,1,1,0,0 };

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	//使用自定义的结构元素进行腐蚀
	for(j = 0; j <Height-12; j++)
	{
		for(i = 0;i <Width-12; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc =m_lpDibDataPro+ j*ByteWidth+ i*3;

			// 指向目标图像倒数第j行，第i个象素的指针
			lpDst =lpTemp+ j*ByteWidth+ i*3;

			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
		
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
			   return ;

			//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
			//则将目标图像中的当前点赋成白色
			//注意在DIB图像中内容是上下倒置的
			for (m = 0; m<12; m++ )
			{
				for (n = 0; n<12; n++)
				{   
					if(structure[m][n]==0)
					   continue;
					pixel = *(lpSrc + m*ByteWidth + n*3);
					if(pixel == 255)
					{	
                        int a,b;
						for (a = 0; a<12; a++ )
						{
							for (b = 0; b<12; b++)
							{  								
								*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)255;
								*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)255;
								*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)255;
							}
						}
					    goto nextpiexl;
					}
				}
			}
nextpiexl:;
		}
	}
    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete []lpTemp;

}

void CSingleSpermRecognition::OnExpand() // 膨胀运算
{
	// TODO: Add your command handler code here

    // 指向源图像的指针
	unsigned char* lpSrc;

	// 指向缓存图像的指针
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
    
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);
	
	// 长，宽，每行字节数  
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 
	
	// 腐蚀结构模板
	int structure[12][12] = { 0,0,1,1,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,1,1,0,0 };

	//循环变量
	long i;
	long j;
	int  n;
	int  m; 

	//像素值
	unsigned char pixel;

	//使用自定义的结构元素进行膨胀
	for(j = 0; j <Height-12; j++)
	{
		for(i = 0;i <Width-12; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc =m_lpDibDataPro+ j*ByteWidth+ i*3;

			// 指向目标图像倒数第j行，第i个象素的指针
			lpDst =lpTemp+ j*ByteWidth+ i*3;

			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;
		
			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && pixel != 0)
			   return ;

			//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
			//则将目标图像中的当前点赋成白色
			//注意在DIB图像中内容是上下倒置的
			for (m = 0;m < 12;m++ )
			{
				for (n = 0;n < 12;n++)
				{   
					if(structure[m][n]==0)
					   continue;
					pixel = *(lpSrc + m*ByteWidth + n*3);
					if(pixel == 0 ) 
					{	
                        int a,b;
						for(a = 0; a < 12; a++ )
						{
							for(b = 0; b < 12; b++)
							{   
                              if( *(m_lpThrSegSavedDataPro+ (j+a)*ByteWidth + (i+b)*3 +0) == 0)
							  {
								*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)0;
								*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)0;
								*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)0;
							  }
							}
						}

					    goto nextpiexl;

					}
				}
			}
nextpiexl:;
		}
	}

    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete []lpTemp;

}


void CSingleSpermRecognition::OnTagOtsu()   // 顶体和头部分割
{
	BYTE *lpSrc;
	int   thresholdValue = 1; // 阈值
	int   ihist[256];         // 图像直方图，256个点
	int   i, j, k;            // various counters
	int   n, n1, n2;
	double m, m1, m2, sum, csum, fmax, sb;
	long  lw = WIDTHBYTES(m_lpDibIHPro->biWidth * m_lpDibIHPro->biBitCount); //每行的字节数

	// 对直方图置零...
	memset(ihist, 0, sizeof(ihist));

	// 生成直方图
	for (i=0; i<m_lpDibIHPro->biHeight; i++)
	{
		for (j=0; j<m_lpDibIHPro->biWidth; j++) 
		{
			lpSrc = (unsigned char*)m_lpDibDataPro+ lw*i+ j*3; // 

			// 计数加1
			if(*lpSrc == 0)  
			ihist[*(m_smSpermBDPro+ lw*i+ j*3)]++;
		}
	}

	// set up everything
	sum  = 0.0;
	csum = 0.0;
	   n = 0;
	for(k=0; k<=255; k++) 
	{
		sum += (double)k*(double)ihist[k];       /* x*f(x) 质量矩 */
		  n += ihist[k];                         /* f(x)   质量   */
	}
	m = sum/n;

	// do the otsu global thresholding method
	fmax = -1.0;
	  n1 = 0;
	for(k=0; k<255; k++) 
	{
		n1 += ihist[k];
		if(!n1) 
			continue;
		n2 = n-n1;
		if(n2 == 0) 
			break;
		csum += (double)k*ihist[k];
		m1 = csum/n1;
		m2 = (sum-csum) / n2;

		//	sb=(double)n1*(double)n2*(m1-m2)*(m1-m2);
		sb = (double)n1*(m1-m)*(m1-m) + (double)n2*(m2-m)*(m2-m);

		/* bbg: note: can be optimized. */
		if(sb > fmax) 
		{
			fmax = sb;
			thresholdValue = k;
		}
	}

	// at this point we have our thresholding value
	m_iThresholdFH = thresholdValue;//保存阈值

}


void CSingleSpermRecognition::OnTagThreshold() // 分割头部和顶体
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	OnTagOtsu();

    // 阈值分割
	int i, j;
	LPBYTE  lpSrc;
	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			if(*(m_lpDibDataPro+ i*ByteWidth+ j*3+ 0) == 0) // 对目标再次分割
			{

			   lpSrc = m_smSpermBDPro+ i*ByteWidth + j*3; 
			   if( *(lpSrc+ 0) < m_iThresholdFH - gThreshAdjustPart ) // 阈值修正
			   {
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 0) = 255;  // 蓝色头部
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 1) = 0;
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 2) = 0;	 	
			   }
			   else{			   
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 0) = 0;    // 红色顶体和中断
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 1) = 0;
				   *(m_lpDibDataPro+ i*ByteWidth+ j*3+ 2) = 255;		   
			   }  
			}
		}    
	}

}

void CSingleSpermRecognition::OnExpandHead()	//头部膨胀运算
{
	// TODO: Add your command handler code here

    // 指向源图像的指针
	unsigned char* lpSrc;

	// 指向缓存图像的指针
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
    
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);
	
	//    
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 
	
	int structure[10][10] = { 0,0,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,0,0 };


	// 循环变量
	long i;
	long j;
	int  n;
	int  m;

	// 使用自定义的结构元素进行膨胀
	for(j = 0; j <Height-10; j++)
	{
		for(i = 0;i <Width-10; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc =m_lpDibDataPro+ j*ByteWidth+ i*3;

			// 指向目标图像倒数第j行，第i个象素的指针
			lpDst =lpTemp+ j*ByteWidth+ i*3;

			for (m = 0;m < 10;m++ )
			{
				for (n = 0;n < 10;n++)
				{   
					if(structure[m][n]==0)
					   continue;
					if( *(lpSrc + m*ByteWidth + n*3 + 0) == 255 &&
						*(lpSrc + m*ByteWidth + n*3 + 1) == 0   &&
					    *(lpSrc + m*ByteWidth + n*3 + 2) == 0 )  // 蓝色
					{	
                        int a,b;
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if( *(lpSrc + a*ByteWidth + b*3 +0) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +1) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +2) == 255) // 红色顶体
								{
									*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)255;
									*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)0;
									*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)0;
								}
							}
						}
					    goto nextpiexl;
					}
				}
			}
nextpiexl:;
		}
	}

    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete lpTemp;

}

//头部腐蚀运算
void CSingleSpermRecognition::OnErosionHead()	
{
	// TODO: Add your command handler code here

    // 指向源图像的指针
	unsigned char* lpSrc;

	// 指向缓存图像的指针
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
    
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);
	
	//    
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int structure[10][10] = { 0,0,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,0,0 };
	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//使用自定义的结构元素进行腐蚀
	for(j = 0; j <Height-10; j++)
	{
		for(i = 0;i <Width-10; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc =m_lpDibDataPro+ j*ByteWidth+ i*3;

			// 指向目标图像倒数第j行，第i个象素的指针
			lpDst =lpTemp+ j*ByteWidth+ i*3;

			for (m = 0;m < 10;m++ )
			{
				for (n = 0;n < 10;n++)
				{   
					if(structure[m][n]==0)
					   continue;
                    int a,b;
					for (a = 0; a < 10; a++ )
					{
						for (b = 0; b < 10; b++)
						{   
						 if(*(lpSrc + a*ByteWidth + b*3 + 0) == 255 &&
							*(lpSrc + a*ByteWidth + b*3 + 1) == 255 &&
							*(lpSrc + a*ByteWidth + b*3 + 2) == 255 ) 
                         goto nextpiexl;
						}
					}

					if( *(lpSrc + m*ByteWidth + n*3 + 0) == 0 &&
						*(lpSrc + m*ByteWidth + n*3 + 1) == 0 &&
					    *(lpSrc + m*ByteWidth + n*3 + 2) == 255 )  // 红色
					{	
                        
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if( *(lpSrc + a*ByteWidth + b*3 +0) == 255 &&
									*(lpSrc + a*ByteWidth + b*3 +1) == 0   &&
									*(lpSrc + a*ByteWidth + b*3 +2) == 0) // 蓝色头部
								{
									*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)0;
									*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)0;
									*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)255;
								}
							}
						}
					    goto nextpiexl;
					}
					if( *(lpSrc + m*ByteWidth + n*3 + 0) == 0   &&
						*(lpSrc + m*ByteWidth + n*3 + 1) == 255 &&
						*(lpSrc + m*ByteWidth + n*3 + 2) == 0 )  // 绿色
					{	
                        int a,b;
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if( *(lpSrc + a*ByteWidth + b*3 +0) == 255 &&
									*(lpSrc + a*ByteWidth + b*3 +1) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +2) == 0) // 蓝色头部
								{
									*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)255;
									*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)0;
									*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)0;
								}
							}
						}
					    goto nextpiexl;
					}
				}
			}
nextpiexl:;
		}
	}

    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete lpTemp;

}


void CSingleSpermRecognition::OnMarkTail() 
{
	// TODO: Add your command handler code here

	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 
	int i, j;
	LPBYTE lpSrc, lpDst;
    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{

		   lpSrc = m_lpThrSegSavedDataPro+ i*ByteWidth + j*3;  // 二值化图像

           if( *(lpSrc +0)==0 &&
			   *(lpSrc +1)==0 &&
			   *(lpSrc +2)==0 ) // 整个黑色目标区域
		   {
			   lpDst = m_lpDibDataPro+ i*ByteWidth + j*3; 
			   
			   if( !( *(lpDst +0)==0 && *(lpDst +1)==0 && *(lpDst +2)==255) && // 红色顶体
                   !( *(lpDst +0)==255 && *(lpDst +1)==0 && *(lpDst +2)==0) )  // 蓝色头部
			   {
                   *(lpDst +0) = (unsigned char)0;
				   *(lpDst +1) = (unsigned char)255;   // 尾部置为绿色
				   *(lpDst +2) = (unsigned char)0;
			   }			   	   
		   }

		}    
	}

}


// 拷贝目标，去掉杂质
void CSingleSpermRecognition::CopySperm(POINT p, LPBYTE lpTemp) 
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int i;
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1,
					};
  
	LPBYTE  lpSrc, lpDst;
	POINT   q;
 	POINT  *PosQ;

	PosQ = new POINT[Width*Height];
    unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
    PosQ[iEnd++] = p;
	m_visit[p.x][p.y] = TRUE;	 

	while( iStart < iEnd ){

		p = PosQ[--iEnd];

 		// Copy Piexl
		lpSrc = m_lpDibDataPro+ p.x*ByteWidth+ p.y*3;
	    lpDst = lpTemp+ p.x*ByteWidth+ p.y*3;
 		*(lpDst +0) = *(lpSrc +0);
	    *(lpDst +1) = *(lpSrc +1);
 		*(lpDst +2) = *(lpSrc +2);
 
 		for(i=0; i<4; i++){
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
 
 			if( q.x<Height && q.x>=0 &&
				q.y<Width  && q.y>=0 && 
				m_visit[q.x][q.y] == FALSE &&  //未访问
				( PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == BLUE_PIEXL || //蓝色
				  PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == GREEN_PIEXL || //绿色
				  PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == RED_PIEXL )  //红色
 			){	
 					m_visit[q.x][q.y] = TRUE; 	
					PosQ[iEnd++] = q;
			}

 		}
 	}

 	delete []PosQ;

}
 

void CSingleSpermRecognition::OnCopySperm() 
{
	// TODO: Add your command handler code here

	// 初始化
	for(int k=0; k<m_lpDibIHPro->biHeight; k++)
		memset(m_visit[k], FALSE, sizeof(BOOL)*m_lpDibIHPro->biWidth);

	LPBYTE lpTemp  = new unsigned char[m_lpDibIHPro->biSizeImage];
    memset(lpTemp, 255, m_lpDibIHPro->biSizeImage);
 
	CopySperm(m_CenterPos, lpTemp);
	
	memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);

	delete []lpTemp;
}

void CSingleSpermRecognition::OnExpandTail()	//尾部膨胀运算
{
	// TODO: Add your command handler code here

    // 指向源图像的指针
	unsigned char* lpSrc;

	// 指向缓存图像的指针
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
    
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);
	
	//    
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

    int structure[10][10] = { 0,0,1,1,1,1,1,1,0,0,
							  0,1,1,1,1,1,1,1,1,0,
							  1,1,1,1,1,1,1,1,1,1,
							  1,1,1,1,1,1,1,1,1,1,
							  1,1,1,1,1,1,1,1,1,1,
							  1,1,1,1,1,1,1,1,1,1,
							  1,1,1,1,1,1,1,1,1,1,
							  1,1,1,1,1,1,1,1,1,1,
							  0,1,1,1,1,1,1,1,1,0,
							  0,0,1,1,1,1,1,1,0,0 };

	// 循环变量
	long i;
	long j;
	int  n;
	int  m;

	// 使用自定义的结构元素进行膨胀
	for(j = 0; j <Height-10; j++)
	{
		for(i = 0;i <Width-10; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc =m_lpDibDataPro+ j*ByteWidth+ i*3;

			// 指向目标图像倒数第j行，第i个象素的指针
			lpDst =lpTemp+ j*ByteWidth+ i*3;

			for (m = 0;m < 10;m++ )
			{
				for (n = 0;n < 10;n++)
				{   
					if(structure[m][n]==0)
					   continue;
					if( *(lpSrc + m*ByteWidth + n*3 + 0) == 0   &&
						*(lpSrc + m*ByteWidth + n*3 + 1) == 255 &&
					    *(lpSrc + m*ByteWidth + n*3 + 2) == 0 )  // 绿色
					{	
                        int a,b;
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if((*(lpSrc + a*ByteWidth + b*3 +0) == 255 &&
									*(lpSrc + a*ByteWidth + b*3 +1) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +2) == 0)   // 蓝色头部
								 ||
								   (*(lpSrc + a*ByteWidth + b*3 +0) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +1) == 0 &&
									*(lpSrc + a*ByteWidth + b*3 +2) == 255) // 红色顶体
								 )
								{
									*(lpDst + a*ByteWidth + b*3 +0) = (unsigned char)0;
									*(lpDst + a*ByteWidth + b*3 +1) = (unsigned char)255;
									*(lpDst + a*ByteWidth + b*3 +2) = (unsigned char)0;
								}
							}
						}
					    goto nextpiexl;
					}
				}
			}
nextpiexl:;
		}
	}

    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete lpTemp;
	
}

int CSingleSpermRecognition::GetRegionArea(POINT p)
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  lw = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int dir[8][2] = {	-1, 0,
						-1, 1,
						 0, 1,
						 1, 1,
						 1, 0,
						 1,-1,
						 0,-1,
						-1,-1				
					};

	int     i, area = 0;
	LPBYTE  lpSrc;

	POINT   q;
 	POINT  *PosQ;

	PosQ = new POINT[Width*Height];
    unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
    PosQ[iEnd++] = p;
	m_visit[p.x][p.y] = TRUE;	 

	while( iStart < iEnd ){

		p = PosQ[--iEnd];
        area++;      
 		for(i=0; i<8; i++){
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
			lpSrc = m_lpDibDataPro+ q.x*lw+ q.y*3;
         	if( q.x<Height && q.x>=0 &&
			    q.y<Width  && q.y>=0 && 
			    FALSE == m_visit[q.x][q.y] &&  
			    RED_PIEXL == PIEXL(lpSrc)    
			){
 					m_visit[q.x][q.y] = TRUE; 	
					PosQ[iEnd++] = q;
			}
 		}
 	}

 	delete []PosQ;
	PosQ = NULL;
	return area;
}

void CSingleSpermRecognition::SearchHole(vector<POINT>& holeSeedArray)  
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  lw = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount);

	vector<Hole>holeArray;

	int   i, j, k;
	LPBYTE lpSrc;
    for(k=0; k<Height; ++k)		   
	   memset(m_visit[k], FALSE, sizeof(BOOL)*Width);

    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{	
		     lpSrc = m_lpDibDataPro + i*lw + j*3;
			 if( m_visit[i][j] == FALSE && PIEXL(lpSrc) == RED_PIEXL ) 
			 {
                 Hole h;
				 h.holeSeed.x = i;
				 h.holeSeed.y = j;
				 h.area = GetRegionArea(h.holeSeed);
				 holeArray.push_back(h);
			 }
		}    
	} 

	int  n = holeArray.size();
	int  MaxArea = -1, MaxK;
	for(k=0; k<n; ++k) {
        if(holeArray[k].area > MaxArea) {
			MaxArea = holeArray[k].area;
	        MaxK    = k;
		}
	}

	for(k=0; k<n; ++k) {
        if( k!= MaxK ) {
            holeSeedArray.push_back(holeArray[k].holeSeed);
		}
	}

}

void CSingleSpermRecognition::OnFillHole(vector<POINT>& SeedPoint)
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int i, k, n = SeedPoint.size();
	int dir[8][2] = {	-1, 0,
						-1, 1,
						 0, 1,
						 1, 1,
						 1, 0,
						 1,-1,
						 0,-1,
						-1,-1					
					};
	
	LPBYTE  lpSrc;
    POINT   p, q;
 	POINT  *PosQ;

	PosQ = new POINT[Width*Height];
    unsigned int iStart,iEnd;
   
	for(k=0; k<Height; k++)		   
	   memset(m_visit[k], FALSE, sizeof(BOOL)*Width);

	for(k=0; k<n; ++k)
	{
		iStart = 0;
		iEnd   = 0;
		p = SeedPoint[k];
		PosQ[iEnd++] = p;
		m_visit[p.x][p.y] = TRUE;	 

		while( iStart < iEnd ){

			p = PosQ[--iEnd];

 			// fill with blue
			lpSrc = m_lpDibDataPro+ p.x*ByteWidth+ p.y*3;
			*(lpSrc +0) = 255;
 			*(lpSrc +1) = 0;
 			*(lpSrc +2) = 0; 
 
 			for(i=0; i<8; i++){
 				q.x = p.x + dir[i][0];
 				q.y = p.y + dir[i][1];
 
 				if( q.x<Height && q.x>=0  &&
					q.y<Width  && q.y>=0  && 
					m_visit[q.x][q.y] == FALSE &&  //未访问
					(PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == RED_PIEXL ) //红色
				) {	
 					  m_visit[q.x][q.y] = TRUE; 	
					  PosQ[iEnd++] = q;
				}
 			}
 		}
	}
 	delete []PosQ;
	PosQ = NULL;
}

// 过滤粘连物
void CSingleSpermRecognition::CopyHead(POINT p, LPBYTE& lpTemp) 
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int i;
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1, 
					};

	LPBYTE  lpSrc, lpDst;
	POINT   q;
 	POINT  *PosQ;

	PosQ = new POINT[Width*Height];
    unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
    PosQ[iEnd++] = p;
	m_visit[p.x][p.y] = TRUE;

	while( iStart < iEnd ){

		p = PosQ[--iEnd];

 		// Copy Piexl
	    lpSrc = m_lpDibDataPro+ p.x*ByteWidth+ p.y*3;
    	lpDst = lpTemp+ p.x*ByteWidth+ p.y*3;
		*(lpDst +0) = *(lpSrc +0);
	    *(lpDst +1) = *(lpSrc +1);
		*(lpDst +2) = *(lpSrc +2);
 
 		for(i=0; i<4; i++){
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
 
			if( q.x<Height && q.x>=0 &&
			    q.y<Width  && q.y>=0 && 
			    m_visit[q.x][q.y] == FALSE &&  // 未访问
			    ( PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == BLUE_PIEXL || // 蓝色
			      PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == RED_PIEXL )   // 红色
			){
 					m_visit[q.x][q.y] = TRUE; 	
					PosQ[iEnd++] = q;
			}
 		}
 	}

 	delete []PosQ;


}

POINT CSingleSpermRecognition::GetSeed(CPoint p) 
{

    long    Width  = m_lpDibIHPro->biWidth;
	long    Height = m_lpDibIHPro->biHeight;
	long    lw     = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int dir[4][2] = {	   
				   -1, 0,
					1, 0,
					0,-1,
					0, 1, 
				};
    int k;
	CPoint  q, resP;
	resP = CPoint(-1,-1);

	CPoint *PosQ;
	PosQ = new CPoint[Width*Height];
	unsigned int iStart, iEnd;
	iStart = 0;
	iEnd   = 0;
	PosQ[iEnd++] = p;
	m_visit[p.x][p.y] = 1;	

    while(iStart < iEnd){
	
		p = PosQ[--iEnd];
		for(k=0; k<4; k++)
		{ 	
			m_visit[p.x][p.y] = TRUE;
			
			// Near Piexl 
			q.x = p.x+dir[k][0];
			q.y = p.y+dir[k][1];

 			if( q.x<Height && q.x>=0 &&
				q.y<Width  && q.y>=0 &&
				m_visit[q.x][q.y] == FALSE /*未访问*/ ){

				if( PIEXL(m_lpDibDataPro+ q.x*lw + q.y*3) == GREEN_PIEXL ){
				    resP = q;
					delete []PosQ;
					PosQ = NULL;
					return resP;
				}
				else if( PIEXL(m_lpDibDataPro+ q.x*lw + q.y*3) == BLUE_PIEXL )
				    PosQ[iEnd++] = q;

			}
		}  
	}

	delete []PosQ;
	PosQ = NULL;
	return resP;

}

void CSingleSpermRecognition::CopyTail(POINT p, LPBYTE& lpTemp)  // 拷贝尾部
{
    long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int i;
	int dir[4][2] = {	   
					   -1, 0,
						1, 0,
						0,-1,
						0, 1, 
					};

	LPBYTE  lpSrc, lpDst;

	POINT   q;
 	POINT  *PosQ;

	PosQ = new POINT[Width*Height];
    unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
    PosQ[iEnd++] = p;
	m_visit[p.x][p.y] = 1;	 

	while( iStart < iEnd ){

		p = PosQ[--iEnd];

 		// Copy Piexl
		lpSrc = m_lpDibDataPro+ p.x*ByteWidth+ p.y*3;
	    lpDst = lpTemp+ p.x*ByteWidth+ p.y*3;
		*(lpDst +0) = *(lpSrc +0);
	    *(lpDst +1) = *(lpSrc +1);
		*(lpDst +2) = *(lpSrc +2);
 
 		for(i=0; i<4; i++){
 			q.x = p.x + dir[i][0];
 		    q.y = p.y + dir[i][1];
 
         	if( q.x<Height && q.x>=0 &&
			    q.y<Width  && q.y>=0 && 
			    m_visit[q.x][q.y] == FALSE &&  //未访问
			    PIEXL(m_lpDibDataPro+ q.x*ByteWidth+ q.y*3) == GREEN_PIEXL   //绿色尾部
			){
 					m_visit[q.x][q.y] = TRUE; 	
					PosQ[iEnd++] = q;
			}
 		}
 	}

 	delete []PosQ;

}

// 
BOOL CSingleSpermRecognition::IsAllPiexlsInLineP_QAreGreen(POINT p, POINT q)
{	
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount);

	// 存放生成的直线上点坐标
	POINT *lineP = new POINT[m_CenterPos.x*2];
	size_t index = 0;
	
	// 直线的Bresenham生成算法
	CPoint S, E;
	S = p;
	E = q;
    double dltY = double(E.y - S.y);
	double dltX = double(E.x - S.x);
	int x, y, d;
	int dx, dy;
	int stepX, stepY;
	stepX = (int)dltX >= 0 ? 1 : -1;
	stepY = (int)dltY >= 0 ? 1 : -1;

	dx = abs(dltX);
	dy = abs(dltY);

	if( dx >= dy ){	
		x = 0;
		y = 0;
		d = 2*x*dy - 2*y*dx + 2*dy - dx; 	 
		for(int i=0; i<dx; ++i)
		{
			lineP[index++] = CPoint(x+S.x, y+S.y);
			if(d>=0){
				y += stepY;
				x += stepX;
				d += 2*dy - 2*dx; 
			}
			else{
				x += stepX;
				d += 2*dy;
			}
		}
	}
	else{
		x = 0;
		y = 0;
		d = 2*y*dx - 2*x*dy + 2*dx - dy; 		 
		for(int i=0; i<dy; ++i)
		{
			lineP[index++] = CPoint(x+S.x, y+S.y);
			if(d>=0){
				y += stepY;
				x += stepX;
				d += 2*dx - 2*dy; 
			}
			else{
				y += stepY;
				d += 2*dx;
			}
		}		
	}	

	BOOL  ans = TRUE;
	for(int i=0; i<index;i++)
	{
		if( PIEXL(m_lpDibDataPro+ lineP[i].x*ByteWidth + lineP[i].y*3) != GREEN_PIEXL )
		{
            ans = FALSE;
			break;
		}
	}
	
	delete []lineP;			
	lineP = NULL;
    return ans;

}

// 处理尾部交叉情况
void CSingleSpermRecognition::HandleTailCross()     
{	
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int i, j, k;
	bool    HasGreen, HasBlue;
	LPBYTE  a[4];
    for(i=1; i<Height; i++)
	{
		for(j=1; j<Width; j++)
		{		        
			// 找出尾部和头部的公共边界 
           	a[0] = m_lpDibDataPro+ (i-1)*ByteWidth + (j-1)*3;
  	        a[1] = m_lpDibDataPro+ (i-1)*ByteWidth + (j+0)*3;
	  	    a[2] = m_lpDibDataPro+ (i+0)*ByteWidth + (j-1)*3;
		  	a[3] = m_lpDibDataPro+ (i+0)*ByteWidth + (j+0)*3;

            HasGreen = false;
			HasBlue  = false;

			for( k =0; k<4; k++)
			{
				if( PIEXL(a[k]) == GREEN_PIEXL ) // 绿色尾部
					HasGreen = true;

				if( PIEXL(a[k]) == BLUE_PIEXL ) // 蓝色头部
				    HasBlue  = true;
			}

         	// 找出尾部和头部的公共边界，存入（Vector）m_HeadTailBoundary
			if( HasGreen && HasBlue )
			{   
				for( k =0; k<4; k++)
				{
					if( PIEXL(a[k]) == GREEN_PIEXL )
					{
					    m_HeadTailBoundary.push_back( CPoint(i+k/2-1, j+k%2-1) );
					}
				}
			} 
        
		}    
	}

	//
	int  n;
	POINT p, q;
	vector<POINT> TailPoints;
	vector<POINT> memoPoints;
	for(i=0; i<Height; i++)
	for(j=0; j<Width; j++)
	    if( PIEXL(m_lpDibDataPro + i*ByteWidth + j*3) == GREEN_PIEXL )
			TailPoints.push_back(CPoint(i,j));

	if( m_HeadTailBoundary.size()>0 )
	{
        long x = 0, y = 0;
		n = m_HeadTailBoundary.size(); 
		for(i=0; i<n; ++i){
		    x += m_HeadTailBoundary[i].x;
		    y += m_HeadTailBoundary[i].y;
		}
		x /= n;
		y /= n;
	    
		long dist, min = 1<<20;
		for(i=0; i<n; ++i){
		    dist = distance(CPoint(x,y), m_HeadTailBoundary[i]);
			if( dist < min ){
				min = dist;
			    p = m_HeadTailBoundary[i];
			}
		}

		// 对尾部每个像素，判断是否要剔除
		n = TailPoints.size(); 
	    for(i=0; i<n; ++i)
		{
           q = TailPoints[i];
           if( !IsAllPiexlsInLineP_QAreGreen(p, q) )
           memoPoints.push_back(q);
		}
	}
	 
	n = memoPoints.size();
	for(i=0; i<n; ++i)
	{
        *(m_lpDibDataPro + memoPoints[i].x*ByteWidth + memoPoints[i].y*3 +0 ) = 255;
        *(m_lpDibDataPro + memoPoints[i].x*ByteWidth + memoPoints[i].y*3 +1 ) = 255;
        *(m_lpDibDataPro + memoPoints[i].x*ByteWidth + memoPoints[i].y*3 +2 ) = 255;
	}

}

// 过滤粘连物
void CSingleSpermRecognition::FilterConglutinate()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount);  

	int k;
    LPBYTE  lpSrc;
	lpSrc = new unsigned char[m_lpDibIHPro->biSizeImage];
	memset(lpSrc, 255, m_lpDibIHPro->biSizeImage);
	
	for(k=0; k<Height; k++)
		memset(m_visit[k], FALSE, sizeof(BOOL)*Height);

    // Search a blue piexl point which is the most closest to the point of m_CenterPos
	int i,j;
	int dist, minD = 1<<20;
	CPoint p = m_CenterPos;
	for(i=0; i<Height; i++){
	    for(j=0; j<Width; j++)
		{
			dist = distance(CPoint(i,j), m_CenterPos);
			if( PIEXL(m_lpDibDataPro + i*ByteWidth + j*3) == BLUE_PIEXL &&
				dist < minD )
			{ 
                minD = dist;
				p = CPoint(i,j);
			}
		}
	}

	CopyHead(p, lpSrc);

	for(k=0; k<2*m_CenterPos.x; k++)
		memset(m_visit[k], FALSE, sizeof(BOOL)*2*m_CenterPos.x);
  
	p = GetSeed(p);
	if( p != CPoint(-1,-1) )
        CopyTail(p, lpSrc);

	int d = 3;
    for(i=0; i<d; i++){
	    for(j=0; j<Width; j++){
           *(lpSrc + i*ByteWidth + j*3 + 0) = 255;
           *(lpSrc + i*ByteWidth + j*3 + 1) = 255;
           *(lpSrc + i*ByteWidth + j*3 + 2) = 255;
		   *(lpSrc + (Height-i-1)*ByteWidth + j*3 + 0) = 255;
           *(lpSrc + (Height-i-1)*ByteWidth + j*3 + 1) = 255;
           *(lpSrc + (Height-i-1)*ByteWidth + j*3 + 2) = 255;
		}
	}
	
	for(j=0; j<d; j++){
		for(i=0; i<Height; i++){
           *(lpSrc + i*ByteWidth + j*3 + 0) = 255;
           *(lpSrc + i*ByteWidth + j*3 + 1) = 255;
           *(lpSrc + i*ByteWidth + j*3 + 2) = 255;
		   *(lpSrc + i*ByteWidth + (Width-j-1)*3 + 0) = 255;
           *(lpSrc + i*ByteWidth + (Width-j-1)*3 + 1) = 255;
           *(lpSrc + i*ByteWidth + (Width-j-1)*3 + 2) = 255;
		}
	}
	memcpy(m_lpDibDataPro, lpSrc, m_lpDibIHPro->biSizeImage);

	delete []lpSrc;
   
} 
  
// 单层细化
void CSingleSpermRecognition::SpermThining()      
{
	
	// 长，宽，每行字节数  
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  lw = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

    LPBYTE a[9];

	BOOL   bIsCopy;

	//循环变量
	long i;
	long j;
	int  k;

	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];   
    memset(lpTemp, 255, m_lpDibIHPro->biSizeImage);
	for(j = 1; j <Height-1; j++)
	{
		for(i = 1;i <Width-1; i++)
		{	
			a[0]=(m_lpDibDataPro + (j-1)*lw+(i-1)*3);
			a[1]=(m_lpDibDataPro + (j-1)*lw+i*3);
			a[2]=(m_lpDibDataPro + (j-1)*lw+(i+1)*3);
			a[3]=(m_lpDibDataPro + j*lw+(i-1)*3);
			a[4]=(m_lpDibDataPro + j*lw+i*3);
			a[5]=(m_lpDibDataPro + j*lw+(i+1)*3);
			a[6]=(m_lpDibDataPro + (j+1)*lw+(i-1)*3);
			a[7]=(m_lpDibDataPro + (j+1)*lw+i*3);
			a[8]=(m_lpDibDataPro + (j+1)*lw+(i+1)*3);

			bIsCopy = TRUE;
            for(k=0; k<9; k++)
				if( PIEXL(a[k]) == RGB(255,255,255) ){
                   bIsCopy = FALSE;
				   break;
				}	
			if(bIsCopy == TRUE){
				*(lpTemp + j*lw+i*3+0) = *(m_lpDibDataPro + j*lw+i*3+0);
				*(lpTemp + j*lw+i*3+1) = *(m_lpDibDataPro + j*lw+i*3+1);
				*(lpTemp + j*lw+i*3+2) = *(m_lpDibDataPro + j*lw+i*3+2);
			}

		}
	}

    memcpy(m_lpDibDataPro, lpTemp, m_lpDibIHPro->biSizeImage);
	delete []lpTemp;

}

/*-----------------------------------------------------*/
/*-----------------形态学分析第一步--------------------*/
/*---------------------形态分割------------------------*/
/*-----------------------------------------------------*/
void CSingleSpermRecognition::OnSperMopySeg() // 形态分割
{
	// TODO: Add your control notification handler code here
        
	// 图像预处理
//     OnGray();          // 灰度化
// 	   MediaFilter();     // 中值去噪
// 	   MeanFilter();      // 高斯去噪
	   SaveGrayDib();     // 保存预处理结果图,备用

    // 整体图像分割
       OnSegment();       // 分割
       SaveSegDib();      // 保存分割后的二值化图像,备用
	   
	   OnErosion();       // 腐蚀

	   OnExpand();        // 膨胀
	   OnExpand();        // 膨胀	
	    
	// 部分图像分割
	   OnTagThreshold();  // 分割顶体和头部

	   OnExpandHead();    // 膨胀头部
	   OnMarkTail();      // 标记尾部
	  
	   OnCopySperm();     // 拷贝图像,合并:顶体,头部,尾部
	  
	   vector<POINT>HoleSeed;
	   SearchHole(HoleSeed);
	   OnFillHole(HoleSeed);
	   
	// 
       OnExpandTail();  // 尾部膨胀

       OnErosionHead(); // 头部腐蚀
	   OnExpandHead();  // 头部膨胀
	   OnErosionHead(); // 头部腐蚀

	   FilterConglutinate();  // 滤掉粘连的其它细胞

	   HandleTailCross();

    // SpermThining();        // 单层细化

	// 将形态分割结果保存在 m_smSperm 中
	memcpy(m_smSpermBIHPro, m_lpDibIHPro, sizeof(BITMAPFILEHEADER) );
	memcpy(m_smSpermBDPro, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);

}

/*-----------------以下进行形态学分析的形态参数计算-------------------*/

//图像边缘提取： 掏空内点法
void CSingleSpermRecognition::OnGetHeadEdge() 
{

	// TODO: Add your command handler code here
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

//------顶体和头部的边缘------//

    // 指向缓存图像的指针
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);

	int i, j, k;
	bool a[8];
    for(i=1; i<Height-1; i++)
	{
		for(j=1; j<Width-1; j++)
		{		
			// 红色顶体或者蓝色头部
           	a[0] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-1)*3) == BLUE_PIEXL ? false : true;
			a[1] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-0)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-0)*3) == BLUE_PIEXL ? false : true;
			a[2] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j+1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j+1)*3) == BLUE_PIEXL ? false : true;
			a[3] = PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j-1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j-1)*3) == BLUE_PIEXL ? false : true;
			a[4] = PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j+1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j+1)*3) == BLUE_PIEXL ? false : true;
			a[5] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-1)*3) == BLUE_PIEXL ? false : true;
			a[6] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-0)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-0)*3) == BLUE_PIEXL ? false : true;
			a[7] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j+1)*3) == RED_PIEXL||
				   PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j+1)*3) == BLUE_PIEXL ? false : true;

			int sum = 0;
			for(k= 0; k<8; k++)
				sum += (int)a[k]; 
            if(sum == 0)
            {
                *(lpTemp+ i*ByteWidth + j*3 +0) = 255;
				*(lpTemp+ i*ByteWidth + j*3 +1) = 255;
				*(lpTemp+ i*ByteWidth + j*3 +2) = 255;
            }
		}    
	}

	LPBYTE lpSrc; 
    for(i=0; i<Height; i++)
	{
	   for(j=0; j<Width; j++)
	   {
          lpSrc = lpTemp + i*ByteWidth +j*3;
		  if( PIEXL(lpSrc) == RED_PIEXL || // 红
		      PIEXL(lpSrc) == BLUE_PIEXL )  // 蓝
			  m_HeadEdgePoint.push_back(CPoint(i,j));
	   }
	}

//------尾部的边缘------//

	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);

    for(i=1; i<Height-1; i++)
	{
		for(j=1; j<Width-1; j++)
		{		  
			// 绿色尾部
           	a[0] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-1)*3) == GREEN_PIEXL ? false : true;		
			a[1] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j-0)*3) == GREEN_PIEXL ? false : true;		
			a[2] = PIEXL(m_lpDibDataPro+ (i-1)*ByteWidth + (j+1)*3) == GREEN_PIEXL ? false : true;		
			a[3] = PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j-1)*3) == GREEN_PIEXL ? false : true;	
			a[4] = PIEXL(m_lpDibDataPro+ (i-0)*ByteWidth + (j+1)*3) == GREEN_PIEXL ? false : true;
			a[5] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-1)*3) == GREEN_PIEXL ? false : true;
			a[6] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j-0)*3) == GREEN_PIEXL ? false : true;
			a[7] = PIEXL(m_lpDibDataPro+ (i+1)*ByteWidth + (j+1)*3) == GREEN_PIEXL ? false : true;

			int sum = 0;
			for(k= 0; k<8; k++)
				sum += (int)a[k]; 
            if(sum == 0)
            {
                *(lpTemp+ i*ByteWidth + j*3 +0) = 255;
				*(lpTemp+ i*ByteWidth + j*3 +1) = 255;
				*(lpTemp+ i*ByteWidth + j*3 +2) = 255;
            }
		}    
	}

    for(i=0; i<Height; i++)
	{
	   for(j=0; j<Width; j++)
	   {
          lpSrc = lpTemp + i*ByteWidth +j*3;
		  if( PIEXL(lpSrc) == GREEN_PIEXL )  // 绿色
			  m_TailEdgePoint.push_back(CPoint(i,j));
	   }
	}

	delete  []lpTemp;
    lpTemp = NULL;

}

// 计算两点间距离
double CSingleSpermRecognition::distance(CPoint p, CPoint q) 
{
     return  (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y);
}

// 计算长轴长度
double CSingleSpermRecognition::GetLength()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

    int n = m_HeadEdgePoint.size();

	double  dist, max = 0;
	LPBYTE  lpSrcI = NULL, lpSrcJ = NULL;
	int i, j;  
	int maxI, maxJ;
	maxI = 0;
	maxJ = 0;
	for(i=0; i<n; i++) {	
		for(j=i+1; j<n; j++) {

			lpSrcI = m_lpDibDataPro + m_HeadEdgePoint[i].x*ByteWidth + m_HeadEdgePoint[i].y*3;
			lpSrcJ = m_lpDibDataPro + m_HeadEdgePoint[j].x*ByteWidth + m_HeadEdgePoint[j].y*3;
		
			if( PIEXL(lpSrcI) != PIEXL(lpSrcJ) ) {  
				dist = distance(m_HeadEdgePoint[i], m_HeadEdgePoint[j]);
				if(dist > max) {	
				   max = dist;
				   maxI = i;
				   maxJ = j;
				}
			}			
		}
	}
    if(n == 0)
		return sqrt(max);

	lpSrcI = m_lpDibDataPro + m_HeadEdgePoint[maxI].x*ByteWidth + m_HeadEdgePoint[maxI].y*3;
	if( PIEXL(lpSrcI) == BLUE_PIEXL ){	
		m_ProlateTip[0] = m_HeadEdgePoint[maxI]; // 让头尾相接处点存放于m_ProlateTip[0]
		m_ProlateTip[1] = m_HeadEdgePoint[maxJ]; 
	}
	else{	
		m_ProlateTip[0] = m_HeadEdgePoint[maxJ]; // 让头尾相接处点存放于m_ProlateTip[0]
		m_ProlateTip[1] = m_HeadEdgePoint[maxI]; 
	}
	return  sqrt(max);

}

//  排序用的比较函数
int cmp(const void *a,const void *b)
{

	CPoint *p, *q;
	p = (CPoint*)a;
	q = (CPoint*)b;
	if(p->x != q->x)
	   return p->x - q->x;
	else
		return p->y - q->y;
	
}

// 计算宽度
double CSingleSpermRecognition::GetWidth()
{
    int n = m_HeadEdgePoint.size();

	if(n ==0)
		return 0;

	double dltY = double(m_ProlateTip[0].y - m_ProlateTip[1].y);
	double dltX = double(m_ProlateTip[0].x - m_ProlateTip[1].x);
    double a;      //长轴倾斜角 

	int i, j;
	double  dist, max = 0;
    
    if( (int)dltX == 0 )
	{
       a = PI/2.0;
	}
	else if( (int)dltY == 0 )
	{
	   a = 0;
	}
    else{
	   a = atan(dltY/dltX);
	}

    // 做坐标旋转变换，使得长轴沿水平方向
	CPoint *t_PoTemp = new CPoint[n];
	for(i=0; i<n; i++)
	{
        t_PoTemp[i].x =  cos(a)* m_HeadEdgePoint[i].x  + sin(a)* m_HeadEdgePoint[i].y ;
        t_PoTemp[i].y = -sin(a)* m_HeadEdgePoint[i].x  + cos(a)* m_HeadEdgePoint[i].y ;
	}

    qsort(t_PoTemp, n, sizeof(CPoint), cmp); //按X坐标升序，并且Y坐标升序排序
	
	for(i=1; i<n; i++)
	{
		j = i;
		while( t_PoTemp[i-1].x == t_PoTemp[j].x && j<n ) j++;
		
		dist = distance( t_PoTemp[i-1], t_PoTemp[j-1] );
		if(dist >max){
			max = dist;
			m_MinorAxisTip[0] = t_PoTemp[i-1];
			m_MinorAxisTip[1] = t_PoTemp[j-1];
		}
		   
	}

	int OriginX, OriginY;
	OriginX = m_MinorAxisTip[0].x;
	OriginY = m_MinorAxisTip[0].y;

	m_MinorAxisTip[0].x = cos(a)* OriginX  - sin(a)* OriginY ;
    m_MinorAxisTip[0].y = sin(a)* OriginX  + cos(a)* OriginY ;

	OriginX = m_MinorAxisTip[1].x;
	OriginY = m_MinorAxisTip[1].y;

	m_MinorAxisTip[1].x = cos(a)* OriginX  - sin(a)* OriginY ;
	m_MinorAxisTip[1].y = sin(a)* OriginX  + cos(a)* OriginY ;

	delete []t_PoTemp;
    return sqrt(max);

}

// 计算整个椭圆体面积
double CSingleSpermRecognition::GetArea()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int area = 0;
	LPBYTE  lpSrc;
	int i, j;
    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
            if( PIEXL(lpSrc) == RED_PIEXL || // 红
				PIEXL(lpSrc) == BLUE_PIEXL )  // 蓝
			 area++;
		}    
	}

    return area;
}

// 计算顶体面积
double CSingleSpermRecognition::GetPerforArea()
{ 
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int area = 0;
	LPBYTE  lpSrc;
	int i, j;
    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
            if( PIEXL(lpSrc) == RED_PIEXL ) // 红色 
			 area++;
		}    
	}

    return area;

}


// 计算头部面积
double CSingleSpermRecognition::GetHeadArea()
{ 
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	int area = 0;
	LPBYTE  lpSrc;
	int i, j;
    for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
            if( PIEXL(lpSrc) == BLUE_PIEXL ) // 蓝色
			 area++;
		}    
	}

    return area;
}

// 计算整个椭圆周长
double CSingleSpermRecognition::GetPerimeter()
{
	return m_HeadEdgePoint.size();
}

// 计算尾巴长度
double CSingleSpermRecognition::GetTailLength()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	m_TailTip = m_ProlateTip[0];
	double  dist, max = 0;
    vector<CPoint>::iterator  itr;
	for(itr= m_TailEdgePoint.begin(); itr != m_TailEdgePoint.end(); itr++)
	{
        dist = distance(m_ProlateTip[0], *itr);
		if(dist >= max)
		{
			max = dist;
			m_TailTip = *itr;
		}
	}

    return sqrt(max);

}

// 计算尾巴宽度
double CSingleSpermRecognition::GetTailWidth()
{
	int i, j;
	double  dist, max = 0;
    int n = m_HeadTailBoundary.size();
	for(i=0; i<n; i++)
	{
	   for(j=i+1; j<n; j++)
	   {
           dist = distance(m_HeadTailBoundary[i], m_HeadTailBoundary[j]);
		   if(dist > max){
		      max = dist;
		   }
	   }
	}	
	
	return sqrt(max);

}

// 点乘运算
double DotProduct(CPoint p, CPoint q)
{
	return p.x*q.x + p.y*q.y;
}

// 向量取模运算
double GetModule(CPoint p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}

// 计算尾巴偏角
double CSingleSpermRecognition::GetTailAngle()
{
	double deviAngle;
	typedef  CPoint VECTOR;
	VECTOR HeadAxis, TailAxis; 
    HeadAxis = VECTOR(m_ProlateTip[1].x - m_ProlateTip[0].x,  m_ProlateTip[1].y - m_ProlateTip[0].y );
    TailAxis = VECTOR(m_TailTip.x - m_ProlateTip[0].x, m_TailTip.y - m_ProlateTip[0].y);
	if(GetModule(HeadAxis)*GetModule(TailAxis)  < 1.0e-6 )
		return 0;
    deviAngle = acos( DotProduct(HeadAxis, TailAxis)/(GetModule(HeadAxis)*GetModule(TailAxis)) );	
	
	return  180 - 180*deviAngle/PI;

}

// 标记椭圆轮廓
void CSingleSpermRecognition::OnMarkEllipse()
{
   long  Width = m_lpDibIHPro->biWidth;
   long  Height= m_lpDibIHPro->biHeight;
   long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

   int n = m_HeadEdgePoint.size();
   int i;
   LPBYTE lpSrc;

   for(i=0; i<n; i++)
   {
      lpSrc = m_lpDibDataPro + m_HeadEdgePoint[i].x*ByteWidth + m_HeadEdgePoint[i].y*3;
	  *(lpSrc+ 0) = 0;
	  *(lpSrc+ 1) = 0;
	  *(lpSrc+ 2) = 0;
   }
   
} 


// 标记长轴
void CSingleSpermRecognition::OnMarkProlate()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	double slope;
	double dltY = double(m_ProlateTip[0].y - m_ProlateTip[1].y);
	double dltX = double(m_ProlateTip[0].x - m_ProlateTip[1].x);
    
	LPBYTE  lpSrc;
	int i, j;

    if( (int)dltX == 0 )
	{
        int litter = min(m_ProlateTip[0].y, m_ProlateTip[1].y);
		int big    = max(m_ProlateTip[0].y, m_ProlateTip[1].y);
		for(j=litter; j<big; j++)
		{
			lpSrc = m_lpDibDataPro+ m_ProlateTip[0].x*ByteWidth + j*3;
			*(lpSrc+ 0) = 255;
			*(lpSrc+ 1) = 255;
 			*(lpSrc+ 2) = 0;    
		}    

	}
	else if( (int)dltY == 0 )
	{
        int litter = min(m_ProlateTip[0].x, m_ProlateTip[1].x);
		int big    = max(m_ProlateTip[0].x, m_ProlateTip[1].x);
		for(i=litter; i<big; i++)
		{
			lpSrc = m_lpDibDataPro + i*ByteWidth + m_ProlateTip[0].y*3;
			*(lpSrc+ 0) = 255;
			*(lpSrc+ 1) = 255;
 			*(lpSrc+ 2) = 0;    
		} 
	}
    else{

    	int rLitter = min(m_ProlateTip[0].x, m_ProlateTip[1].x);
		int rBig    = max(m_ProlateTip[0].x, m_ProlateTip[1].x);
    	int cLitter = min(m_ProlateTip[0].y, m_ProlateTip[1].y);
		int cBig    = max(m_ProlateTip[0].y, m_ProlateTip[1].y);
		slope = dltY/dltX;
	
        for(i=rLitter; i<rBig; i++)
		{
			for(j=cLitter; j<cBig; j++)
			{
				double k = i - m_ProlateTip[0].x;
				double t = j - m_ProlateTip[0].y;
				if( int(t) == int(slope*k) ){

					lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
					*(lpSrc+ 0) = 255;
					*(lpSrc+ 1) = 255;
 					*(lpSrc+ 2) = 0; 
                }
			}    
		}	   
	}
 

}  


// 标记短轴
void CSingleSpermRecognition::OnMarkMinorAxis()
{
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	double slope;
	double dltY = double(m_MinorAxisTip[0].y - m_MinorAxisTip[1].y);
	double dltX = double(m_MinorAxisTip[0].x - m_MinorAxisTip[1].x);
    
	LPBYTE  lpSrc;
	int i, j;

    if( (int)dltX == 0 )
	{
        int litter = min(m_MinorAxisTip[0].y, m_MinorAxisTip[1].y);
		int big    = max(m_MinorAxisTip[0].y, m_MinorAxisTip[1].y);
		for(j=litter; j<big; j++)
		{
			lpSrc = m_lpDibDataPro+ m_MinorAxisTip[0].x*ByteWidth + j*3;
			*(lpSrc+ 0) = 255;
			*(lpSrc+ 1) = 255;
 			*(lpSrc+ 2) = 0;    
		}    

	}
	else if( (int)dltY == 0 )
	{
        int litter = min(m_MinorAxisTip[0].x, m_MinorAxisTip[1].x);
		int big    = max(m_MinorAxisTip[0].x, m_MinorAxisTip[1].x);
		for(i=litter; i<big; i++)
		{
			lpSrc = m_lpDibDataPro + i*ByteWidth + m_MinorAxisTip[0].y*3;
			*(lpSrc+ 0) = 255;
			*(lpSrc+ 1) = 255;
 			*(lpSrc+ 2) = 0;    
		} 
	}
    else{

    	int rLitter = min(m_MinorAxisTip[0].x, m_MinorAxisTip[1].x);
		int rBig    = max(m_MinorAxisTip[0].x, m_MinorAxisTip[1].x);
    	int cLitter = min(m_MinorAxisTip[0].y, m_MinorAxisTip[1].y);
		int cBig    = max(m_MinorAxisTip[0].y, m_MinorAxisTip[1].y);
		slope = dltY/dltX;
	
        for(i=rLitter; i<rBig; i++)
		{
			for(j=cLitter; j<cBig; j++)
			{
				double k = i - m_MinorAxisTip[0].x;
				double t = j - m_MinorAxisTip[0].y;
				if( int(t) == int(slope*k) ){

					lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
					*(lpSrc+ 0) = 255;
					*(lpSrc+ 1) = 255;
 					*(lpSrc+ 2) = 0; 
                }
			}    
		}	   
	}
 
}
 
 
// 标记尾轴
void CSingleSpermRecognition::OnMarkTailAxis()
{
    
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

	double slope;
	double dltY = double(m_ProlateTip[0].y - m_TailTip.y);
	double dltX = double(m_ProlateTip[0].x - m_TailTip.x);
    
	LPBYTE  lpSrc;
	int i, j;

    if( (int)dltX == 0 )
	{
        int litter = min(m_ProlateTip[0].y, m_TailTip.y);
		int big    = max(m_ProlateTip[0].y, m_TailTip.y);
		for(j=litter; j<big; j++)
		{
			lpSrc = m_lpDibDataPro+ m_TailTip.x*ByteWidth + j*3;
			*(lpSrc+ 0) = 0;
			*(lpSrc+ 1) = 0;
 			*(lpSrc+ 2) = 255;    
		}    

	}
	else if( (int)dltY == 0 )
	{
        int litter = min(m_ProlateTip[0].x, m_TailTip.x);
		int big    = max(m_ProlateTip[0].x, m_TailTip.x);
		for(i=litter; i<big; i++)
		{
			lpSrc = m_lpDibDataPro + i*ByteWidth + m_TailTip.y*3;
			*(lpSrc+ 0) = 0;
			*(lpSrc+ 1) = 0;
 			*(lpSrc+ 2) = 255;    
		} 
	}
    else{

    	int rLitter = min(m_ProlateTip[0].x, m_TailTip.x);
		int rBig    = max(m_ProlateTip[0].x, m_TailTip.x);
    	int cLitter = min(m_ProlateTip[0].y, m_TailTip.y);
		int cBig    = max(m_ProlateTip[0].y, m_TailTip.y);
		slope = dltY/dltX;
	
        for(i=rLitter; i<rBig; i++)
		{
			for(j=cLitter; j<cBig; j++)
			{
				double k = i - m_TailTip.x;
				double t = j - m_TailTip.y;
				if( int(t) == int(slope*k) ){

					lpSrc = m_lpDibDataPro+ i*ByteWidth + j*3;
					*(lpSrc+ 0) = 0;
					*(lpSrc+ 1) = 0;
 					*(lpSrc+ 2) = 255; 
                }
			}    
		}	   
	} 

}

/*-----------------------------------------------------*/
/*-----------------形态学分析第二步--------------------*/
/*---------------------参数计算------------------------*/
/*-----------------------------------------------------*/
void CSingleSpermRecognition::OnSperMopyParamCalc()  // 参数计算
{
	// TODO: Add your control notification handler code here
	
	double	_length = 0.0;
	double	_width  = 0.0;
	double	_area   = 0.0;
	double	_ellipticity = 0.0;
	double	_perfor_area = 0.0;
	double	_head_area   = 0.0; 
	double	_perimeter   = 0.0;
    double  _head_perfor_area = 0.0;

	double	_tail_length = 0.0;
	double	_tail_width  = 0.0;
	double	_tail_angle  = 0.0;

	double  _extension   = 0.0;
	double  _symmetry    = 0.0;
	double  _ruga        = 0.0;

    OnGetHeadEdge();

	_length = GetLength();
	_width  = GetWidth();
    _area   = GetArea();
    _perimeter   = GetPerimeter();
	_ellipticity = _width < 1.0e-6 ? 0.0 : _length/_width;
    _perfor_area = GetPerforArea();
	_head_area   = GetHeadArea();
    _head_perfor_area = _area < 1.0e-6 ? 0.0 : _perfor_area/_area;

	_tail_length = GetTailLength();
	_tail_width  = GetTailWidth();
	_tail_angle  = GetTailAngle();

	// 计算伸展度((L-W)/(L+W)
	_extension   = ( ((_length + _width)<1.0e-6) || ((_length - _width)< 1.0e-6) ) 
		? 0.0 : (_length - _width)/(_length + _width); 

	// 计算对称度(πLW/4S)
	_symmetry    = _area<1.0e-6 ? 0.0 : PI*_length*_width/(4*_area);
	
	// 计算皱褶度(4πS/P2)
	_ruga        = _perimeter*_perimeter<1.0e-6 ? 0.0 : (4*PI*_area/(_perimeter*_perimeter));

//	OnMarkEllipse();   // 标记椭圆轮廓
//  OnMarkProlate();   // 标记长轴
//	OnMarkMinorAxis(); // 标记短轴
//  OnMarkTailAxis();  // 标记尾轴

	m_SperMopyParameter.m_length        = _length ;    
	m_SperMopyParameter.m_width         = _width  ;
	m_SperMopyParameter.m_area          = _area   ;
	m_SperMopyParameter.m_ellipticity       = _ellipticity  ;
	m_SperMopyParameter.m_perfor_area       =  _perfor_area ;
	m_SperMopyParameter.m_head_area         =  _head_area   ;
	m_SperMopyParameter.m_perimeter         = _perimeter    ;
	m_SperMopyParameter.m_head_perfor_area  = _head_perfor_area ;

	m_SperMopyParameter.m_tail_length       = _tail_length  ;
	m_SperMopyParameter.m_tail_width        = _tail_width   ;
	m_SperMopyParameter.m_tail_angle        = _tail_angle   ;

	m_SperMopyParameter.m_extension   = _extension ; 
	m_SperMopyParameter.m_symmetry    = _symmetry  ;
	m_SperMopyParameter.m_ruga        = _ruga      ;
}
