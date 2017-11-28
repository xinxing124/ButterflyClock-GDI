// GDIPClockDlg.h : header file
//

#if !defined(AFX_GDIPCLOCKDLG_H__B8680715_567C_499C_889E_F2CE8B813863__INCLUDED_)
#define AFX_GDIPCLOCKDLG_H__B8680715_567C_499C_889E_F2CE8B813863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HSMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CGDIPClockDlg dialog

class CGDIPClockDlg : public CDialog
{
// Construction
public:
	BOOL ImageFromIDResource(UINT nID,LPCTSTR sTR,Image * &pImg);
	BOOL UpdateClockDisplay(int Transparent=100);
	CGDIPClockDlg(CWnd* pParent = NULL);	// standard constructor
protected:
	BLENDFUNCTION m_Blend;
	HDC m_hdcMemory;
	Image *m_pImageClock;
	Image *m_pImageClock1;
	Image *m_pImageHHour;
	Image *m_pImageHMinu;
	Image *m_pImageHSec;
	Image *m_pImageNum;
	int m_BakWidth , m_BakHeight ;
	int m_HourWidth, m_HourHeight;
	int m_MinuWidth , m_MinuHeight;
	int m_SecWidth  , m_SecHeight ;
	CHSMenu		m_menu;			//�Ҽ��˵�
	CHSMenu		m_menuTransparency;	//͸���Ȳ˵�
	BOOL		m_bTopMost;		//�Ƿ񴰿���ǰ
	BOOL		m_bAutoRun;		//�Ƿ񿪻�����
	int			m_nTransparency;	//͸����
	BOOL		m_bMagnet;			//���ڴ���
	CPoint		m_ptClient;		//��갴��ʱ����ڴ��ڵ�λ��
	CPoint		m_ptScreen;		//��갴��ʱ�������Ļ��λ��
	SIZE		m_sizeScreen;	//��Ļ�Ĵ�С
	int			m_magnetScope;	//���Է�Χ
	CPoint		m_ptWndInit;		//�������к󴰿ڵĳ�ʼλ��
	BOOL		m_bWndTransparent;	//�Ƿ���괩͸
	HINSTANCE hFuncInst ;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;
	
// Dialog Data
	//{{AFX_DATA(CGDIPClockDlg)
	enum { IDD = IDD_GDIPCLOCK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIPClockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGDIPClockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnExit();
protected:
	void	InitMenu		();
	void	ReadSettingFile	( BOOL	bRead=TRUE );
	void	ShowTrayIcon	( BOOL	bShow=TRUE );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTopMost();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnAutoRun();
	afx_msg void OnTransparency(UINT id);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnMagnet();
	afx_msg void OnWndTransparent();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIPCLOCKDLG_H__B8680715_567C_499C_889E_F2CE8B813863__INCLUDED_)
