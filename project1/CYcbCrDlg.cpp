// CYcbCrDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CYcbCrDlg.h"
#include "project1View.h"
#include "resource.h"

// CYcbCrDlg dialog

IMPLEMENT_DYNAMIC(CYcbCrDlg, CDialogEx)

CYcbCrDlg::CYcbCrDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_YCBCR, pParent)
	, m_nY(0)
	, m_nCb(0)
	, m_nCr(0)
	, m_pView(nullptr)
{
}

CYcbCrDlg::~CYcbCrDlg()
{
}

void CYcbCrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYcbCrDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CYcbCrDlg message handlers

BOOL CYcbCrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grab pointers directly from the underlying layout resource tree
	CSliderCtrl* pLuma = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_Y_LUMA);
	CSliderCtrl* pCb = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_CB);
	CSliderCtrl* pCr = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_CR);

	// Configure the custom tracking boundaries for YCbCr
	if (pLuma != nullptr)
	{
		pLuma->SetRange(-100, 100, TRUE); // Brightness percentage offset span
		pLuma->SetPos(0);                 // Default center index
		pLuma->SetLineSize(1);
		pLuma->SetPageSize(10);
	}

	if (pCb != nullptr)
	{
		pCb->SetRange(-128, 128, TRUE);   // Chroma Blue balance span
		pCb->SetPos(0);                   // Default center index
		pCb->SetLineSize(1);
		pCb->SetPageSize(10);
	}

	if (pCr != nullptr)
	{
		pCr->SetRange(-128, 128, TRUE);   // Chroma Red balance span
		pCr->SetPos(0);                   // Default center index
		pCr->SetLineSize(1);
		pCr->SetPageSize(10);
	}

	// Set baseline default labels on window frame popup open
	SetDlgItemText(IDC_STATIC_Y_TEXT, _T("Luminance (Y): 0%"));
	SetDlgItemText(IDC_STATIC_CB_TEXT, _T("Chroma Blue (Cb): 0"));
	SetDlgItemText(IDC_STATIC_CR_TEXT, _T("Chroma Red (Cr): 0"));
	SetDlgItemText(IDC_STATIC_YCBCR, _T("YCbCr = (0, 0, 0)"));

	Invalidate(TRUE);
	UpdateWindow();

	return TRUE;
}

// --- LIVE SCROLL TRACKING WORKER ---
void CYcbCrDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == TB_ENDTRACK)
		return;

	if (pScrollBar != nullptr)
	{
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		int controlID = pSlider->GetDlgCtrlID();
		int currentPos = pSlider->GetPos();

		CString strLabel;

		if (controlID == IDC_SLIDER_Y_LUMA)
		{
			m_nY = currentPos;
			strLabel.Format(_T("Luminance (Y): %d%%"), m_nY);
			SetDlgItemText(IDC_STATIC_Y_TEXT, strLabel);
		}
		else if (controlID == IDC_SLIDER_CB)
		{
			m_nCb = currentPos;
			strLabel.Format(_T("Chroma Blue (Cb): %d"), m_nCb);
			SetDlgItemText(IDC_STATIC_CB_TEXT, strLabel);
		}
		else if (controlID == IDC_SLIDER_CR)
		{
			m_nCr = currentPos;
			strLabel.Format(_T("Chroma Red (Cr): %d"), m_nCr);
			SetDlgItemText(IDC_STATIC_CR_TEXT, strLabel);
		}

		CString strSummary;
		strSummary.Format(_T("YCbCr = (%d, %d, %d)"), m_nY, m_nCb, m_nCr);
		SetDlgItemText(IDC_STATIC_YCBCR, strSummary);

		if (m_pView != nullptr)
		{
			// This will execute your live view matrix handler
			m_pView->ApplyLiveYCbCr(m_nY, m_nCb, m_nCr);
		}
	}
}