#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CQuantumDPPDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CQuantumDPPDlg, CDialogEx)

CQuantumDPPDlg::CQuantumDPPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUANTUM_DPP, pParent)
	, m_pView(nullptr)
{
}

CQuantumDPPDlg::~CQuantumDPPDlg()
{
}

void CQuantumDPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QDPP_SPACING, m_ctrlSpacingValue);
	DDX_Control(pDX, IDC_STATIC_QDPP_REPULSION, m_ctrlRepulsionValue);
	DDX_Control(pDX, IDC_STATIC_QDPP_ENTANGLE, m_ctrlEntangleValue);
	DDX_Control(pDX, IDC_STATIC_QDPP_DOTSIZE, m_ctrlDotSizeValue);
}

BEGIN_MESSAGE_MAP(CQuantumDPPDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CQuantumDPPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grid spacing: 4-30 pixels between candidate points (smaller = denser)
	CSliderCtrl* pSpacing = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_SPACING);
	if (pSpacing) { pSpacing->SetRange(4, 30); pSpacing->SetPos(10); }

	// Repulsion radius: 5-60 pixels
	CSliderCtrl* pRepulsion = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_REPULSION);
	if (pRepulsion) { pRepulsion->SetRange(5, 60); pRepulsion->SetPos(20); }

	// Entanglement strength: 0-100%
	CSliderCtrl* pEntangle = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_ENTANGLE);
	if (pEntangle) { pEntangle->SetRange(0, 100); pEntangle->SetPos(50); }

	// Dot size: 1-8 pixel radius
	CSliderCtrl* pDotSize = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_DOTSIZE);
	if (pDotSize) { pDotSize->SetRange(1, 8); pDotSize->SetPos(2); }

	UpdateLabels();
	ApplyPreview();

	return TRUE;
}

void CQuantumDPPDlg::UpdateLabels()
{
	CSliderCtrl* pSpacing = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_SPACING);
	CSliderCtrl* pRepulsion = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_REPULSION);
	CSliderCtrl* pEntangle = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_ENTANGLE);
	CSliderCtrl* pDotSize = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_DOTSIZE);

	CString strVal;

	strVal.Format(_T("Grid Spacing: %d px"), pSpacing ? pSpacing->GetPos() : 10);
	m_ctrlSpacingValue.SetWindowText(strVal);

	strVal.Format(_T("Repulsion Radius: %d px"), pRepulsion ? pRepulsion->GetPos() : 20);
	m_ctrlRepulsionValue.SetWindowText(strVal);

	strVal.Format(_T("Entanglement: %d%%"), pEntangle ? pEntangle->GetPos() : 50);
	m_ctrlEntangleValue.SetWindowText(strVal);

	strVal.Format(_T("Dot Size: %d px"), pDotSize ? pDotSize->GetPos() : 2);
	m_ctrlDotSizeValue.SetWindowText(strVal);
}

void CQuantumDPPDlg::ApplyPreview()
{
	if (!m_pView) return;

	CSliderCtrl* pSpacing = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_SPACING);
	CSliderCtrl* pRepulsion = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_REPULSION);
	CSliderCtrl* pEntangle = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_ENTANGLE);
	CSliderCtrl* pDotSize = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QDPP_DOTSIZE);

	int spacing = pSpacing ? pSpacing->GetPos() : 10;
	int repulsion = pRepulsion ? pRepulsion->GetPos() : 20;
	double entangle = (pEntangle ? pEntangle->GetPos() : 50) / 100.0;
	int dotSize = pDotSize ? pDotSize->GetPos() : 2;

	m_pView->ApplyQuantumDPP(spacing, repulsion, entangle, dotSize);
}

void CQuantumDPPDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabels();

	// Only recompute the (expensive) stippling on mouse release, not every
	// tick while dragging - this algorithm is much heavier than a simple
	// LUT, so live-per-tick preview would feel sluggish (same lesson learned
	// from the Gaussian Noise random_device performance issue earlier).
	if (nSBCode == SB_ENDSCROLL)
	{
		ApplyPreview();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}