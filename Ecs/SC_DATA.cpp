#include "StdAfx.h"
#include "SC_DATA.h"


CSC_DATA::CSC_DATA(void)
{
	m_pControl = NULL;
	m_bModified = TRUE;
}


CSC_DATA::CSC_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrSC_NO)
{
	m_pControl = NULL;
	m_bModified = TRUE;

	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_SC_NO = pstrSC_NO;
}

CSC_DATA::~CSC_DATA(void)
{
}
#pragma region ±‚¡∏
void CSC_DATA::SetSC_NO(CString pstrSC_NO)
{
	K_SC_NO = pstrSC_NO;
}

void CSC_DATA::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CSC_DATA::SetPLC_NO(CString pstrPLC_NO)
{
	K_PLC_NO = pstrPLC_NO;
}

void CSC_DATA::SetSC_GRP_NO(CString pSC_GRP_NO)
{
	if(V_SC_GRP_NO == pSC_GRP_NO)
		return;

	V_SC_GRP_NO = pSC_GRP_NO;
	m_bModified = true;
}

void CSC_DATA::SetONLINE_MODE_RD(CString pONLINE_MODE_RD)
{
	if(V_ONLINE_MODE_RD == pONLINE_MODE_RD)
		return;

	V_ONLINE_MODE_RD = pONLINE_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetAUTO_MODE_RD(CString pAUTO_MODE_RD)
{
	if(V_AUTO_MODE_RD == pAUTO_MODE_RD)
		return;

	V_AUTO_MODE_RD = pAUTO_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD)
{
	if(V_ACTIVE_MODE_RD == pACTIVE_MODE_RD)
		return;

	V_ACTIVE_MODE_RD = pACTIVE_MODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetUCSTATUS_RD(CString pUCSTATUS_RD)
{
	if(V_UCSTATUS_RD == pUCSTATUS_RD)
		return;

	V_UCSTATUS_RD = pUCSTATUS_RD;
	m_bModified = true;
}

void CSC_DATA::SetSENSOR_FK_RD(CString pSENSOR_FK_RD)
{
	if(V_SENSOR_FK_RD == pSENSOR_FK_RD)
		return;

	V_SENSOR_FK_RD = pSENSOR_FK_RD;
	m_bModified = true;
}

void CSC_DATA::SetPOS_H_RD(CString pPOS_H_RD)
{
	if(V_POS_H_RD == pPOS_H_RD)
		return;

	V_POS_H_RD = pPOS_H_RD;
	m_bModified = true;
}

void CSC_DATA::SetPOS_V_RD(CString pPOS_V_RD)
{
	if(V_POS_V_RD == pPOS_V_RD)
		return;

	V_POS_V_RD = pPOS_V_RD;
	m_bModified = true;
}

void CSC_DATA::SetCOMPLETE_RD(CString pCOMPLETE_RD)
{
	if(V_COMPLETE_RD == pCOMPLETE_RD)
		return;

	V_COMPLETE_RD = pCOMPLETE_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_STA_FK1_RD(CString pERR_STA_FK1_RD)
{
	if(V_ERR_STA_FK1_RD == pERR_STA_FK1_RD)
		return;

	V_ERR_STA_FK1_RD = pERR_STA_FK1_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_STA_FK2_RD(CString pERR_STA_FK2_RD)
{
	if(V_ERR_STA_FK2_RD == pERR_STA_FK2_RD)
		return;

	V_ERR_STA_FK2_RD = pERR_STA_FK2_RD;
	m_bModified = true;
}

void CSC_DATA::SetERR_CODE_RD(CString pERR_CODE_RD)
{
	if(V_ERR_CODE_RD == pERR_CODE_RD)
		return;

	V_ERR_CODE_RD = pERR_CODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetOD_RQ_YN(CString pOD_RQ_YN)
{
	if(V_OD_RQ_YN == pOD_RQ_YN)
		return;

	V_OD_RQ_YN = pOD_RQ_YN;
	m_bModified = true;

}

void CSC_DATA::SetREAD_UPD_DT(CTime pREAD_UPD_DT)
{
	if(V_READ_UPD_DT == pREAD_UPD_DT)
		return;

	V_READ_UPD_DT = pREAD_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetWRITE_UPD_DT(CTime pWRITE_UPD_DT)
{
	if(V_WRITE_UPD_DT == pWRITE_UPD_DT)
		return;



	V_WRITE_UPD_DT = pWRITE_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetOD_USER_ID(CString pOD_USER_ID)
{
	if(V_OD_USER_ID == pOD_USER_ID)
		return;



	V_OD_USER_ID = pOD_USER_ID;
	m_bModified = true;

}

void CSC_DATA::SetOD_UPD_DT(CTime pOD_UPD_DT)
{
	if(V_OD_UPD_DT == pOD_UPD_DT)
		return;



	V_OD_UPD_DT = pOD_UPD_DT;
	m_bModified = true;

}

void CSC_DATA::SetOD_RQ_FLAG(CString pOD_RQ_FLAG)
{
	if(V_OD_RQ_FLAG == pOD_RQ_FLAG)
		return;

	V_OD_RQ_FLAG = pOD_RQ_FLAG;
	m_bModified = true;

}

void CSC_DATA::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if(V_CMD_RQ_ID == pCMD_RQ_ID)
		return;



	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;

}

void CSC_DATA::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if(V_CMD_RQ_YN == pCMD_RQ_YN)
		return;



	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;

}

void CSC_DATA::SetMC_NO(CString pMC_NO)
{
	if(V_MC_NO == pMC_NO)
		return;

	V_MC_NO = pMC_NO;
	m_bModified = true;

}

void CSC_DATA::SetMC_NO_NM(CString pMC_NO_NM)
{
	if(V_MC_NO_NM == pMC_NO_NM)
		return;

	V_MC_NO_NM = pMC_NO_NM;
	m_bModified = true;

}

void CSC_DATA::SetEQP_TIME(CString pstrEQP_TIME)
{
	if(V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CSC_DATA::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if(V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CSC_DATA::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if (V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}

#pragma endregion
void CSC_DATA::SetRESPONSE_CODE_RD(CString pstrRESPONSE_CODE_RD)
{
	if (V_RESPONSE_CODE_RD == pstrRESPONSE_CODE_RD)
		return;

	V_RESPONSE_CODE_RD = pstrRESPONSE_CODE_RD;
	m_bModified = true;
}

void CSC_DATA::SetCRANE_STATUS_RD(CString pstrCRANE_STATUS_RD)
{
	if (V_CRANE_STATUS_RD == pstrCRANE_STATUS_RD)
		return;

	V_CRANE_STATUS_RD = pstrCRANE_STATUS_RD;
	m_bModified = true;
}

void CSC_DATA::SetCRANE_ONLINE_RD(CString pstrCRANE_ONLINE_RD)
{
	if (V_CRANE_ONLINE_RD == pstrCRANE_ONLINE_RD)
		return;

	V_CRANE_ONLINE_RD = pstrCRANE_ONLINE_RD;
	m_bModified = true;
}

void CSC_DATA::SetCRANE_REQUEST_RD(CString pstrCRANE_REQUEST_RD)
{
	if (V_CRANE_REQUEST_RD == pstrCRANE_REQUEST_RD)
		return;

	V_CRANE_REQUEST_RD = pstrCRANE_REQUEST_RD;
	m_bModified = true;
}

void CSC_DATA::SetIS_ERROR_RD(CString pstrIS_ERROR_RD)
{
	if (V_IS_ERROR_RD == pstrIS_ERROR_RD)
		return;

	V_IS_ERROR_RD = pstrIS_ERROR_RD;
	m_bModified = true;
}

void CSC_DATA::SetRECOVERABLE_ERROR_RD(CString pstrRECOVERABLE_ERROR_RD)
{
	if (V_RECOVERABLE_ERROR_RD == pstrRECOVERABLE_ERROR_RD)
		return;

	V_RECOVERABLE_ERROR_RD = pstrRECOVERABLE_ERROR_RD;
	m_bModified = true;
}

void CSC_DATA::SetCRANE_AT_HP_RD(CString pstrCRANE_AT_HP_RD)
{
	if (V_CRANE_AT_HP_RD == pstrCRANE_AT_HP_RD)
		return;

	V_CRANE_AT_HP_RD = pstrCRANE_AT_HP_RD;
	m_bModified = true;
}

void CSC_DATA::SetWARMING_UP_RD(CString pstrWARMING_UP_RD)
{
	if (V_WARMING_UP_RD == pstrWARMING_UP_RD)
		return;

	V_WARMING_UP_RD = pstrWARMING_UP_RD;
	m_bModified = true;
}

void CSC_DATA::SetPROD_CHECK_RD(CString pstrPROD_CHECK_RD)
{
	if (V_PROD_CHECK_RD == pstrPROD_CHECK_RD)
		return;

	V_PROD_CHECK_RD = pstrPROD_CHECK_RD;
	m_bModified = true;
}

void CSC_DATA::SetORDER_CHECK_RD(CString pstrORDER_CHECK_RD)
{
	if (V_ORDER_CHECK_RD == pstrORDER_CHECK_RD)
		return;

	V_ORDER_CHECK_RD = pstrORDER_CHECK_RD;
	m_bModified = true;
}

void CSC_DATA::SetJOB_TYP(CString pstrJOB_TYP)
{
	if (V_JOB_TYP == pstrJOB_TYP)
		return;

	V_JOB_TYP = pstrJOB_TYP;
	m_bModified = true;
}

void CSC_DATA::SetLUGG_NO(CString pstrLUGG_NO)
{
	if (V_LUGG_NO == pstrLUGG_NO)
		return;

	V_LUGG_NO = pstrLUGG_NO;
	m_bModified = true;
}

void CSC_DATA::SetITN_LUGG(CString pstrITN_LUGG)
{
	if (V_ITN_LUGG == pstrITN_LUGG)
		return;

	V_ITN_LUGG = pstrITN_LUGG;
	m_bModified = true;
}

void CSC_DATA::SetSTART_BANK(CString pstrSTART_BANK)
{
	if (V_START_BANK == pstrSTART_BANK)
		return;

	V_START_BANK = pstrSTART_BANK;
	m_bModified = true;
}

void CSC_DATA::SetSTART_BAY(CString pstrSTART_BAY)
{
	if (V_START_BAY == pstrSTART_BAY)
		return;

	V_START_BAY = pstrSTART_BAY;
	m_bModified = true;
}

void CSC_DATA::SetSTART_LEVEL(CString pstrSTART_LEVEL)
{
	if (V_START_LEVEL == pstrSTART_LEVEL)
		return;

	V_START_LEVEL = pstrSTART_LEVEL;
	m_bModified = true;
}

void CSC_DATA::SetSTART_HSPOS(CString pstrSTART_HSPOS)
{
	if (V_START_HSPOS == pstrSTART_HSPOS)
		return;

	V_START_HSPOS = pstrSTART_HSPOS;
	m_bModified = true;
}

void CSC_DATA::SetSTART_DEEP_CELL(CString pstrSTART_DEEP_CELL)
{
	if (V_START_DEEP_CELL == pstrSTART_DEEP_CELL)
		return;

	V_START_DEEP_CELL = pstrSTART_DEEP_CELL;
	m_bModified = true;
}

void CSC_DATA::SetDEST_BANK(CString pstrDEST_BANK)
{
	if (V_DEST_BANK == pstrDEST_BANK)
		return;

	V_DEST_BANK = pstrDEST_BANK;
	m_bModified = true;
}

void CSC_DATA::SetDEST_BAY(CString pstrDEST_BAY)
{
	if (V_DEST_BAY == pstrDEST_BAY)
		return;

	V_DEST_BAY = pstrDEST_BAY;
	m_bModified = true;
}

void CSC_DATA::SetDEST_LEVEL(CString pstrDEST_LEVEL)
{
	if (V_DEST_LEVEL == pstrDEST_LEVEL)
		return;

	V_DEST_LEVEL = pstrDEST_LEVEL;
	m_bModified = true;
}

void CSC_DATA::SetDEST_HSPOS(CString pstrDEST_HSPOS)
{
	if (V_DEST_HSPOS == pstrDEST_HSPOS)
		return;

	V_DEST_HSPOS = pstrDEST_HSPOS;
	m_bModified = true;
}

void CSC_DATA::SetDEST_DEEP_CELL(CString pstrDEST_DEEP_CELL)
{
	if (V_DEST_DEEP_CELL == pstrDEST_DEEP_CELL)
		return;

	V_DEST_DEEP_CELL = pstrDEST_DEEP_CELL;
	m_bModified = true;
}

void CSC_DATA::SetSUSPEND(CString pstrSuspend)
{
	if (V_SUSPEND == pstrSuspend)
		return;

	V_SUSPEND = pstrSuspend;
	m_bModified = true;
}

void CSC_DATA::SetSC_TYP(CString pstrScTyp)
{
	if (V_SC_TYP == pstrScTyp)
		return;

	V_SC_TYP = pstrScTyp;
	m_bModified = true;
}

CString CSC_DATA::GetCid()
{
	CString strCID;
	strCID.Format(_T("17120%03s"), K_SC_NO);
	return strCID;
}

CString CSC_DATA::GetCid(CString nChar4)
{
	CString strCID;
	strCID.Format(_T("17120%03s"), K_SC_NO);
	return strCID;
}