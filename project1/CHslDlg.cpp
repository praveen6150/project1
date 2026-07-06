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

// CHslDlg message handlers
BOOL CHslDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grab pointers directly from the underlying layout resource tree
	CSliderCtrl* pHue = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE);
	CSliderCtrl* pSat = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SATURATION);
	CSliderCtrl* pLit = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_LIGHTNESS);

	if (pHue != nullptr)
	{
		pHue->SetRange(-180, 180, TRUE);
		pHue->SetPos(0);
		pHue->SetLineSize(1);
		pHue->SetPageSize(10);
	}

	if (pSat != nullptr)
	{
		pSat->SetRange(-100, 100, TRUE);
		pSat->SetPos(0);
		pSat->SetLineSize(1);
		pSat->SetPageSize(10);
	}

	if (pLit != nullptr)
	{
		pLit->SetRange(-100, 100, TRUE);
		pLit->SetPos(0);
		pLit->SetLineSize(1);
		pLit->SetPageSize(10);
	}

	// Sync member tracking state variables to match initial slider reset values
	m_nHue = 0;
	m_nSaturation = 0;
	m_nLightness = 0;

	// Establish default baseline label readouts
	SetDlgItemText(IDC_STATIC_HUE_TEXT, _T("Hue: 0\u00B0"));
	SetDlgItemText(IDC_STATIC_SATURATION_TEXT, _T("Saturation: 0%"));
	SetDlgItemText(IDC_STATIC_LIGHTNESS_TEXT, _T("Lightness: 0%"));
	SetDlgItemText(IDC_STATIC_HSL, _T("HSL = (0, 0, 0)"));

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
