// Bcr.cpp: implementation of the CBcr class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "Bcr.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#define DEVICE m_port.m_strDevice

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBcr
//
IMPLEMENT_DYNAMIC(CBcr, CEquipment)

CBcr::CBcr(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
	: CEquipment(pDoc, pstrThreadNo,  nIndex)
{
	m_pInfo = new CBcrInfo(this);
	m_enKind = CEquipment::enBCR;
}

CBcr::~CBcr()
{
	if (m_pInfo) {delete m_pInfo;};
}

void CBcr::AutoRunProc()
{
	CString strBCR_NO, strBCR_MC_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){	return;	}
	if(m_pRsw->m_pRecordSet == NULL){ return;}

	int nBcrCnt = m_pRsw->m_pRecordSet->RecordCount;
	if(nBcrCnt < 1)
	{
		return;
	}
	CBcrInfo* pBcrInfo = m_pInfo;
	if(pBcrInfo == NULL) {	return; };

	m_pRsw->MoveFirst();
	for(int nIdxFor = 0; nIdxFor < nBcrCnt; nIdxFor++)
	{
		strBCR_NO = m_pRsw->GetItem(_T("BCR_NO"));
		CBCR_MST* pBCR_MST = m_pInfo->m_MapBCR_MST[strBCR_NO.Mid(1,2)];
		if(pBCR_MST == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};

		CTrackInfo* pTrackInfo = (CTrackInfo*)(m_pDoc->GetTrackInfoNew(pBCR_MST->m_strBCR_MC_NO));
		if(pTrackInfo == NULL)
		{
			m_pRsw->MoveNext();
			continue;
		}

		CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;
		if(pCV_DATA == NULL)
		{
			m_pRsw->MoveNext();
			continue; 
		}
		pBCR_MST->SetBCR_NO(m_pRsw->GetItem(_T("BCR_NO")));
		pBCR_MST->SetBCR_MC_NO(m_pRsw->GetItem(_T("BCR_MC_NO")));
		pBCR_MST->SetBCR_STA(m_pRsw->GetItem(_T("BCR_STA")));
		pBCR_MST->SetBCR_RCV_VAL(m_pRsw->GetItem(_T("BCR_RCV_VAL")));
		pBCR_MST->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pBCR_MST->SetCHK_BYPASS_YN(m_pRsw->GetItem(_T("CHK_BYPASS_YN")));
		pBCR_MST->SetEQP_TIME(m_pRsw->GetItem(_T("EQP_TIME")));
		pBCR_MST->SetEQP_CONNECTED_YN(m_pRsw->GetItem(_T("CONNECTED_YN")));
		pBCR_MST->SetUPD_DT(m_pRsw->GetItem(_T("UPD_DT")));

		if (pBCR_MST->m_bListAdd)
		{
			for (int i = 6; i > 0; i--)
			{
				pBCR_MST->V_BCR_RCV_VAL[i] = pBCR_MST->V_BCR_RCV_VAL[i - 1];
				pBCR_MST->V_UPD_DT[i] = pBCR_MST->V_UPD_DT[i - 1];
			}
			pBCR_MST->m_bListAdd = false;
		}

		pBcrInfo->InvokeControl(pBCR_MST, pCV_DATA);
		m_pRsw->MoveNext();
	}
}

void CBcr::CommandProc()
{
}

CString CBcr::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");

	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(BD.BCR_NO,'000') AS BCR_NO, \n")
				  _T("		  ") + m_pDoc->NVL + _T("(BD.BCR_MC_NO,'000') AS BCR_MC_NO,  \n")
				  _T("        ") + m_pDoc->NVL + _T("(BD.BCR_STA, '0') AS BCR_STA, \n")
				  _T("        ") + m_pDoc->NVL + _T("(BD.BCR_RCV_VAL, '0') AS BCR_RCV_VAL, \n")
				  _T("		  ") + m_pDoc->NVL + _T("(BD.SUSPEND,'0') AS SUSPEND,")
				  _T("		  ") + m_pDoc->NVL + _T("(BD.CHK_BYPASS_YN,'0') AS CHK_BYPASS_YN,")
				  _T("		  ") + m_pDoc->NVL + _T("(BD.USE_YN, '0') AS USE_YN, \n")
				  _T("        ") + m_pDoc->NVL + _T("(BD.UPD_USER_ID ,'0') AS UPD_USER_ID, \n")
				  _T("        ") + m_pDoc->NVL + _T("(BD.UPD_DT,") + m_pDoc->SYSDATE + _T(") AS UPD_DT, ")
				  _T("	     CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME,		\n")
				  _T("	     EM.CONNECTED_YN															  								\n")
			      _T("  FROM BCR_DATA BD																								\n")
				  _T(" INNER JOIN (SELECT PLC_NO, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%02s' AND EQP_TYP = 'BCR') EM		\n")
				  _T("         ON BD.PLC_NO = EM.PLC_NO																					\n")
			      _T(" WHERE BD.WH_TYP = '%02s' \n"), m_WH_TYP, m_WH_TYP);

	return strSql;
}

void CBcr::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}