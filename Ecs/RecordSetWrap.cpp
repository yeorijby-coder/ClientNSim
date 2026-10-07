#include "StdAfx.h"
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#include "RecordSetWrap.h"

volatile LONG CResCount::s_nWrapMade  = 0;
volatile LONG CResCount::s_nWrapGone  = 0;
volatile LONG CResCount::s_nRsOpen    = 0;
volatile LONG CResCount::s_nRsClone   = 0;
volatile LONG CResCount::s_nRsClose   = 0;
volatile LONG CResCount::s_nDbMade    = 0;
volatile LONG CResCount::s_nDbGone    = 0;
volatile LONG CResCount::s_nConnOpen  = 0;
volatile LONG CResCount::s_nConnClose = 0;
volatile LONG CResCount::s_nMadeKind[6] = { 0, 0, 0, 0, 0, 0 };
volatile LONG CResCount::s_nGoneKind[6] = { 0, 0, 0, 0, 0, 0 };

void*         CResCount::s_caller[CResCount::MAX_CALLER]     = { 0 };
volatile LONG CResCount::s_callerLive[CResCount::MAX_CALLER] = { 0 };
volatile LONG CResCount::s_callerMade[CResCount::MAX_CALLER] = { 0 };
static CRITICAL_SECTION g_csCaller;
static bool             g_bCsReady = false;

// @.주소 하나에 자리 하나를 준다. 자리가 모자라면 마지막 자리에 몰아 센다.
int CResCount::Slot(void* pv)
{
	if (g_bCsReady == false)
	{
		::InitializeCriticalSection(&g_csCaller);
		g_bCsReady = true;
	}

	::EnterCriticalSection(&g_csCaller);

	int nFound = MAX_CALLER - 1;
	for (int ii = 0; ii < MAX_CALLER - 1; ii++)
	{
		if (s_caller[ii] == pv)
		{
			nFound = ii;
			break;
		}
		if (s_caller[ii] == NULL)
		{
			s_caller[ii] = pv;
			nFound = ii;
			break;
		}
	}

	::LeaveCriticalSection(&g_csCaller);
	return nFound;
}

// @.설비 종류를 자리번호로 바꾼다. (Equipment.h 의 EN_KIND)
int CResCount::KindSlot(int nKind)
{
	switch (nKind)
	{
	case 10:
	case 11:	return 0;	// CV
	case 20:	return 1;	// SC
	case 30:	return 2;	// RTV
	case 70:	return 3;	// BCR
	case 80:	return 4;	// DISPLAY
	}
	return 5;
}

///////////////////////////////////////////////
// @.지금까지의 수를 한 줄로 적는다. exe 가 있는 폴더의 MemCount.log 다.
//   살아 있는 수(live)는 만든 수에서 지운 수를 뺀 것이다.

void CResCount::WriteLine()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	CString strPath = szPath;
	int nSlash = strPath.ReverseFind(_T('\\'));
	if (nSlash > 0)
		strPath = strPath.Left(nSlash + 1);
	strPath += _T("MemCount.log");

	LONG nWrapLive = s_nWrapMade - s_nWrapGone;
	LONG nRsLive   = s_nRsOpen + s_nRsClone - s_nRsClose;
	LONG nDbLive   = s_nDbMade - s_nDbGone;
	LONG nConnLive = s_nConnOpen - s_nConnClose;

	CString strLine;
	strLine.Format(_T("%s  wrapLive=%ld (made %ld / gone %ld)  rsLive=%ld (open %ld / clone %ld / close %ld)  dbLive=%ld  connLive=%ld\r\n"),
		(LPCTSTR)COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),
		nWrapLive, s_nWrapMade, s_nWrapGone,
		nRsLive, s_nRsOpen, s_nRsClone, s_nRsClose,
		nDbLive, nConnLive);

	CString strKind;
	for (int ii = 0; ii < 6; ii++)
	{
		CString strOne;
		strOne.Format(_T("  [%d] made %ld gone %ld live %ld"),
			ii, s_nMadeKind[ii], s_nGoneKind[ii], s_nMadeKind[ii] - s_nGoneKind[ii]);
		strKind += strOne;
	}
	CString strCaller;
	for (int jj = 0; jj < MAX_CALLER; jj++)
	{
		if (s_callerLive[jj] <= 0)
			continue;

		CString strOne2;
		strOne2.Format(_T("  <0x%08X live %ld / made %ld>"),
			(DWORD)(DWORD_PTR)s_caller[jj], s_callerLive[jj], s_callerMade[jj]);
		strCaller += strOne2;
	}

	strLine = strLine.Left(strLine.GetLength() - 2) + strKind + strCaller + _T("\r\n");

	CStdioFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText) == FALSE)
		return;

	try
	{
		file.SeekToEnd();
		file.WriteString(strLine);
	}
	catch (CFileException* e)
	{
		e->Delete();
	}

	file.Close();
}


CRecordSetWrap::CRecordSetWrap(_RecordsetPtr precordSet)
{
	m_pRecordSet = precordSet;
	m_bFieldIndexed = FALSE;
	::InterlockedIncrement(&CResCount::s_nWrapMade);

	// @.나를 부른 자리의 주소를 적어 둔다.
	m_nCallerSlot = CResCount::Slot(_ReturnAddress());
	::InterlockedIncrement(&CResCount::s_callerMade[m_nCallerSlot]);
	::InterlockedIncrement(&CResCount::s_callerLive[m_nCallerSlot]);
}



CRecordSetWrap::~CRecordSetWrap(void)
{
	::InterlockedIncrement(&CResCount::s_nWrapGone);
	if (m_nCallerSlot >= 0 && m_nCallerSlot < CResCount::MAX_CALLER)
		::InterlockedDecrement(&CResCount::s_callerLive[m_nCallerSlot]);

	if(m_pRecordSet != NULL)
	{
		m_pRecordSet->Close();
		// @.닫은 수에 넣어야 로그의 rsLive 가 맞는다. 여기서 닫는 것을 안 세어
		//   살아 있는 것처럼 보였다.
		::InterlockedIncrement(&CResCount::s_nRsClose);
	}
	m_pRecordSet = NULL;
}

BOOL CRecordSetWrap::MoveNext()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MoveNext();

	return TRUE;
}


BOOL CRecordSetWrap::MovePrevious()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MovePrevious();

	return TRUE;
}


BOOL CRecordSetWrap::MoveFirst()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MoveFirst();

	return TRUE;
}

///////////////////////////////////////////////
// @.조회 결과의 컬럼 자리를 한 번만 잡아 둔다.
//   컬럼 구성은 레코드셋이 열려 있는 동안 바뀌지 않으므로 한 번이면 된다.
//   이름은 대문자로 맞춰 둔다. ADO 는 대소문자를 가리지 않았으나
//   CMapStringToPtr 은 가리기 때문이다.
//   자리번호는 0 도 쓰므로 1 을 더해 담는다. (NULL 이 '없음' 이 되게)

void CRecordSetWrap::BuildFieldIndex()
{
	m_bFieldIndexed = TRUE;

	if (m_pRecordSet == NULL)
		return;

	try
	{
		long nCount = m_pRecordSet->Fields->GetCount();

		for (long ii = 0; ii < nCount; ii++)
		{
			CString strName = (LPCTSTR)m_pRecordSet->Fields->GetItem(_variant_t(ii))->GetName();
			strName.MakeUpper();

			m_mapFieldIdx.SetAt(strName, (void*)(ii + 1));
		}
	}
	catch (_com_error&)
	{
	}
	catch (...)
	{
	}
}

CString CRecordSetWrap::GetItem(CString strFiledName)
{
	if (m_pRecordSet == NULL)
	{
		return _T("");
	}

	if (m_pRecordSet->adoEOF == TRUE)
	{
		return _T("");
	}

	if (m_bFieldIndexed == FALSE)
		BuildFieldIndex();

	// @.부른 쪽이 준 이름 그대로 먼저 찾아본다.
	//   이 프로그램은 _T("LUGG_NO_RD") 처럼 대문자로 적어 부르므로 거의
	//   언제나 여기서 걸린다. 예전에는 부를 때마다 MakeUpper 로 문자열을
	//   새로 만들었는데, 한 주기에 설비마다 행마다 칸마다 부르는 길이라
	//   그 자체가 만만치 않았다. 못 찾았을 때만 대문자로 바꿔 다시 본다.
	void* pIdx = NULL;
	BOOL  bFound = m_mapFieldIdx.Lookup(strFiledName, pIdx);

	if (bFound == FALSE)
	{
		CString strKey = strFiledName;
		strKey.MakeUpper();
		bFound = m_mapFieldIdx.Lookup(strKey, pIdx);
	}

	TRY
	{
		if (bFound == TRUE)
		{
			// @.자리번호로 바로 꺼낸다. 이름을 훑지 않는다.
			long nIdx = (long)(INT_PTR)pIdx - 1;
			return m_pRecordSet->Fields->GetItem(_variant_t(nIdx))->Value;
		}

		// @.대응표에 없는 이름이면 예전처럼 이름으로 찾아 본다.
		//   별칭을 쓰는 조회에서 이름이 어긋나도 동작은 그대로 두려는 것이다.
		return m_pRecordSet->Fields->GetItem(_variant_t(strFiledName))->Value;
	}
	CATCH (COleException, exOle)
	{
		return _T("");
	}
	CATCH (CException, e)
	{
		return _T("");
	}
	END_CATCH

	return _T("");
}

