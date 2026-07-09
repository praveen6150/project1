#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CSigmoidContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSigmoidContrastDlg)

public:
	CSigmoidContrastDlg(CWnd* pParent = nullptr);
	virtual ~CSigmoidContrastDlg();

	enum { IDD = IDD_SIGMOID_CONTRAST };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlGainValue;
	CStatic m_ctrlMidpointValue;

	void UpdateLabelsAndPreview();

	DECLARE_MESSAGE_MAP()
};