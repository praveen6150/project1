#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CQuantumSimDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CQuantumSimDlg, CDialogEx)

CQuantumSimDlg::CQuantumSimDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUANTUM_SIM, pParent)
	, m_pView(nullptr)
{
}

CQuantumSimDlg::~CQuantumSimDlg()
{
}

void CQuantumSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QUANTUM_LEVELS, m_ctrlLevelsValue);
	DDX_Control(pDX, IDC_STATIC_QUANTUM_UNCERTAINTY, m_ctrlUncertaintyValue);
}

BEGIN_MESSAGE_MAP(CQuantumSimDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CQuantumSimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Energy Levels: 2 to 32 quantization bands
	CSliderCtrl* pLevels = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUANTUM_LEVELS);
	if (pLevels) {
		pLevels->SetRange(2, 32);
		pLevels->SetPos(8);
	}

	// Uncertainty: stored as slider 0-100, representing sigma 0.0 to 2.0
	CSliderCtrl* pUncertainty = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUANTUM_UNCERTAINTY);
	if (pUncertainty) {
		pUncertainty->SetRange(0, 100);
		pUncertainty->SetPos(20);
	}

	UpdateLabelsAndPreview();

	return TRUE;
}

void CQuantumSimDlg::UpdateLabelsAndPreview()
{
	CSliderCtrl* pLevels = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUANTUM_LEVELS);
	CSliderCtrl* pUncertainty = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUANTUM_UNCERTAINTY);

	int levels = pLevels ? pLevels->GetPos() : 8;
	int uncertaintyPos = pUncertainty ? pUncertainty->GetPos() : 20;
	double uncertainty = uncertaintyPos / 50.0; // slider 0-100 -> sigma 0.0-2.0

	CString strVal;
	strVal.Format(_T("Energy Levels: %d"), levels);
	m_ctrlLevelsValue.SetWindowText(strVal);

	strVal.Format(_T("Uncertainty (\u03C3): %.2f"), uncertainty);
	m_ctrlUncertaintyValue.SetWindowText(strVal);

	if (m_pView)
		m_pView->ApplyQuantumSim(levels, uncertainty);
}

void CQuantumSimDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateLabelsAndPreview();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

