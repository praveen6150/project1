// CHsiDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CHsiDlg.h"
#include "project1View.h"
#include "resource.h"

// CHsiDlg dialog

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
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SATURATION, &CHsiDlg::OnNMCustomdrawSliderSaturation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_INTENSITY, &CHsiDlg::OnNMCustomdrawSliderIntensity)
END_MESSAGE_MAP()


// CHsiDlg message handlers

BOOL CHsiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nTimerID = 0;
	m_bDragging = false;
	m_dwLastMoveTime = 0;     
	m_nLastPos = 0;     
	m_nLastControlID = 0;     

	// Grab pointers directly from the underlying layout resource tree
	CSliderCtrl* pHue = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HUE);
	CSliderCtrl* pSat = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SATURATION);
	CSliderCtrl* pInt = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY);

	// Configure the exact requested custom tracking boundaries
	if (pHue != nullptr)
	{
		pHue->SetRange(-180, 180, TRUE); // Hue cycle span
		pHue->SetPos(0);           // Default center index
		pHue->SetLineSize(1);
		pHue->SetPageSize(10);
	
	}

	if (pSat != nullptr)
	{
		pSat->SetRange(-100, 100, true); // Saturation percentage span
		pSat->SetPos(0);           // Default center index
		pSat->SetLineSize(1);
		pSat->SetPageSize(10);
	}

	if (pInt != nullptr)
	{
		pInt->SetRange(-100, 100, true); // Intensity percentage span
		pInt->SetPos(0);           // Default center index
		pInt->SetLineSize(1);
		pInt->SetPageSize(10);
	}

	// Set baseline default labels on window frame popup open
	SetDlgItemText(IDC_STATIC_HUE_TEXT, _T("Hue: 0\u00B0"));
	SetDlgItemText(IDC_STATIC_SATURATION_TEXT, _T("Saturation: 0%"));
	SetDlgItemText(IDC_STATIC_INTENSITY_TEXT, _T("Intensity: 0%"));
	SetDlgItemText(IDC_STATIC_HSI, _T("HSI = (0, 0, 0)"));

	Invalidate(TRUE);
	UpdateWindow();
	BuildPreviewImage();

	return TRUE;
}

// --- 2. LIVE SCROLL TRACKING WORKER ---
void CHsiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == TB_ENDTRACK)
		return;

	if (pScrollBar == nullptr) return;

	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int controlID = pSlider->GetDlgCtrlID();
	int currentPos = pSlider->GetPos();
	CString strLabel;

	// ── Label updates (always instant, no cost) ───────────────────────────
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

	// ── Velocity detection ────────────────────────────────────────────────
	DWORD dwNow = GetTickCount();
	DWORD dwInterval = dwNow - m_dwLastMoveTime;  // ms since last move event
	m_dwLastMoveTime = dwNow;

	// Always kill the previous settle timer first
	if (m_nTimerID) { KillTimer(m_nTimerID); m_nTimerID = 0; }

	if (dwInterval < FAST_THRESHOLD_MS)
	{
		// ── FAST: user is flicking the slider quickly → skip image update ─
		// Just restart the settle timer. No preview, no full update.
		m_nTimerID = SetTimer(1, SETTLE_MS, NULL);
	}
	else if (dwInterval < SLOW_THRESHOLD_MS)
	{
		// ── MEDIUM: moderate speed → show cheap downscaled preview only ───
		m_pView->ApplyLiveHSIPreview(m_nHue, m_nSaturation, m_nIntensity, &m_previewImage);
		m_nTimerID = SetTimer(1, SETTLE_MS, NULL);
	}
	else
	{
		// ── SLOW: user is moving deliberately slowly → full res live update
		m_pView->ApplyLiveHSI(m_nHue, m_nSaturation, m_nIntensity);
		// No timer needed — we already applied full res
		// But still set settle timer in case they stop here
		m_nTimerID = SetTimer(1, SETTLE_MS, NULL);
	}
}

void CHsiDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = 0;

		// ── SETTLED: user stopped moving → always apply full resolution ───
		if (m_pView != nullptr)
			m_pView->ApplyLiveHSI(m_nHue, m_nSaturation, m_nIntensity);
	}
	CDialogEx::OnTimer(nIDEvent);
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

void CHsiDlg::BuildPreviewImage()
{
	Cproject1Doc* pDoc = m_pView->GetDocument();
	if (!pDoc || pDoc->m_imageOriginal.IsNull()) return;

	int fullW = pDoc->m_imageOriginal.GetWidth();
	int fullH = pDoc->m_imageOriginal.GetHeight();
	int prevW = max(1, fullW / PREVIEW_SCALE);
	int prevH = max(1, fullH / PREVIEW_SCALE);

	m_previewImage.Destroy();
	m_previewImage.Create(prevW, prevH, pDoc->m_imageOriginal.GetBPP());

	HDC hdcSrc = pDoc->m_imageOriginal.GetDC();
	HDC hdcDest = m_previewImage.GetDC();

	// ✅ HALFTONE correctly handles bottom-up BMP/JPEG orientation
	::SetStretchBltMode(hdcDest, HALFTONE);
	::SetBrushOrgEx(hdcDest, 0, 0, NULL);

	::StretchBlt(hdcDest, 0, 0, prevW, prevH,
		hdcSrc, 0, 0, fullW, fullH, SRCCOPY);

	pDoc->m_imageOriginal.ReleaseDC();
	m_previewImage.ReleaseDC();
}

