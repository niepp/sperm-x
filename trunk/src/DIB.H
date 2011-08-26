//-----------------------------------------//    
// "Dib.h":  设备无关位图(DIB)类声明 
// Author :  Happynp
// EMail  :  happy_np@163.com
// Date   :  7.24.2007
//-----------------------------------------//
#ifndef  MY_DIB_CLASS
#define  MY_DIB_CLASS

#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)     (((bits)+31)/32*4)
#endif

#define DIB_HEADER_MARKER    ((WORD)('M'<< 8)|'B') 

class CDib : public CObject  // 从CObject继承,从而可串行化
{
public:
    CDib();
	CDib(LPBITMAPINFOHEADER lpInfoSrc, LPBYTE lpDataSrc);
    virtual ~CDib();
    BOOL  IsEmpty();					          //判断DIB是否为空
    BOOL  ReadDibFile(CFile *pFile);              //读取位图文件
    BOOL  SaveDibFile(CFile* pFile);              //保存位图文件		
	void  Serialize(CArchive& ar);                //文档串行化过程
    WORD  GetDibPalEntryNum();                    //取得位图的调色板索引项数
	BOOL  DrawDib(CDC* pDC, CPoint origin= (0,0));       //显示位图
    
public:

    LPBITMAPFILEHEADER  m_pFileHeader;      //位图文件头
    LPBITMAPINFOHEADER  m_pInfoHeader;      //位图信息头
    LPBYTE              m_pData;            //位图数据起始地址 
};

#endif  //MY_DIB_CLASS