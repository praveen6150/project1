#pragma once
#include "afxdialogex.h"


// CPosterizationDlg dialog

class CPosterizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPosterizationDlg)

public:
	CPosterizationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPosterizationDlg();
	int m_nLevels; // Holds the selected number of tone levels (2 to 64)
	CSliderCtrl m_sliderLevels; // Tracks the slider control UI element

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};
