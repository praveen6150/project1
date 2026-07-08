#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CBinaryThresholdDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CBinaryThresholdDlg, CDialogEx)

CBinaryThresholdDlg::CBinaryThresholdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINARY_THRESHOLD, pParent)
	, m_pView(nullptr)
{
}

CBinaryThresholdDlg::~CBinaryThresholdDlg()
{
}

void CBinaryThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BINARY_THRESHOLD_VALUE, m_ctrlThresholdValue);
}

BEGIN_MESSAGE_MAP(CBinaryThresholdDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CBinaryThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BINARY_THRESHOLD);
	if (pSlider) {
		pSlider->SetRange(0, 255);
		pSlider->SetPos(128); // sensible neutral starting point, mid-gray
	}

	CString strVal;
	strVal.Format(_T("Threshold: %d"), 128);
	m_ctrlThresholdValue.SetWindowText(strVal);

	// Apply the initial preview immediately so the image reflects the starting slider position
	if (m_pView)
		m_pView->ApplyBinaryThreshold(128);

	return TRUE;
}

int CBinaryThresholdDlg::GetSliderValue()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BINARY_THRESHOLD);
	return pSlider ? pSlider->GetPos() : 0;
}

void CBinaryThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int thresholdValue = GetSliderValue();

	CString strVal;
	strVal.Format(_T("Threshold: %d"), thresholdValue);
	m_ctrlThresholdValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplyBinaryThreshold(thresholdValue);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
