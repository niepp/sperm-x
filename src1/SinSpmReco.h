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
	IplImage* SegSperm();  // ��̬�ָ�
	virtual ~SinSpmReco();

protected:

	// ��ֵͼ��ʴ����
	void Erosion(IplImage *Img);
	
	// ��ֵͼ��������
	void Expand(IplImage*Img, IplImage* lpSegSavedImg);
	
	bool HasThePiexlInAround(unsigned char piexl, int row, int col, IplImage* Img);

	bool IsEdgePt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img);
	
	bool IsObjectPt(unsigned char objectpiexl, unsigned char bkpiexl, int row, int col, IplImage* Img);
	
	CvPoint GetSeed(unsigned char objectpiexl, unsigned char bkpiexl, IplImage *Img);
	
	// �߽����
	void EdgeTrack(unsigned char objectpiexl, unsigned char bkpiexl, CvPoint seed, IplImage *Img, stack<CvPoint>&vp);
	
	void SaveMaxAreaRegion(unsigned char objectpiexl, unsigned char bkpiexl, IplImage* Img);

	void GetskPoint(stack<CvPoint>&skp, vector<skPoint>&vkp, int r);
	
	bool GetInitSnake(IplImage* srcImage, vector<skPoint>&vkp);
	
	void Fill(IplImage* Img, CvPoint p);
	
	void GetReg(IplImage* Img, CvPoint p, vector<CvPoint>&reg);

	void RecoTail(IplImage* Img);

	//ͷ����ʴ����
	void OnErosionTail(IplImage * Img);

	// �����ͷ���ָ�
	void OnTagOtsu(IplImage *Img);
	
	//ͷ����������
	void OnExpandHead(IplImage * Img);
	
	//ͷ����ʴ����
	void OnErosionHead(IplImage * Img);
	
};

#endif //_SIN_SPM_RECO_H