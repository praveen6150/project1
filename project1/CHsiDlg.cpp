// CHsiDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h" 
#include "CHsiDlg.h"
#include "project1View.h"
#include "resource.h"


IMPLEMENT_DYNAMIC(CHsiDlg, CDialogEx)

CHsiDlg::CHsiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HSI, pParent)
	, m_nHue(0)
	, m_nSaturation(0)
	, m_nIntensity(0)
	, m_pView(nullptr)
{

}

CHsiDlg::~CHsiDlg()
{
}

void CHsiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHsiDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SATURATION, &CHsiDlg::OnNMCustomdrawSliderSaturation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_INTENSITY, &CHsiDlg::OnNMCustomdrawSliderIntensity)
END_MESSAGE_MAP()


// CHsiDlg message handlers

BOOL CHsiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grab pointers directly from the underlying layout resource tree
	CSliderCtrl* pHue = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE);
	CSliderCtrl* pSat = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SATURATION);
	CSliderCtrl* pInt = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY);

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

	if (pInt != nullptr)
	{
		pInt->SetRange(-100, 100, TRUE);
		pInt->SetPos(0);
		pInt->SetLineSize(1);
		pInt->SetPageSize(10);
	}

	SetDlgItemText(IDC_STATIC_HUE_TEXT, _T("Hue: 0\u00B0"));
	SetDlgItemText(IDC_STATIC_SATURATION_TEXT, _T("Saturation: 0%"));
	SetDlgItemText(IDC_STATIC_INTENSITY_TEXT, _T("Intensity: 0%"));
	SetDlgItemText(IDC_STATIC_HSI, _T("HSI = (0, 0, 0)"));

	return TRUE;
}
// --- 2. LIVE SCROLL TRACKING WORKER ---
void CHsiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
	else if (controlID == IDC_SLIDER_INTENSITY)
	{
		m_nIntensity = currentPos;
		strLabel.Format(_T("Intensity: %d%%"), m_nIntensity);
		SetDlgItemText(IDC_STATIC_INTENSITY_TEXT, strLabel);
	}

	CString strSummary;
	strSummary.Format(_T("HSI = (%d, %d, %d)"), m_nHue, m_nSaturation, m_nIntensity);
	SetDlgItemText(IDC_STATIC_HSI, strSummary);

	if (m_pView == nullptr) return;

	// Always apply the full-resolution update directly — no timer, no throttling
	m_pView->ApplyLiveHSI(m_nHue, m_nSaturation, m_nIntensity);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHsiDlg::OnNMCustomdrawSliderSaturation(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CHsiDlg::OnNMCustomdrawSliderIntensity(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

