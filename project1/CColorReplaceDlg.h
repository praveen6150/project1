#pragma once

#include "afxdialogex.h"
#include <afxcmn.h> // Guaranteed compilation of CSliderCtrl

class Cproject1View; // Clean forward declaration

class CColorReplaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorReplaceDlg)

public:
	// Constructors & Destructors
	CColorReplaceDlg(CWnd* pParent = nullptr);
	virtual ~CColorReplaceDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLORREPLACE };
#endif

	// Public Setup & Interface Methods
	void SetTargetView(Cproject1View* pView);
	void OnEyedropperColorPicked(COLORREF pickedColor);

	// Public Member Variables (Accessible by outside classes)
	COLORREF m_targetColor;
	COLORREF m_replaceColor;
	int m_tolerance;   // 0 - 150
	Cproject1View* m_pTargetView; // FIXED: Now fully public so SetTargetView and other callers work safely

protected:
	// Overrides
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	// Message Map Handlers
	afx_msg void OnBnClickedButtonEyedropperTarget();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTargetColor();
	afx_msg void OnBnClickedButtonReplaceColor();

	// Protected UI Controls
	CSliderCtrl m_sliderTolerance;

	// Internal Helpers
	void UpdateLive();
	void UpdateButtonColors();

	DECLARE_MESSAGE_MAP()
};
