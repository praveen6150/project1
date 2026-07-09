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

	// Curve space: x=0 is left edge, x=255 is right edge
	//              y=0 is BOTTOM edge, y=255 is TOP edge (graphs read bottom-up)
	int cx = (int)((double)clientPt.x / rect.Width() * 255.0);
	int cy = (int)((1.0 - (double)clientPt.y / rect.Height()) * 255.0);

	cx = (std::max)(0, (std::min)(cx, 255));
	cy = (std::max)(0, (std::min)(cy, 255));

	return CPoint(cx, cy);
}

CPoint CCurveControl::CurveToClient(CPoint curvePt)
{
	CRect rect;
	GetClientRect(&rect);

	int px = (int)((double)curvePt.x / 255.0 * rect.Width());
	int py = (int)((1.0 - (double)curvePt.y / 255.0) * rect.Height());

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

	// 1. SMOOTH BACKGROUND GRADIENT
	for (int y = 0; y < rect.Height(); y++)
	{
		int intensity = (int)((1.0 - (double)y / rect.Height()) * 255.0);
		intensity = (std::max)(0, (std::min)(intensity, 255));

		COLORREF gradColor = RGB(intensity, intensity, intensity);
		dc.FillSolidRect(0, y, rect.Width(), 1, gradColor);
	}

	// 2. GRID LINES
	CPen gridPen(PS_SOLID, 1, RGB(128, 128, 128));
	CPen* pOldPen = dc.SelectObject(&gridPen);
	for (int i = 0; i <= 255; i += 32)
	{
		CPoint top = CurveToClient(CPoint(i, 255));
		dc.MoveTo(top.x, 0);
		dc.LineTo(top.x, rect.Height());

		CPoint left = CurveToClient(CPoint(0, i));
		dc.MoveTo(0, left.y);
		dc.LineTo(rect.Width(), left.y);
	}
	dc.SelectObject(pOldPen);

	// 3. DIAGONAL REFERENCE LINE
	CPen refPen(PS_DOT, 1, RGB(255, 128, 0));
	pOldPen = dc.SelectObject(&refPen);
	CPoint refStart = CurveToClient(CPoint(0, 0));
	CPoint refEnd = CurveToClient(CPoint(255, 255));
	dc.MoveTo(refStart);
	dc.LineTo(refEnd);
	dc.SelectObject(pOldPen);

	// 4. DRAW INTERPOLATED CURVE
	BYTE lut[256];
	GetLUT(lut);

	CPen curvePen(PS_SOLID, 3, RGB(0, 220, 255));
	pOldPen = dc.SelectObject(&curvePen);
	CPoint prevPt = CurveToClient(CPoint(0, lut[0]));
	dc.MoveTo(prevPt);
	for (int i = 1; i < 256; i++)
	{
		CPoint pt = CurveToClient(CPoint(i, lut[i]));
		dc.LineTo(pt);
	}
	dc.SelectObject(pOldPen);

	// 5. DRAW CONTROL HANDLE POINTS
	for (size_t i = 0; i < m_points.size(); i++)
	{
		CPoint screenPt = CurveToClient(m_points[i]);
		CBrush brush(RGB(255, 230, 0));
		CBrush* pOldBrush = dc.SelectObject(&brush);
		dc.Ellipse(screenPt.x - 5, screenPt.y - 5, screenPt.x + 5, screenPt.y + 5);
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
