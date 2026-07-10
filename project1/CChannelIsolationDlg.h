#pragma once
#include "afxdialogex.h"

class Cproject1View; // forward declaration

class CChannelIsolationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChannelIsolationDlg)

public:
	CChannelIsolationDlg(CWnd* pParent = nullptr);
	virtual ~CChannelIsolationDlg();

	enum { IDD = IDD_CHANNEL_ISOLATION };

	Cproject1View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnChannelChanged();

	void UpdatePreview();

	DECLARE_MESSAGE_MAP()
};
