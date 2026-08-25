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
	if (pDisPlayData == NULL)
		return;

	if (pDisPlayData->m_bModified == FALSE)
		return;

	// 레이아웃(EcsLayout*.xml)에 해당 전광판 컨트롤(CID 171802nn)이 없으면 NULL이다
	if (pDisPlayData->m_pControl == NULL)
	{
		pDisPlayData->m_bModified = FALSE;
		return;
	}

	pDisPlayData->m_pControl->m_strText = pDisPlayData->V_DISP_DATA;
	pDisPlayData->m_pControl->InvalidateControl(m_pEquipment->m_pDoc->m_hWndView, FALSE);
	pDisPlayData->m_bModified = FALSE;	// TRUE면 매 주기마다 다시 갱신되어 깜빡임
	m_bModified = FALSE;
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
	strEQP_NO.Format(_T("%01s"), pstrEQP_NO);
	return strEQP_NO;
}