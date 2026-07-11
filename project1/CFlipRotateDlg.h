#pragma once
#include "afxdialogex.h"
#include <gdiplus.h>

enum FlipRotateOp
{
    OP_FLIP_H,
    OP_FLIP_V,
    OP_ROTATE_90,
    OP_ROTATE_180,
    OP_ROTATE_270,
    OP_ROTATE_ARBITRARY
};

class Cproject1View;

class CFlipRotateDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CFlipRotateDlg)

public:
    CFlipRotateDlg(CWnd* pParent = nullptr);
    virtual ~CFlipRotateDlg();
    void SetTargetView(Cproject1View* pView);

    void OnCancel();

    BOOL PreTranslateMessage(MSG* pMsg);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG2 };
#endif

    FlipRotateOp m_op;
    int m_arbitraryAngle;


protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    Cproject1View* m_pTargetView = nullptr;

    void OnBnClickedRadioRotate();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    CSliderCtrl m_sliderAngle;

    CToolTipCtrl m_toolTip;
    afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpinRotateAngle(NMHDR* pNMHDR, LRESULT* pResult);
    //afx_msg void OnDeltaposSpinRotateAngle(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
public:
    CSpinButtonCtrl m_ctrlAngleSpin;
    CEdit m_ctrlAngleEdit;
};

