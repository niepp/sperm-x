#include "stdafx.h"
#include "ImageProcess.h"
#include "IGvfSnake.h"

IGvfSnake::IGvfSnake(   
					long height,       //  图像区域高
					long width,        //  图像区域宽
					IplImage* ppImageData, // 8位灰度图像数据 
			
					/* Snake形变参数---各项权重系数 */
					double alpha,      // alpha  elasticity parameter
					double beta,       // beta   rigidity parameter
					double gama,       // gama   external force weight

					double u,          // gvf场参数
					int  nMaxDist,     // 大于我，就在相邻控制点间插值
					int  nMinDist,     // 小于我，就合并相邻控制点
					int  nMaxIterT,    // 最大迭代次数	
					double dp          // 稳定控制点的比例大于这个值就迭代停止
					
			      ) : m_height(height),
				  m_width(width),
				  m_ppImageData(ppImageData),
				  m_alpha(alpha),
				  m_beta(beta),
				  m_gama(gama),
				  m_u(u),
				  m_nMaxDist(nMaxDist),
				  m_nMinDist(nMinDist),
				  m_nMaxIterT(nMaxIterT),
				  m_dp(dp)
				  
{
	m_nIterT = 0;
	m_ppGVF  = 0;
}

IGvfSnake::~IGvfSnake()
{
	if(m_ppGVF) {
		int i = 0;
		for(i=0; i<m_height; ++i)
		{
			delete []m_ppGVF[i];
			m_ppGVF[i] = 0;
		}
		delete []m_ppGVF;
		m_ppGVF = 0;
	}
	if(m_ppf){
		int i = 0;
		for(i=0; i<m_height; ++i)
		{
			delete []m_ppf[i];
			m_ppf[i] = 0;
		}
		delete []m_ppf;
		m_ppf = 0;
	}
}

// Snake初始化: 设定初始轮廓，计算图像能量
void IGvfSnake::SnakeInitialize()
{
	GetEdgeImage();
	GvfSolver(45);
}

void IGvfSnake::SnakeInitialize(const vector<skPoint>& initSnakeContour)
{
	m_vSnake = initSnakeContour;
	GetEdgeImage();
	GvfSolver(45);
} 

// 直接用梯度值场作为EdgeImage，可以换换sobel或canny算子的结果做EdgeImage
void IGvfSnake::GetEdgeImage()
{
	long i, j;
    m_ppf = new double*[m_height];
	for(j=0; j<m_height; ++j) {
		m_ppf[j] = new double[m_width];
	}	
	for(i=0; i<m_height; i++) {
		for(j=0; j<m_width; j++) {
			m_ppf[i][j] = 0.0;
		}
	}
	long lw = m_ppImageData->widthStep;
	double gX, gY;
	for(i=1; i<m_height; i++) {
		for(j=1; j<m_width; j++) {
			gX = (unsigned char)m_ppImageData->imageData[i*lw+j] - (unsigned char)m_ppImageData->imageData[i*lw+j-1];
			gY = (unsigned char)m_ppImageData->imageData[i*lw+j] - (unsigned char)m_ppImageData->imageData[(i-1)*lw+j];
			m_ppf[i][j] = sqrt(gX*gX + gY*gY);
		}
	}
}

void IGvfSnake::GvfSolver(int iterT)
{
	int i, j, k, t;
	m_ppGVF = new skVector*[m_height];
	for(k=0; k<m_height; k++){
		m_ppGVF[k] = new skVector[m_width];
	    memset(m_ppGVF[k], 0, sizeof(skVector)*m_width);
	}
	double **GradFx;          // df/dx
	double **GradFy;          // df/dy
	double **b;               // b(x,y)
	double **c1;              // c1(x,y)
	double **c2;              // c2(x,y)
    
	GradFx = new double*[m_height];
	for(k=0; k<m_height; k++)
		GradFx[k] = new double[m_width];

	GradFy = new double*[m_height];
	for(k=0; k<m_height; k++)
		GradFy[k] = new double[m_width];

	b = new double*[m_height];
	for(k=0; k<m_height; k++)
		b[k] = new double[m_width];
		
	c1 = new double*[m_height];
	for(k=0; k<m_height; k++)
		c1[k] = new double[m_width];
	
	c2 = new double*[m_height];
	for(k=0; k<m_height; k++)
		c2[k] = new double[m_width];

   /***************I:规一化Edge map: f(x,y)*****************/

	int iMax = -INF;
	int iMin =  INF;
	for(i=0; i<m_height; i++){
		for(j=0; j<m_width; j++){
			if(m_ppf[i][j] > iMax)
               iMax = m_ppf[i][j];
			if(m_ppf[i][j] < iMin)
               iMin = m_ppf[i][j];
		}
	}
	for(i=0; i<m_height; i++){
		for(j=0; j<m_width; j++){
		   m_ppf[i][j] = (m_ppf[i][j] - iMin)/(iMax-iMin);
		}
	}

   /***************II:计算 Edge map 的梯度: Grad f(x,y)*****************/
	
   // df/dx : [f(i,j) - f(i,j-1)]/2
   // df/dy : [f(i,j) - f(i-1,j)]/2

   /* Deal with corners */
   GradFx[0][0] = 0;
   GradFx[m_height-1][0] = 0;
   GradFx[0][m_width-1] = 0; 
   GradFx[m_height-1][m_width-1] = 0;

   GradFy[0][0] = 0;
   GradFy[m_height-1][0] = 0;
   GradFy[0][m_width-1] = 0;
   GradFy[m_height-1][m_width-1] = 0;

   /* Deal with left and right column */
   for(i=1; i<m_height-1; i++){
       GradFx[i][0] = 0;
	   GradFx[i][m_width-1] = 0;
	   GradFy[i][0] = (m_ppf[i+1][0]-m_ppf[i-1][0])/2.0;
	   GradFy[i][m_width-1] = (m_ppf[i+1][m_width-1]-m_ppf[i-1][m_width-1])/2.0;
   }

   /* Deal with top and bottom row */
   for(j=1; j<m_width-1; j++){
	   GradFx[0][j] = (m_ppf[0][j+1]-m_ppf[0][j-1])/2.0;
	   GradFx[m_height-1][j] = (m_ppf[m_height-1][j+1]-m_ppf[m_height-1][j-1])/2.0;  
	   GradFy[0][j] = 0;
	   GradFy[m_height-1][j] = 0;
   }
   
   /* Compute interior derivative using central difference */
   for(i=1; i<m_height-1; i++){
      for(j=1; j<m_width-1; j++){
          GradFx[i][j] = (m_ppf[i][j+1] - m_ppf[i][j-1])/2.0;
          GradFy[i][j] = (m_ppf[i+1][j] - m_ppf[i-1][j])/2.0;
	  }
   }

   /******* III: Compute parameters and initializing arrays *******/
   // b(x,y), c1(x,y), c2(x,y)
   for(i=0; i<m_height; i++){
      for(j=0; j<m_width; j++){
           b[i][j] = GradFx[i][j]*GradFx[i][j] + GradFy[i][j]*GradFy[i][j];
          c1[i][j] = b[i][j]*GradFx[i][j];
		  c2[i][j] = b[i][j]*GradFy[i][j];
	  }
   }

   // 迭代初始值: V = Grad(f)
   for(i=0; i<m_height; i++){
	  for(j=0; j<m_width; j++){
          m_ppGVF[i][j].dx = GradFx[i][j];
          m_ppGVF[i][j].dy = GradFy[i][j];		         
	  }
   }

	double dx, dy, dt; // 迭代步长
	double r;
	dx = 1;
	dy = 1;
	dt = 1;
    /*------------------------------------------------------------
	|                                                             |
	|   注意-> Courant-Friedrichs-Lewy 条件: dt<= dx*dy / 4*miu   |
    |                                                             |
	-------------------------------------------------------------*/
	r = m_u*dt/(dx*dy);  
 
	/************* Solve GVF = (u,v) iteratively *************/
    // Laplacian u(x,y) and Laplacian v(x,y)
    double **Lu, **Lv;  
	// 分配内存空间
	Lu = new double*[m_height];
	for(k=0; k<m_height; k++)
		Lu[k] = new double[m_width];

	Lv = new double*[m_height];
	for(k=0; k<m_height; k++)
		Lv[k] = new double[m_width];
  
   /* IV: Compute Laplace operator using Neuman condition */
	for(t=0; t<iterT; t++)
	{
		/* IV.1: Deal with corners */
		Lu[0][0] = (m_ppGVF[0][1].dx + m_ppGVF[1][0].dx)/2.0 - m_ppGVF[0][0].dx; 
		Lv[0][0] = (m_ppGVF[0][1].dy + m_ppGVF[1][0].dy)/2.0 - m_ppGVF[0][0].dy; 
		Lu[0][m_width-1] = (m_ppGVF[0][m_width-2].dx + m_ppGVF[1][m_width-1].dx)/2.0 - m_ppGVF[0][m_width-1].dx;
		Lv[0][m_width-1] = (m_ppGVF[0][m_width-2].dy + m_ppGVF[1][m_width-1].dy)/2.0 - m_ppGVF[0][m_width-1].dy;
		Lu[m_height-1][0] = (m_ppGVF[m_height-2][0].dx + m_ppGVF[m_height-1][1].dx)/2.0 - m_ppGVF[m_height-1][0].dx;
		Lv[m_height-1][0] = (m_ppGVF[m_height-2][0].dy + m_ppGVF[m_height-1][1].dy)/2.0 - m_ppGVF[m_height-1][0].dy;
		Lu[m_height-1][m_width-1] = (m_ppGVF[m_height-2][m_width-1].dx + m_ppGVF[m_height-1][m_width-2].dx)/2.0 - m_ppGVF[m_height-1][m_width-1].dx;
		Lv[m_height-1][m_width-1] = (m_ppGVF[m_height-2][m_width-1].dy + m_ppGVF[m_height-1][m_width-2].dy)/2.0 - m_ppGVF[m_height-1][m_width-1].dy;
		
		/* IV.2: Deal with left and right columns */
		for(i=1; i<m_height-1; i++){
			Lu[i][0] = (2*m_ppGVF[i][1].dx + m_ppGVF[i-1][0].dx + m_ppGVF[i+1][0].dx)*0.25 - m_ppGVF[i][0].dx;		     
			Lv[i][0] = (2*m_ppGVF[i][1].dy + m_ppGVF[i-1][0].dy + m_ppGVF[i+1][0].dy)*0.25 - m_ppGVF[i][0].dy;
			Lu[i][m_width-1] = (2*m_ppGVF[i][m_width-1].dx + m_ppGVF[i-1][m_width-1].dx + m_ppGVF[i+1][m_width-1].dx)*0.25 - m_ppGVF[i][m_width-1].dx;	
			Lv[i][m_width-1] = (2*m_ppGVF[i][m_width-1].dy + m_ppGVF[i-1][m_width-1].dy + m_ppGVF[i+1][m_width-1].dy)*0.25 - m_ppGVF[i][m_width-1].dy;	
		}
		
		/* IV.3: Deal with top and bottom rows */
		for(j=1; j<m_width-1; j++){
			Lu[0][j] = (2*m_ppGVF[1][j].dx + m_ppGVF[0][j-1].dx + m_ppGVF[0][j+1].dx)*0.25 - m_ppGVF[0][j].dx;		     
			Lv[0][j] = (2*m_ppGVF[1][j].dy + m_ppGVF[0][j-1].dy + m_ppGVF[0][j+1].dy)*0.25 - m_ppGVF[0][j].dy;	
			Lu[m_height-1][j] = (2*m_ppGVF[m_height-2][j].dx + m_ppGVF[m_height-1][j-1].dx + m_ppGVF[m_height-1][j+1].dx)*0.25 - m_ppGVF[m_height-1][j].dx;	
			Lv[m_height-1][j] = (2*m_ppGVF[m_height-2][j].dy + m_ppGVF[m_height-1][j-1].dy + m_ppGVF[m_height-1][j+1].dy)*0.25 - m_ppGVF[m_height-1][j].dy;	
		}
		
		/* IV.4: Compute interior */
		for(i=1; i<m_height-1; i++)
		{
			for(j=1; j<m_width-1; j++)
			{
				Lu[i][j] = (m_ppGVF[i][j-1].dx + m_ppGVF[i][j+1].dx 
					+m_ppGVF[i-1][j].dx + m_ppGVF[i+1][j].dx)*0.25 - m_ppGVF[i][j].dx;
				Lv[i][j] = (m_ppGVF[i][j-1].dy + m_ppGVF[i][j+1].dy 
					+m_ppGVF[i-1][j].dy + m_ppGVF[i+1][j].dy)*0.25 - m_ppGVF[i][j].dy;
			}
		}
		
		/******** V: Update GVF ************/
		for(i=0; i<m_height; i++){
			for(j=0; j<m_width; j++){
				
                m_ppGVF[i][j].dx = (1 - b[i][j]*dt)*m_ppGVF[i][j].dx + r*Lu[i][j]*4.0 + c1[i][j]*dt;
                m_ppGVF[i][j].dy = (1 - b[i][j]*dt)*m_ppGVF[i][j].dy + r*Lv[i][j]*4.0 + c2[i][j]*dt;
				
			}
		}
	}

	for(k=0; k<m_height; k++)
	    delete  []GradFx[k];
	delete []GradFx;
	GradFx = 0;

	for(k=0; k<m_height; k++)
	    delete  []GradFy[k];
	delete []GradFy;
	GradFy = 0;

	for(k=0; k<m_height; k++)
	    delete  []b[k];
	delete []b;
	b = 0;
	
	for(k=0; k<m_height; k++)
	    delete  []c1[k];
	delete []c1;
	c1 = 0;

	for(k=0; k<m_height; k++)
	    delete  []c2[k];
	delete []c2;
	c2 = 0;

	for(k=0; k<m_height; k++)
	    delete  []Lu[k];
	delete []Lu;
	Lu = 0;
	
	for(k=0; k<m_height; k++)
	    delete  []Lv[k];
	delete []Lv;
	Lv = 0;
}

double IGvfSnake::CalculateAverageDist(const vector<skPoint>&vSnake)
{
	double d, h = 0.0;
    int i, n = vSnake.size();
	for(i=1; i<=n; i++) {
		d = (vSnake[i]-vSnake[i-1]).GetModule();
		h += d;
	}
	return h/n;
}

skVector IGvfSnake::CalculateFlexuralForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex)
{
	int  n = vSnake.size();
	double avg_dist = CalculateAverageDist(vSnake);
	skPoint v_i, v_i_1;
	v_i   = skNode;
	v_i_1 = vSnake[ (NodeIndex-1 + n) % n ];
    double mod = (v_i_1 - v_i).GetModule();
	skVector avgForce = v_i_1 - v_i;
	avgForce *= avg_dist/mod;
	return (avgForce - (v_i_1 - v_i));
}

// FlexuralForce = (vi_1 - vi) + (vi+1 - vi)
skVector IGvfSnake::CalculateTensileForce(const vector<skPoint>&vSnake, const skPoint&skNode, int NodeIndex)
{
	int  n = vSnake.size();
    skPoint V_i_1, V_i, V_i1;
	skVector  TensileForce;
	V_i_1 = vSnake[ (NodeIndex-1 + n) % n ];
	V_i   = skNode;
	V_i1  = vSnake[ (NodeIndex+1) % n ];
	TensileForce = V_i_1 + V_i1 - 2*V_i;
	return TensileForce;
}

skVector IGvfSnake::CalculateExternalForce(const skPoint&skNode)
{	 
    return m_ppGVF[skNode.dy][skNode.dx];
}

skPoint  IGvfSnake::Search(const vector<skPoint>&vSnake, int NodeIndex)
{  
	int  dir[9][2] = {   0, 0,
		                -1, 0,
						-1, 1,
						 0, 1,
						 1, 1,
						 1, 0,
						 1,-1,
						 0,-1,
						-1,-1				
					};
    int i;
	const skPoint NowPos = vSnake[NodeIndex];
	skPoint NewPos, BestNode;
    skVector CompositionForce;
	skVector fForce, tForce, gForce;
	double dMaxFFModule = -INF;
	double dMaxTFModule = -INF;
	double dMaxGFModule = -INF;
	double dMin = INF;
		
	for(i=0; i<9; ++i)
    {
        NewPos = NowPos + skPoint(dir[i][0], dir[i][1]);
		if( !IsInner(NewPos) )continue;
	
		fForce = CalculateFlexuralForce(vSnake, NewPos, NodeIndex);
		tForce = CalculateTensileForce(vSnake, NewPos, NodeIndex);
		gForce = CalculateExternalForce(NewPos);
		
		if(fForce.GetModule() > dMaxFFModule)
	       dMaxFFModule = fForce.GetModule();
		if(tForce.GetModule() > dMaxTFModule)
	       dMaxTFModule = tForce.GetModule();
		if(gForce.GetModule() > dMaxGFModule)
		   dMaxGFModule = gForce.GetModule();
	}

	for(i=0; i<9; ++i)
    {
        NewPos = NowPos + skPoint(dir[i][0], dir[i][1]);
		if( !IsInner(NewPos) )continue;
		 	 
		fForce = CalculateFlexuralForce(vSnake, NewPos, NodeIndex)/dMaxFFModule;
		tForce = CalculateTensileForce(vSnake, NewPos, NodeIndex)/dMaxTFModule;
		gForce = CalculateExternalForce(NewPos)/dMaxGFModule;

        CompositionForce = m_alpha * fForce
			             + m_beta  * tForce
						 + m_gama  * gForce;

		if(CompositionForce.GetModule() < dMin)
		{
	       dMin = CompositionForce.GetModule();
		   BestNode = NewPos;
		}
	}  
	return BestNode;
}

// Snake控制点插值
void IGvfSnake::Interpolate()
{
	 list<skPoint> sNodeList;
	 skPoint newNode;
	 double dist;
     int i, n = m_vSnake.size();
	 for(i=1; i<=n; ++i)
	 {
		 dist = (m_vSnake[i%n] - m_vSnake[i-1]).GetModule();
		 if(dist < m_nMinDist)
			 continue;
		 else if(dist > m_nMaxDist)
		 {
			 skPoint n1 = m_vSnake[i%n], n2 = m_vSnake[i-1];
			 skPoint nn = (n1+n2)/2.0;
			 if(nn!=n1 && nn!=n2) sNodeList.push_back(nn);
		     sNodeList.push_back(m_vSnake[i%n]);
		 }
		 else{
			 sNodeList.push_back(m_vSnake[i%n]);
		 }
	 }
	 m_vSnake.clear();
	 m_vSnake.reserve(sNodeList.size());
	 list<skPoint>::iterator itr;
	 while( !sNodeList.empty() )
	 { 
		 m_vSnake.push_back(sNodeList.front());
		 sNodeList.pop_front();
	 }
}

bool IGvfSnake::SnakeDeform(int nIterT)
{
	int t;
	for(t=0; t<nIterT; ++t, ++m_nIterT) {
		Interpolate();
		int i = 0, cnt = 0, n = m_vSnake.size();
		skPoint skp;
		for(i=0; i<n; ++i) {
			skp = Search(m_vSnake, i);
			if(m_vSnake[i] == skp) cnt++;
			m_vSnake[i] = skp;
		}
		if(cnt>n*m_dp || m_nIterT>m_nMaxIterT) return false;
	}
	return true;
}

void IGvfSnake::ShowSnakeDeform(IplImage* srcImage)
{
	CvPoint  p1, p2;
	long i, n;
	IplImage *srcSavedImage = 0;
	srcSavedImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);
    cvCopyImage(srcImage, srcSavedImage);
	while(1) {
		cvCopyImage(srcSavedImage, srcImage);
		n = m_vSnake.size();
		for(i=1; i<=n; ++i) {
			p1.x = m_vSnake[i-1].dx;
			p1.y = m_vSnake[i-1].dy;
			p2.x = m_vSnake[i%n].dx;
			p2.y = m_vSnake[i%n].dy;
			cvCircle(srcImage, p1, 1, cvScalarAll(HEADRG));
			cvLine(srcImage, p1, p2, cvScalarAll(HEADRG), 1);
		}
		if(!SnakeDeform(1)) break;
	}
	cvReleaseImage(&srcSavedImage);
}

IplImage* IGvfSnake::GetppImageData()
{
	return m_ppImageData;
}
