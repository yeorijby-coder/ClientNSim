#pragma once
#include "afx.h"
#include "CV_DATA.h"
#include "DciButtonCtrl.h"

// BCR_MST 명령 대상입니다.

class CBCR_MST : public CObject
{
public:
	CBCR_MST(void);
	CBCR_MST(CString pstrWH_TYP, CString strBCR_MC_NO, CString pstrBCR_NO);

	~CBCR_MST(void);

public:
	//key
	CString K_WH_TYP;
	CString K_BCR_NO;

public:
	CString m_strBCR_MC_NO;

public:
	void SetBCR_NO(CString pBCR_NO);
	void SetWH_TYP(CString K_WH_TYP);

public:
	CString V_BCR_NO;
	CString V_BCR_MC_NO;
	CString V_BCR_STA;
	CString V_BCR_RCV_VAL[7];
	CString V_UPD_DT[7];
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_USE_YN;
	CString V_UPD_USER_ID;
	CString V_SUSPEND;
	CString V_CHK_BYPASS_YN;
	CString V_OD_RQ_ID;

	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;

public:
	BOOL m_bModified;
	BOOL m_bListAdd;

public:
	CDciButtonCtrl* m_pControl;

public:
	void SetBCR_MC_NO(CString pBCR_MC_NO);
	void SetBCR_STA(CString pBCR_STA);
	void SetBCR_RCV_VAL(CString pBCR_RCV_VAL);
	void SetCMD_RQ_ID(CString pCMD_RQ_ID);
	void SetCMD_RQ_YN(CString pCMD_RQ_YN);
	void SetUSE_YN(CString pUSE_YN);
	void SetUPD_USER_ID(CString pUPD_USER_ID);
	void SetSUSPEND(CString pSUSPEND);
	void SetCHK_BYPASS_YN(CString pCHK_BYPASS_YN);
	void SetOD_RQ_ID(CString pOD_RQ_ID);
	void SetUPD_DT(CString pUPD_DT);

	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);

public:
	CString GetCid();
	CString GetCid(CString pnChar4);
};


