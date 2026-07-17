#include "pch.h"
#include "project1.h"
#include "CColorReplaceDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CColorReplaceDlg, CDialogEx)

CColorReplaceDlg::CColorReplaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLORREPLACE, pParent)
	, m_targetColor(RGB(255, 0, 0))
	, m_replaceColor(RGB(0, 0, 255))
	, m_tolerance(40)
	, m_pTargetView(nullptr)
{}

CColorReplaceDlg::~CColorReplaceDlg()
{}

void CColorReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TOLERANCE, m_sliderTolerance);
}

BEGIN_MESSAGE_MAP(CColorReplaceDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_COLOR, &CColorReplaceDlg::OnBnClickedButtonReplaceColor)
END_MESSAGE_MAP()

void CColorReplaceDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CColorReplaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderTolerance.SetRange(0, 150, TRUE);
	m_sliderTolerance.SetPos(m_tolerance);
	m_sliderTolerance.SetTicFreq(50);

	CString text;
	text.Format(_T("Tolerance: %d"), m_tolerance);
	SetDlgItemText(IDC_STATIC_TOLERANCE_TEXT, text);

	SetDlgItemText(IDC_STATIC_HOVER_COLOR, _T("Hover: -"));
	UpdateTargetLabel();
	UpdateReplaceLabel();

	// Start live hover/click tracking on the canvas for the entire life of this dialog
	if (m_pTargetView)
		m_pTargetView->StartColorPickerTracking(this);

	return TRUE;
}

void CColorReplaceDlg::UpdateTargetLabel()
{
	CString text;
	text.Format(_T("Target: R%d G%d B%d"), GetRValue(m_targetColor), GetGValue(m_targetColor), GetBValue(m_targetColor));
	SetDlgItemText(IDC_STATIC_TARGET_COLOR, text);
}

void CColorReplaceDlg::UpdateReplaceLabel()
{
	CString text;
	text.Format(_T("Replace: R%d G%d B%d"), GetRValue(m_replaceColor), GetGValue(m_replaceColor), GetBValue(m_replaceColor));
	SetDlgItemText(IDC_BUTTON_REPLACE_COLOR, text);
}

void CColorReplaceDlg::OnBnClickedButtonReplaceColor()
{
	CColorDialog dlg(m_replaceColor, CC_FULLOPEN, this);
	if (dlg.DoModal() == IDOK)
	{
		m_replaceColor = dlg.GetColor();
		UpdateReplaceLabel();
		UpdateLive();
	}
}

void CColorReplaceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderTolerance)
	{
		m_tolerance = m_sliderTolerance.GetPos();

		CString text;
		text.Format(_T("Tolerance: %d"), m_tolerance);
		SetDlgItemText(IDC_STATIC_TOLERANCE_TEXT, text);

		UpdateLive();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorReplaceDlg::UpdateLive()
{
	if (m_pTargetView)
		m_pTargetView->ApplyLiveColorReplace(m_targetColor, m_replaceColor, m_tolerance);
}

void CColorReplaceDlg::OnHoverColorUpdate(COLORREF color)
{
	CString text;
	text.Format(_T("Hover: R%d G%d B%d"), GetRValue(color), GetGValue(color), GetBValue(color));
	SetDlgItemText(IDC_STATIC_HOVER_COLOR, text);
}

void CColorReplaceDlg::OnEyedropperColorPicked(COLORREF color)
{
	m_targetColor = color;
	UpdateTargetLabel();
	UpdateLive();
}

void CColorReplaceDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CColorReplaceDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}

void CColorReplaceDlg::PostNcDestroy()
{
	if (m_pTargetView)
		m_pTargetView->StopColorPickerTracking();

	CDialogEx::PostNcDestroy();
}
