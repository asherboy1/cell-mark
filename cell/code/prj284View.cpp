
// prj284View.cpp: Cprj284View 类的实现
//
#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "prj284.h"
#include "MainFrm.h"
#include "afxwin.h"
#include <stack>
#include <vector>
#endif

#include "prj284Doc.h"
#include "prj284View.h"
#define Random(x) (rand() % x)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<iostream>
using namespace std;
#define DISTANCE(x0,y0,x1,y1) sqrt((float)((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)))


// Cprj284View

IMPLEMENT_DYNCREATE(Cprj284View, CView)

BEGIN_MESSAGE_MAP(Cprj284View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_APP_ABOUT, &Cprj284View::OnCellMark1)
	ON_COMMAND(ID_EDIT_COPY, &Cprj284View::OnCelltwoValue1)
	ON_COMMAND(ID_EDIT_FIND, &Cprj284View::OnCellFillHoles1)
	ON_COMMAND(ID_EDIT_REDO, &Cprj284View::OnCellShink1)
	ON_COMMAND(ID_FILE_MRU_FILE9, &Cprj284View::OnCellFindCenter1)
	ON_COMMAND(ID_HELP_FINDER, &Cprj284View::OnCellCount1)
	ON_COMMAND(IDOK, &Cprj284View::OnAllDone1)
END_MESSAGE_MAP()

// Cprj284View 构造/析构

Cprj284View::Cprj284View() noexcept
{
	// TODO: 在此处添加构造代码

}

Cprj284View::~Cprj284View()
{
}

BOOL Cprj284View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cprj284View 绘图

void Cprj284View::OnDraw(CDC* pDC)
{
	Cprj284Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (noclick)
		LoadBmp();
	noclick = false;
	//ShowBitmap(pDC, BmpName);               //显示图片    
//}
// TODO: 在此处为本机数据添加绘制代码
	CSize size;
	size.cx = bih.biWidth; size.cy = bih.biHeight;

	if (EndPoint.x < 20)
	{
		StartPoint = CPoint(0, 0);
		
		EndPoint = CPoint(size.cx - 1, size.cy - 1);
		
	}
	display(pDC, CPoint(0, 0), size);

}


// Cprj284View 打印

BOOL Cprj284View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cprj284View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cprj284View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Cprj284View 诊断

#ifdef _DEBUG
void Cprj284View::AssertValid() const
{
	CView::AssertValid();
}

void Cprj284View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cprj284Doc* Cprj284View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cprj284Doc)));
	return (Cprj284Doc*)m_pDocument;
}
#endif //_DEBUG


// Cprj284View 消息处理程序


bool Cprj284View::LoadBmp()
{
	CString str;
	//MessageBox(BmpName);
	FILE *fp = fopen("C:/Users/dell/Desktop/Blood.bmp", "rb");//BmpName
	if (fp == 0)
	{
		MessageBox(TEXT("无法打开文件!"));
		return 0;
	}
	//读取文件头 解决BMP格式倒置的方法  
	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);
	//图像文件的总字节数  
	m_nSize = bfh.bfSize;
	//判断是否是bmp格式图片  
	if (bfh.bfType != 0x4d42)   //'BM'  
	{
		MessageBox(TEXT("不是BMP格式图片!"));
		return 0;
	}
	//读取信息头  
	fread(&bih.biSize, sizeof(DWORD), 1, fp);
	fread(&bih.biWidth, sizeof(LONG), 1, fp);
	fread(&bih.biHeight, sizeof(LONG), 1, fp);
	fread(&bih.biPlanes, sizeof(WORD), 1, fp);
	fread(&bih.biBitCount, sizeof(WORD), 1, fp);
	fread(&bih.biCompression, sizeof(DWORD), 1, fp);
	fread(&bih.biSizeImage, sizeof(DWORD), 1, fp);
	fread(&bih.biXPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biYPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biClrUsed, sizeof(DWORD), 1, fp);
	fread(&bih.biClrImportant, sizeof(DWORD), 1, fp);
	if (bih.biSize != sizeof(bih))
	{
		MessageBox(TEXT("本结构所占用字节数出现错误"));
		return 0;
	}
	//位图压缩类型，必须是 0（不压缩） 1（BI_RLE8压缩类型）或2（BI_RLE压缩类型）之一  
	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		MessageBox(TEXT("位图被压缩!"));
		return 0;
	}
	//获取图像高宽和每个像素所占位数  
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //每个像素所占位数  
									//计算图像每行像素所占的字节数（必须是32的倍数）  
	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4; //修改处
	//str.Format(TEXT("位数为%d"), m_nBitCount);
	//MessageBox(str);
	//图片大小 调用系统自带的文件头 BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih;   
	//否则用 BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;要 m_nImage = m_nLineByte * m_nHeight - 2;  
	m_nImage = m_nLineByte * m_nHeight;
	//位图实际使用的颜色表中的颜色数 biClrUsed  
	m_nPalette = 0;                       //初始化  
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;


	//申请位图空间 大小为位图大小 m_nImage  
	//malloc只能申请4字节的空间 （未知） 
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);

	return true;
}


bool Cprj284View::display(CDC* pDC, CPoint origin, CSize size)
{
	if (&bih == NULL) return FALSE;
	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, bih.biWidth, bih.biHeight,
		m_pImage, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, SRCCOPY);
	return TRUE;
}


void Cprj284View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RGB rgb; HSI hsi; CString str; HDC hDC = ::GetDC(NULL);
	COLORREF val = ::GetPixel(hDC, point.x, point.y);
	int posin = m_nLineByte * (m_nHeight - 1 - point.y) + point.x * 3;
	unsigned char *lpSrc;
	//m_pImage[posin + 2], m_pImage[posin + 1], m_pImage[posin]

	if ((point.x > 0) && (point.x < bih.biWidth) && (point.y > 0) && (point.y < bih.biHeight))
	{

		if (bih.biBitCount == 8) {
			lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - point.y) + point.x;
			str.Format(TEXT("(x=%d y=%d)=%d"), point.x, point.y, *lpSrc);
		}


		if (bih.biBitCount == 24)
		{
			rgb.b = m_pImage[posin];
			rgb.g = m_pImage[posin + 1];
			rgb.r = m_pImage[posin + 2];
			RgbtoHsi(&rgb, &hsi);
			int gray = (int)(0.114*rgb.r + 0.587*rgb.g + 0.299*rgb.b);
			str.Format(TEXT("Pos(%d %d) RGB(%d %d %d) Gray(%d) HSI(%4.1f %3.2f %3.2f--%d %d %d)"), point.x, point.y
				, rgb.r, rgb.g, rgb.b, gray, hsi.Hue, hsi.Saturation, hsi.Intensity,
				(int)(hsi.Hue*255.0 / 360), (int)(hsi.Saturation*255.0), (int)(hsi.Intensity*255.0));
		}
		((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, str);
	}


	CView::OnMouseMove(nFlags, point);
}


void Cprj284View::RgbtoHsi(RGB *pRgb, HSI *pHsi)
{
	// TODO: 在此处添加实现代码.
	long int p;
	p = (pRgb->r < pRgb->g) ? pRgb->r : pRgb->g;
	p = (pRgb->b < p) ? pRgb->b : p;


	pHsi->Intensity = ((double)(double(pRgb->r) + double(pRgb->g) + double(pRgb->b))) / 255.0 / 3;
	pHsi->Saturation = 1 - 3.0 / (double(pRgb->r) + pRgb->g + pRgb->b)*p;
	if (pRgb->g >= pRgb->b)
		pHsi->Hue = 57.296*acos((((double)(pRgb->r - pRgb->g) + (pRgb->r - pRgb->b)) / 2 / sqrt((double)(pRgb->r - pRgb->g)*(pRgb->r - pRgb->g) + (double)(pRgb->r - pRgb->b)*(double)(pRgb->g - pRgb->b))));
	else
		pHsi->Hue = 360 - 180.0 / 3.14159*double(acos(((double)(pRgb->r - pRgb->g) + (pRgb->r - pRgb->b)) / 2 / sqrt((double)(pRgb->r - pRgb->g)*(pRgb->r - pRgb->g) + (double)(pRgb->r - pRgb->b)*(double)(pRgb->g - pRgb->b))));
		
	return;
}


void Cprj284View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bDrag) {
		m_LastPoint = point;
		m_bDrag = true;
	}
	CView::OnLButtonDown(nFlags, point);
}


void Cprj284View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDrag = false;
	StartPoint.x = min(m_LastPoint.x, point.x);
	StartPoint.y = min(m_LastPoint.y, point.y);
	EndPoint.x = max(m_LastPoint.x, point.x);
	EndPoint.y = max(m_LastPoint.y, point.y);
	Invalidate(true);
	CView::OnLButtonUp(nFlags, point);
}


void Cprj284View::OnCellMark1()
{
	// TODO: 在此添加命令处理程序代码
	m_CellProc = 1;
	//Cprj284Doc* pDoc = GetDocument();

	// 更改光标形状
	BeginWaitCursor();

	unsigned char*	lpSrc;

	// 指向转置图像对应象素的指针
	unsigned char*	lpDst;




	// 指向转置图像的指针
	unsigned char*	lpNewDIBBits;
	// 暂时分配内存，以保存新图像
	lpNewDIBBits = new unsigned char[m_nHeight * m_nLineByte];

	// 判断是否内存分配失败
	if (lpNewDIBBits == NULL)
	{
		
		// 分配内存失败
		return;
	}

	lpSrc = (unsigned char *)m_pImage;
	memcpy(lpNewDIBBits, lpSrc, m_nLineByte * m_nHeight);

	double meanH = 210.0 * 360 / 255;//you select 
	double meanS = 55.0 / 255;//you select 
	double MarkDoor = 0.09;
	double MayBeMarkDoor = 0.15;

	// 针对图像每行进行操作
	for (int i = StartPoint.y; i < EndPoint.y; i++)
	{
		// 针对每行图像每列进行操作
		for (int j = StartPoint.x; j < EndPoint.x; j++)
		{

			// 指向源DIB第i行，第j个象素的指针
			lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
			//if(*lpSrc<10) {
			//  *lpSrc=10;
			//}
			RGB rgb;
			HSI Hsi;
			rgb.b = *lpSrc; rgb.g = *(lpSrc + 1); rgb.r = *(lpSrc + 2);
			RgbtoHsi(&rgb, &Hsi);
			//get area (value get from cell area)



			double x1 = Hsi.Hue;//0—360
			double x2 = meanH;//近似
			if (x1 < 90) x1 += 360;

			//归一化
			double y1 = Hsi.Saturation;
			double y2 = meanS;
			x1 /= 180; x2 /= 180;//0--1 
			//y1*=2;y2*=2;//0--2

			double dis = DISTANCE(x1, y1, x2, y2);
			if (dis < MarkDoor) {//Mark
				*lpSrc = 0; *(lpSrc + 1) = 0; *(lpSrc + 2) = 255;//Red
			}
			else if (dis < MayBeMarkDoor) {//may be Mark
				*lpSrc = 255; *(lpSrc + 1) = 0; *(lpSrc + 2) = 0;//Blue
			}
			//为了避开后面判断
			else {//not Mark/maybe Mark
				if (*lpSrc == 0) *lpSrc = 1;//Mark判断
				else if (*lpSrc == 255) *lpSrc = 254;//maybe mark判断
				if (*(lpSrc + 1) == 255) *(lpSrc + 1) = 254; //edge判断

			}
		}
	}

	Invalidate(true);
	MessageBox(TEXT("Mark(Red) & maybe Mark(Blue) "));

	bool MarkChg = true;
	while (MarkChg) {
		MarkChg = false;
		for (int i = StartPoint.y + 1; i < EndPoint.y - 1; i++)
		{
			// 针对每行图像每列进行操作
			for (int j = StartPoint.x + 1; j < EndPoint.x - 1; j++)
			{

				// 指向源DIB第i行，第j个象素的指针
				lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				if (*lpSrc == 255) {//maybe Mark 
					bool bProc = false;
					if (j > 0) if (*(lpSrc - 3) == 0) bProc = true;
					if (j < m_nWidth - 1) if (*(lpSrc + 3) == 0) bProc = true;
					if (i > 0) if (*(lpSrc + m_nLineByte) == 0) bProc = true;
					if (i < m_nLineByte - 1) if (*(lpSrc - m_nLineByte) == 0) bProc = true;
					// maybe Mark have Mark Point to Mark 
					if (bProc) {
						*lpSrc = 0;
						MarkChg = true;
						*(lpSrc + 2) = 128;
					}
				}
			}
		}
	}

	Invalidate(true);
	MessageBox(TEXT("maybe Mark to Mark (Bright Red(128,0,0)"));

	const int edgeDoor = 45;
	//get edge information
	for (int i = StartPoint.y + 1; i < EndPoint.y - 1; i++) {//boarder no edge
		for (int j = StartPoint.x + 1; j < EndPoint.x - 1; j++) {
			lpDst = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
			if (*(lpDst) == 0 || *(lpDst) == 255) {//Mark /Maybe Mark
				double pixel[9];
				lpSrc = lpNewDIBBits + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				for (int m = -1; m < 2; m++)
					for (int n = -1; n < 2; n++) {
						unsigned char* lpSrc1 = lpSrc - m_nLineByte * m + 3 * n;
						pixel[(m + 1) * 3 + n + 1] = ((int)*lpSrc1 + *(lpSrc1 + 1) + *(lpSrc1 + 2)) / 3;
					}
				//Sobel
				double tmp1 =
					pixel[0] + 2 * pixel[1] + pixel[2] - pixel[6] - 2 * pixel[7] - pixel[8];
				double tmp2 =
					pixel[0] + 2 * pixel[3] + pixel[6] - pixel[2] - 2 * pixel[5] - pixel[8];
				double edge = sqrt(tmp1*tmp1 + tmp2 * tmp2);
				if (edge > edgeDoor) {
					*(lpDst + 1) = 255;//edge


				}

			}
		}
	}

	const int M = 5;
	bool bdelete;
	//filter 
	for (int i = StartPoint.y + M; i < EndPoint.y - M; i++)//
	{
		// 针对每行图像每列进行操作
		for (int j = StartPoint.x + M; j < EndPoint.x - M; j++)
		{

			// 指向源DIB第i行，第j个象素的指针
			lpDst = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
			if (*(lpDst + 1) == 255)//edge 
			{
				bdelete = true;
				for (int m = -M; m <= M; m++)
					for (int n = -M; n <= M; n++)
					{
						if (m == -M || m == M || n == -M || n == M) {
							if (*(lpDst + m_nLineByte * m + n * 3) || (*(lpDst + m_nLineByte * m + n * 3 + 1) == 255))//noMark && no Edge
							{
								bdelete = false;
								m = M + 1; n = M + 1;//out
							}
						}
					}
				if (bdelete)
					*(lpDst + 1) = 0;//delete edge
			}
		}
	}


	
	delete[] lpNewDIBBits;
	Invalidate(true);
	// 恢复光标
	EndWaitCursor();

}


void Cprj284View::OnCelltwoValue1()
{
	if (m_CellProc == 1)
	{
		// TODO: 在此添加命令处理程序代码
		m_CellProc = 2;
		//CImgProcDoc* pDoc = GetDocument();
		//24bit to gray image
		unsigned char*	lpSrc;

		// 指向转置图像对应象素的指针
		unsigned char*	lpDst;

		long int lNewLineBytes = (m_nWidth + 3) / 4 * 4;


		// 指向转置图像的指针
		unsigned char*	lpNewDIBBits;
		// 暂时分配内存，以保存新图像
		lpNewDIBBits = new unsigned char[m_nHeight * lNewLineBytes];

		// 判断是否内存分配失败
		if (lpNewDIBBits == NULL)
		{
			// 分配内存失败
			
			return;
		}


		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				lpDst = lpNewDIBBits + lNewLineBytes * (m_nHeight - 1 - i) + j;
				unsigned int v;
				v = 0;
				if (*(lpSrc) == 0)//Mark
				{
					v = TWOVALUE_H;
					if (*(lpSrc + 1))
						v |= EDGEPOINT;//set edge
				}
				*lpDst = (unsigned char)v;
			}

		}
		m_nLineByte = lNewLineBytes;
		// 复制转置后的图像
		/*
		delete 	pDoc->m_pDib;
		pDoc->m_pDib = new CDib(CSize(lNewLineBytes, m_nHeight), 8);
		*/
		//lpSrc = (unsigned char*)m_lpvColorTable;

		//&bih = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);


		bih.biWidth = lNewLineBytes;
		bih.biHeight = m_nHeight;
		bih.biBitCount = 8;
		bih.biClrUsed = 0;
		bih.biCompression = BI_RGB;
		bih.biClrImportant = 0;

		
		
		
		m_pImage = (BYTE *) new char[lNewLineBytes*m_nHeight];
		/*
			for (int i = 0; i < 256; i++) {
				*lpSrc = (unsigned char)i; lpSrc++;
				*lpSrc = (unsigned char)i; lpSrc++;
				*lpSrc = (unsigned char)i; lpSrc++;
				*lpSrc = 0; lpSrc++;
			}
		*/
		memcpy(m_pImage, lpNewDIBBits, lNewLineBytes*m_nHeight);
		delete []lpNewDIBBits;

		Invalidate(true);
	}
	else
		MessageBox(TEXT("请先执行Mark"));
}


void Cprj284View::OnCellFillHoles1()
{
	if (m_CellProc == 2)
	{
		// TODO: 在此添加命令处理程序代码
		m_CellProc = 3;
		//CImgProcDoc* pDoc = GetDocument();

		// 更改光标形状
		BeginWaitCursor();


		unsigned char *lpSrc;
		//0x7X---edge
		//0x8X---Mark--not edge
		//0xfX--Mark --edge
		//0xX1---visited
		for (int i = StartPoint.y + 1; i <= EndPoint.y - 1; i++)
		{
			lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x;
			for (int j = StartPoint.x + 1; j <= EndPoint.x - 1; j++)
			{
				lpSrc++;

				//if no-marked & no-visited
				if (!(*lpSrc & MARK_VISITED)) {//未访问过的黑点
					ProcessFillHoles(j, i);//line,col
				}

			}
		}
		Invalidate(true);
		MessageBox(TEXT("FillHole"));

		//edge area back
		//0xfX--Mark --edge
		for (int i = StartPoint.y; i <= EndPoint.y; i++)
		{
			lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x - 1;
			for (int j = StartPoint.x; j <= EndPoint.x; j++)
			{
				lpSrc++;
				if (!(*lpSrc & MARKED))
					*lpSrc = 0;
				else //if marked & edge
					if (*lpSrc & EDGEPOINT)
						*lpSrc = 0;

			}
		}

		Invalidate(true);
		// 恢复光标
		EndWaitCursor();
	}
	else
		MessageBox(TEXT("请先执行Two Value"));
}


void Cprj284View::ProcessFillHoles(int wd, int ht)
{
	// TODO: 在此处添加实现代码.
	CPoint str;
	//CImgProcDoc* pDoc = GetDocument();
	stack<CPoint> s;
	vector<CPoint> v;//v save for fill holes

	const int MAX_HOLE = 100;

	int xt, yt;
	xt = wd;
	yt = ht;
	s.push(CPoint(xt, yt));
	v.push_back(CPoint(xt, yt));

	unsigned char *lpSrc;
	lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - yt) + xt;
	*lpSrc |= VISITED;//vistied	

	bool bBorder = false;

	while (s.size())
	{
		//Add new members to stack
		//Above current pixel
		lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - yt) + xt;
		if (yt > StartPoint.y) {
			//if no-marked & no-visited
			if (!(*(lpSrc + m_nLineByte) & MARK_VISITED))
			{
				s.push(CPoint(xt, yt - 1));
				v.push_back(CPoint(xt, yt - 1));
				*(lpSrc + m_nLineByte) |= VISITED;
			}
		}
		else bBorder = true;
		//Below current pixel

		if (yt < EndPoint.y) {
			//if no-marked & no-visited
			if (!(*(lpSrc - m_nLineByte) & MARK_VISITED))
			{
				s.push(CPoint(xt, yt + 1));
				v.push_back(CPoint(xt, yt + 1));
				*(lpSrc - m_nLineByte) |= VISITED;
			}
		}
		else bBorder = true;
		//Left of current pixel
		if (xt > StartPoint.x) {
			//if no-marked & no-visited
			if (!(*(lpSrc - 1) & MARK_VISITED))
			{
				s.push(CPoint(xt - 1, yt));
				v.push_back(CPoint(xt - 1, yt));
				*(lpSrc - 1) |= VISITED;
			}
		}
		else bBorder = true;
		//Right of current pixel
		if (xt < EndPoint.x) {
			//if no-marked & no-visited
			if (!(*(lpSrc + 1) & MARK_VISITED))
			{
				s.push(CPoint(xt + 1, yt));
				v.push_back(CPoint(xt + 1, yt));
				*(lpSrc + 1) |= VISITED;
			}
		}
		else bBorder = true;
		//Retrieve current stack member
		xt = s.top().x;
		yt = s.top().y;
		s.pop();
	}
}


void Cprj284View::OnCellShink1()
{
	if (m_CellProc == 3)
	{
		// TODO: 在此添加命令处理程序代码
		m_CellProc = 4;
		//CImgProcDoc* pDoc = GetDocument();
		const int pre_shrink_count = 3;
		unsigned char *lpSrc;
		// 先去掉pre_shrink_count层皮
		GenEdge();
		for (int k = 0; k < pre_shrink_count; k++)
		{
			for (int i = StartPoint.y; i <= EndPoint.y; i++)
			{
				lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x - 1;
				for (int j = StartPoint.x; j <= EndPoint.x; j++)
				{
					lpSrc++;

					// 去掉边界!
					if (*lpSrc & EDGEPOINT)
						(*lpSrc) &= NO_MARK;//marked=0;
				}
			}
			if (k == 0)
				GenEdge4();
			else
				GenEdge();

		}
		Invalidate(true);
	}
	else
		MessageBox(TEXT("请先执行Fill Holes"));
}


void Cprj284View::GenEdge(void)
{
	// TODO: 在此处添加实现代码.

	unsigned char *lpSrc;


	for (int j = StartPoint.y; j <= EndPoint.y; j++)
	{
		lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
		for (int i = StartPoint.x; i <= EndPoint.x; i++)
		{
			lpSrc++;
			*lpSrc &= NO_EDGE_POINT;//no edge
			if (*lpSrc & MARKED)//marked
			{
				if (j == StartPoint.y || i == StartPoint.x || j == EndPoint.y || i == EndPoint.x)	// 
				{
					*lpSrc |= EDGEPOINT;
				}

				else if              // normal

					(
						!((*(lpSrc - m_nLineByte - 1) & MARKED)
							&& (*(lpSrc - m_nLineByte) & MARKED)
							&& (*(lpSrc - m_nLineByte + 1) & MARKED)
							&& (*(lpSrc - 1) & MARKED)
							&& (*(lpSrc + 1) & MARKED)
							&& (*(lpSrc + m_nLineByte - 1) & MARKED)
							&& (*(lpSrc + m_nLineByte) & MARKED)
							&& (*(lpSrc + m_nLineByte + 1) & MARKED)
							)
						) *lpSrc |= EDGEPOINT;
			}

		}
	}
}


void Cprj284View::GenEdge4(void)
{
	// TODO: 在此处添加实现代码.
	//CImgProcDoc* pDoc = GetDocument();
	unsigned char *lpSrc;


	for (int j = StartPoint.y; j <= EndPoint.y; j++)
	{
		lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
		for (int i = StartPoint.x; i <= EndPoint.x; i++)
		{
			lpSrc++;
			*lpSrc &= NO_EDGE_POINT;//no edge
			if (*lpSrc & MARKED)//marked
			{
				if (j == StartPoint.y || i == StartPoint.x || j == EndPoint.y || i == EndPoint.x)	// 
				{
					*lpSrc |= EDGEPOINT;
				}

				else if              // normal
					(
						!((*(lpSrc - m_nLineByte) & MARKED)
							&& (*(lpSrc - 1) & MARKED)
							&& (*(lpSrc + 1) & MARKED)
							&& (*(lpSrc + m_nLineByte) & MARKED)
							)
						) *lpSrc |= EDGEPOINT;
			}

		}
	}
}


void Cprj284View::OnCellFindCenter1()
{
	if (m_CellProc == 4)
	{
		// TODO: 在此添加命令处理程序代码
		m_CellProc = 5;
		//CImgProcDoc* pDoc = GetDocument();

		CENTER_POINT pt;

		points_temp.clear();
		bool changed;
		unsigned char *lpSrc;
		BeginWaitCursor();

		for (int k = 0; k < 40; k++) // 标志中心点的腐蚀	
		{
			changed = false;
			// 清除visited标志
			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					*lpSrc &= NO_VISITED;//
				}
			}

			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					{
						if (j > StartPoint.y && j<EndPoint.y && i>StartPoint.x && i < EndPoint.x) // 最边上的不用处理
						{
							m_bFullEdge = true;
							if (*lpSrc & EDGEPOINT && !(*lpSrc & VISITED))	// 没有访问过的边界
							{
								if (!(*(lpSrc - 1) &  MARKED) &&
									!(*(lpSrc + 1) &  MARKED) &&
									!(*(lpSrc + m_nLineByte) &  MARKED) &&
									!(*(lpSrc - m_nLineByte) &  MARKED))
								{
									if (k == 0) // 基本上这种是噪音
									{
										continue;
									}
									// 孤立的点

									*lpSrc |= CENTERED;
									// 保存一下CENTER_POINT信息
									pt.x = i;
									pt.y = j;
									pt.radius = k + pre_shrink_count + 4;//circle adjust 
									points_temp.push_back(pt);

									continue;
								}
								else
									MarkIt(i, j); // 判断是否需要保存
							// 没有访问过 标志了 并且是非边缘邻域
							// 需要保存!
								if (m_bFullEdge)
									SaveIt(i, j, k + pre_shrink_count + 4);//保存
							}
						}
					}
				}
			}
			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					// 去掉边界!
					if (*lpSrc & EDGEPOINT)
					{
						changed = true;
						*lpSrc &= NO_MARK;
					}
				}
			}
			if (k % 2 == 0)
				GenEdge4();
			else
				GenEdge();

			if (!changed)
				break;


		}

		CString msg;
		msg.Format(TEXT("获得的中心点数目= %d"), points_temp.size());
		MessageBox(msg);

		// 取平均值,获得中心点
		vector<CENTER_POINT>	points;
		for (int j = StartPoint.y; j <= EndPoint.y; j++)
		{
			lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
			for (int i = StartPoint.x; i <= EndPoint.x; i++)
			{
				lpSrc++;
				if (j > StartPoint.y && j<EndPoint.y && i>StartPoint.x && i < EndPoint.x) // 最边上的不用处理
					if (*lpSrc & CENTERED)
					{
						if (!(*(lpSrc - 1) & CENTERED) && !(*(lpSrc + 1) & CENTERED) &&
							!(*(lpSrc + m_nLineByte) & CENTERED) &&
							!(*(lpSrc - m_nLineByte) & CENTERED) &&
							!(*(lpSrc + m_nLineByte - 1) & CENTERED) &&
							!(*(lpSrc + m_nLineByte + 1) & CENTERED) &&
							!(*(lpSrc - m_nLineByte - 1) & CENTERED) &&
							!(*(lpSrc - m_nLineByte + 1) & CENTERED))
						{
							// 孤立的点
							pt.x = i;
							pt.y = j;
							for (unsigned int n = 0; n < points_temp.size(); n++)
							{
								if (points_temp.at(n).x == i && points_temp.at(n).y == j)
								{
									pt.radius = points_temp.at(n).radius;
									break;
								}
							}
							points.push_back(pt);
							continue;
						}
						else
						{
							tot_area = 0;
							max_radius = 0;
							tot_x = 0;
							tot_y = 0;
							CalcCenterArea(i, j);
							pt.x = tot_x / tot_area;
							pt.y = tot_y / tot_area;
							pt.radius = max_radius;
							*(lpSrc - (pt.y - j)*m_nLineByte + pt.x - i) |= CENTERED;
							points.push_back(pt);
						}
					}
			}

		}

		msg.Format(TEXT("取平均值,获得中心点数目= %d"), points.size());
		MessageBox(msg);

		// draw to display points
		CDC *pdc = GetDC();
		CPen	Redpen;
		Redpen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		CPen	Redpen1;
		Redpen1.CreatePen(PS_DOT, 3, RGB(255, 0, 0));
		CPen Greenpen;
		Greenpen.CreatePen(PS_DOT, 1, RGB(0, 255, 0));
		CPen Bluepen;
		Bluepen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
		CPen Bluepen1;
		Bluepen1.CreatePen(PS_DOT, 3, RGB(0, 0, 255));

		m_vCenterPoints.clear();
		int x0, y0;
		bool adj;
		// 清除center标志
		for (int j = StartPoint.y; j <= EndPoint.y; j++)
		{
			lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
			for (int i = StartPoint.x; i <= EndPoint.x; i++)
			{
				lpSrc++;
				*lpSrc &= NO_CENTER;
			}
		}
		// 平均化相近的中心点

		for (unsigned int i = 0; i < points.size(); i++)
		{
			x0 = points.at(i).x;
			y0 = points.at(i).y;
			pt = points.at(i);

			adj = false;

			//Red 相近- delete
			pdc->SelectObject(Redpen);
			for (unsigned int j = i + 1; j < points.size() - 1; j++)
			{
				int x = points.at(j).x;
				int y = points.at(j).y;
				if (abs(x0 - x) + abs(y0 - y) < 10) // 相近
				{
					points.at(i).x = (x + x0) / 2;
					points.at(i).y = (y + y0) / 2;
					points.at(i).radius = (points.at(i).radius + points.at(j).radius) / 2;

					pt = points.at(j);
					//display err position--delete 
					Arc(pdc->m_hDC,//-3 for display 
						pt.x - pt.radius + 3,
						pt.y - pt.radius + 3,
						pt.x + pt.radius - 3,
						pt.y + pt.radius - 3,
						pt.x + pt.radius - 3,
						pt.y - 3,
						pt.x + pt.radius - 3,
						pt.y - 3
					);

					points.erase(points.begin() + j);//&points.at(j));
					i--;
					adj = true;
					break;
				}
			}
			if (!adj) // 非相近
			{
				if (points.at(i).radius > 4)
				{
					m_vCenterPoints.push_back(points.at(i));
					*(m_pImage + m_nLineByte * (m_nHeight - 1 - points.at(i).y) + points.at(i).x) |= CENTERED;
					if (i % 5 == 0 && i)	TRACE("\n");
					TRACE("%3d:(%3d %3d)--%2d\t", i, points.at(i).x, points.at(i).y, points.at(i).radius);
					//display position
					pdc->SelectObject(Greenpen);

					Arc(pdc->m_hDC,
						pt.x - pt.radius,
						pt.y - pt.radius,
						pt.x + pt.radius,
						pt.y + pt.radius,
						pt.x + pt.radius,
						pt.y,
						pt.x + pt.radius,
						pt.y
					);

				}
			}
		}
		msg.Format(TEXT("平均化相近的中心点后数目= %d"), m_vCenterPoints.size());
		MessageBox(msg);

		double r0, r;
		int tx, ty;
		// 去掉被包含的圆
		//Blue 相近- delete
		bool bdelete = false;
		pdc->SelectObject(Bluepen1);
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			x0 = m_vCenterPoints.at(i).x;
			y0 = m_vCenterPoints.at(i).y;
			r0 = m_vCenterPoints.at(i).radius;
			for (unsigned int j = i + 1; j < m_vCenterPoints.size() - 1; j++)
			{
				int x = m_vCenterPoints.at(j).x;
				int y = m_vCenterPoints.at(j).y;
				r = m_vCenterPoints.at(j).radius;
				if (DISTANCE(x0, y0, x, y) < abs(r0 - r) + 1) // 包含
				{
					bdelete = true;
					if (r0 > r) // 去掉r0
						pt = m_vCenterPoints.at(i);
					else
						pt = m_vCenterPoints.at(j);
					Arc(pdc->m_hDC,
						pt.x - pt.radius,
						pt.y - pt.radius,
						pt.x + pt.radius,
						pt.y + pt.radius,
						pt.x + pt.radius,
						pt.y,
						pt.x + pt.radius,
						pt.y
					);
					if (r0 > r) // 去掉r0
					{
						m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
						i--;
					}
					else
						m_vCenterPoints.erase(m_vCenterPoints.begin() + j);//(&m_vCenterPoints.at(j));
				}
			}
		}
		if (bdelete) {
			msg.Format(TEXT("去掉被包含的圆(Blue )后数目= %d"), m_vCenterPoints.size());
			MessageBox(msg);
		}

		vector<CENTER_POINT> tocheck;
		int total;
		bool isok;

		// 去掉潜在的错误(圆 r<9)
		pdc->SelectObject(Redpen1);
		bdelete = false;
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{	//baord area process
			CENTER_POINT centerp;
			centerp = m_vCenterPoints.at(i);
			if (centerp.x - centerp.radius < 0)
				centerp.radius -= (centerp.x - centerp.radius);
			if (centerp.y - centerp.radius < 0)
				centerp.radius -= (centerp.y - centerp.radius);
			if (centerp.x + centerp.radius > m_nWidth - 1)
				centerp.radius += (centerp.x + centerp.radius - m_nWidth);
			if (centerp.y + centerp.radius > m_nHeight - 1)
				centerp.radius += (centerp.y + centerp.radius - m_nHeight);

			if (m_vCenterPoints.at(i).radius < 8) // need adjust <
			{

				Arc(pdc->m_hDC,
					centerp.x - centerp.radius,
					centerp.y - centerp.radius,
					centerp.x + centerp.radius,
					centerp.y + centerp.radius,
					centerp.x + centerp.radius,
					centerp.y,
					centerp.x + centerp.radius,
					centerp.y
				);

				m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
				i--;
				bdelete = true;
			}
		}
		if (bdelete) {
			msg.Format(TEXT("去掉潜在的错误(圆 r<8 Redpen)后数目=%d"), m_vCenterPoints.size());
			MessageBox(msg);
		}
		bdelete = false;
		pdc->SelectObject(Bluepen1);
		// 去掉潜在的错误(同两个圆相交,并且不相交的部分是噪声)
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			tocheck.clear();
			x0 = m_vCenterPoints.at(i).x;
			y0 = m_vCenterPoints.at(i).y;
			r0 = m_vCenterPoints.at(i).radius;
			for (unsigned int j = 0; j < m_vCenterPoints.size(); j++)
			{
				if (i == j)
					continue;
				int x = m_vCenterPoints.at(j).x;
				int y = m_vCenterPoints.at(j).y;
				r = m_vCenterPoints.at(j).radius;
				if (DISTANCE(x0, y0, x, y) < abs(r0 + r)) // 相交
				{
					pt.x = x; pt.y = y; pt.radius = int(r);
					tocheck.push_back(pt);
				}
			}
			unsigned int size = (unsigned int)tocheck.size();
			if (size > 0) // 同两个以上的圆相交
			{
				total = 0;
				for (tx = int(x0 - r0); tx < x0 + r0; tx++)
					for (ty = int(y0 - r0); ty < y0 + r0; ty++)
					{
						if (DISTANCE(x0, y0, tx, ty) < r0) // 所有圆内部的点
						{
							if (tx<StartPoint.x || tx>EndPoint.x || ty<StartPoint.y || ty>EndPoint.y)
								continue;
							isok = true;
							for (unsigned int n = 0; n < size; n++)
							{
								pt = tocheck.at(n);// 取得
								if (DISTANCE(tx, ty, pt.x, pt.y) < pt.radius)
								{
									isok = false;
									break;
								}
							}
							if (isok) // 同所有的圆都不相交的部分
							{
								total++;
							}
						}
					}
				if (total < Pi*r0*r0*0.5) // need adjust 50%
				{
					CENTER_POINT centerp;
					centerp = m_vCenterPoints.at(i);
					Arc(pdc->m_hDC,
						centerp.x - centerp.radius,
						centerp.y - centerp.radius,
						centerp.x + centerp.radius,
						centerp.y + centerp.radius,
						centerp.x + centerp.radius,
						centerp.y,
						centerp.x + centerp.radius,
						centerp.y
					);
					m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
					i--; bdelete = true;
				}
			}
		}

		if (bdelete) {
			msg.Format(TEXT("去掉潜在的错误(同两个圆相交,并且不相交的部分是噪声)Bluepen后数目=%d"), m_vCenterPoints.size());
			MessageBox(msg);
		}

		DeleteObject(Redpen);
		DeleteObject(Greenpen);
		DeleteObject(Bluepen);


		// 恢复光标
		EndWaitCursor();
	}
	else
	MessageBox(TEXT("请先执行Shink"));
}


void Cprj284View::SaveIt(int i, int j, int radius)
{
	// TODO: 在此处添加实现代码.
	unsigned char *lpSrc;
	lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
		return;
	if (!(*lpSrc & CENTERED)) {
		CENTER_POINT pt;
		pt.x = i;
		pt.y = j;
		pt.radius = radius;
		points_temp.push_back(pt);

		// marke current point
		*lpSrc |= CENTERED;
	}

	*lpSrc &= NO_VISITED;//访问过才进栈，去掉则多次进栈

	if (*(lpSrc - 1) & VISITED)
	{
		SaveIt(i - 1, j, radius);
	}
	if (*(lpSrc + 1) & VISITED)
	{
		SaveIt(i + 1, j, radius);
	}
	if (*(lpSrc + m_nLineByte) & VISITED)
	{
		SaveIt(i, j - 1, radius);
	}
	if (*(lpSrc - m_nLineByte) & VISITED)
	{
		SaveIt(i, j + 1, radius);
	}

	if (*(lpSrc - m_nLineByte + 1) & VISITED)
	{
		SaveIt(i + 1, j + 1, radius);
	}
	if (*(lpSrc + m_nLineByte - 1) & VISITED)
	{
		SaveIt(i - 1, j + 1, radius);
	}
	if (*(lpSrc - m_nLineByte + 1) & VISITED)
	{
		SaveIt(i + 1, j - 1, radius);
	}
	if (*(lpSrc - m_nLineByte - 1) & VISITED)
	{
		SaveIt(i - 1, j - 1, radius);
	}
}


void Cprj284View::MarkIt(int i, int j)
{
	// TODO: 在此处添加实现代码.
	//CImgProcDoc* pDoc = GetDocument();
	unsigned char *lpSrc;

	lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	*(lpSrc) |= VISITED;

	//if (!m_bFullEdge)  //加入：1）速度慢 2）丢细胞
	//	return;

	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
	{
		return;
	}
	if (!(*(lpSrc - 1) & VISITED) &&	// 没有访问过
		*(lpSrc - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j); // 左边
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + 1) & VISITED) &&	// 没有访问过
		*(lpSrc + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j); // 右边
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte) & MARKED)	// 标志了
	{
		if (*(lpSrc + m_nLineByte) & EDGEPOINT)		// 并且是边缘
			MarkIt(i, j - 1); // 上面
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte) & MARKED)	// 标志了
	{
		if (*(lpSrc - m_nLineByte) & EDGEPOINT)		// 并且是边缘
			MarkIt(i, j + 1); // 下面
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte - 1) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + m_nLineByte - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j - 1); // 左上
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte - 1) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - m_nLineByte - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j + 1); // 左下
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte + 1) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + m_nLineByte + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j - 1); // 右上
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte + 1) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - m_nLineByte + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j + 1); // 右下
		else
			m_bFullEdge = false;
	}
}


void Cprj284View::CalcCenterArea(int i, int j)
{
	// TODO: 在此处添加实现代码.
	unsigned char *lpSrc;
	lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
	{
		return;
	}
	tot_area++;
	tot_x += i;
	tot_y += j;
	*lpSrc &= NO_CENTER;

	for (unsigned int n = 0; n < points_temp.size(); n++)
	{
		if (points_temp.at(n).x == i && points_temp.at(n).y == j)
		{
			if (points_temp.at(n).radius > max_radius)
				max_radius = points_temp.at(n).radius;
			break;
		}
	}

	if (*(lpSrc - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j);
	}
	if (*(lpSrc + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j);
	}
	if (*(lpSrc + m_nLineByte) & CENTERED)
	{
		CalcCenterArea(i, j - 1);
	}
	if (*(lpSrc - m_nLineByte) & CENTERED)
	{
		CalcCenterArea(i, j + 1);
	}

	if (*(lpSrc - m_nLineByte + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j + 1);
	}
	if (*(lpSrc - m_nLineByte - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j + 1);
	}
	if (*(lpSrc + m_nLineByte + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j - 1);
	}
	if (*(lpSrc + m_nLineByte - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j - 1);
	}
	
}


void Cprj284View::OnCellCount1()
{
	if (m_CellProc == 5)
	{
		// TODO: 在此添加命令处理程序代码
		m_CellProc = 0;
		//CImgProcDoc* pDoc = GetDocument();
		// reopen color image
		//OnFileReload();
		noclick = true; //重新加载文件?

		double tota, totr;
		tota = 0; totr = 0;
		double m_nHistHSI[256 * 3];
		unsigned char *lpSrc;
		for (int i = 0; i < 256 * 3; i++)
		{
			// 清零
			m_nHistHSI[i] = 0;
		}


		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			tota += double(m_vCenterPoints.at(i).radius) * double(m_vCenterPoints.at(i).radius * 3.14);
			totr += m_vCenterPoints.at(i).radius;

			//get Hsi 	
			for (int m = m_vCenterPoints.at(i).x - m_vCenterPoints.at(i).radius; m < m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius; m++)
				for (int n = m_vCenterPoints.at(i).y - m_vCenterPoints.at(i).radius; n < m_vCenterPoints.at(i).y + m_vCenterPoints.at(i).radius; n++)
				{
					if (m >= StartPoint.x && m <= EndPoint.x && n >= StartPoint.y && n <= EndPoint.y)
					{
						if (DISTANCE(m, n, m_vCenterPoints.at(i).x, m_vCenterPoints.at(i).y) <= m_vCenterPoints.at(i).radius)
						{
							lpSrc = (unsigned char *)m_pImage + m_nLineByte * (m_nHeight - 1 - n) + m * 3;
							RGB Rgb;
							HSI Hsi;
							Rgb.b = *lpSrc; Rgb.g = *(lpSrc + 1); Rgb.r = *(lpSrc + 2);
							RgbtoHsi(&Rgb, &Hsi);
							unsigned int H, S, I;
							H = (unsigned int)(Hsi.Hue / 360.0*255.0);
							S = (unsigned int)(Hsi.Saturation*255.0);
							I = (unsigned int)(Hsi.Intensity*255.0);

							// 计数加1
							m_nHistHSI[H]++;//H
							m_nHistHSI[256 + S]++;//S
							m_nHistHSI[256 * 2 + I]++;//I
						}
					}
				}

		}
		int min[3];
		int max[3];
		min[0] = min[1] = min[2] = 255;
		max[0] = max[1] = max[2] = 0;
		double add;
		for (int j = 0; j < 3; j++)
		{
			add = 0;
			for (int i = 0; i < 256; i++)
			{
				// 清零
				add += m_nHistHSI[j * 256 + i];
				if (add > tota / 20) {//>5%
					min[j] = i;
					i = 256;//out
				}

			}
		}
		for (int j = 0; j < 3; j++)
		{
			add = 0;
			for (int i = 255; i > 0; i--)
			{
				// 清零
				add += m_nHistHSI[j * 255 + i];
				if (add > tota / 20) {//>5%
					max[j] = i;
					i = 0;//out
				}

			}
		}
		
		Invalidate(true);
		MessageBox(TEXT("ReLoad Image"));


		CString msg;
		msg.Format(TEXT("共有%d个细胞,平均半径%d,平均面积%d"),
			m_vCenterPoints.size(),
			(int)(totr / m_vCenterPoints.size() + .5),
			(int)(tota / m_vCenterPoints.size() + .5));
		MessageBox(msg);
		/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &pFrame->m_wndStatusBar;
	if (pStatus)
		pStatus->SetPaneText(0, msg);
		*/
		//归一化
		for (int j = 0; j < 3; j++)
		{
			double max = 0;
			for (int i = 0; i < 256; i++)
				if (max < m_nHistHSI[j * 256 + i])
					max = m_nHistHSI[j * 256 + i];
			for (int i = 0; i < 256; i++)
				m_nHistHSI[j * 256 + i] /= max;
		}
		CDC *pdc = GetDC();
		CPen	Redpen;
		Redpen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));

		pdc->SelectObject(Redpen);
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++) {
			Arc(pdc->m_hDC,
				m_vCenterPoints.at(i).x - m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y - m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y
			);
		}
		
		
		DeleteObject(Redpen);
	}
	else
	MessageBox(TEXT("请先执行Find Center"));

}


void Cprj284View::OnAllDone1()
{
	// TODO: 在此添加命令处理程序代码
	OnCellMark1();
	OnCelltwoValue1();
	OnCellFillHoles1();
	OnCellShink1();
	OnCellFindCenter1();
	OnCellCount1();
}



