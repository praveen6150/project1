#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CBrightnessContrastDlg.h"
#include "project1View.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CBrightnessContrastDlg, CDialogEx)

CBrightnessContrastDlg::CBrightnessContrastDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BRIGHTNESS_CONTRAST, pParent)
	, m_nBrightness(0)
	, m_nContrast(0)
{
	m_pView = nullptr;
}

CBrightnessContrastDlg::~CBrightnessContrastDlg()
{
}

void CBrightnessContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// Leave this empty! Removing DDX completely prevents any NULL handle crashes.
}

BEGIN_MESSAGE_MAP(CBrightnessContrastDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// 1. SAFE TRACKING: Read directly from the active scrollbar pointer instead of variables
void CBrightnessContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	if (pScrollBar != nullptr)
	{
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		int controlID = pSlider->GetDlgCtrlID();
		int pos = pSlider->GetPos();
		CString strLabel;

		if (controlID == IDC_EDIT_BRIGHTNESS)
		{
			m_nBrightness = pos;
			strLabel.Format(_T("Change Brightness: %d %%"), m_nBrightness);
			SetDlgItemText(IDC_STATIC_BRIGHT_TEXT, strLabel);
		}
		else if (controlID == IDC_EDIT_CONTRAST)
		{
			m_nContrast = pos;
			strLabel.Format(_T("Change Contrast: %d %%"), m_nContrast);
			SetDlgItemText(IDC_STATIC_CONTRAST_TEXT, strLabel);
		}
		if (m_pView != nullptr)
			m_pView->ApplyLiveBrightnessContrast(m_nBrightness, m_nContrast);
	}
}

// 2. SAFE INITIALIZATION: Force the positions directly using the window IDs
BOOL CBrightnessContrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog(); // This call is fine here

	CSliderCtrl* pBrightCtrl = (CSliderCtrl*)GetDlgItem(IDC_EDIT_BRIGHTNESS);
	CSliderCtrl* pContrastCtrl = (CSliderCtrl*)GetDlgItem(IDC_EDIT_CONTRAST);

	if (pBrightCtrl)
	{
		pBrightCtrl->SetRange(-100, 100, TRUE);
		pBrightCtrl->SetPos(0);
		pBrightCtrl->SetLineSize(1);
		pBrightCtrl->SetPageSize(10);
	}

	if (pContrastCtrl)
	{
		pContrastCtrl->SetRange(-100, 100, TRUE);
		pContrastCtrl->SetPos(0);
		pContrastCtrl->SetLineSize(1);
		pContrastCtrl->SetPageSize(10);
	}

	return TRUE;
}