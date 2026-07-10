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

	// Initialize scales to 0% - 100% (or change ranges to suit your requirements)
	if (pSliderC != nullptr) { pSliderC->SetRange(0, 100, TRUE); pSliderC->SetPos(100); }
	if (pSliderM != nullptr) { pSliderM->SetRange(0, 100, TRUE); pSliderM->SetPos(100); }
	if (pSliderY != nullptr) { pSliderY->SetRange(0, 100, TRUE); pSliderY->SetPos(100); }
	if (pSliderK != nullptr) { pSliderK->SetRange(0, 100, TRUE); pSliderK->SetPos(100); }

	// Set baseline tracker defaults
	m_nC = 100;
	m_nM = 100;
	m_nY = 100;
	m_nK = 100;

	// Establish default baseline label string readouts
	SetDlgItemText(IDC_STATIC_C_TEXT, _T("Cyan: 100%"));
	SetDlgItemText(IDC_STATIC_M_TEXT, _T("Magenta: 100%"));
	SetDlgItemText(IDC_STATIC_Y_TEXT, _T("Yellow: 100%"));
	SetDlgItemText(IDC_STATIC_K_TEXT, _T("Black: 100%"));
	SetDlgItemText(IDC_STATIC_CMYK, _T("CMYK = (100, 100, 100, 100)"));

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