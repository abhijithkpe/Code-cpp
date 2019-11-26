
// UMT_ReceiverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UMT_Receiver.h"
#include "UMT_ReceiverDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUMT_ReceiverDlg dialog




CUMT_ReceiverDlg::CUMT_ReceiverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUMT_ReceiverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUMT_ReceiverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_TransmitterIPCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_TransmitterPortctrl);
	DDX_Control(pDX, IDC_EDIT3, m_RecieverIPCtrl);
	DDX_Control(pDX, IDC_EDIT4, m_RecieverPortCtrl);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_UMTFilePathCtrl);
}

BEGIN_MESSAGE_MAP(CUMT_ReceiverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUMT_ReceiverDlg::OnBnClickedRecieve)
	ON_BN_CLICKED(IDCANCEL, &CUMT_ReceiverDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUMT_ReceiverDlg message handlers

BOOL CUMT_ReceiverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUMT_ReceiverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUMT_ReceiverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUMT_ReceiverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUMT_ReceiverDlg::OnBnClickedRecieve()
{
	bool fileexist = false;
	m_UMTFilePathCtrl.GetWindowTextW(m_filepath);
	m_TransmitterIPCtrl.GetWindowTextW(m_destIp);
	m_TransmitterPortctrl.GetWindowTextW(m_destPort);
	m_RecieverIPCtrl.GetWindowTextW(m_sourceIp);
	m_RecieverPortCtrl.GetWindowTextW(m_sourcePort);


	fileexist = m_EthUmtReceiver.SetFilePath(m_filepath);
	if(fileexist == true){
		if(m_EthUmtReceiver.InitWinSockCom()){
			if(m_EthUmtReceiver.SetEthParameters(CT2A(m_sourceIp),CT2A(m_destIp),(unsigned short)atoi(CT2A(m_destPort)),(unsigned short)atoi(CT2A(m_sourcePort)),SOCK_DGRAM)){
				if(m_EthUmtReceiver.InitEthernetCom())
				{
					m_EthUmtReceiver.WaitForData(NULL,m_EthUmtReceiver.OnRxData, THREAD_BASE_PRIORITY_MAX);
				}
				else{
					AfxMessageBox(_T("ERROR: Unable to create ethernet connection\n"));
					m_EthUmtReceiver.CloseWinSockCom();
					return;
				}
			}
			else{
				AfxMessageBox(_T("ERROR: Unable to create ethernet connection\n"));
				m_EthUmtReceiver.CloseWinSockCom();
				return;
			}
		}
		else{
			AfxMessageBox(_T("ERROR: Unable to create ethernet connection\n"));
			return;
		}
	}
	else{
	}
}

void CUMT_ReceiverDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if( IDOK == ( MessageBox( _T( "Stop reciever Operation. Do you want to proceed?" ), _T( "Warning" ), MB_OKCANCEL )))
	{
		if(m_EthUmtReceiver.mf_umtfilestream.is_open())
			m_EthUmtReceiver.CloseUmtFile();
		m_EthUmtReceiver.CloseWinSockCom();
		CDialogEx::OnCancel();
	}
	else{
	}
}
