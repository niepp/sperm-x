#include "IGvfSnake.h"

#ifndef _SIN_SPM_RECO_H
#define _SIN_SPM_RECO_H

class SinSpmReco{

private:
	int m_height, m_width;
	IplImage* m_srcImage;
	vector<POINT>m_RegionPoints;

public:
	SinSpmReco(int h, int w, unsigned char*data, int ImgSize, vector<POINT>& RegionPoints);
	SinSpmReco(IplImage *Img, vector<POINT>& RegionPoints);
	SinSpmReco(LPBITMAPINFOHEADER  lpBmInfoSrc, LPBYTE lpBmDataSrc, vector<POINT>& RegionPoints);
	IplImage* SegSperm();  // 形态分割
	virtual ~SinSpmReco();

protected:

	// 二值图腐蚀运算
	void Erosion(IplImage *Img);
	
	// 二值图膨胀运算
	void Expand(IplImage*Img, IplImage* lpSegSavedImg);
	
	bool HasThePiexlInAround(unsigned char piexl, int row, int col, IplImage* Img);

	bool IsEdgePt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img);
	
	bool IsObjectPt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img);
	
	CvPoint GetSeed(unsigned char objectpiexl, unsigned char bkpiexl, IplImage *Img);
	
	// 边界跟踪
	void EdgeTrack(unsigned char objectpiexl, unsigned char bkpiexl, CvPoint seed, IplImage *Img, stack<CvPoint>&vp);
	
	void SaveMaxAreaRegion(unsigned char objectpiexl, unsigned char bkpiexl, IplImage* Img);

	void GetskPoint(stack<CvPoint>&skp, vector<skPoint>&vkp, int r);
	
	bool GetInitSnake(IplImage* srcImage, vector<skPoint>&vkp);
	
	void Fill(IplImage* Img, CvPoint p);
	
	void GetReg(IplImage* Img, CvPoint p, vector<CvPoint>&reg);

	void RecoTail(IplImage* Img);

	//头部腐蚀运算
	void OnErosionTail(IplImage * Img);

	// 顶体和头部分割
	void OnTagOtsu(IplImage *Img);
	
	//头部膨胀运算
	void OnExpandHead(IplImage * Img);
	
	//头部腐蚀运算
	void OnErosionHead(IplImage * Img);
	
};

#endif //_SIN_SPM_RECO_H