// CSolarizationDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CSolarizationDlg.h"
#include "project1Doc.h"
#include "project1View.h"


// CSolarizationDlg dialog

IMPLEMENT_DYNAMIC(CSolarizationDlg, CDialogEx)

CSolarizationDlg::CSolarizationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOLARIZATION_DLG, pParent)
	, m_nIntensity(0) // Initialize here
{

}

CSolarizationDlg::~CSolarizationDlg()
{
}

void CSolarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_SOLAR_INTENSITY, m_sliderIntensity);
}


BEGIN_MESSAGE_MAP(CSolarizationDlg, CDialogEx)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


void CSolarizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_sliderIntensity)
    {
        m_nIntensity = m_sliderIntensity.GetPos();
        int percentage = (int)((m_nIntensity * 100.0) / 255.0 + 0.5);
        CString strVal;
        strVal.Format(_T("Solarization Factor: %d%% (Value: %d)"), percentage, m_nIntensity);
        SetDlgItemText(IDC_STATIC_SOLAR_VAL, strVal);

        CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
        if (pFrame)
        {
            CFrameWnd* pActiveFrame = pFrame->GetActiveFrame();
            if (pActiveFrame)
            {
                Cproject1View* pView = (Cproject1View*)pActiveFrame->GetActiveView();
                if (pView)
                {
                    pView->ApplyLiveSolarization(m_nIntensity);
                }
            }
        }
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL CSolarizationDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
	m_nIntensity = 255;
    // Set slider range to cover the entire 8-bit grayscale spectrum
    m_sliderIntensity.SetRange(0, 255);
    m_sliderIntensity.SetPos(m_nIntensity);

    // Synchronize your initial static value label
    CString strVal;
    strVal.Format(_T("%d"), m_nIntensity);
    SetDlgItemText(IDC_STATIC_SOLAR_VAL, strVal);

    return TRUE;
}
// CSolarizationDlg message handlers
