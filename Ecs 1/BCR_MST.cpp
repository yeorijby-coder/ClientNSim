
#include "stdafx.h"
#include "BCR_MST.h"

CBCR_MST::CBCR_MST(void)
{
	m_pControl = NULL;
}

CBCR_MST::CBCR_MST(CString pstrWH_TYP, CString pstrBCR_MC_NO, CString pstrBCR_NO)
{
	m_pControl = NULL;
	K_WH_TYP = pstrWH_TYP;
	K_BCR_NO = pstrBCR_NO;
	m_strBCR_MC_NO = pstrBCR_MC_NO;
	m_bListAdd = false;
}

CBCR_MST::~CBCR_MST(void)
{
}

void CBCR_MST::SetWH_TYP(CString pstrWH_TYP)
{
	K_WH_TYP = pstrWH_TYP;
}

void CBCR_MST::SetBCR_NO(CString pstrBCR_NO)
{
	if (V_BCR_NO == pstrBCR_NO)
		return;

	V_BCR_NO = pstrBCR_NO;
	m_bModified = true;
}

void CBCR_MST::SetBCR_MC_NO(CString pBCR_MC_NO)
{
	if(V_BCR_MC_NO == pBCR_MC_NO)
		return;

	V_BCR_MC_NO = pBCR_MC_NO;
	m_bModified = true;
}

void CBCR_MST::SetBCR_STA(CString pBCR_STA)
{
	if (V_BCR_STA == pBCR_STA)
		return;

	V_BCR_STA = pBCR_STA;
	m_bModified = true;
}

void CBCR_MST::SetBCR_RCV_VAL(CString pBCR_RCV_VAL)
{
	if (V_BCR_RCV_VAL[0] == pBCR_RCV_VAL)
		return;

	V_BCR_RCV_VAL[0] = pBCR_RCV_VAL;
	m_bListAdd = true;
	m_bModified = true;
}

void CBCR_MST::SetCMD_RQ_ID(CString pCMD_RQ_ID)
{
	if (V_CMD_RQ_ID == pCMD_RQ_ID)
		return;

	V_CMD_RQ_ID = pCMD_RQ_ID;
	m_bModified = true;
}

void CBCR_MST::SetCMD_RQ_YN(CString pCMD_RQ_YN)
{
	if (V_CMD_RQ_YN == pCMD_RQ_YN)
		return;

	V_CMD_RQ_YN = pCMD_RQ_YN;
	m_bModified = true;
}

void CBCR_MST::SetUSE_YN(CString pUSE_YN)
{
	if (V_USE_YN == pUSE_YN)
		return;

	V_USE_YN = pUSE_YN;
	m_bModified = true;
}

void CBCR_MST::SetUPD_USER_ID(CString pUPD_USER_ID)
{
	if (V_UPD_USER_ID == pUPD_USER_ID)
		return;

	V_UPD_USER_ID = pUPD_USER_ID;
	m_bModified = true;
}

void CBCR_MST::SetSUSPEND(CString pSUSPEND)
{
	if (V_SUSPEND == pSUSPEND)
		return;

	V_SUSPEND = pSUSPEND;
	m_bModified = true;
}

void CBCR_MST::SetCHK_BYPASS_YN(CString pCHK_BYPASS_YN)
{
	if (V_CHK_BYPASS_YN == pCHK_BYPASS_YN)
		return;

	V_CHK_BYPASS_YN = pCHK_BYPASS_YN;
	m_bModified = true;
}

void CBCR_MST::SetOD_RQ_ID(CString pOD_RQ_ID)
{
	if (V_OD_RQ_ID == pOD_RQ_ID)
		return;

	V_OD_RQ_ID = pOD_RQ_ID;
	m_bModified = true;
}

void CBCR_MST::SetUPD_DT(CString pUPD_DT)
{
	if (V_UPD_DT[0] == pUPD_DT)
		return;

	V_UPD_DT[0] = pUPD_DT;
	m_bListAdd = true;
	m_bModified = true;
}

void CBCR_MST::SetEQP_TIME(CString pstrEQP_TIME)
{
	if (V_EQP_TIME == pstrEQP_TIME)
		return;

	V_EQP_TIME = pstrEQP_TIME;
	m_bModified = true;
}

void CBCR_MST::SetEQP_COLOR(CString pstrEQP_COLOR)
{
	if (V_EQP_COLOR == pstrEQP_COLOR)
		return;

	V_EQP_COLOR = pstrEQP_COLOR;
	m_bModified = true;
}

void CBCR_MST::SetEQP_CONNECTED_YN(CString pstrEQP_CONNECTED_YN)
{
	if (V_EQP_CONNECTED_YN == pstrEQP_CONNECTED_YN)
		return;

	V_EQP_CONNECTED_YN = pstrEQP_CONNECTED_YN;
	m_bModified = true;
}


CString CBCR_MST::GetCid()
{
	CString strCID;
	strCID.Format(_T("111707%02s"), K_BCR_NO);
	return strCID;
}

CString CBCR_MST::GetCid(CString nChar4)
{
	CString strCID;
	strCID.Format(_T("111707%02s"), nChar4, K_BCR_NO);
	return strCID;
}