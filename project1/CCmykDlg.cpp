#include "pch.h"
#include "project1.h"
#include "CCmykDlg.h"
#include "afxdialogex.h"
#include "project1View.h" // Access to ApplyCmykFilter

IMPLEMENT_DYNAMIC(CCmykDlg, CDialogEx)

CCmykDlg::CCmykDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CMYK, pParent)
	, m_pView(nullptr)
	, m_nC(100)
	, m_nM(100)
	, m_nY(100)
	, m_nK(100)
{
}

CCmykDlg::~CCmykDlg()
{
}

void CCmykDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCmykDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CCmykDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Grab pointers dynamically from the resource tree layout
	CSliderCtrl* pSliderC = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_CYAN);
	CSliderCtrl* pSliderM = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_MAGENTA);
	CSliderCtrl* pSliderY = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_YELLOW);
	CSliderCtrl* pSliderK = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLACK);

	if (pSliderC != nullptr) { pSliderC->SetRange(-128, 128, TRUE); pSliderC->SetPos(0); }
	if (pSliderM != nullptr) { pSliderM->SetRange(-128, 128, TRUE); pSliderM->SetPos(0); }
	if (pSliderY != nullptr) { pSliderY->SetRange(-128, 128, TRUE); pSliderY->SetPos(0); }
	if (pSliderK != nullptr) { pSliderK->SetRange(-128, 128, TRUE); pSliderK->SetPos(0); }

	m_nC = 0;
	m_nM = 0;
	m_nY = 0;
	m_nK = 0;

	SetDlgItemText(IDC_STATIC_C_TEXT, _T("Cyan: 0"));
	SetDlgItemText(IDC_STATIC_M_TEXT, _T("Magenta: 0"));
	SetDlgItemText(IDC_STATIC_Y_TEXT, _T("Yellow: 0"));
	SetDlgItemText(IDC_STATIC_K_TEXT, _T("Black: 0"));
	SetDlgItemText(IDC_STATIC_CMYK, _T("CMYK = (0, 0, 0, 0)"));

	return TRUE;
}

void CCmykDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == nullptr) return;

	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int controlID = pSlider->GetDlgCtrlID();
	int currentPos = pSlider->GetPos();
	CString strLabel;

	if (controlID == IDC_SLIDER_CYAN)
	{
		m_nC = currentPos;
		strLabel.Format(_T("Cyan: %d%%"), m_nC);
		SetDlgItemText(IDC_STATIC_C_TEXT, strLabel);
	}
	else if (controlID == IDC_SLIDER_MAGENTA)
	{
		m_nM = currentPos;
		strLabel.Format(_T("Magenta: %d%%"), m_nM);
		SetDlgItemText(IDC_STATIC_M_TEXT, strLabel);
	}
	else if (controlID == IDC_SLIDER_YELLOW)
	{
		m_nY = currentPos;
		strLabel.Format(_T("Yellow: %d%%"), m_nY);
		SetDlgItemText(IDC_STATIC_Y_TEXT, strLabel); 
	}
	else if (controlID == IDC_SLIDER_BLACK)
	{
		m_nK = currentPos;
		strLabel.Format(_T("Black: %d%%"), m_nK);
		SetDlgItemText(IDC_STATIC_K_TEXT, strLabel);
	}

	CString strSummary;
	strSummary.Format(_T("CMYK = (%d, %d, %d, %d)"), m_nC, m_nM, m_nY, m_nK);
	SetDlgItemText(IDC_STATIC_CMYK, strSummary);

	if (m_pView == nullptr) return;

	// Pass integer slider parameters directly back to the active view logic
	m_pView->ApplyCmykFilter(m_nC, m_nM, m_nY, m_nK);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}