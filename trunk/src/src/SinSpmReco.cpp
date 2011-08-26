#include "stdafx.h"
#include "SinSpmReco.h"
#include "AllFunction.h"
#include "ImageProcess.h"
#include <math.h>

int gThreshAdjustPart = -1;

void SinSpmReco::Erosion(IplImage *Img) // ��ʴ����
{
	// TODO: Add your command handler code here
	
    // ָ��Դͼ���ָ��
	char* lpSrc;

	// ָ�򻺴�ͼ���ָ��
	char* lpDst;
	char* lpTemp = new char[Img->imageSize];
    
	memcpy(lpTemp, Img->imageData, Img->imageSize);
	
	// ������ÿ���ֽ���
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep;

	// ��ʴ�ṹģ��
    int structure[8][8] = { 0,0,1,1,1,1,0,0,
							0,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,0,
							0,0,1,1,1,1,0,0 };

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	char pixel;

	//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
	for(j = 0; j <Height-8; j++)
	{
		for(i = 0;i <Width-8; i++)
		{

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = Img->imageData + j*ByteWidth+ i;

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
			lpDst = lpTemp+ j*ByteWidth+ i;

			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (char)*lpSrc;
		
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != (char)255 && pixel != (char)0)
			   return ;

			//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
			//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0; m<8; m++ )
			{
				for (n = 0; n<8; n++)
				{   
					if(structure[m][n]==0)
					   continue;
					pixel = *(lpSrc + m*ByteWidth + n);
					if(pixel == (char)255)
					{	
                        int a,b;
						for (a = 0; a<8; a++ )
						{
							for (b = 0; b<8; b++)
							{  								
								*(lpDst + a*ByteWidth + b) = (char)255;
							}
						}
					    goto nextpiexl;
					}
				}
			}
nextpiexl:;
		}
	}
    memcpy(Img->imageData, lpTemp, Img->imageSize);
	delete []lpTemp; lpTemp = NULL;

}

void SinSpmReco::Expand(IplImage*Img, IplImage* lpSegSavedImg) // ��������
{
	// TODO: Add your command handler code here

    // ָ��Դͼ���ָ��
	char* lpSrc;

	// ָ�򻺴�ͼ���ָ��
	char* lpDst;
	char* lpTemp = new char[Img->imageSize];
    
	memcpy(lpTemp, Img->imageData, Img->imageSize);
	
	// ������ÿ���ֽ���  
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep; 
	
	// ��ʴ�ṹģ��
    int structure[8][8] = { 0,0,1,1,1,1,0,0,
							0,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,0,
							0,0,1,1,1,1,0,0 };

	//ѭ������
	long i;
	long j;
	int  n;
	int  m; 

	//����ֵ
	char pixel;

	//ʹ���Զ���ĽṹԪ�ؽ�������
	for(j = 0; j <Height-8; j++)
	{
		for(i = 0;i <Width-8; i++)
		{

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = Img->imageData+ j*ByteWidth+ i;

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
			lpDst = lpTemp+ j*ByteWidth+ i;

			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (char)*lpSrc;
		
			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != (char)255 && pixel != (char)0)
			   return ;

			//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
			//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
			//ע����DIBͼ�������������µ��õ�
			for (m = 0;m < 8;m++ )
			{
				for (n = 0;n < 8;n++)
				{   
					if(structure[m][n]==0)
					   continue;
					pixel = *(lpSrc + m*ByteWidth + n);
					if(pixel == (char)0 ) 
					{	
                        int a,b;
						for(a = 0; a < 8; a++ )
						{
							for(b = 0; b < 8; b++)
							{   
                              if( lpSegSavedImg->imageData[(j+a)*ByteWidth + (i+b)] == (char)0)
							  {
								*(lpDst + a*ByteWidth + b) = (char)0;
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
    memcpy(Img->imageData, lpTemp, Img->imageSize);
	delete []lpTemp;
	lpTemp = NULL;
}

void SinSpmReco::OnErosionTail(IplImage * Img)
{
	// TODO: Add your command handler code here

	int Height = Img->height;
	int Width  = Img->width;
	int ByteWidth = Img->widthStep;

    // ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ�򻺴�ͼ���ָ��
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[Img->imageSize];
    
	memcpy(lpTemp, Img->imageData, Img->imageSize);

	int structure[7][7] = { 0,0,1,1,1,0,0,
							0,1,1,1,1,1,0,
							1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,
							0,1,1,1,1,1,0,
							0,0,1,1,1,0,0};
	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
	for(i=0; i<Height-7; i++)
	{
		for(j=0; j<Width-7; j++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char*)(Img->imageData + i*ByteWidth + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
			lpDst = lpTemp + i*ByteWidth + j;

			bool bHasHead=false, bHasBk=false;
			for (m=0; m<7; m++ )
			{
				for (n=0; n<7; n++)
				{   
					if( structure[m][n]==0 )
						continue;
					if( lpSrc[m*ByteWidth+n] == (unsigned char)(255) )
						bHasBk = true;
					if( lpSrc[m*ByteWidth+n] == (unsigned char)(HEADRG) )
						bHasHead = true;
				}
			}
			if(bHasBk&&!bHasHead)
				lpDst[3*ByteWidth+3] = (unsigned char)(255);
		}
	}
    memcpy(Img->imageData, lpTemp, Img->imageSize);
	delete lpTemp;
}

bool SinSpmReco::HasThePiexlInAround(unsigned char piexl, int row, int col, IplImage* Img)
{
	int   k, nr, nc;
	long  width = Img->width;
	long  height= Img->height;
	int dir[4][2] = {
		{0,1},
		{1,0},
		{0,-1},
		{-1,0}
	};
	for(k=0; k<4; k++)
	{
		nr = row + dir[k][0];
		nc = col + dir[k][1];
		if( (nr<height&&nr>=0 && nc<width&&width>=0) &&
			(CV_IMAGE_ELEM(Img, uchar, nr, nc) == (uchar)(piexl)) )
			return true;
	}
		
	return false;

}

bool SinSpmReco::IsEdgePt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img)
{	
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep;
	int dir[4][2] = {
		{0,1},
		{1,0},
		{0,-1},
		{-1,0}
	};
	if(   row == Height-1 || row == 0
	   || col == Width-1  || col == 0
		)
		return true;
	bool hasW, hasO;
	hasW = hasO = false;
	for(int k=0; k<4; k++) {
		int I = (row+dir[k][0]);
		int J = (col+dir[k][1]);
		if(Img->imageData[I*ByteWidth + J] == (char)objectpiexl)
			hasO = true;
		else if(Img->imageData[I*ByteWidth + J] == (char)bkpiexl)
			hasW = true;
	}
	return hasW&&hasO;
}

bool SinSpmReco::IsObjectPt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img)
{	
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep;
	int dir[4][2] = {
		{0,1},
		{1,0},
		{0,-1},
		{-1,0}
	};
	if( !( row < Height-1 && row >0
		&& col < Width-1  && col >0 )
		)
		return false;
	bool flag = false;
	for(int k=0; k<4; k++) {
		int I = (row+dir[k][0]);
		int J = (col+dir[k][1]);
		if(CV_IMAGE_ELEM(Img, uchar, I, J) == (uchar)(bkpiexl))
			return false;
	}
	return true;
}

CvPoint SinSpmReco::GetSeed(unsigned char objectpiexl, unsigned char bkpiexl, IplImage *Img)
{
	// TODO: Add your command handler code here
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep;
	int i, j, k;
	int dir[4][2] = {
		{0,1},
		{1,0},
		{0,-1},
		{-1,0}
	};
	
	bool hasBK, hasOT;
    for(i=1; i<Height-1; i++)
	{
		for(j=1; j<Width-1; j++)
		{
			hasBK = hasOT = false;
			for(k=0; k<4; k++) {
				uchar tmp = CV_IMAGE_ELEM(Img, uchar, i+dir[k][0], j+dir[k][1]);
				if(tmp==(uchar)bkpiexl) hasBK = true;
				if(tmp==(uchar)objectpiexl) {
					int I = (i+dir[k][0]);
					int J = (j+dir[k][1]);					
					if(IsObjectPt(objectpiexl, bkpiexl, I, J, Img))
						hasOT = true;
				}
			}
			if( hasBK && hasOT )
				return cvPoint(i,j);
		}
	}
	return cvPoint(-1,-1);
}

// �߽����
void SinSpmReco::EdgeTrack(unsigned char objectpiexl, unsigned char bkpiexl, CvPoint seed, IplImage *Img, stack<CvPoint>&vp)
{
	long  Width = Img->width;
	long  Height= Img->height;
	long  ByteWidth = Img->widthStep;
	int k;
	int dir[8][2] = { // ����ʱ������
		{0,1}, // ���У��У�	
		{-1,1},
		{-1,0},
		{-1,-1},
		{0,-1},
		{1,-1},
		{1,0},
		{1,1}
	};
	bool *fArr = new bool[Height*Width];
	memset(fArr, false, sizeof(bool)*Height*Width);
	CvPoint p = seed, nxtp=seed;
	vp.push(p);
	fArr[p.x*Width+p.y] = true;
	while(1) {
		bool end = false;
		p = vp.top();
		for(k=0;k<8;k++) {
			nxtp.x = p.x + dir[k][0];
			nxtp.y = p.y + dir[k][1];
			if(nxtp==seed) end = true;
			if(!fArr[nxtp.x*Width+nxtp.y]
				&& CV_IMAGE_ELEM(Img, uchar, nxtp.x, nxtp.y) == (uchar)objectpiexl
				&& IsEdgePt(objectpiexl, bkpiexl, nxtp.x, nxtp.y, Img)
				)
			{
				vp.push(nxtp);
				fArr[nxtp.x*Width+nxtp.y] = true;
				break;
			}
		}
		if(k==8) {
			if(end) break;
			else {
				nxtp = vp.top();
				vp.pop();
			}
		}
	}
}

// ����ֵͼ���У����������������������ɾ��������С����������
void SinSpmReco::SaveMaxAreaRegion(unsigned char objectpiexl, unsigned char bkpiexl, IplImage* Img)
{
	int i, j;
	int height = Img->height;
	int width  = Img->width;
	int dir[4][2] = {
		   -1, 0,
			1, 0,
			0,-1,
			0, 1,
	};
	CvPoint   q;
	vector<vector<CvPoint> >vReg;
	stack<CvPoint>PosQ;	
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if( CV_IMAGE_ELEM(Img, uchar, i, j) == (uchar)(0) )
			{
				CvPoint p = cvPoint(i,j);
				PosQ.push(p);
				vector<CvPoint>vr;
				while(!PosQ.empty())
				{
					p = PosQ.top();
					PosQ.pop();
					// Process Piexl
					vr.push_back(p);
					CV_IMAGE_ELEM(Img, uchar, p.x, p.y) = (uchar)(255);
					for(i=0; i<4; i++)
					{
						q.x = p.x + dir[i][0];
						q.y = p.y + dir[i][1];
						if( q.x<height && q.x>=0 &&
							q.y<width  && q.y>=0 &&
							CV_IMAGE_ELEM(Img, uchar, q.x, q.y) == (uchar)(0)
							)
						{
							PosQ.push(q);
						}
					}
				}
				vReg.push_back(vr);
			}
		}
	}

	unsigned int nmax = 0, idx=0;
	for(i=0; i<vReg.size(); i++)
	{
		if(vReg[i].size() > nmax) {
			nmax = vReg[i].size();
			idx = i;
		}
	}

	for(i=0; i<vReg[idx].size(); i++)
	{
		CV_IMAGE_ELEM(Img, uchar, vReg[idx][i].x, vReg[idx][i].y) = (uchar)(0);
	}

}

void SinSpmReco::GetskPoint(stack<CvPoint>&skp, vector<skPoint>&vkp, int r)
{
	CvPoint p = skp.top();
	vkp.push_back(skPoint(p.x, p.y));
	skp.pop();
	int k=1;
	while(!skp.empty()) {
		if(k++ >= r) {
			p = skp.top();
			vkp.push_back(skPoint(p.x, p.y));
			k = 0;
		}
		skp.pop();
	}
}

bool SinSpmReco::GetInitSnake(IplImage* srcImage, vector<skPoint>&vkp)
{
	IplImage *desImage, *MaskImage;
	desImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);
	MaskImage= cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);
	cvCopyImage(srcImage, desImage);
	cvCopyImage(srcImage, MaskImage);

	Erosion(desImage);

	Expand(desImage, MaskImage);	
	Expand(desImage, MaskImage);	

	cvReleaseImage(&MaskImage);

	SaveMaxAreaRegion(0, 255, desImage);
	
	CvPoint p = GetSeed(0, 255, desImage);

	if(p.x<0 || p.y<0) return false;
	stack<CvPoint>skp;
	EdgeTrack(0, 255, p, desImage, skp);

	GetskPoint(skp, vkp, 5);

	cvReleaseImage(&desImage);

	while(!skp.empty()) skp.pop();
	return true;

}

void SinSpmReco::Fill(IplImage* Img, CvPoint p)
{
	int i;
	int height = Img->height;
	int width  = Img->width;
	int dir[4][2] = {
		   -1, 0,
			1, 0,
			0,-1,
			0, 1,
	};
	CvPoint   q;
	CvPoint  *PosQ;
	bool **visit = new bool*[height];
	for(i=0; i<height; i++) {
		visit[i] = new bool[width];
		memset(visit[i], false, sizeof(bool)*width);
	}
	PosQ = new CvPoint[height*width];
	unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
	PosQ[iEnd++] = p;
	visit[p.x][p.y] = true;
	while( iStart < iEnd )
	{
		p = PosQ[--iEnd];	
		// Process Piexl
		CV_IMAGE_ELEM(Img, uchar, p.x, p.y) = (uchar)(HEADRG);

		for(i=0; i<4; i++)
		{
			q.x = p.x + dir[i][0];
			q.y = p.y + dir[i][1];
			if( q.x<height && q.x>=0 &&
				q.y<width  && q.y>=0 &&
				visit[q.x][q.y] == false && //δ����
				CV_IMAGE_ELEM(Img, uchar, q.x, q.y) == (uchar)(0)
			)
			{
				visit[q.x][q.y] = true;
				PosQ[iEnd++] = q;
			}
		}
	}	
	delete []PosQ;
	for(i=0; i<height; i++) {
		delete []visit[i];
	}
	delete []visit;
}


void SinSpmReco::GetReg(IplImage* Img, CvPoint p, vector<CvPoint>&reg)
{	
	int i;
	int height = Img->height;
	int width  = Img->width;
	int dir[4][2] = {
		   -1, 0,
			1, 0,
			0,-1,
			0, 1,
	};
	CvPoint   q;
	CvPoint  *PosQ;
	PosQ = new CvPoint[height*width];
	unsigned int iStart,iEnd;
	iStart = 0;
	iEnd   = 0;
	PosQ[iEnd++] = p;
	while( iStart < iEnd )
	{
		p = PosQ[--iEnd];
		reg.push_back(p);
		CV_IMAGE_ELEM(Img, uchar, p.x, p.y) = (uchar)(255);

		for(i=0; i<4; i++)
		{
			q.x = p.x + dir[i][0];
			q.y = p.y + dir[i][1];
			if( q.x<height && q.x>=0 &&
				q.y<width  && q.y>=0 &&
				CV_IMAGE_ELEM(Img, uchar, q.x, q.y) == (uchar)(0)
			)
			{
				PosQ[iEnd++] = q;
			}
		}
	}	
	delete []PosQ;
}

void SinSpmReco::RecoTail(IplImage* Img)
{
	int i, j;
	int height = Img->height;
	int width  = Img->width;
	vector<CvPoint>rv, mrv;
	unsigned int maxs = 0;
	for(i=0; i<height; i++)	
	{
		for(j=0; j<width; j++)
		{
			if(CV_IMAGE_ELEM(Img, uchar, i, j) == (uchar)(0))
			{
				rv.clear();
				GetReg(Img, cvPoint(i,j), rv);
				if(rv.size() > maxs)
				{
					maxs = rv.size();
					mrv  = rv;
				}
			}
		}
	}

	memset(Img->imageData, 255, Img->imageSize);

	for(i=0; i<mrv.size(); i++)
	{
		CV_IMAGE_ELEM(Img, uchar, mrv[i].x, mrv[i].y) = (uchar)(0);
	}
}

void SinSpmReco::OnTagOtsu(IplImage *Img)   // �����ͷ���ָ�
{
	int height = Img->height;
	int width  = Img->width;
	int   thresholdValue = 1; // ��ֵ
	int   ihist[256];         // ͼ��ֱ��ͼ��256����
	int   i, j, k;            // various counters
	int   n, n1, n2;
	double m, m1, m2, sum, csum, fmax, sb;

	// ��ֱ��ͼ����...
	memset(ihist, 0, sizeof(ihist));

	// ����ֱ��ͼ
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++) 
		{
			// ������1
			if(CV_IMAGE_ELEM(Img, uchar, i, j) == (uchar)(HEADRG))
				ihist[(uchar)(m_srcImage->imageData[m_srcImage->widthStep*i+ j])]++;
		}
	}

	// set up everything
	sum  = 0.0;
	csum = 0.0;
	   n = 0;
	for(k=0; k<=255; k++) 
	{
		sum += (double)k*(double)ihist[k];       /* x*f(x) ������ */
		  n += ihist[k];                         /* f(x)   ����   */
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
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if(CV_IMAGE_ELEM(Img, uchar, i, j) == (uchar)(HEADRG)) 
			{
				unsigned char v = (char)(m_srcImage->imageData[i*Img->widthStep+j]);
				if(v > thresholdValue + gThreshAdjustPart )
					CV_IMAGE_ELEM(Img, uchar, i, j) = (uchar)(PerforRG);
			}
		}
	}

}

void SinSpmReco::OnExpandHead(IplImage * Img)	//ͷ����������
{
	// TODO: Add your command handler code here

	int Height = Img->height;
	int Width  = Img->width;
	int ByteWidth = Img->widthStep;

    // ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ�򻺴�ͼ���ָ��
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[Img->imageSize];
    
	memcpy(lpTemp, Img->imageData, Img->imageSize);

	int structure[10][10] = { 0,0,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,0,0 };

	// ѭ������
	long i;
	long j;
	int  n;
	int  m;

	// ʹ���Զ���ĽṹԪ�ؽ�������
	for(j = 0; j <Height-10; j++)
	{
		for(i = 0;i <Width-10; i++)
		{

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char*)(Img->imageData + j*ByteWidth + i);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
			lpDst = lpTemp+ j*ByteWidth+ i;

			for (m = 0; m < 10; m++ )
			{
				for (n = 0; n < 10; n++)
				{   
					if(structure[m][n]==0)
					   continue;
					if( *(lpSrc + m*ByteWidth + n + 0) == (uchar)(HEADRG) )  // ͷ��
					{	
                        int a, b;
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if( *(lpSrc + a*ByteWidth + b) == (uchar)(PerforRG) ) // ����
								{
									*(lpDst + a*ByteWidth + b) = (uchar)(HEADRG);
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

    memcpy(Img->imageData, lpTemp, Img->imageSize);
	delete lpTemp;

}

//ͷ����ʴ����
void SinSpmReco::OnErosionHead(IplImage * Img)
{
	// TODO: Add your command handler code here

	int Height = Img->height;
	int Width  = Img->width;
	int ByteWidth = Img->widthStep;

    // ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ�򻺴�ͼ���ָ��
	unsigned char* lpDst;
	unsigned char* lpTemp = new unsigned char[Img->imageSize];
    
	memcpy(lpTemp, Img->imageData, Img->imageSize);

	int structure[10][10] = { 0,0,1,1,1,1,1,1,0,0,
							0,1,1,1,1,1,1,1,1,0,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,
							0,1,1,1,1,1,1,1,1,0,
							0,0,1,1,1,1,1,1,0,0 };
	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
	for(j = 0; j <Height-10; j++)
	{
		for(i = 0; i <Width-10; i++)
		{

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char*)(Img->imageData + j*ByteWidth + i);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��
			lpDst = lpTemp + j*ByteWidth + i;

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
							if(*(lpSrc + a*ByteWidth + b) == (uchar)255 ) 
								goto nextpiexl;
						}
					}

					if( *(lpSrc + m*ByteWidth + n) == (uchar)(PerforRG) )  // ����
					{	                        
						for (a = 0; a < 10; a++ )
						{
							for (b = 0; b < 10; b++)
							{   
								if( *(lpSrc + a*ByteWidth + b) == (uchar)(HEADRG)) // ͷ��
								{
									*(lpDst + a*ByteWidth + b) = (uchar)(PerforRG);
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

    memcpy(Img->imageData, lpTemp, Img->imageSize);
	delete lpTemp;
}

SinSpmReco::SinSpmReco(int h, int w, unsigned char*data, int ImgSize, vector<POINT>& RegionPoints) : m_RegionPoints(RegionPoints)
{
    m_height = h;
	m_width = w;
	m_srcImage = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 1);
	memcpy(m_srcImage->imageData, data, ImgSize);
}

SinSpmReco::SinSpmReco(IplImage *Img, vector<POINT>& RegionPoints) : m_RegionPoints(RegionPoints)
{
    m_height = Img->height;
	m_width = Img->width;
	m_srcImage = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1);
	cvCopyImage(Img, m_srcImage);
}

IplImage* SinSpmReco::SegSperm()
{
	long height, width;
	height = m_height;
	width  = m_width;
	
	IplImage *Image, *binaryImage;
	Image = cvCreateImage(cvGetSize(m_srcImage), IPL_DEPTH_8U, 1);
	binaryImage = cvCreateImage(cvGetSize(m_srcImage), IPL_DEPTH_8U, 1);
	cvCopyImage(m_srcImage, Image);
	memset(binaryImage->imageData, 255, binaryImage->imageSize);
	
	int   i,j,n;	
	n = m_RegionPoints.size();
	int col, row;
	for(i=0; i<n; ++i)
	{       
		row = height-1-m_RegionPoints[i].x;
		col = m_RegionPoints[i].y;
		CV_IMAGE_ELEM(binaryImage, uchar, row, col) = (uchar)(0);
	}

	IGvfSnake theGvfSnake( height, width, Image, 1.0e2, 1.0e3, 85.6, 0.2, 5, 2, 150, 0.85);
	
	IplImage *savedImage = cvCreateImage(cvGetSize(binaryImage), IPL_DEPTH_8U, 1);
	memset(savedImage->imageData, 255, savedImage->imageSize);

	vector<skPoint>vkp;
	bool hr = GetInitSnake(binaryImage, vkp);
	if(!hr) return NULL;
	
	int k=0;
	for(k=0; k<vkp.size(); k++) {
		swap(vkp[k].dy, vkp[k].dx);
	}

	theGvfSnake.SnakeInitialize(vkp);
  	theGvfSnake.ShowSnakeDeform(binaryImage);

	CvPoint p = cvPoint(0,0);
	for(k=0; k<vkp.size(); k++) {
		p.x += vkp[k].dx;
		p.y += vkp[k].dy;
	}
	p.x /= vkp.size();
	p.y /= vkp.size();
	swap(p.x, p.y);
	Fill(binaryImage, p);
	cvCopyImage(binaryImage, savedImage);
	
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if( CV_IMAGE_ELEM(binaryImage, uchar, i, j) == (uchar)(0) )
			{
				CV_IMAGE_ELEM(binaryImage, uchar, i, j) = (uchar)(255);
			}
		}
	}
	
	IplImage *MopyImg = cvCreateImage(cvGetSize(binaryImage), IPL_DEPTH_8U, 1);
	cvCopyImage(binaryImage, MopyImg);

	const int iter = 3;
	
    cvErode(MopyImg, MopyImg, NULL, iter);

	IplImage *TailImg = cvCreateImage(cvGetSize(binaryImage), IPL_DEPTH_8U, 1);
	cvCopyImage(savedImage, TailImg);

	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if( CV_IMAGE_ELEM(MopyImg, uchar, i, j) == (uchar)(HEADRG) )
			{
				CV_IMAGE_ELEM(TailImg, uchar, i, j) = (uchar)(255);
			}
		}
	}
	
	RecoTail(TailImg);

	cvErode(TailImg, TailImg, NULL, iter+3); // ����β��

	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if( CV_IMAGE_ELEM(savedImage, uchar, i, j) == (uchar)(255) )
			{
				CV_IMAGE_ELEM(TailImg, uchar, i, j) = (uchar)(255);
			}
		}
	}
	
	OnTagOtsu(binaryImage);
	
	OnExpandHead(binaryImage);
	OnErosionHead(binaryImage);
	
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if( CV_IMAGE_ELEM(binaryImage, uchar, i, j) == (uchar)(255) &&
				CV_IMAGE_ELEM(TailImg, uchar, i, j) == (uchar)(0) )
			{
				CV_IMAGE_ELEM(binaryImage, uchar, i, j) = (uchar)(0);
			}
		}
	}
	
	hr = false;
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {
			if( CV_IMAGE_ELEM(binaryImage, uchar, i, j) == (uchar)(PerforRG) )
			{
				if( HasThePiexlInAround(0, i, j, binaryImage) )
				{
					p = cvPoint(j,i);
					hr = true;
					goto FIND;
				}
			}
		}
	}
FIND:;
	 if(hr) {
		 cvFloodFill(binaryImage, p, cvScalarAll(0), cvScalarAll(0), cvScalarAll(0), NULL, 8, NULL);
	 }
	 
	cvCopyImage(binaryImage, m_srcImage);
	cvReleaseImage(&Image);
	cvReleaseImage(&binaryImage);
	cvReleaseImage(&savedImage);
	cvReleaseImage(&MopyImg);
	cvReleaseImage(&TailImg);
	return m_srcImage;
}

SinSpmReco::~SinSpmReco()
{
	cvReleaseImage(&m_srcImage);
}

