
// UMT_ReceiverDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "UMT_Ethernet_Com.h"

using namespace std;

// CUMT_ReceiverDlg dialog
class CUMT_ReceiverDlg : public CDialogEx
{
// Construction
public:
	CUMT_ReceiverDlg(CWnd* pParent = NULL);	// standard constructor
	
	CUMT_Ethernet_Com m_EthUmtReceiver;

// Dialog Data
	enum { IDD = IDD_UMT_RECEIVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_TransmitterIPCtrl;
	CEdit m_TransmitterPortctrl;
	CEdit m_RecieverIPCtrl;
	CEdit m_RecieverPortCtrl;
	CMFCEditBrowseCtrl m_UMTFilePathCtrl;
	CString m_sourceIp,m_destIp,m_sourcePort,m_destPort,m_filepath;
	
public:
	afx_msg void OnBnClickedRecieve();

	afx_msg void OnBnClickedCancel();
};
