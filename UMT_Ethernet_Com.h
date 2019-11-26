
// UMT_Ethernet_Com.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#ifdef _AFXDLL
#undef _AFXDLL
#endif
#define BUF_SIZE 500
#define SizeOfUMTRxPacktSizeConst 184
#define UMTRxPacktChecksumInd 3
#define UMTRxPackTimePosConst 12
#define UMTRxPackDataPosConst 16
#define SizeOfUMTRxDataSizeConst 168
#define M_MAX_PATH_SIMGUI 500
#define M_WSA_VERSION  MAKEWORD(2,0)
#define PRECISION 10
#define ChecksumStartPos 4
#define ClearConst 0
#define ThousandConst 1000


typedef void (*DataHandlerProc)(unsigned char *, unsigned long); 


using namespace std;


class CUMT_Ethernet_Com{
public:
	CUMT_Ethernet_Com(void);
	~CUMT_Ethernet_Com(void);
private:
	SOCKET m_SocketId;
	ADDRINFO *mp_HostAddr, *mp_DestAddr;
	char m_DestIpAddr[16];
	char m_SrcIpAddr[16];
	unsigned short m_DestPortNumber,m_BindPortNumber;
	unsigned char m_DataBuf[BUF_SIZE];
	signed long bytes_read;
	signed long m_ComType;
	DataHandlerProc fp_DataHandlerProc;
	HANDLE m_DataWaitThread;
	DWORD m_DataWaitThreadId;
	struct timeval m_Timeout;
	static bool mFileExist;
	static CString m_filepath;
	
	
public:
	static ofstream mf_umtfilestream;
	static bool SetFilePath(CString folderpath);
	static CString GetFilePath();

	bool InitWinSockCom();
	bool CloseWinSockCom();
	bool SetEthParameters(char *src_ip_addr,char *hw_ip_addr,unsigned short dest_port_number,unsigned short bind_port_num,signed long com_type);
	bool InitEthernetCom();
	bool CloseEthernetCom();

	static void OpenUmtFile();
	static void CloseUmtFile();
	static void DataWiatingThread(LPVOID l_param);
	BOOL WaitForData(unsigned long timeout, DataHandlerProc data_handler_proc, unsigned short priority);
	
	static void OnRxData(unsigned char *p_data, unsigned long data_size);
	
};