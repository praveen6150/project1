// CBitPlaneDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CBitPlaneDlg.h"
#include "project1Doc.h"
#include "project1View.h"

// CBitPlaneDlg dialog

IMPLEMENT_DYNAMIC(CBitPlaneDlg, CDialogEx)

CBitPlaneDlg::CBitPlaneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_POINTPROCESS_BITPLANE, pParent)
{

}

CBitPlaneDlg::~CBitPlaneDlg()
{
}

void CBitPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BIT_PLANE, m_sliderBit);
}


BEGIN_MESSAGE_MAP(CBitPlaneDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CBitPlaneDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_sliderBit.SetRange(0, 7);
    m_sliderBit.SetPos(m_nBitPlane);

    CString strVal;
    strVal.Format(_T("Bit-Plane Level: Bit %d"), m_nBitPlane);
    SetDlgItemText(IDC_STATIC_BIT_TEXT, strVal);

    return TRUE;
}

void CBitPlaneDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_sliderBit)
    {
        m_nBitPlane = m_sliderBit.GetPos();

        CString strVal;
        strVal.Format(_T("Bit-Plane Level: Bit %d"), m_nBitPlane);
        SetDlgItemText(IDC_STATIC_BIT_TEXT, strVal);

        CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
        if (pFrame)
        {
            CFrameWnd* pActiveFrame = pFrame->GetActiveFrame();
            if (pActiveFrame)
            {
                Cproject1View* pView = (Cproject1View*)pActiveFrame->GetActiveView();
                if (pView)
                {
                    pView->ApplyLiveBitPlane(m_nBitPlane);
                }
            }
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// CBitPlaneDlg message handlers
