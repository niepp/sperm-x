//-----------------------------------------//    
// "Dib.h":  设备无关位图(DIB)类实现 
// Author :  Happynp
// EMail  :  happy_np@163.com
// Date   :  7.24.2007
//-----------------------------------------//

#include "stdafx.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDib::CDib()
{
	m_pFileHeader = NULL;
	m_pInfoHeader = NULL;
    m_pData       = NULL;
}

CDib::CDib(LPBITMAPINFOHEADER lpInfoSrc, LPBYTE lpDataSrc)
{
	m_pFileHeader = NULL;
	m_pInfoHeader = lpInfoSrc;
    m_pData       = lpDataSrc;
}

CDib::~CDib()
{
    if(!IsEmpty())
	{
	   if(m_pFileHeader)	delete  []m_pFileHeader, m_pFileHeader = NULL;
	   if(m_pInfoHeader)    delete  []m_pInfoHeader, m_pInfoHeader = NULL;
	   if(m_pData)          delete  []m_pData, m_pData = NULL;
	}
}

BOOL CDib::IsEmpty()                 // 判断位图是否为空
{
	if(m_pInfoHeader == NULL && m_pData == NULL)
		return TRUE;
	else 
		return FALSE;
}

WORD CDib::GetDibPalEntryNum()                   //取得位图的调色板索引项数
{
    if(m_pInfoHeader->biClrUsed)
    {
        return (WORD)m_pInfoHeader->biClrUsed;
    }
    switch(m_pInfoHeader->biBitCount)
    {
    case 1:                //2色
        return 2;
    case 4:                //16色
        return 16;
    case 8:                //156色
        return 256;
	case 16:
	case 24:
	case 32:
		return 0;          //没有调色板
    default:               
        return -1;         //error
    }
}

BOOL  CDib::ReadDibFile(CFile *pFile)            //读取位图文件
{  
	UINT  t_nCount, t_nSize;

	// 进行读操作
	try 
	{
		m_pFileHeader = (LPBITMAPFILEHEADER)new unsigned char[sizeof(BITMAPFILEHEADER)];
		// 读取文件头
		t_nCount = pFile->Read((LPVOID)m_pFileHeader, sizeof(BITMAPFILEHEADER)); 

		// 如果读出的长度不对，则返回并进行相应错误处理
		if(t_nCount != sizeof(BITMAPFILEHEADER)) 
		{
			throw;
		}
		// 如果文件类型不为"BM"，则返回并进行相应错误处理
		if(m_pFileHeader->bfType != DIB_HEADER_MARKER)  // 'BM'
		{
			throw;
		}

		// 计算信息头加上调色板的大小，并分配相应的内存
		t_nSize = m_pFileHeader->bfOffBits - sizeof(BITMAPFILEHEADER);
		m_pInfoHeader = (LPBITMAPINFOHEADER)new unsigned char[t_nSize];

		// 读取信息头和调色板
		t_nCount = pFile->Read(m_pInfoHeader, t_nSize);

		// 分配图象数据内存，并从文件中读取图象数据
		m_pData = (LPBYTE) new unsigned char[m_pInfoHeader->biSizeImage];
		t_nCount = pFile->Read(m_pData, m_pInfoHeader->biSizeImage); 

	}
	// 错误处理
	catch(...) 
	{
		AfxMessageBox("Read error");
		return FALSE;
	}
	// 返回
	return TRUE;
}

BOOL  CDib::SaveDibFile(CFile* pFile)        //保存位图	
{
	ASSERT(m_pInfoHeader!= NULL && m_pData!= NULL);  // 断言位图不空
	BITMAPFILEHEADER  t_bmFileHeader;

	// 设置文件头中文件类型为"BM"
	t_bmFileHeader.bfType = 0x4d42;  

	WORD t_wPaletteCount = GetDibPalEntryNum();
	// 计算信息头和调色板的大小尺寸
	int t_nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*t_wPaletteCount;

	// 设置文件头信息
	t_bmFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + t_nSizeHdr + m_pInfoHeader->biSizeImage;
	t_bmFileHeader.bfReserved1 = 0; 
	t_bmFileHeader.bfReserved2 = 0;
	t_bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			                 + sizeof(RGBQUAD)*t_wPaletteCount;	

	// 进行写操作
	try 
	{
		pFile->Write((LPVOID)&t_bmFileHeader, sizeof(BITMAPFILEHEADER));  //文件头
		pFile->Write((LPVOID) m_pInfoHeader,  t_nSizeHdr);                //信息头
		pFile->Write((LPVOID) m_pData, m_pInfoHeader->biSizeImage);       //数据区
	}

	// 错误处理
	catch(CException* pe) 
	{
		pe->Delete();
		AfxMessageBox("write error");
		return FALSE;
	}

	// 返回
	return TRUE;
}
	
void  CDib::Serialize(CArchive& ar)         // 文档串行化过程
{	
	if(ar.IsStoring())
	{
		SaveDibFile(ar.GetFile());
	}
	else
	{
		ReadDibFile(ar.GetFile());
	}
}

BOOL  CDib::DrawDib(CDC* pDC, CPoint origin)   // 显示位图
{
	::StretchDIBits(pDC->GetSafeHdc(), 
		origin.x, origin.y, m_pInfoHeader->biWidth, m_pInfoHeader->biHeight, 
		origin.x, origin.y, m_pInfoHeader->biWidth, m_pInfoHeader->biHeight, 
		 m_pData, (LPBITMAPINFO)m_pInfoHeader, DIB_RGB_COLORS, SRCCOPY);
    return TRUE;
}