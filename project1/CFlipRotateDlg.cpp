#include "pch.h"
#include "project1.h"
#include "CFlipRotateDlg.h"
#include "afxdialogex.h"
#include "project1View.h" 

#pragma comment(lib, "gdiplus.lib")

IMPLEMENT_DYNAMIC(CFlipRotateDlg, CDialogEx)

CFlipRotateDlg::CFlipRotateDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG2, pParent)
    , m_op(OP_FLIP_H)
    , m_arbitraryAngle(0)
{
}

CFlipRotateDlg::~CFlipRotateDlg()
{
}

void CFlipRotateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_ANGLE, m_sliderAngle);
    DDX_Control(pDX, IDC_SPIN_ROTATE_ANGLE, m_ctrlAngleSpin);
    DDX_Control(pDX, IDC_EDIT_ROTATE_ANGLE, m_ctrlAngleEdit);
}

BEGIN_MESSAGE_MAP(CFlipRotateDlg, CDialogEx)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_RADIO_ROTATE, &CFlipRotateDlg::OnBnClickedRadioRotate)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CFlipRotateDlg::OnToolTipNotify)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROTATE_ANGLE, &CFlipRotateDlg::OnDeltaposSpinRotateAngle)
END_MESSAGE_MAP()

BOOL CFlipRotateDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Slider range: 0 to 360 degrees
    m_sliderAngle.SetRange(0, 360, TRUE);
    m_sliderAngle.SetPos(0);
    m_sliderAngle.SetTicFreq(45);

    m_toolTip.Create(this, TTS_ALWAYSTIP);
    m_toolTip.AddTool(&m_sliderAngle, LPSTR_TEXTCALLBACK);
    m_toolTip.Activate(TRUE);

    EnableToolTips(TRUE);

    m_ctrlAngleSpin.SetRange(0, 360);
    m_ctrlAngleSpin.SetPos(0); // match the slider's starting position

    CString strVal;
    strVal.Format(_T("%d\xB0"), 0);
    m_ctrlAngleEdit.SetWindowText(strVal);

    return TRUE;
}

BOOL CFlipRotateDlg::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
    UINT_PTR nID = pNMHDR->idFrom;

    if (nID == 0) return FALSE; // no valid tool found

    static TCHAR szBuf[16];
    _stprintf_s(szBuf, _T("%d\xB0"), m_arbitraryAngle);
    pTTT->lpszText = szBuf;

    return TRUE;
}

void CFlipRotateDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_sliderAngle)
    {
        m_arbitraryAngle = m_sliderAngle.GetPos();

        // Keep the spin/edit control in sync with the slider
        m_ctrlAngleSpin.SetPos(m_arbitraryAngle);
        CString strVal;
        strVal.Format(_T("%d\xB0"), m_arbitraryAngle);
        m_ctrlAngleEdit.SetWindowText(strVal);

        if (m_pTargetView)
            m_pTargetView->SetPreviewRotationAngle((double)m_arbitraryAngle, TRUE);

        m_toolTip.Pop();
        m_toolTip.Update();
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CFlipRotateDlg::OnBnClickedRadioRotate()
{
    BOOL bRotate = (((CButton*)GetDlgItem(IDC_RADIO_ROTATE))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_RADIO_90)->EnableWindow(bRotate);
    GetDlgItem(IDC_RADIO_180)->EnableWindow(bRotate);
    GetDlgItem(IDC_RADIO_270)->EnableWindow(bRotate);
}

void CFlipRotateDlg::OnOK()
{
    if (((CButton*)GetDlgItem(IDC_RADIO_FLIP_H))->GetCheck() == BST_CHECKED)
        m_op = OP_FLIP_H;
    else if (((CButton*)GetDlgItem(IDC_RADIO_FLIP_V))->GetCheck() == BST_CHECKED)
        m_op = OP_FLIP_V;
    else if (((CButton*)GetDlgItem(IDC_RADIO_90))->GetCheck() == BST_CHECKED)
        m_op = OP_ROTATE_90;
    else if (((CButton*)GetDlgItem(IDC_RADIO_180))->GetCheck() == BST_CHECKED)
        m_op = OP_ROTATE_180;
    else if (((CButton*)GetDlgItem(IDC_RADIO_270))->GetCheck() == BST_CHECKED)
        m_op = OP_ROTATE_270;
    else if (((CButton*)GetDlgItem(IDC_RADIO_ROTATE))->GetCheck() == BST_CHECKED)
    {
        // "Rotate by angle" selected but no sub-choice made yet — sensible default
        m_op = OP_ROTATE_90;
    }
    else
    {
        m_op = OP_ROTATE_ARBITRARY;
        m_arbitraryAngle = m_sliderAngle.GetPos();
    }

    if (m_pTargetView)
        m_pTargetView->SetPreviewRotationAngle(0, FALSE);

    CDialogEx::OnOK();
}

void CFlipRotateDlg::SetTargetView(Cproject1View* pView)
{
    m_pTargetView = pView;
}

void CFlipRotateDlg::OnCancel() 
{
    if (m_pTargetView)
        m_pTargetView->SetPreviewRotationAngle(0, FALSE);

    CDialogEx::OnCancel();
}

BOOL CFlipRotateDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_toolTip.GetSafeHwnd() != NULL)
        m_toolTip.RelayEvent(pMsg);

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CFlipRotateDlg::OnDeltaposSpinRotateAngle(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    int newAngle = pNMUpDown->iPos + pNMUpDown->iDelta;

    if (newAngle < 0) newAngle = 0;
    if (newAngle > 360) newAngle = 360;

    m_sliderAngle.SetPos(newAngle);
    m_arbitraryAngle = newAngle;

    CString strVal;
    strVal.Format(_T("%d\xB0"), newAngle);
    m_ctrlAngleEdit.SetWindowText(strVal);

    if (m_pTargetView)
        m_pTargetView->SetPreviewRotationAngle((double)newAngle, TRUE);

    *pResult = 0;
}
