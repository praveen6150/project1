#include "pch.h"
#include "project1.h"
#include "CExposureDlg.h"
#include "project1View.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CExposureDlg, CDialogEx)

CExposureDlg::CExposureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EXPOSURE, pParent)
	, m_exposureStops100(0)
{}

CExposureDlg::~CExposureDlg()
{}

void CExposureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_sliderExposure);
}

BEGIN_MESSAGE_MAP(CExposureDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CExposureDlg::SetTargetView(Cproject1View* pView)
{
	m_pTargetView = pView;
}

BOOL CExposureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderExposure.SetRange(-200, 200, TRUE);
	m_sliderExposure.SetPos(0);
	m_sliderExposure.SetTicFreq(100);

	UpdateLabel();

	return TRUE;
}

void CExposureDlg::UpdateLabel()
{
	CString text;
	double stops = m_exposureStops100 / 100.0;
	text.Format(_T("Exposure: %+.2f EV"), stops);
	SetDlgItemText(IDC_STATIC_EXPOSURE_TEXT, text);
}

void CExposureDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderExposure)
	{
		m_exposureStops100 = m_sliderExposure.GetPos();
		UpdateLabel();

		if (m_pTargetView)
			m_pTargetView->ApplyLiveExposure(m_exposureStops100);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CExposureDlg::OnOK()
{
	CDialogEx::OnOK();
}

void CExposureDlg::OnCancel()
{
	if (m_pTargetView)
		m_pTargetView->RevertToOriginal();

	CDialogEx::OnCancel();
}
