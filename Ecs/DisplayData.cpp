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
	strCID.Format(_T("17180%03s"), K_DISP_NO);
	return strCID;
}
