// GDIPClock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GDIPClock.h"
#include "GDIPClockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockApp

BEGIN_MESSAGE_MAP(CGDIPClockApp, CWinApp)
	//{{AFX_MSG_MAP(CGDIPClockApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockApp construction

CGDIPClockApp::CGDIPClockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGDIPClockApp object

CGDIPClockApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockApp initialization

BOOL CGDIPClockApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	HANDLE hMutex= CreateMutex(NULL,true,_T("TsClock")); //_T()里面的是程序的标题
	if(hMutex && ERROR_ALREADY_EXISTS==GetLastError())
	{
		ReleaseMutex(hMutex);
		return TRUE;
	};

	//获取程序的绝对路径
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL,szPath,MAX_PATH);	//应用程序绝对路径
	m_strAppPath=szPath;
	int index = m_strAppPath.ReverseFind('\\');
	m_strAppPath = m_strAppPath.Left(index+1);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CGDIPClockDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	//关闭gdiplus的环境
	GdiplusShutdown(gdiplusToken);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CGDIPClockApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}
