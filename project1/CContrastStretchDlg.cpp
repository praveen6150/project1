// CContrastStretchDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CContrastStretchDlg.h"
#include "project1Doc.h"
#include "project1View.h"

// CContrastStretchDlg dialog

IMPLEMENT_DYNAMIC(CContrastStretchDlg, CDialogEx)

CContrastStretchDlg::CContrastStretchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CContrastStretchDlg::~CContrastStretchDlg()
{
}

void CContrastStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST_MIN, m_sliderMin);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST_MAX, m_sliderMax);
}


BEGIN_MESSAGE_MAP(CContrastStretchDlg, CDialogEx)
	ON_WM_HSCROLL()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CONTRAST_MIN, &CContrastStretchDlg::OnNMCustomdrawSliderContrastMin)
END_MESSAGE_MAP()


BOOL CContrastStretchDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_sliderMin.SetRange(0, 254);
    m_sliderMin.SetPos(m_nMin);

    m_sliderMax.SetRange(1, 255);
    m_sliderMax.SetPos(m_nMax);

    CString str;
    str.Format(_T("Min Input Cutoff: %d"), m_nMin);
    SetDlgItemText(IDC_STATIC_MIN_TEXT, str);

    str.Format(_T("Max Input Cutoff: %d"), m_nMax);
    SetDlgItemText(IDC_STATIC_MAX_TEXT, str);

    return TRUE;
}

void CContrastStretchDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_nMin = m_sliderMin.GetPos();
    m_nMax = m_sliderMax.GetPos();

    // Safety constraint: keep Min strictly below Max
    if (m_nMin >= m_nMax)
    {
        if (pScrollBar == (CScrollBar*)&m_sliderMin)
        {
            m_nMin = m_nMax - 1;
            m_sliderMin.SetPos(m_nMin);
        }
        else
        {
            m_nMax = m_nMin + 1;
            m_sliderMax.SetPos(m_nMax);
        }
    }

    CString str;
    str.Format(_T("Min Input Cutoff: %d"), m_nMin);
    SetDlgItemText(IDC_STATIC_MIN_TEXT, str);

    str.Format(_T("Max Input Cutoff: %d"), m_nMax);
    SetDlgItemText(IDC_STATIC_MAX_TEXT, str);

    // Call live preview execution
    CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
    if (pFrame)
    {
        CFrameWnd* pActiveFrame = pFrame->GetActiveFrame();
        if (pActiveFrame)
        {
            Cproject1View* pView = (Cproject1View*)pActiveFrame->GetActiveView();
            if (pView)
            {
                pView->ApplyLiveContrastStretch(m_nMin, m_nMax);
            }
        }
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CContrastStretchDlg::OnNMCustomdrawSliderContrastMin(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
