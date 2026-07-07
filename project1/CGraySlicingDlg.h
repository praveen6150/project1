#pragma once
#include "afxdialogex.h"

// Forward declaration
class Cproject1View;

class CGraySlicingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraySlicingDlg)

public:
	CGraySlicingDlg(CWnd* pParent = nullptr);
	virtual ~CGraySlicingDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAY_SLICING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckPreserveBg();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()

public:
	CSliderCtrl m_ctrlSliderLow;
	CSliderCtrl m_ctrlSliderHigh;
	CStatic m_ctrlStaticLabel;
	BOOL m_bPreserveBG;

	Cproject1View* m_pView; // View pointer for live updates

	// Inline Getters for range tracking
	int GetLowValue() { return m_ctrlSliderLow.GetPos(); }
	int GetHighValue() { return m_ctrlSliderHigh.GetPos(); }
	BOOL ShouldPreserveBG() { return m_bPreserveBG; }
};

