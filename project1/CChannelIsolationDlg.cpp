#include "pch.h"
#include "project1.h"
#include "afxdialogex.h"
#include "CChannelIsolationDlg.h"
#include "project1Doc.h"
#include "project1View.h"

IMPLEMENT_DYNAMIC(CChannelIsolationDlg, CDialogEx)

CChannelIsolationDlg::CChannelIsolationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANNEL_ISOLATION, pParent)
	, m_pView(nullptr)
{}

CChannelIsolationDlg::~CChannelIsolationDlg()
{}

void CChannelIsolationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChannelIsolationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL_RED, &CChannelIsolationDlg::OnChannelChanged)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL_GREEN, &CChannelIsolationDlg::OnChannelChanged)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL_BLUE, &CChannelIsolationDlg::OnChannelChanged)
END_MESSAGE_MAP()

BOOL CChannelIsolationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Default to Red selected
	CheckRadioButton(IDC_RADIO_CHANNEL_RED, IDC_RADIO_CHANNEL_BLUE, IDC_RADIO_CHANNEL_RED);

	UpdatePreview();

	return TRUE;
}

void CChannelIsolationDlg::OnChannelChanged()
{
	UpdatePreview();
}

void CChannelIsolationDlg::UpdatePreview()
{
	if (!m_pView)
		return;

	int channel = 0; // 0 = Red, 1 = Green, 2 = Blue

	if (IsDlgButtonChecked(IDC_RADIO_CHANNEL_RED))
		channel = 0;
	else if (IsDlgButtonChecked(IDC_RADIO_CHANNEL_GREEN))
		channel = 1;
	else if (IsDlgButtonChecked(IDC_RADIO_CHANNEL_BLUE))
		channel = 2;

	m_pView->ApplyChannelIsolation(channel);
}
