
// UMT_TransmissionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UMT_Transmission.h"
#include "UMT_TransmissionDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include <sstream>

#define SEND_UPDATE_TIMER 1001
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


// CUMT_TransmissionDlg dialog




CUMT_TransmissionDlg::CUMT_TransmissionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUMT_TransmissionDlg::IDD, pParent)
	, m_csSourceIP(_T(""))
	, m_csSourcePort(_T(""))
	, m_csDestIP(_T(""))
	, m_csDestPort(_T(""))
	, m_csUMTFile(_T("")),
	m_Pos( 0 )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nUpdateRateValue = 0;
}

CUMT_TransmissionDlg::~CUMT_TransmissionDlg()
{
	KillTimer( SEND_UPDATE_TIMER );
}

void CUMT_TransmissionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_UpdateRateCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_csSourceIP);
	DDX_Text(pDX, IDC_EDIT2, m_csSourcePort);
	DDX_Text(pDX, IDC_EDIT3, m_csDestIP);
	DDX_Text(pDX, IDC_EDIT4, m_csDestPort);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_csUMTFile);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nUpdateRateValue);
}

BEGIN_MESSAGE_MAP(CUMT_TransmissionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUMT_TransmissionDlg::OnBnClickedUMTTransfer)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUMT_TransmissionDlg message handlers

BOOL CUMT_TransmissionDlg::OnInitDialog()
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

	m_UpdateRateCtrl.AddString( _T("10"));
	m_UpdateRateCtrl.AddString( _T("100"));
	// initialize socket
	m_UDPConnector.InitSocketCom();
	// socket initialized
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUMT_TransmissionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUMT_TransmissionDlg::OnPaint()
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
HCURSOR CUMT_TransmissionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUMT_TransmissionDlg::OnBnClickedUMTTransfer()
{

	UpdateData( TRUE );
	if( m_csDestIP.IsEmpty() || m_csSourceIP.IsEmpty() || m_csDestPort.IsEmpty() ||
		m_csSourcePort.IsEmpty() || ( -1 == m_nUpdateRateValue))
	{ 
		AfxMessageBox( _T( "Please enter Txr / Rxr details properly"));
		return;
	}
	if( m_UDPConnector.SetUDPParameters(  static_cast<CT2A>(m_csSourceIP), 
		static_cast<CT2A>(m_csDestIP),static_cast<unsigned short>(atoi (static_cast<CT2A>(m_csDestPort))),
		static_cast<unsigned short>(atoi (static_cast<CT2A>(m_csSourcePort))), SOCK_DGRAM ) )
	{
		m_UDPConnector.InitUDPCom();
	}

	////test
	//char *hello = "Hello from server"; 
	//m_UDPConnector.WriteData( ( char *)hello, strlen(hello));
	////test

	m_DataWritingThread = CreateThread( NULL,0,(LPTHREAD_START_ROUTINE)DataWritingThread,
										this,CREATE_SUSPENDED,&m_DataWritingThreadId );
	if( NULL == m_DataWritingThread )
	{
		AfxMessageBox( _T( "Unable to create data waiting thread"));
		return;
	}
	else
	{
		SetThreadPriority( m_DataWritingThread, THREAD_PRIORITY_HIGHEST );
		ResumeThread( m_DataWritingThread );
	}
	Sleep( 5000 );// sleep
    UINT Timer = SetTimer( SEND_UPDATE_TIMER, m_nUpdateRateValue, NULL);
}


void CUMT_TransmissionDlg::DataWritingThread( LPVOID lparam )
{
 	CUMT_TransmissionDlg* pDlg  = static_cast<CUMT_TransmissionDlg*>( lparam );
	pDlg->m_DataVector.clear();
	BYTE Preamble = 0xAC; // 1 byte
	BYTE Header1 = 0x49; // 2
	BYTE Header2 = 0x4A; // bytes
	BYTE Reserved[3] = {0};///3 byte
	BYTE Validation_Flag = 0x0;  // 1 byte
	temp32bitint Packet_Number = {0};  //4 bytes
	BYTE Checksum = 0x0;	
	DWORD error;
	pDlg->m_csUMTFile = L"C:\\user_motion_1.csv";
	
	pDlg->m_InputFileRead.open(pDlg->m_csUMTFile,std::ios::in);
	if( !pDlg->m_InputFileRead.is_open() )
	{		
		error = GetLastError();
		if( error )
		{
			AfxMessageBox( _T( "ERROR: Can't Open File\n" ));
			return ;
		}
	 }

	std::string sData;
	int pos = pDlg->m_InputFileRead.tellg();
	if( 0 == pDlg->m_Pos )  
	{
		std::getline( pDlg->m_InputFileRead,sData );
		sData.clear();
	}
	else
	{
		pDlg->m_InputFileRead.seekg( pDlg->m_Pos, std::ios::beg );
	}
	char* char_array;
	if(( !pDlg->m_InputFileRead.eof()) && (!pDlg->m_InputFileRead.fail()))
	{
		//Packet common part
		pDlg->m_DataVector.push_back( Preamble );
		pDlg->m_DataVector.push_back( Header1 );
		pDlg->m_DataVector.push_back( Header2 );
		pDlg->m_DataVector.push_back( Checksum );
		for(unsigned short ind = 0; ind < 3; ind++)
		{
			pDlg->m_DataVector.push_back( Reserved[ind] );
		}
		pDlg->m_DataVector.push_back( Validation_Flag );
		for(unsigned short ind = 0; ind < 4; ind++)
		{
			pDlg->m_DataVector.push_back( Packet_Number.byte[ind] );
		}
		//Packet common part
	
		std::getline( pDlg->m_InputFileRead,sData );
		int n = sData.length(); 
		char_array = new char[n + 1]; 
		strcpy(char_array, sData.c_str()); 
		char* pch = strtok (char_array, ",");
		temp32bitint u_time;
		u_time.value = std::stoi( pch );
		for(unsigned short ind = 0; ind < 4; ind++)
		{
			pDlg->m_DataVector.push_back( u_time.byte[ind] );
		}
		unsigned short index = 0;
		temp64bitdouble u_UmtPara;
		while(( NULL != char_array ) && !( index >= 24 ))
		{
			pch = strtok (NULL, ",");
			if(( index <= 11 )||( index >=15 ))
			{
				u_UmtPara.value = std::stod( pch );
				for( unsigned short ind = 0; ind < 8; ++ind )
				{
					pDlg->m_DataVector.push_back( u_UmtPara.byte[ind] );
				}
			}
			++index;
		}
		pDlg->m_Pos = pDlg->m_InputFileRead.tellg();
		// Calculating the checkSum
		for( unsigned short ind = 4; ind < 184; ++ind )
		{
			Checksum ^= pDlg->m_DataVector[ind];
		}
		pDlg->m_DataVector[3] = Checksum;//update checksum
		//m_DoubleDataVector.push_back( DataVector );
	}
	else
	{
		pDlg->KillTimer( SEND_UPDATE_TIMER );
		AfxMessageBox( _T( "SUCCESS: Data send successful" ));
	}
	delete[] char_array;	
	pDlg->m_InputFileRead.close();

}


void CUMT_TransmissionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if( SEND_UPDATE_TIMER == nIDEvent )
	{

		char Array[184];
		for( unsigned int nIndex =0 ;nIndex < m_DataVector.size(); ++nIndex )
		{
			Array[nIndex] = m_DataVector[nIndex];	
		}
		m_UDPConnector.WriteData( Array,sizeof( Array));
		DataWritingThread( this );
	}
	CDialogEx::OnTimer(nIDEvent);
}




















//bool CUMT_TransmissionDlg::ParseUMTFile()
//{
//	m_DataVector.clear();
//	BYTE Preamble = 0xAC; // 1 byte
//	BYTE Header1 = 0x49; // 2
//	BYTE Header2 = 0x49; // bytes
//	BYTE Reserved[3] = {0};///3 byte
//	BYTE Validation_Flag = 0x0;  // 1 byte
//	temp32bitint Packet_Number = {0};  //4 bytes
//	BYTE Checksum = 0x0;	
//	
//	//REMOTE_PC_DATA_TYPE cur_rpc_data;
//	const unsigned short FIVEHUNDRED = 500U;
//	char sTmpBuffer[FIVEHUNDRED];
//	
//	//std::vector<BYTE> DataVector;
//	//std::Vector<std::
//	DWORD error;
//	m_csUMTFile = L"C:\\user_motion_1.csv";
//	
//	m_InputFileRead.open(m_csUMTFile,std::ios::in);
//	if(!m_InputFileRead.is_open())
//	{		
//		error = GetLastError();
//		if(error)
//		{
//			printf("ERROR: Can't Open File\n");
//			return false;
//		}
//	  }
//	std::string sData;
//	int pos = m_InputFileRead.tellg();
//	if( pos == 0 )  
//	{
//		std::getline( m_InputFileRead,sData );
//		sData.clear();
//	}
//	m_InputFileRead.seekg( m_Pos, std::ios::beg );
//	char* char_array;
//	if((!m_InputFileRead.eof()) && (!m_InputFileRead.fail()))
//	{
//		//Packet common part
//		m_DataVector.push_back( Preamble );
//		m_DataVector.push_back( Header1 );
//		m_DataVector.push_back( Header2 );
//		m_DataVector.push_back( Checksum );
//		for(unsigned short ind = 0; ind < 3; ind++)
//		{
//			m_DataVector.push_back( Reserved[ind] );
//		}
//		m_DataVector.push_back( Validation_Flag );
//		for(unsigned short ind = 0; ind < 4; ind++)
//		{
//			m_DataVector.push_back( Packet_Number.byte[ind] );
//		}
//		//Packet common part
//	
//		std::getline( m_InputFileRead,sData );
//		int n = sData.length(); 
//		char_array = new char[n + 1]; 
//		strcpy(char_array, sData.c_str()); 
//		char* pch = strtok (char_array, ",");
//		temp32bitint u_time;
//		u_time.value = std::stoi( pch );
//		for(unsigned short ind = 0; ind < 4; ind++)
//		{
//			m_DataVector.push_back( u_time.byte[ind] );
//		}
//		unsigned short index = 0;
//		temp64bitdouble u_UmtPara;
//		while ((char_array != NULL ) && !(index >= 24))
//		{
//			pch = strtok (NULL, ",");
//			if(( index <= 11 )||( index >=15 ))
//			{
//				u_UmtPara.value = std::stod( pch );
//				for(unsigned short ind = 0; ind < 8; ind++)
//				{
//					m_DataVector.push_back(u_UmtPara.byte[ind]);
//				}
//			}
//			++index;
//		}
//		m_Pos += m_InputFileRead.tellg();
//		// Calculating the checkSum
//		for(unsigned short ind = 4; ind < 184; ind++)
//		{
//			Checksum ^= m_DataVector[ind];
//		}
//		m_DataVector[3] = Checksum;//update checksum
//		//m_DoubleDataVector.push_back( DataVector );
//	}
//	delete[] char_array;
//	
//	
//	m_InputFileRead.close();
//	
//	return true;
//}


//bool CUMT_TransmissionDlg::ParseUMTFile()
//{
//	BYTE Preamble = 0xAC; // 1 byte
//	BYTE Header1 = 0x49; // 2
//	BYTE Header2 = 0x49; // bytes
//	BYTE Reserved[3] = {0};///3 byte
//	BYTE Validation_Flag = 0x0;  // 1 byte
//	temp32bitint Packet_Number = {0};  //4 bytes
//	BYTE Checksum = 0x0;	
//
//	//REMOTE_PC_DATA_TYPE cur_rpc_data;
//	const unsigned short FIVEHUNDRED = 500U;
//	char sTmpBuffer[FIVEHUNDRED];
//
//	std::vector<BYTE> DataVector;
//	//std::Vector<std::
//	DWORD error;
//	m_csUMTFile = L"C:\\user_motion_1.csv";
//
//	std::ifstream InputFileRead;
//	InputFileRead.open(m_csUMTFile,std::ios::in);
//	if(!InputFileRead.is_open())
//	{		
//		error = GetLastError();
//		if(error)
//		{
//			printf("ERROR: Can't Open File\n");
//			return false;
//		}
//    }
//	std::string sData;
//	std::getline( InputFileRead,sData );
//	sData.clear();
//	char* char_array;
//	while((!InputFileRead.eof()) && (!InputFileRead.fail()))
//	{
//		//Packet common part
//		DataVector.push_back( Preamble );
//		DataVector.push_back( Header1 );
//		DataVector.push_back( Header2 );
//		DataVector.push_back( Checksum );
//		for(unsigned short ind = 0; ind < 3; ind++)
//		{
//			DataVector.push_back( Reserved[ind] );
//		}
//		DataVector.push_back( Validation_Flag );
//		for(unsigned short ind = 0; ind < 4; ind++)
//		{
//			DataVector.push_back( Packet_Number.byte[ind] );
//		}
//		//Packet common part
//
//		std::getline( InputFileRead,sData );
//		int n = sData.length(); 
//		char_array = new char[n + 1]; 
//		strcpy(char_array, sData.c_str()); 
//		char* pch = strtok (char_array, ",");
//		temp32bitint u_time;
//		u_time.value = std::stoi( pch );
//		for(unsigned short ind = 0; ind < 4; ind++)
//		{
//			DataVector.push_back( u_time.byte[ind] );
//		}
//		unsigned short index = 0;
//		temp64bitdouble u_UmtPara;
//		while ((char_array != NULL ) && !(index >= 24))
//		{
//			pch = strtok (NULL, ",");
//			if(( index <= 11 )||( index >=15 ))
//			{
//				u_UmtPara.value = std::stod( pch );
//				for(unsigned short ind = 0; ind < 8; ind++)
//				{
//					DataVector.push_back(u_UmtPara.byte[ind]);
//				}
//			}
//			++index;
//		}
//		// Calculating the checkSum
//		for(unsigned short ind = 4; ind < 184; ind++)
//		{
//			Checksum ^= DataVector[ind];
//		}
//		DataVector[3] = Checksum;//update checksum
//		m_DoubleDataVector.push_back( DataVector );
//	}
//	delete[] char_array;
//
//
//	InputFileRead.close();
//	UINT Timer = SetTimer( SEND_UPDATE_TIMER, m_nUpdateRateValue, NULL );
//	return true;
//}