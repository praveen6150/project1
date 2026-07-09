#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CLevelsAdjustmentDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CLevelsAdjustmentDlg, CDialogEx)

CLevelsAdjustmentDlg::CLevelsAdjustmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEVELS_ADJUSTMENT, pParent)
	, m_pView(nullptr)
{}

CLevelsAdjustmentDlg::~CLevelsAdjustmentDlg()
{}

void CLevelsAdjustmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEVELS_BLACK, m_ctrlBlackValue);
	DDX_Control(pDX, IDC_STATIC_LEVELS_WHITE, m_ctrlWhiteValue);
	DDX_Control(pDX, IDC_STATIC_LEVELS_GAMMA, m_ctrlGammaValue);
}

BEGIN_MESSAGE_MAP(CLevelsAdjustmentDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CLevelsAdjustmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Black point: 0-254 (must stay below white point)
	CSliderCtrl* pBlack = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_BLACK);
	if (pBlack) {
		pBlack->SetRange(0, 254);
		pBlack->SetPos(0); // no crushing by default
	}

	// White point: 1-255
	CSliderCtrl* pWhite = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_WHITE);
	if (pWhite) {
		pWhite->SetRange(1, 255);
		pWhite->SetPos(255); // no clipping by default
	}

	// Gamma: stored as slider 1-300, representing gamma 0.01 to 3.00
	CSliderCtrl* pGamma = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_GAMMA);
	if (pGamma) {
		pGamma->SetRange(10, 300); // maps to gamma 0.10 - 3.00
		pGamma->SetPos(100);       // maps to gamma 1.00 (neutral)
	}

	UpdateLabelsAndPreview();

	return TRUE;
}

void CLevelsAdjustmentDlg::UpdateLabelsAndPreview()
{
	CSliderCtrl* pBlack = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_BLACK);
	CSliderCtrl* pWhite = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_WHITE);
	CSliderCtrl* pGamma = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LEVELS_GAMMA);

	int blackPos = pBlack ? pBlack->GetPos() : 0;
	int whitePos = pWhite ? pWhite->GetPos() : 255;
	int gammaPos = pGamma ? pGamma->GetPos() : 100;

	// Enforce black < white so we never divide by zero or invert the range
	if (blackPos >= whitePos)
	{
		if (pBlack && GetFocus() == pBlack)
			blackPos = whitePos - 1;
		else if (pWhite)
			whitePos = blackPos + 1;
	}

	double gammaValue = gammaPos / 100.0; // convert slider int back to real gamma

	CString strVal;
	strVal.Format(_T("Black Point: %d"), blackPos);
	m_ctrlBlackValue.SetWindowText(strVal);

	strVal.Format(_T("White Point: %d"), whitePos);
	m_ctrlWhiteValue.SetWindowText(strVal);

	strVal.Format(_T("Gamma: %.2f"), gammaValue);
	m_ctrlGammaValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplyLevelsAdjustment(blackPos, whitePos, gammaValue);
}

void CLevelsAdjustmentDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelsAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
