// GDIPClock.h : main header file for the GDIPCLOCK application
//

#if !defined(AFX_GDIPCLOCK_H__E0616D36_8133_4A61_9C83_7C6627B40B86__INCLUDED_)
#define AFX_GDIPCLOCK_H__E0616D36_8133_4A61_9C83_7C6627B40B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockApp:
// See GDIPClock.cpp for the implementation of this class
//

class CGDIPClockApp : public CWinApp
{
public:
	CGDIPClockApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIPClockApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	CString		m_strAppPath;
// Implementation

	//{{AFX_MSG(CGDIPClockApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIPCLOCK_H__E0616D36_8133_4A61_9C83_7C6627B40B86__INCLUDED_)
