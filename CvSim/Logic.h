// Logic.h: interface for the CKeyWord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGIC_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
#define AFX_LOGIC_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDom.h"
#include "TrackInfo.h"
#include "Dictionary.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum EN_CONDITION_TYPE {
	BT_SIGN_BIT_OFF = 0,	// 
	BT_SIGN_BIT_ON = 1,	// 
	BT_SIGN_NOT_ZERO = 2,	// 
	BT_SIGN_IS_ZERO = 3,	// 
	BT_SIGN_NO = 4,	// 
	BT_SIGN_EXIST = 5,	// 
	BT_SIGN_IS_STATION = 6,	// 
	BT_SIGN_LESS = 7,	// <
	BT_SIGN_GREATER = 8,	// >
	BT_SIGN_LTOE = 9,	// <=(Less Then Or Equal)
	BT_SIGN_GTOE = 10,	// >=(Greater Then Or Equal)
	BT_SIGN_EQUAL_SELF = 11,	// 
	BT_SIGN_NOT_EQUAL_SELF = 12,	// 
	BT_SIGN_IS_SC_DEST = 13,	// 
	BT_SIGN_EXCEPT = 14,	// 
	BT_SIGN_TIME_ELAPSE = 15,	// 
	BT_SIGN_NOT_SC_DEST = 16,	// 
	BT_SIGN_IS_MAGAZINE = 17,	// 이 트랙이 매거진이다 (작업정보 창에서 지정)
	BT_SIGN_MG_FULL = 18,	// 매거진이 만재다
	BT_SIGN_MG_NOT_FULL = 19,	// 매거진이 아직 만재가 아니다
	BT_SIGN_SF_OFF = 20,	// StartFlag 가 꺼져 있다 (진행 중인 순서가 없다)
	BT_SIGN_SF_ON = 21,	// StartFlag 가 켜져 있다
	BT_SIGN_IS_FORKLIFT = 22,	// EcsDefine.xml 에 <ForkLift/> 가 붙은 트랙이다
	BT_SIGN_TWIN_RET_OK = 23	// 트윈 H/S 가 출고대를 올려도 되는 때인가
};
enum EN_ACTION_TYPE {
	BT_ACTION_BIT_ON	= 51,		//
	BT_ACTION_BIT_OFF	= 52,		// 특정비트만 남기고 나머지는 다 OFF
	BT_ACTION_INVERSE	= 53,		// 
	BT_ACTION_SF_ON		= 54,		// Start Flag
	BT_ACTION_SF_OFF	= 55,		// Start Flag
	BT_ACTION_TIME_RESET= 56,		// 
	BT_ACTION_WORD_RESET= 57,		// 
	BT_ACTION_BIT_F_OFF = 58,		// 특정 비트만 강제로 끄기
	BT_ACTION_MG_COUNT_UP = 59,		// 매거진에 한 장 쌓는다 ('만재' 체크면 바로 만재)
	BT_ACTION_MG_COUNT_CLEAR = 60	// 매거진 장수를 0 으로 되돌린다
};


class CBehavior
{
public:
	CBehavior(void) {}
	virtual ~CBehavior(void) {}

public:
	int					m_nType;
	CString				m_strKeyWord;

public:
	virtual void SetProperty(int enType, LPCTSTR lpszKeyWord = NULL) = 0;
};

class CCondition : public CBehavior
{
public:
	CCondition(void) {}
	virtual ~CCondition(void) {}

public:
	EN_CONDITION_TYPE m_enType;

public:
	void SetProperty(int enType, LPCTSTR lpszKeyWord = NULL)
	{
		m_enType = (EN_CONDITION_TYPE)enType;
		m_nType = enType;
		m_strKeyWord.Format(_T("%s"), lpszKeyWord);
	}
};
class CConditionArray : public CArray<CCondition, CCondition>
{
public:
	CConditionArray() {}
	virtual ~CConditionArray() {}
public:
	CString m_strDesc;
};

class CAction : public CBehavior
{
public:
	CAction(void) {}
	virtual ~CAction(void) {}

public:
	EN_ACTION_TYPE m_enType;

public:
	void SetProperty(int enType, LPCTSTR lpszKeyWord = NULL)
	{
		m_enType = (EN_ACTION_TYPE)enType;
		m_nType = enType;
		m_strKeyWord.Format(_T("%s"), lpszKeyWord);
	}
};
class CActionArray : public CArray<CAction, CAction>
{
public:
	CActionArray() {}
	virtual ~CActionArray() {}
public:
	CString m_strDesc;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LogicGroup => Logic => Condition, Action => Num, KeyWord
class CLogic
{
public:
	CLogic() {}
	virtual ~CLogic() {}
	//{
	//	int i = 0;
	//	for (i = 0; i < m_pConditions.GetSize(); ++i)
	//		delete m_pConditions[i];
	//	m_pConditions.RemoveAll();
	//}

public:
//	int m_nSize;		// Logic 갯수 
//	
	CString m_strDesc;
	int m_nId;

public:
	CConditionArray  m_arrConditions;
	CActionArray  m_arrActions;

	//CConditionArray* m_pConditions;
	//CActionArray* m_pActions;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CLogicArray : public CArray<CLogic, CLogic>
{
public:
	CLogicArray() {}
	virtual ~CLogicArray() {}
public:
	CString m_strDesc;
	CString m_strName;
};


class CEcsDoc;
class CEquipment;
class CLogicSystem : public CXmlDom  
{
public:
	CLogicSystem(CEcsDoc* pDoc);
	virtual ~CLogicSystem();

protected:
	CEcsDoc* m_pDoc;

public:
	EN_COMM_METHOD m_enCommMethod;
	EN_CPU_KIND m_enCpuKind;
	EN_ADDRESS_AREA m_enAddressArea;
	BOOL m_bUseDestTable;
	BOOL m_bUseScDest;
	enum { enDefaultPropSize = 4 };	// type, addr, name, InOrder  <= 추후 1개가 Append 됨

public:
	BOOL ParseXml();
};

#endif // !defined(AFX_LOGIC_H__AF775845_842E_4611_8389_C2F449BCF36F__INCLUDED_)
