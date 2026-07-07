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
	DDX_Control(pDX, IDC_STATIC_SLIDER_VALUE, m_ctrlSliderValue);
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

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_SHIFT);
	if (pSlider) {
		pSlider->SetRange(0, 255);
		pSlider->SetPos(0);
	}

	// Initialize the value label to match the slider's starting position
	CString strVal;
	strVal.Format(_T("%d"), 0);
	m_ctrlSliderValue.SetWindowText(strVal);

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
	int sliderValue = GetSliderValue();

	// Update the numeric label to reflect the current slider position
	CString strVal;
	strVal.Format(_T("%d"), sliderValue);
	m_ctrlSliderValue.SetWindowText(strVal);

	if (m_pView && selectedIndex != CB_ERR) {
		m_pView->ApplyLivePseudoColor(selectedIndex, sliderValue);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
