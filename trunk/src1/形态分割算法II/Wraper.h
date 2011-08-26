#if !defined(_CWRAPER_H)
#define _CWRAPER_H

#include "SinSpmReco.h"
#include "SpermParamCalc.h"

class  CWraper
{
	// Construction
public:
	
	// standard constructor
	CWraper(LPBITMAPINFOHEADER  lpBmInfoSrc, LPBYTE lpBmDataSrc, vector<POINT>& RegionPoints, int r);
	
	virtual ~CWraper();
	
	bool OnSperMopySeg();
	void OnSperMopyParamCalc();
	
	SperMopyParameter  m_SperMopyParameter;  // ��̬����
	
	// ��ǰ����ͼ��
	LPBITMAPINFOHEADER m_lpDibIHPro;
	LPBYTE             m_lpDibDataPro;
	vector<POINT>      m_RegionPoints;

};

#endif // !defined(_CWRAPER_H)
