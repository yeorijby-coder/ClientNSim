
#include "StdAfx.h"
#include "stdafx.h"
#include "CollectDB.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "RecordSetWrap.h"

#define CRLF _T("\n")

extern "C" __declspec(dllimport) int  Usb_Qu_Getstate(); 

CCollectDB::CCollectDB(CEcsDoc* pDoc)
{
	m_bThreadDoWork = FALSE;
	m_pDoc = pDoc;
	m_pThread = NULL;
	m_pDB_ACCESS = NULL;
}

CCollectDB::~CCollectDB(void)
{
	if(m_pDB_ACCESS != NULL){ delete m_pDB_ACCESS; }
	m_pDB_ACCESS = NULL;

}

BOOL CCollectDB::IsDB_POSSIBLE()
{
	if(m_pDB_ACCESS == NULL || m_pDB_ACCESS->m_pAdoDB == NULL)
	{
		if(m_pDB_ACCESS != NULL)
		{
			if(m_pDB_ACCESS->m_pAdoDB == NULL)
			{
				delete m_pDB_ACCESS->m_pAdoDB;
				m_pDB_ACCESS->m_pAdoDB = NULL;
			}
			delete m_pDB_ACCESS;
			m_pDB_ACCESS = NULL;
		}

		m_pDB_ACCESS = new CURMDBAccess(m_pDoc, new CAdoDB(m_pDoc));
		return m_pDB_ACCESS->m_pAdoDB->ConnectDB();
	}

	if(m_pDB_ACCESS->m_pAdoDB->m_bConnected == FALSE)
	{
		// (예전에는 delete m_pDB_ACCESS 뒤에 m_pDB_ACCESS->m_pAdoDB = NULL 을 써서
		//  해제된 메모리에 쓰는 순서였다. DB 재접속 경로를 탈 때마다 무작위로 죽었다)
		delete m_pDB_ACCESS->m_pAdoDB;
		m_pDB_ACCESS->m_pAdoDB = NULL;
		delete m_pDB_ACCESS;
		
		m_pDB_ACCESS = NULL;
		return FALSE;
	}

	return m_pDB_ACCESS->m_pAdoDB->m_bConnected;
}

BOOL CCollectDB::StartDoWork()
{
	if(m_bThreadDoWork == TRUE)
	{
		return FALSE;
	}

	
	m_bThreadDoWork = TRUE;
	m_pThread = ::AfxBeginThread(DoWork, (LPVOID)this);
	if(m_pThread == NULL)
	{
		m_bThreadDoWork = FALSE;
		return FALSE;
	}

	return IsAllive();
}

BOOL CCollectDB::IsAllive()
{
	return m_bThreadDoWork;
}

BOOL CCollectDB::StopDoWork()
{
	m_bThreadDoWork = FALSE;
	::WaitForSingleObject(m_pThread, INFINITE);
	return TRUE;
}

UINT CCollectDB::DoWork(LPVOID pParm)
{
	CEquipment *pEquipment = NULL;
	CCollectDB* pThis = (CCollectDB*)pParm;
	while(pThis->m_bThreadDoWork)
	{			
		CEcsDoc* pDoc = pThis->m_pDoc;
		if(pDoc == NULL)
		{
			::Sleep(500);
			break;
		}
		// @.예전에는 설비마다 이것을 불렀다. 설비가 마흔 가까이 되니 한 바퀴에
		//   마흔 번이었다. 한 바퀴에 한 번만 본다.
		if(pThis->IsDB_POSSIBLE() == FALSE)
		{
			::Sleep(500);
			continue;
		}

		int nEqpCount = pDoc->m_pEquipments.GetCount();
		for(int nIdxEqp = 0; nIdxEqp < nEqpCount; nIdxEqp++)
		{
			if(pDoc->m_bExit == true)
			{
				pThis->StopDoWork();
				break;
			}

			// @.DB 가 쓸 만한지는 한 바퀴에 한 번만 본다. (아래 for 앞으로 옮겼다)
			pEquipment = pDoc->m_pEquipments[nIdxEqp];

			//설비들 값
			if(pEquipment == NULL || pEquipment->m_pRsw == NULL)
			{
				pThis->Collect_EQUIPMENT(pEquipment);
				::Sleep(50); //Aß°¡
			}
		}	

		//if(pDoc->m_blConnectStatus == TRUE)
		//{
		//	int HostCnt = 1; //2 (³ªAß¿¡ ¼oA¤CO ºIºÐ)
		//	for(int nIdxHost = 0; nIdxHost < HostCnt; nIdxHost++)
		//	{
		//		CString strHostNum = _T("HOST");  //_T("HOST") + CConvert::ToString(nIdxHost+1); 
		//		pThis->ConnectStatus(pDoc->m_pConnectStatus, strHostNum);
		//		::Sleep(50); //Aß°¡
		//	}
		//}

		// @.60초마다 자원 계수기를 적는다. (한 바퀴가 1.4초 안팎이라 40바퀴쯤)
		static int nTick = 0;
		if (++nTick >= 40)
		{
			nTick = 0;
			CResCount::WriteLine();
		}

		::Sleep(1000); //1000
	}
	pThis->m_bThreadDoWork = FALSE;
	return 0;
}

void CCollectDB::Collect_EQUIPMENT(CEquipment* pEquipment)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	if (pEquipment == NULL)
		return;

	/*
	switch (pEquipment->m_enKind)
	{
	case CEquipment::enCV :
		{
			CCv* pCv = (CCv*)pEquipment;
			if (pCv != NULL)
			{
				//pCv->m_strInPlc = pEquipment->m_strThreadNo;
				strSql = pCv->GetSelectQry();
			}
			else
			{
				return;
			}
		}
		break;
	default:
		strSql = pEquipment->GetSelectQry();
		break;
	}
	//*/



	strSql = pEquipment->GetSelectQry();
	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}

	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);
	if(nRowCnt <= 0)
	{
		// @.행이 없어도 레코드셋은 열린 채로 돌아온다. 그냥 빠져나가면
		//   열린 레코드셋이 접속을 붙잡고 있어 접속이 닫히지 않는다.
		if (pRsptr != NULL)
		{
			try
			{
				if (pRsptr->GetState() != adStateClosed)
					pRsptr->Close();
			}
			catch (_com_error&)
			{
			}
			catch (...)
			{
			}
		}

		return;
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	::InterlockedIncrement(&CResCount::s_nMadeKind[CResCount::KindSlot(pEquipment->m_enKind)]);
	pEquipment->SetVar(pRsw);

	// @.받아갔는지 여기서 다시 보지 않는다. 보는 사이에 받아간 쪽 스레드가
	//   이미 지웠을 수 있어, 같은 것을 두 번 지우는 길이 있었다.
	//   받지 않는 종류는 CEquipment::SetVar 의 기본 구현이 그 자리에서 지운다.
}

void CCollectDB::ConnectStatus(CConnectStatus* pConnectStatus, CString strHostNum)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	pConnectStatus->m_HOST_NUM = strHostNum;
	strSql = pConnectStatus->GetSelectQry();

	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}

	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);

	if(nRowCnt <= 0)
	{
		return;
	}

	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	pConnectStatus->SetVar(pRsw);
	delete pConnectStatus->m_pRsw; 
	pConnectStatus->m_pRsw = NULL;
}