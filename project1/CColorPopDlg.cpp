#include "pch.h"
#include "project1.h"
#include "CColorPopDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CColorPopDlg, CDialogEx)

CColorPopDlg::CColorPopDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLORPOP, pParent)
	, m_targetColor(RGB(255, 0, 0))
	, m_hueTolerance(30)
	, m_pTargetView(nullptr)
{
}

CColorPopDlg::~CColorPopDlg()
{
}

void CColorPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HUE_TOLERANCE, m_sliderTolerance);
}

BEGIN_MESSAGE_MAP(CColorPopDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CColorPopDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CColorPopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderTolerance.SetRange(0, 180, TRUE);
	m_sliderTolerance.SetPos(m_hueTolerance);
	m_sliderTolerance.SetTicFreq(45);

	CString text;
	text.Format(_T("Hue Tolerance: %d"), m_hueTolerance);
	SetDlgItemText(IDC_STATIC_HUE_TOLERANCE_TEXT, text);

	SetDlgItemText(IDC_STATIC_HOVER_COLOR_POP, _T("Hover: -"));
	UpdateTargetLabel();

	if (m_pTargetView)
		m_pTargetView->StartColorPickerTracking2(this);

	return TRUE;
}

void CColorPopDlg::UpdateTargetLabel()
{
	CString text;
	text.Format(_T("Target: R%d G%d B%d"), GetRValue(m_targetColor), GetGValue(m_targetColor), GetBValue(m_targetColor));
	SetDlgItemText(IDC_STATIC_TARGET_COLOR_POP, text);
}

void CColorPopDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderTolerance)
	{
		m_hueTolerance = m_sliderTolerance.GetPos();

		CString text;
		text.Format(_T("Hue Tolerance: %d"), m_hueTolerance);
		SetDlgItemText(IDC_STATIC_HUE_TOLERANCE_TEXT, text);

		UpdateLive();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorPopDlg::UpdateLive()
{
	if (m_pTargetView)
		m_pTargetView->ApplyLiveColorPop(m_targetColor, m_hueTolerance);
}

void CColorPopDlg::OnHoverColorUpdate(COLORREF color)
{
	CString text;
	text.Format(_T("Hover: R%d G%d B%d"), GetRValue(color), GetGValue(color), GetBValue(color));
	SetDlgItemText(IDC_STATIC_HOVER_COLOR_POP, text);
}

void CColorPopDlg::OnEyedropperColorPicked(COLORREF color)
{
	m_targetColor = color;
	UpdateTargetLabel();
	UpdateLive();
}

void CColorPopDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CColorPopDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}

void CColorPopDlg::PostNcDestroy()
{
	if (m_pTargetView)
		m_pTargetView->StopColorPickerTracking2();

	CDialogEx::PostNcDestroy();
}
