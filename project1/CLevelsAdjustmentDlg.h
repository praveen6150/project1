#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CLevelsAdjustmentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLevelsAdjustmentDlg)

public:
	CLevelsAdjustmentDlg(CWnd* pParent = nullptr);
	virtual ~CLevelsAdjustmentDlg();

	enum { IDD = IDD_LEVELS_ADJUSTMENT };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlBlackValue;
	CStatic m_ctrlWhiteValue;
	CStatic m_ctrlGammaValue;

	void UpdateLabelsAndPreview();

	DECLARE_MESSAGE_MAP()
};