#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CSepiaToneDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CSepiaToneDlg, CDialogEx)

CSepiaToneDlg::CSepiaToneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEPIA_TONE, pParent)
	, m_pView(nullptr)
{}

CSepiaToneDlg::~CSepiaToneDlg()
{}

void CSepiaToneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SEPIA_INTENSITY, m_ctrlIntensityValue);
}

BEGIN_MESSAGE_MAP(CSepiaToneDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSepiaToneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Intensity: 0-100, representing 0% to 100% blend strength
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SEPIA_INTENSITY);
	if (pSlider) {
		pSlider->SetRange(0, 100);
		pSlider->SetPos(100); // full sepia by default
	}

	UpdateLabelAndPreview();

	return TRUE;
}

void CSepiaToneDlg::UpdateLabelAndPreview()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SEPIA_INTENSITY);
	int intensityPos = pSlider ? pSlider->GetPos() : 100;

	CString strVal;	
	strVal.Format(_T("Intensity: %d%%"), intensityPos);
	m_ctrlIntensityValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplySepiaTone(intensityPos / 100.0);
}

void CSepiaToneDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}