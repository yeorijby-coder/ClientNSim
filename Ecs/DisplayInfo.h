#pragma once

#include "info.h"
#include "DciStaticCtrl.h"
#include "DisplayData.h"
#include "TrackInfo.h"

class CDisplayInfo : public CInfo
{
	DECLARE_SERIAL(CDisplayInfo)

public:
	CDisplayInfo() {}
	CDisplayInfo(CEquipment* pEquipment);
	virtual ~CDisplayInfo();

public:
	enum EN_ERROR_CODE {
		enErrorNone,
		enErrorLength,
		enErrorNoSTX,
		enErrorNoETX,
		enErrorData,
		enErrorNoRead,
		enErrorInvalidJob,
		enErrorMissmatchIDPID,
		enErrorCommInterface,
		enErrorCommInterface1,
		enErrorCommInterface2,
		enErrorCommInterface3,
		enErrorCommInterface4,
		enErrorCommInterface5,
		enErrorCommunication
	};
	enum EN_STATION_KIND { enStationKindNo, enStationKindSto, enStationKindRet, enStationKindArv, enStationKindSize };
	enum EN_BCR_KIND { enBcrKindNo, enBcrKindSingle, enBcrKindTwin, enBcrKindTriple, enBcrKindSize };
	enum EN_BCR_STATION { enBcrMagazine, enBcrStoStation, enBcrArvStation, enBcrStationSize };

public:
	CDciStaticCtrl* m_pControl;

public:
	CMap<CString, LPCTSTR, CDisplayData*, CDisplayData*> m_MapDisplayData;
	CDisplayData* m_pDisplayData;

public:
	//virtual void Serialize(CArchive& ar);

public:
	void InvokeControl(CDisplayData* pDisPlayData);
	virtual void InvokeControl();


public:
	CDisplayData* CreateDisPlayData(CString pstrEQP_NO);
	CString GetStringPLC_NO();
	CString GetStringPLC_NO(int pnPLC_NO);
	CString GetStringDISP_NO(CString pstrDISP_NO);


public:
	//int GetDisplayBoardID(int nTrackNum);
	//CDisplayBoard* GetDisplayBoard(int nID);
	void SetDisplayInfo(int nID, CString& strDisplay);
	void SetErrorCode(int nErrorCode);
	CString GetErrorString();
	CString GetXmlString();
	COLORREF GetColor();
};

class CDisplayBoard : public CObject
{
public:
	CDisplayBoard(int nID) :m_nID(nID) {}
	virtual ~CDisplayBoard() {}

public:
	int	m_nID;
	CString m_strDisplay;
	CTrackInfoArray m_pTracks;
};

class CDisplayBoardArray : public CArray<CDisplayBoard*, CDisplayBoard*>
{
public:
	CDisplayBoardArray() {}
	virtual ~CDisplayBoardArray() {}
};

