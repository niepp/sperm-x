// SpermParamCalc.cpp : implementation file
//
/*******************************************************
*                                                      *
*  	SpermParamCalc.cpp                                 *
*	author: happynp                                    *
*	Email: happynp@gmail.com                           *
*   2009/6/23                                          *
*   ������̬ѧ������ ����������̬ѧ��������            *
*                                                      *
*******************************************************/ 


#include "stdafx.h"

#include "SpermParamCalc.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SpermParamCalc dialog

SpermParamCalc::SpermParamCalc(
							LPBITMAPINFOHEADER  lpBmInfoSrc, 
							LPBYTE lpBmDataSrc)
{
    m_lpDibIHPro = NULL;
	m_lpDibDataPro  = NULL;

    m_lpDibIHPro = (LPBITMAPINFOHEADER)new unsigned char[sizeof(BITMAPINFOHEADER)];
    memcpy(m_lpDibIHPro, lpBmInfoSrc, sizeof(BITMAPINFOHEADER));
	m_lpDibDataPro  = (LPBYTE)new unsigned char[lpBmInfoSrc->biSizeImage];
	memcpy(m_lpDibDataPro, lpBmDataSrc, lpBmInfoSrc->biSizeImage);

    m_ProlateTip[0] = CPoint(-1,-1);
	m_ProlateTip[1] = CPoint(-1,-1);
		
	m_MinorAxisTip[0] = CPoint(-1,-1);
	m_MinorAxisTip[1] = CPoint(-1,-1);
    m_TailTip = CPoint(-1,-1);          

}

SpermParamCalc::~SpermParamCalc()
{	
	delete []m_lpDibIHPro;
    m_lpDibIHPro = NULL;
  
	delete []m_lpDibDataPro;
	m_lpDibDataPro = NULL;
}

/////////////////////////////////////////////////////////////////////////////
/*-----------------���½�����̬ѧ��������̬��������-------------------*/

//ͼ���Ե��ȡ�� �Ϳ��ڵ㷨
void SpermParamCalc::OnGetHeadEdge() 
{

	// TODO: Add your command handler code here
	long  Width = m_lpDibIHPro->biWidth;
	long  Height= m_lpDibIHPro->biHeight;
	long  ByteWidth = WIDTHBYTES(Width * m_lpDibIHPro->biBitCount); 

//------�����ͷ���ı�Ե------//

    // ָ�򻺴�ͼ���ָ��
	unsigned char* lpTemp = new unsigned char[m_lpDibIHPro->biSizeImage];
	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);

	int i, j, k;
	bool a[8];
    for(i=1; i<Height-1; i++)
	{
		for(j=1; j<Width-1; j++)
		{		
			// ��ɫ���������ɫͷ��
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
		  if( PIEXL(lpSrc) == RED_PIEXL || // ��
		      PIEXL(lpSrc) == BLUE_PIEXL )  // ��
			  m_HeadEdgePoint.push_back(CPoint(i,j));
	   }
	}

//------β���ı�Ե------//

	memcpy(lpTemp, m_lpDibDataPro, m_lpDibIHPro->biSizeImage);

    for(i=1; i<Height-1; i++)
	{
		for(j=1; j<Width-1; j++)
		{		  
			// ��ɫβ��
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
		  if( PIEXL(lpSrc) == GREEN_PIEXL )  // ��ɫ
			  m_TailEdgePoint.push_back(CPoint(i,j));
	   }
	}

	delete  []lpTemp;
    lpTemp = NULL;

}

// ������������
double SpermParamCalc::distance(CPoint p, CPoint q) 
{
     return  (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y);
}

// ���㳤�᳤��
double SpermParamCalc::GetLength()
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
		m_ProlateTip[0] = m_HeadEdgePoint[maxI]; // ��ͷβ��Ӵ�������m_ProlateTip[0]
		m_ProlateTip[1] = m_HeadEdgePoint[maxJ]; 
	}
	else{	
		m_ProlateTip[0] = m_HeadEdgePoint[maxJ]; // ��ͷβ��Ӵ�������m_ProlateTip[0]
		m_ProlateTip[1] = m_HeadEdgePoint[maxI]; 
	}
	return  sqrt(max);

}

//  �����õıȽϺ���
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

// ������
double SpermParamCalc::GetWidth()
{
    int n = m_HeadEdgePoint.size();

	if(n ==0)
		return 0;

	double dltY = double(m_ProlateTip[0].y - m_ProlateTip[1].y);
	double dltX = double(m_ProlateTip[0].x - m_ProlateTip[1].x);
    double a;      //������б�� 

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

    // ��������ת�任��ʹ�ó�����ˮƽ����
	CPoint *t_PoTemp = new CPoint[n];
	for(i=0; i<n; i++)
	{
        t_PoTemp[i].x =  cos(a)* m_HeadEdgePoint[i].x  + sin(a)* m_HeadEdgePoint[i].y ;
        t_PoTemp[i].y = -sin(a)* m_HeadEdgePoint[i].x  + cos(a)* m_HeadEdgePoint[i].y ;
	}

    qsort(t_PoTemp, n, sizeof(CPoint), cmp); //��X�������򣬲���Y������������
	
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

// ����������Բ�����
double SpermParamCalc::GetArea()
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
            if( PIEXL(lpSrc) == RED_PIEXL || // ��
				PIEXL(lpSrc) == BLUE_PIEXL )  // ��
			 area++;
		}    
	}

    return area;
}

// ���㶥�����
double SpermParamCalc::GetPerforArea()
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
            if( PIEXL(lpSrc) == RED_PIEXL ) // ��ɫ 
			 area++;
		}    
	}

    return area;

}


// ����ͷ�����
double SpermParamCalc::GetHeadArea()
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
            if( PIEXL(lpSrc) == BLUE_PIEXL ) // ��ɫ
			 area++;
		}    
	}

    return area;
}

// ����������Բ�ܳ�
double SpermParamCalc::GetPerimeter()
{
	return m_HeadEdgePoint.size();
}

// ����β�ͳ���
double SpermParamCalc::GetTailLength()
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

// ����β�Ϳ��
double SpermParamCalc::GetTailWidth()
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
			// �ҳ�β����ͷ���Ĺ����߽� 
           	a[0] = m_lpDibDataPro+ (i-1)*ByteWidth + (j-1)*3;
  	        a[1] = m_lpDibDataPro+ (i-1)*ByteWidth + (j+0)*3;
	  	    a[2] = m_lpDibDataPro+ (i+0)*ByteWidth + (j-1)*3;
		  	a[3] = m_lpDibDataPro+ (i+0)*ByteWidth + (j+0)*3;

            HasGreen = false;
			HasBlue  = false;

			for( k =0; k<4; k++)
			{
				if( PIEXL(a[k]) == GREEN_PIEXL ) // ��ɫβ��
					HasGreen = true;

				if( PIEXL(a[k]) == BLUE_PIEXL ) // ��ɫͷ��
				    HasBlue  = true;
			}

         	// �ҳ�β����ͷ���Ĺ����߽磬���루Vector��m_HeadTailBoundary
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

// �������
double DotProduct(CPoint p, CPoint q)
{
	return p.x*q.x + p.y*q.y;
}

// ����ȡģ����
double GetModule(CPoint p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}

// ����β��ƫ��
double SpermParamCalc::GetTailAngle()
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

// �����Բ����
void SpermParamCalc::OnMarkEllipse()
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


// ��ǳ���
void SpermParamCalc::OnMarkProlate()
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


// ��Ƕ���
void SpermParamCalc::OnMarkMinorAxis()
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
 
 
// ���β��
void SpermParamCalc::OnMarkTailAxis()
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

/*---------------------��������------------------------*/

void SpermParamCalc::OnSperMopyParamCalc()  // ��������
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

	// ������չ��((L-W)/(L+W)
	_extension   = ( ((_length + _width)<1.0e-6) || ((_length - _width)< 1.0e-6) ) 
		? 0.0 : (_length - _width)/(_length + _width); 

	// ����Գƶ�(��LW/4S)
	_symmetry    = _area<1.0e-6 ? 0.0 : PI*_length*_width/(4*_area);
	
	// �������޶�(4��S/P2)
	_ruga        = _perimeter*_perimeter<1.0e-6 ? 0.0 : (4*PI*_area/(_perimeter*_perimeter));

// 	OnMarkEllipse();   // �����Բ����
//  OnMarkProlate();   // ��ǳ���
// 	OnMarkMinorAxis(); // ��Ƕ���
//  OnMarkTailAxis();  // ���β��

	m_SperMopyParameter.m_length        = _length;    
	m_SperMopyParameter.m_width         = _width;
	m_SperMopyParameter.m_area          = _area;
	m_SperMopyParameter.m_ellipticity       = _ellipticity;
	m_SperMopyParameter.m_perfor_area       =  _perfor_area;
	m_SperMopyParameter.m_head_area         =  _head_area;
	m_SperMopyParameter.m_perimeter         = _perimeter;
	m_SperMopyParameter.m_head_perfor_area  = _head_perfor_area;

	m_SperMopyParameter.m_tail_length       = _tail_length;
	m_SperMopyParameter.m_tail_width        = _tail_width;
	m_SperMopyParameter.m_tail_angle        = _tail_angle;

	m_SperMopyParameter.m_extension   = _extension; 
	m_SperMopyParameter.m_symmetry    = _symmetry;
	m_SperMopyParameter.m_ruga        = _ruga;
}
