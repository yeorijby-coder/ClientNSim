// ScPair.cpp: implementation of the CScPair class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "ScPair.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CScPair, CSc)
	//±¸Çö X
CScPair::CScPair(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
	: CSc(pDoc,pstrThreadNo, nIndex)
{
	m_pDoc = pDoc;
	m_enKind = CEquipment::enSC;
	m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
}

CScPair::~CScPair()
{
	delete m_pInfo;
}


void CScPair::AutoRunProc() 
{
	CString strSC_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){ return; };
	if(m_pRsw->m_pRecordSet == NULL){ return;}

	int nScCnt = (int)m_pRsw->m_pRecordSet->RecordCount; 
	m_pRsw->MoveFirst(); 
	for(int nIdxFor = 0; nIdxFor < nScCnt; nIdxFor++)
	{
		strSC_NO = m_pRsw->GetItem(_T("MC_NO"));
		CSC_DATA* pSC_DATA = NULL;

		// LDH
//		pSC_DATA = (m_pDoc != NULL)? m_pDoc->GetSC_DATA(strSC_NO) : NULL;

		// JBY
		m_pInfo->m_MapSC_DATA.Lookup(strSC_NO, pSC_DATA);
		pSC_DATA = m_pInfo->m_MapSC_DATA[strSC_NO];

		if(pSC_DATA == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};
		pSC_DATA->SetSC_GRP_NO(m_pRsw->GetItem(_T("SC_GRP_NO")));
		pSC_DATA->SetMC_NO(m_pRsw->GetItem(_T("MC_NO")));
		pSC_DATA->SetMC_NO_NM(m_pRsw->GetItem(_T("MC_NO_NM")));
		//pSC_DATA->SetRESPONSE_CODE_RD(m_pRsw->GetItem( _T("RESPONSE_CODE_RD")));
		pSC_DATA->SetERR_CODE_RD(m_pRsw->GetItem(_T("ERROR_CODE_RD")));
		//pSC_DATA->SetCRANE_STATUS_RD(m_pRsw->GetItem(_T("CRANE_STATUS_RD")));
		//pSC_DATA->SetCRANE_ONLINE_RD(m_pRsw->GetItem(_T("CRANE_ONLINE_RD")));
		//pSC_DATA->SetCRANE_REQUEST_RD(m_pRsw->GetItem(_T("CRANE_REQUEST_RD")));
		//pSC_DATA->SetIS_ERROR_RD(m_pRsw->GetItem(_T("IS_ERROR_RD")));
		//pSC_DATA->SetRECOVERABLE_ERROR_RD(m_pRsw->GetItem(_T("RECOVERABLE_ERROR_RD")));
		//pSC_DATA->SetCRANE_AT_HP_RD(m_pRsw->GetItem(_T("CRANE_AT_HP_RD")));
		//pSC_DATA->SetWARMING_UP_RD(m_pRsw->GetItem(_T("WARMING_UP_RD")));
		//pSC_DATA->SetPROD_CHECK_RD(m_pRsw->GetItem(_T("PROD_CHECK_RD")));
		//pSC_DATA->SetORDER_CHECK_RD(m_pRsw->GetItem(_T("ORDER_CHECK_RD")));
		pSC_DATA->SetJOB_TYP(m_pRsw->GetItem(_T("JOB_TYP")));
		pSC_DATA->SetLUGG_NO(m_pRsw->GetItem(_T("LUGG_NO")));
		pSC_DATA->SetITN_LUGG(m_pRsw->GetItem(_T("ITN_LUGG")));
		pSC_DATA->SetSTART_BANK(m_pRsw->GetItem(_T("START_BANK")));
		pSC_DATA->SetSTART_BAY(m_pRsw->GetItem(_T("START_BAY")));
		pSC_DATA->SetSTART_LEVEL(m_pRsw->GetItem(_T("START_LEVEL")));
		pSC_DATA->SetSTART_HSPOS(m_pRsw->GetItem(_T("START_HSPOS")));
		//pSC_DATA->SetSTART_DEEP_CELL(m_pRsw->GetItem(_T("START_DEEP_CELL")));
		pSC_DATA->SetDEST_BANK(m_pRsw->GetItem(_T("DEST_BANK")));
		pSC_DATA->SetDEST_BAY(m_pRsw->GetItem(_T("DEST_BAY")));
		pSC_DATA->SetDEST_LEVEL(m_pRsw->GetItem(_T("DEST_LEVEL")));
		pSC_DATA->SetDEST_HSPOS(m_pRsw->GetItem(_T("DEST_HSPOS")));
		//pSC_DATA->SetDEST_DEEP_CELL(m_pRsw->GetItem(_T("DEST_DEEP_CELL")));
		pSC_DATA->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pSC_DATA->SetEQP_TIME(m_pRsw->GetItem(_T("EQP_TIME")));
		pSC_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem(_T("CONNECTED_YN")));
		pSC_DATA->SetSC_TYP(m_pRsw->GetItem(_T("SC_TYP")));
		//pSC_DATA->SetOD_RQ_YN(m_pRsw->GetItem(_T("od_rq_yn")));
		//pSC_DATA->SetOD_RQ_FLAG(m_pRsw->GetItem(_T("OD_RQ_FLAG")));
		//pSC_DATA->SetCMD_RQ_ID(m_pRsw->GetItem(_T("CMD_RQ_ID")));
		//pSC_DATA->SetCMD_RQ_YN(m_pRsw->GetItem(_T("CMD_RQ_YN")));
		//pSC_DATA->SetOD_USER_ID(m_pRsw->GetItem(_T("OD_USER_ID")));

		if(GetModified() == TRUE)
		{
			//::PostMessage(m_pDoc->m_hWndView, WM_USER_REFRESH_DIALOG, 0, m_enKind);					
		}
		if (m_pRsw->GetItem(_T("DEST_BAY")) != "000" && m_pRsw->GetItem(_T("DEST_BAY")) != "0")
		{
			pSC_DATA->V_Prev_DEST_BAY = m_pRsw->GetItem(_T("DEST_BAY"));
		}
		m_pInfo->InvokeControl(pSC_DATA);
		m_pRsw->MoveNext();	
	}
}

void CScPair::CommandProc()
{
	
}

