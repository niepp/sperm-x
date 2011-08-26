
#ifndef _ALLFUNCTION_H
#define _ALLFUNCTION_H

#include "AllDataStructure.h"

#define GetArraySize(a) (sizeof(a)/sizeof(a[0]))

void Output( IplImage* img,const char* filename);
void Output( int** data,const int m,const int n,const char* filename);

void TraceAllBoundary(IplImage* srcImage,vector< vector<CvPoint> >& bound);

void FindRegionContour(int** regionindex,int ROW,int COL,const vector<CvPoint>& region,vector<CvPoint>& contour);
void FindRegionContour(IplImage* binaryimage,vector<REGION>& vec);

int MyOTSUImage(IplImage* sourceImage);
void DeleteSmallBlackRegion(IplImage* sourceImage,size_t num=100);
void FillSmallWhiteRegion(IplImage* sourceImage,size_t num=100);   //  填充目标区域中的背景

void RegionGrowFromBG(IplImage* srcImage,vector<REGION>& rgn,int startvalue,int endvalue);   //  rgn里存放起始轮廓

                              //   生成srcImage关于前景的二值图.
bool IsBoundary(IplImage* srcImage,CvPoint pt);
void HistgramEqualization(IplImage* src,IplImage* dst);

void ImageSeg(IplImage* srcImage,IplImage* dstImage,vector<REGION>& rgn);
	      // method = 1 直方图均衡化， 2 OTSU，3 区域增长
void ImageExpand(IplImage* srcImage,IplImage* dstImage,vector<REGION>& rgn);
CvPoint GetCenterPoint(const vector<CvPoint>& contour);
void Deburr(IplImage* boundaryImage);
void DisconnectFromNarrow(IplImage* srcImage,int windowsize); //  奇数
int GetBackValue(IplImage* dstimage);
void DeleteAll8TypeArea(IplImage* srcImage,IplImage* binaryImage,vector< vector<CvPoint> >& AllOutline) ;  //         去掉所有8字形区域
int SegForEachFromBG(IplImage* srcImage,int** regionindex,int startvalue,int difference,vector<REGION>& vec);
bool operator==(const CvPoint& a,const CvPoint& b);
bool operator!=(const CvPoint& a,const CvPoint& b);
bool operator==(const CvSize& a,const CvSize& b);
bool operator!=(const CvSize& a,const CvSize& b);

bool IsLegalCoordinate(int m,int n,IplImage* img);
void SearchIllegalNearPoint(IplImage* srcImage,int** regionindex,int index,int BGvalue
							,int difference,const CvPoint& pt,vector<CvPoint>& vec);
ostream& operator<<(ostream& os,const CvPoint& pt);


void GetTableColumnNames(const _ConnectionPtr& pCn, std::set<CString>column_names,  const CString& table_name);
bool IsTableExist(const _ConnectionPtr& pCn, const CString &table);

#endif