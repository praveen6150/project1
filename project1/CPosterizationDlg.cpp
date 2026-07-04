// CPosterizationDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CPosterizationDlg.h"
#include "project1Doc.h"  
#include "project1View.h" 

// CPosterizationDlg dialog

IMPLEMENT_DYNAMIC(CPosterizationDlg, CDialogEx)

CPosterizationDlg::CPosterizationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_POINTPROCESS_POSTERIZATION, pParent)
{

}

CPosterizationDlg::~CPosterizationDlg()
{
}

void CPosterizationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_POSTER_LEVELS, m_sliderLevels);
}

BEGIN_MESSAGE_MAP(CPosterizationDlg, CDialogEx)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CPosterizationDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set slider range from 2 color levels up to 64 color levels
    m_sliderLevels.SetRange(2, 64);
    m_sliderLevels.SetPos(m_nLevels);

    CString strVal;
    strVal.Format(_T("Posterization Levels: %d"), m_nLevels);
    SetDlgItemText(IDC_STATIC_POSTER_TEXT, strVal);

    return TRUE;
}

void CPosterizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_sliderLevels)
    {
        m_nLevels = m_sliderLevels.GetPos();

        CString strVal;
        strVal.Format(_T("Posterization Levels: %d"), m_nLevels);
        SetDlgItemText(IDC_STATIC_POSTER_TEXT, strVal);

        // Send the live preview update straight to the view canvas
        CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
        if (pFrame)
        {
            CFrameWnd* pActiveFrame = pFrame->GetActiveFrame();
            if (pActiveFrame)
            {
                Cproject1View* pView = (Cproject1View*)pActiveFrame->GetActiveView();
                if (pView)
                {
                    pView->ApplyLivePosterization(m_nLevels);
                }
            }
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
//BEGIN_MESSAGE_MAP(CPosterizationDlg, CDialogEx)
//END_MESSAGE_MAP()


// CPosterizationDlg message handlers
