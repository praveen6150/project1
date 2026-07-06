#pragma once
#include "afxdialogex.h"

// Forward declaration of the View class
class Cproject1View;

class CHsvDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CHsvDlg)

public:
    CHsvDlg(CWnd* pParent = nullptr);   // Standard constructor
    virtual ~CHsvDlg();

    Cproject1View* m_pView;             // Pointer back to view for live updates

    // Adjusted color values
    int m_nHue;
    int m_nSaturation;
    int m_nValue;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_HSV };      // Matches your new resource ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    // Combined message map handler for slider tracking
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    DECLARE_MESSAGE_MAP()

private:
    CImage m_previewImage;              // Holds the downscaled live thumbnail layout
};