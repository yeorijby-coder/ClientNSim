#pragma once

#include "afx.h"
#include "Info.h"
#include "DciStaticCtrl.h"

class CDisplayData
{

public:
	CDisplayData(void);
	CDisplayData(CString pstrWH_TYP, CString pstrPLC_NO, CString pstrDISP_NO);

	~CDisplayData(void);

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_DISP_NO;

public:
	void SetWH_TYP(CString pWH_TYP);
	void SetPLC_NO(CString pPLC_NO);
	void SetDISP_NO(CString pDISP_NO);

public:
	//data
	CString V_DISP_DATA;

public:
	void SetDISP_DATA(CString pDISP_DATA);

public:
	CDciStaticCtrl* m_pControl;

public:
	BOOL m_bModified;


public:
	CString GetCid();

};

