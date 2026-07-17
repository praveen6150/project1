
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "project1.h"
#include "CTestSliderDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,  // Your new dedicated slot for the progress bar!
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// ── Create the status bar ONCE ──────────────────────────────────────
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// ── Set a fixed width for the progress pane (index 1, the 2nd pane) ───
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 150);

	// ── Create the progress control inside that pane's rectangle ──────────
	CRect rectPane;
	m_wndStatusBar.GetItemRect(1, &rectPane);

	m_wndProgress.Create(WS_CHILD | PBS_SMOOTH,
		rectPane, &m_wndStatusBar, 1);
	// Don't show it yet — only show when an operation starts
	m_wndProgress.ShowWindow(SW_HIDE);

	SetWindowText(_T("Digital Image Processing - Version 1.0.0"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE; // Remove the default "Untitled - " prefix

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::StartProgress(int maxValue)
{
	if (m_wndProgress.GetSafeHwnd() == NULL) return;   // ← make sure this is here too

	CRect rectPane;
	m_wndStatusBar.GetItemRect(1, &rectPane);
	m_wndProgress.MoveWindow(&rectPane);
	m_wndProgress.SetRange(0, maxValue);
	m_wndProgress.SetPos(0);
	m_wndProgress.ShowWindow(SW_SHOW);
}

void CMainFrame::UpdateProgress(int currentValue)
{
	if (m_wndProgress.GetSafeHwnd() == NULL) return;
	m_wndProgress.SetPos(currentValue);
}

void CMainFrame::EndProgress()
{
	if (m_wndProgress.GetSafeHwnd() == NULL) return;
	m_wndProgress.ShowWindow(SW_HIDE);
}

void CMainFrame::RefreshProgressBar()
{
	if (m_wndProgress.GetSafeHwnd() != NULL)
		m_wndProgress.UpdateWindow();
}
