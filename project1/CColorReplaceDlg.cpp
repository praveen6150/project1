#include "pch.h"
#include "project1.h"
#include "CColorReplaceDlg.h"
#include "project1View.h"
#include "afxdialogex.h"
#include "afxcolorbutton.h"

IMPLEMENT_DYNAMIC(CColorReplaceDlg, CDialogEx)

CColorReplaceDlg::CColorReplaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLORREPLACE, pParent)
	, m_targetColor(RGB(255, 0, 0))     // default target: red
	, m_replaceColor(RGB(0, 0, 255))    // default replace: blue
	, m_tolerance(40)
	, m_pTargetView(nullptr)            // FIXED: Initialize the view pointer safely to null
{
}

CColorReplaceDlg::~CColorReplaceDlg()
{
}

void CColorReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TOLERANCE, m_sliderTolerance);
}

BEGIN_MESSAGE_MAP(CColorReplaceDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TARGET_COLOR, &CColorReplaceDlg::OnBnClickedButtonTargetColor)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_COLOR, &CColorReplaceDlg::OnBnClickedButtonReplaceColor)
	ON_BN_CLICKED(IDC_BUTTON_EYEDROPPER_TARGET, &CColorReplaceDlg::OnBnClickedButtonEyedropperTarget)
END_MESSAGE_MAP() // FIXED: Placed cleanly on its own macro line

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

	UpdateButtonColors();

	return TRUE;
}

void CColorReplaceDlg::UpdateButtonColors()
{
	CString targetText, replaceText;
	targetText.Format(_T("Target: R%d G%d B%d"), GetRValue(m_targetColor), GetGValue(m_targetColor), GetBValue(m_targetColor));
	replaceText.Format(_T("Replace: R%d G%d B%d"), GetRValue(m_replaceColor), GetGValue(m_replaceColor), GetBValue(m_replaceColor));

	SetDlgItemText(IDC_BUTTON_TARGET_COLOR, targetText);
	SetDlgItemText(IDC_BUTTON_REPLACE_COLOR, replaceText);
}

void CColorReplaceDlg::OnBnClickedButtonTargetColor()
{
	CColorDialog dlg(m_targetColor, CC_FULLOPEN, this);
	if (dlg.DoModal() == IDOK)
	{
		m_targetColor = dlg.GetColor();
		UpdateButtonColors();
		UpdateLive();
	}
}

void CColorReplaceDlg::OnBnClickedButtonReplaceColor()
{
	CColorDialog dlg(m_replaceColor, CC_FULLOPEN, this);
	if (dlg.DoModal() == IDOK)
	{
		m_replaceColor = dlg.GetColor();
		UpdateButtonColors();
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
	if (m_pTargetView != nullptr)
		m_pTargetView->ApplyLiveColorReplace(m_targetColor, m_replaceColor, m_tolerance);
}

void CColorReplaceDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CColorReplaceDlg::OnCancel()
{
	if (m_pTargetView != nullptr)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}

void CColorReplaceDlg::OnBnClickedButtonEyedropperTarget()
{
	if (m_pTargetView != nullptr)
	{
		ShowWindow(SW_HIDE);

		CWnd* pMainWnd = AfxGetMainWnd();
		if (pMainWnd)
		{
			pMainWnd->SetForegroundWindow();
			pMainWnd->BringWindowToTop();
		}

		CFrameWnd* pParentFrame = m_pTargetView->GetParentFrame();
		if (pParentFrame)
			pParentFrame->ActivateFrame();

		m_pTargetView->StartEyedropperMode(this);
	}
}

void CColorReplaceDlg::OnEyedropperColorPicked(COLORREF pickedColor)
{
	m_targetColor = pickedColor;

	ShowWindow(SW_SHOW);
	SetForegroundWindow();

	UpdateButtonColors();
	UpdateLive();
}

void CColorReplaceDlg::PostNcDestroy()
{
	if (m_pTargetView)
		m_pTargetView->CancelEyedropperMode();

	CDialogEx::PostNcDestroy();
}
