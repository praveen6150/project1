// CColorBalanceDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CColorBalanceDlg.h"
#include "project1Doc.h"
#include "project1View.h"

// CColorBalanceDlg dialog

IMPLEMENT_DYNAMIC(CColorBalanceDlg, CDialogEx)

CColorBalanceDlg::CColorBalanceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_POINTPROCESS_COLORBALANCE, pParent)
{

}

CColorBalanceDlg::~CColorBalanceDlg()
{
}

void CColorBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_CB_RED, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_CB_GREEN, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_CB_BLUE, m_sliderBlue);
}


BEGIN_MESSAGE_MAP(CColorBalanceDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CColorBalanceDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
	m_nRed = m_nGreen = m_nBlue = 0;  

    // Set ranges from -100 to 100
    m_sliderRed.SetRange(-100, 100, true);
    m_sliderGreen.SetRange(-100, 100, true);
    m_sliderBlue.SetRange(-100, 100, true);
    
    m_sliderRed.SetPos(m_nRed);
    m_sliderGreen.SetPos(m_nGreen);
    m_sliderBlue.SetPos(m_nBlue);

    UpdateLabels();
    return TRUE;
}

void CColorBalanceDlg::UpdateLabels()
{
    // Updates the edit/static indicators side-by-side
    SetDlgItemInt(IDC_EDIT_CB_RED, m_nRed, TRUE);
    SetDlgItemInt(IDC_EDIT_CB_GREEN, m_nGreen, TRUE);
    SetDlgItemInt(IDC_EDIT_CB_BLUE, m_nBlue, TRUE);
}

void CColorBalanceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_nRed = m_sliderRed.GetPos();
    m_nGreen = m_sliderGreen.GetPos();
    m_nBlue = m_sliderBlue.GetPos();

    UpdateLabels();

    // Trigger real-time view preview 
    CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
    if (pFrame)
    {
        CFrameWnd* pActiveFrame = pFrame->GetActiveFrame();
        if (pActiveFrame)
        {
            Cproject1View* pView = (Cproject1View*)pActiveFrame->GetActiveView();
            if (pView)
            {
                pView->ApplyLiveColorBalance(m_nRed, m_nGreen, m_nBlue);
            }
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

