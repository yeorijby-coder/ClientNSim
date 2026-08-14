#pragma once
#include "afx.h"
#include "Info.h"
#include "DciTrackCtrl.h"
// CCV_DATA 명령 대상입니다.

class CCV_DATA : public CInfo
{

public:
	CCV_DATA(void);
	//트랙전용
	CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrTRACK_NO);

	//설비전용 parent
	CCV_DATA(CString pstrWH_TYP, CString pstrPLC_NO);
	~CCV_DATA(void);


public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_TRACK_NO;

public:
	void SetWH_TYP(CString pWH_TYP);
	void SetPLC_NO(CString pPLC_NO);
	void SetTRACK_NO(CString pTRACK_NO);

public:
	//data
	CString V_MC_NO;
	CString V_MC_NO_NM;
	CString V_LUGG_NO_RD;
	CString V_LUGG_NO_OD;
	CString V_DEST_POS_RD;
	CString V_DEST_POS_OD;
	CString V_JOB_TYP_RD;
	CString V_JOB_TYP_OD;
	CString V_TR_PAUSE_RD;
	CString V_TR_PAUSE_OD;
	CString V_COMMING_RD;
	CString V_COMMING_OD;
	CString V_ERROR_CODE;
	CString V_AUTO_MODE_RD;
	CString V_STO_READY_RD;
	CString V_RET_READY_RD;
	CString V_STOHS_READY_RD;
	CString V_RETHS_READY_RD;
	CString V_RTV_DEPARTHS_READY_RD;
	CString V_RTV_ARRIVEHS_READY_RD;
	CString V_SENSOR0_DATA_RD;
	CString V_SENSOR1_DATA_RD;
	CString V_SENSOR2_DATA_RD;
	CString V_DELETE_TRACK_RD;
	CString V_RTV_LOCK_SENSOR;
	CString V_BARCODE;
	CString V_JMBARCODE;
	CString V_ERR_RQ_RD;
	CString V_ERR_RQ_OD;
	CString V_OD_RQ_YN;
	CString V_OD_RQ_FLAG;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_HOST_SEND_YN;
	CString V_HOST_ERR_SEND_YN;
	CString V_COMMING_CHECK_TR1;
	CString V_COMMING_CHECK_TR2;
	CString V_COMMING_CHECK_TR3;
	CString V_COMMING_CHECK_TR4;
	CString V_COMMING_DEST_TR;
	CString V_STN_KIND;
	CString V_USE_YN;
	CString V_OD_USER_ID;
	CTime V_READ_UPD_DT;
	CTime V_WRITE_UPD_DT;
	CTime V_OD_UPD_DT;
	
	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;

	CString V_DOOR_STATUS_RD;
	CString V_DEVERTER_HS_DOWN_RD;
	CString V_DEVERTER_HS_UP_RD;
	CString V_RGV_STA_LOAD_RD;
	CString V_SC_PA_PK_POSSIBLE_RD;
	CString V_DOOR_OPEN_REQ_RD;
	CString V_DOOR_CLOSE_REQ_RD;
	CString V_MTR1_RUN_RD;
	CString V_MTR2_RUN_RD;
	CString V_SZ_CHK_LOW_RD;
	CString V_SZ_CHK_HIGH_RD;
	CString V_SRT_READY_STA_RD;
	CString V_PICK4_PASS_RD;



public:
	CMap<CString, LPCTSTR, CCV_DATA*, CCV_DATA*> m_MapTracks;
	CDciTrackCtrl* m_pTrackCtrl;

public:
	BOOL m_bModified;
	BOOL IsRejectTrack() { return V_COMMING_DEST_TR == "0"; }
	BOOL IsNotInvokeTrack() { return V_STN_KIND == "0"; }

	// Door가 1개밖에 없으니까 이렇게 쓰는 거임~ 2개 이상일때는 비트로 받아와야함!
	BOOL IsDoorOpen() { return V_DOOR_STATUS_RD == "1"; }
	BOOL IsDoorClose() { return V_DOOR_STATUS_RD == "0"; }

	
public:
	void SetLUGG_NO_RD(CString pLUGG_NO_RD);
	void SetLUGG_NO_OD(CString pLUGG_NO_OD);
	void SetDEST_POS_RD(CString pDEST_POS_RD);
	void SetDEST_POS_OD(CString pDEST_POS_OD);
	void SetJOB_TYP_RD(CString pJOB_TYP_RD);
	void SetJOB_TYP_OD(CString pJOB_TYP_OD);
	void SetTR_PAUSE_RD(CString pTR_PAUSE_RD);
	void SetTR_PAUSE_OD(CString pTR_PASUE_OD);
	void SetCOMMING_RD(CString V_COMMING_RD);
	void SetCOMMING_OD(CString V_COMMING_OD);
	void SetERR_RQ_RD(CString pERR_RQ_RD);
	void SetERR_RQ_OD(CString pERR_RQ_OD);
	void SetERROR_CODE(CString pERROR_CODE);
	void SetAUTO_MODE_RD(CString pAUTO_MODE_RD);
	void SetSTO_READY_RD(CString pSTO_READY_RD);
	void SetRET_READY_RD(CString pRET_READY_RD);
	void SetSTOHS_READY_RD(CString pSTOHS_READY_RD);
	void SetRETHS_READY_RD(CString pRETHS_READY_RD);
	void SetRTV_DEPARTHS_READY_RD(CString pRTV_DEPARTHS_READY_RD);
	void SetRTV_ARRIVEHS_READY_RD(CString pRTV_ARRIVEHS_READY_RD);
	void SetSENSOR0_DATA_RD(CString pSENSOR0_DATA_RD);
	void SetSENSOR1_DATA_RD(CString pSENSOR1_DATA_RD);
	void SetSENSOR2_DATA_RD(CString pSENSOR2_DATA_RD);
	void SetOD_RQ_YN(CString pOD_RQ_YN);
	void SetREAD_UPD_DT(CTime pREAD_UPD_DT);
	void SetWRITE_UPD_DT(CTime pWRITE_UPD_DT);
	void SetOD_USER_ID(CString pOD_USER_ID);
	void SetOD_UPD_DT(CTime pOD_UPD_DT);
	void SetUSE_YN(CString pUES_YN);
	void SetOD_RQ_FLAG(CString pOD_RQ_FLAG);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);

	void SetMC_NO(CString pMC_NO);
	void SetMC_NO_NM(CString pMC_NO_NM);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);

	void SetDELETE_TRACK_RD(CString V_DELETE_TRACK_RD);
	void SetRTV_LOCK_SENSOR(CString V_RTV_LOCK_SENSOR);
	void SetBARCODE(CString V_BARCODE);
	void SetJMBARCODE(CString V_JMBARCODE);
	void SetHOST_SEND_YN(CString V_HOST_SEND_YN);
	void SetHOST_ERR_SEND_YN(CString V_HOST_ERR_SEND_YN);
	void SetCOMMING_CHECK_TR1(CString V_COMMING_CHECK_TR1);
	void SetCOMMING_CHECK_TR2(CString V_COMMING_CHECK_TR2);
	void SetCOMMING_CHECK_TR3(CString V_COMMING_CHECK_TR3);
	void SetCOMMING_CHECK_TR4(CString V_COMMING_CHECK_TR4);
	void SetCOMMING_DEST_TR(CString V_COMMING_DEST_TR);
	void SetSTN_KIND(CString V_STN_KIND);
	void SetDOOR_STATUS_RD(CString pDOOR_STATUS_RD, WORD& wDoorStatus);

public:
	virtual CString GetMapKeyCV();
	virtual CString GetMapKeyTRACK();
};


