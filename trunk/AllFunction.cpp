

#include "stdafx.h"

#include "AllFunction.h"
#include "ctime"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int gThreshAdjust = 10;
double gGrowStep = 0.85;

bool operator==(const CvSize& a,const CvSize& b)
{
	return (a.width==b.width)&&(a.height==b.height);
}
bool operator!=(const CvSize& a,const CvSize& b)
{
	return !(a==b);
}

bool operator==(const CvPoint& a,const CvPoint& b)
{
	return a.x==b.x && a.y==b.y;
}
bool operator!=(const CvPoint& a,const CvPoint& b)
{
	return  !(a==b);
}
ostream& operator<<(ostream& os,const CvPoint& pt)
{
	os<<"("<<pt.y<<","<<pt.x<<")\t";
	return os;
}


void Deburr(IplImage* boundaryImage)
{
	int i,j;
    int dir[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
	for(i=1;i<boundaryImage->height-1;++i)
		for(j=1;j<boundaryImage->width-1;++j)
		{
			if((unsigned char)boundaryImage->imageData[i*boundaryImage->widthStep+j] == (unsigned char)255)
				continue;
			int sum=0;
			for(int k=0;k<8;++k)
			{
				int m=i+dir[k][0];
				int n=j+dir[k][1];
				if((unsigned char)boundaryImage->imageData[m*boundaryImage->widthStep+n] == (unsigned char)0)
					++sum;
			}
			if(sum<=1)
				boundaryImage->imageData[i*boundaryImage->widthStep+j]=(unsigned char)255;
		}
}

void DisconnectFromNarrow(IplImage* srcImage,int windowsize)
{
	assert( windowsize%2 == 1 );
	int i,j;
	for(i=windowsize/2;i<=srcImage->height-windowsize/2;++i)
		for(j=windowsize/2;j<=srcImage->width-windowsize/2;++j)
		{
			int m,n;
			int ct=0;
			for(m=i-windowsize/2;m<i+windowsize/2;++m)
				for(n=j-windowsize/2;n<j+windowsize/2;++n)
				{
					if((unsigned char)srcImage->imageData[m*srcImage->widthStep+n] == 0)
						++ct;
				}
			if(ct <= windowsize*windowsize/10 )
				srcImage->imageData[i*srcImage->widthStep+j]=(unsigned char)255;
		}
}
CvPoint GetCenterPoint(const vector<CvPoint>& contour)
{
	int i;
	CvPoint pt;
	pt.x=0; 
	pt.y=0;
	for(i=0;i<contour.size();++i)
	{
		pt.x+=contour[i].x;
		pt.y+=contour[i].y;
	}
	pt.x/=contour.size();
	pt.y/=contour.size();
	return pt;
}
bool IsLegalCoordinate(int m,int n,IplImage* img)
{
	if(m<0 || n<0 || m>=img->height || n>=img->width)
		return false;
	return true;
}
void ImageExpand(IplImage* srcImage,IplImage* dstImage,vector<REGION>& rgn)
{
	int i,j;
	for(i=0;i<rgn.size();++i)
	{
		CvPoint center=GetCenterPoint(rgn[i].contour);
		for(j=0;j<rgn[i].contour.size();++j)
		{
			double k;
			if(center.x!=rgn[i].contour[j].x)
			{
				k=double(rgn[i].contour[j].y-center.y) / (rgn[i].contour[j].x-center.x);
                CvPoint pre=rgn[i].contour[j];
				int delta=-1;
				CvPoint boundaryPoint=pre;
				for(int exp=1;exp<=10;++exp)
				{
                    CvPoint cur;
					int step=1;
					cur.x=pre.x+step;
					cur.y=k*step+pre.y;
					if(cur.x<0 || cur.x>=srcImage->width || cur.y<0 || cur.y >=srcImage->height)
						break;
					int valPre=(unsigned char)srcImage->imageData[pre.y*srcImage->widthStep+pre.x];
					int valCur=(unsigned char)srcImage->imageData[cur.y*srcImage->widthStep+cur.x];
                    if(abs(valPre-valCur) > delta)
					{
                        delta=abs(valPre-valCur);
						boundaryPoint = valCur < valPre ? cur:pre;
					}
					pre=cur;
				}
				rgn[i].contour[j]=boundaryPoint;
			}
		}
	}
	for(i=0;i<rgn.size();++i)
	{
		for(j=0;j<rgn[i].contour.size();++j)
		{
			CvPoint pt=rgn[i].contour[j];
			dstImage->imageData[pt.y*dstImage->widthStep+pt.x]=0;
		}
	}
}
void FindRegionContour(int** regionindex,int ROW,int COL,const vector<CvPoint>& region,vector<CvPoint>& contour)
{
    int direction[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
	for(int k=0;k<region.size();++k)
	{
		CvPoint pt=region[k];
		int dir = 0;
		for(dir=0;dir<GetArraySize(direction);dir+=GetArraySize(direction)/4)
		{
			int m=pt.y+direction[dir][0];
			int n=pt.x+direction[dir][1];
			if(m<0||n<0||m>=ROW||n>=COL)
				break;
			if(regionindex[m][n] != regionindex[pt.y][pt.x])
				break;
		}
		if(dir<GetArraySize(direction))
			contour.push_back(pt);
	}
}
void ImageSeg(IplImage* srcImage,IplImage* dstImage,vector<REGION>& rgn) 
{
	REGION xx;
	rgn.push_back(xx);
	clock_t start=clock();
	cvCopyImage(srcImage,dstImage); 
	int threshvalue=MyOTSUImage(dstImage);
    int backvalue=GetBackValue(srcImage);
	DeleteSmallBlackRegion(dstImage,400);
	FillSmallWhiteRegion(dstImage);
	FindRegionContour(dstImage,rgn);
	RegionGrowFromBG(srcImage,rgn,backvalue-(threshvalue+1),0);
	cvSet(dstImage,cvScalar(255));
	int i,j;
	for(i=0;i<rgn.size();++i)
	{
		if(rgn[i].contour.empty())
			continue;
		CvPoint pt=rgn[i].centerpoint;
		const int x=20; //?
		if(pt.x<=x || srcImage->width-pt.x<=x || pt.y<=x || srcImage->height-pt.y <=x)
		{
			rgn[i].contour.clear();
			rgn[i].region.clear();
			rgn[i].centerpoint.y=rgn[i].centerpoint.x=-10000;
		}
	}
	for(i=0;i<rgn.size();++i)
	{
		if(rgn[i].contour.empty())
			continue;
		for(j=0;j<rgn[i].region.size();++j)
		{
			CvPoint pt=rgn[i].region[j];
			dstImage->imageData[pt.y*dstImage->widthStep+pt.x]=0;
		}
	}

	vector<REGION>::iterator it;
	for(it=rgn.begin()+1;it!=rgn.end();) 
		if(find(it->region.begin(),it->region.end(),it->centerpoint) == it->region.end())
		{
			for(i=0;i<it->region.size();++i)
			{
				CvPoint pt=it->region[i];
				dstImage->imageData[pt.y*dstImage->widthStep+pt.x]=(unsigned char)255;
			}
			it->region.clear();
			it->contour.clear();
			rgn.erase(it );
		}
		else
			++it;
}
void HistgramEqualization(IplImage* src,IplImage* dst)
{
    CvHistogram *hist = 0;
    const int HDIM=256;
    int n = HDIM;     
    double nn[HDIM];
    uchar T[HDIM];
    CvMat *T_mat;
    
    int x;
    int sum = 0; // sum of pixels of the source image 图像中象素点的总和
    double val = 0;

    // calculate histgram 计算直方图
    hist = cvCreateHist( 1, &n, CV_HIST_ARRAY, 0, 1 );  
    cvCalcHist( &src, hist, 0, 0 ); 
    
    // Create Accumulative Distribute Function of histgram
    val = 0;
    for ( x = 0; x < n; x++)
    {
        val = val + cvGetReal1D (hist->bins, x);
        nn[x] = val;
    }

    // Compute intensity transformation 计算变换函数的离散形式
    sum = src->height * src->width;
    for( x = 0; x < n; x++ )
    {
        T[x] = (uchar) (255 * nn[x] / sum); // range is [0,255]
    }

    // Do intensity transform for source image
	cvCopyImage(src, dst);
    T_mat = cvCreateMatHeader( 1, 256, CV_8UC1 );
    cvSetData( T_mat, T, 0 );    
    // directly use look-up-table function 直接调用内部函数完成 look-up-table 的过程
    cvLUT( src, dst, T_mat ); 
    cvReleaseHist ( &hist );
}

void FindRegionContour(IplImage* binaryimage,vector<REGION>& vec)
{
	Deburr(binaryimage);
	int dir[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	int i,j,k;
	bool** flag=new bool*[binaryimage->height];
	for(i=0;i<binaryimage->height;++i)
	{
		flag[i]=new bool[binaryimage->width];
		memset(flag[i],false,sizeof(false)*binaryimage->width);
	}
	for(i=0;i<binaryimage->height;++i)
	{
		for(j=0;j<binaryimage->width;++j)
		{
			if((unsigned char)binaryimage->imageData[i*binaryimage->widthStep+j]!=(unsigned char)0)
				continue;
			if(flag[i][j]) continue;
			queue<CvPoint> temp;
			REGION region;
			temp.push(cvPoint(j,i));
			flag[i][j]=true;
			while (!temp.empty())
			{
				CvPoint pt=temp.front();
				temp.pop();
				region.region.push_back(pt);
				for(k=0;k<GetArraySize(dir);++k)
				{
					int m=pt.y+dir[k][0];
					int n=pt.x+dir[k][1];
					if(m<0 || m>=binaryimage->height || n<0 || n>=binaryimage->width)
						continue;
					if((unsigned char)binaryimage->imageData[m*binaryimage->widthStep+n]!=0)
						continue;
					if(flag[m][n])
						continue;
					temp.push(cvPoint(n,m));
					flag[m][n]=true;
				}
			}
			int sumX=0,sumY=0;
			for(k=0;k<region.region.size();++k)
				if(IsBoundary(binaryimage,region.region[k]))
				{
					region.contour.push_back(region.region[k]);
					sumX+=region.region[k].x;
					sumY+=region.region[k].y;
				}
			region.centerpoint.x=sumX/region.contour.size();
			region.centerpoint.y=sumY/region.contour.size();
			vec.push_back(region);
		}
	}
	for(i=0;i<binaryimage->height;++i)
		delete [](flag[i]);
	delete []flag;
}
void SearchIllegalNearPoint(IplImage* srcImage,int** regionindex,int index,int BGvalue
							,int difference,const CvPoint& pt,vector<CvPoint>& vec)
{
    int i,j;
	int direction[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
    for(int k=0;k<GetArraySize(direction);k+=GetArraySize(direction)/8)
	{
         i=pt.y + direction[k][0];
		 j=pt.x + direction[k][1];
		 if(IsLegalCoordinate(i,j,srcImage) == false)
			 continue;
		 int val2=(unsigned char)srcImage->imageData[i*srcImage->widthStep+j];
		 if(val2+difference >= BGvalue) continue;
		 if(regionindex[i][j] != -1 && regionindex[i][j]!=index)
			 vec.push_back(cvPoint(j,i));
	}
}
void DeleteAll8TypeArea(IplImage* srcImage,IplImage* binaryImage,vector< vector<CvPoint> >& AllOutline) 
{
	int i,j,k,m,n;
	int direction[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
	bool **flag;                      // need to be released
	flag=new bool*[binaryImage->height];
    for(i=0;i<binaryImage->height;++i)
	{
		flag[i]=new bool[binaryImage->width];
		memset(flag[i],false,sizeof(bool)*binaryImage->width);
	}	
	for(i=0;i<binaryImage->height;++i)
	{
		for(j=0;j<binaryImage->width;++j)
		{
			int val=(byte)binaryImage->imageData[i*binaryImage->widthStep+j];
			if(val!=0) continue;
			for(k=0;k<GetArraySize(direction);++k)
			{
				int m=i+direction[k][0];
				int n=j+direction[k][1];
				if(m<0||n<0||m>=binaryImage->height||n>=binaryImage->width)
					continue;
				val=(byte)binaryImage->imageData[m*binaryImage->widthStep+n];
				if(val==0)
					break;
			}
			if(k==GetArraySize(direction))
				binaryImage->imageData[i*binaryImage->widthStep+j]=(unsigned char)255;
		}
	}
	for(i=0;i<binaryImage->height;++i)
		for(j=0;j<binaryImage->width;++j)
		{
			if(IsBoundary(binaryImage,cvPoint(j,i)) == false )
				continue;
			if(flag[i][j]) continue;
            vector<CvPoint> outline;
			outline.push_back(cvPoint(j,i));
			flag[i][j] = true;
			CvPoint StartPoint=cvPoint(j,i);
			int preDir=0;
            while ( !outline.empty() )
            {
				CvPoint pt=outline.back();
				int times;
				for(k=preDir,times=0;times<GetArraySize(direction);k=(k+1)%GetArraySize(direction),++times)
				{
					m=pt.y+direction[k][0];
					n=pt.x+direction[k][1];
					if(m<0||m>=binaryImage->height || n<0 || n>= binaryImage->width)
						continue;
					int val=(unsigned char)binaryImage->imageData[m*binaryImage->widthStep+n];
					if(val == 255)
						continue;
					if(flag[m][n] == false)
					{
						outline.push_back(cvPoint(n,m));
						flag[m][n]=true;
						preDir=k-GetArraySize(direction)/4;
						preDir=(preDir+GetArraySize(direction))%GetArraySize(direction);
						break;
					}
					if( cvPoint(n,m)== StartPoint )
					{
						AllOutline.push_back(outline);
						{							
							for(int i=0;i<outline.size();++i)
							{
								CvPoint pt=outline[i];
								srcImage->imageData[pt.y*srcImage->widthStep+pt.x]=0;
							}
							printf("first point:%d %d\n",outline[0].y,outline[0].x);
							cvShowImage("srcImage2",srcImage);
							cvWaitKey();
						}
                        outline.clear();
						break;
					}
					else
					{
						vector<CvPoint> SmallOutline;
						CvPoint temp=outline.back();
						vector<CvPoint>::reverse_iterator rit=outline.rbegin();
						while (*rit != temp)
						     ++rit;
						copy(outline.rbegin(),rit,back_insert_iterator< vector<CvPoint> >(SmallOutline));
						if(SmallOutline.size()>1)
						{
							AllOutline.push_back(SmallOutline);
							for(int i=0;i<SmallOutline.size();++i)
							{
								CvPoint pt=SmallOutline[i];
								srcImage->imageData[pt.y*srcImage->widthStep+pt.x]=0;
							}
							printf("first point:%d %d\n",SmallOutline[0].y,SmallOutline[0].x);
							cvShowImage("srcImage2",srcImage);
							cvWaitKey();
						}
					    outline.erase((++rit).base(),outline.end());
						binaryImage->imageData[temp.y*binaryImage->widthStep+temp.x]=(unsigned char)255;
						break;
					}
				}
			}
		}
	for(i=0;i<binaryImage->height;++i)
		delete [](flag[i]);
	delete []flag;

}
void TraceAllBoundary(IplImage* srcImage,vector< vector<CvPoint> >& bound)
{
	int direction[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
	int i,j,k,m,n;
	bool **flag;                      // need to be released
	flag=new bool*[srcImage->height];
    for(i=0;i<srcImage->height;++i)
	{
		flag[i]=new bool[srcImage->width];
		memset(flag[i],false,sizeof(bool)*srcImage->width);
	}
	for(i=0;i<srcImage->height;++i)
	{
		for(j=0;j<srcImage->width;++j)
		{
			if((unsigned char)srcImage->imageData[i*srcImage->widthStep+j] !=0 ||flag[i][j])
				continue;
			for(k=0;k<8;++k)
			{
				m= i+direction[k][0];
				n= j+direction[k][1];
				if(flag[m][n])
					break;
			}
			if(k<8) continue;
			int p=i,q=j;
            k=0;
			vector<CvPoint> temp;
			while (flag[p][q] == false)    //  找一条边界
			{	
				flag[p][q] = true;
				temp.push_back(cvPoint(q,p));
				int dir;
				for(dir=(k-2+8)%8;;dir=(dir+1)%8)
				{
					m = p+direction[dir][0];
					n = q+direction[dir][1];
					if( (unsigned char)srcImage->imageData[m*srcImage->widthStep+n] == 0)
						break;
				}	
				k=dir;
				p=m;q=n;
			}
			if(temp.size() > 1)
				bound.push_back(temp);
		}			
	}
	for(i=0;i<srcImage->height;++i)
		delete [](flag[i]);
	delete []flag;
}

bool IsBoundary(IplImage* srcImage,CvPoint pt)
{
	int dir[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	int val=(unsigned char)srcImage->imageData[pt.y*srcImage->widthStep+pt.x];
	if(val == 255 ) return false;
	for(int i=0;i<GetArraySize(dir);++i)
	{
		int m=pt.y+dir[i][0];
		int n=pt.x+dir[i][1];
		if(m<0 || m>= srcImage->height || n<0 || n>=srcImage->width)  //  下标不合法
			return true;
		val=(unsigned char)srcImage->imageData[m*srcImage->widthStep+n];
		if(val == 255) { return true;}
	}
	return false;
}

void Output(IplImage* img,const char* filename)
{
	FILE* fp=fopen(filename,"w");
	int i,j;
	for(i=0;i<img->height;++i)
	{
		for(j=0;j<img->width;++j)
			fprintf(fp,"%4d",(unsigned char)img->imageData[i*img->widthStep+j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
void Output( int** data,const int m,const int n,const char* filename)
{
	FILE* fp=fopen(filename,"w");
	int i,j;
	for(i=0;i<m;++i)
	{
		for(j=0;j<n;++j)
			fprintf(fp,"%4d",data[i][j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
int MyOTSUImage(IplImage* sourceImage)
{
	IplImage *tempImg=0;
    long width=sourceImage->width;
	long height=sourceImage->height;
    long bytelinegray = WIDTHBYTES(width*8);
	tempImg=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);             // need to be released
	//	unsigned char temppixel;
	int i,j;
	unsigned char thresholdValue=1; // 阈值 	
	int ihist[256];       //  图像直方图，256个点 
	//sum为总的，m为sum的平均值，csum为n1部分的总数，m1，m2为两部分的平均
    double m,m1,m2,sum,csum,fmax,sb; 
	//n记录总个数,n1,n2为两部分的个数
    int n,n1,n2; 
	// 对直方图置零... 
	memset(ihist,0,sizeof(ihist)); 
	// 生成直方图 
	for(i=0;i<width;i++) 
		for(j=0;j<height;j++) 
		{ 	       
			ihist[(unsigned char)*(sourceImage->imageData+bytelinegray*(height - 1 - j)+i)]++; 		        
		} 
		sum=csum=0.0; 
		n=0; 
		for(i=0;i<=255;i++)  
		{ 	
			sum+=(double)i*(double)ihist[i];   //x*f(x) 质量矩
			n+=ihist[i];                           // f(x)   质量    	
		} 
		m=sum/n; //求sum的平均
		fmax=-1.0; 
		n1=0; 
		for(i=0;i<255;i++) 
		{ 	
			n1+=ihist[i]; 	
			if(!n1)  	
				continue; 	
			n2=n-n1; 	
			if(n2==0)  	
				break; 		
			csum+=(double)i*ihist[i]; 	
			m1=csum/n1; 	
			m2=(sum-csum) / n2;   
			sb=(double)n1*(m1-m)*(m1-m)+(double)n2*(m2-m)*(m2-m); 
			if(sb>fmax)  
			{ 		
				fmax=sb; 	
				thresholdValue=(unsigned char)i; 	
			} 
		} 
		thresholdValue += gThreshAdjust;
		for(i=0;i<width;i++)
		{
			for(j=0;j<height;j++)
			{ 					
				unsigned char temp=*(sourceImage->imageData+bytelinegray*(height - 1 - j)+i);
				if( temp< thresholdValue )
					*(tempImg->imageData+bytelinegray*(height - 1 - j)+i)=(unsigned char)0;
				else
					*(tempImg->imageData+bytelinegray*(height - 1 - j)+i)=(unsigned char)255;
			} 
		}	

		for(j=0;j<height;j++)
		{
			for(i=0;i<width;i++)
			{
				*(sourceImage->imageData+bytelinegray*(height-1-j)+i)=
					*(tempImg->imageData+bytelinegray*(height-1-j)+i);
			}
		}
	cvReleaseImage(&tempImg);
    return thresholdValue;
}

void DeleteSmallBlackRegion(IplImage* sourceImage,size_t num)
{
	int dir[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	vector< vector<CvPoint> > regions;
    vector<CvPoint> temp;
	int i,j;
	bool** flag;                   // need to be released
	flag= new bool*[sourceImage->height];
	for(i=0;i<sourceImage->height;++i)
	{
		flag[i]=new bool[sourceImage->width];
		memset(flag[i],false,sizeof(bool)*sourceImage->width);
	}
	for(i=0;i<sourceImage->height;++i)
		for(j=0;j<sourceImage->width;++j)
		{
			if(flag[i][j])  continue;
		
			int val=(unsigned char)sourceImage->imageData[i*sourceImage->widthStep+j];
			if(val == 255) continue;
			
			queue<CvPoint> q;
            flag[i][j]=true;
			q.push(cvPoint(j,i));
			temp.clear();
			while (!q.empty())
			{
				CvPoint pt=q.front();
				temp.push_back(pt);
				q.pop();
				for(int k=0;k<GetArraySize(dir);++k)
				{
					int m=pt.y+dir[k][0];
					int n=pt.x+dir[k][1];
					if(m<0 || m>= sourceImage->height || n<0 || n>=sourceImage->width	
						) continue;

					int val=(unsigned char)sourceImage->imageData[m*sourceImage->widthStep+n];
					if( flag[m][n] || val == 255 ) 
						continue;
                    flag[m][n]=true;
					q.push(cvPoint(n,m));
				}
			}
			int sz=temp.size();
			if( temp.size() < num )
				regions.push_back(temp);
		}
	for(i=0;i<regions.size();++i)
	{
		for(j=0;j<regions[i].size();++j)
			sourceImage->imageData[regions[i][j].y * sourceImage->widthStep+regions[i][j].x]=(unsigned char)255;
	}
	for(i=0;i<sourceImage->height;++i)
		delete [] flag[i];
	delete []flag;
}

void FillSmallWhiteRegion(IplImage* sourceImage,size_t num)   //  填充目标区域中的背景
{
	int dir[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	vector< vector<CvPoint> > regions;
    vector<CvPoint> temp;
	int i,j;
	bool** flag;                        // need to be released
	flag= new bool*[sourceImage->height];
	for(i=0;i<sourceImage->height;++i)
	{
		flag[i]=new bool[sourceImage->width];
		memset(flag[i],false,sizeof(bool)*sourceImage->width);
	}
	for(i=0;i<sourceImage->height;++i)
		for(j=0;j<sourceImage->width;++j)
		{
			if(flag[i][j])  continue;	
			int val=(unsigned char)sourceImage->imageData[i*sourceImage->widthStep+j];
			if(val == 0) continue;
			queue<CvPoint> q;
            flag[i][j]=true;
			q.push(cvPoint(j,i));
			temp.clear();
			while (!q.empty())
			{
				CvPoint pt=q.front();
				temp.push_back(pt);
				q.pop();
				for(int k=0;k<GetArraySize(dir);++k)
				{
					int m=pt.y+dir[k][0];
					int n=pt.x+dir[k][1];
					if(m<0 || m>= sourceImage->height || n<0 || n>=sourceImage->width	
						) continue;

					int val=(unsigned char)sourceImage->imageData[m*sourceImage->widthStep+n];
					if( flag[m][n] || val == 0 ) 
						continue;
                    flag[m][n]=true;
					q.push(cvPoint(n,m));
				}
			}
			int sz=temp.size();
			if( temp.size() < num )
				regions.push_back(temp);
		}
	for(i=0;i<regions.size();++i)
	{
		for(j=0;j<regions[i].size();++j)
			sourceImage->imageData[regions[i][j].y * sourceImage->widthStep+regions[i][j].x]=0;
	}
	for(i=0;i<sourceImage->height;++i)
		delete []flag[i];
	delete []flag;
}

int GetBackValue(IplImage* srcImage)
{
	int i,j;
	size_t num[256];
	memset(num,0,sizeof(num));
	for(i=0;i<srcImage->height;++i)
		for(j=0;j<srcImage->width;++j){
			int val=(unsigned char)srcImage->imageData[srcImage->widthStep * i +j];
			++num[val];
		}
	size_t maxval = num[0] ;
	int backGrayValue = 0 ;
	for(i=1;i<256;++i)
		if(maxval < num[i] ){
			maxval = num[i];
			backGrayValue=i;
	}
	return backGrayValue;
}

int SegForEachFromBG(IplImage* srcImage,int** regionindex,int BKGrayValue,int difference,vector<REGION>& vec)
{
    int direction[8][2] = {   {-1,0},{-1,1},{0,1},{1,1}
	,{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
	int i,j,k;
	int delta=0;
	for(i=1;i<vec.size();++i)
	{	
	    if(vec[i].region.size()>75*75) continue;
		for(j=0;j<vec[i].contour.size();++j)
		{
			queue<CvPoint> q;
			q.push(vec[i].contour[j]);
			while (!q.empty())
			{
				CvPoint pt=q.front();
				q.pop();
				for(k=0;k<GetArraySize(direction);k+=GetArraySize(direction)/4)
				{
					int m=pt.y+direction[k][0];
					int n=pt.x+direction[k][1];
					if(IsLegalCoordinate(m,n,srcImage) == false)
						continue;
					if(regionindex[m][n] != -1) continue;
					int val=(unsigned char)srcImage->imageData[m*srcImage->widthStep+n];
					if(val>=BKGrayValue-difference) 
						continue;
					vector<CvPoint> temp;
					SearchIllegalNearPoint(srcImage,regionindex,regionindex[pt.y][pt.x]
						,BKGrayValue,difference,cvPoint(n,m),temp);
					if(temp.empty())
					{
						++delta;
						regionindex[m][n] = regionindex[pt.y][pt.x];
						vec[regionindex[m][n]].region.push_back(cvPoint(n,m));
						q.push(cvPoint(n,m));
					}
				}
			}
		}
	}
	for(i=1;i<vec.size();++i)
	{
		vec[i].contour.clear();
		FindRegionContour(regionindex,srcImage->height,srcImage->width,
			vec[i].region,vec[i].contour);
	}
	return delta;
}
void RegionGrowFromBG(IplImage* srcImage,vector<REGION>& rgn,int startvalue,int endvalue)
{
    int direction[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}}; //  在图像中顺时针旋转
    int i,j;
	bool **flag=new bool*[srcImage->height];
    int** regionindex=new int*[srcImage->height];
    for(i=0;i<srcImage->height;++i){
		flag[i]=new bool[srcImage->width];
		regionindex[i]=new int[srcImage->width];
		memset(flag[i],false,sizeof(bool)*srcImage->width);
		memset(regionindex[i],-1,sizeof(int)*srcImage->width);
	}
	int difference;
	int backGrayValue=GetBackValue(srcImage);
	for(i=1;i<rgn.size();++i)
	{
		int sumX=0,sumY=0;
		int sz=rgn[i].contour.size();
		for(j=0;j<sz;++j)
		{
			CvPoint pt=rgn[i].contour[j];
			sumX+=pt.x;
			sumY+=pt.y;
		}
		rgn[i].centerpoint.x=sumX/sz;
		rgn[i].centerpoint.y=sumY/sz;
		
		for(j=0;j<rgn[i].region.size();++j)
		{
			CvPoint pt=rgn[i].region[j];
			regionindex[pt.y][pt.x]=i;
		}
	}
	if(startvalue>endvalue)
		startvalue^=endvalue,endvalue^=startvalue,startvalue^=endvalue;
	int index=0,delta[256],sumDelta=0;
	int step=(endvalue-startvalue) * gGrowStep;;  // 这个值很敏感
 	
	for( difference=endvalue;difference>=startvalue;--difference)
	{
		int d=SegForEachFromBG(srcImage,regionindex,backGrayValue,difference,rgn); 
		delta[index++]=d;
		if(index<=step)
		{
			sumDelta+=delta[index-1];
		}
		else
		{
			int avg=sumDelta/step;
			if(delta[index-1]>avg)
				break;
			sumDelta-=delta[index-step-1];
			sumDelta+=delta[index-1];
		}
	}
	
	for(i=0;i<srcImage->height;++i)
	{
		delete [](flag[i]);
		delete [](regionindex[i]);
	}
	delete []flag;
	delete []regionindex;
}

void GetTableColumnNames(const _ConnectionPtr& pCn, std::set<CString>column_names,  const CString& table_name)
{
	_RecordsetPtr pRecordset;  
	pRecordset.CreateInstance(__uuidof(Recordset));  
	pRecordset->Open((LPCTSTR)table_name.GetString(), pCn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdTable);  
	FieldsPtr fds = pRecordset->GetFields();  
	for(int i = 0; i < fds->GetCount(); i++)
	{
		FieldPtr fd = fds->GetItem(_variant_t(short(i)));
		column_names.insert((LPCTSTR)fd->GetName());
	}
}


bool IsTableExist(const _ConnectionPtr& pCn, const CString &table)
{
	_RecordsetPtr pRstSchema = pCn->OpenSchema(adSchemaTables); 
 	while (!pRstSchema->EndOfFile)
	{
		_bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")-> Value;//获取表的名称
		if(strcmp((char*)table_name, table.GetString()) == 0)
		{
			return true;
		}

		pRstSchema->MoveNext();
	}
	return false;
}
