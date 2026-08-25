#pragma once
#include "afx.h"

#include "DciRvCtrl.h"
#include "DciButtonCtrl.h"

class CSC_DATA : public CObject
{
public:
	CSC_DATA(void);
	CSC_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrSC_NO);

	~CSC_DATA(void);

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_SC_NO;

public:
	void SetWH_TYP(CString K_WH_TYP);
	void SetPLC_NO(CString K_PLC_NO);
	void SetSC_NO(CString pSC_NO);

public:
	CString V_SC_GRP_NO;
	CString V_MC_NO;
	CString V_MC_NO_NM;
	CString V_RESPONSE_CODE_RD;
	CString V_ERR_CODE_RD;
	CString V_CRANE_STATUS_RD;
	CString V_CRANE_ONLINE_RD;
	CString V_CRANE_REQUEST_RD;
	CString V_IS_ERROR_RD;
	CString V_RECOVERABLE_ERROR_RD;
	CString V_CRANE_AT_HP_RD;
	CString V_WARMING_UP_RD;
	CString V_PROD_CHECK_RD;
	CString V_ORDER_CHECK_RD;
	CString V_JOB_TYP;
	CString V_LUGG_NO;
	CString V_ITN_LUGG;
	CString V_START_BANK;
	CString V_START_BAY;
	CString V_START_LEVEL;
	CString V_START_HSPOS;
	CString V_START_DEEP_CELL;
	CString V_DEST_BANK;
	CString V_DEST_BAY;
	CString V_DEST_LEVEL;
	CString V_DEST_HSPOS;
	CString V_DEST_DEEP_CELL;
	CString V_SUSPEND;
	CString V_SC_TYP;
	CString V_ONLINE_MODE_RD;
	CString V_AUTO_MODE_RD;
	CString V_ACTIVE_MODE_RD;
	CString V_UCSTATUS_RD;
	CString V_SENSOR_FK_RD;
	CString V_POS_H_RD;
	CString V_POS_V_RD;
	CString V_COMPLETE_RD;
	CString V_ERR_STA_FK1_RD;
	CString V_ERR_STA_FK2_RD;
	CString V_OD_RQ_YN;
	CString V_OD_RQ_FLAG;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;

	CString V_Prev_DEST_BAY;

	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;
	CString V_OD_USER_ID;
	CTime V_OD_UPD_DT;

	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;

public:
	BOOL m_bModified;

public:
	CDciRvCtrl* m_pControl;
	//CDciRvCtrl* m_pControl2;
	//CDciRvCtrl* m_pControl3;
	//CDciRvCtrl* m_pControl4;
	//CDciRvCtrl* m_pControl5;

public:
	void SetSC_GRP_NO(CString pSC_GRP_NO);
	void SetONLINE_MODE_RD(CString pONLINE_MODE_RD);
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetERR_CODE_RD(CString pERR_CODE_RD);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);
	void SetOD_USER_ID(CString pOD_USER_ID);
	void SetOD_UPD_DT(CTime pOD_UPD_DT);

	void SetMC_NO(CString pMC_NO);
	void SetMC_NO_NM(CString pMC_NO_NM);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);

	void SetRESPONSE_CODE_RD(CString V_RESPONSE_CODE_RD);
	void SetCRANE_STATUS_RD(CString V_CRANE_STATUS_RD);
	void SetCRANE_ONLINE_RD(CString V_CRANE_ONLINE_RD);
	void SetCRANE_REQUEST_RD(CString V_CRANE_REQUEST_RD);
	void SetIS_ERROR_RD(CString V_IS_ERROR_RD);
	void SetRECOVERABLE_ERROR_RD(CString V_RECOVERABLE_ERROR_RD);
	void SetCRANE_AT_HP_RD(CString V_CRANE_AT_HP_RD);
	void SetWARMING_UP_RD(CString V_WARMING_UP_RD);
	void SetPROD_CHECK_RD(CString V_PROD_CHECK_RD);
	void SetORDER_CHECK_RD(CString V_ORDER_CHECK_RD);
	void SetJOB_TYP(CString V_JOB_TYP);
	void SetLUGG_NO(CString V_LUGG_NO);
	void SetITN_LUGG(CString V_ITN_LUGG);
	void SetSTART_BANK(CString V_START_BANK);
	void SetSTART_BAY(CString V_START_BAY);
	void SetSTART_LEVEL(CString V_START_LEVEL);
	void SetSTART_HSPOS(CString V_START_HSPOS);
	void SetSTART_DEEP_CELL(CString V_START_DEEP_CELL);
	void SetDEST_BANK(CString V_DEST_BANK);
	void SetDEST_BAY(CString V_DEST_BAY);
	void SetDEST_LEVEL(CString V_DEST_LEVEL);
	void SetDEST_HSPOS(CString V_DEST_HSPOS);
	void SetDEST_DEEP_CELL(CString V_DEST_DEEP_CELL);
	void SetSUSPEND(CString V_SUSPEND);
	void SetSC_TYP(CString V_SC_TYP);
	void SetACTIVE_MODE_RD(CString pACTIVE_MODE_RD);
	void SetUCSTATUS_RD(CString pUCSTATUS_RD);
	void SetSENSOR_FK_RD(CString pSENSOR_FK_RD);
	void SetPOS_H_RD(CString pPOS_H_RD);
	void SetPOS_V_RD(CString pPOS_V_RD);
	void SetCOMPLETE_RD(CString pCOMPLETE_RD);
	void SetERR_STA_FK1_RD(CString pERR_STA_FK1_RD);
	void SetERR_STA_FK2_RD(CString pERR_STA_FK2_RD);

	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetOD_RQ_FLAG(CString pOD_RQ_FLAG);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);

public:
	CString GetCid();
	CString GetCid(CString pnChar4);
};

