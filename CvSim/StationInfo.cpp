// StationInfo.cpp: implementation of the CStationInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "StationInfo.h"
#include "Equipment.h"
#include "TrackInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CStationInfo, CObject)

CStationInfo::CStationInfo(EN_KIND enKind, CEquipment* pEquipment, CTrackInfo* pTrack /* = NULL */)
{
	m_enKind = enKind;
	m_pEquipment = pEquipment;
	m_pTrack = pTrack;

//	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
}

CStationInfo::~CStationInfo()
{

}

BOOL CStationInfo::IsValidID(CString& strStationID)
{
	if (strStationID.GetLength() == 1 || strStationID.GetLength() == g_nLenStationID)
		return TRUE;

	return FALSE;
//	return (strStationID.GetLength() == g_nLenStationID);
}

int CStationInfo::GetWarehouseNo(CString& strStationID)
{
//	return (strStationID.GetLength() == 5) ? _ttoi(strStationID.Mid(1,1)) : 0;
	return 1;
}

int CStationInfo::GetTrackDestination()
{
	if (m_pTrack == NULL)
	{
		DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
		DEBUGER_ASSERT_VALID(m_pEquipment->m_enKind == CEquipment::enSC);
		//DEBUGER_ASSERT_VALID(m_strID.GetLength() == g_nLenStationID);

		int nTemp = (_ttoi(m_strID.Right(4)));

		if (m_strID.GetLength() == 3)
			nTemp = _ttoi(m_strID);

		/*
		 * 크레인 목적지는 호기 번호로 쓴다.
		 *
		 *   스테이션 ID 는 901~911 이지만, 설비(PLC)에 실리는 목적지 자리는
		 *   한 바이트라 그 값이 들어가지 못한다. WCS 도 CV 에 지시할 때
		 *   9NN 을 호기 번호로 되돌려 보낸다.
		 *   (WCS_IO_SCH_Original 의 GfCvDestPos)
		 *
		 *   여기서 901 을 그대로 돌려주면 트랙의 목적지 표(m_nStationArray)에
		 *   9NN 이 들어가고, 화물 이동이 그 표와 레지스터 값을 견주는데
		 *   레지스터에는 호기 번호가 들어 있어 영영 맞지 않는다.
		 *   그러면 화물이 다음 트랙으로 넘어가지 못한다.
		 */
		if ((nTemp > 900) && (nTemp < 1000))
			nTemp -= 900;

		return nTemp;
	}

	if(_ttoi(m_strID) != 0)
		return _ttoi(m_strID);
	return m_pTrack->m_nNumber;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CStationInfoArray::IsValidStartStation(int nJobPattern, CString& strStationID)
{
	if (CStationInfo::IsValidID(strStationID) == FALSE)
		return FALSE;

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		switch (nJobPattern)
		{
		case enJobPatternSto:
		case enJobPatternMove:
			if ((pStation->m_enKind != CStationInfo::enStoStation) &&
				(pStation->m_enKind != CStationInfo::enArvStation))
				continue;
			break;

		case enJobPatternRet:
		case enJobPatternPR:
		case enJobPatternR2R:
		case enJobPatternW2W:
			if (pStation->m_enKind != CStationInfo::enScStation)
				continue;
			break;

		default:
			DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
			continue;
		}

		if (pStation->m_strID == strStationID)
			return TRUE;
	}

	return FALSE;
}

BOOL CStationInfoArray::IsValidDestStation(int nJobPattern, CString& strStationID)
{
	if (CStationInfo::IsValidID(strStationID) == FALSE)
		return FALSE;

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		switch (nJobPattern)
		{
		case enJobPatternSto:
		case enJobPatternW2W:
		case enJobPatternR2R:
			if (pStation->m_enKind != CStationInfo::enScStation)
				continue;
			break;

		case enJobPatternRet:
		case enJobPatternPR:
			if ((pStation->m_enKind != CStationInfo::enArvStation) && 
				(pStation->m_enKind != CStationInfo::enRetStation))
				continue;
			break;

		case enJobPatternMove:
			if ((pStation->m_enKind != CStationInfo::enArvStation) && 
				(pStation->m_enKind != CStationInfo::enRetStation))
				continue;
			break;
	
		default:
			DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
			continue;
		}

		if (pStation->m_strID == strStationID)
			return TRUE;
	}

	return FALSE;
}

CStationInfo* CStationInfoArray::GetStationInfo(CString& strStationID)
{
	//DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStationID), strStationID);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->m_strID == strStationID)
			return pStation;
	}

	return NULL;
}

CStationInfo* CStationInfoArray::GetScStationInfo(int nWarehouse, CString& strLocation)
{
	DEBUGER_ASSERT_VALID_LOG(CLib::IsValidLocation(nWarehouse, strLocation), strLocation);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);
		DEBUGER_ASSERT_VALID(pStation->m_pEquipment != NULL);

		if (pStation->m_enKind != CStationInfo::enScStation)
			continue;

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		DEBUGER_ASSERT_VALID(pStation->m_pEquipment->m_enKind == CEquipment::enSC);
		if (pStation->m_pEquipment->m_nNumber == CLib::GetStackerNum(nWarehouse, strLocation))
			return pStation;
	}

	return NULL;
}

CString CStationInfoArray::GetStationName(CString& strStationID)
{
	DEBUGER_ASSERT_VALID_LOG(CStationInfo::IsValidID(strStationID), strStationID);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pStation = m_pData[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->m_strID == strStationID)
			return pStation->m_strName;
	}

	return _T("");
}