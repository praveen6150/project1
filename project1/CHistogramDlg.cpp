#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CHistogramDlg.h"
#include "project1View.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_HISTOGRAM, pParent)
    , m_pView(nullptr)
    , m_nCurrentChannel(0)
    , m_nHoverX(-1)
    , m_bMouseTracking(false)
    , m_nCachedMax(0)
    , m_nAllMax(0)
    , m_nTotalPixels(0)
    , m_padL(60)
    , m_padR(15)
    , m_padT(20)
    , m_padB(60)
    , m_bStatsPanelDirty(true)   // ← force first paint to draw it
{
    memset(m_nCachedHist, 0, sizeof(m_nCachedHist));
    memset(m_nAllHist, 0, sizeof(m_nAllHist));
    memset(&m_stats, 0, sizeof(m_stats));
}

CHistogramDlg::~CHistogramDlg() {}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_CTLCOLOR()        
    ON_WM_ERASEBKGND()      
    ON_BN_CLICKED(IDC_RADIO_RED, &CHistogramDlg::OnBnClickedRadioRed)
    ON_BN_CLICKED(IDC_RADIO_GREEN, &CHistogramDlg::OnBnClickedRadioGreen)
    ON_BN_CLICKED(IDC_RADIO_BLUE, &CHistogramDlg::OnBnClickedRadioBlue)
    ON_BN_CLICKED(IDC_BTN_ALL, &CHistogramDlg::OnBnClickedBtnAll)
    ON_WM_NCDESTROY()
END_MESSAGE_MAP()

// ── Init ──────────────────────────────────────────────────────────────────
BOOL CHistogramDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_brushWhite.CreateSolidBrush(RGB(255, 255, 255));
    CheckRadioButton(IDC_RADIO_RED, IDC_RADIO_BLUE, IDC_RADIO_RED);

    // ── Widen the dialog to fit stats panel ───────────────────────────────
    CRect dlgRect;
    GetWindowRect(&dlgRect);
    int newW = dlgRect.Width() + STATS_PANEL_W + 10;
    SetWindowPos(nullptr, 0, 0, newW, dlgRect.Height(),
        SWP_NOMOVE | SWP_NOZORDER);

    ComputeAllHistograms();
    return TRUE;
}

HBRUSH CHistogramDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // Make radio buttons (and optionally static text) sit on white
    if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)m_brushWhite.GetSafeHandle();
    }
    return hbr;
}

BOOL CHistogramDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);
    pDC->FillSolidRect(&rc, RGB(255, 255, 255));
    return TRUE;
}

// ── Channel switches ───────────────────────────────────────────────────────
void CHistogramDlg::OnBnClickedRadioRed()
{
    m_nCurrentChannel = 0;
    m_bStatsPanelDirty = true;
    Invalidate(FALSE);
}

void CHistogramDlg::OnBnClickedRadioGreen()
{
    m_nCurrentChannel = 1;
    m_bStatsPanelDirty = true;
    Invalidate(FALSE);
}

void CHistogramDlg::OnBnClickedRadioBlue()
{
    m_nCurrentChannel = 2;
    m_bStatsPanelDirty = true;
    Invalidate(FALSE);
}

void CHistogramDlg::OnBnClickedBtnAll()
{
    m_nCurrentChannel = 3;
    m_bStatsPanelDirty = true;
    Invalidate(FALSE);
}

// ── Compute ALL 3 channels + statistics in one pass ───────────────────────
void CHistogramDlg::ComputeAllHistograms()
{
    memset(m_nAllHist, 0, sizeof(m_nAllHist));
    m_nAllMax = 0;
    m_nTotalPixels = 0;

    if (!m_pView) return;
    Cproject1Doc* pDoc = m_pView->GetDocument();
    if (!pDoc || pDoc->m_image.IsNull()) return;

    int width = pDoc->m_image.GetWidth();
    int height = pDoc->m_image.GetHeight();
    int bpp = pDoc->m_image.GetBPP();
    BYTE* pBits = (BYTE*)pDoc->m_image.GetBits();
    int pitch = pDoc->m_image.GetPitch();
    int bytesPerPix = bpp / 8;

    m_nTotalPixels = width * height;

    // Single pass — collect all 3 channels at once
    for (int y = 0; y < height; y++)
    {
        BYTE* pRow = pBits + (y * pitch);
        for (int x = 0; x < width; x++)
        {
            BYTE* pPixel = pRow + (x * bytesPerPix);
            m_nAllHist[0][pPixel[2]]++; // Red
            m_nAllHist[1][pPixel[1]]++; // Green
            m_nAllHist[2][pPixel[0]]++; // Blue
        }
    }

    // Find global max and per-channel max
    for (int ch = 0; ch < 3; ch++)
        for (int i = 0; i < 256; i++)
            if (m_nAllHist[ch][i] > m_nAllMax)
                m_nAllMax = m_nAllHist[ch][i];

    // Copy selected channel into m_nCachedHist for tooltip use
    memcpy(m_nCachedHist, m_nAllHist[0], sizeof(m_nCachedHist));
    m_nCachedMax = 0;
    for (int i = 0; i < 256; i++)
        if (m_nCachedHist[i] > m_nCachedMax)
            m_nCachedMax = m_nCachedHist[i];

    // ── Compute statistics for each channel ───────────────────────────────
    for (int ch = 0; ch < 3; ch++)
    {
        ChannelStats& s = m_stats[ch];
        s = {};

        // Min / Max
        s.minVal = 255; s.maxVal = 0;
        for (int i = 0; i < 256; i++) {
            if (m_nAllHist[ch][i] > 0) {
                if (i < s.minVal) s.minVal = i;
                if (i > s.maxVal) s.maxVal = i;
            }
        }

        // Peak
        int chMax = 0;
        for (int i = 0; i < 256; i++)
            if (m_nAllHist[ch][i] > chMax) { chMax = m_nAllHist[ch][i]; s.peak = i; }

        // Mean
        double sum = 0;
        for (int i = 0; i < 256; i++)
            sum += (double)i * m_nAllHist[ch][i];
        s.mean = (m_nTotalPixels > 0) ? sum / m_nTotalPixels : 0.0;

        // Median
        int half = m_nTotalPixels / 2;
        int cumul = 0;
        s.median = 0;
        for (int i = 0; i < 256; i++) {
            cumul += m_nAllHist[ch][i];
            if (cumul >= half) { s.median = i; break; }
        }

        // Std Dev
        double variance = 0;
        for (int i = 0; i < 256; i++)
            variance += m_nAllHist[ch][i] * (i - s.mean) * (i - s.mean);
        s.stdDev = (m_nTotalPixels > 0)
            ? sqrt(variance / m_nTotalPixels) : 0.0;
    }
    m_bStatsPanelDirty = true;
}

// ── Mouse tracking ────────────────────────────────────────────────────────
void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseTracking)
    {
        TRACKMOUSEEVENT tme = {};
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
        m_bMouseTracking = true;
    }
    m_nHoverX = point.x;

    // Only repaint the graph area — stats panel is untouched on hover
    InvalidateRect(&m_rectGraph, FALSE);

    CDialogEx::OnMouseMove(nFlags, point);
}

void CHistogramDlg::OnMouseLeave()
{
    m_nHoverX = -1;
    m_bMouseTracking = false;
    InvalidateRect(&m_rectGraph, FALSE);   // same here — graph only
}

// ── Main paint engine ─────────────────────────────────────────────────────
void CHistogramDlg::OnPaint()
{
    CPaintDC dc(this);

    if (!m_pView) return;
    Cproject1Doc* pDoc = m_pView->GetDocument();
    if (!pDoc || pDoc->m_image.IsNull()) return;

    CWnd* pStaticGraph = GetDlgItem(IDC_STATIC_GRAPH);
    if (!pStaticGraph) return;

    // Sync cached hist for selected channel (for tooltip)
    int ch = (m_nCurrentChannel == 3) ? 0 : m_nCurrentChannel;
    memcpy(m_nCachedHist, m_nAllHist[ch], sizeof(m_nCachedHist));
    m_nCachedMax = 0;
    for (int i = 0; i < 256; i++)
        if (m_nCachedHist[i] > m_nCachedMax) m_nCachedMax = m_nCachedHist[i];

    pStaticGraph->GetWindowRect(&m_rectGraph);
    ScreenToClient(&m_rectGraph);

    int totalW = m_rectGraph.Width();
    int totalH = m_rectGraph.Height();
    int graphW = totalW - m_padL - m_padR;
    int graphH = totalH - m_padT - m_padB;

    // ── Double buffer (graph area only) ───────────────────────────────────
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, totalW, totalH);
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    CRect rectAll(0, 0, totalW, totalH);
    memDC.FillSolidRect(&rectAll, RGB(255, 255, 255));
    memDC.SetBkMode(TRANSPARENT);

    CFont fontSmall, fontAxis;
    fontSmall.CreatePointFont(82, _T("Segoe UI"));
    fontAxis.CreatePointFont(90, _T("Segoe UI"));
    CFont* pOldFont = memDC.SelectObject(&fontSmall);

    CPen gridPen(PS_DOT, 1, RGB(200, 200, 200));
    CPen* pOldPen = memDC.SelectObject(&gridPen);
    CBrush* pOldBrush = (CBrush*)memDC.SelectStockObject(NULL_BRUSH);

    for (int g = 0; g <= 4; g++)
    {
        int gridY = m_padT + (graphH * g / 4);
        memDC.MoveTo(m_padL, gridY);
        memDC.LineTo(m_padL + graphW, gridY);

        int pct = 100 - (g * 25);
        CString strY;
        strY.Format(_T("%d%%"), pct);
        memDC.SetTextColor(RGB(120, 120, 120));
        memDC.SelectObject(&fontSmall);
        CRect lr(20, gridY - 8, m_padL - 4, gridY + 8);
        memDC.DrawText(strY, &lr, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
    }

    {
        LOGFONT lf = {};
        lf.lfHeight = -13;
        lf.lfEscapement = 900; lf.lfOrientation = 900;
        lf.lfWeight = FW_NORMAL;
        _tcscpy_s(lf.lfFaceName, _T("Segoe UI"));
        CFont fr; fr.CreateFontIndirect(&lf);
        CFont* pOldRotFont = memDC.SelectObject(&fr);

        memDC.SetTextColor(RGB(90, 90, 90));

        CString strLabel = _T("Pixel Count");
        CSize textSize = memDC.GetTextExtent(strLabel);

        int labelX = 6;  // moved closer to the left edge
        int labelY = m_padT + (graphH / 2) + (textSize.cx / 2);

        memDC.TextOut(labelX, labelY, strLabel);

        memDC.SelectObject(pOldRotFont);
        memDC.SelectObject(&fontSmall);
    }

    if (m_nCurrentChannel == 3)
        DrawAllChannels(memDC, graphW, graphH);
    else
        DrawSingleChannel(memDC, graphW, graphH);

    CPen framePen(PS_SOLID, 1, RGB(110, 110, 110));
    memDC.SelectObject(&framePen);
    memDC.MoveTo(m_padL, m_padT);
    memDC.LineTo(m_padL, m_padT + graphH);
    memDC.LineTo(m_padL + graphW, m_padT + graphH);
    memDC.LineTo(m_padL + graphW, m_padT);

    const int stripGap = 4, stripH = 10;
    const int tickGap = 3, tickH = 4;
    const int tickLGap = 2, labelH = 14;
    const int titleGap = 3;

    int stripY = m_padT + graphH + stripGap;
    int tickY = stripY + stripH + tickGap;
    int labelY = tickY + tickH + tickLGap;
    int titleY = labelY + labelH + titleGap;

    for (int i = 0; i < graphW; i++)
    {
        BYTE intensity = (BYTE)((i / (double)graphW) * 255.0);
        COLORREF gc;
        if (m_nCurrentChannel == 3)
            gc = RGB(intensity, (BYTE)(128 - abs(intensity - 128)), 255 - intensity);
        else if (m_nCurrentChannel == 0) gc = RGB(intensity, 0, 0);
        else if (m_nCurrentChannel == 1) gc = RGB(0, intensity, 0);
        else                             gc = RGB(0, 0, intensity);

        CPen gp(PS_SOLID, 1, gc);
        memDC.SelectObject(&gp);
        memDC.MoveTo(m_padL + i, stripY);
        memDC.LineTo(m_padL + i, stripY + stripH);
    }

    CPen stripBorderPen(PS_SOLID, 1, RGB(110, 110, 110));
    memDC.SelectObject(&stripBorderPen);
    memDC.MoveTo(m_padL, stripY);
    memDC.LineTo(m_padL + graphW, stripY);
    memDC.LineTo(m_padL + graphW, stripY + stripH);
    memDC.LineTo(m_padL, stripY + stripH);
    memDC.LineTo(m_padL, stripY);

    int xTicks[] = { 0, 64, 128, 192, 255 };
    memDC.SelectObject(&fontSmall);
    for (int t = 0; t < 5; t++)
    {
        int tv = xTicks[t];
        int txX = m_padL + (int)((tv / 255.0) * graphW);
        CPen tp(PS_SOLID, 1, RGB(110, 110, 110));
        memDC.SelectObject(&tp);
        memDC.MoveTo(txX, tickY);
        memDC.LineTo(txX, tickY + tickH);

        CString strX; strX.Format(_T("%d"), tv);
        memDC.SetTextColor(RGB(80, 80, 80));
        CRect xlr(txX - 15, labelY, txX + 15, labelY + labelH);
        memDC.DrawText(strX, &xlr, DT_CENTER | DT_SINGLELINE);
    }

    memDC.SelectObject(&fontAxis);
    memDC.SetTextColor(RGB(0, 100, 180));
    CRect xTitleRect(m_padL, titleY, m_padL + graphW, titleY + 16);
    memDC.DrawText(_T("Pixel Intensity (0 - 255)"), &xTitleRect,
        DT_CENTER | DT_SINGLELINE);

    DrawTooltipOverlay(memDC, graphW, graphH);

    dc.BitBlt(m_rectGraph.left, m_rectGraph.top, totalW, totalH,
        &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);
    memDC.SelectObject(pOldFont);
    memDC.SelectObject(pOldBitmap);

    // ── Stats panel — only redraw when dirty, otherwise blit cached bitmap ─
   // ── Stats panel — aligned vertically with the graph plot area ─────────────
    int panelX = m_rectGraph.left + totalW + 8;
    int panelY = m_rectGraph.top + m_padT;
    int panelW = STATS_PANEL_W;
    int panelH = graphH;   // match graph plot height exactly, not totalH
    m_rectStatsPanel = CRect(panelX, panelY, panelX + panelW, panelY + panelH);

    if (m_bStatsPanelDirty || m_statsPanelCache.GetSafeHandle() == nullptr)
    {
        CDC panelDC;
        panelDC.CreateCompatibleDC(&dc);

        if (m_statsPanelCache.GetSafeHandle())
            m_statsPanelCache.DeleteObject();
        m_statsPanelCache.CreateCompatibleBitmap(&dc, panelW, panelH);
        CBitmap* pOldPanelBmp = panelDC.SelectObject(&m_statsPanelCache);

        DrawStatsPanel(panelDC, 0, 0, panelW, panelH);

        panelDC.SelectObject(pOldPanelBmp);
        m_bStatsPanelDirty = false;
    }

    CDC srcDC;
    srcDC.CreateCompatibleDC(&dc);
    CBitmap* pOldSrcBmp = srcDC.SelectObject(&m_statsPanelCache);
    dc.BitBlt(panelX, panelY, panelW, panelH, &srcDC, 0, 0, SRCCOPY);
    srcDC.SelectObject(pOldSrcBmp);
         
}


// ── Draw single channel histogram ─────────────────────────────────────────
void CHistogramDlg::DrawSingleChannel(CDC& memDC, int graphW, int graphH)
{
    COLORREF fillTheme, strokeTheme;
    if (m_nCurrentChannel == 0) { fillTheme = RGB(235, 75, 75);   strokeTheme = RGB(165, 20, 20); }
    else if (m_nCurrentChannel == 1) { fillTheme = RGB(65, 195, 115);  strokeTheme = RGB(20, 115, 55); }
    else { fillTheme = RGB(65, 145, 235);  strokeTheme = RGB(20, 75, 165); }

    // Peak marker
    if (m_nCachedMax > 0)
    {
        int peakI = 0;
        for (int i = 0; i < 256; i++)
            if (m_nCachedHist[i] == m_nCachedMax) { peakI = i; break; }
        int peakX = m_padL + (int)((peakI / 255.0) * graphW);

        CPen peakPen(PS_DOT, 1, RGB(80, 80, 80));
        memDC.SelectObject(&peakPen);
        memDC.MoveTo(peakX, m_padT);
        memDC.LineTo(peakX, m_padT + graphH);

        CFont fs; fs.CreatePointFont(82, _T("Segoe UI"));
        memDC.SelectObject(&fs);
        CString sp; sp.Format(_T("Peak: %d"), peakI);
        memDC.SetTextColor(RGB(60, 60, 60));
        CRect pr(peakX - 30, m_padT - 14, peakX + 30, m_padT);
        memDC.DrawText(sp, &pr, DT_CENTER | DT_SINGLELINE);
    }

    // Polygon
    CBrush fb(fillTheme);
    memDC.SelectObject(&fb);
    CPen sp2(PS_SOLID, 1, strokeTheme);
    memDC.SelectObject(&sp2);

    if (m_nCachedMax > 0)
    {
        CPoint pts[258];
        pts[0] = CPoint(m_padL, m_padT + graphH);
        for (int i = 0; i < 256; i++)
        {
            pts[i + 1] = CPoint(
                m_padL + (int)((i / 255.0) * graphW),
                m_padT + graphH - (int)((double)m_nCachedHist[i] / m_nCachedMax * graphH)
            );
        }
        pts[257] = CPoint(m_padL + graphW, m_padT + graphH);
        memDC.Polygon(pts, 258);
    }
}

// ── Draw all 3 channels overlaid with semi-transparency ───────────────────
void CHistogramDlg::DrawAllChannels(CDC& memDC, int graphW, int graphH)
{
    if (m_nAllMax == 0) return;

    // Channel themes
    struct ChTheme { COLORREF fill; COLORREF stroke; };
    ChTheme themes[3] = {
        { RGB(220, 60,  60),  RGB(180, 20, 20)  }, // Red
        { RGB(50,  180, 90),  RGB(20,  120, 50) }, // Green
        { RGB(50,  120, 220), RGB(20,  70,  160) }  // Blue
    };

    // For each channel, draw into a separate off-screen bitmap then AlphaBlend
    for (int ch = 0; ch < 3; ch++)
    {
        // Create off-screen layer for this channel
        CDC layerDC;
        layerDC.CreateCompatibleDC(&memDC);
        CBitmap layerBmp;
        layerBmp.CreateCompatibleBitmap(&memDC, graphW, graphH);
        CBitmap* pOldBmp = layerDC.SelectObject(&layerBmp);

        // Fill with white (will become transparent via AlphaBlend)
        layerDC.FillSolidRect(0, 0, graphW, graphH, RGB(255, 255, 255));

        // Draw filled polygon for this channel onto the layer
        CBrush lb(themes[ch].fill);
        layerDC.SelectObject(&lb);
        CPen lp(PS_SOLID, 1, themes[ch].stroke);
        layerDC.SelectObject(&lp);

        CPoint pts[258];
        pts[0] = CPoint(0, graphH);
        for (int i = 0; i < 256; i++)
        {
            pts[i + 1] = CPoint(
                (int)((i / 255.0) * graphW),
                graphH - (int)((double)m_nAllHist[ch][i] / m_nAllMax * graphH)
            );
        }
        pts[257] = CPoint(graphW, graphH);
        layerDC.Polygon(pts, 258);

        // AlphaBlend this layer onto memDC at graph position (semi-transparent)
        BLENDFUNCTION bf = {};
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 140; // 0=transparent, 255=opaque. 140≈55%
        bf.AlphaFormat = 0;

        ::AlphaBlend(
            memDC.GetSafeHdc(),
            m_padL, m_padT,        // dest x,y
            graphW, graphH,        // dest w,h
            layerDC.GetSafeHdc(),
            0, 0,                  // src x,y
            graphW, graphH,        // src w,h
            bf
        );

        layerDC.SelectObject(pOldBmp);
    }

    // ── Channel legend inside top-right of graph ──────────────────────────
    CFont fleg; fleg.CreatePointFont(72, _T("Segoe UI"));
    memDC.SelectObject(&fleg);
    memDC.SetBkMode(TRANSPARENT);

    const TCHAR* labels[3] = { _T("Red"), _T("Green"), _T("Blue") };
    COLORREF     colors[3] = { RGB(200,30,30), RGB(20,140,50), RGB(20,70,200) };

    for (int ch = 0; ch < 3; ch++)
    {
        int ly = m_padT + 6 + ch * 16;
        int lx = m_padL + graphW - 60;

        // Color swatch
        CBrush swb(colors[ch]);
        CRect  swr(lx, ly + 2, lx + 10, ly + 11);
        memDC.FillRect(&swr, &swb);

        // Label
        memDC.SetTextColor(colors[ch]);
        CRect lr(lx + 13, ly, lx + 65, ly + 14);
        memDC.DrawText(labels[ch], &lr, DT_LEFT | DT_SINGLELINE);
    }
}

// ── Stats panel ───────────────────────────────────────────────────────────
void CHistogramDlg::DrawStatsPanel(CDC& dc, int panelX, int panelY,
    int panelW, int panelH)
{
    // ── White background ────────────────────────────────────────────────
    CRect panelRect(panelX, panelY, panelX + panelW, panelY + panelH);
    dc.FillSolidRect(&panelRect, RGB(255, 255, 255));

    // ── Clean closed border (drawn last over everything via Rectangle) ───
    CPen borderPen(PS_SOLID, 1, RGB(170, 170, 170));
    CPen* pOldPen = dc.SelectObject(&borderPen);
    CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

    dc.SetBkMode(TRANSPARENT);

    CFont fTitle, fHead, fData, fDataBold;
    fTitle.CreatePointFont(95, _T("Segoe UI Semibold"));
    fHead.CreatePointFont(85, _T("Segoe UI Semibold"));
    fData.CreatePointFont(82, _T("Segoe UI"));
    fDataBold.CreatePointFont(82, _T("Segoe UI Semibold"));

    int x = panelX + 12;
    int y = panelY + 10;
    int cw = panelW - 24;

    // ── Title ────────────────────────────────────────────────────────────
    dc.SelectObject(&fTitle);
    dc.SetTextColor(RGB(30, 30, 30));
    CRect titleR(x, y, x + cw, y + 20);
    dc.DrawText(_T("Statistics"), &titleR, DT_CENTER | DT_SINGLELINE);
    y += 24;

    CPen divPen(PS_SOLID, 1, RGB(210, 210, 210));
    dc.SelectObject(&divPen);
    dc.MoveTo(x, y); dc.LineTo(x + cw, y);
    y += 10;

    // ── Selected channel section ────────────────────────────────────────
    int selCh = (m_nCurrentChannel == 3) ? 0 : m_nCurrentChannel;
    const TCHAR* chNames[3] = { _T("Red"), _T("Green"), _T("Blue") };
    COLORREF chColors[3] = { RGB(190,0,0), RGB(0,130,40), RGB(0,60,180) };

    dc.SelectObject(&fHead);
    dc.SetTextColor(chColors[selCh]);
    CString selTitle;
    selTitle.Format(_T("[ %s Channel(s) ]"),
        (m_nCurrentChannel == 3) ? _T("All") : chNames[selCh]);
    CRect selTR(x, y, x + cw, y + 17);
    dc.DrawText(selTitle, &selTR, DT_CENTER | DT_SINGLELINE);
    y += 22;

    ChannelStats& ss = m_stats[selCh];
    struct StatRow { const TCHAR* label; CString value; };

    CString sMean, sMedian, sStdDev, sMin, sMax, sPeak;
    sMean.Format(_T("%.1f"), ss.mean);
    sMedian.Format(_T("%.0f"), ss.median);
    sStdDev.Format(_T("%.1f"), ss.stdDev);
    sMin.Format(_T("%d"), ss.minVal);
    sMax.Format(_T("%d"), ss.maxVal);
    sPeak.Format(_T("%d"), ss.peak);

    StatRow rows[] = {
        { _T("Mean"),    sMean   },
        { _T("Median"),  sMedian },
        { _T("Std Dev"), sStdDev },
        { _T("Min"),     sMin    },
        { _T("Max"),     sMax    },
        { _T("Peak"),    sPeak   },
    };

    int rowIdx = 0;
    for (auto& r : rows)
    {
        if (rowIdx++ % 2 == 0)
        {
            CRect rowBg(x - 4, y, x + cw + 4, y + 17);
            dc.FillSolidRect(&rowBg, RGB(245, 247, 250));
        }

        dc.SelectObject(&fData);
        dc.SetTextColor(RGB(90, 90, 90));
        CRect lR(x, y, x + 65, y + 17);
        dc.DrawText(r.label, &lR, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        dc.SelectObject(&fDataBold);
        dc.SetTextColor(RGB(20, 20, 20));
        CRect vR(x + 65, y, x + cw, y + 17);
        dc.DrawText(r.value, &vR, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
        y += 18;
    }

    y += 6;
    dc.SelectObject(&divPen);
    dc.MoveTo(x, y); dc.LineTo(x + cw, y);
    y += 10;

    // ── All channels comparison table ──────────────────────────────────
    dc.SelectObject(&fHead);
    dc.SetTextColor(RGB(50, 50, 50));
    CRect allTR(x, y, x + cw, y + 17);
    dc.DrawText(_T("All Channels"), &allTR, DT_CENTER | DT_SINGLELINE);
    y += 22;

    dc.SelectObject(&fData);
    dc.SetTextColor(RGB(110, 110, 110));
    int c1 = x, c2 = x + 36, c3 = x + 75,
        c4 = x + 113, c5 = x + 142, c6 = x + cw;

    CRect hR0(c1, y, c2, y + 15); dc.DrawText(_T("Ch"), &hR0, DT_LEFT | DT_SINGLELINE);
    CRect hR1(c2, y, c3, y + 15); dc.DrawText(_T("Mean"), &hR1, DT_LEFT | DT_SINGLELINE);
    CRect hR2(c3, y, c4, y + 15); dc.DrawText(_T("Med"), &hR2, DT_LEFT | DT_SINGLELINE);
    CRect hR3(c4, y, c5, y + 15); dc.DrawText(_T("Std"), &hR3, DT_LEFT | DT_SINGLELINE);
    CRect hR4(c5, y, c6, y + 15); dc.DrawText(_T("Peak"), &hR4, DT_LEFT | DT_SINGLELINE);
    y += 16;

    dc.SelectObject(&divPen);
    dc.MoveTo(x, y); dc.LineTo(x + cw, y);
    y += 5;

    for (int ch = 0; ch < 3; ch++)
    {
        ChannelStats& cs = m_stats[ch];

        if (ch % 2 == 0)
        {
            CRect rb(x - 4, y, x + cw + 4, y + 17);
            dc.FillSolidRect(&rb, RGB(245, 247, 250));
        }

        dc.SelectObject(&fDataBold);
        dc.SetTextColor(chColors[ch]);
        CRect r0(c1, y, c2, y + 16);
        dc.DrawText(chNames[ch], &r0, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        dc.SelectObject(&fData);
        dc.SetTextColor(RGB(40, 40, 40));

        CString v1, v2, v3, v4;
        v1.Format(_T("%.1f"), cs.mean);
        v2.Format(_T("%.0f"), cs.median);
        v3.Format(_T("%.1f"), cs.stdDev);
        v4.Format(_T("%d"), cs.peak);

        CRect r1(c2, y, c3, y + 16); dc.DrawText(v1, &r1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        CRect r2(c3, y, c4, y + 16); dc.DrawText(v2, &r2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        CRect r3(c4, y, c5, y + 16); dc.DrawText(v3, &r3, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        CRect r4(c5, y, c6, y + 16); dc.DrawText(v4, &r4, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        y += 18;
    }

    // ── Final clean border, drawn LAST so nothing overlaps it ────────────
    dc.SelectObject(&borderPen);
    dc.Rectangle(&panelRect);

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

// ── Crosshair + tooltip overlay ───────────────────────────────────────────
void CHistogramDlg::DrawTooltipOverlay(CDC& memDC, int graphW, int graphH)
{
    if (m_nHoverX < 0) return;

    int localX = m_nHoverX - m_rectGraph.left - m_padL;
    if (localX < 0 || localX > graphW) return;

    int intensity = (int)((localX / (double)graphW) * 255.0);
    intensity = max(0, min(255, intensity));

    int crossX = m_padL + (int)((intensity / 255.0) * graphW);

    // Vertical crosshair
    CPen crossPen(PS_SOLID, 1, RGB(40, 40, 40));
    CPen* pOldPen = memDC.SelectObject(&crossPen);
    memDC.MoveTo(crossX, m_padT);
    memDC.LineTo(crossX, m_padT + graphH);

    // ── Build tooltip text ────────────────────────────────────────────────
    CString strTip;

    if (m_nCurrentChannel == 3)
    {
        // All channels mode: show R, G, B counts at this intensity
        int rCount = m_nAllHist[0][intensity];
        int gCount = m_nAllHist[1][intensity];
        int bCount = m_nAllHist[2][intensity];
        double rPct = m_nTotalPixels > 0 ? rCount / (double)m_nTotalPixels * 100.0 : 0;
        double gPct = m_nTotalPixels > 0 ? gCount / (double)m_nTotalPixels * 100.0 : 0;
        double bPct = m_nTotalPixels > 0 ? bCount / (double)m_nTotalPixels * 100.0 : 0;

        strTip.Format(
            _T("Intensity : %d\n")
            _T("R Count   : %d (%.2f%%)\n")
            _T("G Count   : %d (%.2f%%)\n")
            _T("B Count   : %d (%.2f%%)"),
            intensity,
            rCount, rPct,
            gCount, gPct,
            bCount, bPct
        );

        // Draw 3 dots (one per channel)
        COLORREF dotColors[3] = { RGB(220,50,50), RGB(50,180,80), RGB(50,100,220) };
        for (int ch = 0; ch < 3; ch++)
        {
            int cnt = m_nAllHist[ch][intensity];
            int dotY = m_nAllMax > 0
                ? m_padT + graphH - (int)((double)cnt / m_nAllMax * graphH)
                : m_padT + graphH;
            CBrush db(dotColors[ch]);
            CPen   dp(PS_SOLID, 2, RGB(20, 20, 20));
            memDC.SelectObject(&dp);
            CBrush* ob = memDC.SelectObject(&db);
            memDC.Ellipse(crossX - 4, dotY - 4, crossX + 4, dotY + 4);
            memDC.SelectObject(ob);
        }
    }
    else
    {
        // Single channel mode
        int pixelCount = m_nCachedHist[intensity];
        double pct = m_nTotalPixels > 0
            ? pixelCount / (double)m_nTotalPixels * 100.0 : 0.0;

        strTip.Format(
            _T("Intensity : %d\nCount     : %d\nPercent   : %.2f%%"),
            intensity, pixelCount, pct
        );

        // Single dot
        int dotY = m_nCachedMax > 0
            ? m_padT + graphH - (int)((double)pixelCount / m_nCachedMax * graphH)
            : m_padT + graphH;
        CBrush db(RGB(255, 255, 255));
        CPen   dp(PS_SOLID, 2, RGB(30, 30, 30));
        memDC.SelectObject(&dp);
        CBrush* ob = memDC.SelectObject(&db);
        memDC.Ellipse(crossX - 4, dotY - 4, crossX + 4, dotY + 4);
        memDC.SelectObject(ob);
    }

    memDC.SelectObject(pOldPen);

    // ── Tooltip box ───────────────────────────────────────────────────────
    CFont fontTip;
    fontTip.CreatePointFont(85, _T("Segoe UI"));
    CFont* pOldFont = memDC.SelectObject(&fontTip);
    memDC.SetBkMode(TRANSPARENT);

    CRect measureRect(0, 0, 220, 200);
    memDC.DrawText(strTip, &measureRect, DT_CALCRECT | DT_LEFT);
    int tipW = measureRect.Width() + 16;
    int tipH = measureRect.Height() + 12;

    int tipX = crossX + 8;
    int tipY = m_padT + 8;
    if (tipX + tipW > m_padL + graphW) tipX = crossX - tipW - 8;

    CRect tipRect(tipX, tipY, tipX + tipW, tipY + tipH);

    CRect shadowRect(tipRect); shadowRect.OffsetRect(2, 2);
    CBrush shadowBrush(RGB(180, 180, 180));
    memDC.FillRect(&shadowRect, &shadowBrush);

    CBrush tipBgBrush(RGB(255, 255, 230));
    memDC.FillRect(&tipRect, &tipBgBrush);

    CPen tipBorderPen(PS_SOLID, 1, RGB(100, 100, 100));
    memDC.SelectObject(&tipBorderPen);
    memDC.MoveTo(tipRect.left, tipRect.top);
    memDC.LineTo(tipRect.right, tipRect.top);
    memDC.LineTo(tipRect.right, tipRect.bottom);
    memDC.LineTo(tipRect.left, tipRect.bottom);
    memDC.LineTo(tipRect.left, tipRect.top);

    CRect textRect(tipRect);
    textRect.DeflateRect(8, 6);
    memDC.SetTextColor(RGB(30, 30, 30));
    memDC.DrawText(strTip, &textRect, DT_LEFT | DT_TOP);

    memDC.SelectObject(pOldFont);
}

void CHistogramDlg::OnNcDestroy()
{
    CDialogEx::OnNcDestroy();
    if (m_pView != nullptr)
        m_pView->m_pHistDlg = nullptr;
    delete this;
}
