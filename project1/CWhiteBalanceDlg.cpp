#include "pch.h"
#include "project1.h"
#include "CWhiteBalanceDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CWhiteBalanceDlg, CDialogEx)

CWhiteBalanceDlg::CWhiteBalanceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_WHITEBALANCE, pParent)
	, m_temperature(0)
	, m_tint(0)
{
}

CWhiteBalanceDlg::~CWhiteBalanceDlg()
{
}

void CWhiteBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TEMPERATURE, m_sliderTemperature);
	DDX_Control(pDX, IDC_SLIDER_TINT, m_sliderTint);
}

BEGIN_MESSAGE_MAP(CWhiteBalanceDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CWhiteBalanceDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CWhiteBalanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderTemperature.SetRange(-100, 100, TRUE);
	m_sliderTemperature.SetPos(0);
	m_sliderTemperature.SetTicFreq(50);

	m_sliderTint.SetRange(-100, 100, TRUE);
	m_sliderTint.SetPos(0);
	m_sliderTint.SetTicFreq(50);

	CString text;
	text.Format(_T("Temperature: %d"), m_temperature);
	SetDlgItemText(IDC_STATIC_TEMPERATURE_TEXT, text);

	text.Format(_T("Tint: %d"), m_tint);
	SetDlgItemText(IDC_STATIC_TINT_TEXT, text);

	return TRUE;
}

void CWhiteBalanceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString text;

	if (pScrollBar == (CScrollBar*)&m_sliderTemperature)
	{
		m_temperature = m_sliderTemperature.GetPos();
		text.Format(_T("Temperature: %d"), m_temperature);
		SetDlgItemText(IDC_STATIC_TEMPERATURE_TEXT, text);
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderTint)
	{
		m_tint = m_sliderTint.GetPos();
		text.Format(_T("Tint: %d"), m_tint);
		SetDlgItemText(IDC_STATIC_TINT_TEXT, text);
	}

	if (m_pTargetView)
		m_pTargetView->ApplyLiveWhiteBalance(m_temperature, m_tint);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWhiteBalanceDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CWhiteBalanceDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}

