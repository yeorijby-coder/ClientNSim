#pragma once
#include "afx.h"
#include "DciRvCtrl.h"

// RTV_DATA 명령 대상입니다.

class CRTV_DATA : public CObject
{
public:
	CRTV_DATA(void);
	CRTV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrRTV_NO);

	~CRTV_DATA(void);

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_RTV_NO;

public:
	void SetWH_TYP(CString pWH_TYP);
	void SetPLC_NO(CString pPLC_NO);
	void SetRTV_NO(CString pRTV_NO);

public:

	//data
	CString V_AUTO_MODE_RD;
	CString V_WAITING_ORDER_RD;
	CString V_SENSOR_RTV_RD;
	CString V_ERR_CODE_RD;
	CString	V_POS_H_RD;
	CString V_SPEED_RD;
	CString V_FD_OVERTIME_RD;
	CString V_SENSOR_FK1_RD;
	CString V_SENSOR_FK2_RD;
	CString V_CMD_RQ_YN;
	CString V_CMD_RQ_ID;
	CString V_OD_RQ_YN;
	CString V_RTV_PASSCV_OD;
	CString V_RTV_DEST_OD;
	CString V_LUGG_OD;
	CString V_STEP;
	CString V_JOB_TYP_OD;
	CString V_DEPART_TRACK;
	CString V_ARRIVE_TRACK;
	CString V_SUSPEND;
	CString V_OD_USER_ID;
	CString V_SC_TYP;
	CString V_IS_ERROR_RD;
	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;

	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;

public:
	BOOL m_bModified;
	CDciRvCtrl* m_pControl;

public:
	CMap<CString, LPCTSTR, int, int> m_MapRtvPosition;

public:
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetWAITING_ORDER_RD(CString pWAITING_ORDER_RD);
	void SetSENSOR_RTV_RD(CString pSENSOR_RTV_RD);
	void SetERR_CODE_RD(CString pERR_CODE_RD);
	void SetPOS_H_RD(CString pPOS_H_RD);
	void SetSPEED_RD(CString pSPEED_RD);
	void SetFD_OVERTIME_RD(CString pFD_OVERTIME_RD);
	void SetSENSOR_FK1_RD(CString pSENSOR_FK1_RD);
	void SetSENSOR_FK2_RD(CString pSENSOR_FK2_RD);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetRTV_PASSCV_OD(CString pRTV_PASSCV_OD);
	void SetRTV_DEST_OD(CString pRTV_DEST_OD);
	void SetLUGG_OD(CString pLUGG_OD);
	void SetSTEP(CString pSTEP);
	void SetJOB_TYP_OD(CString pJOB_TYP_OD);
	void SetDEPART_TRACK(CString pDEPART_TRACK);
	void SetARRIVE_TRACK(CString pARRIVE_TRACK);
	void SetSUSPEND(CString pSUSPEND);
	void SetOD_USER_ID(CString pOD_USER_ID); 
	void SetIS_ERROR_RD(CString pIS_ERROR_RD);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);


public:
	CString GetCid();
};



