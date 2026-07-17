#include "pch.h"
#include "project1.h"
#include "CLuvDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CLuvDlg, CDialogEx)

CLuvDlg::CLuvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LUV, pParent)
	, m_deltaL(0)
	, m_deltaU(0)
	, m_deltaV(0)
{}

CLuvDlg::~CLuvDlg()
{}

void CLuvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LSTAR_LUV, m_sliderL);
	DDX_Control(pDX, IDC_SLIDER_USTAR, m_sliderU);
	DDX_Control(pDX, IDC_SLIDER_VSTAR, m_sliderV);
}

BEGIN_MESSAGE_MAP(CLuvDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CLuvDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CLuvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderL.SetRange(-100, 100, TRUE);
	m_sliderL.SetPos(0);
	m_sliderL.SetTicFreq(50);

	m_sliderU.SetRange(-100, 100, TRUE);
	m_sliderU.SetPos(0);
	m_sliderU.SetTicFreq(50);

	m_sliderV.SetRange(-100, 100, TRUE);
	m_sliderV.SetPos(0);
	m_sliderV.SetTicFreq(50);

	return TRUE;
}

void CLuvDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderL)
		m_deltaL = m_sliderL.GetPos();
	else if (pScrollBar == (CScrollBar*)&m_sliderU)
		m_deltaU = m_sliderU.GetPos();
	else if (pScrollBar == (CScrollBar*)&m_sliderV)
		m_deltaV = m_sliderV.GetPos();

	if (m_pTargetView)
		m_pTargetView->ApplyLiveLuv(m_deltaL, m_deltaU, m_deltaV);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLuvDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CLuvDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}
