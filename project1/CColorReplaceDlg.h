#pragma once
#include "afxdialogex.h"
#include <afxcmn.h>

class Cproject1View;

class CColorReplaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorReplaceDlg)

public:
	CColorReplaceDlg(CWnd* pParent = nullptr);
	virtual ~CColorReplaceDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLORREPLACE };
#endif

	void SetTargetView(Cproject1View* pView);

	// Called by the View's hook whenever the mouse hovers or clicks the canvas
	void OnHoverColorUpdate(COLORREF color);
	void OnEyedropperColorPicked(COLORREF color);

	COLORREF m_targetColor;
	COLORREF m_replaceColor;
	int m_tolerance;
	Cproject1View* m_pTargetView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonReplaceColor();

	CSliderCtrl m_sliderTolerance;

	void UpdateTargetLabel();
	void UpdateReplaceLabel();
	void UpdateLive();

	DECLARE_MESSAGE_MAP()
};

