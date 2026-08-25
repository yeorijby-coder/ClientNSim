// ScInfo.cpp: implementation of the CScInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ScInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// 
//
IMPLEMENT_SERIAL(CScInfo, CInfo, 0)

CScInfo::CScInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pSC_DATA = NULL;
}

CScInfo::~CScInfo()
{
	//@@@@
	if (m_pSC_DATA != NULL)
		delete m_pSC_DATA;
}

void CScInfo::Serialize(CArchive& ar)
{
}

COLORREF CScInfo::GetForkColor1(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pSC_DATA->V_ERR_CODE_RD != _T("0") && pSC_DATA->V_ERR_CODE_RD != _T("0000") && !pSC_DATA->V_ERR_CODE_RD.IsEmpty())
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ERR_STA_FK1_RD != _T("0") && !pSC_DATA->V_ERR_STA_FK1_RD.IsEmpty())
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pSC_DATA->V_ONLINE_MODE_RD == _T("0") ||
		pSC_DATA->V_AUTO_MODE_RD   == _T("0") ||
		pSC_DATA->V_ACTIVE_MODE_RD == _T("0"))
		return DARK_GRAY;

	if (pSC_DATA->V_LUGG_NO != "0")
	{
		int nJobTypTmp = CConvert::ToInt(pSC_DATA->V_JOB_TYP);
		switch (nJobTypTmp)
		{
		case enJobTypeAutoSto: return pConfig->m_clrUSER_COLOR_STO;
		case enJobTypeAutoRet: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeAutoR2R: return pConfig->m_clrUSER_COLOR_RTR;
		case enJobTypeDuplicate: return pConfig->m_clrUSER_COLOR_RTR;
		case enJobTypeAutoMove: return pConfig->m_clrUSER_COLOR_MOVE;
		case enJobTypeAutoA2A: return pConfig->m_clrUSER_COLOR_ATA;
		case enJobTypeAutoPR: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeSemiSto: return pConfig->m_clrUSER_COLOR_STO;
		case enJobTypeSemiRet: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeSemiR2R: return pConfig->m_clrUSER_COLOR_RTR;
		case enJobTypeSemiMove: return pConfig->m_clrUSER_COLOR_MOVE;
		case enJobTypeSemiPR: return pConfig->m_clrUSER_COLOR_RET;
		case enJobTypeManual: return pConfig->m_clrUSER_COLOR_MANUAL;
		}
	}

	return LIGHT_GRAY;
}

COLORREF CScInfo::GetForkColor2()
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_pSC_DATA->V_IS_ERROR_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (m_pSC_DATA->V_ONLINE_MODE_RD != _T("0") || m_pSC_DATA->V_AUTO_MODE_RD != _T("0") || m_pSC_DATA->V_ACTIVE_MODE_RD != _T("0"))
	if (m_pSC_DATA->V_CRANE_ONLINE_RD != _T("0") || m_pSC_DATA->V_CRANE_REQUEST_RD != _T("0") || m_pSC_DATA->V_ORDER_CHECK_RD != _T("1"))
		return DARK_GRAY;

	if (m_pSC_DATA->V_ITN_LUGG == _T("0"))
		return LIGHT_GRAY;

	int nJobTypTmp = CConvert::ToInt(m_pSC_DATA->V_JOB_TYP);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoA2A: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoMove: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	return BLACK;
}

COLORREF CScInfo::GetForkColor2(CSC_DATA* pSC_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (m_pSC_DATA->V_IS_ERROR_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	//if (pSC_DATA->V_ONLINE_MODE_RD == _T("0") || pSC_DATA->V_AUTO_MODE_RD == _T("0") || pSC_DATA->V_ACTIVE_MODE_RD == _T("0"))
	if (m_pSC_DATA->V_CRANE_ONLINE_RD == _T("0") || m_pSC_DATA->V_CRANE_REQUEST_RD == _T("0"))
		return DARK_GRAY;

	int nJobTypTmp = CConvert::ToInt(pSC_DATA->V_JOB_TYP);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoA2A: return pConfig->m_clrUSER_COLOR_ATA;
	case enJobTypeAutoMove: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	//if (pSC_DATA->V_ONLINE_MODE_RD == _T("1") &&	pSC_DATA->V_AUTO_MODE_RD == _T("1") &&	pSC_DATA->V_ACTIVE_MODE_RD == _T("1"))
	if (m_pSC_DATA->V_CRANE_ONLINE_RD == _T("1") && m_pSC_DATA->V_CRANE_REQUEST_RD == _T("1") && m_pSC_DATA->V_ORDER_CHECK_RD == _T("0"))
		return LIGHT_GRAY;
	else
		return DARK_GRAY;

	return BLACK;
}


COLORREF CScInfo::GetForkColor()
{
	// 	if (m_nErrorCode == enScErrorCommunication)
	// 		return pConfig->m_clrDisconnect;
	// 
	// 	if (IsErrorStatus())
	// 		return IsAlarmCode() ? pConfig->m_clrAlarm : pConfig->m_clrError;
	// 
	// 	if ((IsOnlineMode() == FALSE) || (IsAutoMode() == FALSE) || (IsActive() == FALSE))
	// 		return DARK_GRAY;
	// 
	// 	if (IsInvoked() == FALSE)
	// 		return LIGHT_GRAY;
	// 
	// 	switch (m_nInternalJobType)
	// 	{
	// 	case enJobTypeAutoSto			: return pConfig->m_clrAutoSto;
	// 	case enJobTypeAutoRet			: return pConfig->m_clrAutoRet;
	// //	case enJobTypeAutoPriorityRet	: return pConfig->m_clrAutoPriorityRet;
	// 	case enJobTypeAutoR2R			: return pConfig->m_clrAutoR2R;
	// 	case enJobTypeAutoMove			: return pConfig->m_clrAutoMove;
	// 	case enJobTypeAutoPR			: return pConfig->m_clrAutoW2W;
	// 	case enJobTypeSemiSto			: return pConfig->m_clrSemiSto;
	// 	case enJobTypeSemiRet			: return pConfig->m_clrSemiRet;
	// 	case enJobTypeSemiR2R			: return pConfig->m_clrSemiR2R;
	// 	case enJobTypeSemiMove			: return pConfig->m_clrSemiMove;
	// 	case enJobTypeSemiPR			: return pConfig->m_clrSemiW2W;
	// 	case enJobTypeManual			: return pConfig->m_clrManual;
	// 	}

	return BLACK;
}

COLORREF CScInfo::GetRailColor()
{
	if (m_pSC_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	if (m_pSC_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (m_pSC_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (m_pSC_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;


	return (m_pSC_DATA->V_ITN_LUGG != _T("0") || m_pSC_DATA->V_ITN_LUGG != _T("0000")) ? BLUE : BLACK;
	//RAIL 색상
}

COLORREF CScInfo::GetRailColor(CSC_DATA* pSC_DATA)
{
	if (pSC_DATA == NULL)
		DEBUGER_ASSERT_VALID(FALSE);

	if (pSC_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND;

	if (pSC_DATA->V_SUSPEND == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND;

	if (pSC_DATA->V_SUSPEND == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (pSC_DATA->V_ERR_CODE_RD != _T("0") && pSC_DATA->V_ERR_CODE_RD != _T("0000") && !pSC_DATA->V_ERR_CODE_RD.IsEmpty())
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR;

	if (pSC_DATA->V_JOB_TYP == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO;

	if (pSC_DATA->V_JOB_TYP == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET;

	if (pSC_DATA->V_JOB_TYP == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE;

	if (pSC_DATA->V_JOB_TYP == _T("4") || pSC_DATA->V_JOB_TYP == _T("5") || pSC_DATA->V_JOB_TYP == _T("6"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RTR;

	return (pSC_DATA->V_ITN_LUGG != _T("0")) ? m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_SC_INVK : BLACK;
}

COLORREF CScInfo::GetPostColor()
{
	/*if (m_nForkType == 1 || m_nForkType == 2)
	{
		return (m_ucProdLoad != enProdEmpty) ? BLUE : DARK_GRAY;
	}
	else
		return (m_ucProdLoad == enProdExistFork1) ? BLUE : DARK_GRAY;*/

	return BLUE;
}

void CScInfo::InvokeControl(CSC_DATA* pSC_DATA)
{
	if (pSC_DATA == NULL)
		return;

	if (pSC_DATA->m_bModified == FALSE)
		return;

	//DEBUGER_ASSERT_VALID(m_pControl != NULL);
	BOOL bErase = FALSE;

	// 크레인 수평위치(POS_H_RD)로 화면 위치 표현
	int nForkPos = (CConvert::ToInt(pSC_DATA->V_POS_H_RD) < 1) ? 0 : CConvert::ToInt(pSC_DATA->V_POS_H_RD);

	CString strSENSOR_FK_RD = pSC_DATA->V_SENSOR_FK_RD;
	int nProd = 0;
	if (strSENSOR_FK_RD != _T("0") && !strSENSOR_FK_RD.IsEmpty())
		nProd = 1;

	// 층(레이아웃)별 컨트롤 전부 갱신 - 1F/2F/3F 크레인 상태 동기화
	CDciRvCtrl* pCtrlArr[4] = { pSC_DATA->m_pControl, pSC_DATA->m_pControl2, pSC_DATA->m_pControl3, pSC_DATA->m_pControl4 };
	for (int nIdxCtrl = 0; nIdxCtrl < 4; nIdxCtrl++)
	{
		CDciRvCtrl* pCtrl = pCtrlArr[nIdxCtrl];
		if (pCtrl == NULL)
			continue;

		pCtrl->m_clrFork = GetForkColor1(pSC_DATA);
		pCtrl->m_clrFork2 = GetForkColor1(pSC_DATA);
		pCtrl->m_nForkPos = nForkPos;
		bErase = TRUE;
		pCtrl->m_nProd = nProd;
		pCtrl->m_clrRail = GetRailColor(pSC_DATA);
		pCtrl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	pSC_DATA->m_bModified = FALSE;
}

CSC_DATA* CScInfo::CreateSC_DATA(CString pstrEQP_NO)
{
	CSC_DATA* pSC_DATA = new CSC_DATA(m_pEquipment->m_WH_TYP, GetStringPLC_NO(), GetStringEQP_NO(pstrEQP_NO));
	m_pSC_DATA = pSC_DATA;
	return pSC_DATA;
}

CString CScInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CScInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CScInfo::GetStringEQP_NO(CString pstrEQP_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%03s"), pstrEQP_NO);
	return strPLC_NO;
}


