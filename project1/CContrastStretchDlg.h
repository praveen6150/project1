#pragma once
#include "afxdialogex.h"


// CContrastStretchDlg dialog

class CContrastStretchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CContrastStretchDlg)

public:
	CContrastStretchDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CContrastStretchDlg();
	int m_nMin;
	int m_nMax;
	CSliderCtrl m_sliderMin;
	CSliderCtrl m_sliderMax;

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
