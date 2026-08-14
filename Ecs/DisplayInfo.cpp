#include "stdafx.h"
#include "DisplayInfo.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBcrInfo
//
IMPLEMENT_SERIAL(CDisplayInfo, CInfo, 0)

CDisplayInfo::CDisplayInfo(CEquipment* pEquipment) : CInfo(pEquipment)
{
	m_pControl = NULL;
	m_pDisplayData = NULL;
	m_bModified = TRUE;
}

CDisplayInfo::~CDisplayInfo()
{
	if (m_pDisplayData) { delete m_pDisplayData; };
}






void CDisplayInfo::InvokeControl()
{
	if (m_bModified == FALSE)
		return;

	DEBUGER_ASSERT_VALID(m_pControl != NULL);

	m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	m_bModified = FALSE;
}

CDisplayData* CDisplayInfo::CreateDisPlayData(CString pstrEQP_NO)
{
	CDisplayData* pDisplayData = new CDisplayData(m_pEquipment->m_WH_TYP, GetStringPLC_NO(), GetStringDISP_NO(pstrEQP_NO));
	m_pDisplayData = pDisplayData;
	return pDisplayData;
}

void CDisplayInfo::InvokeControl(CDisplayData* pDisPlayData)
{
	if (pDisPlayData->m_bModified == FALSE)
		return;

	DEBUGER_ASSERT_VALID(pDisPlayData != NULL);
	//if (pBCR_MST->K_BCR_NO == _T("22"))
	//{
	//	int a = 0;
	//}
	//pBCR_MST->m_pControl->m_clrBgColor = GetColor(pBCR_MST, pCV_DATA);
	//pBCR_MST->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	//pBCR_MST->m_bModified = TRUE;
	//m_bModified = FALSE;
}


CString CDisplayInfo::GetStringPLC_NO()
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), m_pEquipment->m_nNumber);
	return strPLC_NO;
}

CString CDisplayInfo::GetStringPLC_NO(int pnPLC_NO)
{
	CString strPLC_NO;
	strPLC_NO.Format(_T("%02d"), pnPLC_NO);
	return strPLC_NO;
}

CString CDisplayInfo::GetStringDISP_NO(CString pstrEQP_NO)
{
	CString strEQP_NO;
	strEQP_NO.Format(_T("%05s"), pstrEQP_NO);
	return strEQP_NO;
}