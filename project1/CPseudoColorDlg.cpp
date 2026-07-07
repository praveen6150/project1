#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CPseudoColorDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CPseudoColorDlg, CDialogEx)

CPseudoColorDlg::CPseudoColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PSEUDO_COLOR, pParent)
	, m_pView(nullptr)
{}

CPseudoColorDlg::~CPseudoColorDlg()
{}

void CPseudoColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COLOR_MAP, m_ctrlComboMap);
}

BEGIN_MESSAGE_MAP(CPseudoColorDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR_MAP, &CPseudoColorDlg::OnCbnSelchangeComboColorMap)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CPseudoColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlComboMap.AddString(_T("Jet (Thermal / Rainbow)"));
	m_ctrlComboMap.AddString(_T("Hot (Fire / Red-Yellow)"));
	m_ctrlComboMap.AddString(_T("Cool (Cyan-Magenta)"));
	m_ctrlComboMap.SetCurSel(0);

	// Initialize the slider range from 0 to 255
	// Replace IDC_SLIDER_COLOR_SHIFT with your actual slider ID from the resource editor
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_SHIFT);
	if (pSlider) {
		pSlider->SetRange(0, 255);
		pSlider->SetPos(0); // Start at 0
	}

	return TRUE;
}

void CPseudoColorDlg::OnCbnSelchangeComboColorMap()
{
	int selectedIndex = m_ctrlComboMap.GetCurSel();
	if (m_pView && selectedIndex != CB_ERR) {
		m_pView->ApplyLivePseudoColor(selectedIndex, GetSliderValue());
	}
}

void CPseudoColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int selectedIndex = m_ctrlComboMap.GetCurSel();
	if (m_pView && selectedIndex != CB_ERR) {
		// Pass both the map index and the slider position
		m_pView->ApplyLivePseudoColor(selectedIndex, GetSliderValue());
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
