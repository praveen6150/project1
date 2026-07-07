#pragma once
#include "afxdialogex.h"

// Forward declaration
class Cproject1View;

class CPseudoColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPseudoColorDlg)

public:
	CPseudoColorDlg(CWnd* pParent = nullptr);
	virtual ~CPseudoColorDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSEUDO_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboColorMap();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

protected:
	CStatic   m_ctrlSliderValue;

public:
	CComboBox m_ctrlComboMap;
	Cproject1View* m_pView; // View pointer for live updates

	// Inline getter to retrieve current map index (0 = Jet, 1 = Hot, 2 = Cool)
	int GetSelectedMapIndex() { return m_ctrlComboMap.GetCurSel(); }

	int GetSliderValue() {
		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_COLOR_SHIFT);
		return pSlider ? pSlider->GetPos() : 0;
	}
};