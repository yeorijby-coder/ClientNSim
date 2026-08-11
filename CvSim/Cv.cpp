// Cv.cpp: implementation of the CCv class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Cv.h"
#include "ByteArrayEx.h"

#include "MainFrm.h"
#include "EcsView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCv, CEquipment)

CCv::CCv(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice) : CEquipment(pDoc, nIndex, nNumber, lpszDevice)
{
	m_enKind = CEquipment::enCV;
	m_pInfo = new CCvInfo(this);
	m_bStoreRequesting = FALSE;
	m_bRetrieveRequesting = FALSE;
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_nPlcNum = nNumber;
}

CCv::~CCv()
{
	delete m_pInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::AutoRunProc(int nConnNum)
{
//	UpdateCommStatus(NOTIFY_SEND);

	CheckRequest(nConnNum);

	// @.여기서 재우면 슬롯 수만큼 누적되어 응답이 밀린다.
	//   (18슬롯 x 10ms = 180ms) 루프 주기는 ThreadProc 바깥 Sleep 이 맡는다.
}

//=============================================================================
//	Desc	: 물류흐름제어 모드 업데이트
//			  물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 
//			  Tray를 물류 Loop line으로 투입하지 않음.
//	Date	:
//	Update	: 
//=============================================================================

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CCv::CheckRequest(int nConnNum)
{
	if (!IsConnect(nConnNum))
	{
		m_strLog.Format(_T("%s과(와) 통신연결이 끊어졌습니다!"), DEVICE);
		return FALSE;
	}
		
	CByteArray arrBuffer;
	//CWordArray arrBuffer;

	
	// PLC 번호가 레지스터 배열 범위 안인지 본다.
	if (m_nPlcNum < 1 || m_nPlcNum > CV_PLC_CNT)
	{
		return FALSE;
	}

	// 소켓을 지역 변수에 한 번만 담아서 쓴다.
	//   통신이 끊기면 OnClose 가 m_pSocket[nConnNum] 을 NULL 로 만든다.
	//   (객체를 지우지는 않으므로 담아 둔 포인터는 계속 유효하다)
	//   예전에는 쓸 때마다 배열에서 다시 읽었다. 그래서 아래에서 프레임을
	//   받는 사이에 상대가 끊으면 그 다음 줄부터 NULL 을 건드려 죽었다.
	//   접근 위반 0xC0000005 - 실제로 여러 번 여기서 죽었다.
	CInterfaceSk* pSocket = m_pSocket[nConnNum];
	if (pSocket == NULL)
	{
		return FALSE;
	}

	if (pSocket->CheckRequest(arrBuffer) == FALSE)
	{
		return FALSE;
	}

	// 프레임을 받는 사이에 끊겼으면 응답하지 않는다.
	if (m_pSocket[nConnNum] != pSocket)
	{
		return FALSE;
	}

	// 헤더를 짚기 전에 프레임이 온전한지 본다.
	// 조각난 프레임을 그대로 읽으면 배열 범위를 넘는다.
	if (arrBuffer.GetSize() < 21)		// [20] 까지 읽는다
	{
		return FALSE;
	}

	int nHi = arrBuffer[12];
	int nLow = arrBuffer[11];
	int nnLen = (arrBuffer[8] << 8 ) | arrBuffer[7];
	int nCommand = (arrBuffer[12] << 8) | arrBuffer[11];
	int nStartaddress = (arrBuffer[17] << 16) | (arrBuffer[16] << 8) | (arrBuffer[15]);
	int nDevice = arrBuffer[18];
	int nLen = (arrBuffer[20] << 8) | arrBuffer[19];
	int aa = arrBuffer[19];
	if (nStartaddress > 10000)
		return FALSE;

	const int nRegSize = m_pDoc->m_arrRegData[m_nPlcNum - 1].GetTotalWordSize();

	arrBuffer[9] = nLen;

	pSocket->m_bWriteLog = m_port.m_bWriteLog;

	if (nCommand == 0x0401)
	{
		arrBuffer.SetSize(nLen*2);
		for (int i=0; i<nLen; ++i)
		{
			// 범위 밖은 0 으로 답한다. 버리면(응답을 안 보내면)
			// 소켓 계층이 다음 요청부터 무너져 프로세스가 죽는다.
			int nAddr = nStartaddress + i;
			WORD wTemp = 0;
			if (nAddr >= 0 && nAddr < nRegSize)
				wTemp = m_pDoc->m_arrRegData[m_nPlcNum - 1][nAddr];
			BYTE bTemp1 = CLib::GetByteH(wTemp);
			BYTE bTemp2 = CLib::GetByteL(wTemp);

			arrBuffer[(i*2)+1]	= bTemp1;
			arrBuffer[i*2]		= bTemp2;
		}
 
		if (pSocket->ResponseReadWord(arrBuffer, nLen) == FALSE)
		{
			return FALSE;
		}
	}
	else if (nCommand == 0x1401)
	{
		// 쓸 값이 다 왔는지 본다. 본문은 [21] 부터 워드당 2바이트다.
		if (arrBuffer.GetSize() < (21 + nLen * 2))
		{
			return FALSE;
		}

		for (int i=0; i<nLen; ++i)
		{
			int aaa = arrBuffer[22+i];
			int bbb = arrBuffer[21+i];
			int nAddr = nStartaddress + i;
			if (nAddr < 0 || nAddr >= nRegSize)
				continue;		// 범위 밖은 건너뛴다

			m_pDoc->m_arrRegData[m_nPlcNum-1][nAddr] = (arrBuffer[22+(i*2)] << 8) | arrBuffer[21+(i*2)];
			int nLuggNum = m_pDoc->m_arrRegData[m_nPlcNum-1][nAddr];

			if(i ==  2 ||i ==  3)
				int aaaaa= 0;
		}

		if (pSocket->ResponseWriteWord(arrBuffer, nLen) == FALSE)
		{
			return FALSE;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCv::InvokeStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;

	int nAAA, nBBB, nCCC;

	switch (pStation->m_enKind)
	{
	case CStationInfo::enStoStation:

		nAAA = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];
		nCCC = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0 &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) != enBit12)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
		}
		else if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0 &&
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit12) == enBit12)
		{
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] ^ enBit12;
			nBBB = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7];
		}
		break;

	case CStationInfo::enArvStation:
	case CStationInfo::enRetStation:
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) ||
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == enBit13)
			return;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;

		break;
	}
}

void CCv::MoveNextTrackForKindNormal(CTrackInfo* pTrack)
{
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;
	int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

	BOOL bAaa,bBbb;

	if (!(m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum] == 0 && m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0))
		return;

	if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
		return;
	
	if (!(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == TRUE)))
		return;

	if (((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE))
	{
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum]   = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+1] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+2] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+3] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3];
		m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+4] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^ enBit11;
	}
	else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
		pTrack->m_bStartFlag != TRUE)
	{
		pTrack->m_bStartFlag = TRUE;
		pTrack->m_tTime = COleDateTime::GetCurrentTime();
	}
	else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
		pTrack->m_bStartFlag == TRUE)
	{
		COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
		if (tElapseTime.GetTotalSeconds() > 3)
		{
			pTrack->m_bStartFlag = FALSE;
			m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] | enBit11;
		}
	}
}

void CCv::MoveNextTrackForKindDiverter(CTrackInfo* pTrack)
{
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;
	int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

	BOOL bAaa,bBbb;

	int nLen = pTrack->m_nStationArray.GetSize();
	
	for (int i=0; i<nLen; i++)
	{
		int nStation = pTrack->m_nStationArray[i];
		int nDest = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
		int nNextDevNum = (pTrack->m_nNextCv-m_nStTrNum+1)*10;

		if (pTrack->m_nStationArray[i] == m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1])
		{
			int nNextTrNum = pTrack->m_nNextTrArray[i];
			int nNextDevNum = (nNextTrNum-m_nStTrNum+1)*10;
			if (!(m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum] == 0 && m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] != 0))
				return;
		
			if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
				return;

			if (!(((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit11) == TRUE)))
				return;

			if (((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == TRUE) && ((m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE))
			{
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum]   = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+1] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+2] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+3] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3];
				m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+4] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4];

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+2] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+3] = 0;
				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+4] = 0;

				m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] ^ enBit11;
			}
			else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
				pTrack->m_bStartFlag != TRUE)
			{
				pTrack->m_bStartFlag = TRUE;
				pTrack->m_tTime = COleDateTime::GetCurrentTime();
			}
			else if ((((bAaa = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] & enBit11) == FALSE) && ((bBbb = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+7] & enBit11) == TRUE)) &&
				pTrack->m_bStartFlag == TRUE)
			{
				COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;
				if (tElapseTime.GetTotalSeconds() > 3)
				{
					pTrack->m_bStartFlag = FALSE;
					m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nNextDevNum+8] | enBit11;
				}
			}
		}
	}
}

void CCv::InvokeRetStation(CTrackInfo* pTrack, CStationInfo* pStation)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pStation != NULL);
	if (pTrack == NULL || pStation == NULL)	return;
	int nDevNum = (pTrack->m_nNumber-m_nStTrNum+1)*10;

	switch (pStation->m_enKind)
	{
	case CStationInfo::enRetStation:
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum] == 0) ||
			((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13) == TRUE))
			return;

		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit12;
		m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+8] | enBit11;
		break;

	case CStationInfo::enArvStation:
		/*
		if ((m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == 0) ||
			(m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] & enBit13 == TRUE))
			return;

		if (m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+1] == pTrack->m_nNumber)
			m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] = m_pDoc->m_arrRegData[m_nNumber-1][nDevNum+7] | enBit13;
		*/
		break;
	}
}

void CCv::InvokeScRetHS(CTrackInfo* pTrack, CTrackHS* pTrackHS)
{
	DEBUGER_ASSERT_VALID(pTrack != NULL);
	DEBUGER_ASSERT_VALID(pTrackHS != NULL);

	if ((pTrack->m_nLuggNum != 0) ||
		(pTrack->IsScRetHsReady() != FALSE))
		return;

	CString strLog;
	CJobItem* pJobItem = m_pDoc->m_pJob->FetchScCompleteJob(pTrack->m_nNumber);
	if (pJobItem == NULL)
		return;

//================================================================================================================================================
//	// Test 모드일때 입고작업을 출고 HS에 기록한다.
//================================================================================================================================================
	// TEST 모드일 때
	if (m_pDoc->m_bTesting == TRUE)
	{
		pJobItem->SetJobStatus(enJobStatusCvInvoke);
		CJobItem* pTestJobItem = m_pDoc->m_pJob->FetchByTestStoJob(m_pDoc->m_strTestLocation);
		if (pTestJobItem == NULL)
		{
			m_pDoc->m_bTesting = FALSE;
			return;
		}

		pJobItem = pTestJobItem;
	}
//------------------------------------------------------------------------------------------------------------------------------------------------
//=================================================================================================================
//	// Dead Lock 을 체크하기 위해서 (추가)
//=================================================================================================================
//	// DeadLock Checking 중일때  
//	int nDeadLock = 0;
//	if (pTrack->m_ucGenCode != 0)
//	{
//		nDeadLock = 1;
//	}
//-----------------------------------------------------------------------------------------------------------------
}