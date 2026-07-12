#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CQuantumSimDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQuantumSimDlg)

public:
	CQuantumSimDlg(CWnd* pParent = nullptr);
	virtual ~CQuantumSimDlg();

	enum { IDD = IDD_QUANTUM_SIM };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_ctrlLevelsValue;
	CStatic m_ctrlUncertaintyValue;

	void UpdateLabelsAndPreview();

	DECLARE_MESSAGE_MAP()
};