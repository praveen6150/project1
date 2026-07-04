#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CHistogramDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CHistogramDlg)

public:
    CHistogramDlg(CWnd* pParent = nullptr);
    virtual ~CHistogramDlg();

    Cproject1View* m_pView;
    int m_nCurrentChannel; // 0=Red, 1=Green, 2=Blue, 3=All

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HISTOGRAM };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    CBrush m_brushWhite;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg void OnPaint();
    afx_msg void OnNcDestroy();
    afx_msg void OnBnClickedRadioRed();
    afx_msg void OnBnClickedRadioGreen();
    afx_msg void OnBnClickedRadioBlue();
    afx_msg void OnBnClickedBtnAll();   // ← New "All" toggle button
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();

    DECLARE_MESSAGE_MAP()

private:

    bool m_bStatsPanelDirty;   
    CBitmap m_statsPanelCache; 
    CRect   m_rectStatsPanel;  

    // ── Mouse state ───────────────────────────────────────────────────────
    int  m_nHoverX;
    bool m_bMouseTracking, rowIdx;

    // ── Cached histogram data ─────────────────────────────────────────────
    int  m_nCachedHist[256];      // Selected channel histogram
    int  m_nAllHist[3][256];      // All 3 channels: [0]=R [1]=G [2]=B
    int  m_nCachedMax;            // Max of selected channel
    int  m_nAllMax;               // Global max across all 3 channels
    int  m_nTotalPixels;

    // ── Cached statistics per channel ─────────────────────────────────────
    struct ChannelStats {
        double mean;
        double median;
        double stdDev;
        int    minVal;
        int    maxVal;
        int    peak;       // intensity value with highest count
    };
    ChannelStats m_stats[3];      // [0]=R [1]=G [2]=B

    // ── Layout ────────────────────────────────────────────────────────────
    int   m_padL, m_padR, m_padT, m_padB;
    CRect m_rectGraph;
    static const int STATS_PANEL_W = 190; // width of right stats panel

    // ── Helpers ───────────────────────────────────────────────────────────
    void ComputeAllHistograms();  // computes all 3 channels + stats
    void DrawSingleChannel(CDC& memDC, int graphW, int graphH);
    void DrawAllChannels(CDC& memDC, int graphW, int graphH);
    void DrawStatsPanel(CDC& dc, int panelX, int panelY, int panelW, int panelH);
    void DrawTooltipOverlay(CDC& memDC, int graphW, int graphH);
};
