#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "Display.h"


IMPLEMENT_DYNAMIC(CDisplay, CEquipment)

CDisplay::CDisplay(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc, pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enDISPLAY;
	m_pInfo = new CDisplayInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_pDoc = pDoc;
	m_pRsw = NULL;
}
CDisplay::~CDisplay()
{
	delete m_pInfo;
}

void CDisplay::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if (pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

CString CDisplay::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");

	//CString strPlcNo = m_strInPlc;
	//if (m_strInPlc != m_strThreadNo && m_strInPlc == _T(""))
	//	strPlcNo = m_strThreadNo;


	strSql.Format(_T("SELECT DD.*					\n")
		           _T(" FROM DISPLAY_DATA DD 		\n")
		           _T("WHERE DD.WH_TYP = '%s'		\n")
		           _T("  AND DD.PLC_NO IN ('%s')	\n")
		           _T("ORDER BY DD.DISP_NO			\n"), m_WH_TYP, m_strPlcNo);

	return strSql;
}

void CDisplay::CommandProc()
{

}

void CDisplay::AutoRunProc()
{
	if (m_pInfo == NULL)
	{
		return;
	}

	if (m_pRsw == NULL)
	{
		return;
	}

	if (m_pRsw->m_pRecordSet == NULL) { return; }

	BOOL bManualModify = FALSE;
	if (m_pInfo->m_bManualModified == TRUE)
	{
		m_pInfo->m_bManualModified = FALSE;
		bManualModify = TRUE;
	}

	BOOL bSeach = false;
	int nTrackCnt = (m_pRsw != NULL && m_pRsw->m_pRecordSet != NULL) ? m_pRsw->m_pRecordSet->RecordCount : 0;
	if (m_pRsw != NULL)
		m_pRsw->MoveFirst();

	for (int nIdxFor = 0; nIdxFor < nTrackCnt; nIdxFor++)
	{
		CString strDISP_NO = m_pRsw->GetItem(_T("DISP_NO"));


		CDisplayData* pDisplayData = NULL;

		pDisplayData = (m_pDoc != NULL) ? m_pDoc->GetDisplayData(strDISP_NO) : NULL;
		if (pDisplayData == NULL)
		{
			m_pRsw->MoveNext();
			continue;
		};

		pDisplayData->SetDISP_DATA(m_pRsw->GetItem(_T("DISP_DATA")));




		// °­Á¦ Control Update
		if (bManualModify == TRUE)
			pDisplayData->m_bModified = TRUE;

		m_pInfo->InvokeControl(pDisplayData);
		m_pRsw->MoveNext();

	}

	if (bManualModify == TRUE)
		bManualModify = FALSE;
}
