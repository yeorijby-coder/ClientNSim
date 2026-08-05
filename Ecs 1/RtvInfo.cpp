// RtvInfo.cpp: implementation of the CRtvInfo class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "RtvInfo.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CRtvInfo
//
IMPLEMENT_DYNAMIC(CRtvInfo, CRtv)

CRtvInfo::CRtvInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pRTV_DATA = NULL;
	m_bModified = TRUE;
}

CRtvInfo::~CRtvInfo()
{
}

void CRtvInfo::Serialize(CArchive& ar)
{
}

COLORREF CRtvInfo::GetForkColor1(CRTV_DATA* pRTV_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pRTV_DATA->V_IS_ERROR_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1"))
		return DARK_GRAY;

	int nJobTypTmp = CConvert::ToInt(pRTV_DATA->V_JOB_TYP_OD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_MOVE;//return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}

	return LIGHT_GRAY;
}

COLORREF CRtvInfo::GetForkColor2(CRTV_DATA* pRTV_DATA)
{
	CConfig* pConfig = m_pEquipment->m_pDoc->m_pConfig;
	DEBUGER_ASSERT_VALID(pConfig != NULL);

	if (pRTV_DATA->V_ERR_CODE_RD != _T("0"))
		return pConfig->m_clrUSER_COLOR_ERROR;

	if (pRTV_DATA->V_AUTO_MODE_RD != _T("1") ||
		pRTV_DATA->V_WAITING_ORDER_RD != _T("1"))

		return DARK_GRAY;

	int nJobTypTmp = CConvert::ToInt(pRTV_DATA->V_JOB_TYP_OD);
	switch (nJobTypTmp)
	{
	case enJobTypeAutoSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeAutoRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeAutoR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeAutoMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeAutoPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiSto			: return pConfig->m_clrUSER_COLOR_STO;
	case enJobTypeSemiRet			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeSemiR2R			: return pConfig->m_clrUSER_COLOR_RTR;
	case enJobTypeSemiMove			: return pConfig->m_clrUSER_COLOR_MOVE;
	case enJobTypeSemiPR			: return pConfig->m_clrUSER_COLOR_RET;
	case enJobTypeManual			: return pConfig->m_clrUSER_COLOR_MANUAL;
	}
	return LIGHT_GRAY;
}

COLORREF CRtvInfo::GetRailColor(CRTV_DATA* pRTV_DATA)
{
	if(pRTV_DATA == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
	}

	if (pRTV_DATA->V_SUSPEND == _T("3") || pRTV_DATA->V_SUSPEND == _T("2") || pRTV_DATA->V_SUSPEND == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND;

	if (pRTV_DATA->V_IS_ERROR_RD != _T("0"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR;

	if (pRTV_DATA->V_JOB_TYP_OD == _T("1"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_STO;

	if (pRTV_DATA->V_JOB_TYP_OD == _T("2"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RET;

	if (pRTV_DATA->V_JOB_TYP_OD == _T("3"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE;

	if (pRTV_DATA->V_JOB_TYP_OD == _T("4") || pRTV_DATA->V_JOB_TYP_OD == _T("5") || pRTV_DATA->V_JOB_TYP_OD == _T("6"))
		return m_pEquipment->m_pDoc->m_pConfig->m_clrUSER_COLOR_RTR;

	return BLACK;
}

COLORREF CRtvInfo::GetPostColor()
{
	//return (m_ucProdLoad != enProdEmpty) ? BLUE : DARK_GRAY;
	return DARK_GRAY;
}

int CRtvInfo::GetRtvPosition()
{
// 	int nCount = m_RtvPosInfo.GetSize();
// 	for (int i=0;i<nCount;i++)
// 	{
// 		if (m_RtvPosInfo[i].m_nDurgPos == m_ucHorizontalPos)
// 			return m_RtvPosInfo[i].m_nEcsPos;
// 	}

	return 0;
}

void CRtvInfo::InvokeControl(CRTV_DATA* pRTV_DATA)
{
	if(pRTV_DATA == NULL)
	{  
		return;
	};

	if (pRTV_DATA->m_bModified == FALSE)
		return; 

	//DEBUGER_ASSERT_VALID(m_pControl != NULL);

	BOOL bErase = FALSE;	// (m_pControl->m_nForkPos != m_wHorizontalPos);
	int nPos =  CConvert::ToInt(pRTV_DATA->V_POS_H_RD);
	if (pRTV_DATA->m_pControl)				
	{
		bErase = TRUE;
		pRTV_DATA->m_pControl->m_nForkPos = (nPos < 0 || nPos > 15) ? 0 : nPos;
	}

	CString strSENSOR_FK_RD = m_pRTV_DATA->V_SENSOR_RTV_RD;//0;
	int nProd = 0;
	if (strSENSOR_FK_RD == _T("1"))
		nProd  = 1;

	if (pRTV_DATA->m_pControl)
	{
		pRTV_DATA->m_pControl->m_nProd = nProd;
		pRTV_DATA->m_pControl->m_clrRail = GetRailColor(pRTV_DATA);
		//	m_pControl->m_clrWing = GetPostColor();
		pRTV_DATA->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, bErase);
	}

	if (pRTV_DATA->m_pControl)
		pRTV_DATA->m_pControl->m_clrFork = GetForkColor1(pRTV_DATA);

	pRTV_DATA->m_bModified = FALSE;
}

CRTV_DATA* CRtvInfo::CreateRTV_DATA( CString pstrEQP_NO )
{
	CRTV_DATA* pRTV_DATA = new CRTV_DATA(m_pEquipment->m_WH_TYP, GetStringPLC_NO(), GetStringEQP_NO(pstrEQP_NO));
	m_pRTV_DATA = pRTV_DATA;
	return pRTV_DATA;
	
}

CString CRtvInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CRtvInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CRtvInfo::GetStringEQP_NO(CString pstrEQP_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%03s"), pstrEQP_NO);
	return strPLC_NO;
}