#include "stdafx.h"
#include "DisplayData.h"

CDisplayData::CDisplayData(void)
{
	m_pControl = NULL;
}

CDisplayData::CDisplayData(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrDISP_NO)
{
	m_pControl = NULL;

	K_WH_TYP = pstrWH_TYP;
	K_PLC_NO = pstrPLC_NO;
	K_DISP_NO = pstrDISP_NO;
}


CDisplayData::~CDisplayData(void)
{
}


void CDisplayData::SetDISP_DATA(CString pDISP_DATA)
{
	if (V_DISP_DATA == pDISP_DATA)
		return;

	V_DISP_DATA = pDISP_DATA;
	m_bModified = true;
}

CString CDisplayData::GetCid()
{
	CString strCID;
	strCID.Format(_T("171802%02d"), _ttoi(K_DISP_NO));	// %02s는 공백 패딩("171802 1")이라 레이아웃 id와 매칭되지 않음
	return strCID;
}

CString CDisplayData::GetCid(CString nChar4)
{
	CString strCID;
	strCID.Format(_T("171802%02s"), nChar4, K_DISP_NO);
	return strCID;
}