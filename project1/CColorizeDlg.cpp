#include "pch.h"
#include "project1.h"
#include "CColorizeDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CColorizeDlg, CDialogEx)

CColorizeDlg::CColorizeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLORIZE, pParent)
	, m_hue(180)   // default to a neutral cyan-ish starting point
	, m_sat(50)
{
}

CColorizeDlg::~CColorizeDlg()
{
}

void CColorizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_COLORIZE_HUE, m_sliderHue);
	DDX_Control(pDX, IDC_SLIDER_COLORIZE_SAT, m_sliderSat);
}

BEGIN_MESSAGE_MAP(CColorizeDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CColorizeDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CColorizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderHue.SetRange(0, 360, TRUE);
	m_sliderHue.SetPos(m_hue);
	m_sliderHue.SetTicFreq(60);

	m_sliderSat.SetRange(0, 100, TRUE);
	m_sliderSat.SetPos(m_sat);
	m_sliderSat.SetTicFreq(20);

	CString text;
	text.Format(_T("Hue: %d"), m_hue);
	SetDlgItemText(IDC_STATIC_COLORIZE_HUE_TEXT, text);

	text.Format(_T("Saturation: %d%%"), m_sat);
	SetDlgItemText(IDC_STATIC_COLORIZE_SAT_TEXT, text);

	return TRUE;
}

void CColorizeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString text;

	if (pScrollBar == (CScrollBar*)&m_sliderHue)
	{
		m_hue = m_sliderHue.GetPos();
		text.Format(_T("Hue: %d"), m_hue);
		SetDlgItemText(IDC_STATIC_COLORIZE_HUE_TEXT, text);
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderSat)
	{
		m_sat = m_sliderSat.GetPos();
		text.Format(_T("Saturation: %d%%"), m_sat);
		SetDlgItemText(IDC_STATIC_COLORIZE_SAT_TEXT, text);
	}

	if (m_pTargetView)
		m_pTargetView->ApplyLiveColorize(m_hue, m_sat);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorizeDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CColorizeDlg::OnCancel()
{
	// Revert to original — apply with saturation 0 doesn't fully restore hue-tint,
	// so we need the view to explicitly restore from m_imageOriginal
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}

