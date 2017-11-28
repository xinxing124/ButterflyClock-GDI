// GDIPClockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GDIPClock.h"
#include "GDIPClockDlg.h"
#include "HSGraphics.h"
#include "SetDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TRAYICON	WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockDlg dialog

CGDIPClockDlg::CGDIPClockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGDIPClockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGDIPClockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_magnetScope=50;
}

void CGDIPClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGDIPClockDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGDIPClockDlg, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CGDIPClockDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_EXIT, OnExit)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_TOPMOST, &CGDIPClockDlg::OnTopMost)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_AUTO_RUN, &CGDIPClockDlg::OnAutoRun)
	ON_COMMAND_RANGE(IDM_TRANS1,IDM_TRANS10,OnTransparency)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(IDM_MAGNET, &CGDIPClockDlg::OnMagnet)
	ON_COMMAND(IDM_WND_TRANSPARENT, &CGDIPClockDlg::OnWndTransparent)
	ON_MESSAGE(WM_TRAYICON,OnTrayIcon)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockDlg message handlers

BOOL CGDIPClockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//获取屏幕的大小
	m_sizeScreen.cx=::GetSystemMetrics(SM_CXFULLSCREEN)+10;
	m_sizeScreen.cy=::GetSystemMetrics(SM_CYFULLSCREEN)+30;

	//初始菜单
	InitMenu();
	ReadSettingFile();	//读取配置文件
	//是否鼠标穿透
	if(m_bWndTransparent)	ModifyStyleEx(0,WS_EX_TRANSPARENT);
	else	ModifyStyleEx(WS_EX_TRANSPARENT,0);
	//设置窗体是否最前
	if(m_bTopMost)
		SetWindowPos(&wndTopMost,m_ptWndInit.x,m_ptWndInit.y,0,0,SWP_NOREDRAW | SWP_NOSIZE);
	else
		SetWindowPos(&wndBottom,m_ptWndInit.x,m_ptWndInit.y,0,0,SWP_NOREDRAW | SWP_NOSIZE);
	//启动后立刻更新窗口样式为透明窗体
	UpdateClockDisplay(m_nTransparency*10);
	SetTimer(1,500,NULL);
	//去除任务栏窗口对应按钮
	ModifyStyleEx (WS_EX_APPWINDOW,WS_EX_TOOLWINDOW );
	ShowTrayIcon();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGDIPClockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGDIPClockDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGDIPClockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CGDIPClockDlg::OnDestroy() 
{
	// TODO: Add your message handler code here and/or call default
	ShowTrayIcon(FALSE);
	delete m_pImageClock;
	delete m_pImageHHour;
	delete m_pImageHMinu;
	delete m_pImageHSec;
	FreeLibrary(hFuncInst); 
	CDialog::OnDestroy();
	
}


int CGDIPClockDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    hFuncInst = LoadLibrary("User32.DLL"); 
	BOOL bRet=FALSE;
	if(hFuncInst) 
		UpdateLayeredWindow=(MYFUNC)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
	else
	{
		AfxMessageBox("User32.dll ERROR!");
		exit(0);
	}
	//初始化gdiplus的环境
	// Initialize GDI+.
	m_Blend.BlendOp=0; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags=0; //nothingelseisspecial...
	m_Blend.AlphaFormat=1; //...
	m_Blend.SourceConstantAlpha=255;//AC_SRC_ALPHA

    ImageFromIDResource(IDR_PNGNUM,"PNG",m_pImageNum);
    ImageFromIDResource(IDR_PNGBAK1,"PNG",m_pImageClock1);
    ImageFromIDResource(IDR_PNGBAK,"PNG",m_pImageClock);
    ImageFromIDResource(IDR_PNGHOUR,"PNG",m_pImageHHour);
    ImageFromIDResource(IDR_PNGMIN,"PNG",m_pImageHMinu);
    ImageFromIDResource(IDR_PNGSEC,"PNG",m_pImageHSec);
	m_BakWidth  =m_pImageClock->GetWidth();
	m_BakHeight =m_pImageClock->GetHeight();
	m_HourWidth =m_pImageHHour->GetWidth();
	m_HourHeight=m_pImageHHour->GetHeight();
	m_MinuWidth =m_pImageHMinu->GetWidth();
	m_MinuHeight=m_pImageHMinu->GetHeight();
	m_SecWidth  =m_pImageHSec->GetWidth();
	m_SecHeight =m_pImageHSec->GetHeight();
	::SetWindowPos(m_hWnd, HWND_TOPMOST,0,0,m_BakWidth,m_BakHeight,SWP_NOSIZE|SWP_NOMOVE);   
	return 0;
}

BOOL CGDIPClockDlg::UpdateClockDisplay(int Transparent)
{
	HDC hdcTemp=GetDC()->m_hDC;
	m_hdcMemory=CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap=CreateCompatibleBitmap(hdcTemp,m_BakWidth,m_BakHeight);
	SelectObject(m_hdcMemory,hBitMap);
	if(Transparent<0||Transparent>100)	Transparent=100;

	m_Blend.SourceConstantAlpha=int(Transparent*2.55);//1~255
	HDC hdcScreen=::GetDC (m_hWnd);
	RECT rct;
	GetWindowRect(&rct);
	POINT ptWinPos={rct.left,rct.top};
	Graphics graph(m_hdcMemory);

	Point points[] = { Point(0, 0), 
		               Point(m_BakWidth, 0), 
					   Point(0, m_BakHeight)
					};
	static bool bFly=false;
	bFly?graph.DrawImage(m_pImageClock, points, 3): graph.DrawImage(m_pImageClock1, points, 3); 
	bFly=!bFly;
	
	int OxyX=140;//m_BakWidth/2+8;
	int OxyY=90;//m_BakHeight/2+10;
	SYSTEMTIME SystemTime;   // address of system time structure
	GetLocalTime(&SystemTime);

	Matrix matrixH(1,0,0,1,OxyX,OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
	matrixH.Rotate(SystemTime.wHour*30+SystemTime.wMinute/2.0-180); // 时针旋转的角度度
	Point pointsH[] = { Point(0, 0),Point(m_HourWidth, 0),Point(0, m_HourHeight)};
	matrixH.Translate(-m_HourWidth/2,-m_HourHeight/6);
	matrixH.TransformPoints( pointsH, 3); // 用该矩阵转换points
	graph.DrawImage (m_pImageHHour,pointsH, 3);

	Matrix matrixM(1,0,0,1,OxyX,OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
	matrixM.Rotate(SystemTime.wMinute*6-180); // 分针旋转的角度度
	Point pointsM[] = { Point(0, 0),Point(m_MinuWidth, 0),Point(0, m_MinuHeight)};
	matrixM.Translate(-m_MinuWidth/2,-m_MinuHeight/6);
	matrixM.TransformPoints( pointsM, 3); // 用该矩阵转换pointsM
	graph.DrawImage (m_pImageHMinu,pointsM, 3);
    
	Matrix matrix(1,0,0,1,OxyX,OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
	matrix.Rotate(SystemTime.wSecond*6-180); // 秒针旋转的角度度
	Point pointsS[] = { Point(0, 0),Point( m_SecWidth,0),Point(0,m_SecHeight )};
	matrix.Translate(-m_SecWidth/2,-m_SecHeight/7);
	matrix.TransformPoints( pointsS, 3); // 用该矩阵转换pointsS
	graph.DrawImage (m_pImageHSec,pointsS, 3);
//H
	int xOff = 10;
	graph.DrawImage(m_pImageNum,xOff+0, 0, 14*(SystemTime.wHour/10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
	graph.DrawImage(m_pImageNum,xOff+20,0, 14*(SystemTime.wHour%10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
//:
	graph.DrawImage(m_pImageNum,xOff+20*2,0, 140, 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
//M
	graph.DrawImage(m_pImageNum,xOff+20*3, 0, 14*(SystemTime.wMinute/10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
	graph.DrawImage(m_pImageNum,xOff+20*4,0, 14*(SystemTime.wMinute%10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
//:
	graph.DrawImage(m_pImageNum,xOff+20*5,0, 140, 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
//S
	graph.DrawImage(m_pImageNum,xOff+20*6, 0, 14*(SystemTime.wSecond/10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置
	graph.DrawImage(m_pImageNum,xOff+20*7,0, 14*(SystemTime.wSecond%10), 0,14,23,UnitPixel); //该函数从m_pImageClock中剪切指定rect中的像素draw到指定位置

	SIZE sizeWindow={m_BakWidth,m_BakHeight};
	POINT ptSrc={0,0};
	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);

	BOOL bRet=FALSE;
	bRet= UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,
				&sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2);
	graph.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(m_hWnd,hdcScreen);
	hdcScreen=NULL;
	::ReleaseDC(m_hWnd,hdcTemp);
	hdcTemp=NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory=NULL;
	return bRet;
}

void CGDIPClockDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CTime curTime=CTime::GetCurrentTime();
	UpdateClockDisplay(m_nTransparency*10);
	CDialog::OnTimer(nIDEvent);
}

void CGDIPClockDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnRButtonUp(nFlags, point);
}

void CGDIPClockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//禁止显示移动矩形窗体框
//	::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,TRUE,NULL,0);
	//非标题栏移动整个窗口
//	SendMessage(WM_SYSCOMMAND,0xF012,0);
//	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	SetCapture();
	m_ptClient.x = point.x;
	m_ptClient.y = point.y;
	m_ptScreen=m_ptClient;
	ClientToScreen(&m_ptScreen);
	CDialog::OnLButtonDown(nFlags, point);
}




BOOL CGDIPClockDlg::ImageFromIDResource(UINT nID, LPCTSTR sTR,Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	
	// load from stream
	pImg=Gdiplus::Image::FromStream(pstm);

	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

}

void CGDIPClockDlg::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1){
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}
/*
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
*/
	if( m_bTopMost )	m_menu.CheckMenuItem(IDM_TOPMOST);
	if( m_bAutoRun )	m_menu.CheckMenuItem(IDM_AUTO_RUN);
	if( m_bMagnet )		m_menu.CheckMenuItem(IDM_MAGNET);
	if( m_bWndTransparent )		m_menu.CheckMenuItem(IDM_WND_TRANSPARENT);
	m_menuTransparency.CheckMenuItem( IDM_TRANS1+m_nTransparency-1);
	m_menu.TrackPopMenu(point.x,point.y,this->m_hWnd);
}

void CGDIPClockDlg::OnExit() 
{
	//保存窗口的位置
	CRect	rect;
	GetWindowRect(&rect);
	m_ptWndInit.x = rect.left;
	m_ptWndInit.y = rect.top;
	ReadSettingFile(FALSE);

	PostMessage(WM_CLOSE);
	SendMessage(WM_CLOSE);
}

void CGDIPClockDlg::InitMenu()
{
	m_menu.CreateMenu();
	CHSMenu::SetCustomMenu( CustomMenu );
	CHSMenu::SetCustomMenuItem( CustomMenuItem );
//	CHSMenu::SetMenuAnimate(ANIMATE_UNFOLDSLIDE_LTTRB,12,100);
	CHSMenu::SetMenuAnimate(ANIMATE_SHUTTER_TTB	,12,100);

	m_menu.SetColor( CLR_BKGNDMENU,	RGB(0,0,0) );
	m_menu.SetColor( CLR_BKGNDSEL,	RGB(0,0,0) );
	m_menu.SetColor( CLR_BORDERSEL,	RGB(47,60,174) );
	m_menu.SetColor( CLR_ARROWSEL,	RGB(0,126,0) );
	m_menu.SetColor( CLR_TEXTNORMAL,RGB(0,128,0) );
	m_menu.SetColor( CLR_TEXTSEL,	RGB(0,128,0) );
	m_menu.SetColor( CLR_BORDERMENU,RGB(0,128,0) );
	
	m_menuTransparency.CreateMenu();
	CString strTitle[] = {_T("10%"),_T("20%"),_T("30%"),_T("40%"),_T("50%"),
		_T("60%"),_T("70%"),_T("80%"),_T("90%"),_T("100%")};
	for(int i=0;i<10;i++)
	{
		m_menuTransparency.AppendMenu(strTitle[i],IDM_TRANS1+i);
	}
	m_menu.AppendMenu(_T("最前显示"),IDM_TOPMOST);
	m_menu.AppendMenu(_T("开机启动"),IDM_AUTO_RUN);
	m_menu.AppendMenu(_T("窗口磁性"),IDM_MAGNET);
	m_menu.AppendMenu(_T("鼠标穿透"),IDM_WND_TRANSPARENT);
	m_menu.AppendPopup(_T("透明度"),&m_menuTransparency);
	m_menu.AppendMenu(_T("退出"),IDM_EXIT);
}
void CGDIPClockDlg::OnTopMost()
{
	m_bTopMost = !m_bTopMost;
	CWnd	*pWnd;
	if(m_bTopMost)
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
	else
		SetWindowPos(&wndBottom,0,0,0,0,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
	ReadSettingFile(FALSE);
}

void CGDIPClockDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CGDIPClockDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(GetCapture() == this)
	{
		ClientToScreen(&point);
		if(m_bMagnet)	//窗口磁性
		{
			CRect	rect;
			GetWindowRect(&rect);
			if(abs(rect.left) < m_magnetScope	//窗口在桌面左边磁性的范围内
				&& point.x-m_ptScreen.x<m_magnetScope)	//鼠标向右移动的距离不超过磁性范围
			{
				point.x=m_ptClient.x;
				m_ptScreen.x=point.x;	//被桌面吸住后，设置鼠标开始拖动的屏幕坐标为当前坐标
			}
			else if(abs(rect.right -m_sizeScreen.cx) < m_magnetScope//窗口在桌面右边磁性的范围内
				&& m_ptScreen.x-point.x < m_magnetScope)//鼠标向左移动的距离不超过磁性范围
			{
				point.x=m_sizeScreen.cx-rect.Width()+m_ptClient.x;
				m_ptScreen.x=point.x;	//被桌面吸住后，设置鼠标开始拖动的屏幕坐标为当前坐标
			}

			if(abs(rect.top) < m_magnetScope	//窗口在桌面上边磁性的范围内
				&& point.y-m_ptScreen.y<m_magnetScope)	//鼠标向下移动的距离不超过磁性范围
			{
				point.y=m_ptClient.y;
				m_ptScreen.y=point.y;	//被桌面吸住后，设置鼠标开始拖动的屏幕坐标为当前坐标
			}
			else if(abs(rect.bottom -m_sizeScreen.cy) < m_magnetScope//窗口在桌面下边磁性的范围内
				&& m_ptScreen.y-point.y < m_magnetScope)//鼠标向上移动的距离不超过磁性范围
			{
				point.y=m_sizeScreen.cy-rect.Height()+m_ptClient.y;
				m_ptScreen.y=point.y;	//被桌面吸住后，设置鼠标开始拖动的屏幕坐标为当前坐标
			}
		}
		SetWindowPos(NULL,point.x-m_ptClient.x,point.y-m_ptClient.y,0,0,SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CGDIPClockDlg::ReadSettingFile(BOOL	bRead)
{
	CGDIPClockApp	*pApp=( CGDIPClockApp* )AfxGetApp();
	CString	strPath = pApp->m_strAppPath+_T("setting.ini");
	if(bRead)		//读取
	{
		m_bTopMost = ::GetPrivateProfileInt( _T("SettingInfo"), _T("TopMost"), 1, strPath );
		m_bAutoRun = ::GetPrivateProfileInt( _T("SettingInfo"), _T("AutoRun"), 0, strPath );
		m_nTransparency = ::GetPrivateProfileInt( _T("SettingInfo"), _T("Transparency"), 10, strPath );
		m_bMagnet = ::GetPrivateProfileInt( _T("SettingInfo"), _T("Magnet"), 10, strPath );
		m_ptWndInit.x = ::GetPrivateProfileInt( _T("SettingInfo"), _T("CX_WndInit"), m_sizeScreen.cx-m_BakWidth, strPath );
		m_ptWndInit.y = ::GetPrivateProfileInt( _T("SettingInfo"), _T("CY_WndInit"), 0, strPath );
		m_bWndTransparent = ::GetPrivateProfileInt( _T("SettingInfo"), _T("WndTransparent"), 1, strPath );
	}
	else	//写入
	{
		CString strValue;
		strValue.Format( _T("%d"), m_bTopMost );
		::WritePrivateProfileString( _T("SettingInfo"), _T("TopMost"), strValue,strPath );
		strValue.Format( _T("%d"), m_bAutoRun );
		::WritePrivateProfileString( _T("SettingInfo"), _T("AutoRun"), strValue,strPath );
		strValue.Format( _T("%d"), m_nTransparency );
		::WritePrivateProfileString( _T("SettingInfo"), _T("Transparency"), strValue,strPath );
		strValue.Format( _T("%d"), m_bMagnet );
		::WritePrivateProfileString( _T("SettingInfo"), _T("Magnet"), strValue,strPath );
		strValue.Format( _T("%d"), m_ptWndInit.x );
		::WritePrivateProfileString( _T("SettingInfo"), _T("CX_WndInit"), strValue,strPath );
		strValue.Format( _T("%d"), m_ptWndInit.y );
		::WritePrivateProfileString( _T("SettingInfo"), _T("CY_WndInit"), strValue,strPath );
		strValue.Format( _T("%d"), m_bWndTransparent );
		::WritePrivateProfileString( _T("SettingInfo"), _T("WndTransparent"), strValue,strPath );
		::WritePrivateProfileString( _T("SettingInfo"), _T("Author"), _T("盛青松"),strPath );
	}
}
void CGDIPClockDlg::OnAutoRun()
{
	//获取程序的绝对路径
	char szPath[MAX_PATH]={0};
	if(!m_bAutoRun)
		GetModuleFileName(NULL,szPath,MAX_PATH);	//应用程序绝对路径
	HKEY	hKey;
	long	lnRes = RegOpenKeyEx(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0L,KEY_WRITE,&hKey);
	if( ERROR_SUCCESS == lnRes )
	{
		lnRes = RegSetValueEx(hKey,_T("TsClock"),0,REG_SZ,(CONST BYTE*)szPath,sizeof(szPath));
		if( ERROR_SUCCESS == lnRes )
		{
			m_bAutoRun = !m_bAutoRun;
			RegCloseKey(hKey);
		}
	}
	ReadSettingFile(FALSE);
}

void CGDIPClockDlg::OnTransparency(UINT id)
{
	m_nTransparency = id-IDM_TRANS1+1;
	ReadSettingFile(FALSE);
}
void CGDIPClockDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CGDIPClockDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

BOOL CGDIPClockDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CGDIPClockDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
}

void CGDIPClockDlg::OnMagnet()
{
	m_bMagnet = !m_bMagnet;

}

void CGDIPClockDlg::OnWndTransparent()
{
	m_bWndTransparent = !m_bWndTransparent;
	if(m_bWndTransparent)	ModifyStyleEx(0,WS_EX_TRANSPARENT);
	else	ModifyStyleEx(WS_EX_TRANSPARENT,0);
}

void	CGDIPClockDlg::ShowTrayIcon(BOOL	bShow)
{
	if(bShow)
	{
		//显示托盘
		NOTIFYICONDATA nid;    //NOTIFYICONDATA结构包含了系统用来处理托盘图标的信息，
		//它包括选择的图标、回调消息、提示消息和图标对应的窗口等内容。
		nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);         //以字节为单位的这个结构的大小
		nid.hWnd = this->m_hWnd;          //接收托盘图标通知消息的窗口句柄
		nid.uID = IDI_TRAY;          //应用程序定义的该图标的ID号
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;         //设置该图标的属性
		nid.uCallbackMessage=WM_TRAYICON;             //应用程序定义的消息ID号，此消息传递给hWnd  
		nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_TRAY));   //图标的句柄
		char	szTip[63]=_T("蝴蝶时钟\n\nTsing松工作室\nQQ:761092547\nE-mail:sqs_2010@163.com");
		strcpy(nid.szTip,szTip);    //鼠标停留在图标上显示的提示信息 
		nid.uTimeout=1000;
		nid.dwInfoFlags=0x00000004;	//NIIF_USER
		Shell_NotifyIcon(NIM_ADD,&nid);    //在托盘区添加图标de函数 ,函数说明见文章附录
	}
	else
	{
		//删除托盘图标
		NOTIFYICONDATA notifyData;
		notifyData.cbSize=sizeof(notifyData);
		notifyData.hWnd=m_hWnd;
		notifyData.uID=IDI_TRAY;
		notifyData.uFlags=0;
		Shell_NotifyIcon(NIM_DELETE,&notifyData);
	}
}
LRESULT CGDIPClockDlg::OnTrayIcon(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case WM_LBUTTONDOWN:
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		break;
	case WM_RBUTTONUP:
		{
		CPoint point;
		::GetCursorPos(&point);
		point.x -=5;
		point.y -=5;
		CMenu menu,*pMenuItem;
		menu.LoadMenu(IDR_MENU1);
		pMenuItem=menu.GetSubMenu(0);

		if( m_bTopMost )	pMenuItem->CheckMenuItem(IDM_TOPMOST,MF_BYCOMMAND | MF_CHECKED);
		if( m_bAutoRun )	pMenuItem->CheckMenuItem(IDM_AUTO_RUN,MF_BYCOMMAND | MF_CHECKED);
		if( m_bMagnet )		pMenuItem->CheckMenuItem(IDM_MAGNET,MF_BYCOMMAND | MF_CHECKED);
		if( m_bWndTransparent )		pMenuItem->CheckMenuItem(IDM_WND_TRANSPARENT,MF_BYCOMMAND | MF_CHECKED);
		pMenuItem->CheckMenuItem(IDM_TRANS1+m_nTransparency-1,MF_BYCOMMAND | MF_CHECKED);
	
		SetForegroundWindow();
		::TrackPopupMenu(pMenuItem->m_hMenu,TPM_BOTTOMALIGN ,point.x,point.y,0,m_hWnd,NULL);
		break;
		}
	default:
		break;
	}
	return 0; 
}
void CGDIPClockDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
}
