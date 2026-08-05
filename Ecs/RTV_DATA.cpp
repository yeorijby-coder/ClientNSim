#include "stdafx.h"
#include "RTV_DATA.h"


// RTV_DATA

CRTV_DATA::CRTV_DATA(void)
{
	m_pControl = NULL;
}
CRTV_DATA::CRTV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrRTV_NO)
{
	m_pControl = NULL;
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_RTV_NO = pstrRTV_NO;
}

CRTV_DATA::~CRTV_DATA()
{
}

void CRTV_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CRTV_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CRTV_DATA::SetRTV_NO(CString pstrRTV_NO)
{
	K_RTV_NO = pstrRTV_NO;
}

void CRTV_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if (V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;

	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetWAITING_ORDER_RD(CString pWAITING_ORDER_RD)
{
	if (V_WAITING_ORDER_RD == pWAITING_ORDER_RD)
		return;

	V_WAITING_ORDER_RD = pWAITING_ORDER_RD;
	m_bModified = true;
}

void CRTV_DATA::SetSENSOR_RTV_RD(CString pSENSOR_RTV_RD)
{
	if (V_SENSOR_RTV_RD == pSENSOR_RTV_RD)
		return;

	V_SENSOR_RTV_RD = pSENSOR_RTV_RD;
	m_bModified = true;
}

void CRTV_DATA::SetERR_CODE_RD(CString pERR_CODE_RD)
{
	if (V_ERR_CODE_RD == pERR_CODE_RD)
		return;

	V_ERR_CODE_RD = pERR_CODE_RD;
	m_bModified = true;
}

void CRTV_DATA::SetPOS_H_RD(CString pPOS_H_RD)
{
	if(V_POS_H_RD == pPOS_H_RD)
		return;

	V_POS_H_RD = pPOS_H_RD;
	m_bModified = true;
}

void CRTV_DATA::SetSPEED_RD(CString pSPEED_RD)
{
	if (V_SPEED_RD == pSPEED_RD)
		return;

	V_SPEED_RD = pSPEED_RD;
	m_bModified = true;
}

void CRTV_DATA::SetFD_OVERTIME_RD(CString pFD_OVERTIME_RD)
{
	if (V_FD_OVERTIME_RD == pFD_OVERTIME_RD)
		return;

	V_FD_OVERTIME_RD = pFD_OVERTIME_RD;
	m_bModified = true;
}

void CRTV_DATA::SetSENSOR_FK1_RD(CString pSENSOR_FK1_RD)
{
	if (V_SENSOR_FK1_RD == pSENSOR_FK1_RD)
		return;

	V_SENSOR_FK1_RD = pSENSOR_FK1_RD;
	m_bModified = true;
}

void CRTV_DATA::SetSENSOR_FK2_RD(CString pSENSOR_FK2_RD)
{
	if (V_SENSOR_FK2_RD == pSENSOR_FK2_RD)
		return;

	V_SENSOR_FK2_RD = pSENSOR_FK2_RD;
	m_bModified = true;
}

void CRTV_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if (V_CMD_RQ_YN == pCMD_RQ_YN)
		return;

	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CRTV_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if (V_CMD_RQ_ID == pCMD_RQ_ID)
		return;

	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CRTV_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if (V_OD_RQ_YN == pOD_RQ_YN)
		return;

	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;
}

void CRTV_DATA::SetRTV_PASSCV_OD(CString pRTV_PASSCV_OD)
{
	if (V_RTV_PASSCV_OD == pRTV_PASSCV_OD)
		return;

	V_RTV_PASSCV_OD = pRTV_PASSCV_OD;
	m_bModified = true;
}

void CRTV_DATA::SetRTV_DEST_OD(CString pRTV_DEST_OD)
{
	if(V_RTV_DEST_OD == pRTV_DEST_OD)
		return;

	V_RTV_DEST_OD = pRTV_DEST_OD;
	m_bModified = true;
}

void CRTV_DATA::SetLUGG_OD(CString pLUGG_OD)
{
	if (V_LUGG_OD == pLUGG_OD)
		return;

	V_LUGG_OD = pLUGG_OD;
	m_bModified = true;
}

void CRTV_DATA::SetSTEP(CString pSTEP)
{
	if (V_STEP == pSTEP)
		return;

	V_STEP = pSTEP;
	m_bModified = true;
}

void CRTV_DATA::SetJOB_TYP_OD(CString pJOB_TYP_OD)
{
	if (V_JOB_TYP_OD == pJOB_TYP_OD)
		return;

	V_JOB_TYP_OD = pJOB_TYP_OD;
	m_bModified = true;
}

void CRTV_DATA::SetDEPART_TRACK(CString pDEPART_TRACK)
{
	if (V_DEPART_TRACK == pDEPART_TRACK)
		return;

	V_DEPART_TRACK = pDEPART_TRACK;
	m_bModified = true;
}

void CRTV_DATA::SetARRIVE_TRACK(CString pARRIVE_TRACK)
{
	if (V_ARRIVE_TRACK == pARRIVE_TRACK)
		return;

	V_ARRIVE_TRACK = pARRIVE_TRACK;
	m_bModified = true;
}

void CRTV_DATA::SetSUSPEND(CString pSUSPEND)
{
	if (V_SUSPEND == pSUSPEND)
		return;

	V_SUSPEND = pSUSPEND;
	m_bModified = true;
}

void CRTV_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if (V_OD_USER_ID == pOD_USER_ID)
		return;

	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;
}

void CRTV_DATA::SetIS_ERROR_RD(CString pIS_ERROR_RD)
{
	if (V_IS_ERROR_RD == pIS_ERROR_RD)
		return;

	V_IS_ERROR_RD = pIS_ERROR_RD;
	m_bModified = true;
}

void CRTV_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;



	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;
}

void CRTV_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;
}

void CRTV_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if (V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CRTV_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if (V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CRTV_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if (V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}
CString CRTV_DATA::GetCid()
{
	CString strCID;
	strCID.Format(_T("17130%03s"), K_RTV_NO);
	return strCID;
}