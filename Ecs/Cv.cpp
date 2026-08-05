// Cv.cpp: implementation of the CCv class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Cv.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "CvSkinDlg.h"

#define DEVICE m_port.m_strDevice
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CCv
//
IMPLEMENT_DYNAMIC(CCv, CEquipment)

CCv::CCv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc, pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enCV;
	m_pInfo = new CCvInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_pDoc = pDoc;
	m_pRsw = NULL;
}
CCv::~CCv()
{
	m_pInfo->m_MapTrackInfo.RemoveAll();
	delete m_pInfo;
}


void CCv::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CCv::GetModified()
{
	if(m_pDoc->m_pCvSkinDlg == NULL)
		return FALSE;

	CTrackInfo* pTrackInfo = ((CCvSkinDlg*)m_pDoc->m_pCvSkinDlg)->m_pTrackInfo;
	if(pTrackInfo == NULL)
		return FALSE;
	
	BOOL bModified = pTrackInfo->m_pCV_DATA->m_bModified;
	//pTrackInfo->m_pCV_DATA->m_bModified = FALSE; 
	return bModified;
}

CString CCv::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");

	strSql.Format(_T("SELECT CD.WH_TYP																										\n")
				  _T("       ,CD.TRACK_NO																									\n")
				  _T("       ,CD.MC_NO																										\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.MC_NO_NM, ' ') AS MC_NO_NM															\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.LUGG_NO_RD, '0') AS LUGG_NO_RD														\n")
		          _T("       ,") + m_pDoc->NVL + _T("(CD.DEST_POS_RD, '0') AS DEST_POS_RD													\n")
		          _T("       ,") + m_pDoc->NVL + _T("(CD.JOB_TYP_RD, '0') AS	JOB_TYP_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.TR_PAUSE_RD, '0') AS TR_PAUSE_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_RD, '0') AS COMMING_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ERROR_CODE, '0') AS ERROR_CODE														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.AUTO_MODE_RD, '0') AS AUTO_MODE_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.STO_READY_RD, '0') AS STO_READY_RD														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.RET_READY_RD, '0') AS RET_READY_RD														\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.STOHS_READY_RD, '0') AS STOHS_READY_RD												\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.RETHS_READY_RD, '0') AS RETHS_READY_RD												\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.RTV_DEPARTHS_READY_RD, '0') AS RTV_DEPARTHS_READY_RD								\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.RTV_ARRIVEHS_READY_RD, '0') AS RTV_ARRIVEHS_READY_RD								\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR0_DATA_RD, '0') AS SENSOR0_DATA_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR1_DATA_RD, '0') AS SENSOR1_DATA_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR2_DATA_RD, '0') AS SENSOR2_DATA_RD											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.DELETE_TRACK_RD, '0') AS DELETE_TRACK_RD											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.RTV_LOCK_SENSOR, '0') AS RTV_LOCK_SENSOR											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.BARCODE, '0') AS BARCODE															\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.ERR_RQ_RD, '0') AS ERR_RQ_RD														\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.OD_RQ_YN, '0') AS OD_RQ_YN															\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.OD_RQ_FLAG, '0') AS OD_RQ_FLAG														\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.CMD_RQ_ID, '0') AS CMD_RQ_ID														\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.CMD_RQ_YN, '0') AS CMD_RQ_YN														\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.HOST_SEND_YN, '0') AS HOST_SEND_YN													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.HOST_ERR_SEND_YN, '0') AS HOST_ERR_SEND_YN											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.USE_YN, '0') AS USE_YN																\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_CHECK_TR1, '0') AS COMMING_CHECK_TR1										\n")
			      _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_CHECK_TR2, '0') AS COMMING_CHECK_TR2										\n")
			      _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_CHECK_TR3, '0') AS COMMING_CHECK_TR3										\n")
			      _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_CHECK_TR4, '0') AS COMMING_CHECK_TR4										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.COMMING_DEST_TR, '0') AS COMMING_DEST_TR											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.STN_KIND, '0') AS STN_KIND															\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DOOR_STATUS_RD, '0') AS DOOR_STATUS_RD												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DEVERTER_HS_DOWN_RD, '0') AS DEVERTER_HS_DOWN_RD									\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.DEVERTER_HS_UP_RD, '0') AS DEVERTER_HS_UP_RD										\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.RGV_STA_LOAD_RD, '0') AS RGV_STA_LOAD_RD											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.SC_PA_PK_POSSIBLE_RD, '0') AS SC_PA_PK_POSSIBLE_RD									\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.DOOR_OPEN_REQ_RD, '0') AS DOOR_OPEN_REQ_RD											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.DOOR_CLOSE_REQ_RD, '0') AS DOOR_CLOSE_REQ_RD										\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.MTR1_RUN_RD, '0') AS MTR1_RUN_RD													\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.MTR2_RUN_RD, '0') AS MTR2_RUN_RD													\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.SZ_CHK_LOW_RD, '0') AS SZ_CHK_LOW_RD												\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.SZ_CHK_HIGH_RD, '0') AS SZ_CHK_HIGH_RD												\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.SRT_READY_STA_RD, '0') AS SRT_READY_STA_RD											\n")
		          _T("	     ,") + m_pDoc->NVL + _T("(CD.PICK4_PASS_RD, '0') AS PICK4_PASS_RD												\n")

				  _T("	     ,CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME			\n")
				  _T("	     ,EM.CONNECTED_YN															  									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM, '0') AS BCR_BOTTOM				  										\n")
				  _T(" FROM CV_DATA CD 																										\n")
				  _T(" INNER JOIN (SELECT PLC_NO, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%02s' AND EQP_TYP = 'CV') EM			\n")
				  _T("         ON CD.PLC_NO = EM.PLC_NO																						\n")
				  _T(" LEFT JOIN JOB_MST JM				                            														\n")
				  _T("         ON CD.LUGG_NO_RD = JM.LUGG_NO																				\n")
				  _T("WHERE CD.WH_TYP = '%02s'																								\n")
				  _T("  AND CD.PLC_NO IN ('%s')																								\n")
				  _T("ORDER BY TRACK_NO																										\n"),m_WH_TYP, m_WH_TYP, m_strThreadNo);
	
	return strSql;
}
void CCv::CommandProc()
{

}
void CCv::AutoRunProc()
{
	if(m_pInfo == NULL)
	{
		return;
	}

	if(m_pRsw == NULL)
	{			
		return;		
	}

	if(m_pRsw->m_pRecordSet == NULL){ return;}

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

	for(int nIdxFor = 0; nIdxFor < nTrackCnt; nIdxFor++)
	{
		CString strTRACK_NO = m_pRsw->GetItem( _T("MC_NO"));
		CTrackInfo* pTrackInfo = m_pInfo->m_MapTrackInfo[strTRACK_NO];
		if(pTrackInfo == NULL)
		{	m_pRsw->MoveNext();	continue;	}
		CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;
		if(pCV_DATA == NULL){	m_pRsw->MoveNext(); continue;  }

		pCV_DATA->SetMC_NO(m_pRsw->GetItem(_T("MC_NO")));
		pCV_DATA->SetMC_NO_NM(m_pRsw->GetItem(_T("MC_NO_NM")));
		pCV_DATA->SetLUGG_NO_RD(m_pRsw->GetItem( _T("LUGG_NO_RD")));
		pCV_DATA->SetDEST_POS_RD(m_pRsw->GetItem( _T("DEST_POS_RD")));
		pCV_DATA->SetJOB_TYP_RD(m_pRsw->GetItem( _T("JOB_TYP_RD")));
		pCV_DATA->SetTR_PAUSE_RD(m_pRsw->GetItem(_T("TR_PAUSE_RD")));
		pCV_DATA->SetCOMMING_RD(m_pRsw->GetItem(_T("COMMING_RD")));
		pCV_DATA->SetERROR_CODE(m_pRsw->GetItem(_T("ERROR_CODE")));
		pCV_DATA->SetAUTO_MODE_RD(m_pRsw->GetItem(_T("AUTO_MODE_RD")));
		pCV_DATA->SetSTO_READY_RD(m_pRsw->GetItem(_T("STO_READY_RD")));
		pCV_DATA->SetRET_READY_RD(m_pRsw->GetItem(_T("RET_READY_RD")));
		pCV_DATA->SetSTOHS_READY_RD(m_pRsw->GetItem(_T("STOHS_READY_RD")));
		pCV_DATA->SetRETHS_READY_RD(m_pRsw->GetItem(_T("RETHS_READY_RD")));
		pCV_DATA->SetRTV_DEPARTHS_READY_RD(m_pRsw->GetItem(_T("RTV_DEPARTHS_READY_RD")));
		pCV_DATA->SetRTV_ARRIVEHS_READY_RD(m_pRsw->GetItem(_T("RTV_ARRIVEHS_READY_RD")));
		pCV_DATA->SetSENSOR0_DATA_RD(m_pRsw->GetItem(_T("SENSOR0_DATA_RD")));
		pCV_DATA->SetSENSOR1_DATA_RD(m_pRsw->GetItem(_T("SENSOR1_DATA_RD")));
		pCV_DATA->SetSENSOR2_DATA_RD(m_pRsw->GetItem(_T("SENSOR2_DATA_RD")));
		pCV_DATA->SetDELETE_TRACK_RD(m_pRsw->GetItem(_T("DELETE_TRACK_RD")));
		pCV_DATA->SetRTV_LOCK_SENSOR(m_pRsw->GetItem(_T("RTV_LOCK_SENSOR")));
		pCV_DATA->SetBARCODE(m_pRsw->GetItem(_T("BARCODE")));
		pCV_DATA->SetJMBARCODE(m_pRsw->GetItem(_T("BCR_BOTTOM")));
		pCV_DATA->SetERR_RQ_RD(m_pRsw->GetItem(_T("ERR_RQ_RD")));
		pCV_DATA->SetOD_RQ_YN(m_pRsw->GetItem(_T("OD_RQ_YN")));
		pCV_DATA->SetOD_RQ_FLAG(m_pRsw->GetItem(_T("OD_RQ_FLAG")));
		pCV_DATA->SetCMD_RQ_ID(m_pRsw->GetItem(_T("CMD_RQ_ID")));
		pCV_DATA->SetCMD_RQ_YN(m_pRsw->GetItem(_T("CMD_RQ_YN")));
		pCV_DATA->SetHOST_SEND_YN(m_pRsw->GetItem(_T("HOST_SEND_YN")));
		pCV_DATA->SetHOST_ERR_SEND_YN(m_pRsw->GetItem(_T("HOST_ERR_SEND_YN")));
		pCV_DATA->SetCOMMING_CHECK_TR1(m_pRsw->GetItem(_T("COMMING_CHECK_TR1")));
		pCV_DATA->SetCOMMING_CHECK_TR2(m_pRsw->GetItem(_T("COMMING_CHECK_TR2")));
		pCV_DATA->SetCOMMING_CHECK_TR3(m_pRsw->GetItem(_T("COMMING_CHECK_TR3")));
		pCV_DATA->SetCOMMING_CHECK_TR4(m_pRsw->GetItem(_T("COMMING_CHECK_TR4")));
		pCV_DATA->SetCOMMING_DEST_TR(m_pRsw->GetItem(_T("COMMING_DEST_TR")));
		pCV_DATA->SetSTN_KIND(m_pRsw->GetItem(_T("STN_KIND")));
		pCV_DATA->SetEQP_TIME(m_pRsw->GetItem(_T("EQP_TIME")));
		pCV_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem(_T("CONNECTED_YN")));

		pTrackInfo->m_wDoorStatus = 0x0000;

		if (pTrackInfo->IsDoorTrack())
		{
			CString strDOOR_STATUS_RD = m_pRsw->GetItem(_T("DOOR_STATUS_RD"));
			pCV_DATA->SetDOOR_STATUS_RD(strDOOR_STATUS_RD, pTrackInfo->m_wDoorStatus);
		}

		// 강제 Control Update
		if (bManualModify == TRUE)
			pCV_DATA->m_bModified = TRUE;

		pTrackInfo->InvokeControl(pTrackInfo->m_pTrackCtrl);
		m_pRsw->MoveNext();

		m_pDoc->m_nLayoutLg[CConvert::ToInt(strTRACK_NO)] = pCV_DATA->V_LUGG_NO_RD; //Layout 작업번호
		m_pDoc->m_nLayoutPl[CConvert::ToInt(strTRACK_NO)] = pCV_DATA->V_JMBARCODE; //Layout 바코드
		m_pDoc->m_nLayoutCm[CConvert::ToInt(strTRACK_NO)] = pCV_DATA->V_COMMING_RD; //Layout 입고가능/입고불가 표현


//		m_pInfo->InvokeControl();
	}

	if (bManualModify == TRUE)
		bManualModify = FALSE;
}


