#include "pch.h"
#include "CCurveControl.h"
#include <algorithm>

BEGIN_MESSAGE_MAP(CCurveControl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CCurveControl::CCurveControl()
	: m_draggedPointIndex(-1)
	, m_changeCallback(nullptr)
	, m_callbackContext(nullptr)
{
	ResetCurve();
}

CCurveControl::~CCurveControl()
{
}

void CCurveControl::ResetCurve()
{
	m_points.clear();
	m_points.push_back(CPoint(0, 0));     // bottom-left: black stays black
	m_points.push_back(CPoint(255, 255)); // top-right: white stays white
}

void CCurveControl::SetChangeCallback(void (*callback)(void* context), void* context)
{
	m_changeCallback = callback;
	m_callbackContext = context;
}

void CCurveControl::NotifyChange()
{
	if (m_changeCallback)
		m_changeCallback(m_callbackContext);
}

CPoint CCurveControl::ClientToCurve(CPoint clientPt)
{
	CRect rect;
	GetClientRect(&rect);
	int stripThickness = 12;

	int plotWidth = rect.Width() - stripThickness;
	int plotHeight = rect.Height() - stripThickness;

	int cx = (int)((double)(clientPt.x - stripThickness) / plotWidth * 255.0);
	int cy = (int)((1.0 - (double)clientPt.y / plotHeight) * 255.0);

	cx = (std::max)(0, (std::min)(cx, 255));
	cy = (std::max)(0, (std::min)(cy, 255));

	return CPoint(cx, cy);
}

CPoint CCurveControl::CurveToClient(CPoint curvePt)
{
	CRect rect;
	GetClientRect(&rect);
	int stripThickness = 12;

	int plotWidth = rect.Width() - stripThickness;
	int plotHeight = rect.Height() - stripThickness;

	int px = stripThickness + (int)((double)curvePt.x / 255.0 * plotWidth);
	int py = (int)((1.0 - (double)curvePt.y / 255.0) * plotHeight);

	return CPoint(px, py);
}

int CCurveControl::FindNearbyPointIndex(CPoint clientPt)
{
	for (size_t i = 0; i < m_points.size(); i++)
	{
		CPoint screenPt = CurveToClient(m_points[i]);
		int dx = screenPt.x - clientPt.x;
		int dy = screenPt.y - clientPt.y;
		if ((dx * dx + dy * dy) <= (HIT_RADIUS * HIT_RADIUS))
			return (int)i;
	}
	return -1;
}

void CCurveControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int hitIndex = FindNearbyPointIndex(point);

	if (hitIndex >= 0)
	{
		// Grabbed an existing point to drag
		m_draggedPointIndex = hitIndex;
	}
	else
	{
		// Add a new point at the clicked location, keeping the list sorted by x
		CPoint curvePt = ClientToCurve(point);

		size_t insertPos = 0;
		while (insertPos < m_points.size() && m_points[insertPos].x < curvePt.x)
			insertPos++;

		m_points.insert(m_points.begin() + insertPos, curvePt);
		m_draggedPointIndex = (int)insertPos;
	}

	SetCapture(); // ensures we keep getting mouse messages even if cursor leaves the control
	Invalidate();
	NotifyChange();

	CStatic::OnLButtonDown(nFlags, point);
}

void CCurveControl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_draggedPointIndex >= 0 && (nFlags & MK_LBUTTON))
	{
		CPoint curvePt = ClientToCurve(point);

		// Don't let the endpoints (first/last) be dragged past their neighbors,
		// and don't let any point cross over an adjacent one on the x-axis
		int minX = (m_draggedPointIndex == 0) ? 0 : m_points[m_draggedPointIndex - 1].x + 1;
		int maxX = (m_draggedPointIndex == (int)m_points.size() - 1) ? 255 : m_points[m_draggedPointIndex + 1].x - 1;

		if (m_draggedPointIndex == 0) minX = maxX = 0;                          // lock left endpoint's x
		if (m_draggedPointIndex == (int)m_points.size() - 1) minX = maxX = 255; // lock right endpoint's x

		// Secure boundaries on both axes using the macro-safe std variant
		//curvePt.x = std::clamp(curvePt.x, (std::min)(minX, maxX), (std::max)(minX, maxX));
		//curvePt.y = std::clamp(curvePt.y, 0, 255);

		curvePt.x = std::clamp(curvePt.x, static_cast<long>((std::min)(minX, maxX)), static_cast<long>((std::max)(minX, maxX)));
		curvePt.y = std::clamp(curvePt.y, 0L, 255L); // The 'L' suffix instantly makes 0 and 255 a literal long

		m_points[m_draggedPointIndex] = curvePt;

		Invalidate();
		NotifyChange();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CCurveControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_draggedPointIndex >= 0)
	{
		ReleaseCapture();
		m_draggedPointIndex = -1;
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CCurveControl::OnPaint()
{
	// Force the underlying Win32 control to accept and route mouse clicks
	ModifyStyle(0, SS_NOTIFY);

	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	// Space at the bottom and left for our visual color gradient strips
	int stripThickness = 12;
	CRect plotRect = rect;
	plotRect.bottom -= stripThickness;
	plotRect.left += stripThickness;

	// 1. RECTIFIED: CLEAN WHITE CANVAS
	dc.FillSolidRect(&plotRect, RGB(255, 255, 255));

	// 2. HORIZONTAL INPUT GRADIENT STRIP (Bottom: Black to White)
	for (int x = plotRect.left; x < plotRect.right; x++)
	{
		int intensity = (int)(((double)(x - plotRect.left) / plotRect.Width()) * 255.0);
		intensity = (std::max)(0, (std::min)(intensity, 255));
		dc.FillSolidRect(x, plotRect.bottom, 1, stripThickness, RGB(intensity, intensity, intensity));
	}

	// 3. VERTICAL OUTPUT GRADIENT STRIP (Left: Black to White going up)
	for (int y = plotRect.top; y < plotRect.bottom; y++)
	{
		int intensity = (int)((1.0 - (double)(y - plotRect.top) / plotRect.Height()) * 255.0);
		intensity = (std::max)(0, (std::min)(intensity, 255));
		dc.FillSolidRect(0, y, stripThickness, 1, RGB(intensity, intensity, intensity));
	}

	// 4. SOFT NEUTRAL GRAY GRID LINES (Perfect visibility on white)
	CPen gridPen(PS_SOLID, 1, RGB(220, 220, 220));
	CPen* pOldPen = dc.SelectObject(&gridPen);
	for (int i = 32; i < 255; i += 32)
	{
		// Vertical grid lines
		int px = plotRect.left + (int)((double)i / 255.0 * plotRect.Width());
		dc.MoveTo(px, plotRect.top);
		dc.LineTo(px, plotRect.bottom);

		// Horizontal grid lines
		int py = plotRect.top + (int)((1.0 - (double)i / 255.0) * plotRect.Height());
		dc.MoveTo(plotRect.left, py);
		dc.LineTo(plotRect.right, py);
	}
	dc.SelectObject(pOldPen);

	// 5. SEAMLESS DASHED REFERENCE LINE
	CPen refPen(PS_DOT, 1, RGB(160, 160, 160));
	pOldPen = dc.SelectObject(&refPen);
	int xStart = plotRect.left;
	int yStart = plotRect.bottom;
	int xEnd = plotRect.right;
	int yEnd = plotRect.top;
	dc.MoveTo(xStart, yStart);
	dc.LineTo(xEnd, yEnd);
	dc.SelectObject(pOldPen);

	// 6. DEEP ROYAL BLUE TONE CURVE (Stands out sharply on a white grid)
	BYTE lut[256];
	GetLUT(lut);

	CPen curvePen(PS_SOLID, 3, RGB(0, 102, 204));
	pOldPen = dc.SelectObject(&curvePen);

	for (int i = 0; i < 256; i++)
	{
		int px = plotRect.left + (int)((double)i / 255.0 * plotRect.Width());
		int py = plotRect.top + (int)((1.0 - (double)lut[i] / 255.0) * plotRect.Height());

		if (i == 0)
			dc.MoveTo(px, py);
		else
			dc.LineTo(px, py);
	}
	dc.SelectObject(pOldPen);

	// 7. VIBRANT RED HANDLE POINTS WITH BORDERS
	for (size_t i = 0; i < m_points.size(); i++)
	{
		int px = plotRect.left + (int)((double)m_points[i].x / 255.0 * plotRect.Width());
		int py = plotRect.top + (int)((1.0 - (double)m_points[i].y / 255.0) * plotRect.Height());

		CBrush brush(RGB(255, 50, 50)); // Classic crisp red handle dots
		CBrush* pOldBrush = dc.SelectObject(&brush);

		// Drawing a neat circle handle
		CPen borderPen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen* pOldBpen = dc.SelectObject(&borderPen);

		dc.Ellipse(px - 5, py - 5, px + 5, py + 5);

		dc.SelectObject(pOldBpen);
		dc.SelectObject(pOldBrush);
	}
}

// Catmull-Rom spline interpolation through the user's control points,
// producing a smooth full 256-entry lookup table.
void CCurveControl::GetLUT(BYTE lut[256])
{
	if (m_points.size() < 2)
	{
		for (int i = 0; i < 256; i++) lut[i] = (BYTE)i;
		return;
	}

	for (int x = 0; x < 256; x++)
	{
		int k = 0;
		while (k < (int)m_points.size() - 2 && m_points[k + 1].x < x)
			k++;

		int i0 = (std::max)(k - 1, 0);
		int i1 = k;
		int i2 = (std::min)(k + 1, (int)m_points.size() - 1);
		int i3 = (std::min)(k + 2, (int)m_points.size() - 1);

		CPoint p0 = m_points[i0];
		CPoint p1 = m_points[i1];
		CPoint p2 = m_points[i2];
		CPoint p3 = m_points[i3];

		double segWidth = (double)(p2.x - p1.x);
		double t = (segWidth > 0.0) ? (x - p1.x) / segWidth : 0.0;
		t = (std::max)(0.0, (std::min)(t, 1.0));

		double t2 = t * t;
		double t3 = t2 * t;

		double y = 0.5 * (
			(2.0 * p1.y) +
			(-p0.y + p2.y) * t +
			(2.0 * p0.y - 5.0 * p1.y + 4.0 * p2.y - p3.y) * t2 +
			(-p0.y + 3.0 * p1.y - 3.0 * p2.y + p3.y) * t3
			);

		y = (std::max)(0.0, (std::min)(y, 255.0));
		lut[x] = (BYTE)(y + 0.5);
	}
}
