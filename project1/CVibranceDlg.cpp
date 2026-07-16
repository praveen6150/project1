#include "pch.h"
#include "project1.h"
#include "CVibranceDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CVibranceDlg, CDialogEx)

CVibranceDlg::CVibranceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_VIBRANCE, pParent)
	, m_amount(0)
{
}

CVibranceDlg::~CVibranceDlg()
{
}

void CVibranceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_VIBRANCE, m_sliderAmount);
}

BEGIN_MESSAGE_MAP(CVibranceDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CVibranceDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CVibranceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderAmount.SetRange(-100, 100, TRUE);
	m_sliderAmount.SetPos(0);
	m_sliderAmount.SetTicFreq(50);

	CString text;
	text.Format(_T("Vibrance: %d"), m_amount);
	SetDlgItemText(IDC_STATIC_VIBRANCE_TEXT, text);

	return TRUE;
}

void CVibranceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderAmount)
	{
		m_amount = m_sliderAmount.GetPos();

		CString text;
		text.Format(_T("Vibrance: %d"), m_amount);
		SetDlgItemText(IDC_STATIC_VIBRANCE_TEXT, text);

		if (m_pTargetView)
			m_pTargetView->ApplyLiveVibrance(m_amount);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVibranceDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CVibranceDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}