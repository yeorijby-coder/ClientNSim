#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include "EcsDef.h"

//#include "CvTrackInfo.h"
//#include "CvExtraInfo.h"
//#include "OpBoxInfo.h"

//#include "ScInfo.h"
//#include "LifterInfo.h"
//#include "RgvInfo.h"
//#include "DisplayInfo.h"
//#include "BcrInfo.h"

//#include "CvSocket.h"
//#include "ScSocket.h"
//#include "LfSocket.h"

//#include "Cv.h"


class CCvMsg
{
public:
	CCvMsg() { bModified = FALSE; };
public:
	int nCommand;
	int nTrackNum;
	int nLuggNum;
	int nJobType;
	int nStartPos;
	int	nDestPos;
	int nStartAddr;
	//	int nProdGen;

	int nLfStartPos;
	int nLfDestPos;

	int nHeight;

	BYTE  ucMode;
	BYTE  ucStatus;
	BYTE  ucActive;
	WORD  wData;
	WORD  wErrCode;

	BOOL bModified;
};

class CCv;
class CCvSocket;
class CCvTrackInfo;
class CEcsDoc;

class CCvThreadInfo
{
public:
	CCvThreadInfo() { m_pSocket = NULL; };
public:
	int m_nNum;
	DWORD m_dwReadTimeOut;

	CEcsDoc* m_pDoc;
	CCvSocket* m_pSocket;
	CCvTrackInfo* m_pCvTrackInfo;
	CCv* m_pCCv;

	CCvMsg m_CvMsg;
	CCvMsg m_CvRgvMsg;

	HWND m_hwndNotify;

	HANDLE m_hEventSendConveyor;
	HANDLE m_hEventKillConveyorThread;
};
UINT CvThreadProc(LPVOID pParam);





#define SC_CMD_INVOKE		1
#define SC_CMD_COMPLETE		2
#define SC_CMD_CANCEL		3
#define SC_CMD_RESET		4
#define SC_CMD_ESTOP		5
#define SC_CMD_STOP			6
#define SC_CMD_ACTIVE		7
#define SC_CMD_CALL_TO_HOME	8
#define SC_CMD_DUAL_STO		9
#define SC_CMD_EMPTY_RET	10
#define SC_CMD_MANUAL		11
#define SC_CMD_STO_SUSPEND	12
#define SC_CMD_RET_SUSPEND	13

#define CMD_SC_STORE	 0
#define CMD_SC_RETRIEVE  1
#define CMD_SC_RTR		 2
#define CMD_SC_SITE		 3

class CScMsg
{
public:
	CScMsg() {
		bModified = FALSE;
		nLuggNumForkTwo = 0;
		strLocationForkTwo = "";
		strSecondLocForkTwo = "";
		nDeleteFork = 0;
	};

public:
	int nDeleteFork;
	int nCommand;
	int nCmdType;

	int nLuggNum;
	int nLuggNumForkTwo;
	int nStartHS;
	int nDestHS;

	CString strLocation;
	CString strSecondLoc;
	CString strLocationForkTwo;
	CString strSecondLocForkTwo;
	BOOL bModified;
};

#define DSP_CMD_TEST		1
#define DSP_CMD_CHECK		2
#define DSP_CMD_DATA		3
#define DSP_CMD_CLEAR		4
#define DSP_CMD_ERROR		5
#define DSP_CMD_PREV		6

class CDisplayMsg
{
public:
	CDisplayMsg() { m_bModified = FALSE; m_nNum = 1; m_nColor = 0x04; };

public:
	int		m_nCommand;

	int		m_nNum;
	CString m_strProductID;
	CString m_strLine;
	int		m_nColor;

	BOOL    m_bModified;
};

#endif // __THREAD_H__
