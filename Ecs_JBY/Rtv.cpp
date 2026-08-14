// Rtv.cpp: implementation of the CRtv class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Rtv.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "RtvSkinDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice


// CRtv
//
IMPLEMENT_DYNAMIC(CRtv, CEquipment)

CRtv::CRtv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc, pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enRTV;
	m_pInfo = new CRtvInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_pDoc = pDoc;
	m_pRsw = NULL;
}

CRtv::~CRtv()
{
	delete m_pInfo;
}


void CRtv::AutoRunProc()
{
	CString strRTV_NO;
	if(m_pInfo == NULL){ return; };
	if(m_pRsw == NULL){	return;	}
	if(m_pRsw->m_pRecordSet == NULL){ return;}
	
	int nRtvCnt = m_pRsw->m_pRecordSet->RecordCount;
	if(nRtvCnt < 1)
	{
		return;
	}
	m_pRsw->MoveFirst();
	
	CRtvInfo * pRtvInfo = m_pInfo;
	if(pRtvInfo == NULL){ return; }

	m_pRsw->MoveFirst(); 
	for(int nIdxFor = 0; nIdxFor < nRtvCnt; nIdxFor++)
	{
		strRTV_NO = m_pRsw->GetItem(_T("RTV_NO"));
		CRTV_DATA* pRTV_DATA = m_pInfo->m_MapRTV_DATA[strRTV_NO];
		if(pRTV_DATA == NULL)
		{ 
			m_pRsw->MoveNext();
			continue; 
		};
		pRTV_DATA->SetAUTO_MODE_RD(m_pRsw->GetItem(_T("AUTO_MODE_RD")));
		pRTV_DATA->SetWAITING_ORDER_RD(m_pRsw->GetItem(_T("WAITING_ORDER_RD")));
		pRTV_DATA->SetSENSOR_RTV_RD(m_pRsw->GetItem(_T("SENSOR_RTV_RD")));
		pRTV_DATA->SetERR_CODE_RD(m_pRsw->GetItem(_T("ERR_CODE_RD")));
		pRTV_DATA->SetPOS_H_RD(m_pRsw->GetItem(_T("POS_H_RD")));
		pRTV_DATA->SetSPEED_RD(m_pRsw->GetItem(_T("SPEED_RD")));
		pRTV_DATA->SetFD_OVERTIME_RD(m_pRsw->GetItem(_T("FD_OVERTIME_RD")));
		pRTV_DATA->SetSENSOR_FK1_RD(m_pRsw->GetItem(_T("SENSOR_FK1_RD")));
		pRTV_DATA->SetSENSOR_FK2_RD(m_pRsw->GetItem(_T("SENSOR_FK2_RD")));
		pRTV_DATA->SetCMD_RQ_YN(m_pRsw->GetItem(_T("CMD_RQ_YN")));
		pRTV_DATA->SetCMD_RQ_ID(m_pRsw->GetItem(_T("CMD_RQ_ID")));
		pRTV_DATA->SetOD_RQ_YN(m_pRsw->GetItem(_T("OD_RQ_YN")));
		pRTV_DATA->SetRTV_PASSCV_OD(m_pRsw->GetItem(_T("RTV_PASSCV_OD")));
		pRTV_DATA->SetRTV_DEST_OD(m_pRsw->GetItem(_T("RTV_DEST_OD")));
		pRTV_DATA->SetLUGG_OD(m_pRsw->GetItem(_T("LUGG_OD")));
		pRTV_DATA->SetSTEP(m_pRsw->GetItem(_T("STEP")));
		pRTV_DATA->SetJOB_TYP_OD(m_pRsw->GetItem(_T("JOB_TYP_OD")));
		pRTV_DATA->SetDEPART_TRACK(m_pRsw->GetItem(_T("DEPART_TRACK")));
		pRTV_DATA->SetARRIVE_TRACK(m_pRsw->GetItem(_T("ARRIVE_TRACK")));
		pRTV_DATA->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pRTV_DATA->SetIS_ERROR_RD(m_pRsw->GetItem(_T("IS_ERROR_RD")));
		pRTV_DATA->SetEQP_TIME(m_pRsw->GetItem(_T("EQP_TIME")));
		pRTV_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem(_T("CONNECTED_YN")));

		if(GetModified() == TRUE)
		{
			//::PostMessage(m_pDoc->m_hWndView, WM_USER_REFRESH_DIALOG, 0, m_enKind);					
		}
		pRtvInfo->InvokeControl(pRTV_DATA);
		m_pRsw->MoveNext();
	}
}
void CRtv::CommandProc()
{
}

CString CRtv::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");
	strSql.Format(_T(" SELECT RD.RTV_NO,										  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.AUTO_MODE_RD,'0') AS AUTO_MODE_RD, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.WAITING_ORDER_RD,'0') AS WAITING_ORDER_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SENSOR_RTV_RD,'0') AS SENSOR_RTV_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.ERR_CODE_RD,'0') AS ERR_CODE_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RTV_HORI_DEF.RV_POS,'0') AS POS_H_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SPEED_RD,'0') AS SPEED_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.FD_OVERTIME_RD,'0') AS FD_OVERTIME_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SENSOR_FK1_RD,'0') AS SENSOR_FK1_RD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.SENSOR_FK2_RD,'0') AS SENSOR_FK2_RD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.CMD_RQ_YN,'0') AS CMD_RQ_YN, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.CMD_RQ_ID,'0') AS CMD_RQ_ID,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.OD_RQ_YN,'0') AS OD_RQ_YN, \n ")
		_T("        ") + m_pDoc->NVL + _T("(RD.RTV_PASSCV_OD,'0') AS RTV_PASSCV_OD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.RTV_DEST_OD,'0') AS RTV_DEST_OD, \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.LUGG_OD,'0') AS LUGG_OD,  \n")
		_T("        ") + m_pDoc->NVL + _T("(RD.STEP,'0') AS STEP, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.JOB_TYP_OD,'0') AS JOB_TYP_OD, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.DEPART_TRACK,'0') AS DEPART_TRACK, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.ARRIVE_TRACK,'0') AS ARRIVE_TRACK, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.SUSPEND,'0') AS SUSPEND, \n")
		_T("		") + m_pDoc->NVL + _T("(RD.IS_ERROR_RD,'0') AS IS_ERROR_RD, \n")
		_T("	     CASE WHEN TO_CHAR(NOW() - EM.UPD_DT, 'YYYYMMDDHH24MISS')::INTEGER > 5 THEN 6 ELSE 0 END AS EQP_TIME,		  											\n")
		_T("	     EM.CONNECTED_YN															  																			\n")
		_T("			 FROM RTV_DATA RD INNER JOIN (SELECT PLC_NO, PLC_IP, PLC_PORT_FROM, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = 'RTV') EM		\n")
		_T("                                     ON RD.PLC_NO = EM.PLC_NO \n")
		_T("                              LEFT OUTER JOIN RTV_HORI_DEF RTV_HORI_DEF \n")
		_T("                                     ON RTV_HORI_DEF.HORI = RD.POS_H_RD \n")
		_T("                                    AND RTV_HORI_DEF.PLC_NO = RD.PLC_NO \n")
		_T("                                    AND RTV_HORI_DEF.RTV_NO = RD.RTV_NO \n")
		_T("                                    AND RTV_HORI_DEF.WH_TYP = RD.WH_TYP \n")
		//_T("		EM.PLC_IP, EM.PLC_PORT_FROM, EM.CONNECTED_YN, EM.USE_YN \n")
		//_T("  FROM RTV_DATA RD INNER JOIN EQP_MST EM \n")
		//_T("                           ON EM.WH_TYP = RD.WH_TYP \n")
		//_T("                          AND RD.PLC_NO = EM.PLC_NO \n")
		_T(" WHERE RD.WH_TYP = '%s' \n")
		, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);

     	return strSql;
}

void CRtv::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CRtv::GetModified()
{
	if(m_pDoc->m_pRtvSkinDlg == NULL)
		return FALSE;

	CRTV_DATA* pRTV_DATA = ((CRtvSkinDlg*)m_pDoc->m_pRtvSkinDlg)->m_pRTV_DATA;
	if(pRTV_DATA == NULL)
		return FALSE;

	return  pRTV_DATA->m_bModified;
}