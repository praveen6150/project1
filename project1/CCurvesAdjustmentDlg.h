#pragma once
#include "afxdialogex.h"
#include "CCurveControl.h"

class Cproject1View; // forward declaration

class CCurvesAdjustmentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurvesAdjustmentDlg)

public:
	CCurvesAdjustmentDlg(CWnd* pParent = nullptr);
	virtual ~CCurvesAdjustmentDlg();

	enum { IDD = IDD_CURVES_ADJUSTMENT };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonResetCurve();

	CCurveControl m_curveControl;

	// Static callback bridge (C-style function pointer, required since
	// CCurveControl can't hold a direct pointer-to-member-function)
	static void OnCurveChanged(void* context);
	void ApplyCurrentCurve();

	DECLARE_MESSAGE_MAP()
};
