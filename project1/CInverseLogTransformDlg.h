#pragma once
#include "afxdialogex.h"
#include <cmath>

// Forward declaration
class Cproject1View;

class CInverseLogTransformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInverseLogTransformDlg)

public:
	CInverseLogTransformDlg(CWnd* pParent = nullptr);
	virtual ~CInverseLogTransformDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INVERSE_LOG_TRANSFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckInvAutoCalc();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()

public:
	CSliderCtrl m_ctrlSliderInvC;
	CStatic m_ctrlStaticInvCLabel;
	BOOL m_bInvAutoCalc;

	Cproject1View* m_pView; // View pointer for live preview

	// Getter for scale constant
	double GetConstantC() {
		// If auto-calculated, return an ideal scaling base, else pull from slider position
		return m_bInvAutoCalc ? 255.0 / (std::exp(1.0) - 1.0) : (m_ctrlSliderInvC.GetPos() / 10.0);
	}
};