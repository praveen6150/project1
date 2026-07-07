#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CGraySlicingDlg.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CGraySlicingDlg, CDialogEx)

CGraySlicingDlg::CGraySlicingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRAY_SLICING, pParent)
	, m_bPreserveBG(TRUE) // Default to preserving background
	, m_pView(nullptr)
{}

CGraySlicingDlg::~CGraySlicingDlg()
{}

void CGraySlicingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SLICE_LOW, m_ctrlSliderLow);
	DDX_Control(pDX, IDC_SLIDER_SLICE_HIGH, m_ctrlSliderHigh);
	DDX_Control(pDX, IDC_STATIC_SLICE_LABEL, m_ctrlStaticLabel);
	DDX_Check(pDX, IDC_CHECK_PRESERVE_BG, m_bPreserveBG);
}

BEGIN_MESSAGE_MAP(CGraySlicingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_PRESERVE_BG, &CGraySlicingDlg::OnBnClickedCheckPreserveBg)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CGraySlicingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set ranges up to 255 for color intensity values
	m_ctrlSliderLow.SetRange(0, 255);
	m_ctrlSliderHigh.SetRange(0, 255);

	// Default initialization range bounds
	m_ctrlSliderLow.SetPos(100);
	m_ctrlSliderHigh.SetPos(150);

	CheckDlgButton(IDC_CHECK_PRESERVE_BG, BST_CHECKED);

	CString strLabel;
	strLabel.Format(_T("Range Selected: [%d, %d]"), 100, 150);
	m_ctrlStaticLabel.SetWindowText(strLabel);

	return TRUE;
}

void CGraySlicingDlg::OnBnClickedCheckPreserveBg()
{
	UpdateData(TRUE);
	if (m_pView) {
		m_pView->ApplyLiveGraySlicing(GetLowValue(), GetHighValue(), m_bPreserveBG);
	}
}

void CGraySlicingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		int lowVal = m_ctrlSliderLow.GetPos();
		int highVal = m_ctrlSliderHigh.GetPos();

		// Keep bounds sane: Prevent low slider from overtaking high slider
		if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SLICE_LOW && lowVal > highVal)
		{
			m_ctrlSliderHigh.SetPos(lowVal);
			highVal = lowVal;
		}
		else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SLICE_HIGH && highVal < lowVal)
		{
			m_ctrlSliderLow.SetPos(highVal);
			lowVal = highVal;
		}

		CString strLabel;
		strLabel.Format(_T("Range Selected: [%d, %d]"), lowVal, highVal);
		m_ctrlStaticLabel.SetWindowText(strLabel);

		if (m_pView) {
			m_pView->ApplyLiveGraySlicing(lowVal, highVal, m_bPreserveBG);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}