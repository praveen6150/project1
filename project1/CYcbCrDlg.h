#pragma once
#include "afxdialogex.h"

// Forward declaration of the view class so the dialog can talk to it
class Cproject1View;

// CYcbCrDlg dialog
class CYcbCrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CYcbCrDlg)

public:
	CYcbCrDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CYcbCrDlg();

	// Pointer hook link back to our view canvas workspace
	Cproject1View* m_pView;

	// Tracking memory states for YCbCr processing calculations
	int m_nY;
	int m_nCb;
	int m_nCr;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_YCBCR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};