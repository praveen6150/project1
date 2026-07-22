#include "pch.h"
#include "project1.h"
#include "CBleachBypassDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CBleachBypassDlg, CDialogEx)

CBleachBypassDlg::CBleachBypassDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BLEACHBYPASS, pParent)
	, m_intensity(60)   // a reasonable default that shows the effect clearly
{
}

CBleachBypassDlg::~CBleachBypassDlg()
{
}

void CBleachBypassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BLEACHBYPASS_INTENSITY, m_sliderIntensity);
}

BEGIN_MESSAGE_MAP(CBleachBypassDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CBleachBypassDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CBleachBypassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderIntensity.SetRange(0, 100, TRUE);
	m_sliderIntensity.SetPos(m_intensity);
	m_sliderIntensity.SetTicFreq(25);

	UpdateLabel();

	if (m_pTargetView)
		m_pTargetView->ApplyLiveBleachBypass(m_intensity);

	return TRUE;
}

void CBleachBypassDlg::UpdateLabel()
{
	CString text;
	text.Format(_T("Intensity: %d%%"), m_intensity);
	SetDlgItemText(IDC_STATIC_BLEACHBYPASS_TEXT, text);
}

void CBleachBypassDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderIntensity)
	{
		m_intensity = m_sliderIntensity.GetPos();
		UpdateLabel();

		if (m_pTargetView)
			m_pTargetView->ApplyLiveBleachBypass(m_intensity);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBleachBypassDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CBleachBypassDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}