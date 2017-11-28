// HSMenu.cpp: implementation of the CHSMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSMenu.h"
#include "HSGraphics.h"
#include "resource.h"
//#include "HSAnimationMemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


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



//////////////////////////////////////////////////////////////////////
// HSITEM Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
void HSITEM::Assign(const HSITEM &MenuItem)
{
	m_uID			= MenuItem.m_uID; 
	m_ulData		= MenuItem.m_ulData;
	m_uFlags		= MenuItem.m_uFlags;
	m_nTop			= MenuItem.m_nTop;
	m_strName		= MenuItem.m_strName;
	m_pPopupHSMenu	= MenuItem.m_pPopupHSMenu;
}

HSITEM& HSITEM::operator =(const HSITEM &MenuItem)
{
	Assign( MenuItem );
	return *this;
}

HSITEM::HSITEM( const HSITEM& MenuItem )
{
	Assign( MenuItem );
}
*/
inline HSITEM::HSITEM()
{
	m_nTop			= 0;
	m_nHeight		= 0;
	m_uID			= 0; 
	m_ulData		= 0;
	m_uFlags		= ITEM_NORMAL;
	m_pPopupHSMenu	= NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*
CLR_BKGNDSEL		0
CLR_BKGNDMENU		1
CLR_BORDERMENU		2
CLR_BORDERSEL		3
CLR_BORDERSPR		4
CLR_TEXTSEL			5
CLR_TEXTNORMAL		6
*/

COLORREF CHSMenu::m_clrMenu[CLRCOUNT_BASE] =
											{ GetSysColor( COLOR_HIGHLIGHT ),
											  GetSysColor( COLOR_MENU ),
											  RGB( 170, 170, 170 ),
											  RGB( 47 , 60 , 174 ),
											  RGB( 160, 160, 160 ),
											  GetSysColor( COLOR_HIGHLIGHTTEXT ),
											  GetSysColor( COLOR_MENUTEXT ),
											  GetSysColor( COLOR_HIGHLIGHTTEXT )
											};

UINT	CHSMenu::m_nMenuEdge		= 3;
UINT	CHSMenu::m_nPopupDelay		= 300;
UINT	CHSMenu::m_nFontHeight		= 18;
UINT	CHSMenu::m_nMenuAniType		= ANIMATE_NONE;
UINT    CHSMenu::m_nMenuAniStep		= 12;
UINT    CHSMenu::m_nMenuAniDelay	= 100;
UINT	CHSMenu::m_nItemAniType		= ANIMATE_NONE;
UINT    CHSMenu::m_nItemAniStep		= 4;
UINT    CHSMenu::m_nItemAniDelay	= 60;
HFONT	CHSMenu::m_hFont			= (HFONT)GetStockObject( DEFAULT_GUI_FONT );
ULONG	CHSMenu::m_ulMenuFlag		= 0;

PFNCUSTOMMENU		CHSMenu::m_pfnCustomMenu	 = (PFNCUSTOMMENU)NULL;
PFNCUSTOMMENUITEM	CHSMenu::m_pfnCustomMenuItem = (PFNCUSTOMMENUITEM)NULL;

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CHSMenu, CObject );

WORD CHSMenu::GetVersion()
{
	return MAKEWORD( 3, 8 );
}

CHSMenu::CHSMenu()
{
	m_nIconSize=32;
}

CHSMenu::~CHSMenu()
{
}

// Create a HSMenu
BOOL CHSMenu::CreateMenu( void )
{
	Initialize();

	// Register HSMenu
	if ( (m_ulMenuFlag & MENU_REGISTERED) == 0 )
	{
		WNDCLASS	wc;

		wc.style			= 0;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.lpszMenuName		= NULL;
		wc.hbrBackground	= NULL;
		wc.hIcon			= NULL;

		wc.hCursor			= ::LoadCursor( NULL, IDC_ARROW );
		wc.hInstance		= AfxGetInstanceHandle();
		wc.lpfnWndProc		= lpfnWndProc;
		wc.lpszClassName	= "HSMENU_PRODUCT_BY_HANSONG";

		if ( !::RegisterClass( &wc ) )
			return FALSE;

		m_uMenuFlag |= MENU_REGISTERED;
	}

	return TRUE;
}

// Initialize Parameter
inline void CHSMenu::Initialize( void )
{
	m_pParent		= NULL;
	m_uMenuFlag		= MENU_NORMAL;
	m_nMenuHeight	= m_nMenuEdge*2 + MENUCAPTION;
	m_nMenuWidth	= m_nMenuEdge*2 + 0;
	m_nCurSel		= SELNONE;
	m_nCurPop		= SELNONE;
	m_nSprHeight	= 5;
	m_nMenuItemHeight = 22;

	m_qMenuItem.clear();
}

void CHSMenu::SetState(UINT uFlag, int bSet)
{
	if ( bSet )
		 m_uMenuFlag |=  ( uFlag );
	else m_uMenuFlag &= ~( uFlag );
}

BOOL CHSMenu::GetState(UINT uFlag) const
{
	return ( ( m_uMenuFlag & ( uFlag ) ) != 0ul );
}
/*
void CHSMenu::AppendMenu(UINT uFlag, UINT uID, ULONG ulData, LPCSTR pszName)
{
	AppendMenu( pszName, uID, ulData, uFlag );
}
*/
// Append a New menu item to end
void CHSMenu::AppendMenu(LPCTSTR pszName, UINT uID, ULONG ulData, UINT uFlags)
{
	HSITEM	MenuItem;

	MenuItem.m_uFlags = ( uFlags & ITEM_X );
	MenuItem.m_ulData  = ulData;
	MenuItem.m_uID = uID;
	MenuItem.m_strName = pszName ? pszName : "( Empty )";

	// Save New Item informations
	m_qMenuItem.push_back( MenuItem );
}

// Append a popup menu item
BOOL CHSMenu::AppendPopup(LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID, ULONG ulData, UINT uFlags)
{
	if ( pPopupMenu && pPopupMenu->m_pParent )
		return FALSE;

	AppendMenu( pszName, uID, ulData, uFlags&~ITEM_SEPARATOR );

	return SetSubMenu( m_qMenuItem.size()-1, pPopupMenu, MF_BYPOSITION );
}

// Insert a menu item 
BOOL CHSMenu::InsertMenu(int nItem, LPCTSTR pszName, UINT uID, ULONG ulData, UINT uFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	if ( m_qMenuItem.size() < 2 )
		AppendMenu( pszName, uID, ulData, uFlags );
	else
	{
		HSITEM	MenuItem;
		HSITEM& tMenuItem = m_qMenuItem[nItem];

		MenuItem.m_uFlags = ( uFlags & ITEM_X );
		MenuItem.m_ulData  = ulData;
		MenuItem.m_uID = uID;
		MenuItem.m_strName = pszName ? pszName : "( Empty )";

		// Save New Item informations
		m_qMenuItem.insert( m_qMenuItem.begin() + nItem, MenuItem );
	}

	return TRUE;
}

BOOL CHSMenu::InsertPopup(int nItem, LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID, ULONG ulData, UINT uFlags, UINT uFlag)
{
	if ( m_qMenuItem.size() < 2 )
		return AppendPopup( pszName, pPopupMenu, uID, ulData, uFlags );
	else
	{
		if ( ( pPopupMenu && pPopupMenu->m_pParent ) ||
		     ( uFlags & ITEM_SEPARATOR )			 ||
			 !InsertMenu( nItem, pszName, uID, ulData, uFlags, uFlag )
		   ) return FALSE;

		SetSubMenu( nItem, pPopupMenu, uFlag );
	}
	
	return TRUE;
}

BOOL CHSMenu::LoadMenu(UINT uIDResource, int nMenuPos )
{
	CMenu *pMenu = new CMenu;

	if ( pMenu->LoadMenu( uIDResource ) )
	{
//		FreeMenu();
		MapMenuFrom( pMenu->GetSubMenu( nMenuPos ) );
		pMenu->DestroyMenu();

		delete pMenu;
		return TRUE;
	}

	delete pMenu;
	return FALSE;
}


void CHSMenu::MapMenuFrom( CMenu *pMenu )
{
	CString strText;
	int nTotalMenu	 = pMenu->GetMenuItemCount();

	CreateMenu();

	for ( int nPosition=0; nPosition<nTotalMenu; nPosition++ )
	{
 		pMenu->GetMenuString( nPosition, strText, MF_BYPOSITION );

 		UINT	uID		= pMenu->GetMenuItemID( nPosition );
 		UINT	uFlags	= pMenu->GetMenuState( nPosition, MF_BYPOSITION );

 		if ( uID == (UINT)-1 )
 		{
 			CMenu *pMenuPop = pMenu->GetSubMenu( nPosition );

 			if ( pMenuPop )
 			{
 				CHSMenu *pHSMenuPop = new CHSMenu;
  				pHSMenuPop->MapMenuFrom( pMenuPop );
				AppendPopup( strText, pHSMenuPop, uID, NULL, uFlags );
			}
 		}
 		else
 		{
 			AppendMenu( strText, uID, NULL, uFlags );
 		}
	}
}


void CHSMenu::FreeMenu()
{
	CPtrArray	arypHSMenu;

	GetPopMenuArray( this, &arypHSMenu );
	DestroyAllMenu();

	//Start from 1
	int nTotalItem = arypHSMenu.GetSize();

	for ( int nItem=1; nItem<nTotalItem; nItem++ )
	{
		CHSMenu *pHSMenu = (CHSMenu*)arypHSMenu.GetAt( nItem );

		delete pHSMenu;		//  内存泄漏?????????????
	}

	Initialize();
}

void CHSMenu::GetPopMenuArray( CHSMenu* pHSMenu, CPtrArray* pPtrArray )
{
	pPtrArray->Add( (void*)pHSMenu );

	int nTotalMenu = pHSMenu->GetMenuItemCount();

	for ( int nItem=0; nItem<nTotalMenu; nItem++ )
	{
		CHSMenu *pDel = m_qMenuItem[nItem].m_pPopupHSMenu;

		if ( pDel )
			 pDel->GetPopMenuArray( pDel, pPtrArray );
	}
}
// Delete menu item
BOOL CHSMenu::DeleteMenu(int nItem, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	SetSubMenu( nItem, NULL, uFlag );

	if ( nItem == ( m_qMenuItem.size() - 1 ) )	// must do this first
	{	// Remove The Last Item
		m_nMenuHeight -= m_qMenuItem[nItem].m_nHeight;
		m_qMenuItem.pop_back();
	}
	else
	if ( nItem == 0 )
		m_qMenuItem.pop_front();
	else
		m_qMenuItem.erase( m_qMenuItem.begin() + nItem );

	int iend = m_qMenuItem.size();

	return TRUE;
}

// Enable or Disable a Menu item
BOOL CHSMenu::EnableMenuItem(int nItem, UINT uItemFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;
	
	if ( uItemFlags == ITEM_ENABLE )
	{
		m_qMenuItem[nItem].m_uFlags &= ~( ITEM_DISABLE | ITEM_GRAY );
	}
	else
	{
		if ( uItemFlags & ITEM_DISABLE )
			m_qMenuItem[nItem].m_uFlags |= ITEM_DISABLE;

		if ( uItemFlags & ITEM_GRAY )
			m_qMenuItem[nItem].m_uFlags |= ITEM_GRAY;
	}

	return TRUE;
}

void CHSMenu::CheckMenuItem	( UINT	uID)
{
	for(int i=0;i<m_qMenuItem.size();i++)
	{
		if(m_qMenuItem[i].m_uID == uID)
		{
			m_qMenuItem[i].m_uFlags |= ITEM_CHECKED;
			break;
		}
	}
}
// Return Current Selected Item, none to return ( SELNONE )
int CHSMenu::GetCurSelItem( void ) const
{
	return m_nCurSel;
}

// Get the Total item 
int CHSMenu::GetMenuItemCount() const
{
	return (int)m_qMenuItem.size();
}

// Private FN
int CHSMenu::GetItemIndex(int nItem, UINT uFlag) const
{
	if ( m_qMenuItem.size() == 0 )
		return FINDNONE;

	int nIndex = FINDNONE;

	if ( uFlag == MF_BYPOSITION )
	{
		if ( nItem >= 0 && nItem < m_qMenuItem.size() ) 
			nIndex = nItem;
	}
	else
	{
		DEQUEITEM::size_type  index, iend;

		iend = m_qMenuItem.size();

		for ( index = 0; index < iend; index++ )
			if ( m_qMenuItem[index].m_uID == (UINT)nItem ) 	
			{
				nIndex = (int)index;
				break;
			}
	}

	return nIndex;
}

// Get the submenu of this item, return NULL if none
CHSMenu* CHSMenu::GetSubMenu(int nItem, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return NULL;

	return (CHSMenu*)m_qMenuItem[nItem].m_pPopupHSMenu;
}

// Set a submenu for this item
BOOL CHSMenu::SetSubMenu(int nItem, CHSMenu *pPopupMenu, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	if ( pPopupMenu && pPopupMenu->m_pParent )
		return FALSE;
	
	HSITEM& MenuItem = m_qMenuItem[nItem];

	if ( MenuItem.m_uFlags & ITEM_SEPARATOR )
		return FALSE;

	if ( MenuItem.m_pPopupHSMenu )
	{
		MenuItem.m_uFlags &= ~ITEM_POPUP;
		MenuItem.m_pPopupHSMenu->m_pParent = NULL;
		MenuItem.m_pPopupHSMenu->SetState( MENU_CHILD, FALSE );
	}

	if ( pPopupMenu )
	{
		MenuItem.m_uFlags |= ITEM_POPUP;
		pPopupMenu->m_pParent = this;
		pPopupMenu->SetState( MENU_CHILD, TRUE );
	}

	MenuItem.m_pPopupHSMenu = pPopupMenu;
	return TRUE;
}

// Set addition data for a item
BOOL CHSMenu::SetMenuData(int nItem, ULONG ulData, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	m_qMenuItem[nItem].m_ulData = ulData;
	return TRUE;
}

// Get menu item text
BOOL CHSMenu::GetMenuString(int nItem, LPTSTR lpsName, int nCount, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	return ( lstrcpyn( lpsName, m_qMenuItem[nItem].m_strName.c_str(), nCount ) != NULL );
}

// Set menu item text
BOOL CHSMenu::GetMenuString(int nItem, CString &strName, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	strName = m_qMenuItem[nItem].m_strName.c_str();
	return TRUE;
}

// Set Menu Item Text
BOOL CHSMenu::SetMenuString(int nItem, LPCTSTR lpszNewName, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	m_qMenuItem[nItem].m_strName = lpszNewName ? lpszNewName : "( Empty )";

	return TRUE;
}

// Get addition item date
BOOL CHSMenu::GetMenuData(int nItem, ULONG *pulData, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	*pulData = m_qMenuItem[nItem].m_ulData;
	return TRUE;
}

BOOL CHSMenu::GetMenuItemRect(int nItem, RECT *prcItem, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	const HSITEM& MenuItem = m_qMenuItem[nItem];

	prcItem->left	= m_nMenuEdge;
	prcItem->top	= MenuItem.m_nTop;
	prcItem->right	= m_nMenuWidth - m_nMenuEdge;
	prcItem->bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

	return TRUE;
}

// Get Menu item Flags
UINT CHSMenu::GetMenuState(int nItem, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return 0;

	return ( m_qMenuItem[nItem].m_uFlags );
}


BOOL CHSMenu::SetMenuState(int nItem, UINT uFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	m_qMenuItem[nItem].m_uFlags = (  uFlags & ITEM_X );

	return TRUE;
}

BOOL CHSMenu::ModifyMenu(int nItem, UINT uID, ULONG ulData, UINT uFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	HSITEM& MenuItem  = m_qMenuItem[nItem];
	MenuItem.m_uID    = uID;
	MenuItem.m_ulData  = ulData;
	MenuItem.m_uFlags = ( uFlags & ITEM_X );

	return TRUE;
}

// Set menu item ID
BOOL CHSMenu::SetMenuID(int nItem, UINT uID, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return FALSE;

	m_qMenuItem[nItem].m_uID = uID;
	return TRUE;
}

int CHSMenu::GetMenuPosition(UINT uID) const
{
	return GetItemIndex( uID, MF_BYCOMMAND );
}

void CHSMenu::SetCustomMenu(PFNCUSTOMMENU pfnAddress)
{
	m_pfnCustomMenu = pfnAddress;
}

void CHSMenu::SetCustomMenuItem(PFNCUSTOMMENUITEM pfnAddress)
{
	m_pfnCustomMenuItem = pfnAddress;
}

// Get menu item id by position
UINT CHSMenu::GetMenuID(int nPosition) const
{
	if ( nPosition < 0 || nPosition >= m_qMenuItem.size() ) 
		return 0;

	return ( m_qMenuItem[nPosition].m_uID );
}

void CHSMenu::GetMenuCXY(int &nMenuWidth, int &nMenuHeight)
{
	m_nMenuHeight	= m_nMenuEdge*2 + MENUCAPTION;
	m_nMenuWidth	= m_nMenuEdge*2 + 0;

	int nTotalItem = m_qMenuItem.size();

	for ( int nIndex=0; nIndex<nTotalItem; nIndex++ )
	{
		MEASUREITEMSTRUCT	mis;

		mis.itemData = nIndex;
		MeasureItem( &mis );

		m_qMenuItem[nIndex].m_nTop	  = m_nMenuHeight - mis.itemHeight - m_nMenuEdge ;
		m_qMenuItem[nIndex].m_nHeight = mis.itemHeight;
	}

	nMenuWidth  = m_nMenuWidth;
	nMenuHeight = m_nMenuHeight;
}

void CHSMenu::SetMenuAnimate(UINT nType, UINT nStep, UINT nDelay, BOOL bAnimateMenu )
{
	if ( bAnimateMenu )
	{
		m_nMenuAniType  = nType;
		m_nMenuAniStep	= nStep;
		m_nMenuAniDelay = nDelay;
	}
	else
	{
		m_nItemAniType  = nType;
		m_nItemAniStep	= nStep;
		m_nItemAniDelay = nDelay;
	}
}

UINT CHSMenu::GetMenuAnimateType( BOOL bAnimateMenu )
{
	return bAnimateMenu ? m_nMenuAniType : m_nItemAniType;
}

UINT CHSMenu::GetMenuAnimateStep( BOOL bAnimateMenu )
{
	return bAnimateMenu ? m_nMenuAniStep : m_nItemAniStep;
}

UINT CHSMenu::GetMenuAnimateDelay( BOOL bAnimateMenu )
{
	return bAnimateMenu ? m_nMenuAniDelay : m_nItemAniDelay;
}

// Static FN Set Menu Font
void CHSMenu::SetMenuFont(HFONT hFont)
{
	m_hFont = ( hFont ? hFont : (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
}

HFONT CHSMenu::GetMenuFont( void )
{
	return m_hFont;
}

//Static FN Set menu edge width
void CHSMenu::SetMenuEdge(int nEdge)
{
	m_nMenuEdge = ( nEdge <= 0 ? 1 : nEdge );
}

//Static FN Get menu edge width
UINT CHSMenu::GetMenuEdge() const
{
	return m_nMenuEdge;
}

//Static FN Set submenu popup delay time
void CHSMenu::SetMenuPopDelay(UINT nDelay)
{
	m_nPopupDelay = nDelay;
}

//FN Set menu item height
void CHSMenu::SetMenuItemHeight(int nHeight)
{
	m_nMenuItemHeight = ( ( nHeight - 4 ) > m_nFontHeight ? nHeight : m_nFontHeight + 4 ) ;
}

//Static FN Set the menu color which you want to change
void CHSMenu::SetColor(int nIndex, COLORREF clrSet )
{
	if ( nIndex >= 0 && nIndex < CLRCOUNT_BASE )
		m_clrMenu[nIndex] = clrSet;
}

//Static FN Set menu Separator height 
void CHSMenu::SetSeparatorHeight(int nHeight)
{
	m_nSprHeight = abs( nHeight );
}

//Static FN  Get menu separator height
int CHSMenu::GetSeparatorHeight()
{
	return m_nSprHeight;
}

//Static FN Get submenu popup delay time
UINT CHSMenu::GetMenuPopDelay() 
{
	return m_nPopupDelay;
}

//FN Get menu item height
int CHSMenu::GetMenuItemHeight() 
{
	return m_nMenuItemHeight;
}

//Static FN Get menu color which you want to know
COLORREF CHSMenu::GetColor(int nIndex)
{
	if ( nIndex >= 0 && nIndex < CLRCOUNT_BASE )
		 return m_clrMenu[nIndex];
	
	return -1;
}

void CHSMenu::MeasureItem( LPMEASUREITEMSTRUCT lpMIS )
{
	HSITEM& MenuItem = m_qMenuItem[lpMIS->itemData];

	HGDIOBJ hfntOld;
	SIZE	sizeText;

	// To Get New Item Name width
	HDC	hDC = CreateIC( "DISPLAY", NULL, NULL, NULL );
	hfntOld = SelectObject( hDC, m_hFont );
	GetTextExtentPoint32( hDC, MenuItem.m_strName.c_str(), MenuItem.m_strName.length(), &sizeText );
	SelectObject( hDC, hfntOld );
	DeleteDC( hDC );
////////
	if ( MenuItem.m_uFlags & ITEM_SEPARATOR )
		 lpMIS->itemHeight = m_nSprHeight;	
	else
	{
		m_nFontHeight = sizeText.cy;

		if ( sizeText.cy > m_nMenuItemHeight )
			  lpMIS->itemHeight = sizeText.cy + 2 ;
		 else lpMIS->itemHeight = m_nMenuItemHeight;
	}

	lpMIS->itemWidth  = sizeText.cx;
///////
	if ( sizeText.cx + 75 > m_nMenuWidth - m_nMenuEdge*2 )
		 m_nMenuWidth = sizeText.cx + 75 + m_nMenuEdge*2;

	m_nMenuHeight += lpMIS->itemHeight;
}


// ( Point x and y is Screen point )
void CHSMenu::ResetDispPoint( int& x, int& y)
{
	static RECT	rectScreen;
	static BOOL bNoGet = TRUE;
	
	// Get System Screen Rectangle
	if ( bNoGet )
	{
		SystemParametersInfo( SPI_GETWORKAREA, 0, &rectScreen, 0 );
		bNoGet = FALSE;
	}

	if ( x + m_nMenuWidth > rectScreen.right )
	{
		if ( m_pParent )
			 x = x - m_nMenuWidth - m_pParent->CHSMenu::m_nMenuWidth + m_nMenuEdge*2;
		else x = x - m_nMenuWidth;
	}

	if ( y + m_nMenuHeight > rectScreen.bottom )
	{
		if ( m_pParent )
		{
			HSITEM& MenuItem = m_pParent->m_qMenuItem[m_pParent->m_nCurSel];

			 y = y - m_nMenuHeight + MenuItem.m_nHeight + m_nMenuEdge*2;
		}
		else y = y - m_nMenuHeight;
	}

	if ( x < 0 )	x = 0;
	if ( y < 0 )	y = 0;
}

// Display and track menu
UINT CHSMenu::TrackPopMenu(short x, short y, HWND hwndParent, UINT uFlag)
{
	int nX = x, nY = y;

	m_hWndParent = hwndParent;

	GetMenuCXY( m_nMenuWidth, m_nMenuHeight );
	ResetDispPoint( (int&)nX, (int&)nY );

	// Create Menu Window
	m_hMenu = ::CreateWindowEx( WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
								"HSMENU_PRODUCT_BY_HANSONG",
								"",
								WS_POPUP,
								nX,
								nY,
								m_nMenuWidth,
								m_nMenuHeight,
								m_hWndParent,
								NULL,
								AfxGetInstanceHandle(),
								(LPVOID)this );

	if ( !m_hMenu )	return FALSE;

	HRGN hrgnMenu = CreateRectRgn( 0, 0, m_nMenuWidth, m_nMenuHeight );
	::SetWindowRgn( m_hMenu, hrgnMenu, TRUE );
	::DeleteObject( hrgnMenu );

	if ( m_pfnCustomMenu && m_qMenuItem.size() )
		 m_pfnCustomMenu( m_hMenu );

	::ShowWindow( m_hMenu, SW_SHOW );

	::ReleaseCapture();
	::SetCapture( m_hMenu );
//	m_ulMenuFlag |= MENU_HASCAPTURE;

	if ( m_pParent )
	{
		m_pParent->CHSMenu::SetState( MENU_POPUPED, TRUE );
		m_pParent->CHSMenu::m_nCurPop = m_pParent->CHSMenu::m_nCurSel;
	}

	m_nCurSel = SELNONE;
	m_nCurPop = SELNONE;

	MSG	Msg;

	while ( GetMessage( &Msg, NULL, 0, 0 ) && IsWindow( m_hMenu ) )
	{
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}

	if ( m_nCurSel != SELNONE )
		return m_qMenuItem[m_nCurSel].m_uID;

	return (UINT)0;
}

// Update the Menu Item be Selected now
int CHSMenu::GetNewSel(short x, short y)
{
	int iend  = m_qMenuItem.size();

	// Outside menu active area ?
	if ( !(	x > m_nMenuEdge &&
			y > m_nMenuEdge &&
			x < m_nMenuWidth  - m_nMenuEdge &&
			y < m_nMenuHeight - m_nMenuEdge &&
			iend != 0
		  )
		) return SELNONE;

	// if current selected item Isn't none, Use it 
	// else find start at middle
	int index = ( m_nCurSel != SELNONE ) ? m_nCurSel : iend / 2;

	while ( TRUE )
	{
		HSITEM& MenuItem = m_qMenuItem[index];

		if ( y > MenuItem.m_nTop )
		{
			if ( y <= ( MenuItem.m_nTop + MenuItem.m_nHeight ) )
				return index;

			if ( ++index >= iend )	return SELNONE;
		}
		else
		{
			if ( --index < 0 )  return SELNONE;
		}
	}
}

// Get HSMenu When Mouse Move to SubMenu or Form Sub to Parent
CHSMenu* CHSMenu::GetNewMenu(HWND hWnd, short x, short y)
{
	HWND	hWndPoint;
	TCHAR	sClassName[50];
	POINT	pt;

	pt.x = x;
	pt.y = y;

	::ClientToScreen( hWnd, &pt );

	if ( ( hWndPoint = ::WindowFromPoint( pt ) ) == NULL )	return this;
	GetClassName( hWndPoint, sClassName, sizeof(sClassName)/sizeof(TCHAR) );
	// If it isn't CHSMenu Class return this menu, Mean no change Capture
	if ( lstrcmp( sClassName, "HSMENU_PRODUCT_BY_HANSONG" ) )	return this;

	// Return the CHSMenu Pointer, Have mouse Focus
	return ( ( CHSMenu* )( GetWindowLong( hWndPoint, GWL_USERDATA ) ) ); 
}

//void CHSMenu::OnChar( HWND hWnd, char c )
//{
//}

void CHSMenu::OnKeyDown( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nNewSel = m_nCurSel;
	int nOldSel = m_nCurSel;
	int nCount  = m_qMenuItem.size();
	
	nCount <= 0 ? nChar = VK_ESCAPE : nOldSel %= nCount;

	switch( nChar )
	{
		case VK_DOWN:
			if ( m_nCurSel == SELNONE ) 
			{
				nNewSel = -1;
				nOldSel = SELNONE;
			}

			do
			{
				nNewSel = ( nNewSel + 1 ) % nCount;
			}while ( m_qMenuItem[nNewSel].m_uFlags & ITEM_SEPARATOR );

			RedrawItem( hWnd, nNewSel, nOldSel );
			break;

		case VK_UP:
			if ( m_nCurSel == SELNONE ) 
			{
				nNewSel = nCount;
				nOldSel = SELNONE;
			}

			do
			{
				nNewSel = nNewSel ? ( nNewSel - 1 ) % nCount : nCount - 1;
			}while ( m_qMenuItem[nNewSel].m_uFlags & ITEM_SEPARATOR );

			RedrawItem( hWnd, nNewSel, nOldSel );
			break;

		case VK_RIGHT:
			if ( m_nCurSel != SELNONE )
			{
				const HSITEM& MenuItem = m_qMenuItem[m_nCurSel];

				if ( MenuItem.m_pPopupHSMenu && !( MenuItem.m_uFlags & ( ITEM_GRAY | ITEM_DISABLE ) ) )
				{
					POINT	ptDisp;	
					ptDisp.x = m_nMenuWidth - m_nMenuEdge;
					ptDisp.y = MenuItem.m_nTop - m_nMenuEdge;
					::ClientToScreen( hWnd, &ptDisp );
					// track the new menu
					MenuItem.m_pPopupHSMenu->TrackPopMenu( ptDisp.x, ptDisp.y, m_hWndParent );
				}
			}
			break;

		case VK_LEFT:
			if ( m_pParent )
			{
				::ReleaseCapture();
				::SetActiveWindow( m_pParent->CHSMenu::m_hMenu );
				::SetCapture( m_pParent->CHSMenu::m_hMenu );
				DestroyMenu();
			}
			break;

		case VK_ESCAPE:
			if ( m_pParent )
			{
				::ReleaseCapture();
				::SetActiveWindow( m_pParent->CHSMenu::m_hMenu );
				::SetCapture( m_pParent->CHSMenu::m_hMenu );
			}
			DestroyMenu();
			break;		
			
//		case VK_LWIN:
//		case VK_RWIN:
//			DestroyAllMenu();
			break;

		case VK_SPACE:
		case VK_RETURN:
			if ( IsEffectItem() )
			{
				DestroyAllMenu();
				::SendMessage( m_hWndParent, WM_COMMAND, MAKEWORD( m_qMenuItem[m_nCurSel].m_uID, 0 ),0 );
			}
	}
}
/*
void CHSMenu::OnKeyUp( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
}
*/

// Mouse Move Message
void CHSMenu::OnMouseMove(HWND hWnd, short x, short y)
{
	// No need to Call function "GetNewMenu" when this menu have no submenu
	if ( GetState( MENU_POPUPED ) || GetState( MENU_CHILD) )
	{
		CHSMenu *pNewMenu;
		// If Mouse move on a other HSMenu Change Capture
		if ( ( pNewMenu = GetNewMenu( hWnd, x, y ) ) != this )
		{
			if ( m_nCurSel != m_nCurPop )
			{
				RedrawItem( hWnd, m_nCurPop, m_nCurSel );
				TimerDelay( hWnd );
			}

			::ReleaseCapture();
			// Reset Capture To New (Under Point)Menu
//			::SetActiveWindow( pNewMenu->m_hMenu );
			::SetCapture( pNewMenu->m_hMenu );
			return;
		}
	}

	// Get the new item be selected now
	int	nNewSel = GetNewSel( x, y );
	// if Select no change return
	if ( nNewSel != m_nCurSel )
	{
		RedrawItem( hWnd, nNewSel, m_nCurSel );
		TimerDelay( hWnd );
	}
}

void CHSMenu::RedrawItem(HWND hWnd, int nCurSel, int nOldSel)
{
	DRAWITEMSTRUCTPRO	disp;

	disp.hDC = ::GetDC( hWnd );
	disp.rect.left  = m_nMenuEdge;
	disp.rect.right	= m_nMenuWidth - m_nMenuEdge;
	
	// Clear Old selected item background
	if ( nOldSel != SELNONE )
	{	
		SetState( MENU_SELECTED, FALSE );		// Flag for Clear

		HSITEM& MenuItem = m_qMenuItem[nOldSel];

		disp.ulData		 = MenuItem.m_ulData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		DrawItemAnimation( &disp );
	}

	// Draw the new select item background
	if ( nCurSel != SELNONE )
	{
		SetState( MENU_SELECTED, TRUE );	// Draw Select 

		HSITEM& MenuItem = m_qMenuItem[nCurSel];

		disp.ulData		 = MenuItem.m_ulData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		DrawItemAnimation( &disp );
	}

	::ReleaseDC( hWnd, disp.hDC );
	m_nCurSel = nCurSel;
}

void CHSMenu::OnTimer(HWND hWnd, unsigned short uID)
{
	::KillTimer( hWnd, uID );

	switch ( uID )
	{ 
		case TIMERTRACK:
			if ( !GetState( MENU_POPUPED ) && m_nCurSel != SELNONE )
			{
				POINT	ptDisp;	
				const	HSITEM& MenuItem = m_qMenuItem[m_nCurSel];
				// the init disp point
				ptDisp.x = m_nMenuWidth - m_nMenuEdge ;
				ptDisp.y = MenuItem.m_nTop - m_nMenuEdge - MENUCAPTION;
				::ClientToScreen( hWnd, &ptDisp );
				// track the new menu
				MenuItem.m_pPopupHSMenu->TrackPopMenu( ptDisp.x, ptDisp.y, m_hWndParent );
			}
			else
			::SetTimer( hWnd, TIMERTRACK, 10, NULL );
			break;

		case TIMERDESTROY:
			SetState( MENU_TIMERDESTROY, FALSE );
			DestroyAllPop();
			break;
	}
}

void CHSMenu::TimerDelay(HWND hWnd)
{
	::KillTimer( hWnd, TIMERTRACK );

	if ( !GetState( MENU_TIMERDESTROY ) && GetState( MENU_POPUPED ) )
	{
		SetState( MENU_TIMERDESTROY, TRUE );
		::SetTimer( hWnd, TIMERDESTROY, m_nPopupDelay, NULL );
	}

	// Draw the new select item background
	if ( m_nCurSel != SELNONE )
	{
		if ( m_nCurSel == m_nCurPop )
		{
			::KillTimer( hWnd, TIMERTRACK );
			::KillTimer( hWnd, TIMERDESTROY );
			SetState( MENU_TIMERDESTROY, FALSE );
		}
		else	// Popup a New Menu ?
		{
			const HSITEM& MenuItem = m_qMenuItem[m_nCurSel];

			if (  MenuItem.m_pPopupHSMenu && !(MenuItem.m_uFlags & ITEM_GRAY) )
			{
				::SetTimer( hWnd, TIMERTRACK, m_nPopupDelay, NULL );
			}
		}
	}
}

void CHSMenu::DrawAnimateMenuItem(RECT *pRect, HDC hDC, HDC hDCMem, int nType, int nStep, int nTimeDelay)
{
	DrawAnimation( pRect, hDC, hDCMem, nType, nStep, nTimeDelay ); 
/*
	int nWidth		= pRect->right	- pRect->left;
	int nHeight		= pRect->bottom	- pRect->top;
	::BitBlt( hDC, pRect->left, pRect->top, nWidth, nHeight, hDCMem, 0, 0, SRCCOPY );
*/
}

void CHSMenu::DrawItemAnimation(LPDRAWITEMSTRUCTPRO lpdisp )
{
	if ( ( lpdisp->uFlags & MENU_SELECTED ) && !( lpdisp->uFlags & ITEM_SEPARATOR ) )
	{
		int		nWidth  = lpdisp->rect.right - lpdisp->rect.left;
		int		nHeight = lpdisp->rect.bottom- lpdisp->rect.top;

		HDC		hdcMem  = ::CreateCompatibleDC( lpdisp->hDC );
		HBITMAP hbmpMem = ::CreateCompatibleBitmap( lpdisp->hDC, nWidth, nHeight );
		HGDIOBJ hobjOld = ::SelectObject( hdcMem, hbmpMem );
		::BitBlt( hdcMem, 0, 0, nWidth, nHeight, lpdisp->hDC, lpdisp->rect.left, lpdisp->rect.top, SRCCOPY );

//------------------------------------------------------------------------------------------

		DRAWITEMSTRUCTPRO	disp;

		disp.hDC			= hdcMem;
		disp.lpcStr			= lpdisp->lpcStr;
		disp.ulData			= lpdisp->ulData;
		disp.uFlags			= lpdisp->uFlags;
		disp.rect.left		= 0;
		disp.rect.top		= 0;
		disp.rect.right		= nWidth;
		disp.rect.bottom	= nHeight;

		DrawItem( &disp );

//------------------------------------------------------------------------------------------

		DrawAnimateMenuItem( &lpdisp->rect, lpdisp->hDC, disp.hDC, m_nItemAniType, m_nItemAniStep, m_nItemAniDelay ); 

		::SelectObject( hdcMem, hobjOld );
		::DeleteObject( hbmpMem );
		::DeleteDC( hdcMem );
	}
	else
	{
		DrawItem( lpdisp );
	}
}
void CHSMenu::OnKillFocus( HWND hWnd,CWnd* pNewWnd)
{
}

// Draw the menu item which you want
void CHSMenu::DrawItem(LPDRAWITEMSTRUCTPRO lpdisp)
{
	// Draw a separator
	if ( lpdisp->uFlags & ITEM_SEPARATOR )
	{
		DrawItemBkgnd( lpdisp );
		DrawItemSpr( lpdisp );
		return;
	}

	// Draw or Clear Select Background
	DrawItemBkgnd( lpdisp );

	// Draw Icon
	DrawItemIcon( lpdisp );
	// Draw the Menu item Text
	DrawItemText( lpdisp );

	// if this Item has son, then Graphics arrowhead
	if ( lpdisp->uFlags & ITEM_POPUP )
		DrawItemArrow( lpdisp );
}

// Draw(Selected) or Clear(Leave) the item Background 
void CHSMenu::DrawItemBkgnd(LPDRAWITEMSTRUCTPRO lpdisp)
{
	int		nClrBorder, nClrBk;
	RECT&	rect = lpdisp->rect;
	HRGN	hrgnItem = NULL;
	
	if ( GetState( MENU_SELECTED ) && !( lpdisp->uFlags & ITEM_SEPARATOR ) )
	{
		nClrBk	   = CLR_BKGNDSEL;
		nClrBorder = CLR_BORDERSEL;
	}
	else
	{
		nClrBk	   = CLR_BKGNDMENU;
		nClrBorder = CLR_BKGNDMENU;
	}

	if ( m_pfnCustomMenuItem )
		 hrgnItem = (HRGN)m_pfnCustomMenuItem( &rect );

	if ( hrgnItem == NULL )
	{
		DrawRect(	lpdisp->hDC, 
					m_clrMenu[nClrBorder], 
					m_clrMenu[nClrBk], 
					rect.left, 
					rect.top, 
					rect.right, 
					rect.bottom	 );
	}
	else
	{
		DrawRgn(lpdisp->hDC, m_clrMenu[nClrBorder], m_clrMenu[nClrBk], hrgnItem );
		::DeleteObject( hrgnItem );
	}
}
//绘制菜单项图标
void CHSMenu::DrawItemIcon(LPDRAWITEMSTRUCTPRO lpdisp)
{
	if( lpdisp->uFlags & ITEM_CHECKED )	//菜单项是check状态
	{
		POINT	point;
		CRect	rect = lpdisp->rect;
		point.x = rect.left;
		point.y = (rect.top+rect.bottom-m_nIconSize)/2;
		HICON	hIcon=LoadIcon(IDI_CHECK);
		DrawIcon(lpdisp->hDC,point.x,point.y,hIcon);
	}
}
// Draw Menu Item's Name
void CHSMenu::DrawItemText(LPDRAWITEMSTRUCTPRO lpdisp)
{
	HGDIOBJ		hfntOld;
	COLORREF	clrText;
	RECT		rect;
	RECT&		rectT = lpdisp->rect;

	rect.left   = rectT.left + 20;
	rect.top    = ( rectT.top + rectT.bottom - m_nFontHeight ) / 2;
	rect.right	= 1000;
	rect.bottom	= 100;
	
	if ( lpdisp->uFlags & ITEM_GRAY )	 
		clrText = GetSysColor( COLOR_GRAYTEXT );
	else
		clrText = m_clrMenu[GetState( MENU_SELECTED ) ? CLR_TEXTSEL : CLR_TEXTNORMAL];
	
	SetTextColor( lpdisp->hDC, clrText );
	SetBkMode( lpdisp->hDC, TRANSPARENT );
	hfntOld = SelectObject( lpdisp->hDC, m_hFont );
	DrawText( lpdisp->hDC, lpdisp->lpcStr, -1, &rect, DT_NOCLIP|DT_LEFT ); 
	SelectObject( lpdisp->hDC, hfntOld );
}

// Draw Separator Item
void CHSMenu::DrawItemSpr(LPDRAWITEMSTRUCTPRO lpdisp)
{
	POINT		pt[2];
	RECT&		rect = lpdisp->rect;

	pt[0].x = rect.left + 8;
	pt[1].x = rect.right - 8;
	pt[0].y = pt[1].y = ( rect.top + rect.bottom ) / 2;

	DrawPolyline( lpdisp->hDC, m_clrMenu[CLR_BORDERSPR], pt, 2 );
}

// Draw Item Arrow ( Only Popup Item has )
void CHSMenu::DrawItemArrow(LPDRAWITEMSTRUCTPRO lpdisp)
{
	RECT&		rect = lpdisp->rect;

	int clrArrow;

	if ( lpdisp->uFlags & ITEM_GRAY )
		clrArrow = GetSysColor( COLOR_GRAYTEXT );
	else
		clrArrow = m_clrMenu[GetState( MENU_SELECTED ) ? CLR_ARROWSEL : CLR_TEXTNORMAL];

	DrawArrow( lpdisp->hDC, clrArrow, rect.right-8, (rect.top+rect.bottom)/2, 4 );
}

BOOL CHSMenu::IsEffectItem()
{	
	if ( m_nCurSel == SELNONE ) return FALSE;

	UINT uFlag = m_qMenuItem[m_nCurSel].m_uFlags; 

	return ( ( uFlag & ITEM_POPUP     ) == 0 && 
			 ( uFlag & ITEM_DISABLE   ) == 0 &&
			 ( uFlag & ITEM_GRAY      ) == 0 &&
			 ( uFlag & ITEM_SEPARATOR ) == 0
			);
}

BOOL CHSMenu::PointInMenu(short x, short y)
{
	if ( x>0 && x<m_nMenuWidth && y>0 && y<m_nMenuHeight )
	{
		HRGN hrgnMenu = ::CreateRectRgn( 0, 0, 0, 0 );
		::GetWindowRgn( m_hMenu, hrgnMenu );

		BOOL bInMenu = ::PtInRegion( hrgnMenu, x, y );
		::DeleteObject( hrgnMenu );

		return bInMenu;
	}

	return FALSE;
}

/*void CHSMenu::OnEraseBkgnd(HWND hWnd, HDC hDC)
{

}
*/

void CHSMenu::DrawMenuBkgnd(HWND hWnd, HDC hDC)
{
	HRGN hrgnMenu = ::CreateRectRgn( 0, 0, 0, 0 );
	::GetWindowRgn( hWnd, hrgnMenu );

	DrawRgn( hDC, m_clrMenu[CLR_BORDERMENU], m_clrMenu[CLR_BKGNDMENU], hrgnMenu );

	::DeleteObject( hrgnMenu );
}

void CHSMenu::DrawAnimateMenu( RECT *pRect, HDC hDC, HDC hDCMem, int nType, int nStep, int nTimeDelay)
{
	DrawAnimation( pRect, hDC, hDCMem, nType, nStep, nTimeDelay ); 
/*
	int nWidth		= pRect->right	- pRect->left;
	int nHeight		= pRect->bottom	- pRect->top;
	::BitBlt( hDC, pRect->left, pRect->top, nWidth, nHeight, hDCMem, 0, 0, SRCCOPY );
*/
}

void CHSMenu::OnPaint(HWND hWnd, HDC hDC)
{
//------------------------------------------------------------------------------------------
	if ( !GetState( MENU_SELECTED ) && m_nCurPop == SELNONE && m_nCurSel == SELNONE && m_qMenuItem.size() )
	{
		RECT rect;
		rect.left = rect.top = 0;
		rect.right  = m_nMenuWidth;
		rect.bottom = m_nMenuHeight;

		HDC		hdcMem  = ::CreateCompatibleDC( hDC );
		HBITMAP hbmpMem = ::CreateCompatibleBitmap( hDC, m_nMenuWidth, m_nMenuHeight );
		HGDIOBJ hobjOld = ::SelectObject( hdcMem, hbmpMem );
		::BitBlt( hdcMem, 0, 0, m_nMenuWidth, m_nMenuHeight, hDC, 0, 0, SRCCOPY );

		SetState( MENU_SELECTED, TRUE );
		OnPaint( hWnd, hdcMem );

		DrawAnimateMenu( &rect, hDC, hdcMem, m_nMenuAniType, m_nMenuAniStep, m_nMenuAniDelay );

		::SelectObject( hdcMem, hobjOld );
		::DeleteObject( hbmpMem );
		::DeleteDC( hdcMem );

		return;
	}
//------------------------------------------------------------------------------------------
	SetState( MENU_SELECTED, FALSE );

	DrawMenuBkgnd( hWnd, hDC );

	DRAWITEMSTRUCTPRO	disp;
	DEQUEITEM::const_iterator	it, itend = m_qMenuItem.end();

	disp.hDC = hDC;
	disp.rect.left  = m_nMenuEdge;
	disp.rect.right	= m_nMenuWidth - m_nMenuEdge;
	
	for ( it = m_qMenuItem.begin(); it != itend; it++ )
	{
		disp.ulData		 = it->m_ulData;
		disp.uFlags		 = it->m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = it->m_strName.c_str();	
		disp.rect.top	 = it->m_nTop;
		disp.rect.bottom = it->m_nTop + it->m_nHeight;

		DrawItemAnimation( &disp );
	}

	if ( m_nCurPop != SELNONE )
	{
		SetState( MENU_SELECTED, TRUE );

		HSITEM& MenuItem = m_qMenuItem[m_nCurPop];

		disp.ulData		 = MenuItem.m_ulData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		DrawItemAnimation( &disp );
	}
}

#define ID_HSMENU_RENAME		1000
#define ID_HSMENU_PROPERTIY		1001

void CHSMenu::OnLButtonDown(HWND hWnd, short x, short y)
{
	if ( !PointInMenu( x, y ) ) 
		DestroyAllMenu();
}

void CHSMenu::OnLButtonUp(HWND hWnd, short x, short y)
{
	if ( PointInMenu( x, y ) && IsEffectItem() )
	{
		DestroyAllMenu();
//		::SendMessage( m_hWndParent, WM_COMMAND, MAKEWORD( m_qMenuItem[m_nCurSel].m_uID, 0 ),0 );
		::SendMessage( m_hWndParent, WM_COMMAND, m_qMenuItem[m_nCurSel].m_uID, 0 );
	}
}

void CHSMenu::OnRButtonDown(HWND hWnd, short x, short y)
{
	if ( !PointInMenu( x, y ) ) 
		DestroyAllMenu();
}

void CHSMenu::OnRButtonUp(HWND hWnd, short x, short y)
{
/*	if ( ::IsWindow( m_hMenu ) && PointInMenu( x, y ) )
	{
		CHSMenu* pHSMenu = new CHSMenu;
		pHSMenu->CreateMenu();
		pHSMenu->AppendMenu( "Rename", ID_HSMENU_RENAME );
		pHSMenu->AppendMenu( "Propertiy", ID_HSMENU_PROPERTIY );

		POINT pt;
		pt.x = x;
		pt.y = y;
		::ClientToScreen( m_hMenu, &pt );
		UINT uID = pHSMenu->TrackPopMenu( pt.x, pt.y, m_hWndParent );
		delete pHSMenu;

		switch( uID )
		{	
			case ID_HSMENU_RENAME:
				AfxMessageBox( "Rename" );
				break;

			case ID_HSMENU_PROPERTIY:
				break;

			default:;
		}	

		::ReleaseCapture();
		::SetCapture( hWnd );
	}
*/
}

// this function cycle destroy all the popup item's son
void CHSMenu::DestroyAllPop( void )
{
	if ( m_nCurPop == SELNONE )	return;

	int		 nCurPop;
	CHSMenu	*pDel, *pNext;

	pNext = m_qMenuItem[m_nCurPop].m_pPopupHSMenu;

	::SetActiveWindow( m_hMenu );

	do
	{
		pDel = pNext;
		nCurPop = pDel->m_nCurPop;

		if ( nCurPop != SELNONE )
			 pNext = pDel->m_qMenuItem[nCurPop].m_pPopupHSMenu;
		else pNext = NULL;

		pDel->DestroyMenu();

	} while ( pNext );	

	m_nCurPop = SELNONE;
	SetState( MENU_POPUPED, FALSE );
}

// Deatroy All Menu Window
void CHSMenu::DestroyAllMenu()
{
	CHSMenu	 *pDel = this;
	DestroyAllPop();

	do
	{
		pDel->DestroyMenu();
		pDel = pDel->m_pParent;

	} while ( pDel );
}

inline void CHSMenu::DestroyMenu()
{
	if ( m_hMenu )
	{
		SetState( MENU_SELECTED, FALSE );
		SetState( MENU_POPUPED,  FALSE );
		::DestroyWindow( m_hMenu );
		m_hMenu = NULL;

		for(int i=0;i<m_qMenuItem.size();i++)
			m_qMenuItem[i].m_uFlags &= ~ITEM_CHECKED;
	}
}

static LRESULT CALLBACK lpfnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CHSMenu	*pHSMenu = (CHSMenu *)( GetWindowLong( hWnd, GWL_USERDATA ) );
	static int nn=0;
	switch ( uMsg )
	{
		case WM_MOUSEMOVE:
			pHSMenu->OnMouseMove( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_TIMER:
			pHSMenu->OnTimer( hWnd, wParam );
			break;

		case WM_LBUTTONDOWN:
			pHSMenu->OnLButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_LBUTTONUP:
			pHSMenu->OnLButtonUp( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_RBUTTONDOWN:
			pHSMenu->OnRButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_RBUTTONUP:
			pHSMenu->OnRButtonUp( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

//		case WM_ERASEBKGND:
//			pHSMenu->OnEraseBkgnd( hWnd, (HDC)wParam );
//			break;

		case WM_PAINT:
			{
				PAINTSTRUCT	ps;
				BeginPaint( hWnd, &ps );
				pHSMenu->OnPaint( hWnd, ps.hdc );
				EndPaint( hWnd, &ps );
			}
			break;

//		case WM_MOUSEWHEEL:
//			break;

		case WM_MBUTTONDOWN:
			pHSMenu->OnRButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_KEYDOWN:
			pHSMenu->OnKeyDown( hWnd, (UINT)wParam, HIWORD(lParam), LOWORD(lParam));
			break;

//		case WM_KEYUP:
//			pHSMenu->OnKeyUp( hWnd, (UINT)wParam, HIWORD(lParam), LOWORD(lParam));
//			break;

//		case WM_CHAR:
//			pHSMenu->OnChar( hWnd, (char)wParam );
//			break;

		case WM_SYSKEYDOWN:
			pHSMenu->DestroyAllMenu();
			break;

		case WM_ACTIVATE:
		{
			if ( LOWORD(wParam) == WA_INACTIVE && pHSMenu->m_hWndParent != (HWND)lParam )
			{
				TCHAR	sClassName[50];
				GetClassName( (HWND)lParam, sClassName, sizeof(sClassName)/sizeof(TCHAR) );
				if ( lstrcmp( sClassName, "HSMENU_PRODUCT_BY_HANSONG" ) )
					pHSMenu->DestroyAllMenu();
			}
		}	break;

//		case WM_CAPTURECHANGED:
//			break;

		case WM_CREATE:
			SetWindowLong( hWnd, GWL_USERDATA, (long)( (LPCREATESTRUCT)lParam )->lpCreateParams );
			break;

//		case WM_DESTROY:
//			pHSMenu->OnDestroy( hWnd );
//			break;

		case WM_KILLFOCUS:
			pHSMenu->OnKillFocus( hWnd, (CWnd*)wParam);
			break;
		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}





DWORD CustomMenu(HWND hMenu)
{
	HRGN	hRgn;
	RECT	rect;
	::GetWindowRect( hMenu, &rect );
	int	nMenuWidth  = rect.right  - rect.left;
	int	nMenuHeight = rect.bottom - rect.top	;

//	 hRgn = CreateRoundRectRgn( 0, 0, nMenuWidth, nMenuHeight, 20, 20 );

	int		nRate = 9;
	POINT	ptSel[6];

	ptSel[0].x = nRate;
	ptSel[0].y = 0;
	ptSel[1].x = nMenuWidth;
	ptSel[1].y = 0;
	ptSel[2].x = nMenuWidth;
	ptSel[2].y = nMenuHeight - nRate;
	ptSel[3].x = nMenuWidth - nRate;
	ptSel[3].y = nMenuHeight;
	ptSel[4].x = 0;
	ptSel[4].y = nMenuHeight;
	ptSel[5].x = 0;
	ptSel[5].y = nRate;

	hRgn = ::CreatePolygonRgn( ptSel, 6, WINDING );

	::SetWindowRgn( hMenu, hRgn, true );
	::DeleteObject( hRgn );

//-----------------------------------------------------------------------------------
/*
	// Translucence window

	// Only active on platform windows2000/xp 
	// win98/me do not support.

	// Do not enable that code when the animate stype of class 'CHSMenu' is turn on
	// unless the performance of your CPU is excellent !

	SetWindowLong( hMenu, GWL_EXSTYLE, GetWindowLong( hMenu, GWL_EXSTYLE ) | 0x80000 );
	HINSTANCE instance = LoadLibrary( "User32.dll" );
	if ( instance )
	{
		typedef BOOL ( WINAPI *PSETLAYEREDWINDOWATTRIBUTES )( HWND, COLORREF, BYTE, DWORD );
		PSETLAYEREDWINDOWATTRIBUTES pSetLayeredWindowAttributes = NULL;
		pSetLayeredWindowAttributes = (PSETLAYEREDWINDOWATTRIBUTES)GetProcAddress( instance, "SetLayeredWindowAttributes" );

		if ( pSetLayeredWindowAttributes )
			 pSetLayeredWindowAttributes( hMenu, 0, 231, 2 ); 

		FreeLibrary( instance );
	}
*/
//-----------------------------------------------------------------------------------

	return DWORD(0);
}


DWORD CustomMenuItem( const RECT *prcItem )
{
//	return CreateRoundRectRgn( prcItem->left, prcItem->top, prcItem->right, prcItem->bottom,18,18 );

/*
{
	HRGN hRgnRect, hRgnElliptic;

	hrgnBk = CreateRectRgn( 0, 0, 0, 0 );
	hRgnRect  = CreateRectRgn( prcItem->left, prcItem->top, prcItem->right, prcItem->bottom );
	hRgnElliptic = CreateEllipticRgn( prcItem->left-20, prcItem->top, prcItem->left+30, prcItem->bottom );

	CombineRgn( hrgnBk, hRgnRect, hRgnElliptic, RGN_DIFF );
	hrgnBk = CreateRectRgn( prcItem->left, prcItem->top, prcItem->right, prcItem->bottom );
	hrgnBk = CreateRoundRectRgn( prcItem->left, prcItem->top, prcItem->right, prcItem->bottom, 18, 18 );
}
*/


	int		nRate = 9;
	POINT	ptSel[6];

	ptSel[0].x = prcItem->left + nRate;
	ptSel[0].y = prcItem->top;
	ptSel[1].x = prcItem->right;
	ptSel[1].y = prcItem->top;
	ptSel[2].x = prcItem->right;
	ptSel[2].y = prcItem->bottom - nRate ;
	ptSel[3].x = prcItem->right - nRate;
	ptSel[3].y = prcItem->bottom;
	ptSel[4].x = prcItem->left;
	ptSel[4].y = prcItem->bottom;
	ptSel[5].x = prcItem->left;
	ptSel[5].y = prcItem->top + nRate;

	return (DWORD)CreatePolygonRgn( ptSel, 6, WINDING );
}
