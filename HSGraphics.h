// HSGraphics.h: interface for the CHSGraphics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSGRAPHICS_H__290ECE04_CBBA_11D6_AD10_FE19737D63AF__INCLUDED_)
#define AFX_HSGRAPHICS_H__290ECE04_CBBA_11D6_AD10_FE19737D63AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define		DIR_LEFT		1
#define		DIR_RIGHT		2
#define		DIR_TOP			3
#define		DIR_BOTTOM		4


#define		ANIMATE_NONE				0
#define		ANIMATE_SLIDE_TTB			1
#define		ANIMATE_SLIDE_LTR			2
#define		ANIMATE_UNFOLD_MTTB			3
#define		ANIMATE_UNFOLD_MTLR			4
#define		ANIMATE_SHUTTER_TTB			5
#define		ANIMATE_SHUTTER_LTR			6
#define		ANIMATE_UNFOLD_LTTRB		7
#define		ANIMATE_UNFOLDSLIDE_LTTRB	8
#define		ANIMATE_STRETCH_TTB			9
#define		ANIMATE_STRETCH_LTR			10
#define		ANIMATE_NOISE				11
#define		ANIMATE_FADE				12
#define		ANIMATE_RAND				100


/////////////////////////////////    API   << CHSGraphics >>    /////////////////////////////////////////////

#define			STEP			128//255
#define			COLORNUM		8


///////////////////////////////////////////////////////////////////////////////////////////////
//$//								作者 : 韩 松                                 
//$//                                                                          
//$//			邮箱地址　：hs_china@hotmail.com								
//$//																		
//$//			作者声明　：此部分代码全是作者所写，可以随便传播，但要保持　　
//$//					   文件的完整性,如果您有问题或有好的建议、意见请您	
//$//					   给我来信,非常感谢!							
//$//　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
//////////////////////////////////////////////////////////////////////////////////////////////

	 HICON WINAPI LoadIcon			( int nIconID );

	 void WINAPI AlphaBlend			( HDC hdcResult, int nX, int nY, int nWidth, int nHeight, HDC hdcDest, int nXDest, int nYDest, HDC hdcSrc, int nXSrc, int nYSrc, BYTE bAlpha );

	 void WINAPI DrawAnimation		( RECT *prect, HDC hDCSrc, HDC hDCDest, int nType, int nStep, int nTimeDelay );

	 void WINAPI CreateRoundRectRgn	( HRGN hRgn, const RECT &rect, int nWidth, int nHeight, BOOL bLeftTopR, BOOL bLeftBottomR, BOOL bRightTopR, BOOL bRightBottomR);

	 void WINAPI RgnFromBmp			( HRGN hrgn, HDC hDC, UINT uID, COLORREF clrTran );

	 void WINAPI GetBitmapRgn		( HRGN hrgn, HDC hDC, UINT uID, COLORREF clrTran);

	 void WINAPI BitBltTran			( HDC hDC, int nLeft, int nTop, int nWidth, int nHeight, HDC hdcSrc, int nX, int nY, COLORREF clrTran );

	 void WINAPI DrawShadowIcon		( HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, COLORREF clrShadow );
	 void WINAPI DrawFadeIcon		( HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, UINT nRate );
	 void WINAPI DrawFadeBmp		( HDC hDC, int nX, int nY, HBITMAP hBmp, COLORREF clrTarget, UINT nRate );
	 void WINAPI DrawTranBmp		( HDC hDC, int nX, int nY, UINT uID, COLORREF clrTran );
	 void WINAPI DrawTranBmp		( HDC hDC, int nX, int nY, HBITMAP hBmp, int nWidth, int nHeight, COLORREF clrTran );

	 void WINAPI Draw3DRgnEdge		( HDC hDC, BOOL bHeave, HRGN rgnWnd );
	 void WINAPI Draw3DLine			( HDC hDC, BOOL bHeave, const POINT& ptStart, const POINT& ptEnd );
	 void WINAPI Draw3DRectEdge		( HDC hDC, BOOL bHeave, int nLeft, int nTop, int nRight, int nBottom );

	 void WINAPI DrawRgn			( HDC hDC, COLORREF clrBkgnd, HRGN hRgn );
	 void WINAPI DrawRgn			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, HRGN hRgn);
	 void WINAPI DrawFrameRgn		( HDC hDC, HRGN hRgn, COLORREF clrEdge, int nWidth, int nHeight );
	 void WINAPI DrawPartionRgn		( HDC hDC, COLORREF clrLeftTop, COLORREF clrRightBottom, BOOL bVerticalLine, int nOffset, HRGN hRgn);

	 void WINAPI DrawRect			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int  nBottom );
	 void WINAPI DrawRect			( HDC hDC, COLORREF clrFill, int nLeft, int nTop, int nRight, int nBottom );
	 void WINAPI DrawArrow			( HDC hDC, COLORREF clrArrow, int nX, int nY, int nSize, int nDirection = DIR_RIGHT);
	 void WINAPI DrawPolyline		( HDC hDC, COLORREF clrLine, POINT* pt, int nSize );
	 void WINAPI DrawEllipse			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight,  int nBottom );
	 void WINAPI DrawRoundRect		( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, const RECT& rect, int nWidth, int nHeight);
	 void WINAPI DrawRoundRect		( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom, int nWidth, int nHeight);
	 void WINAPI DrawRectEdge		( HDC hDC, const RECT& rect, int nWidth, COLORREF clrEdge, int nStyle = PS_SOLID);

	 void WINAPI DrawGradientRgn	( HDC hDC, BOOL bVertical, HRGN hRgn, COLORREF clrStart, COLORREF clrEnd );
	 void WINAPI DrawGradientRect	( HDC hDC, BOOL bVertical, const RECT& rectFill, COLORREF clrStart, COLORREF clrEnd );
	 void WINAPI DrawLightPointRect	( HDC hDC, const RECT &rectFill, const POINT &pointLight, COLORREF clrBkgnd, COLORREF clrPoint, float fFouce = 0.0f);


#endif // !defined(AFX_HSGRAPHICS_H__290ECE04_CBBA_11D6_AD10_FE19737D63AF__INCLUDED_)
