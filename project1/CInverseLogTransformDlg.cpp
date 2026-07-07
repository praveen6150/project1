#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CInverseLogTransformDlg.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CInverseLogTransformDlg, CDialogEx)

CInverseLogTransformDlg::CInverseLogTransformDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INVERSE_LOG_TRANSFORM, pParent)
	, m_bInvAutoCalc(FALSE)
	, m_pView(nullptr)
{}

CInverseLogTransformDlg::~CInverseLogTransformDlg()
{}

void CInverseLogTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_INV_C_VALUE, m_ctrlSliderInvC);
	DDX_Control(pDX, IDC_STATIC_INV_C_LABEL, m_ctrlStaticInvCLabel);
	DDX_Check(pDX, IDC_CHECK_INV_AUTO_CALC, m_bInvAutoCalc);
}

BEGIN_MESSAGE_MAP(CInverseLogTransformDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_INV_AUTO_CALC, &CInverseLogTransformDlg::OnBnClickedCheckInvAutoCalc)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CInverseLogTransformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Slider range mapping to representative scaling ranges
	m_ctrlSliderInvC.SetRange(10, 2000);
	m_ctrlSliderInvC.SetPos(1484); // Approximates normal scale base

	m_bInvAutoCalc = TRUE;
	CheckDlgButton(IDC_CHECK_INV_AUTO_CALC, BST_CHECKED);
	m_ctrlSliderInvC.EnableWindow(FALSE);
	m_ctrlStaticInvCLabel.SetWindowText(_T("Constant C = Auto"));

	return TRUE;
}

void CInverseLogTransformDlg::OnBnClickedCheckInvAutoCalc()
{
	UpdateData(TRUE);
	if (m_bInvAutoCalc)
	{
		m_ctrlSliderInvC.EnableWindow(FALSE);
		m_ctrlStaticInvCLabel.SetWindowText(_T("Constant C = Auto"));
	}
	else
	{
		m_ctrlSliderInvC.EnableWindow(TRUE);
		double c_value = m_ctrlSliderInvC.GetPos() / 10.0;
		CString strLabel;
		strLabel.Format(_T("Constant C = %.1f"), c_value);
		m_ctrlStaticInvCLabel.SetWindowText(strLabel);
	}

	if (m_pView) {
		m_pView->ApplyLiveInverseLog(GetConstantC());
	}
}

void CInverseLogTransformDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->GetDlgCtrlID() == IDC_SLIDER_INV_C_VALUE)
	{
		double c_value = m_ctrlSliderInvC.GetPos() / 10.0;
		CString strLabel;
		strLabel.Format(_T("Constant C = %.1f"), c_value);
		m_ctrlStaticInvCLabel.SetWindowText(strLabel);

		if (m_pView) {
			m_pView->ApplyLiveInverseLog(c_value);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
