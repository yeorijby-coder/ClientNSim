// Host.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "Host.h"

#include "EcsDoc.h"
#include "MainFrm.h"
#include "EcsView.h"
#include "Lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CListenSk, CAsyncSocketEx)
IMPLEMENT_DYNAMIC(CHostSv, CAsyncSocketEx)
IMPLEMENT_DYNAMIC(CHostCl, CAsyncSocketEx)

//////////////////////////////////////
// CListenSk

CListenSk::CListenSk()
{
}

CListenSk::CListenSk(CEcsDoc * pDoc)
{
	m_pDoc = pDoc;
	m_bConnected = FALSE;
	m_bInkJetConnected = FALSE;
}

//CListenSk::CListenSk(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CListenSk::~CListenSk()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSk, CAsyncSocketEx)
//{{AFX_MSG_MAP(CListenSk)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////
// CListenSk member functions

#define		LOG_ERROR		m_pDoc->m_pLog->Error
#define		LOG_EVENT		m_pDoc->m_pLog->Event
#define     LOG_JOB			m_pDoc->m_pLog->Job
#define		LOG_DEBUG		m_pDoc->m_pLog->Debug
//#define     JOB				m_pDoc->m_pJob

void CListenSk::OnAccept(int nErrorCode) 
{
	CString strLocalAddr;
	UINT    nLocalPort;
	
	if ( GetSockName(strLocalAddr, nLocalPort) )
	{
		if ( m_pDoc->m_pConfig->m_nEcsPort == int(nLocalPort))
		{
			if (m_bConnected) 
			{
				m_pDoc->m_pHostSv->ShutDown();
				delete m_pDoc->m_pHostSv;
				m_pDoc->m_pHostSv = NULL;
				
				m_pDoc->m_pHostSv = new CHostSv(m_pDoc);
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				
				LOG_ERROR(LOG_POS_SYSTEM, LOG_SYSTEM, _T("ECS서버와 IMS 클라이언트 재연결 성공..!"));
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectOK;
			}
			else 
			{
				m_pDoc->m_pHostSv = new CHostSv(m_pDoc);
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
			}
			
			if (!Accept(*m_pDoc->m_pHostSv))
			{
				m_pDoc->m_pHostSv->ShutDown();
				delete m_pDoc->m_pHostSv;
				m_pDoc->m_pHostSv = NULL;
				
				m_bConnected = FALSE;
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_ERROR);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
			}
			else
			{
				m_bConnected = TRUE;
				m_pDoc->m_pHostSv->UpdateCommSv(NOTIFY_RECEIVE);
				m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectOK;
				
				LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, _T("ECS서버와 IMS 클라이언트 연결 성공"));
			}
		}
	}
	else
	{
		CString strLog;
		strLog.Format(_T("CListenSk::OnAccept.. GetSockName에러.. 내용[%s]"), CLib::GetSystemErrMsg());
		m_pDoc->m_pHostSv->m_enCommStatus = enStatusConnectFail;
		LOG_ERROR(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);
	}
	
	CAsyncSocketEx::OnAccept(nErrorCode);
}

/////////////////////////////////
// CHostSv

CHostSv::CHostSv(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	m_nHostSeqNum = 0;
}

//CHostSv::CHostSv(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CHostSv::~CHostSv()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHostSv, CAsyncSocketEx)
//{{AFX_MSG_MAP(CHostSv)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////
// CHostSv member functions

void CHostSv::OnClose(int nErrorCode) 
{
	UpdateCommSv(NOTIFY_ERROR);
	
	CString strLog;
	strLog.Format(_T("ECS서버와 IMS 클라이언트 연결 해제..! [%s]"), CLib::GetSystemErrMsg(nErrorCode));
	
	LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);
	m_pDoc->m_pHostListener->m_bConnected = FALSE;
	m_pDoc->m_pHostSv = NULL;
	
	ShutDown();
	delete this;
	
	CAsyncSocketEx::OnClose(nErrorCode);
}

void CHostSv::OnReceive(int nErrorCode) 
{
	
	CAsyncSocketEx::OnReceive(nErrorCode);
	
	if (nErrorCode) {
		AfxMessageBox(_T("The Windows Sockets implementation detected that the network subsystem failed."));
	}
	
	int nMsgLen = 0;
	char RxBuff[1024];
	CString strLog;
	
	DWORD dwLen = 0;
	DWORD nRealLen = 0;
	
	if(IOCtl( FIONREAD, &dwLen ) == FALSE)
	{
		strLog.Format(_T("CHostCl::OnReceive.. 메시지 수신 에러 [%s]"), CLib::GetSystemErrMsg());
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		Answer(CMD_UNPREDICT, 0, MSG_SOCKET_ERROR);
		return;
	}
	
	memset(RxBuff, 0x00, sizeof(RxBuff));
	if ((nRealLen = Receive(RxBuff, dwLen)) != dwLen)
	{
		strLog.Format(_T("ECS서버 메시지 수신.. 수신 메시지 길이 이상.. LEN[%d]  REAL_LEN[%d]"), dwLen, nRealLen);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		Answer(CMD_UNPREDICT, 0, MSG_INVALID_HEADER_LENG);
		return;
	}
//=========================================================================================================================================================================
//	// 받아오는 구문을 수정해야 하리라. Stx Etx 별로 나누어서 읽어야 하리라.
//=========================================================================================================================================================================
//	// 원본 소스 
//	if (RxBuff[MSG_LENGTH_HEADER] != STX)
//	{
//		Answer(CMD_UNPREDICT, 0, MSG_NO_STX);
//		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS서버 데이터 수신.. No STX 에러..!");
//		return;
//	}
//	
//	int nFrameLen = dwLen -1;
//	if (RxBuff[nFrameLen] != ETX)
//	{
//		Answer(CMD_UNPREDICT, 0, MSG_NO_ETX);
//		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS서버 데이터 수신.. No ETX 에러..!");
//		return;
//	}
//	
//	if (dwLen < 50)
//	{
//		Answer(CMD_UNPREDICT, 0, MSG_INVALID_MSG_LENG);
//		strLog.Format("ECS서버 데이터 수신.. 메세지에서 정의된 길이는 맞지만... 메세지 길이가 너무 짧습니다...! LEN[%d]", dwLen);
//		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
//		return;
//	}
//
//	Parsing(RxBuff + MSG_LENGTH_HEADER);
//	UpdateCommSv(NOTIFY_RECEIVE);
//
//	// 수정본 소스
	CString strRecv = RxBuff;
	if (strRecv.GetLength() < 14)
		return;

	int nLength = _ttoi(strRecv.Mid(10, 4));
	CAsyncSocketEx::TokenRxFrame(strRecv, enSTX, enETX, TRUE);
	for (POSITION pos=m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (strRxFrame[MSG_LENGTH_HEADER] != enSTX)
		{
			Answer(CMD_UNPREDICT, 0, MSG_NO_STX);
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, _T("ECS서버 데이터 수신.. No STX 에러..!"));
			continue;
		}

		int nFrameLen = dwLen -1;
		if (strRxFrame[nLen-1] != enETX)
		{
			Answer(CMD_UNPREDICT, 0, MSG_NO_ETX);
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, _T("ECS서버 데이터 수신.. No ETX 에러..!"));
			continue;
		}

//		if (nLength < 35)
//		{
//			Answer(CMD_UNPREDICT, 0, MSG_INVALID_MSG_LENG);
//			strLog.Format("ECS서버 데이터 수신.. 메세지에서 정의된 길이는 맞지만... 메세지 길이가 너무 짧습니다...! LEN[%d]", dwLen);
//			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
//			continue;
//		}

		// @.받은 전문을 화면 수신 리스트에도 보낸다. (보낸 것만 보이고 받은 것은 안 보였다)
		m_pDoc->AddHostRecv(strRxFrame);

		Parsing(RxBuff + MSG_LENGTH_HEADER);
		UpdateCommSv(NOTIFY_RECEIVE);
	}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

}

//#define LGV_INFO			m_pDoc->m_pLgvInfo
//#define TRACK_MSG			m_pDoc->m_ConveyorThreadInfo[CV_6_PLC].m_ConveyorMsg

/*
 * @.상태전문의 장비번호가 담을 자리를 넘어섬을 때 알린다.
 *
 *   범위를 넘으면 버리는 수밖에 없는데, 전에는 그것을 조용히 했다.
 *   SC_CNT 이 5 였던 동안 6호기 이상의 상태가 들어오지 않는 줄도 몰랐고,
 *   그 호기를 쓰는 로직그룹은 입고 지시가 작업번호 0 으로 나갔다.
 *
 *   상태는 주기적으로 계속 오므로 번호당 한 번만 적는다.
 */
void CHostSv::WarnStatusRange(LPCTSTR lpszKind, int nDeviceNo, int nMax, LPCTSTR lpszDefine)
{
	int nDummy = 0;
	if (m_mapWarnedDevice.Lookup(nDeviceNo, nDummy))
		return;

	m_mapWarnedDevice.SetAt(nDeviceNo, 1);

	CString strLog;
	strLog.Format(_T("%s %d번 상태전문을 버렸습니다 - 받는 범위가 1~%d 입니다. EcsDef.h 의 %s 를 올려야 합니다."),
				  lpszKind, nDeviceNo, nMax, lpszDefine);
	m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::WarnStatusRange"));
	m_pDoc->WriteDiag(strLog);
}

void CHostSv::Parsing(char *pFrame)
{
	CString strLog;
	
	CString strFrame	= pFrame;
	BYTE ucMsgType		= pFrame[1];
//	int nResultCode		= _ttoi(strFrame.Mid(3, 2));
//	int nLuggNum		= 0;	// _ttoi(strFrame.Mid(5, 4));
	
	switch (ucMsgType)
	{
	case	CMD_COMPLETE:
		{
			//ucAckNak = pFrame[3];		// DUMMY	// JOB DEFINE
			// ECS 완료전문 : STX(0) Type(1) 작업구분(2) 창고구분(3) 작업번호(4-7) 완료구분(8) 완료차수(9) 도착작업대(10-12) ETX
			int nLuggNum = _ttoi(strFrame.Mid(4, 4));
			CString ucJobDefine	= strFrame.Mid(2, 1);
			int nStep = _ttoi(strFrame.Mid(9, 1));

			// @.이 완료보고가 로직 슬롯 하나에라도 닿았는지. 닿지 않으면 상위는
			//   보고를 받았지만 아무 일도 안 일어나고, 다음 작업이 안 나간다.
			//   전에는 그것이 조용해서 "완료는 됐는데 새 작업이 안 내려온다" 로만 보였다.
			BOOL bMatched = FALSE;

			int nLogicGroupCnt = m_pDoc->m_pLogicGorupInfos.GetCount();

			for (int i = 0; i < nLogicGroupCnt; i++)
			{
				SLogicGorupInfo* pLogicGorupInfo = m_pDoc->m_pLogicGorupInfos.GetAt(i);

				if (pLogicGorupInfo == NULL)
					continue;

				for (int j = 0; j < pLogicGorupInfo->m_nStoStnCnt; j++)
				{
					SJobInvokeInfo* pJobInvokeInfo = pLogicGorupInfo->m_pJobInvokeInfos.GetAt(j);

					if (pJobInvokeInfo == NULL)
						continue;

					if (pJobInvokeInfo->m_nWorkingLuggNum != nLuggNum)
						continue;

					bMatched = TRUE;

					if (ucJobDefine == _T("1"))
					{
						pJobInvokeInfo->m_bCompleteStore = TRUE;
					}
					else if (ucJobDefine == _T("2") || ucJobDefine == _T("3"))
					{
						// @.출고 / 피킹출고 끝. 슬롯을 풀어 다음 주기에 이동으로 다시 시작한다.
						if (nStep == 1)
						{
							pJobInvokeInfo->m_nWorkingLuggNum = 0;
						}
					}
					else if (ucJobDefine == _T("6"))
					{
						pJobInvokeInfo->m_bCompleteMove = TRUE;
					}
					else
					{
						// @.슬롯은 맞는데 작업구분을 모른다. 반자동(10~15)이 두 자리로 나가
						//   전문이 밀렸을 때 이러했다. 그대로 두면 슬롯이 영영 물려 있는다.
						strLog.Format(_T("알 수 없는 작업구분의 완료보고 [작업번호:%d] [작업구분:%s] - 전문을 확인하십시오."),
									  nLuggNum, (LPCTSTR)ucJobDefine);
						m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
						m_pDoc->WriteDiag(strLog);
					}
				}
			}

			// @.어느 슬롯과도 안 맞은 완료보고. 받기는 받았으나 로직은 아무 것도
			//   이어가지 않는다. 상위를 다시 띄워 지난 번호를 잃었거나, 로직이 아닌
			//   곳에서 만든 작업이거나, 이미 슬롯이 풀려 새 번호로 나간 뒤일 때다.
			//   조용히 버리면 "완료는 됐는데 새 작업이 안 내려온다" 로만 보인다.
			if (bMatched == FALSE)
			{
				strLog.Format(_T("완료보고를 받았지만 로직이 물고 있는 작업이 아닙니다 [작업번호:%d] [작업구분:%s] - 이 보고로는 다음 작업이 나가지 않습니다."),
							  nLuggNum, (LPCTSTR)ucJobDefine);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
				m_pDoc->WriteDiag(strLog);
			}

			/*	// 일반적인 입고 완료 케이스
			if (ucJobDefine == _T("1"))
			{
				if (m_pDoc->m_nWorkingLuggNum1 == nLuggNum)
				{
					m_pDoc->m_bCompleteStore1 = TRUE;
				}

				if (m_pDoc->m_nWorkingLuggNum2 == nLuggNum)
				{
					m_pDoc->m_bCompleteStore2 = TRUE;
				}
			}

			if (ucJobDefine == _T("2"))
			{
				if (nStep == 1)
				{
					if (m_pDoc->m_nWorkingLuggNum1 == nLuggNum)
					{
						m_pDoc->m_nWorkingLuggNum1 = 0;
					}
					if (m_pDoc->m_nWorkingLuggNum2 == nLuggNum)
					{
						m_pDoc->m_nWorkingLuggNum2 = 0;
					}
				}
			}
			else if (ucJobDefine == _T("6"))
			{
				if (m_pDoc->m_nWorkingLuggNum1 == nLuggNum)
				{
					m_pDoc->m_bCompleteStore1 = TRUE;
				}
				if (m_pDoc->m_nWorkingLuggNum2 == nLuggNum)
				{
					m_pDoc->m_bCompleteStore2 = TRUE;
				}
			}
			//*/
			Answer(CMD_COMPLETE, nLuggNum, RECEIVE_OK);
		}
		break;
	case	CMD_ERROR:
		{
			// ECS 에러전문 : STX(0) Type(1) 창고구분(2) 장비분류(3) 장비번호(4-6) 에러종류(7)
			//                에러코드(8-11) 작업번호(12-15) Bank(16-17) Bay(18-20) Level(21-22) ETX
			// [09]Interface목록서 IV.4 기준. 예전에는 창고구분을 빼고 읽어 한 칸씩 밀려 있었다.
			int nDeviceClass= _ttoi(strFrame.Mid( 3, 1));
			int nEquipNum	= _ttoi(strFrame.Mid( 4, 3));
			int nErrorKind	= _ttoi(strFrame.Mid( 7, 1));
			int nErrorCode	= _ttoi(strFrame.Mid( 8, 4));
			int nLuggNum	= _ttoi(strFrame.Mid(12, 4));
			int nErrorBank	= _ttoi(strFrame.Mid(16, 2));
			int nErrorBay	= _ttoi(strFrame.Mid(18, 3));
			int nErrorLevel	= _ttoi(strFrame.Mid(21, 2));

			// 이중 입고 에러일때는 이중입고 처리 버튼을 활성화 할것! 
			if (nDeviceClass == 1 && nErrorKind == 1)
			{
				// 재지정할 로케이션을 구할것 
				if (nErrorLevel >= m_pDoc->m_nMaxLevel)
				{
					nErrorLevel = 1;
					++nErrorBay;

					if (nErrorBay >= m_pDoc->m_nMaxBay)
						nErrorBay = 1;

				}
				m_pDoc->m_nPrevLuggNum = nLuggNum;			// ?
				m_pDoc->m_strAlterLocation.Format(_T("%02d%03d%02d"), nErrorBank, nErrorBay, nErrorLevel);
			}
			Answer(CMD_ERROR, nLuggNum, RECEIVE_OK);		
		}
		break;
		
	case	CMD_STATUS:
		{
			// ECS 상태전문(장비별 개별 프레임) : STX(0) Type(1) 창고구분(2) 장비분류(3) 장비번호(4-6) 상태(7) 작업번호(8-11) 사이즈체크(12) ECS작업유무(13) ETX
			// 장비분류 => 1:S/C, 2:C/V, 3:LGV, 4:RGV, 6:BCR
			int nDeviceClass = _ttoi(strFrame.Mid(3, 1));
			int nDeviceNo    = _ttoi(strFrame.Mid(4, 3));
			int nStatus      = _ttoi(strFrame.Mid(7, 1));

			// @.범위를 넘는 번호는 담을 자리가 없어 버릴 수밖에 없다. 그런데 조용히
			//   버리니 SC_CNT 가 5 였던 동안 6호기 상태가 안 들어오는 것도 몰랐다.
			//   상한을 넘으면 알리고, EcsDef.h 의 값을 고치게 한다.
			if (nDeviceClass == 1)			// S/C
			{
				if (nDeviceNo >= 1 && nDeviceNo <= SC_CNT)
					m_pDoc->m_nScStatus[nDeviceNo - 1] = nStatus;
				else
					WarnStatusRange(_T("크레인"), nDeviceNo, SC_CNT, _T("SC_CNT"));
			}
			else if (nDeviceClass == 2)		// C/V (스테이션 상태 : 0=작업불가, 1=작업가능)
			{
				if (nDeviceNo >= 1 && nDeviceNo <= STO_STN_CNT)
					m_pDoc->m_bStoStation[nDeviceNo - 1] = nStatus;
				else
					WarnStatusRange(_T("작업대"), nDeviceNo, STO_STN_CNT, _T("STO_STN_CNT"));
			}

			m_pDoc->m_bReceiveStatus = TRUE;
		}
		Answer(CMD_STATUS, 0, RECEIVE_OK);		
		break;
//	case	CMD_LOAD_ARRV:
//		Answer(CMD_LOAD_ARRV, nLuggNum, RECEIVE_OK);
//		break;

	// [09]Interface목록서 IV.8 공파렛트 입고 요구
	//   STX Type(1) 작업구분(2) P/M스테이션(3-5) UserData(6) ETX
	case	CMD_PALLET:
		{
			CString strLog;
			strLog.Format(_T("공파렛트 입고 요구.. 스테이션=[%d]"), _ttoi(strFrame.Mid(3, 3)));
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
		Answer(CMD_PALLET, 0, RECEIVE_OK);
		break;

	// [09]Interface목록서 IV.9 P-BoxRack 입고 요구
	//   STX Type(1) 작업번호#1(2-5) 작업번호#2(6-9) ETX
	case	CMD_IN_OUT_REQUEST:
		{
			CString strLog;
			strLog.Format(_T("P-BOX 입고 요구.. #1=[%d] #2=[%d]"),
				_ttoi(strFrame.Mid(2, 4)), _ttoi(strFrame.Mid(6, 4)));
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
		Answer(CMD_IN_OUT_REQUEST, 0, RECEIVE_OK);
		break;

	// [09]Interface목록서 IV.6 BCR 보고
	//   STX Type(1) 읽은스테이션(2-4) 읽은데이터(5-24) UserData(25) ETX
	case	CMD_BARCODE:
		{
			CString strLog;
			strLog.Format(_T("바코드 보고.. 스테이션=[%d] 데이터=[%s]"),
				_ttoi(strFrame.Mid(2, 3)), strFrame.Mid(5, 20));
			m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostSv::Parsing"));
		}
		Answer(CMD_BARCODE, 0, RECEIVE_OK);
		break;

	default:
		{
			CString strLog;
			strLog.Format(_T("에러.. Unknown MsgType=[%c]"), ucMsgType);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
			//			LOG_ERROR(LOG_POS_HOST, nLuggNum, strLog);
		}
		// 문서 III.A : 수신한 모든 메세지는 응답해야 한다.
		// 응답하지 않으면 보낸 쪽이 타임아웃으로 접속을 끊고 재접속을 반복한다.
		Answer(ucMsgType, 0, MSG_UNKNOWN_MSG_TYPE);
		break;	
	}
}

void CHostSv::JobOrderDirective(CJobItem& rJobItem)
{
	CString strLog;
	int nLuggNum = rJobItem.m_nLuggNum;
	int nJobType = rJobItem.m_nJobType;
	CJobItem* pJobItem = NULL;
	if (pJobItem = m_pDoc->m_pJob->Find(nLuggNum))
	{
		if (pJobItem->IsEqual(&rJobItem))
		{
			strLog.Format(_T("동일한 작업이 존재... IMS 확인 [%d]"), nLuggNum);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"), &rJobItem);
			Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorAlreadyJob);
		}
		else
		{
			strLog.Format(_T("작업번호 중복... IMS 확인 [%d]"), nLuggNum);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"), &rJobItem);
			Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorDuplicatedLuggNum);
		}
		return;
	}
	
	switch (nJobType)
	{
	case enJobTypeAutoSto:
	case enJobTypeAutoMove:
		{
			CStationInfo* pStation = m_pDoc->GetStationInfo(rJobItem.m_strStartStn);
			DEBUGER_ASSERT_VALID(pStation != NULL);
			DEBUGER_ASSERT_VALID(pStation->m_pTrack != NULL);
			
			// 이거는 보지 않음!
// 			if ((pStation->m_enKind == CStationInfo::enStoStation) && (pStation->m_pTrack->IsStoStationReady() == FALSE))
// 			{
// 				strLog.Format(_T("출발지 상태 이상! [%s]"), pStation->m_strName);
// 				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
// 				Answer(enHostCommandOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorInvalidStartStation);
// 				return;
// 			}
			
			if (pJobItem = m_pDoc->m_pJob->FetchReservedJobByStartStation(pStation->m_strID))
			{
				strLog.Format(_T("해당 출발지 예약된 작업이 존재! [출발위치=%s] [작업번호=%d]"), pStation->m_strName, pJobItem->m_nLuggNum);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
				Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorAlreadyJob);
				return;
			}
		}
		break;
		
	case enJobTypeAutoRet:
	case enJobTypeAutoPR:
		break;

	case enJobTypeAutoR2R:
		{
			if (CLib::GetStackerNum(rJobItem.m_nStartWH, rJobItem.m_strStartLoc) != CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
			{
				strLog.Format(_T("S/C 동일호기 작업이 아님! [랙이동 LOCATION 이상] [출발LOC=%s] [도착LOC=%s]"), rJobItem.m_strStartLoc, rJobItem.m_strDestLoc);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
				Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorInvalidLocation);
				return;
			}
		}
		break;
	
	case enJobTypeAutoW2W:
		{
			if (CLib::GetStackerNum(rJobItem.m_nStartWH, rJobItem.m_strStartLoc) == CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
			{
				strLog.Format(_T("S/C 동일호기 작업! [호기이동 LOCATION 이상] [출발LOC=%s] [도착LOC=%s]"), rJobItem.m_strStartLoc, rJobItem.m_strDestLoc);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
				Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorInvalidLocation);
				return;
			}
		}
		break;
		
	default:
		strLog.Format(_T("정의되지 않은 작업구분 [JobType=%d]"), nJobType);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
		Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	if ((pJobItem = m_pDoc->m_pJob->Add(rJobItem)) == NULL)
	{
		strLog.Format(_T("작업추가 실패! [%s]"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
		Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorBufferFull);
		return;
	}

	//==========================================================================================================
	// 출고대 끝단 정보 표현 
	//==========================================================================================================
	if (pJobItem != NULL && m_pDoc != NULL)
	{
		int nIndex = (pJobItem->m_nLuggNum > 0 && pJobItem->m_nLuggNum < 10000) ? pJobItem->m_nLuggNum : 0; 	

		m_pDoc->m_strDepartPos[nIndex] = pJobItem->GetStartStationString();
		m_pDoc->m_strArrivePos[nIndex] = pJobItem->GetDestStationString();
		m_pDoc->m_strWeightVal[nIndex] = pJobItem->m_strWeight;
		m_pDoc->m_strProductID[nIndex] = pJobItem->m_strProductID;
	}
	//----------------------------------------------------------------------------------------------------------
	
	strLog.Format(_T("신규작업 수신 및 추가 [%s]"), rJobItem.GetLogString());
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"), pJobItem);
	Answer(enHostCommandOrder, nLuggNum, nJobType, enHostErrorNone);
	
	m_pDoc->m_pJob->Invoke(nLuggNum);


}

void CHostSv::RedirectWeightDirective(CJobItem& rJobItem)
{
	CString strLog;
	CJobItem* pJobItem = NULL;
	
	if ((pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum)) == NULL)
	{
		strLog.Format(_T("중량 에러 작업정보가 존재하지 않음! [%d]"), rJobItem.m_nLuggNum);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectWeightDirective"));
		Answer(enHostCommandReDirWeight, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNotExistJob);
		return;
	}
	else
	{
		
		rJobItem.m_strBarcode	= rJobItem.m_strProductID = pJobItem->m_strProductID;

		m_pDoc->m_pJob->Remove(pJobItem);

		if ((pJobItem = m_pDoc->m_pJob->Add(rJobItem)) == NULL)
		{
			strLog.Format(_T("작업추가 실패! [%s]"), rJobItem.GetLogString());
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::JobOrderDirective"));
			Answer(enHostCommandReDirWeight, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorBufferFull);
			return;
		}
	}
	
	switch (rJobItem.m_nJobType)
	{
	case enJobTypeAutoSto:		break;	
	case enJobTypeAutoMove:		break;	
	default:
		strLog.Format(_T("유효하지 않은 작업구분! [%s]"), pJobItem->GetJobStatusString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectWeightDirective"), pJobItem);
		Answer(enHostCommandReDirWeight, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	pJobItem->SetJobStatus(enJobStatusCvNew);
	m_pDoc->m_pJob->Backup();
	
	strLog.Format(_T("%s 중량 에러 재지정 [DEST=%s]"), pJobItem->GetJobTypeString(), pJobItem->GetDestStationString());
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::RedirectWeightDirective"), pJobItem);
	Answer(enHostCommandReDirWeight, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorNone);
}

void CHostSv::RedirectLocDirective(CJobItem& rJobItem)
{
	CString strLog;
	CJobItem* pJobItem = NULL;
	
	if ((pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum)) == NULL)
	{
		strLog.Format(_T("이중입고에러 작업정보가 존재하지 않음! [%d]"), rJobItem.m_nLuggNum);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"));
		Answer(enHostCommandRedirect, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNotExistJob);
		return;
	}
	
	if (pJobItem->m_nJobType != rJobItem.m_nJobType)
	{
		strLog.Format(_T("작업구분 불일치! [OLD=%s] [NEW=%s]"), pJobItem->GetJobStatusString(), rJobItem.GetJobTypeString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"), pJobItem);
		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	if (pJobItem->m_nJobStatus != enJobStatusErrorDualStore)
	{
		strLog.Format(_T("이중입고에러 상태가 아님! [%s]"), pJobItem->GetJobStatusString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"), pJobItem);
		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorNotExistJob);
		return;
	}
	
	switch (pJobItem->m_nJobType)
	{
	case enJobTypeAutoSto:
	case enJobTypeAutoR2R:
	case enJobTypeAutoW2W:
		if (CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc) != CLib::GetStackerNum(rJobItem.m_nDestWH, rJobItem.m_strDestLoc))
		{
			strLog.Format(_T("S/C 동일호기 %s 작업이 아님! [재지정 LOCATION 이상] [OLD=%s] [NEW=%s]"), rJobItem.GetJobTypeString(), pJobItem->m_strDestLoc, rJobItem.m_strDestLoc);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"), pJobItem);
			Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorInvalidLocation);
			return;
		}
		break;
		
	default:
		strLog.Format(_T("유효하지 않은 작업구분! [%s]"), pJobItem->GetJobStatusString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"), pJobItem);
		Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorUnknownJobType);
		return;
	}
	
	pJobItem->m_strDestLoc = rJobItem.m_strDestLoc;
	pJobItem->SetJobStatus(enJobStatusDualStoreRetry);
	m_pDoc->m_pJob->Backup();
	
	strLog.Format(_T("%s 이중입고 재지정 [LOC=%s]"), pJobItem->GetJobTypeString(), pJobItem->m_strDestLoc);
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::RedirectLocDirective"), pJobItem);
	Answer(enHostCommandRedirect, pJobItem->m_nLuggNum, pJobItem->m_nJobType, enHostErrorNone);
}

void CHostSv::ResendOrderDirective(CJobItem& rJobItem)
{
	CString strLog;
	CJobItem* pJobItem = NULL;
	
	if (pJobItem = m_pDoc->m_pJob->Find(rJobItem.m_nLuggNum))
	{
		strLog.Format(_T("해당 작업정보 존재! [%s]"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, rJobItem.m_nLuggNum, strLog, _T("CHostSv::ResendOrderDirective"));
		Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorDuplicatedLuggNum);
		return;
	}
	
	if ((pJobItem = m_pDoc->m_pJob->Add(rJobItem)) == NULL)
	{
		strLog.Format(_T("작업추가 실패! [%s]"), rJobItem.GetLogString());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostSv::ResendOrderDirective"));
		Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorBufferFull);
		return;
	}
	
	//==========================================================================================================
	// 출고대 끝단 정보 표현 
	//==========================================================================================================
	if (pJobItem != NULL && m_pDoc != NULL)
	{
		int nIndex = (pJobItem->m_nLuggNum > 0 && pJobItem->m_nLuggNum < 10000) ? pJobItem->m_nLuggNum : 0; 	

		m_pDoc->m_strDepartPos[nIndex] = pJobItem->GetStartStationString();
		m_pDoc->m_strArrivePos[nIndex] = pJobItem->GetDestStationString();
		m_pDoc->m_strWeightVal[nIndex] = pJobItem->m_strWeight;
		m_pDoc->m_strProductID[nIndex] = pJobItem->m_strProductID;
	}
	//----------------------------------------------------------------------------------------------------------

	// 해당 함수 호출 직전에 처리함!
//	pJobItem->m_nJobStatus = enJobStatusResend;
	
	strLog.Format(_T("재송신작업 수신 및 추가 [%s]"), pJobItem->GetLogString());
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostSv::ResendOrderDirective"), pJobItem);
	m_pDoc->Alarm(LOG_POS_HOST, strLog, pJobItem);
	Answer(enHostCommandResendOrder, rJobItem.m_nLuggNum, rJobItem.m_nJobType, enHostErrorNone);
}

void CHostSv::Answer(TCHAR chType, int nLuggNum, int nJobType, int nResultCode)
{
	
//	TCHAR chAckNak = (nResultCode == enHostErrorNone) ? 'A' : 'N';
//	chType += (chType == enHostCommandUnpredict) ? 0 : 32;	// MAKE LOWER CASE CHARACTER

//	UpdateCommStatus(NOTIFY_SEND);
	
	Answer((BYTE)chType, nLuggNum, nResultCode);
}


void CHostSv::Answer(BYTE ucMsgType, int nLuggNum, int nReasonCode)
{
	CString strProductID ="";
	char TxBuff[64];
	char AckNak = (nReasonCode == MSG_NO_ERROR) ? 'A':'N';
	ucMsgType += (ucMsgType == CMD_UNPREDICT) ? 0 : 32; // make lower case character
	
	memset(TxBuff, 0x20, sizeof(TxBuff));
	sprintf_s(TxBuff, "ECS_MBX   %04d ", MSG_LENGTH_RESPONSE_DATA);  //MSG_LENGTH_RESPONSE_DATA - 10
	// 문서 III.A : 작업번호 뒤에 장비번호 3자리가 온다. (해당 없으면 000)
	sprintf_s(TxBuff + MSG_LENGTH_HEADER, sizeof(TxBuff) - MSG_LENGTH_HEADER, "%c%c%c%02d%04d%03d%c",
		STX, ucMsgType, AckNak, nReasonCode, nLuggNum, 0, ETX);
	
	UpdateCommSv(NOTIFY_SEND);
	Send(TxBuff, MSG_LENGTH_RESPONSE_MSG);
}

void CHostSv::UpdateCommSv(char Status)
{
	if (m_pDoc != NULL && m_pDoc->m_hWndView != NULL)
		::PostMessage(m_pDoc->m_hWndView, WM_USER_HOST_NOTIFY, 0, Status);
}

CString CHostSv::GetStartPos(int nStartPos)
{
	CString strTemp;
	
	if(nStartPos == ECS_STN_POS_101)
		strTemp.Format(_T("입고대"));
	
	return strTemp;	
}

CString CHostSv::GetDestPos(int nDestPos)
{
	CString strTemp;
	
	if(nDestPos == ECS_STN_POS_101)
		strTemp.Format(_T("출고대"));
	
	return strTemp;
}
/////////////////////////////////
// CHostCl

CHostCl::CHostCl(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	
//	nHogi = 0;
	nStackerStatus = 0;
	nLuggNum = 0;
//	nSto101 = 0;
//	nSto102 = 0;
//	nRet201 = 0;
//	nRet202 = 0;
	nStoRetMode =0;

	memset(m_JobOrderMsg, 0x0, sizeof(m_JobOrderMsg));
	memset(m_AlterLocMsg, 0x0, sizeof(m_AlterLocMsg));
}

//CHostCl::CHostCl(LPCTSTR lpszName, BOOL bWriteLog)
//{
//	m_strName = lpszName;
//	m_bWriteLog = bWriteLog;
//	m_enCommStatus = enStatusClose;
//}

CHostCl::~CHostCl()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHostCl, CAsyncSocketEx)
//{{AFX_MSG_MAP(CHostCl)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

////////////////////////////////////
// CHostCl member functions

void CHostCl::OnConnect(int nErrorCode) 
{
	CString strLog;
	if (nErrorCode)
	{
		// 프로그램 종료시 CEcsView::OnTimer로인해 CEcsDoc 객체소멸후 OnConnect 호출되면 에러
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (pMainFrame != NULL)
		{
			CEcsDoc* pDoc = (CEcsDoc*)pMainFrame->GetActiveDocument();
			if (pDoc != NULL)
			{
				UpdateCommCl(NOTIFY_ERROR);
				
				strLog.Format(_T("ECS HOST CLIENT와 IMS SERVER 연결 실패! [%s]"), CLib::GetSystemErrMsg(nErrorCode));
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostClient::OnConnect"), NULL, TRUE);
				
				CAsyncSocket::ShutDown();
				m_enCommStatus = enStatusConnectFail;
				pDoc->m_pHostCl = NULL;
			}
			m_enCommStatus = enStatusConnectFail;
		}
		m_enCommStatus = enStatusConnectFail;
		
		delete this;
	}
	else
	{
		UpdateCommCl(NOTIFY_SEND);
		
		strLog.Format(_T("ECS HOST CLIENT와 IMS SERVER 연결 성공!"));
		m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, strLog, _T("CHostClient::OnConnect"));
		
		m_enCommStatus = enStatusConnectOK;
		m_pDoc->m_pHostCl = this;
		m_pDoc->StatusReport();
	}
}

void CHostCl::OnClose(int nErrorCode) 
{
	UpdateCommCl(NOTIFY_ERROR);
	
	CString strLog;
	strLog.Format(_T("ECS클라이언트와 IMS 서버 연결 해제..! [%s]"), CLib::GetSystemErrMsg(nErrorCode));
	
	LOG_EVENT(LOG_POS_SYSTEM, LOG_SYSTEM, strLog);
	m_pDoc->m_pHostCl = NULL;
	ShutDown();
	delete this;
	
	CAsyncSocketEx::OnClose(nErrorCode);
}

void CHostCl::OnReceive(int nErrorCode) 
{
	CAsyncSocketEx::OnReceive(nErrorCode);
	
	if (nErrorCode) {
		AfxMessageBox(_T("The Windows Sockets implementation (HostCl) detected that the network subsystem failed."));
	}
	
	int nMsgLen = 0;
	char RxBuff[1024];
	CString strLog;
	
	DWORD dwLen = 0;
	DWORD nRealLen = 0;
	
	if(IOCtl( FIONREAD, &dwLen ) == FALSE)
	{
		strLog.Format(_T("CHostCl::OnReceive.. 메시지 수신 에러 [%s]"), CLib::GetSystemErrMsg());
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
	memset(RxBuff, 0x00, sizeof(RxBuff));
	if ((nRealLen = Receive(RxBuff, dwLen)) != dwLen)
	{
		strLog.Format(_T("ECS클라이언트 메시지 수신.. 수신 메시지 길이 이상.. LEN[%d]  REAL_LEN[%d]"), dwLen, nRealLen);
		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
		UpdateCommCl(NOTIFY_ERROR);
		return;
	}
	
//=========================================================================================================================================================================
//	// 받아오는 구문을 수정해야 하리라. Stx Etx 별로 나누어서 읽어야 하리라.
//=========================================================================================================================================================================
//	// 원본 소스 
//	if (RxBuff[MSG_LENGTH_HEADER] != STX)
//	{
//		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS클라이언트 데이터 수신.. No STX 에러..!");
//		UpdateCommCl(NOTIFY_ERROR);
//		return;
//	}
//	
//	int nFrameLen = dwLen -1;
//	if (RxBuff[nFrameLen] != ETX)
//	{
//		LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, "ECS클라이언트 데이터 수신.. No ETX 에러..!");
//		UpdateCommCl(NOTIFY_ERROR);
//		return;
//	}
//
//	UpdateCommCl(NOTIFY_RECEIVE);	
//	Parsing(RxBuff + MSG_LENGTH_HEADER);	
//
//	// 수정본 소스
	CString strRecv = RxBuff;
	int nLength = _ttoi(strRecv.Mid(10, 4));
	CAsyncSocketEx::TokenRxFrame(strRecv, enSTX, enETX, TRUE);
	for (POSITION pos=m_strRxList.GetHeadPosition(); pos!=NULL; )
	{
		CString& strRxFrame = m_strRxList.GetNext(pos);
		int nLen = strRxFrame.GetLength();

		if (strRxFrame[MSG_LENGTH_HEADER] != enSTX)
		{
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, _T("ECS서버 데이터 수신.. No STX 에러..!"));
			continue;
		}

		int nFrameLen = dwLen -1;
		if (strRxFrame[nLen-1] != enETX)
		{
			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, _T("ECS서버 데이터 수신.. No ETX 에러..!"));
			continue;
		}

//		if (dwLen != 25)
//		{
//			strLog.Format("ECS서버 데이터 수신.. 메세지에서 정의된 길이는 맞지만... 메세지 길이가 너무 짧거나 깁니다...! LEN[%d]", dwLen);
//			LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, strLog);
//			continue;
//		}

		// @.받은 전문을 화면 수신 리스트에도 보낸다. (보낸 것만 보이고 받은 것은 안 보였다)
		m_pDoc->AddHostRecv(strRxFrame);

		Parsing(RxBuff + MSG_LENGTH_HEADER);
		UpdateCommCl(NOTIFY_RECEIVE);
	}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

/*
CString CHostCl::GetStringReason(int nReasonCode)
{
	return CLib::ConvertReasonToString(nReasonCode);
}
*/
void CHostCl::Parsing(char *pFrame)
{
	// 작업지시와 재작업에 대한 응답을 받는 부분이라 Nck를 받을시에 저장되어 있는 전문을 보내면 된다. (최대 3회까지)
	CString strLog;
	
	CString strFrame = pFrame;
	BYTE ucMsgType   = pFrame[1];
	BYTE ucAckNak    = pFrame[2];
	int nResultCode  = _ttoi(strFrame.Mid(3, 2));
	int nLuggNum     = _ttoi(strFrame.Mid(5, 4));
	
	switch (ucMsgType)
	{
	case	CMD_ORDER_ANSWER:
		if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
		{
			// 작업지시에 대한 전문 재보고 

			// @.거절당한 작업번호를 로직 슬롯에서 풀고, 연속 거절 횟수를 센다.
			//   안 풀면 그 슬롯이 이 번호를 계속 물고 있어서, 다음 주기에도 같은
			//   번호로 내고 ECS 는 "이미 지시된 작업입니다" 로 또 거절한다.
			//   그렇다고 끝없이 다시 보내면, 거절 사유가 데이터 문제라 그대로 남아
			//   있을 때 2초마다 새 번호로 전문이 쏟아진다. 번호만 소모하고 정작
			//   사유는 로그에 파묻힌다. 연속 MAX_ORDER_NAK 번이면 로직을 멈춘다.
			int  nNakCount = 0;
			BOOL bStopped  = m_pDoc->OnOrderNak(nLuggNum, &nNakCount);

			if (bStopped == TRUE)
			{
				strLog.Format(_T("작업지시 거절 [작업번호:%d] [%d=%s] - %d회 연속이라 로직을 멈춘다. 사유를 고친 뒤 시작을 다시 누르면 된다."),
							  nLuggNum, nResultCode, CLib::GetHostResultSting(nResultCode), nNakCount);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
				m_pDoc->WriteDiag(strLog);
				return;
			}

			strLog.Format(_T("작업지시 거절 [작업번호:%d] [%d=%s] - %d/%d회. 다음 주기에 다시 시도한다."),
						  nLuggNum, nResultCode, CLib::GetHostResultSting(nResultCode), nNakCount, CEcsDoc::MAX_ORDER_NAK);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
			m_pDoc->WriteDiag(strLog);
			return;
		}

		// @.받아들여졌으면 그 슬롯의 연속 거절 횟수를 0 으로 되돌린다.
			m_pDoc->ClearOrderNak(nLuggNum);
		break;
	case	CMD_ALT_LOC_ANSWER:
		if ((nResultCode != enHostErrorNone) || (ucAckNak != 'A'))
		{
			// 재 작업지시에 대한 전문 재보고 

			int nReleased = m_pDoc->ReleaseWorkingLugg(nLuggNum);

			strLog.Format(_T("재작업 지시 거절 [작업번호:%d] [%d=%s] - 슬롯 %d개 해제. 다음 주기에 다시 시도한다."),
						  nLuggNum, nResultCode, CLib::GetHostResultSting(nResultCode), nReleased);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
			m_pDoc->WriteDiag(strLog);
			return;
		}
		break;
	default:
		{
			CString strLog;
			strLog.Format(_T("에러.. Unknown MsgType=[%c]"), ucMsgType);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Parsing"));
		}
		break;	
	}
}

void CHostCl::UpdateCommCl(char Status)
{
	if (m_pDoc != NULL && m_pDoc->m_hWndView != NULL)
		::PostMessage(m_pDoc->m_hWndView, WM_USER_HOST_NOTIFY, 1, Status);
}

BOOL CHostCl::Complete()
{
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusCompleteRequest);
	if (pJobItem == NULL)
	{
		pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusComplete);
		if (pJobItem == NULL)
			return FALSE;
	}
	
	return Complete(pJobItem->m_nLuggNum);
}

BOOL CHostCl::CompleteAuto()
{
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchByStatus(enJobStatusRefuseComplete);
	if (pJobItem == NULL) 
		return FALSE;
	
	return Complete(pJobItem->m_nLuggNum);
}

BOOL CHostCl::Complete(int nLuggNum, int nScJobType, int nStation, BOOL bDelete, BOOL bManual)
{
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("작업번호를 찾을수 없슴.."));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
		return FALSE;
	}
	
	int nStep = 1;				// C/V 완료
	
	TCHAR TxBuff[256];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	
	switch (CJobItem::ConvertJobTypeToPattern(pJobItem->m_nJobType))
	{
	case	enJobPatternSto:
		nScJobType = 1;		// 입고완료
		nStep = 0;
		break;
	case	enJobPatternRet:
	case	enJobPatternPR:
		nScJobType = 2;		// 출고완료
		if (pJobItem->m_nJobStatus == enJobStatusScComplete)
			nStep = 0;		// S/C 완료
		break;
	case	enJobPatternMove:
		nScJobType = 3;		// 이동완료
		break;
	case	enJobPatternW2W:
		if (pJobItem->m_nJobStatus == enJobStatusScComplete)
		{
			nScJobType = 2;		// 출고완료
			nStep = 0;			// S/C 완료
		}
		else
		{
			nScJobType = 1;		// 입고완료
			nStep = 1;
		}
		break;
	}
	
	if(nStation == 0)
		nStation = _ttoi(pJobItem->m_strDestStn);
	
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_COMPLETE_DATA);
	
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%d%04d%d%d%03d%c"),	
		STX, enHostCommandComplete, pJobItem->m_nJobType, nLuggNum, nScJobType, nStep, nStation, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_COMPLETE_MSG) == TRUE )
	{
		strLog.Format(_T("%s 작업 완료 보고 .. JobType=[%s]  ScJobType=[%d] StepCount=[%d]"),  (bManual) ? _T("수동") : _T(""), pJobItem->GetJobTypeString(), nScJobType, nStep);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Complete"), pJobItem);
		
		if ( pJobItem->m_nJobType == enJobTypeAutoSto ||
			pJobItem->m_nJobType == enJobTypeAutoMove ||
			pJobItem->m_nJobType == enJobTypeAutoR2R ||
			(pJobItem->m_nJobType == enJobTypeAutoPR && nStep == 1) ||
			(pJobItem->m_nJobType == enJobTypeAutoRet && nStep == 1) )
		{
			if (bDelete == TRUE)
				pJobItem->SetJobStatus(enJobStatusCompleteRequest);
//				pJobItem->m_nJobStatus = enJobStatusCompleteRequest;
		}
	}
	else
	{
		strLog.Format(_T("%s 작업 완료 보고 실패.. 에러=[%s]"),  (bManual) ? _T("수동") : _T(""), CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Complete"), pJobItem);
		pJobItem->SetJobStatus(enJobStatusRefuseComplete);
//		pJobItem->m_nJobStatus = enJobStatusRefuseComplete;
		return FALSE;
	}
	
	return TRUE;
}

BOOL CHostCl::ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn)
{
	if (pJobItem == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}
	
	if (IsConnect() == FALSE)
	{
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, _T("통신연결 않됨"), _T("CHostCl::ErrorScReport"), pJobItem);
		return FALSE;		
	}
	
	int nScNum = CLib::GetStackerNum(pJobItem->m_nDestWH, pJobItem->m_strDestLoc);
	int nErrorCode = (enErrorGbn == enErrorDualStore)? enScErrorDualStore : enScErrorEmptyRetrieve;
	int nErrorKind = (enErrorGbn == enErrorDualStore)? 1 : 3;
	
	Error(1, nScNum, nErrorKind, nErrorCode, pJobItem->m_nLuggNum, pJobItem->m_strDestLoc );
	
	CString strLog;
	strLog.Format(_T("에러보고 [%s]"), (enErrorGbn == enErrorDualStore) ? _T("이중입고") : _T("공출고"));
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::ErrorScReport"), pJobItem);
	
	return TRUE;
}


void CHostCl::Error(int nDevKind, int nDevNo, int nErrKind, WORD wErrCode, int nLuggNum, CString strLocation, CString strWeight, BOOL bManual)
{
	CJobItem *pJobItem = NULL;
	CString strLog;
	if (nLuggNum && nErrKind)
	{
		pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
		if (pJobItem == NULL)
		{
			strLog.Format(_T("작업번호를 찾을수 없슴.."));
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
			return;
		}
	}
	
	int	nBank, nBay, nLevel;
	if ( strLocation.IsEmpty() )
	{
		nBank = nBay = nLevel = 0;
	}
	else
	{
		nBank  = CLib::GetBank(strLocation);
		nBay   = CLib::GetBay(strLocation);
		nLevel = CLib::GetLevel(strLocation);
	}
	
	TCHAR TxBuff[256];
	int nErrorCode = wErrCode;
//	if (nErrorCode >= 0 && nErrorCode < 1000 )
//		nErrorCode = wErrCode;
//	else
//		nErrorCode = 0;
	
	int nWeight = _ttoi(strWeight);

	int nWeightLeft = 0;
	int nWeightRight = 0;

	if (nWeight > 10)
	{
		nWeightLeft = nWeight / 10;
		nWeightRight = nWeight % 10;
	}


	memset(TxBuff, 0x0, sizeof(TxBuff));
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_ERROR_DATA);	//23
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%d%04d%d%04d%04d%02d%03d%02d%18d.%d%c"),
		STX, enHostCommandError, nDevKind, nDevNo, nErrKind, nErrorCode, nLuggNum, nBank, nBay, nLevel, nWeightLeft, nWeightRight, ETX);

	if ((nLuggNum > 0) && (nLuggNum < 9000))
	{
		if(	SendMsg(TxBuff, MSG_LENGTH_ERROR_MSG) == TRUE )
		{
			strLog.Format(_T("%s 에러 보고 송신.. DevKind=[%d] DevNo=[%d] ErrKind=[%d] LuggNum=[%d] ErrorCode=[%d] Weight=[%s]"),
				(bManual) ? _T("수동") : _T(""), nDevKind, nDevNo, nErrKind, nLuggNum, nErrorCode, strWeight);
			m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
		}
		else
		{
			strLog.Format(_T("%s 에러 보고 송신 실패.. 에러=[%s]"),  (bManual) ? _T("수동") : _T(""), CLib::GetSystemErrMsg());
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Error"), pJobItem);
			
			if (nLuggNum && nErrKind && pJobItem)
				pJobItem->m_nJobStatus = enJobStatusRefuseError;
		}	
	}
}

void CHostCl::Arrived(int nLuggNum, int nStation, BOOL bManual)
{
	if (nLuggNum == 0 || nStation == 0)
		return;
	
	// 화물감지만으로 도착을 할경우 아래부분 주석처리해야함!
	CJobItem* pJobItem = m_pDoc->m_pJob->Find(nLuggNum);
	CString strLog;
	if (pJobItem == NULL)
	{
		strLog.Format(_T("작업번호를 찾을수 없슴.."));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);

		return;
	}
	
	TCHAR TxBuff[256];
	memset(TxBuff, 0x0, sizeof(TxBuff));
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_ARRIVAL_DATA);
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%03d%04d%s%c"),
		STX, enHostCommandArrive, nStation, nLuggNum, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_ARRIVAL_MSG) == TRUE )
	{
		CString strLog;
		strLog.Format(_T("%s 작업대 도착.. JobType=[%s] Station=[%s]"),
			(bManual) ? _T("수동") : _T(""), pJobItem->GetJobTypeString(), pJobItem->m_strDestStn.Right(3));
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);
		
//		if ( pJobItem->m_nJobType == enJobTypeAutoRet )
//		{
//			m_pDoc->m_pJob->m_nResultCount[pJobItem->GetStackerNum()]++;
//			::PostMessage(m_pDoc->m_hwndView, WM_USER_JOB_NOTIFY, JOB_RESULT_DRAW, 0);
//		}
		
		if (pJobItem->m_nJobType == enJobTypeAutoRet || 
			pJobItem->m_nJobType == enJobTypeAutoPR || 
			pJobItem->m_nJobType == enJobTypeAutoMove )
			m_pDoc->m_pJob->Remove(pJobItem);
	}
	else
	{
		CString strLog;
		strLog.Format(_T("%s 작업대 도착 보고 실패"),  (bManual) ? _T("수동") : _T(""));
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Arrived"), pJobItem);
		pJobItem->m_nJobStatus = enJobStatusRefuseArrived;
	}
}

int CHostCl::AlterLocation(BOOL bManual)
{
	int nUserData = 0;
	CString strLog, strTempMsg, strLocation;
	
	memset(m_AlterLocMsg, 0x0, sizeof(m_AlterLocMsg));

	int nLuggNum = m_pDoc->m_nPrevLuggNum;

	CString strJobType = "";

	strLocation = m_pDoc->m_strAlterLocation;
	int nScNo = m_pDoc->m_nDualStoScNo;

	// [09]Interface목록서 IV.2 재작업 지시
	//   타입1 작업구분1 창고구분1 작업번호4 출발대3 출발B/B/L 7
	//   도착대3 도착B/B/L 7 재지정종류1 크레인호기2   (본문 30자)
	// 예전에는 창고구분이 빠져 29자였고, 받는 쪽이 작업번호부터 어긋나게 읽었다.
		//			    1 2   3 4 5  6  7    
	strTempMsg.Format(_T("1%c%04d%s%s%s%s1%02d"),
		'A',			// 1	// 창고구분
		nLuggNum,		// 2	// 작업번호 
		_T("000"),			// 3	// Source Station
		_T("0000000"),		// 4	// Source Bank, Bay, Level
		_T("000"),			// 5	// Dest. Station
		strLocation,	// 6	// Dest. Bank, Bay, Level
		nScNo);			// 7	// Sc No.

	//m_pDoc->m_nPrevLuggNum = nLuggNum;

	m_nAlterLocSize = strTempMsg.GetLength() + 3;
	
	wsprintf(m_AlterLocMsg,						_T("WMS_MBX   %04d "),	m_nAlterLocSize);
	wsprintf(m_AlterLocMsg + MSG_LENGTH_HEADER, _T("%c%c%s%c"),	STX, CMD_ALT_LOC, strTempMsg, ETX);
	
	int nLenth =  sizeof(m_AlterLocMsg);
	
	if(	SendMsg(m_AlterLocMsg, m_nAlterLocSize + MSG_LENGTH_HEADER) == TRUE )
	{
		strLog.Format(_T("이중입고 재지정 %s 지시.. Location=[%s], ScNo=[%d]"), (bManual) ? _T("수동") : _T(""), strLocation, nScNo);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::AlterLocation"));
	}
	else
	{
		strLog.Format(_T("이중입고 재지정 %s 지시 실패.. Location=[%s], ScNo=[%d]"), (bManual) ? _T("수동") : _T(""), strLocation, nScNo);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::AlterLocation"));
		return 0;
	}
	
	return nLuggNum;
}

BOOL CHostCl::CalcurateLocation(BOOL bSto, CStringArray* pScList)
{
	BOOL bCurrentScOK = FALSE;
	BOOL bNextScOK = FALSE;

	int nCurrentScMinBank = 0;
	int nNextScMinBank = 0;
	
	int nCurrentScMaxBank = 0;
	int nNextScMaxBank = 0;
	
	//m_pDoc->m_strLocation = "";

	int nPrevBank = 0;
	if (m_pDoc->m_strLocation.GetLength() != 7)
	{
		nPrevBank	= 1;
		m_nCurBay	= 1;
		m_nCurLevel	= 1;
		//return FALSE;
	}
	else
	{
		nPrevBank	= _ttoi(m_pDoc->m_strLocation.Mid(0,2));
		m_nCurBay	= _ttoi(m_pDoc->m_strLocation.Mid(2,3));
		m_nCurLevel	= _ttoi(m_pDoc->m_strLocation.Mid(5,2));
	}

	if (nPrevBank == 10)
		int aaaaaaa = 0;

//	if (nPrevBank == 7)
//		nPrevBank = 10;


	//////////////////////////////////
	// 0 : 정상						//
	// 1 : 출고작업 수행중			//
	// 2 : 출고작업 수행 중			//
	// 3 : Rack to Rack 수행 중		//
	// 4 : Online 모드가 아닌 상태	//
	// 5 : 에러발생 상태			//
	// 6 : 입고중지 상태			//
	// 7 : 출고중지 상태			//
	// 8 : 입출고 중지상태			//
	////////////////////////////////// 

	CStringList strUseAbleBanks;
	CString strBank;
	int nCurBank = 0;
	// 작업 가능한 SC 호기를 체크한다. 
	for (int i = 0 ; i < SC_CNT ; i++)
	{
		// SC 호기 구하기 
		int nScNum = i + 1;
		int nStatus = m_pDoc->m_nScStatus[i];

		// 로직그룹 ScNum 목록 체크 - 목록이 있으면 목록의 SC만 사용
		if (pScList != NULL && pScList->GetSize() > 0)
		{
			BOOL bFound = FALSE;
			for (int k = 0; k < pScList->GetSize(); k++)
			{
				if (_ttoi(pScList->GetAt(k)) == nScNum)
				{
					bFound = TRUE;
					break;
				}
			}
			if (bFound == FALSE)
				continue;
		}

		if (nStatus == 4 || nStatus == 5 || nStatus == 8)
		{
			continue;
		}

		if (bSto == TRUE)
		{
			if (nStatus == 6)
			{
				continue;
			}
		}
		else
		{
			if (nStatus == 7)
			{
				continue;
			}
		}

		// 호기에 맞는 Bank 값 구하기 
		int nMinBank = (nScNum * 2) - 1;
		int nMaxBank = (nScNum * 2);
		
		strBank = CConvert::ToString(nMinBank);		strUseAbleBanks.AddTail(strBank);
		strBank = CConvert::ToString(nMaxBank);		strUseAbleBanks.AddTail(strBank);
	}
	
	if (strUseAbleBanks.GetCount() == 0)
		return FALSE;

	int nUseable1stBank = _ttoi(strUseAbleBanks.GetHead());
	for (POSITION pos=strUseAbleBanks.GetHeadPosition(); pos!=NULL; )
	{
		CString& strUseableBank = strUseAbleBanks.GetNext(pos);
		int nLen = strUseableBank.GetLength();		///
		
		int nUseableBank = _ttoi(strUseableBank);

		if (nPrevBank < nUseableBank)
		{
			nCurBank = nUseableBank;
			break;
		}
	}

	if (nCurBank == 0)
	{
		nCurBank = nUseable1stBank;

		++m_nCurBay;
		if (m_pDoc->m_nMaxBay < m_nCurBay )
		{
			m_nCurBay = 1;
			++m_nCurLevel;

			if (m_pDoc->m_nMaxLevel < m_nCurLevel)
			{
				//m_nCurBank = 1;
				m_nCurBay = 1;
				m_nCurLevel = 1;
			}
		}
	}

	// 이번에 지시할 Location 저장 
	m_pDoc->m_strLocation.Format(_T("%02d%03d%02d"), nCurBank, m_nCurBay, m_nCurLevel);

	// 이 뱅크가 어느 호기 것인지 기억해 둔다.
	//   위에서 호기마다 뱅크를 nMinBank = 호기*2-1, nMaxBank = 호기*2 로 잡았다.
	//   입고 작업 전문의 도착지에 이 번호를 싣는다.
	m_nCurScNum = (nCurBank + 1) / 2;

	return TRUE;
}
// n2ndStn은 이동일때만 사용됨!
// lpszLocation : 출고(2,3)일때 출발지 랙 위치 지정 (7자리, NULL이면 기존 방식)
// pScList      : 입고(1)일때 사용할 로직그룹의 SC 호기 목록 (NULL이면 전체 SC)
int CHostCl::JobOrder(int nJobType, int n1stStn, int n2ndStn, BOOL bManual, LPCTSTR lpszLocation, CStringArray* pScList)
{
	int nUserData = 0;
	CString strLog, strTempMsg, strStation, strRetLoc;
	
	memset(m_JobOrderMsg, 0x0, sizeof(m_JobOrderMsg));
	m_strLastOrderFail.Empty();		// @.이번에 못 내면 여기 사유를 담는다

	/*
	 * 작업번호는 HostSim.ini [JOB] LastLuggNum 에 남긴다.
	 *
	 *   전에는 static 1000 이라 프로그램을 켤 때마다 1001 부터 다시 매겼다.
	 *   ECS 의 JOB_MST 에 그 번호가 남아 있으면 (시험이 중간에 끊겨 남은 것 등)
	 *   첫 지시가 이미 지시된 작업 으로 거절되고, CHostCl::Parsing 의 NAK 처리가
	 *   그 번호를 슬롯에서 풀어 다음 주기에 새 번호로 다시 보낸다. 한 번 지시한 것이
	 *   두 번 나간 것처럼 보이던 것이 이것이다. (실제로 만들어진 작업은 하나다)
	 *
	 *   이어서 매기면 지난 번호를 다시 밟지 않는다. 재시도 처리 자체는 그대로 둔다.
	 *   같은 번호로 재시도하면 영영 거절되므로 새 번호로 가는 것이 맞다.
	 */
	static int nLuggNum = 0;

	if (nLuggNum == 0)
	{
		nLuggNum = ::GetPrivateProfileInt(_T("JOB"), _T("LastLuggNum"), 1000, ECS_INI_FILE);

		// @.INI 가 비었거나 손상됐으면 예전처럼 1000 부터 간다.
		if ((nLuggNum < 1000) || (nLuggNum > 8999))
			nLuggNum = 1000;
	}

	++nLuggNum;

	if (nLuggNum > 8999)
		nLuggNum = 1000;

	::WritePrivateProfileString(_T("JOB"), _T("LastLuggNum"), CConvert::ToString(nLuggNum), ECS_INI_FILE);
		
	CString strJobType = "";

	switch(nJobType)
	{
	case 1:		//			   1 2   3 4 5  6  7  8  9  10 11 
		/*
		 * @.내지 못하고 돌아가는 자리가 셋인데 셀 다 조용했다.
		 *   부르는 쪽은 0 만 받아 "입고 작업번호가 0" 으로만 보였다.
		 *   어느 자리에서 돌아섬는지 남긴다. 같은 사유가 계속 되풀이되는 것은
		 *   부르는 쪽(CEcsView::OnTimer)이 한 번만 적도록 막는다.
		 */
		if (n1stStn == 0)
		{
			m_strLastOrderFail = _T("출발 작업대가 0 입니다. 로직그룹의 ViaStns 를 확인하십시오.");
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, m_strLastOrderFail, _T("CHostCl::JobOrder"));
			return 0;
		}

		if (CalcurateLocation(TRUE, pScList) == FALSE)
		{
			// @.쓸 수 있는 크레인이 하나도 없었다는 뜻이다. 로직그룹의 Scs 가
			//   현장과 안 맞거나, 그 호기가 전부 오프라인(4) / 에러(5) / 입출고중지(8)
			//   이거나, 입고중지(6) 일 때 여기로 온다.
			CString strScs;
			if (pScList != NULL)
			{
				for (int k = 0; k < pScList->GetSize(); ++k)
				{
					int nSc = _ttoi(pScList->GetAt(k));
					CString strOne;
					strOne.Format(_T("%s%d(상태%d)"), (k == 0) ? _T("") : _T(","), nSc,
								  (nSc >= 1 && nSc <= SC_CNT) ? m_pDoc->m_nScStatus[nSc - 1] : -1);
					strScs += strOne;
				}
			}

			CString strLogNoSc;
			strLogNoSc.Format(_T("쓸 수 있는 크레인이 없습니다 [출발:%03d][호기:%s]. 상태 4=오프라인 5=에러 6=입고중지 8=입출고중지"),
							  n1stStn, (LPCTSTR)strScs);
			m_strLastOrderFail = strLogNoSc;
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLogNoSc, _T("CHostCl::JobOrder"));
			return 0;
		}

		// 첫 상태를 받지 않았을때 
		if (m_pDoc->m_bReceiveStatus == FALSE)
		{
			m_strLastOrderFail = _T("설비 상태전문(S)을 아직 받지 못했습니다.");
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, m_strLastOrderFail, _T("CHostCl::JobOrder"));
			return 0;
		}

		strStation = m_pDoc->m_strStoStation;

		m_pDoc->m_strStoLocation[nLuggNum] = m_pDoc->m_strLocation;

		// 일단 팔렛트 창고 기준으로
			//				  1 2 3   4 5    6   7  8   9   10 11  12  13 14   15
		strTempMsg.Format(_T("%d%c%04d%d%-20s%03d%7s%03d%03d%7s%04d%03d%1s%-20s%-20s"),
			nJobType,					// 1	// 작업구분
			'A',						// 2	// 창고구분 
			nLuggNum,					// 3	// 작업번호 
			1234,						// 4	// Seq
			_T("PALLET_ID_00000000  "),		// 5	// Pallet Size
			n1stStn,					// 6	// Source Station
			_T("0000000"),				// 7	// Source Bank, Bay, Level
			0,							// 8	// Route Station
			m_nCurScNum,				// 9	// Dest. Station (입고는 화물을 받을 S/C 번호)
			m_pDoc->m_strLocation,		// 10	// Dest. Bank, Bay, Level
			0,							// 11	// Pair 작업번호 
			100,						// 12	// Priority
			_T(" "),					// 13	// JobRoute
			_T("PRODUCT_ID_00000000 "),	// 14	// Product ID
			_T("USER_DATA_000000000 "));	// 15	// UserData

		strJobType = _T("입고");
			
		break;
	case 2:		
	case 3:		
		if (n1stStn == 0)
			return 0;
		strStation = m_pDoc->m_strRetStation;
		//if (CalcurateLocation(FALSE) == FALSE)
		//	return 0;

		// 출고 출발지 랙 위치 : 인자로 전달된 위치(입고 완료된 랙)를 우선 사용
		strRetLoc = m_pDoc->m_strLocation;
		if (lpszLocation != NULL && _tcslen(lpszLocation) == 7)
			strRetLoc = lpszLocation;

		m_pDoc->m_strRetLocation[nLuggNum] = strRetLoc;

		//	//				  1 2   3 4 5  6  7  8   9  10 11 
		//strTempMsg.Format(_T("%d%04d%d%d%3s%7s%3s%03d%7s%d %-20s "),
		//	nJobType,				// 1	// 작업구분
		//	nLuggNum,				// 2	// 작업번호 
		//	4321,					// 3	// Seq
		//	7654321,				// 4	// Pallet Size
		//	_T("000"),				// 5	// Source Station
		//	m_pDoc->m_strLocation,	// 6	// Source Bank, Bay, Level
		//	_T("000"),				// 7	// Route Station
		//	n1stStn,				// 8	// Dest. Station
		//	_T("0000000"),			// 9	// Dest. Bank, Bay, Level
		//	100,					// 10	// Priority
		//	_T("~blabla~"));		// 11	// Product ID

		// 일단 팔렛트 창고 기준으로
			//				  1 2 3   4 5    6   7  8   9   10 11  12  13 14   15
		strTempMsg.Format(_T("%d%c%04d%d%-20s%03d%7s%03d%03d%7s%04d%03d%1s%-20s%-20s"),
			nJobType,					// 1	// 작업구분
			'A',						// 2	// 창고구분 
			nLuggNum,					// 3	// 작업번호 
			1234,						// 4	// Seq
			_T("PALLET_ID_00000000  "),		// 5	// Pallet Size
			0,							// 6	// Source Station
			strRetLoc,					// 7	// Source Bank, Bay, Level
			0,							// 8	// Route Station
			n1stStn,					// 9	// Dest. Station
			_T("0000000"),				// 10	// Dest. Bank, Bay, Level
			0,							// 11	// Pair 작업번호 
			100,						// 12	// Priority
			_T(" "),					// 13	// JobRoute
			_T("PRODUCT_ID_00000000 "),	// 14	// Product ID
			_T("USER_DATA_000000000 "));	// 15	// UserData

		strJobType = _T("출고");
		break;
	case 4:		
	case 5:		
		//	//			   1 2   3 4 5  6  7  8  9  10 11 
		//strTempMsg.Format(_T("%d%04d%d%d%3s%7s%3s%3s%7s%d %-20s "),
		//	nJobType,				// 1	// 작업구분
		//	nLuggNum,				// 2	// 작업번호 
		//	5678,					// 3	// Seq
		//	3456789,				// 4	// Pallet Size
		//	_T("000"),				// 5	// Source Station
		//	m_pDoc->m_strFromPos,	// 6	// Source Bank, Bay, Level
		//	_T("000"),				// 7	// Route Station
		//	_T("000"),				// 8	// Dest. Station
		//	m_pDoc->m_strToPos,		// 9	// Dest. Bank, Bay, Level
		//	100,					// 10	// Priority
		//	_T("~blabla~"));		// 11	// Product ID

		// 일단 팔렛트 창고 기준으로
			//				  1 2 3   4 5    6   7  8   9   10 11  12  13 14   15
		strTempMsg.Format(_T("%d%c%04d%d%-20s%03d%7s%03d%03d%7s%04d%03d%1s%-20s%-20s"),
			nJobType,					// 1	// 작업구분
			'A',						// 2	// 창고구분 
			nLuggNum,					// 3	// 작업번호 
			1234,						// 4	// Seq
			_T("PALLET_ID_00000000  "),		// 5	// Pallet Size
			0,							// 6	// Source Station
			m_pDoc->m_strFromPos,		// 7	// Source Bank, Bay, Level
			0,							// 8	// Route Station
			0,							// 9	// Dest. Station
			m_pDoc->m_strToPos,			// 10	// Dest. Bank, Bay, Level
			0,							// 11	// Pair 작업번호 
			100,						// 12	// Priority
			_T(" "),					// 13	// JobRoute
			_T("PRODUCT_ID_00000000 "),	// 14	// Product ID
			_T("USER_DATA_000000000 "));	// 15	// UserData

		switch(nJobType)
		{
		case 4:		strJobType = _T("랙투랙");			break;
		case 5:		strJobType = _T("호기간 이동");		break;
		}

		break;
	case 6:		
		if (n1stStn == 0 || n2ndStn == 0)
			return 0;

		//	//			      1 2   3 4 5   6  7  8   9  10 11 
		//strTempMsg.Format(_T("%d%04d%d%d%03d%7s%3s%03d%7s%d %-20s "),
		//	nJobType,		// 1	// 작업구분
		//	nLuggNum,		// 2	// 작업번호 
		//	9876,			// 3	// Seq
		//	9876543,		// 4	// Pallet Size
		//	n1stStn,		// 5	// Source Station
		//	_T("0000000"),	// 6	// Source Bank, Bay, Level
		//	_T("000"),		// 7	// Route Station
		//	n2ndStn,		// 8	// Dest. Station
		//	_T("0000000"),	// 9	// Dest. Bank, Bay, Level
		//	100,			// 10	// Priority
		//	_T("~blabla~"));// 11	// Product ID


		// 일단 팔렛트 창고 기준으로
			//				  1 2 3   4 5    6   7  8   9   10 11  12  13 14   15
		strTempMsg.Format(_T("%d%c%04d%d%-20s%03d%7s%03d%03d%7s%04d%03d%1s%-20s%-20s"),
			nJobType,					// 1	// 작업구분
			'A',						// 2	// 창고구분 
			nLuggNum,					// 3	// 작업번호 
			1234,						// 4	// Seq
			_T("PALLET_ID_00000000  "),		// 5	// Pallet Size
			n1stStn,					// 6	// Source Station
			_T("0000000"),				// 7	// Source Bank, Bay, Level
			0,							// 8	// Route Station
			n2ndStn,					// 9	// Dest. Station
			_T("0000000"),				// 10	// Dest. Bank, Bay, Level
			0,							// 11	// Pair 작업번호 
			100,						// 12	// Priority
			_T(" "),					// 13	// JobRoute
			_T("PRODUCT_ID_00000000 "),	// 14	// Product ID
			_T("USER_DATA_000000000 "));	// 15	// UserData

		strJobType = _T("이동");
		break;
	}

	m_pDoc->m_nPrevLuggNum = nLuggNum;

	m_nJobOrderSize = strTempMsg.GetLength() + 3;
	
	wsprintf(m_JobOrderMsg,						_T("WMS_MBX   %04d "),	m_nJobOrderSize);
	wsprintf(m_JobOrderMsg + MSG_LENGTH_HEADER, _T("%c%c%s%c"),	STX, CMD_ORDER, strTempMsg, ETX);
	
	int nLenth =  sizeof(m_JobOrderMsg);
	
	if(	SendMsg(m_JobOrderMsg, m_nJobOrderSize + MSG_LENGTH_HEADER) == TRUE )
	{
		strLog.Format(_T("신규 %s 작업 %s 지시.. Location=[%s], Station=[%s]"), strJobType, (bManual) ? _T("수동") : _T(""), m_pDoc->m_strLocation, strStation);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::JobOrder"));
	}
	else
	{
		strLog.Format(_T("신규 %s 작업 %s 지시 실패.. Location=[%s], Station=[%s]"), strJobType, (bManual) ? _T("수동") : _T(""), m_pDoc->m_strLocation, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::JobOrder"));
		return 0;
	}
	
	return nLuggNum;
}


BOOL CHostCl::BarcodeData(CString strStation, CString strBarcodeData, BOOL bManual)
{
	TCHAR TxBuff[1024];
	int nUserData = 0;
	CString strLog;
	
	memset(TxBuff, 0x0, sizeof(TxBuff));
	
	wsprintf(TxBuff,					 _T("ECS_MBX   %04d "),	MSG_LENGTH_BARCODE_DATA);
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%3s%-31s%d%c"),	STX, enHostCommandBarcode, strStation.Mid(2, 3), strBarcodeData, nUserData, ETX);
	
	int nLenth =  sizeof(TxBuff);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_BARCODE_MSG) == TRUE )
	{
		strLog.Format(_T("BarCode Data %s 보고.. 데이터=[%s], Station=[%s]"),  (bManual) ? _T("수동") : _T(""), strBarcodeData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
	}
	else
	{
		strLog.Format(_T("BarCode Data %s 보고 실패.. 데이터=[%s], Station=[%s]"), (bManual) ? _T("수동") : _T(""), strBarcodeData, strStation);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::BarcodeData"));
		return FALSE;
	}
	
	return TRUE;
}

void CHostCl::Status(int nRegularCycle)
{
	
#define	SC_STATUS_NORMAL_WAIT			0
#define	SC_STATUS_STORING				1
#define	SC_STATUS_RETRIEVING			2
#define	SC_STATUS_RACK_TO_RACK			3
#define	SC_STATUS_NO_ONLINE				4
#define	SC_STATUS_ERROR					5
#define	SC_STATUS_SUSPEND_STO			6
#define	SC_STATUS_SUSPEND_RET			7
#define	SC_STATUS_SUSPEND_ALL			8
#define	SC_STATUS_SUSPEND_R2R			9
	
	int nCvStatus = 1, nScStatus = 0, nStoHSStatus = 1, nRetHSStatus = 1;
	
	CString strLog;
	TCHAR TxBuff[512];
	static	char	PrevBuff[512] = {0};
	memset(TxBuff, 0x0, sizeof(TxBuff));
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_STATUS_DATA);
	TxBuff[MSG_LENGTH_HEADER] = STX;
	TxBuff[MSG_LENGTH_HEADER+1] = enHostCommandStatus;
	
	int i = 0;
	for (i = 0; i < SC_PLC_TOT_CNT; i++)
	{
		int nTemp = (i / 3) + 1;
		CScDongJin* pSc = (CScDongJin*) m_pDoc->GetEquipment(CEquipment::enSC, nTemp);	
		DEBUGER_ASSERT_VALID(pSc != NULL);
		
		CScInfo *pScInfo = (CScInfo *) pSc->GetInfo(i + 1, TRUE);
		DEBUGER_ASSERT_VALID(pScInfo != NULL);
		
		nScStatus = 0;
//		BOOL IsInvoked() { return ((m_ScInfo[0].m_bInvoke == TRUE) || (m_ScInfo[1].m_bInvoke == TRUE)); }
		if (!pScInfo->IsInvoked() && pScInfo->IsScJobStatusIdle() == TRUE)
			nScStatus = SC_STATUS_NORMAL_WAIT;
		
		if (pScInfo->IsInvoked())
		{
			if (pScInfo->m_ScInfo[0].m_bInvoke == TRUE)
			{
				if (pScInfo->m_ScInfo[0].m_nInternalJobType == CScInfo::enRetrieve)
					nScStatus = SC_STATUS_RETRIEVING;
				else if (pScInfo->m_ScInfo[0].m_nInternalJobType == CScInfo::enStore)
					nScStatus = SC_STATUS_STORING;
				else if (pScInfo->m_ScInfo[0].m_nInternalJobType == CScInfo::enRack2Rack)
					nScStatus = SC_STATUS_RACK_TO_RACK;
			}
			else //if (pScInfo->m_ScInfo[1].m_bInvoke == TRUE)
			{
				if (pScInfo->m_ScInfo[1].m_nInternalJobType == CScInfo::enRetrieve)
					nScStatus = SC_STATUS_RETRIEVING;
				else if (pScInfo->m_ScInfo[1].m_nInternalJobType == CScInfo::enStore)
					nScStatus = SC_STATUS_STORING;
				else if (pScInfo->m_ScInfo[1].m_nInternalJobType == CScInfo::enRack2Rack)
					nScStatus = SC_STATUS_RACK_TO_RACK;
			}
		}

		/*
		strRcMode = pInfo->IsScJobStatusManualMode() ? "수동" : (pInfo->IsScJobStatusAutoMode() ? "자동" : "" );
		strScStatus = pInfo->IsScJobStatusIdle() ? "대기" : (pInfo->IsScJobStatusError() ? "이상" : (pInfo->IsScJobStatusReceiveCommand() ? "운전" : ""));
		strForkPos = pInfo->IsScJobStatusForkCenter() ? "CENTER" : (pInfo->IsScJobStatusForkLeft() ? "LEFT" : (pInfo->IsScJobStatusForkRight() ? "RIGHT" : ""));
		strHoist = pInfo->IsScJobStatusHoistUp() ? "UP" : (pInfo->IsScJobStatusHoistDown() ? "DOWN" : "");
		
		pInfo->IsScJobStatusRemoteMode() ? "REMOTE" : "LOCAL");			// 지상반			- D100.C
		pInfo->IsScJobStatusPowerOn() ? "ON" : "OFF");					// Active			- D100.D
		strRcMode);														// 기상반			- D100.B, D100.9
		strScStatus);													// 크래인 상태		- D100.0, D100.1, D100.E
		strForkPos);													// 포크위치			- D100.5, D100.6, D100.7
		pInfo->IsScJobStatusRegPos() ? "주행 정위치" : "");				// 주행 정위치		- D100.2				
		strHoist);														// HOIST			- D100.3, D100.4
		pInfo->IsScJobStatusSensorProduct() ? "화물있음" : "화물없음");	// 화물유무			- D100.8
		pInfo->IsScJobStatusJobComplete() ? "작업완료" : "");			// 완료상태			- D100.F
		pInfo->m_wHorizontalPos);										// 주행				- D102
		pInfo->m_wVerticalPos);											// 승강				- D103
*/

//		if (pScInfo->IsScJobStatusIdle() == FALSE)
//			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->IsScJobStatusRcAutoMode() == TRUE || pScInfo->IsScJobStatusScAutoMode() == TRUE)
			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->IsScJobStatusRcManualMode() == TRUE || pScInfo->IsScJobStatusScManualMode() == TRUE)
			nScStatus = SC_STATUS_NO_ONLINE;
//		if (pScInfo->m_ucScMode != CScInfo::enActive)
//			nScStatus = SC_STATUS_NO_ONLINE;
		if (pScInfo->IsScJobStatusError() == TRUE)
			nScStatus = SC_STATUS_ERROR;
		if (pScInfo->m_bStoSuspend)
			nScStatus = SC_STATUS_SUSPEND_STO;
		if (pScInfo->m_bRetSuspend)
			nScStatus = SC_STATUS_SUSPEND_RET;
		if (pScInfo->m_bR2RSuspend)
			nScStatus = SC_STATUS_SUSPEND_R2R;
		if (pScInfo->m_bStoSuspend && pScInfo->m_bRetSuspend /*&& pScInfo->m_bR2RSuspend*/)
			nScStatus = SC_STATUS_SUSPEND_ALL;
		
		int nLugg = pScInfo->m_ScInfo[0].m_nInternalLuggNum;
//		nScStatus = i;
//		nLugg = ((i+1)*1000)+((i+1)*100)+((i+1)*10)+(i+1);

//		// TEST
//		if(i % 2 == 0)
//		{
//			nScStatus = 0;
//			nLugg = 2222;
//		}
//		else
//		{
//			nScStatus = 2;
//			nLugg = 3333;		
//		}
//		
		wsprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * i), _T("%02d%d%04d"),
			i+1, nScStatus, nLugg);
	}
	
	int nStations[] = {	101, 102, 103, 104, 105, 106, 107, 108, 113,  
						201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
						211, 212, 213, 214, 215, 216, 217, 
						221, 222, 223, 224, 225, 226,  
						227, 228, 229, 230, 231, 232 };

	CStationInfo* pStation = NULL;

	int nStnCnt = 38;
	for (i=0; i<nStnCnt; ++i)
	{
		pStation = m_pDoc->GetStationInfo(CConvert::ToString(nStations[i]));
		DEBUGER_ASSERT_VALID(pStation != NULL);
		
//		if (pStation->m_enKind != CStationInfo::enScStation)
//		{
			BOOL bStatus = FALSE;

			if (nStations[i] == 214)
				int a=0;

			// 입고대 상태만을 올려주는 것으로 합의함!!			
			switch(pStation->m_enKind)
			{
			case CStationInfo::enStoStation: 	bStatus	= pStation->m_pTrack->IsStoStationReady();	break;	
//			case CStationInfo::enRetStation: 	bStatus	= pStation->m_pTrack->IsStoStationReady();	break;	
			case CStationInfo::enArvStation: 	bStatus	= pStation->m_pTrack->IsRetStationReady();	break;	
			}

			if (pStation->m_pTrack->m_nNumber == 3119 || 
				pStation->m_pTrack->m_nNumber == 3219 || 
				pStation->m_pTrack->m_nNumber == 3319 ||
				pStation->m_pTrack->m_nNumber == 3411  )
			{
				bStatus = pStation->m_pTrack->IsStoStationReady();
			}

	//		// TEST
	//		if(i % 2 == 1)
	//		{
	//			bTemp = 1;
	//		}
			
			wsprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_TOT_CNT) + i, _T("%d"), bStatus);
//			nStnCnt++;
//		}
	}
	
	/*
	int StoStations[] = { 111, 110, 113, 116, 119, 214, 211, 215, 314, 311, 315 };
	BOOL bTemp;
	for (i = 0; i < IMS_STN_CNT; i++)
	{
		CTrackInfo* pTrack = m_pDoc->GetTrackInfo(StoStations[i]);
		DEBUGER_ASSERT_VALID(pTrack != NULL);
	
		bTemp = pTrack->IsStoStationReady();
	  
//		// TEST
//		if(i % 2 == 1)
//		{
//			bTemp = 1;
//		}

//		if(pTrack->m_nNumber == 111)
//			bTemp = 1;
		  
		sprintf(TxBuff + MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_CNT) + i, "%d", bTemp);
	}
	*/

	TxBuff[MSG_LENGTH_HEADER + 2 + (7 * SC_PLC_TOT_CNT) + nStnCnt] = ETX;
	
	int rt = memcmp(PrevBuff, TxBuff, MSG_LENGTH_STATUS_MSG);
	if(	SendMsg(TxBuff, MSG_LENGTH_STATUS_MSG) == FALSE )
	{
		strLog.Format(_T("상태 보고 송신 실패.. 에러=[%s]"), CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::Status"));
	}
	else 
	{
		strLog.Format(_T("상태 보고 .. [%s][%s]"), TxBuff, CLib::GetSystemErrMsg());
//		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_HOST, strLog, _T("CHostCl::Status"));
		memcpy(PrevBuff, TxBuff, MSG_LENGTH_STATUS_MSG);
	}
}

void CHostCl::JobRequest(int nStationNo, int nJobKind, BOOL bManual)
{
	if (nJobKind != 1 && nJobKind != 2)
		return; 

	TCHAR TxBuff[256];
	memset(TxBuff, 0x20, sizeof(TxBuff));
	CString strLog;
	
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_PALLET_DATA);//64
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%d%03d%c%c"),
		STX, CMD_PALLET, nJobKind, nStationNo, 0x20, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_PALLET_MSG) == FALSE )
	{
		strLog.Format(_T("%s 공파레트 %s 요청 송신 실패.. 에러=[%s]"),
			(bManual) ? _T("수동") : _T(""),
			(nJobKind == 1) ? _T("입고") : ((nJobKind == 2) ? _T("출고") : _T("이동")),
			CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::JobRequest"));
		return;
	}
	
	strLog.Format(_T("%s 공파레트 %s 요청 송신.. Station=[%d] [%s] "),
		(bManual) ? _T("수동") : _T(""),
		(nJobKind == 1) ? _T("입고") : ((nJobKind == 2) ? _T("출고") : _T("이동")),
		nStationNo, 
		TxBuff);
	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::JobRequest"));
}

void CHostCl::StoJobRequest(int nStationNo, int nJobQty, CString strPalletID, BOOL bManual)
{
	if (nStationNo < 100 && nStationNo > 999)
		return; 

	TCHAR TxBuff[256];
	memset(TxBuff, 0x20, sizeof(TxBuff));
	CString strLog;
	
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_REQSTORE_DATA);//64
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%03d%12s%010d%c"),
		STX, CMD_REQ_STO, nStationNo, strPalletID, nJobQty, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_REQSTORE_MSG) == FALSE )
	{
		strLog.Format(_T("%s 입고작업 요청 송신 실패.. 에러=[%s]"),
			(bManual) ? _T("수동") : _T(""),
			CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::StoJobRequest"));
		return;
	}
	
	strLog.Format(_T("%s 입고작업 요청 송신.. Station=[%d] [%s] "),
		(bManual) ? _T("수동") : _T(""),
		nStationNo, 
		TxBuff);

	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::StoJobRequest"));
}

void CHostCl::RetJobRequest(int nStationNo, int nLuggNum, BOOL bManual)
{
	if (nStationNo < 100 && nStationNo > 999)
		return; 

	TCHAR TxBuff[256];
	memset(TxBuff, 0x20, sizeof(TxBuff));
	CString strLog;
	
	wsprintf(TxBuff, _T("ECS_MBX   %04d "), MSG_LENGTH_REQRETRIEVE_DATA);//64
	wsprintf(TxBuff + MSG_LENGTH_HEADER, _T("%c%c%03d%04d%c"),
		STX, CMD_LOAD_ARRV, nStationNo, nLuggNum, ETX);
	
	if(	SendMsg(TxBuff, MSG_LENGTH_REQRETRIEVE_MSG) == FALSE )
	{
		strLog.Format(_T("%s 출고작업 요청 송신 실패.. 에러=[%s]"),
			(bManual) ? _T("수동") : _T(""),
			CLib::GetSystemErrMsg());
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_HOST, strLog, _T("CHostCl::StoJobRequest"));
		return;
	}
	
	strLog.Format(_T("%s 출고작업 요청 송신.. Station=[%d] [%s] "),
		(bManual) ? _T("수동") : _T(""),
		nStationNo, 
		TxBuff);

	m_pDoc->WriteLog(LOG_TYPE_JOB, LOG_POS_HOST, strLog, _T("CHostCl::StoJobRequest"));
}

int CHostCl::GetClientSeqNum()
{
	if (m_nClientSeqNum > 999999 || m_nClientSeqNum < 1)
		m_nClientSeqNum = 1;
	
	return  m_nClientSeqNum ++;
}

BOOL CHostCl::SendMsg(TCHAR* pTxBuff, int nSndMsgLen)
{
	
	if (m_pDoc->m_pHostCl != NULL)
	{
		//int nTemp = Send(pTxBuff, nSndMsgLen);
		//BOOL bTemp = (nTemp == nSndMsgLen);
		//return bTemp;
		UpdateCommCl(NOTIFY_SEND);

#ifdef _UNICODE
		// Unicode -> ANSI 변환
		int nAnsiLen = WideCharToMultiByte(CP_ACP, 0, pTxBuff, nSndMsgLen, NULL, 0, NULL, NULL);
		char* pAnsiBuff = new char[nAnsiLen + 1];
		memset(pAnsiBuff, 0x00, nAnsiLen + 1);

		WideCharToMultiByte(CP_ACP, 0, pTxBuff, nSndMsgLen, pAnsiBuff, nAnsiLen, NULL, NULL);

		int nTemp = Send(pAnsiBuff, nAnsiLen);

		delete[] pAnsiBuff;
		return (nTemp == nAnsiLen);
#else
		// MBCS면 그대로 전송
		int nTemp = Send(pTxBuff, nSndMsgLen);
		return (nTemp == nSndMsgLen);
#endif
	}
	
	return FALSE;
}
