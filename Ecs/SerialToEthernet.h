#pragma once
#if !defined(AFX_SERIALTOETHERNET_H__AC2BF3D9_1E57_4C44_878D_74626873260E__INCLUDED_)
#define AFX_SERIALTOETHERNET_H__AC2BF3D9_1E57_4C44_878D_74626873260E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif


class CSerialToEthernet : public CAsyncSocket
{
public:
	BOOL		m_bConnected;

	// Operations
public:
	CSerialToEthernet();
	virtual ~CSerialToEthernet();

	BOOL		Initialize(HWND hWnd, UINT nMsg, int nNum, DWORD dwRecvWaitTime);
	BOOL		IsConnected();
	void		SetExitEvent();
	void		SetReceiveEvent();
	CString		GetErrorMsg();

	// Overrides
public:
	BOOL Write(BYTE* pBuffSend, int nSendLen, BYTE* pBuffRecv = NULL, int nRecvLen = 0);
	BOOL Read(BYTE* pBuffSend, int nSendLen, BYTE* pBuffRecv, int nRecvLen);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialToEthernet)
public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSerialToEthernet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	BOOL		Recv(BYTE* pBuff, int nRecvCount);
	void		InitializeCount();
	int			m_nCurrentReceivedLength;
	DWORD		m_dwRecvWaitTime;
	HANDLE* m_pEvent;
	int			m_nRecvLength;
	BYTE* m_pRecvBuff;
	int			m_nNum;
	UINT		m_nMsg;
	HWND		m_hWnd;
	CString		m_strErrorMsg;
};

#endif