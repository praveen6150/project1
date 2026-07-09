#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CCurvesAdjustmentDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CCurvesAdjustmentDlg, CDialogEx)

CCurvesAdjustmentDlg::CCurvesAdjustmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CURVES_ADJUSTMENT, pParent)
	, m_pView(nullptr)
{
}

CCurvesAdjustmentDlg::~CCurvesAdjustmentDlg()
{
}

void CCurvesAdjustmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// Note: we don't DDX_Control the curve area to m_curveControl here —
	// instead we SubclassDlgItem it in OnInitDialog, since m_curveControl
	// is a custom class, not one DDX recognizes automatically.
}

BEGIN_MESSAGE_MAP(CCurvesAdjustmentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CURVE, &CCurvesAdjustmentDlg::OnBnClickedButtonResetCurve)
END_MESSAGE_MAP()

BOOL CCurvesAdjustmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Attach our custom CCurveControl class onto the placeholder static control
	m_curveControl.SubclassDlgItem(IDC_STATIC_CURVE_AREA, this);
	m_curveControl.SetChangeCallback(&CCurvesAdjustmentDlg::OnCurveChanged, this);

	// Set a professional window caption title
	SetWindowText(_T("Interactive Tone Curves Adjustment"));

	ApplyCurrentCurve();

	return TRUE;
}

void CCurvesAdjustmentDlg::OnCurveChanged(void* context)
{
	// Static bridge function: casts back to the dialog instance and applies the curve
	CCurvesAdjustmentDlg* pThis = (CCurvesAdjustmentDlg*)context;
	if (pThis)
		pThis->ApplyCurrentCurve();
}

void CCurvesAdjustmentDlg::ApplyCurrentCurve()
{
	if (!m_pView)
		return;

	BYTE lut[256];
	m_curveControl.GetLUT(lut);
	m_pView->ApplyCurvesLUT(lut);
}

void CCurvesAdjustmentDlg::OnBnClickedButtonResetCurve()
{
	m_curveControl.ResetCurve();
	m_curveControl.Invalidate();
	ApplyCurrentCurve();
}
