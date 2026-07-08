#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "COtsuThresholdDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(COtsuThresholdDlg, CDialogEx)

COtsuThresholdDlg::COtsuThresholdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OTSU_THRESHOLD, pParent)
	, m_pView(nullptr)
	, m_computedThreshold(0)
{
}

COtsuThresholdDlg::~COtsuThresholdDlg()
{
}

void COtsuThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OTSU_VALUE, m_ctrlThresholdValue);
}

BEGIN_MESSAGE_MAP(COtsuThresholdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_OTSU_PREVIEW, &COtsuThresholdDlg::OnBnClickedCheckOtsuPreview)
END_MESSAGE_MAP()

BOOL COtsuThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Display the threshold value that was already computed before the dialog opened
	CString strVal;
	strVal.Format(_T("Computed Threshold: %d"), m_computedThreshold);
	m_ctrlThresholdValue.SetWindowText(strVal);

	return TRUE;
}

void COtsuThresholdDlg::OnBnClickedCheckOtsuPreview()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_OTSU_PREVIEW))->GetCheck())
		m_pView->ApplyOtsuThresholdWithValue(m_computedThreshold);
	else
		m_pView->RestoreOriginalPixels(); // would need a small helper to memcpy backup back into live bits
}