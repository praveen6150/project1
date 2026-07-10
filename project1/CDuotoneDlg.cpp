#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CDuotoneDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CDuotoneDlg, CDialogEx)

CDuotoneDlg::CDuotoneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DUOTONE, pParent)
	, m_pView(nullptr)
	, m_shadowColor(RGB(20, 20, 90))      // default: deep blue shadows
	, m_highlightColor(RGB(255, 240, 200)) // default: warm cream highlights
{}

CDuotoneDlg::~CDuotoneDlg()
{}

void CDuotoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DUOTONE_SHADOW_SWATCH, m_ctrlShadowSwatch);
	DDX_Control(pDX, IDC_STATIC_DUOTONE_HIGHLIGHT_SWATCH, m_ctrlHighlightSwatch);
}

BEGIN_MESSAGE_MAP(CDuotoneDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DUOTONE_SHADOW, &CDuotoneDlg::OnBnClickedButtonDuotoneShadow)
	ON_BN_CLICKED(IDC_BUTTON_DUOTONE_HIGHLIGHT, &CDuotoneDlg::OnBnClickedButtonDuotoneHighlight)
END_MESSAGE_MAP()

BOOL CDuotoneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	PaintSwatch(m_ctrlShadowSwatch, m_shadowColor);
	PaintSwatch(m_ctrlHighlightSwatch, m_highlightColor);

	UpdatePreview();

	return TRUE;
}

// Fills the given static control with a solid color, so the user can see
// exactly which color is currently selected without opening the picker again.
void CDuotoneDlg::PaintSwatch(CStatic& swatchCtrl, COLORREF color)
{
	// Draw directly using the control's device context
	CDC* pDC = swatchCtrl.GetDC();
	CRect rect;
	swatchCtrl.GetClientRect(&rect);
	pDC->FillSolidRect(&rect, color);
	swatchCtrl.ReleaseDC(pDC);
}

void CDuotoneDlg::OnBnClickedButtonDuotoneShadow()
{
	CColorDialog dlg(m_shadowColor);
	if (dlg.DoModal() == IDOK)
	{
		m_shadowColor = dlg.GetColor();
		PaintSwatch(m_ctrlShadowSwatch, m_shadowColor);
		UpdatePreview();
	}
}

void CDuotoneDlg::OnBnClickedButtonDuotoneHighlight()
{
	CColorDialog dlg(m_highlightColor);
	if (dlg.DoModal() == IDOK)
	{
		m_highlightColor = dlg.GetColor();
		PaintSwatch(m_ctrlHighlightSwatch, m_highlightColor);
		UpdatePreview();
	}
}

void CDuotoneDlg::UpdatePreview()
{
	if (m_pView)
		m_pView->ApplyDuotone(m_shadowColor, m_highlightColor);
}
