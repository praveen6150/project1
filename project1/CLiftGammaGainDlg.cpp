#include "pch.h"
#include "project1.h"
#include "CLiftGammaGainDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CLiftGammaGainDlg, CDialogEx)

CLiftGammaGainDlg::CLiftGammaGainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LIFTGAMMAGAIN, pParent)
	, m_liftR(0), m_liftG(0), m_liftB(0)
	, m_gammaR(100), m_gammaG(100), m_gammaB(100)   // 100 = 1.00 (neutral)
	, m_gainR(100), m_gainG(100), m_gainB(100)      // 100 = 1.00 (neutral)
{
}

CLiftGammaGainDlg::~CLiftGammaGainDlg()
{
}

void CLiftGammaGainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LIFT_R, m_sliderLiftR);
	DDX_Control(pDX, IDC_SLIDER_LIFT_G, m_sliderLiftG);
	DDX_Control(pDX, IDC_SLIDER_LIFT_B, m_sliderLiftB);
	DDX_Control(pDX, IDC_SLIDER_GAMMA_R, m_sliderGammaR);
	DDX_Control(pDX, IDC_SLIDER_GAMMA_G, m_sliderGammaG);
	DDX_Control(pDX, IDC_SLIDER_GAMMA_B, m_sliderGammaB);
	DDX_Control(pDX, IDC_SLIDER_GAIN_R, m_sliderGainR);
	DDX_Control(pDX, IDC_SLIDER_GAIN_G, m_sliderGainG);
	DDX_Control(pDX, IDC_SLIDER_GAIN_B, m_sliderGainB);
}

BEGIN_MESSAGE_MAP(CLiftGammaGainDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CLiftGammaGainDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CLiftGammaGainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderLiftR.SetRange(-50, 50, TRUE);  m_sliderLiftR.SetPos(m_liftR);
	m_sliderLiftG.SetRange(-50, 50, TRUE);  m_sliderLiftG.SetPos(m_liftG);
	m_sliderLiftB.SetRange(-50, 50, TRUE);  m_sliderLiftB.SetPos(m_liftB);

	m_sliderGammaR.SetRange(10, 300, TRUE); m_sliderGammaR.SetPos(m_gammaR);
	m_sliderGammaG.SetRange(10, 300, TRUE); m_sliderGammaG.SetPos(m_gammaG);
	m_sliderGammaB.SetRange(10, 300, TRUE); m_sliderGammaB.SetPos(m_gammaB);

	m_sliderGainR.SetRange(0, 200, TRUE);   m_sliderGainR.SetPos(m_gainR);
	m_sliderGainG.SetRange(0, 200, TRUE);   m_sliderGainG.SetPos(m_gainG);
	m_sliderGainB.SetRange(0, 200, TRUE);   m_sliderGainB.SetPos(m_gainB);

	UpdateAllLabels();

	return TRUE;
}

// --- Label update helpers ---

void CLiftGammaGainDlg::UpdateLiftLabel(int channel)
{
	CString text;
	int val, id;
	LPCTSTR name;

	switch (channel)
	{
	case 0: val = m_liftR; id = IDC_STATIC_LIFT_R; name = _T("R"); break;
	case 1: val = m_liftG; id = IDC_STATIC_LIFT_G; name = _T("G"); break;
	default: val = m_liftB; id = IDC_STATIC_LIFT_B; name = _T("B"); break;
	}

	text.Format(_T("%s: %+.2f"), name, val / 100.0);
	SetDlgItemText(id, text);
}

void CLiftGammaGainDlg::UpdateGammaLabel(int channel)
{
	CString text;
	int val, id;
	LPCTSTR name;

	switch (channel)
	{
	case 0: val = m_gammaR; id = IDC_STATIC_GAMMA_R; name = _T("R"); break;
	case 1: val = m_gammaG; id = IDC_STATIC_GAMMA_G; name = _T("G"); break;
	default: val = m_gammaB; id = IDC_STATIC_GAMMA_B; name = _T("B"); break;
	}

	text.Format(_T("%s: %.2f"), name, val / 100.0);
	SetDlgItemText(id, text);
}

void CLiftGammaGainDlg::UpdateGainLabel(int channel)
{
	CString text;
	int val, id;
	LPCTSTR name;

	switch (channel)
	{
	case 0: val = m_gainR; id = IDC_STATIC_GAIN_R; name = _T("R"); break;
	case 1: val = m_gainG; id = IDC_STATIC_GAIN_G; name = _T("G"); break;
	default: val = m_gainB; id = IDC_STATIC_GAIN_B; name = _T("B"); break;
	}

	text.Format(_T("%s: %.2f"), name, val / 100.0);
	SetDlgItemText(id, text);
}

void CLiftGammaGainDlg::UpdateAllLabels()
{
	UpdateLiftLabel(0);  UpdateLiftLabel(1);  UpdateLiftLabel(2);
	UpdateGammaLabel(0); UpdateGammaLabel(1); UpdateGammaLabel(2);
	UpdateGainLabel(0);  UpdateGainLabel(1);  UpdateGainLabel(2);
}

// --- Scroll handling ---

void CLiftGammaGainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

         if (pSlider == &m_sliderLiftR) { m_liftR = pSlider->GetPos();  UpdateLiftLabel(0); }
	else if (pSlider == &m_sliderLiftG) { m_liftG = pSlider->GetPos();  UpdateLiftLabel(1); }
	else if (pSlider == &m_sliderLiftB) { m_liftB = pSlider->GetPos();  UpdateLiftLabel(2); }
	else if (pSlider == &m_sliderGammaR) { m_gammaR = pSlider->GetPos(); UpdateGammaLabel(0); }
	else if (pSlider == &m_sliderGammaG) { m_gammaG = pSlider->GetPos(); UpdateGammaLabel(1); }
	else if (pSlider == &m_sliderGammaB) { m_gammaB = pSlider->GetPos(); UpdateGammaLabel(2); }
	else if (pSlider == &m_sliderGainR) { m_gainR = pSlider->GetPos();  UpdateGainLabel(0); }
	else if (pSlider == &m_sliderGainG) { m_gainG = pSlider->GetPos();  UpdateGainLabel(1); }
	else if (pSlider == &m_sliderGainB) { m_gainB = pSlider->GetPos();  UpdateGainLabel(2); }

	UpdateLive();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLiftGammaGainDlg::UpdateLive()
{
	if (m_pTargetView)
	{
		m_pTargetView->ApplyLiveLiftGammaGain(
			m_liftR, m_liftG, m_liftB,
			m_gammaR, m_gammaG, m_gammaB,
			m_gainR, m_gainG, m_gainB);
	}
}

void CLiftGammaGainDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CLiftGammaGainDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}
