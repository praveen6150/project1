#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CDuotoneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDuotoneDlg)

public:
	CDuotoneDlg(CWnd* pParent = nullptr);
	virtual ~CDuotoneDlg();

	enum { IDD = IDD_DUOTONE };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDuotoneShadow();
	afx_msg void OnBnClickedButtonDuotoneHighlight();

	CStatic m_ctrlShadowSwatch;
	CStatic m_ctrlHighlightSwatch;

	COLORREF m_shadowColor;
	COLORREF m_highlightColor;

	void PaintSwatch(CStatic& swatchCtrl, COLORREF color);
	void UpdatePreview();

	DECLARE_MESSAGE_MAP()
};
