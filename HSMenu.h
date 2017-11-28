// HSMenu.h: interface for the CHSMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_)
#define AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define msg AfxMessageBox

#include	<string>
#include	<deque>
//using namespace std;



////////////////////////////////     CLASS   <<< HSITEM >>      ////////////////////////////

#define			ITEM_SEPARATOR				MF_SEPARATOR	//0x00000800		// Menu Item is Separator
#define			ITEM_GRAY					MF_GRAYED		//0x00000002		// Menu Item Disable Can Not Select
#define			ITEM_DISABLE				MF_DISABLED	//0x00000004		// Menu Item Disable Do Not Display in Menu
#define			ITEM_ENABLE					MF_ENABLED							// Enable Menu Item
#define			ITEM_CHECKED				MF_CHECKED
#define			ITEM_UNCHECKED				MF_UNCHECKED
#define			ITEM_POPUP					MF_POPUP
#define			ITEM_NORMAL					0x00000000
#define			ITEM_X						(ITEM_ENABLE|ITEM_DISABLE|ITEM_GRAY|ITEM_SEPARATOR|ITEM_POPUP)

//////////////////////////////////////////////////////////////////////////////////////////

class CHSMenu;

class HSITEM  
{
public:
	ULONG			m_ulData;
	UINT			m_uID;		
	UINT			m_uFlags;		
	CHSMenu*	    m_pPopupHSMenu;	
	std::string		m_strName;		
	int				m_nTop;			
	int				m_nHeight;

public:
//			 HSITEM( const HSITEM& MenuItem );
	inline   HSITEM();

//	HSITEM&	operator=( const HSITEM& MenuItem );

//protected:
//	void	Assign( const HSITEM& MenuItem );
};

//////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagDRAWITEMSTRUCTPRO
{
	HDC		hDC;
	UINT	uFlags;
	LPCSTR	lpcStr;
	ULONG	ulData;
	RECT	rect;
} DRAWITEMSTRUCTPRO, *LPDRAWITEMSTRUCTPRO;

//////////////////////////////////////////////////////////////////////////////////////////

#define			MENU_NORMAL			0X00000000			
#define			MENU_POPUPED		0X80000000			
#define			MENU_CHILD			0X40000000			
#define			MENU_PARENT			0X20000000			
#define			MENU_ROOT			0X10000000			
#define			MENU_SELECTED		0X08000000			
#define			MENU_TIMERTRACK		0X04000000			
#define			MENU_SELBORDER		0X02000000
#define			MENU_TIMERDESTROY	0X01000000
#define			MENU_OWNERDRAW		0X00800000
#define			MENU_DESTROYED		0X00400000

#define			MENU_REGISTERED		0X00200000
#define			MENU_HASCAPTURE		0X00100000

#define			MENU_XXXX			0XFFFF0000


#define			SELNONE				-1
#define			FINDNONE			-5
#define			MENUCAPTION			0
#define			TIMERTRACK			1
#define			TIMERDESTROY		2

//#define			CLR_BORDERSEL		0					// Color of the selected item
#define			CLR_BKGNDSEL		0						// Background of the selected item
#define			CLR_BKGNDMENU		1						// Normal background
#define			CLR_BORDERMENU		2
#define			CLR_BORDERSEL		3
#define			CLR_BORDERSPR		4				
#define			CLR_TEXTSEL			5						// Color of the selected item
#define			CLR_TEXTNORMAL		6						// Normal text color	
#define			CLR_ARROWSEL		7
//#define		CLR_TEXTGRAY		5						// Gray text color
#define			CLRCOUNT_BASE		8


///////////////////////////////////////////////////////////////////////////////////////////////
//$//								作者 : 韩 松                                 
//$//                                                                          
//$//			程序名称　：CHSMenu                                              
//$//			程序类型　：菜单类												
//$//			邮箱地址　：hs_china@yahoo.com								
//$//           QQ号码    ：102567329
//$//																		
//$//			作者声明　：此部分代码全是作者所写，可以随便传播，但要保持　　
//$//					   文件的完整性,如果您有问题或有好的建议、意见请您	
//$//					   给我来信,非常感谢!							
//$//　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　
//$//					   CHSMenu 类的使用细节请您参考附带的说明。		
//$//					   另有 CHSMenuPro 类(由CHSMenu派生)见文档      
//$//					   HSMenuPro.h/cpp							  
//$//																	
//////////////////////////////////////////////////////////////////////////////////////////////


DWORD CustomMenu(HWND hMenu);
DWORD CustomMenuItem( const RECT *prcItem );

typedef DWORD (*PFNCUSTOMMENU)( HWND hMenu );
typedef DWORD (*PFNCUSTOMMENUITEM)( const RECT* prcItem );

typedef std::deque<HSITEM>	DEQUEITEM;

class CHSMenu :public CObject
{
	DECLARE_DYNAMIC( CHSMenu );

	friend static LRESULT CALLBACK lpfnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	static WORD		GetVersion			( void );
	static HFONT	GetMenuFont			( void );
	static UINT		GetMenuPopDelay		( void );
	static COLORREF	GetColor			( int nIndex );
	static UINT		GetMenuAnimateDelay	( BOOL bAnimateMenu = TRUE );
	static UINT		GetMenuAnimateType	( BOOL bAnimateMenu = TRUE );
	static UINT		GetMenuAnimateStep	( BOOL bAnimateMenu = TRUE );

	static void		SetColor			( int nIndex, COLORREF clrSet );
	static void		SetMenuFont			( HFONT hFont );
	static void		SetMenuEdge			( int nEdge );
	static void		SetMenuPopDelay		( UINT nDelay );
	static void		SetMenuAnimate		( UINT nType, UINT nStep, UINT nDelay, BOOL bAnimateMenu = TRUE );

	static void		SetCustomMenuItem	( PFNCUSTOMMENUITEM pfnAddress );
	static void		SetCustomMenu		( PFNCUSTOMMENU pfnAddress );

	void			SetSeparatorHeight	( int nHeight );
	void			SetMenuItemHeight	( int nHeight );

	int				GetSeparatorHeight	( void );
	int				GetMenuItemHeight	( void );
	UINT			GetMenuEdge			( void ) const;
	int				GetCurSelItem		( void ) const;
	int				GetMenuItemCount	( void ) const;
	int				GetMenuPosition		( UINT uID ) const;
	void			GetMenuCXY			( int& nMenuWidth, int& nMenuHeight );
	UINT			GetMenuID			( int nPosition ) const;
	UINT			GetMenuState		( int nItem, UINT uFlag = MF_BYPOSITION ) const;
	CHSMenu*		GetSubMenu			( int nItem, UINT uFlag = MF_BYPOSITION ) const;
	BOOL			GetMenuString		( int nItem, CString& strName, UINT uFlag = MF_BYPOSITION ) const;
	BOOL			GetMenuString		( int nItem, LPTSTR lpsName, int nCount, UINT uFlag = MF_BYPOSITION )const;
	BOOL			GetMenuData			( int nItem, ULONG *pulData, UINT uFlag = MF_BYPOSITION ) const;
	BOOL			GetMenuItemRect		( int nItem, RECT *prcItem, UINT uFlag = MF_BYPOSITION ) const;

	BOOL			SetSubMenu			( int nItem, CHSMenu *pPopupMenu, UINT uFlag = MF_BYPOSITION );
	BOOL			SetMenuString		( int nItem, LPCTSTR lpszNewName, UINT uFlag = MF_BYPOSITION );
	BOOL			SetMenuData			( int nItem, ULONG ulData, UINT uFlag = MF_BYPOSITION );
	BOOL			SetMenuID			( int nItem, UINT uID, UINT uFlag = MF_BYPOSITION );
	BOOL			ModifyMenu			( int nItem, UINT uID, ULONG ulData, UINT uFlags, UINT uFlag = MF_BYPOSITION );
	BOOL			SetMenuState		( int nItem, UINT uFlags, UINT uFlag =MF_BYPOSITION );

	void			AppendMenu			( LPCTSTR pszName, UINT	uID, ULONG ulData = 0, UINT uFlags = 0 );								// Append a New Menu Item
//	void			AppendMenu			( UINT uFlag, UINT uID = 0, ULONG ulData = 0, LPCSTR pszName = NULL );
	BOOL			AppendPopup			( LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID = 0, ULONG ulData = 0, UINT uFlags = 0 );	// Append a Popup Item in Menu
	void			CheckMenuItem		( UINT	uID );
	BOOL			CreateMenu			( void );
	BOOL			DeleteMenu			( int nItem, UINT uFlag = MF_BYPOSITION );
	BOOL			EnableMenuItem		( int nItem, UINT uItemFlags = ITEM_ENABLE, UINT uFlag = MF_BYPOSITION );
	void			FreeMenu			( void );
	BOOL			LoadMenu			( UINT uIDResource, int nMenuPos = 0 );
	BOOL			InsertMenu			( int nItem, LPCTSTR pszName, UINT uID, ULONG ulData = 0, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION );
	BOOL			InsertPopup			( int nItem, LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID = 0, ULONG ulData = 0, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION );
	UINT			TrackPopMenu		( short x, short y, HWND hwndParent, UINT uFlag = 0 );							// Display and Track Menu		

	CHSMenu		();
	virtual		~CHSMenu();

protected:
//	virtual void	OnDestroy		( HWND hWnd );
	virtual void	OnPaint			( HWND hWnd, HDC hDC );
//	virtual void	OnChar			( HWND hWnd, char c );
	virtual void	OnKeyDown		( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags );
//	virtual void	OnKeyUp			( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags );
	virtual void	OnMouseMove		( HWND hWnd, short x, short y );

	virtual void	OnTimer			( HWND hWnd, unsigned short uID );
//	virtual void	OnEraseBkgnd	( HWND hWnd, HDC hDC );

	virtual void	OnLButtonDown	( HWND hWnd, short x, short y );
	virtual void	OnLButtonUp		( HWND hWnd, short x, short y );
	virtual void	OnRButtonDown	( HWND hWnd, short x, short y );
	virtual void	OnRButtonUp		( HWND hWnd, short x, short y );
	virtual void	OnKillFocus		( HWND hWnd, CWnd* pNewWnd );
protected:

	virtual void	MapMenuFrom		( CMenu *pMenu );

	virtual void	DrawItem		( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemText	( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemIcon	( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemArrow	( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemSpr		( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemBkgnd	( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	MeasureItem		( LPMEASUREITEMSTRUCT lpmis );
	virtual void	DrawMenuBkgnd	( HWND hWnd, HDC hDC );

	virtual void	DrawAnimateMenu		( RECT *pRect, HDC hDC, HDC hDCMem, int nType, int nStep, int nTimeDelay );
	virtual void	DrawAnimateMenuItem	( RECT *pRect, HDC hDC, HDC hDCMem, int nType, int nStep, int nTimeDelay );

	BOOL			IsEffectItem	( void );
	BOOL			GetState		( UINT uFlag ) const;

	BOOL			PointInMenu		( short x, short y );
	void			ResetDispPoint	( int& x, int& y );
	void			TimerDelay		( HWND hWnd );

	int				GetNewSel		( short x, short y );
	int				GetItemIndex	( int nItem, UINT uFlag ) const;
	void			GetPopMenuArray	( CHSMenu* pHSMenu, CPtrArray* pPtrArray );
	CHSMenu*		GetNewMenu		( HWND hWnd, short x, short y );

	void			RedrawItem		( HWND hWnd, int nCurSel, int nOldSel );
	void			DrawItemAnimation( LPDRAWITEMSTRUCTPRO lpdisp );

	inline void		DestroyMenu		( void );
	void			DestroyAllPop	( void );
	void			DestroyAllMenu	( void );

private:
	void			SetState		( UINT uFlag, int bSet );
	inline void		Initialize		( void );

protected:
	HWND			m_hMenu;
	HWND			m_hWndParent;

	int				m_nMenuWidth;
	int				m_nMenuHeight;

	int				m_nSprHeight;
	int				m_nMenuItemHeight;					

	CHSMenu		   *m_pParent;								

	int				m_nCurPop;
	int				m_nCurSel;								// Index fo current selected item

	UINT			m_uMenuFlag;							// Flag of this menu
	DEQUEITEM		m_qMenuItem;							// Save Menu Item Informations
	int				m_nIconSize;							//图标的大小
protected:
	static COLORREF	m_clrMenu[ CLRCOUNT_BASE ];
	static HFONT	m_hFont;
	static UINT		m_nFontHeight;
	static UINT		m_nPopupDelay;						
	static UINT		m_nMenuEdge;

	static UINT		m_nMenuAniType;
	static UINT		m_nMenuAniStep;
	static UINT     m_nMenuAniDelay;
	static UINT		m_nItemAniType;
	static UINT		m_nItemAniStep;
	static UINT     m_nItemAniDelay;

	static PFNCUSTOMMENU		m_pfnCustomMenu;
	static PFNCUSTOMMENUITEM	m_pfnCustomMenuItem;

private:
	static ULONG	m_ulMenuFlag;							// Register Flag		
};

#endif // !defined(AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_)
