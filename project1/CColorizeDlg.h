#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CColorizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorizeDlg)

public:
	CColorizeDlg(CWnd* pParent = nullptr);
	virtual ~CColorizeDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLORIZE };
#endif

	void SetTargetView(Cproject1View* pView);

	int m_hue;   // 0 - 360
	int m_sat;   // 0 - 100

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderHue;
	CSliderCtrl m_sliderSat;

	Cproject1View* m_pTargetView = nullptr;

	DECLARE_MESSAGE_MAP()
};