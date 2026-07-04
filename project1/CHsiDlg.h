#pragma once
#include "afxdialogex.h"

class Cproject1View;
class Cproject1Doc;

class CHsiDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CHsiDlg)

public:
    CHsiDlg(CWnd* pParent = nullptr);
    virtual ~CHsiDlg();

    Cproject1View* m_pView;

    int m_nHue;
    int m_nSaturation;
    int m_nIntensity;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HSI };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnNMCustomdrawSliderSaturation(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMCustomdrawSliderIntensity(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()

private:
    CImage      m_previewImage;
    UINT_PTR    m_nTimerID;
    bool        m_bDragging;

    // Velocity tracking
    DWORD       m_dwLastMoveTime;
    int         m_nLastPos;
    int         m_nLastControlID;

    void BuildPreviewImage();

    static const int  PREVIEW_SCALE = 4;   // 1/4 size preview
    static const DWORD FAST_THRESHOLD_MS = 30;  // < 30ms  → too fast, skip
    static const DWORD SLOW_THRESHOLD_MS = 80;  // > 80ms  → slow, full res
    static const DWORD SETTLE_MS = 200; // stopped → full res
};