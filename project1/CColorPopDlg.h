#pragma once
#include "afxdialogex.h"
#include <afxcmn.h>

class Cproject1View;

class CColorPopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorPopDlg)

public:
	CColorPopDlg(CWnd* pParent = nullptr);
	virtual ~CColorPopDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLORPOP };
#endif

	void SetTargetView(Cproject1View* pView);

	void OnHoverColorUpdate(COLORREF color);
	void OnEyedropperColorPicked(COLORREF color);

	COLORREF m_targetColor;
	int m_hueTolerance;   // 0 - 180
	Cproject1View* m_pTargetView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CSliderCtrl m_sliderTolerance;

	void UpdateTargetLabel();
	void UpdateLive();

	DECLARE_MESSAGE_MAP()
};
