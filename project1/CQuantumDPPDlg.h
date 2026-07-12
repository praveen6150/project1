#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CQuantumDPPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQuantumDPPDlg)

public:
	CQuantumDPPDlg(CWnd* pParent = nullptr);
	virtual ~CQuantumDPPDlg();

	enum { IDD = IDD_QUANTUM_DPP };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlSpacingValue;
	CStatic m_ctrlRepulsionValue;
	CStatic m_ctrlEntangleValue;
	CStatic m_ctrlDotSizeValue;

	void UpdateLabels();
	void ApplyPreview();

	DECLARE_MESSAGE_MAP()
};