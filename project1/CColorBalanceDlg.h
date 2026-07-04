#pragma once
#include "afxdialogex.h"


// CColorBalanceDlg dialog

class CColorBalanceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorBalanceDlg)

public:
	CColorBalanceDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CColorBalanceDlg();
	int m_nRed;   // -100 to 100
	int m_nGreen; // -100 to 100
	int m_nBlue;  // -100 to 100

	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_POINTPROCESS_COLORBALANCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void UpdateLabels();
	DECLARE_MESSAGE_MAP()
};
