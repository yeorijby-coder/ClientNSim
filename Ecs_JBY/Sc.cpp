
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


CString CSc::GetSelectQry()
{
	CString strSql = _T("");

	// 이 현장 크레인 상태는 SC_DATA 에 있다. 컬럼 이름이 다르므로 별칭으로 맞춘다.
	// (옛 SC_DATA_MURATA 는 이 현장에서 쓰지 않는다)
	strSql.Format(_T("SELECT SD.WH_TYP,\n")
			  _T("       SD.PLC_NO,\n")
			  _T("       SD.SC_NO,\n")
			  _T("       SD.SC_GRP_NO,\n")
			  _T("       SD.MC_NO,\n")
			  _T("       SD.MC_NO_NM,\n")
			  _T("       SD.SC_TYP,\n")
			  _T("    '00' AS RESPONSE_CODE_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.ERR_CODE_RD,'0') AS ERROR_CODE_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.UCSTATUS_RD,'0') AS CRANE_STATUS_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.ONLINE_MODE_RD,'0') AS CRANE_ONLINE_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.AUTO_MODE_RD,'0') AS CRANE_REQUEST_RD,\n")
			  _T("    CASE WHEN ") + m_pDoc->NVL + _T("(SD.ERR_CODE_RD,'0') IN ('','0','00','000','0000')\n")
			  _T("              THEN '0' ELSE '1' END AS IS_ERROR_RD,\n")
			  _T("    '0' AS RECOVERABLE_ERROR_RD,\n")
			  _T("    '0' AS CRANE_AT_HP_RD,\n")
			  _T("    '0' AS WARMING_UP_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.SENSOR_FK_RD,'0') AS PROD_CHECK_RD,\n")
			  _T("    '0' AS ORDER_CHECK_RD,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.JOB_TYP_RD,'0') AS JOB_TYP,\n")
			  _T("    CASE WHEN ") + m_pDoc->NVL + _T("(SD.LUGG_NO_FK1_RD,'0') IN ('','0','0000')\n")
			  _T("              THEN '0' ELSE SD.LUGG_NO_FK1_RD END AS LUGG_NO,\n")
			  _T("    CASE WHEN ") + m_pDoc->NVL + _T("(SD.ITN_LUGG_FK1,'0') IN ('','0','0000')\n")
			  _T("              THEN '0' ELSE SD.ITN_LUGG_FK1 END AS ITN_LUGG,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.START_BANK_FK1_RD,'0') AS START_BANK,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.START_BAY_FK1_RD,'0') AS START_BAY,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.START_LEVEL_FK1_RD,'0') AS START_LEVEL,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.START_HSPOS_FK1_RD,'0') AS START_HSPOS,\n")
			  _T("    '0' AS START_DEEP_CELL,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.DEST_BANK_FK1_RD,'0') AS DEST_BANK,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.DEST_BAY_FK1_RD,'0') AS DEST_BAY,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.DEST_LEVEL_FK1_RD,'0') AS DEST_LEVEL,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.DEST_HSPOS_FK1_RD,'0') AS DEST_HSPOS,\n")
			  _T("    '0' AS DEST_DEEP_CELL,\n")
			  _T("    ") + m_pDoc->NVL + _T("(SD.SUSPEND,'0') AS SUSPEND,\n")
			  _T("    EM.PLC_IP, EM.PLC_PORT_FROM, EM.CONNECTED_YN,\n")
			  _T(" CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME,\n")
			  _T(" EM.CONNECTED_YN\n")
			  _T("   FROM SC_DATA SD INNER JOIN (SELECT PLC_NO, PLC_IP, PLC_PORT_FROM, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = 'SC') EM\n")
			  _T("                            ON SD.PLC_NO = EM.PLC_NO\n")
			  _T("  WHERE SD.WH_TYP = '%s'\n")
			  _T("  ORDER BY SD.SC_NO"),  m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);


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