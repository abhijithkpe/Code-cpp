/*
**************************************************************************************************
* License Information: Accord Software & Systems Pvt. Ltd.
* 37, Krishna Reddy Colony, Domlur layout
* Bangalore - 560071, INDIA
* Licensed software and all rights reserved.
**************************************************************************************************
* File: UMT_Ethernet_Com.cpp
*
* Description: 
*
* Compiler:VC++ 2010
*
* Compiler options:
*
* Pragma:
*
* H/W platform:
*
* Portability:
*
* Author(s):Anusha B
*
* Classes:GpssConstells
*
* Functions:
*
* References:
*
* Traceability to Design:
*
* Version History:
**************************************************************************************************/
#include "stdafx.h"
#include "UMT_Ethernet_Com.h"


/*******************************************************************************************************
* Function:CUMT_Ethernet_Com
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
* 1.6		Rajesh E 10/01/2015  10048   PC crashing issue
**********************************************************************************************************/

CString CUMT_Ethernet_Com::m_filepath = NULL;
bool CUMT_Ethernet_Com::mFileExist =false;
ofstream CUMT_Ethernet_Com:: mf_umtfilestream;
CUMT_Ethernet_Com::CUMT_Ethernet_Com(void)
{
	m_SocketId = INVALID_SOCKET;
	mp_HostAddr = NULL;
	mp_DestAddr = NULL;
	ZeroMemory(m_DestIpAddr,sizeof(m_DestIpAddr));
	m_DestPortNumber = ClearConst;
	m_BindPortNumber = ClearConst;
	fp_DataHandlerProc = NULL;
	m_DataWaitThread = NULL;
	m_DataWaitThreadId = ClearConst;
	ZeroMemory(&m_Timeout, sizeof(m_Timeout));
	ZeroMemory(m_DataBuf, sizeof(m_DataBuf));
}

/*******************************************************************************************************
* Function:InitWinSockCom
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
bool CUMT_Ethernet_Com::InitWinSockCom()
{
	/* Initiate Socket Communication */
	WSADATA	WSAData = { 0 };

	if ( 0 != WSAStartup( M_WSA_VERSION, &WSAData ) )
	{
		printf("Incorrect version of winsock.dll found");
		WSACleanup( );
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/*******************************************************************************************************
* Function:InitWinSockCom
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
bool CUMT_Ethernet_Com::CloseWinSockCom()
{
	if(SOCKET_ERROR == WSACleanup())
	{
		printf("Unable to Cleanup WSA\n");
		return FALSE;
	}
	return TRUE;
}

/*******************************************************************************************************
* Function:SetEthParameters
*
* Description: 
*
* Formal parameter(s): T_TCHAR *hw_ip_addr
*					   unsigned short port_number
*					   signed long com_type
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
bool CUMT_Ethernet_Com::SetEthParameters(char *src_ip_addr,char *hw_ip_addr,unsigned short dest_port_number,unsigned short bind_port_number,signed long com_type)
{
	if(NULL == hw_ip_addr)
		return FALSE;
	memcpy_s(m_DestIpAddr,sizeof(m_DestIpAddr),hw_ip_addr,sizeof(m_DestIpAddr));
	if(src_ip_addr != NULL)
	{
		memcpy_s(m_SrcIpAddr,sizeof(m_SrcIpAddr),src_ip_addr,sizeof(m_SrcIpAddr));
	}
	else
	{
		m_SrcIpAddr[0] = '\0';
	}
	
	m_DestPortNumber = dest_port_number;
	m_BindPortNumber = bind_port_number;
	m_ComType = SOCK_DGRAM;
	
	return TRUE;
}

/*******************************************************************************************************
* Function:InitEthernetCom
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
bool CUMT_Ethernet_Com::InitEthernetCom()
{
	signed long error;
	char s_port_No[25];
	ADDRINFOA hints;
	/* Open a datagram socket */
	m_SocketId = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_SocketId == INVALID_SOCKET)
	{
		printf("Could not create socket.\n");
		return FALSE;
	}

	memset(&hints, ClearConst, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = m_ComType;
	hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
	_itoa_s(m_DestPortNumber,s_port_No,sizeof(s_port_No),10);
	error = getaddrinfo(m_DestIpAddr, s_port_No, &hints, &mp_DestAddr);
	if (error != 0) {
		printf( "Error (%d): Cannot resolve host address(%s)\n", WSAGetLastError(), m_DestIpAddr);
		CloseEthernetCom();
		return FALSE;
	}	

	memset(&hints, ClearConst, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = m_ComType;
	hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
	_itoa_s(m_BindPortNumber,s_port_No, sizeof(s_port_No), 10);

	if(strcmp("",m_SrcIpAddr))
	{
		error = getaddrinfo(m_SrcIpAddr, s_port_No, &hints, &mp_HostAddr);
	}
	else
	{
		error = getaddrinfo(NULL, s_port_No, &hints, &mp_HostAddr);
	}
	if (error != 0) {
		printf( "Error (%d): Cannot resolve source address\n", WSAGetLastError());
		CloseEthernetCom();
		return FALSE;
	}

	if(m_ComType == SOCK_DGRAM)
	{
		error = bind(m_SocketId, mp_HostAddr->ai_addr, (INT)mp_HostAddr->ai_addrlen);
		if (error == INVALID_SOCKET)
		{
			printf( "Error: unable to bind socket (%d)\n", WSAGetLastError() );
			error = shutdown(m_SocketId, SD_BOTH);
			if (error == INVALID_SOCKET)
			{
				printf( "shutdown of bind socket(%d) failed - %d", m_SocketId, WSAGetLastError() );
			}
			CloseEthernetCom();
			return FALSE;
		}
	}

	return TRUE;
}

/*******************************************************************************************************
* Function:CloseEthernetCom
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
bool CUMT_Ethernet_Com::CloseEthernetCom()
{	

	if(INVALID_SOCKET != m_SocketId )
	{
		if( INVALID_SOCKET == closesocket(m_SocketId))
		{
			return FALSE;
		}
		else
		{
			m_SocketId = INVALID_SOCKET;
		}
	}

	if(NULL != mp_DestAddr)
	{
		freeaddrinfo(mp_DestAddr);
		mp_DestAddr = NULL;
	}

	if(NULL != mp_HostAddr)
	{
		freeaddrinfo(mp_HostAddr);
		mp_HostAddr = NULL;
	}

	if(NULL != m_DataWaitThread)
	{
		if(CloseHandle(m_DataWaitThread))
		{
			m_DataWaitThread = NULL;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*******************************************************************************************************
* Function:~CUMT_Ethernet_Com
*
* Description: 
*
* Formal parameter(s): none
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
CUMT_Ethernet_Com::~CUMT_Ethernet_Com(void)
{
	CloseUmtFile();
	CloseWinSockCom();
}

/*******************************************************************************************************
* Function:WaitForData
*
* Description: 
*
* Formal parameter(s): unsigned long timeout
*					   DataHandlerProc data_handler_proc
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/

BOOL CUMT_Ethernet_Com::WaitForData(unsigned long timeout, DataHandlerProc data_handler_proc, unsigned short priority)
{

	if(INVALID_SOCKET == m_SocketId )
	{
		printf("CUMT_Ethernet_Com: Socket not yet created\n");
		return FALSE;
	}
	fp_DataHandlerProc = data_handler_proc;
	
	m_Timeout.tv_sec = timeout / ThousandConst; // Extract the seconds
	m_Timeout.tv_usec =(timeout % ThousandConst) * ThousandConst; // and microseconds of the timeout
	
	/*create a thread to monoitor the port*/
	m_DataWaitThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DataWiatingThread,this,CREATE_SUSPENDED,&m_DataWaitThreadId);
	if(NULL ==m_DataWaitThread)
	{

		printf("Unable to create data waiting thread\n");
		return FALSE;
	}
	else
	{
		SetThreadPriority(m_DataWaitThread, priority);
		ResumeThread(m_DataWaitThread);
	}

	return TRUE;
}


/*******************************************************************************************************
* Function:DataWiatingThread
*
* Description: 
*
* Formal parameter(s): LPVOID l_param
*
* Return Value:BOOL
*
* System calls Used:
*
* Specific library calls:
*
* Functions Called:
*
* Global Variables
* Accessed:
* Modified:
*
* Assumption:
*
* Requirements:
*
* Low Level Requirement:
*
* Version History:
*
* <Version Number> <Author> <date> <defect Number> <Modification made and reason>
*   1.0
**********************************************************************************************************/
void CUMT_Ethernet_Com::DataWiatingThread(LPVOID l_param)
{
	CUMT_Ethernet_Com *p_ethrcom = (CUMT_Ethernet_Com *) l_param;
	struct timeval *p_timeval = NULL;
	fd_set fd_Set;
	signed long error_code;
	signed long bytes_read;
	signed long size_of_addr = (signed long)p_ethrcom->mp_DestAddr->ai_addrlen;

	// Clear file descriptor set
	FD_ZERO(&fd_Set);
	// Set the bit for network socket 'dataSock' in the descriptor set
	FD_SET(p_ethrcom->m_SocketId, &fd_Set);

	if(0 != 
		(p_ethrcom->m_Timeout.tv_sec * ThousandConst + p_ethrcom->m_Timeout.tv_usec/ThousandConst))
	{
		p_timeval = &p_ethrcom->m_Timeout;
	}
	else
	{
		p_timeval = NULL; /*wait for infinite time*/
	}
	
	while(INVALID_SOCKET != p_ethrcom->m_SocketId)
	{
		error_code = 1;
	   // Block until data is available on this socket, or iTimeOut milliseconds has passed
	   //error_code = select((INT)(p_ethrcom->m_SocketId + 1), &fd_Set, NULL, NULL, NULL);
		if (error_code <= 0)
		{
			printf("CUMT_Ethernet_Com: Error waiting for data / time out occured\n");
		}
		else
		{	
			if(p_ethrcom->m_ComType == SOCK_DGRAM)
			{
				bytes_read = recvfrom(p_ethrcom->m_SocketId,
					(char *)p_ethrcom->m_DataBuf,BUF_SIZE,NULL,
					p_ethrcom->mp_DestAddr->ai_addr,(INT *)&size_of_addr);
			}

			if(SOCKET_ERROR == bytes_read)
			{
				if(GetLastError() == WSAESHUTDOWN)
				{
					break;
				}
			}
			else
			{
				p_ethrcom->fp_DataHandlerProc(p_ethrcom->m_DataBuf, bytes_read);
			}
		}
	}	

	ExitThread(0);	/*RP*/
}


void CUMT_Ethernet_Com::OnRxData(unsigned char *p_data,unsigned long data_size)
{
	CString temp_str = NULL;
	/*Write header of the file*/
	if(mFileExist == false){
		OpenUmtFile();
	}
	
	if((data_size == SizeOfUMTRxPacktSizeConst) && 
		((p_data[0]== 0xAC) && (p_data[1]== 0x49) && (p_data[2]== 0x4A)))
	{		
		/*copy the checksum sent*/
		BYTE rx_checksum = p_data[UMTRxPacktChecksumInd]; 
		BYTE comp_checksum = ClearConst;

		/* compute the checksum from the rx data */
		for(unsigned short byteind = ChecksumStartPos; byteind < SizeOfUMTRxPacktSizeConst; byteind++)
		{
			comp_checksum ^= p_data[byteind];
		}	
		/* validate the checksum */
		if(rx_checksum == comp_checksum)
		{	
			if(!mf_umtfilestream.bad()){
				mf_umtfilestream << *((unsigned long*)(p_data+UMTRxPackTimePosConst));
				for(unsigned short byteind = UMTRxPackDataPosConst; byteind < SizeOfUMTRxPacktSizeConst; byteind+= sizeof(double)){
					mf_umtfilestream << ",";
					mf_umtfilestream << fixed << setprecision(PRECISION) << *((double*)(p_data+byteind));
				}
				mf_umtfilestream << "\n";
				//mf_umtfilestream.close();
			}
			else{
				AfxMessageBox(_T("ERROR: Unable to open file\n"));
				return;
			}
		}
		else
		{
			/* checksum failure */
			AfxMessageBox(_T("UMT RCV data checksum failure at time: %u\n"),*((unsigned long*)(p_data+UMTRxPackTimePosConst)));
			return;
		}
	}

}


bool CUMT_Ethernet_Com::SetFilePath(CString filepath){
	
	CString tmp_str;
	/*
		verify whether user has browsed output folder. If not
	    alert the user
	*/
	if(_T("") == filepath)
	{
		tmp_str.Format(_T("Please enter the output file path."));
		AfxMessageBox(tmp_str, MB_ICONINFORMATION);
		return false;
	}
	else{
		int retval = PathFileExists(filepath);
		if(retval == 1){
			tmp_str.Format(_T("File already exist.Please enter new file name."));
			AfxMessageBox(tmp_str, MB_ICONINFORMATION);
			return false;
		}
		else
			m_filepath = filepath;
	}
	return true;	
}

CString CUMT_Ethernet_Com:: GetFilePath(){
	return m_filepath;
}

void CUMT_Ethernet_Com:: OpenUmtFile(){
	char file_data[BUF_SIZE] = "";
	char file_path[M_MAX_PATH_SIMGUI]= "";
	strcpy_s(file_path,CT2A(GetFilePath()));
	CString temp_str;
	mf_umtfilestream.open(file_path,ios::out);

	if(!mf_umtfilestream.bad())
	{	
		temp_str.Format(_T("Time_ms(ms),UposX(m),UposY(m),UposZ(m),UvelX(m/s),UvelY(m/s),UvelZ(m/s),UaccX(m/s2),UaccY(m/s2),UaccZ(m/s2),UjerkX(m/s3),UjerkY(m/s3),UjerkZ(m/s3),Uheading(rad),Uele(rad),Ubank(rad),Uang_velX(rad/s),Uang_velY(rad/s),Uang_velZ(rad/s),Uang_accX(rad/s2),Uang_accY(rad/s2),Uang_accZ(rad/s2)\n"));
		mf_umtfilestream.write(CT2CA(temp_str),temp_str.GetLength());
		mFileExist = true;
	}
}
void CUMT_Ethernet_Com::CloseUmtFile()
{
	if(!mf_umtfilestream.bad())
	{
		mf_umtfilestream.flush();
		mf_umtfilestream.close();
	}
}

