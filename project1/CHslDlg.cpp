#include "pch.h"
#include "project1.h" // Replace with your actual main application header if different
#include "CHslDlg.h"
#include "project1View.h" // Included so we can access ApplyLiveHSL

IMPLEMENT_DYNAMIC(CHslDlg, CDialogEx)

CHslDlg::CHslDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HSL, pParent)
	, m_pView(nullptr)
	, m_nHue(0)
	, m_nSaturation(0)
	, m_nLightness(0)
{
}

CHslDlg::~CHslDlg()
{
}

void CHslDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHslDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CHslDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Configure Hue Slider (0 - 360 degrees)
	CSliderCtrl* pSliderHue = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE);
	if (pSliderHue)
	{
		pSliderHue->SetRange(0, 360);
		pSliderHue->SetPos(m_nHue);
	}

	// Configure Saturation Slider (0 - 100%)
	CSliderCtrl* pSliderSat = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SATURATION);
	if (pSliderSat)
	{
		pSliderSat->SetRange(0, 100);
		pSliderSat->SetPos(m_nSaturation);
	}

	// Configure Lightness Slider (0 - 100%)
	CSliderCtrl* pSliderLight = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LIGHTNESS);
	if (pSliderLight)
	{
		pSliderLight->SetRange(0, 100);
		pSliderLight->SetPos(m_nLightness);
	}

	// Initialize label display strings
	CString strLabel;
	strLabel.Format(_T("Hue: %d\u00B0"), m_nHue);
	SetDlgItemText(IDC_STATIC_HUE_TEXT, strLabel);

	strLabel.Format(_T("Saturation: %d%%"), m_nSaturation);
	SetDlgItemText(IDC_STATIC_SATURATION_TEXT, strLabel);

	strLabel.Format(_T("Lightness: %d%%"), m_nLightness);
	SetDlgItemText(IDC_STATIC_LIGHTNESS_TEXT, strLabel);

	CString strSummary;
	strSummary.Format(_T("HSL = (%d, %d, %d)"), m_nHue, m_nSaturation, m_nLightness);
	SetDlgItemText(IDC_STATIC_HSL, strSummary);

	return TRUE;
}

void CHslDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == nullptr) return;

	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int controlID = pSlider->GetDlgCtrlID();
	int currentPos = pSlider->GetPos();
	CString strLabel;

	if (controlID == IDC_SLIDER_HUE)
	{
		m_nHue = currentPos;
		strLabel.Format(_T("Hue: %d\u00B0"), m_nHue);
		SetDlgItemText(IDC_STATIC_HUE_TEXT, strLabel);
	}
	else if (controlID == IDC_SLIDER_SATURATION)
	{
		m_nSaturation = currentPos;
		strLabel.Format(_T("Saturation: %d%%"), m_nSaturation);
		SetDlgItemText(IDC_STATIC_SATURATION_TEXT, strLabel);
	}
	else if (controlID == IDC_SLIDER_LIGHTNESS)
	{
		m_nLightness = currentPos;
		strLabel.Format(_T("Lightness: %d%%"), m_nLightness);
		SetDlgItemText(IDC_STATIC_LIGHTNESS_TEXT, strLabel);
	}

	CString strSummary;
	strSummary.Format(_T("HSL = (%d, %d, %d)"), m_nHue, m_nSaturation, m_nLightness);
	SetDlgItemText(IDC_STATIC_HSL, strSummary);

	if (m_pView == nullptr) return;

	m_pView->ApplyLiveHsl(m_nHue, m_nSaturation, m_nLightness);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
