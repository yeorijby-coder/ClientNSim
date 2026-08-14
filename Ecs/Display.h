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
	//virtual CInfo* GetInfo() { return m_pInfo; }

protected:
	virtual void AutoRunProc();
	virtual void CommandProc();

	bool ReadStatusDB();
	//	void InvokeTrayPickerUpNPass(CTrackInfo* pTrack, CStationInfo* pStation);
public:
	CString m_strInPlc;
	CString GetSelectQry();
	//void SetVar(CRecordSetWrap* pRsw);
	//BOOL GetModified();

};

            