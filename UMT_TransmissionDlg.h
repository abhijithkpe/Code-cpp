
// UMT_TransmissionDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "UDPCommunicator.h"
#include <vector>
#include <fstream>

union temp64bitdouble
{
    double value;
    unsigned char byte[8];
};
union temp32bitint
{
    unsigned long value;
    unsigned char byte[4];
};

// CUMT_TransmissionDlg dialog
class CUMT_TransmissionDlg : public CDialogEx
{
// Construction
public:
	CUMT_TransmissionDlg(CWnd* pParent = NULL);	// standard constructor
	//
	
	~CUMT_TransmissionDlg();
// Dialog Data
	enum { IDD = IDD_UMT_TRANSMISSION_DIALOG };

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
	CComboBox m_UpdateRateCtrl;
	UDPCommunicator m_UDPConnector;
private:
	//bool InitSocketThread();
	bool ParseUMTFile();
public:
	afx_msg void OnBnClickedUMTTransfer();
private:
	CString m_csSourceIP;
	CString m_csSourcePort;
	CString m_csDestIP;
	CString m_csDestPort;
	CString m_csUMTFile;
	int m_nUpdateRateValue;
	std::vector<BYTE> m_DataVector;
	//std::vector<std::vector<BYTE>> m_DoubleDataVector;
	CWnd* pthread;
	DWORD m_DataWritingThreadId;
	HANDLE m_DataWritingThread;
	std::fstream m_InputFileRead;
	int m_Pos;

private:
	static void DataWritingThread( LPVOID lparam );

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
