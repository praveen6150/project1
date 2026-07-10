#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CVignetteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVignetteDlg)

public:
	CVignetteDlg(CWnd* pParent = nullptr);
	virtual ~CVignetteDlg();

	enum { IDD = IDD_VIGNETTE };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlStrengthValue;

	void UpdateLabelAndPreview();

	DECLARE_MESSAGE_MAP()
};