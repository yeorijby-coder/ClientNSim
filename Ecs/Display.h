#pragma once

#include "Equipment.h"
#include "DisplayInfo.h"






class CDisplay : public CEquipment
{
	DECLARE_DYNAMIC(CDisplay)

public:
	CDisplay(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex);
	virtual ~CDisplay();

public:

	CEcsDoc* m_pDoc;
	CDisplayInfo* m_pInfo;
	virtual CInfo* GetInfo() { return m_pInfo; }	// 주석이면 기반 클래스의 NULL이 반환되어
														// CEquipmentArray::InvokeControl 에서 ASSERT + NULL 역참조가 난다

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();

	bool ReadStatusDB();
	//	void InvokeTrayPickerUpNPass(CTrackInfo* pTrack, CStationInfo* pStation);
public:
	CString m_strInPlc;
	CString GetSelectQry();
	void SetVar(CRecordSetWrap* pRsw);
	//BOOL GetModified();

};

            