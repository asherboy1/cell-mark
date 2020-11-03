
// prj284View.h: Cprj284View 类的接口
//

#pragma once
#include <complex>
using namespace std;
#include <vector>
#include "afxwin.h"

struct  RGB
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

struct HSI
{
	double	Hue;
	double	Saturation;
	double	Intensity;
};

typedef struct pyy {
	BYTE b;
	BYTE g;
	BYTE r;
} PIX_VALUE;

struct CENTER_POINT
{
	int x;
	int y;
	int radius;
};

class Cprj284View : 
	public CView
{
protected: // 仅从序列化创建
	Cprj284View() noexcept;
	DECLARE_DYNCREATE(Cprj284View)

// 特性
public:
	Cprj284Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cprj284View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
protected:
	bool LoadBmp();

	bool display(CDC * pDC, CPoint origin, CSize size);


	CString EntName;     //图像文件扩展名  
	CString BmpName;     //图像文件名称  
	CBitmap m_bitmap;    //创建位图对象  

	CBitmap m_bitmaplin;   //创建临时位图对象进行处理---  
	CString BmpNameLin;    //保存图像副本文件---  
	CBitmap m_bitmaplin1;   //创建临时位图对象进行处理---  
	CString BmpNameLin1;    //保存图像副本文件---  
	CPoint m_LastPoint;
	CPoint StartPoint;
	CPoint EndPoint;
	bool m_bDrag = false;
	bool noclick = true;

	int next = 1;
	int m_CellProc = 0;

	int m_nWidth = 0;//图像实际宽度 
	int m_nHeight = 0;      //图像实际高度  
	int m_nDrawWidth = 0;   //图像显示宽度  
	int m_nDrawHeight = 0;  //图像显示高度  
	DWORD m_nImage = 0;     //图像数据的字节数 只含位图  
	DWORD m_nSize = 0;      //图像文件大小  
	long m_nLineByte = 0;    //图像一行所占字节数  
	long m_nBitCount = 0;    //图像每个像素所占位数  
	long m_nPalette = 0;     //位图实际使用的颜色表中的颜色数  

	BYTE* m_pImage{};  
	
	//读入图片数据后的指针  
	BITMAPFILEHEADER bfh{};   //全局变量文件头  
	BITMAPINFOHEADER bih{};   //全局变量信息头  
	RGBQUAD m_pPal{};         //颜色表指针  
	PIX_VALUE **pix = 0;
	const int TWOVALUE_H = 0x80;	// =TWOVALUE_H: set 1
	const int EDGEPOINT = 0x70;//|= set 1, & edge
	LPVOID m_lpvColorTable = NULL;
	const int MARK_VISITED = 0x81;//& 
	const int MARKED = 0x80;//|=set & mark
	const int VISITED = 0x01;//|= set 1 ,& visited
	const int pre_shrink_count = 3;
	const int NO_MARK = 0x7f;//& set 0
	const int NO_EDGE_POINT = 0x8f;//&= noedge
	const int CENTERED = 0x2;//|= set 1 ,& center
	const int NO_CENTER = 0xfd;//&= nocenter
	const double Pi = 3.14159;
	const unsigned char TWOVALUE_L = 0x0;
	const int NO_VISITED = 0xfe;//&= novisited 
	bool m_bFullEdge = true;
	long tot_area = 0, tot_x = 0, tot_y = 0, max_radius = 0;		// 用于递归
	vector<CENTER_POINT>	points_temp;// 用于临时存储CENTER_POINT
	vector<CENTER_POINT>	m_vCenterPoints;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void RgbtoHsi(RGB * pRgb, HSI * pHsi);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCellMark1();
	afx_msg void OnCelltwoValue1();
	afx_msg void OnCellFillHoles1();
	void ProcessFillHoles(int wd, int ht);
	afx_msg void OnCellShink1();
	void GenEdge(void);
	void GenEdge4(void);
	afx_msg void OnCellFindCenter1();
	void SaveIt(int i, int j, int radius);
	void MarkIt(int i, int j);
	void CalcCenterArea(int i, int j);
	afx_msg void OnCellCount1();
	afx_msg void OnAllDone1();

};

#ifndef _DEBUG  // prj284View.cpp 中的调试版本
inline Cprj284Doc* Cprj284View::GetDocument() const
   { return reinterpret_cast<Cprj284Doc*>(m_pDocument); }
#endif

