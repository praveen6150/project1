#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CSaltPepperNoiseDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CSaltPepperNoiseDlg, CDialogEx)

CSaltPepperNoiseDlg::CSaltPepperNoiseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SALT_PEPPER_NOISE, pParent)
	, m_pView(nullptr)
{}

CSaltPepperNoiseDlg::~CSaltPepperNoiseDlg()
{}

void CSaltPepperNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SALTPEPPER_DENSITY, m_ctrlDensityValue);
}

BEGIN_MESSAGE_MAP(CSaltPepperNoiseDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSaltPepperNoiseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Density: 0-100, representing 0% to 100% (as a percentage, /1000 later
	// gives finer control since even 5-10% density looks very heavy)
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SALTPEPPER_DENSITY);
	if (pSlider) {
		pSlider->SetRange(0, 100);
		pSlider->SetPos(10); // moderate default: 1.0%
	}

	UpdateLabelAndPreview();

	return TRUE;
}

void CSaltPepperNoiseDlg::UpdateLabelAndPreview()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SALTPEPPER_DENSITY);
	int densityPos = pSlider ? pSlider->GetPos() : 10;

	double densityPercent = densityPos / 10.0; // slider 0-100 -> 0.0%-10.0%

	CString strVal;
	strVal.Format(_T("Density: %.1f%%"), densityPercent);
	m_ctrlDensityValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplySaltPepperNoise(densityPercent / 100.0); // convert to 0.0-0.10 probability
}

void CSaltPepperNoiseDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}