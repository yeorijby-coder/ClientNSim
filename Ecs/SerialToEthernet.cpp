#include "stdafx.h"
#include "ecs.h"
#include "SerialToEthernet.h"

#include "Lib.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSerialToEthernet::CSerialToEthernet()
{
	m_pRecvBuff = NULL;
	m_pEvent = NULL;
}

CSerialToEthernet::~CSerialToEthernet()
{
	//	CloseHandle(m_pEvent[EVENT_RECEIVE]);
	//	CloseHandle(m_pEvent[EVENT_EXIT]);

	if (m_pEvent)
	{
		CloseHandle(m_pEvent[EVENT_RECEIVE]);
		CloseHandle(m_pEvent[EVENT_EXIT]);
		delete[]m_pEvent;
	}

	m_pEvent = NULL;

	if (m_pRecvBuff != NULL)
		delete m_pRecvBuff;
	m_pRecvBuff = NULL;
}

BOOL CSerialToEthernet::Initialize(HWND hWnd, UINT nMsg, int nNum, DWORD dwRecvWaitTime)
{
	m_hWnd = hWnd;
	m_nMsg = nMsg;
	m_nNum = nNum;
	m_pEvent = new HANDLE[EVENT_COUNT];
	m_pEvent[EVENT_RECEIVE] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pEvent[EVENT_EXIT] = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_dwRecvWaitTime = dwRecvWaitTime;

	m_bConnected = FALSE;
	InitializeCount();

	m_pRecvBuff = new BYTE[LENGTH_OF_RECEIVE_BUFF];
	ZeroMemory(m_pRecvBuff, LENGTH_OF_RECEIVE_BUFF);
	return TRUE;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSerialToEthernet, CAsyncSocket)
	//{{AFX_MSG_MAP(CSerialToEthernet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSerialToEthernet member functions

void CSerialToEthernet::OnClose(int nErrorCode)
{

	m_bConnected = FALSE;

	CString strLog;
	if (nErrorCode)
		strLog = CLib::GetSystemErrMsg(nErrorCode);
	else
		strLog = CLib::GetSystemErrMsg(CAsyncSocket::GetLastError());


	PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_CLOSE);
	TRACE("#6 CLOSE CSerialToEthernet::OnClose");
	//::SetEvent(m_pEvent[EVENT_EXIT]);
	ShutDown();
	InitializeCount();
	m_bConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);

	//	delete this;
}

void CSerialToEthernet::OnConnect(int nErrorCode)
{
	CString strLog;
	if (nErrorCode)
		strLog = CLib::GetSystemErrMsg(nErrorCode);
	else
		strLog = CLib::GetSystemErrMsg(CAsyncSocket::GetLastError());

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();			// 밑에서 NULL Check 함!

	if (nErrorCode)
	{
		ShutDown();
		Close();
		PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_CLOSE);
		TRACE("#7 CLOSE CSerialToEthernet::OnConnect\n");
		m_bConnected = FALSE;

		// 맨 밑에 있어야 함~ 리턴이 있기 때문에 
		if (pMainFrame == NULL)
		{
			CAsyncSocket::OnConnect(nErrorCode);
			return;
		}

		//		pMainFrame->ShowWindow(SW_MINIMIZE);
	}
	else
	{
		PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_CONNECT);

		m_bConnected = TRUE;

		// 맨 밑에 있어야 함~ 리턴이 있기 때문에 
//		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
		if (pMainFrame == NULL)
		{
			CAsyncSocket::OnConnect(nErrorCode);
			return;
		}

		//		pMainFrame->ShowWindow(SW_MINIMIZE);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

void CSerialToEthernet::OnReceive(int nErrorCode)
{
	if (nErrorCode)
	{
		PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_ERROR);
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}

	DWORD dwLen = 0;
	if (IOCtl(FIONREAD, &dwLen) == FALSE)
	{
		PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_ERROR);
	}
	else
	{
		if (dwLen > 0)
		{
			BYTE* pBuff = new BYTE[dwLen + 1];
			ZeroMemory(pBuff, dwLen + 1);
			int nRealReceive = Receive(pBuff, dwLen);
			if (nRealReceive == 0)	// Connection closed
			{
				PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_ERROR);
			}
			else if (nRealReceive == SOCKET_ERROR)
			{
				PostMessage(m_hWnd, m_nMsg, m_nNum, NOTIFY_ERROR);
			}
			else if (nRealReceive > 0)
			{
				memcpy(m_pRecvBuff + m_nCurrentReceivedLength, pBuff, nRealReceive);
				m_nCurrentReceivedLength += nRealReceive;
				if (m_nCurrentReceivedLength >= m_nRecvLength)
				{
					SetEvent(m_pEvent[EVENT_RECEIVE]);
				}
			}
			delete pBuff;	pBuff = NULL;
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CSerialToEthernet::OnOutOfBandData(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

BOOL CSerialToEthernet::Recv(BYTE* pBuff, int nRecvCount)
{
	InitializeCount();
	m_nRecvLength = nRecvCount;
	DWORD dwWait;

	dwWait = WaitForMultipleObjects(EVENT_COUNT, m_pEvent, FALSE, m_dwRecvWaitTime);
	if (dwWait == WAIT_TIMEOUT)
	{
	}
	else if (dwWait == WAIT_EVENT_RECEIVE)
	{
		memcpy(pBuff, m_pRecvBuff, nRecvCount);
		m_nCurrentReceivedLength -= nRecvCount;
		memcpy(pBuff, m_pRecvBuff, nRecvCount);
		if (m_nCurrentReceivedLength > 0)
		{
			BYTE* pTemp = new BYTE[m_nCurrentReceivedLength];
			memcpy(pTemp, m_pRecvBuff + nRecvCount, m_nCurrentReceivedLength);
			ZeroMemory(m_pRecvBuff, LENGTH_OF_RECEIVE_BUFF);
			memcpy(m_pRecvBuff, pTemp, m_nCurrentReceivedLength);
			delete pTemp;	pTemp = NULL;
			m_nRecvLength = 0;
		}
		else if (m_nCurrentReceivedLength = 0)
		{
			InitializeCount();
			ZeroMemory(m_pRecvBuff, LENGTH_OF_RECEIVE_BUFF);
		}
		else if (m_nCurrentReceivedLength < 0)
			return FALSE;
		return TRUE;
	}
	else if (dwWait == WAIT_EVENT_EXIT)
	{
		ShutDown();
		return FALSE;
	}

	return FALSE;
}

CString CSerialToEthernet::GetErrorMsg()
{
	return m_strErrorMsg;
}

void CSerialToEthernet::InitializeCount()
{
	m_nCurrentReceivedLength = 0;
	m_nRecvLength = 0;
}

void CSerialToEthernet::SetExitEvent()
{
	SetEvent(m_pEvent[EVENT_EXIT]);
}

void CSerialToEthernet::SetReceiveEvent()
{
	SetEvent(m_pEvent[EVENT_RECEIVE]);
}


BOOL CSerialToEthernet::IsConnected()
{
	return m_bConnected;
}

BOOL CSerialToEthernet::Read(BYTE* pBuffSend, int nSendLen, BYTE* pBuffRecv, int nRecvLen)
{
	if (Send(pBuffSend, nSendLen) != nSendLen)
		return FALSE;

	if (pBuffRecv == NULL && nRecvLen == 0)
		return TRUE;

	if (Recv(pBuffRecv, nRecvLen) == FALSE)
		return FALSE;

	return TRUE;
}

BOOL CSerialToEthernet::Write(BYTE* pBuffSend, int nSendLen, BYTE* pBuffRecv, int nRecvLen)
{
	if (Send(pBuffSend, nSendLen) != nSendLen)
		return FALSE;

	if (nRecvLen == 0)
		return TRUE;

	if (Recv(pBuffRecv, nRecvLen) == FALSE)
		return FALSE;

	return TRUE;
}