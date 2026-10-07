
#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"

// @.자원 계수기.
//   메모리가 어디서 새는지 추측으로 고치다 더 나빠지는 일을 되풀이하지 않으려고
//   실제로 몇 개가 살아 있는지 센다. 60초마다 MemCount.log 에 적는다.
//
//   읽는 법
//     wrapLive 가 계속 늘면 : CRecordSetWrap 을 아무도 안 지우는 것이다.
//     rsLive   가 계속 늘면 : ADO 레코드셋이 안 닫히는 것이다.
//     둘 다 그대로인데 메모리만 늘면 : ADO 커서 엔진 안쪽이다.

struct CResCount
{
	static volatile LONG	s_nWrapMade;	// CRecordSetWrap 만든 수
	static volatile LONG	s_nWrapGone;	// 지운 수
	static volatile LONG	s_nRsOpen;		// 레코드셋 연 수
	static volatile LONG	s_nRsClone;	// Clone 으로 만든 수
	static volatile LONG	s_nRsClose;	// 닫은 수
	static volatile LONG	s_nDbMade;		// CAdoDB 만든 수
	static volatile LONG	s_nDbGone;		// 지운 수
	static volatile LONG	s_nConnOpen;	// 접속 연 수
	static volatile LONG	s_nConnClose;	// 접속 닫은 수

	// @.종류별로도 센다. 어느 설비가 받아가고도 안 지우는지 보려는 것이다.
	//   자리 : 0=CV 1=SC 2=RTV 3=BCR 4=DISPLAY 5=그 밖
	static volatile LONG	s_nMadeKind[6];
	static volatile LONG	s_nGoneKind[6];
	static int KindSlot(int nKind);

	// @.만든 자리를 주소로 남긴다.
	//   종류별로는 수가 맞는데 전체는 안 맞았다. 수집 말고 다른 데서
	//   만들어 놓고 안 지우는 자리가 있다는 뜻이다. 그 자리를 찾는다.
	//   주소는 Ecs.map 으로 함수 이름을 찾을 수 있다.
	enum { MAX_CALLER = 64 };
	static void*		s_caller[MAX_CALLER];
	static volatile LONG	s_callerLive[MAX_CALLER];
	static volatile LONG	s_callerMade[MAX_CALLER];
	static int Slot(void* pv);

	static void WriteLine();	// MemCount.log 에 한 줄 적는다
};


class CRecordSetWrap
{
public:
	CRecordSetWrap(_RecordsetPtr pRecordSet);
	int		m_nCallerSlot;
	~CRecordSetWrap();

public:
	_RecordsetPtr m_pRecordSet;

public:
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveFirst();
	CString GetItem(CString strFiledName);
	CString GetColumn(CString strFiledName);

private:
	// @.컬럼 이름 -> 자리번호 대응표.
	//   GetItem 은 설비마다, 행마다, 컬럼마다 불린다. 예전에는 그때마다
	//   이름을 BSTR 로 만들어 ADO 에 넘겼고, ADO 는 Fields 를 처음부터
	//   훑어 이름을 맞춰 보았다. 자리번호는 한 번만 잡아 두면 되는 것이다.
	CMapStringToPtr	m_mapFieldIdx;
	BOOL			m_bFieldIndexed;
	void			BuildFieldIndex();
	
};

