#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CGaussianNoiseDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CGaussianNoiseDlg, CDialogEx)

CGaussianNoiseDlg::CGaussianNoiseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSSIAN_NOISE, pParent)
	, m_pView(nullptr)
{}

CGaussianNoiseDlg::~CGaussianNoiseDlg()
{}

void CGaussianNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GAUSSIAN_STDDEV, m_ctrlStdDevValue);
}

BEGIN_MESSAGE_MAP(CGaussianNoiseDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CGaussianNoiseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Std Deviation: 0-100, representing the strength of the noise
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GAUSSIAN_STDDEV);
	if (pSlider) {
		pSlider->SetRange(0, 100);
		pSlider->SetPos(20); // moderate default
	}

	UpdateLabelAndPreview();

	return TRUE;
}

void CGaussianNoiseDlg::UpdateLabelAndPreview()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GAUSSIAN_STDDEV);
	int stdDevPos = pSlider ? pSlider->GetPos() : 20;

	CString strVal;
	strVal.Format(_T("Std Deviation: %d"), stdDevPos);
	m_ctrlStdDevValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplyGaussianNoise(stdDevPos);
}

void CGaussianNoiseDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
