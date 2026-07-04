#pragma once
#include "afxdialogex.h"

class Cproject1View;

class CBrightnessContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessContrastDlg)

public:
	CBrightnessContrastDlg(CWnd* pParent = nullptr);
	virtual ~CBrightnessContrastDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// CRITICAL MISSING DECLARATION: This tells MFC to run your custom setup code
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()

public:
	int m_nBrightness;
	int m_nContrast;
	Cproject1View* m_pView;

	CSliderCtrl m_sliderBrightCtrl;
	CSliderCtrl m_sliderContrastCtrl;
};