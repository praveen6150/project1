#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CSigmoidContrastDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CSigmoidContrastDlg, CDialogEx)

CSigmoidContrastDlg::CSigmoidContrastDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGMOID_CONTRAST, pParent)
	, m_pView(nullptr)
{}

CSigmoidContrastDlg::~CSigmoidContrastDlg()
{}

void CSigmoidContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SIGMOID_GAIN, m_ctrlGainValue);
	DDX_Control(pDX, IDC_STATIC_SIGMOID_MIDPOINT, m_ctrlMidpointValue);
	
}

BEGIN_MESSAGE_MAP(CSigmoidContrastDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSigmoidContrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Gain: stored as slider 1-200, representing real gain 0.1 to 20.0
	// (higher gain = steeper, more aggressive S-curve)
	CSliderCtrl* pGain = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIGMOID_GAIN);
	if (pGain) {
		pGain->SetRange(1, 200);
		pGain->SetPos(50); // maps to gain 5.0, a reasonable moderate default
	}

	// Midpoint: 0-255, where the curve's center sits (128 = true middle gray)
	CSliderCtrl* pMid = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIGMOID_MIDPOINT);
	if (pMid) {
		pMid->SetRange(0, 255);
		pMid->SetPos(128);
	}

	UpdateLabelsAndPreview();

	return TRUE;
}

void CSigmoidContrastDlg::UpdateLabelsAndPreview()
{
	CSliderCtrl* pGain = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIGMOID_GAIN);
	CSliderCtrl* pMid = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIGMOID_MIDPOINT);

	int gainPos = pGain ? pGain->GetPos() : 50; 
	int midPos = pMid ? pMid->GetPos() : 128;

	double gainValue = gainPos / 10.0; // convert slider int back to real gain

	CString strVal;
	strVal.Format(_T("Gain (Strength): %.1f"), gainValue);
	m_ctrlGainValue.SetWindowText(strVal);

	strVal.Format(_T("Midpoint: %d"), midPos);
	m_ctrlMidpointValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplySigmoidContrast(gainValue, midPos);
}

void CSigmoidContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelsAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}