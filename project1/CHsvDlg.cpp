#include "pch.h"
#include "project1.h"
#include "afxdialogex.h" 
#include "CHsvDlg.h"
#include "project1View.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CHsvDlg, CDialogEx)

CHsvDlg::CHsvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HSV, pParent)
	, m_nHue(0)
	, m_nSaturation(0)
	, m_nValue(0)
	, m_pView(nullptr)
{}

CHsvDlg::~CHsvDlg()
{}

void CHsvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHsvDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CHsvDlg message handlers

BOOL CHsvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grab pointers using the updated resource layout IDs
	CSliderCtrl* pHue = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE);
	CSliderCtrl* pSat = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SATURATION);
	CSliderCtrl* pVal = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_VALUE);

	if (pHue) { pHue->SetRange(-180, 180, TRUE); pHue->SetPos(0); pHue->SetLineSize(1); pHue->SetPageSize(10); }
	if (pSat) { pSat->SetRange(-100, 100, TRUE); pSat->SetPos(0); pSat->SetLineSize(1); pSat->SetPageSize(10); }
	if (pVal) { pVal->SetRange(-100, 100, TRUE); pVal->SetPos(0); pVal->SetLineSize(1); pVal->SetPageSize(10); }

	// Set initial display strings
	SetDlgItemText(IDC_STATIC_HUE_TEXT, _T("Hue: 0\u00B0"));
	SetDlgItemText(IDC_STATIC_SATURATION_TEXT, _T("Saturation: 0%"));
	SetDlgItemText(IDC_STATIC_VALUE_TEXT, _T("Value: 0%"));
	SetDlgItemText(IDC_STATIC_HSV, _T("HSV = (0, 0, 0)"));

	// Initialize downscaled preview container to match 1/4 layout scaling
	if (m_pView && !m_pView->GetDocument()->m_imageOriginal.IsNull())
	{
		int w = m_pView->GetDocument()->m_imageOriginal.GetWidth() / 4;
		int h = m_pView->GetDocument()->m_imageOriginal.GetHeight() / 4;
		m_previewImage.Create(w, h, 24);
	}

	return TRUE;
}

void CHsvDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == nullptr) return;

	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int controlID = pSlider->GetDlgCtrlID();
	int currentPos = pSlider->GetPos();
	CString strLabel;

	// Distribute positioning updates to labels
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
	else if (controlID == IDC_SLIDER_VALUE)
	{
		m_nValue = currentPos;
		strLabel.Format(_T("Value: %d%%"), m_nValue);
		SetDlgItemText(IDC_STATIC_VALUE_TEXT, strLabel);
	}

	// Update summary text box
	CString strSummary;
	strSummary.Format(_T("HSV = (%d, %d, %d)"), m_nHue, m_nSaturation, m_nValue);
	SetDlgItemText(IDC_STATIC_HSV, strSummary);

	if (m_pView == nullptr) return;

	// Real-time rendering split logic
	if (nSBCode == SB_THUMBTRACK)
	{
		// Fast performance update via downscaled background preview calculations
		m_pView->ApplyLiveHSVPreview(m_nHue, m_nSaturation, m_nValue, &m_previewImage);
	}
	else if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_ENDSCROLL)
	{
		// Draw complete clean full-resolution image when user finishes scrolling
		m_pView->ApplyLiveHSV(m_nHue, m_nSaturation, m_nValue);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}