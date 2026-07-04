#include "pch.h"
#include "project1.h"
#include "CGammaDlg.h"
#include "afxdialogex.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CGammaDlg, CDialogEx)

CGammaDlg::CGammaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GAMMA, pParent)
	, m_dGamma(1.0)
	, m_pView(nullptr) // Initialize pointer cleanly
{
}

CGammaDlg::~CGammaDlg()
{
}

void CGammaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
}

BEGIN_MESSAGE_MAP(CGammaDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CGammaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderGamma.SetRange(1, 50);
	m_sliderGamma.SetPos(10);

	// CLEANED: We removed the CImage BitBlt block from here entirely 
	// because your View's vector now safely handles the backup!

	return TRUE;
}

void CGammaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_sliderGamma)
	{
		int pos = m_sliderGamma.GetPos();
		m_dGamma = pos / 10.0;

		CString strVal;
		strVal.Format(_T("%.1f"), m_dGamma);
		SetDlgItemText(IDC_STATIC_GAMMA_VAL, strVal);

		if (m_pView != nullptr)
		{
			m_pView->ApplyLiveGamma(m_dGamma);
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}