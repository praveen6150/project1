// CLogTransformDlg.cpp : implementation file
//

#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CLogTransformDlg.h"
#include "project1View.h"

// CLogTransformDlg dialog

IMPLEMENT_DYNAMIC(CLogTransformDlg, CDialogEx)

CLogTransformDlg::CLogTransformDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LOG_TRANSFORM, pParent)
    , m_bAutoCalc(FALSE)
    , m_pView(nullptr) // Initialize it here
{}


CLogTransformDlg::~CLogTransformDlg()
{}

void CLogTransformDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_C_VALUE, m_ctrlSliderC);
    DDX_Control(pDX, IDC_STATIC_C_LABEL, m_ctrlStaticCLabel);
    DDX_Check(pDX, IDC_CHECK_AUTO_CALC, m_bAutoCalc);
}

// REGISTERING THE FUNCTIONS SO MFC ROUTES WINDOWS MESSAGES HERE
BEGIN_MESSAGE_MAP(CLogTransformDlg, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK_AUTO_CALC, &CLogTransformDlg::OnBnClickedCheckAutoCalc)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CLogTransformDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set slider range from 10 to 1000 (corresponds to c = 1.0 to 100.0)
    m_ctrlSliderC.SetRange(10, 1000);

    // Set default optimal position (c = 46.0 -> position 460)
    m_ctrlSliderC.SetPos(460);

    // Initial state: Auto Calculate is Checked, Slider is disabled
    m_bAutoCalc = TRUE;
    CheckDlgButton(IDC_CHECK_AUTO_CALC, BST_CHECKED);
    m_ctrlSliderC.EnableWindow(FALSE);
    m_ctrlStaticCLabel.SetWindowText(_T("Constant C = 46.0"));

    return TRUE;
}

// 2. Inside the BN_CLICKED handler for the checkbox
// 2. Update OnBnClickedCheckAutoCalc() to call the live function
void CLogTransformDlg::OnBnClickedCheckAutoCalc()
{
    UpdateData(TRUE);
    if (m_bAutoCalc)
    {
        m_ctrlSliderC.EnableWindow(FALSE);
        m_ctrlStaticCLabel.SetWindowText(_T("Constant C = 46.0"));
    }
    else
    {
        m_ctrlSliderC.EnableWindow(TRUE);
        int pos = m_ctrlSliderC.GetPos();
        double c_value = pos / 10.0;
        CString strLabel;
        strLabel.Format(_T("Constant C = %.1f"), c_value);
        m_ctrlStaticCLabel.SetWindowText(strLabel);
    }

    // Call live update if view pointer is valid
    if (m_pView) {
        m_pView->ApplyLiveLog(GetConstantC());
    }
}

// 3. Inside the WM_HSCROLL handler
void CLogTransformDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar != NULL && pScrollBar->GetDlgCtrlID() == IDC_SLIDER_C_VALUE)
    {
        int pos = m_ctrlSliderC.GetPos();
        double c_value = pos / 10.0;

        CString strLabel;
        strLabel.Format(_T("Constant C = %.1f"), c_value);
        m_ctrlStaticCLabel.SetWindowText(strLabel);

        // Call live update on drag!
        if (m_pView) {
            m_pView->ApplyLiveLog(c_value);
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
