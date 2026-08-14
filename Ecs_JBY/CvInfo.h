// CvInfo.h: interface for the CCvInfo class.
//

#pragma once

#include "Info.h"
#include "TrackInfo.h"

class CEquipment;


class CCvInfo : public CInfo  
{
	DECLARE_SERIAL(CCvInfo)

public:
	CCvInfo() {}
	CCvInfo(CEquipment* pEquipment);
	virtual ~CCvInfo();

public:
	class CDciStaticCtrlArray : public CArray<CDciStaticCtrl*, CDciStaticCtrl*> {};
	CDciStaticCtrlArray m_pDoorCtrls;

public:
	CTrackInfoArray m_pTracks;


public:
	CMap<CString, LPCTSTR, CTrackInfo*, CTrackInfo*> m_MapTrackInfo;

public:
	virtual void InvokeControl();
	virtual void Serialize(CArchive& ar);
	virtual void Initialize() { m_bModified = TRUE; m_pTracks.Initialize(); }
};

