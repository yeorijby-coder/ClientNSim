
// Sc.cpp: implementation of the CSc class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Sc.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "ScSkinDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice


// CSc
//
IMPLEMENT_DYNAMIC(CSc, CEquipment)

CSc::CSc(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc,pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enSC;
	m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);

	m_pDoc = pDoc;
	m_pRsw = NULL;

//	m_pSC_DATA = NULL;
}

CSc::~CSc()
{
	delete m_pInfo;
}


void CSc::AutoRunProc()
{

	
}

void CSc::CommandProc()
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CString CSc::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");
	
	strSql.Format(_T("SELECT					  SD.WH_TYP,																													\n")
				  _T("							  SD.PLC_NO,																													\n")
				  _T("							  SD.SC_NO,																														\n")
		          _T("							  SD.SC_GRP_NO,																													\n")
		          _T("							  SD.MC_NO,																														\n")
		          _T("							  SD.MC_NO_NM,																													\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.RESPONSE_CODE_RD,'0') AS RESPONSE_CODE_RD,																	     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.ERROR_CODE_RD,'0') AS ERROR_CODE_RD,																		     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.CRANE_STATUS_RD,'0') AS CRANE_STATUS_RD,																	     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.CRANE_ONLINE_RD,'0') AS CRANE_ONLINE_RD,																	     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.CRANE_REQUEST_RD,'0') AS CRANE_REQUEST_RD,																	     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.IS_ERROR_RD,'0') AS IS_ERROR_RD,																			     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.RECOVERABLE_ERROR_RD,'0') AS RECOVERABLE_ERROR_RD,															   				\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.CRANE_AT_HP_RD,'0') AS CRANE_AT_HP_RD,																		     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.WARMING_UP_RD,'0') AS WARMING_UP_RD,																		     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.PROD_CHECK_RD,'0') AS PROD_CHECK_RD,																		     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.ORDER_CHECK_RD,'0') AS ORDER_CHECK_RD,																		     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.JOB_TYP,'0') AS JOB_TYP,																									\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.LUGG_NO,'0') AS LUGG_NO,																									\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.ITN_LUGG,'0') AS ITN_LUGG,																									\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.START_BANK,'0') AS START_BANK,																				     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.START_BAY,'0') AS START_BAY,																				     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.START_LEVEL,'0') AS START_LEVEL,																			     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.START_HSPOS,'0') AS START_HSPOS,																			     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.START_DEEP_CELL,'0') AS START_DEEP_CELL,																	     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.DEST_BANK,'0') AS DEST_BANK,																				     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.DEST_BAY,'0') AS DEST_BAY,																					     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.DEST_LEVEL,'0') AS DEST_LEVEL,																				     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.DEST_HSPOS,'0') AS DEST_HSPOS,																				     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.DEST_DEEP_CELL,'0') AS DEST_DEEP_CELL,																			     			\n")
		          _T("    ") + m_pDoc->NVL + _T("(SD.SUSPEND,'0') AS SUSPEND,																			     			\n")
				  _T("    EM.PLC_IP, EM.PLC_PORT_FROM, EM.CONNECTED_YN,																							     			\n")
				  _T(" CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME,		  								     			\n")
				  _T(" EM.CONNECTED_YN															  																     			\n")
				  _T("	 FROM SC_DATA_MURATA SD INNER JOIN (SELECT PLC_NO, PLC_IP, PLC_PORT_FROM, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = 'SC') EM	\n")
				  _T("                             ON SD.PLC_NO = EM.PLC_NO \n")
				  _T("    WHERE SD.WH_TYP = '%s'                            \n")
				  _T("ORDER BY SD.SC_NO		"),  m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);


	return strSql;
}

void CSc::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CSc::GetModified()
{
	if(m_pDoc->m_pScSkinDlg == NULL)
		return FALSE;

	CSC_DATA* pSC_DATA = ((CScSkinDlg*)m_pDoc->m_pScSkinDlg)->m_pSC_DATA;
	if(pSC_DATA == NULL)
		return FALSE;

	return pSC_DATA->m_bModified;
}