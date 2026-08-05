#include "stdafx.h"
#include "CV_DATA.h"


// CCV_DATA

CCV_DATA::CCV_DATA(void)
{
}

CCV_DATA::CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrTRACK_NO) : CInfo(pstrWH_TYP, pstrPLC_NO, pstrTRACK_NO)
{
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_TRACK_NO = pstrTRACK_NO;
}

CCV_DATA::CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO) : CInfo(pstrWH_TYP, pstrPLC_NO)
{
	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
}


CCV_DATA::~CCV_DATA(void)
{
}

void CCV_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CCV_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CCV_DATA::SetTRACK_NO(CString pstrTRACK_NO)
{
	K_TRACK_NO = pstrTRACK_NO;
}


void CCV_DATA::SetLUGG_NO_RD(CString pLUGG_NO_RD)
{
	if(V_LUGG_NO_RD == pLUGG_NO_RD)
		return;

	V_LUGG_NO_RD = pLUGG_NO_RD;
	m_bModified = true;
}

void CCV_DATA::SetLUGG_NO_OD(CString pLUGG_NO_OD)
{
	if(V_LUGG_NO_OD == pLUGG_NO_OD)
		return;

	V_LUGG_NO_OD = pLUGG_NO_OD;
	m_bModified = true;
}

void CCV_DATA::SetDEST_POS_RD(CString pDEST_POS_RD)
{
	if(V_DEST_POS_RD == pDEST_POS_RD)
		return;



	V_DEST_POS_RD = pDEST_POS_RD;
	m_bModified = true;
}

void CCV_DATA::SetDEST_POS_OD(CString pDEST_POS_OD)
{
	if(V_DEST_POS_OD == pDEST_POS_OD)
		return;



	V_DEST_POS_OD = pDEST_POS_OD;
	m_bModified = true;
}

void CCV_DATA::SetJOB_TYP_RD(CString pJOB_TYP_RD)
{
	if(V_JOB_TYP_RD == pJOB_TYP_RD)
		return;



	V_JOB_TYP_RD = pJOB_TYP_RD;
	m_bModified = true;
}

void CCV_DATA::SetJOB_TYP_OD(CString pJOB_TYP_OD)
{
	if(V_JOB_TYP_OD == pJOB_TYP_OD)
		return;



	V_JOB_TYP_OD = pJOB_TYP_OD;
	m_bModified = true;
}

void CCV_DATA::SetERR_RQ_RD(CString pERR_RQ_RD)
{
	if(V_ERR_RQ_RD == pERR_RQ_RD)
		return;



	V_ERR_RQ_RD = pERR_RQ_RD;
	m_bModified = true;
}

void CCV_DATA::SetERR_RQ_OD(CString pERR_RQ_OD)
{
	if(V_ERR_RQ_OD == pERR_RQ_OD)
		return;



	V_ERR_RQ_OD = pERR_RQ_OD;
	m_bModified = true;
}

void CCV_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if(V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;



	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CCV_DATA::SetSTO_READY_RD(CString pSTO_READY_RD)
{
	if(V_STO_READY_RD == pSTO_READY_RD)
		return;



	V_STO_READY_RD = pSTO_READY_RD;
	m_bModified = true;
}


void CCV_DATA::SetRET_READY_RD(CString pRET_READY_RD)
{
	if(V_RET_READY_RD == pRET_READY_RD)
		return;



	V_RET_READY_RD = pRET_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetSTOHS_READY_RD(CString pSTOHS_READY_RD)
{
	if(V_STOHS_READY_RD == pSTOHS_READY_RD)
		return;



	V_STOHS_READY_RD = pSTOHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRETHS_READY_RD(CString pRETHS_READY_RD)
{
	if(V_RETHS_READY_RD == pRETHS_READY_RD)
		return;



	V_RETHS_READY_RD = pRETHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRTV_DEPARTHS_READY_RD(CString pRTV_DEPARTHS_READY_RD)
{
	if(V_RTV_DEPARTHS_READY_RD == pRTV_DEPARTHS_READY_RD)
		return;

	V_RTV_DEPARTHS_READY_RD = pRTV_DEPARTHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetRTV_ARRIVEHS_READY_RD(CString pRTV_ARRIVEHS_READY_RD)
{
	if(V_RTV_ARRIVEHS_READY_RD == pRTV_ARRIVEHS_READY_RD)
		return;



	V_RTV_ARRIVEHS_READY_RD = pRTV_ARRIVEHS_READY_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR0_DATA_RD(CString pSENSOR0_DATA_RD)
{
	if(V_SENSOR0_DATA_RD == pSENSOR0_DATA_RD)
		return;



	V_SENSOR0_DATA_RD = pSENSOR0_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR1_DATA_RD(CString pSENSOR1_DATA_RD)
{
	if(V_SENSOR1_DATA_RD == pSENSOR1_DATA_RD)
		return;



	V_SENSOR1_DATA_RD = pSENSOR1_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetSENSOR2_DATA_RD(CString pSENSOR2_DATA_RD)
{
	if(V_SENSOR2_DATA_RD == pSENSOR2_DATA_RD)
		return;



	V_SENSOR2_DATA_RD = pSENSOR2_DATA_RD;
	m_bModified = true;
}

void CCV_DATA::SetERROR_CODE(CString pERROR_CODE)
{
	if(V_ERROR_CODE == pERROR_CODE)
		return;

	V_ERROR_CODE = pERROR_CODE;
	m_bModified = true;
}

void CCV_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if(V_OD_RQ_YN == pOD_RQ_YN)
		return;



	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;
}

void CCV_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;



	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if(V_OD_USER_ID == pOD_USER_ID)
		return;



	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;
}

void CCV_DATA::SetOD_UPD_DT(CTime pOD_UPD_DT)
{
	if(V_OD_UPD_DT == pOD_UPD_DT)
		return;



	V_OD_UPD_DT = pOD_UPD_DT;
	m_bModified = true;
}

void CCV_DATA::SetUSE_YN(CString pUSE_YN)
{
	if(V_USE_YN == pUSE_YN)
		return;



	V_USE_YN = pUSE_YN;
	m_bModified = true;
}

void CCV_DATA::SetOD_RQ_FLAG(CString pOD_RQ_FLAG)
{
	if(V_OD_RQ_FLAG == pOD_RQ_FLAG)
		return;



	V_OD_RQ_FLAG = pOD_RQ_FLAG;
	m_bModified = true;
}

void CCV_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CCV_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CCV_DATA::SetMC_NO(CString pMC_NO)
{
	if (V_MC_NO == pMC_NO)
		return;

	V_MC_NO = pMC_NO;
	m_bModified = true;
}

void CCV_DATA::SetMC_NO_NM(CString pMC_NO_NM)
{
	if (V_MC_NO_NM == pMC_NO_NM)
		return;

	V_MC_NO_NM = pMC_NO_NM;
	m_bModified = true;
}

void CCV_DATA::SetTR_PAUSE_RD(CString pTR_PAUSE_RD)
{
	if (V_TR_PAUSE_RD == pTR_PAUSE_RD)
		return;

	V_TR_PAUSE_RD = pTR_PAUSE_RD;
	m_bModified = true;
}

void CCV_DATA::SetTR_PAUSE_OD(CString pTR_PAUSE_OD)
{
	if (V_TR_PAUSE_OD == pTR_PAUSE_OD)
		return;

	V_TR_PAUSE_OD = pTR_PAUSE_OD;
	m_bModified = true;
}

void CCV_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if(V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CCV_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if(V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CCV_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if(V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_RD(CString pCOMMING_RD)
{
	if (V_COMMING_RD == pCOMMING_RD)
		return;

	V_COMMING_RD = pCOMMING_RD;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_OD(CString pCOMMING_OD)
{
	if (V_COMMING_OD == pCOMMING_OD)
		return;

	V_COMMING_OD = pCOMMING_OD;
	m_bModified = true;
}

void CCV_DATA::SetDELETE_TRACK_RD(CString pDELETE_TRACK_RD)
{
	if (V_DELETE_TRACK_RD == pDELETE_TRACK_RD)
		return;

	V_DELETE_TRACK_RD = pDELETE_TRACK_RD;
	m_bModified = true;
}

void CCV_DATA::SetRTV_LOCK_SENSOR(CString pRTV_LOCK_SENSOR)
{
	if (V_RTV_LOCK_SENSOR == pRTV_LOCK_SENSOR)
		return;

	V_RTV_LOCK_SENSOR = pRTV_LOCK_SENSOR;
	m_bModified = true;
}

void CCV_DATA::SetBARCODE(CString pBARCODE)
{
	if (V_BARCODE == pBARCODE)
		return;

	V_BARCODE = pBARCODE;
	m_bModified = true;
}

void CCV_DATA::SetJMBARCODE(CString pJMBARCODE)
{
	if (V_JMBARCODE == pJMBARCODE)
		return;

	V_JMBARCODE = pJMBARCODE;
	m_bModified = true;
}

void CCV_DATA::SetHOST_SEND_YN(CString pHOST_SEND_YN)
{
	if (V_HOST_SEND_YN == pHOST_SEND_YN)
		return;

	V_HOST_SEND_YN = pHOST_SEND_YN;
	m_bModified = true;
}

void CCV_DATA::SetHOST_ERR_SEND_YN(CString pHOST_ERR_SEND_YN)
{
	if (V_HOST_ERR_SEND_YN == pHOST_ERR_SEND_YN)
		return;

	V_HOST_ERR_SEND_YN = pHOST_ERR_SEND_YN;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_CHECK_TR1(CString pCOMMING_CHECK_TR1)
{
	if (V_COMMING_CHECK_TR1 == pCOMMING_CHECK_TR1)
		return;

	V_COMMING_CHECK_TR1 = pCOMMING_CHECK_TR1;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_CHECK_TR2(CString pCOMMING_CHECK_TR2)
{
	if (V_COMMING_CHECK_TR2 == pCOMMING_CHECK_TR2)
		return;

	V_COMMING_CHECK_TR2 = pCOMMING_CHECK_TR2;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_CHECK_TR3(CString pCOMMING_CHECK_TR3)
{
	if (V_COMMING_CHECK_TR3 == pCOMMING_CHECK_TR3)
		return;

	V_COMMING_CHECK_TR3 = pCOMMING_CHECK_TR3;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_CHECK_TR4(CString pCOMMING_CHECK_TR4)
{
	if (V_COMMING_CHECK_TR4 == pCOMMING_CHECK_TR4)
		return;

	V_COMMING_CHECK_TR4 = pCOMMING_CHECK_TR4;
	m_bModified = true;
}

void CCV_DATA::SetCOMMING_DEST_TR(CString pCOMMING_DEST_TR)
{
	if (V_COMMING_DEST_TR == pCOMMING_DEST_TR)
		return;

	V_COMMING_DEST_TR = pCOMMING_DEST_TR;
	m_bModified = true;
}

void CCV_DATA::SetSTN_KIND(CString pSTN_KIND)
{
	if (V_STN_KIND == pSTN_KIND)
		return;

	V_STN_KIND = pSTN_KIND;
	m_bModified = true;
}

void CCV_DATA::SetDOOR_STATUS_RD(CString pDOOR_STATUS_RD, WORD& wDoorStatus)
{
	if (V_DOOR_STATUS_RD == pDOOR_STATUS_RD)
		return;

	V_DOOR_STATUS_RD = pDOOR_STATUS_RD;
	wDoorStatus = (WORD)CConvert::ToInt(pDOOR_STATUS_RD);
	m_bModified = true;
}

CString CCV_DATA::GetMapKeyCV()
{
	CString strReturn;
	strReturn.Format(_T("%02s%02d"), m_strWH_TYP, m_nPlcNo);
	return strReturn;
}



CString CCV_DATA::GetMapKeyTRACK()
{
	CString strReturn;
	strReturn.Format(_T("%05d"), m_nNumber);
	return strReturn;
}