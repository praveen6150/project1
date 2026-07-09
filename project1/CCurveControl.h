#pragma once

#include <afxwin.h>
#include <vector>

// A custom control that draws an editable tone curve and lets the user
// click to add points, or drag existing points to reshape the curve.[cite: 1]
class CCurveControl : public CStatic
{
public:
	CCurveControl();
	virtual ~CCurveControl();

	// Returns the current curve as a full 256-entry lookup table,
	// built by Catmull-Rom spline interpolation through the user's points.[cite: 1]
	void GetLUT(BYTE lut[256]);

	// Resets the curve back to a straight diagonal line (identity mapping)[cite: 1]
	void ResetCurve();

	// Called by the dialog whenever the curve changes, so it can trigger a live preview[cite: 1]
	void SetChangeCallback(void (*callback)(void* context), void* context);

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	// Control points stored in "curve space": x = input (0-255), y = output (0-255)[cite: 1]
	std::vector<CPoint> m_points;

	int m_draggedPointIndex; // -1 if nothing is being dragged[cite: 1]

	// Using static constexpr prevents old-school C++ declaration linker collisions[cite: 1]
	static constexpr int HIT_RADIUS = 8; // pixels, how close a click must be to grab a point[cite: 1]

	void (*m_changeCallback)(void* context);
	void* m_callbackContext;

	// Converts a client-area pixel coordinate to curve-space (0-255, 0-255)[cite: 1]
	CPoint ClientToCurve(CPoint clientPt);

	// Converts curve-space (0-255, 0-255) to a client-area pixel coordinate[cite: 1]
	CPoint CurveToClient(CPoint curvePt);

	int FindNearbyPointIndex(CPoint clientPt);

	void NotifyChange();
};
