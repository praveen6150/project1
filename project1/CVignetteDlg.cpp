#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CVignetteDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CVignetteDlg, CDialogEx)

CVignetteDlg::CVignetteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIGNETTE, pParent)
	, m_pView(nullptr)
{}

CVignetteDlg::~CVignetteDlg()
{}

void CVignetteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIGNETTE_STRENGTH, m_ctrlStrengthValue);
}

BEGIN_MESSAGE_MAP(CVignetteDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CVignetteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Strength: 0-100, representing 0.0 to 1.0 darkening intensity
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_VIGNETTE_STRENGTH);
	if (pSlider) {
		pSlider->SetRange(0, 100);
		pSlider->SetPos(60); // moderate default
	}

	UpdateLabelAndPreview();

	return TRUE;
}

void CVignetteDlg::UpdateLabelAndPreview()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_VIGNETTE_STRENGTH);
	int strengthPos = pSlider ? pSlider->GetPos() : 60;

	CString strVal;
	strVal.Format(_T("Strength: %d%%"), strengthPos);
	m_ctrlStrengthValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplyVignette(strengthPos / 100.0);
}

void CVignetteDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}