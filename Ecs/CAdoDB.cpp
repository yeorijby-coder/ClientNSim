

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "AdoDB.h"
#include "RecordSetWrap.h"	// @.자원 계수기



///////////////////////////////////////////////
// CDBProc

IMPLEMENT_DYNCREATE(CAdoDB, CObject)

CAdoDB::CAdoDB()
{
	::InterlockedIncrement(&CResCount::s_nDbMade);
	m_pDoc = NULL;
	m_pWmsDb = NULL;

	m_strErrMsg = "";

	m_bConnected = FALSE;
}

CAdoDB::CAdoDB(CEcsDoc* pDoc)
{
	::InterlockedIncrement(&CResCount::s_nDbMade);
	m_pDoc = pDoc;
	m_pWmsDb = NULL;

	m_strErrMsg = "";

	m_bConnected = FALSE;
}


CAdoDB::~CAdoDB()
{
	::InterlockedIncrement(&CResCount::s_nDbGone);
	// @.예전에는 깃발만 내리고 접속은 그냥 두었다.
	//   _ConnectionPtr 이 알아서 닫아 줄 것 같지만, 이 객체로 연 레코드셋이
	//   CRecordSetWrap 안에 살아 있으면 그것이 접속을 붙잡고 있어 닫히지 않는다.
	//   수집 스레드가 재접속할 때마다 하나씩 남아, PostgreSQL 의
	//   max_connections(100) 를 몇 분 만에 다 썼다. 그러면 다른 프로그램이
	//   "남은 접속 슬롯이 없다(53300)" 로 로그인조차 못 한다.
	DisconnectDB();
}

///////////////////////////////////////////////
// @.접속을 닫는다. 이미 닫혀 있거나 아직 열지 않았으면 아무 일도 하지 않는다.
//   닫는 도중에 나는 예외는 삼킨다. 닫기에 실패해도 더 할 수 있는 일이 없고,
//   여기서 예외가 올라가면 파괴자를 타고 나가 프로그램이 죽는다.

void CAdoDB::DisconnectDB()
{
	if (m_pWmsDb == NULL)
	{
		m_bConnected = FALSE;
		return;
	}

	try
	{
		if (m_pWmsDb->GetState() != adStateClosed)
		{
			m_pWmsDb->Close();
			::InterlockedIncrement(&CResCount::s_nConnClose);
		}
	}
	catch (_com_error&)
	{
	}
	catch (...)
	{
	}

	m_pWmsDb = NULL;
	m_bConnected = FALSE;
}

BOOL CAdoDB::BeginTrans()
{
	m_pWmsDb->BeginTrans();

	return true;
}

BOOL CAdoDB::CommitTrans()
{
	m_pWmsDb->CommitTrans();

	return true;
}

BOOL CAdoDB::RollbackTrans()
{
	m_pWmsDb->RollbackTrans();

	return true;
}
///////////////////////////////////////////////
//
BOOL CAdoDB::ConnectDB() //보류6
{
	CString strConnet;

	if(m_pDoc == NULL || m_pDoc->m_pConfig == NULL)
	{
		return FALSE;	
	}

#if ORACLE
	strConnet.Format(_T("DRIVER=%s;Dbq=%s;UID=%s;PWD=%s"),
	m_pDoc->m_pConfig->m_strDATABASE_DRIVER, 
	m_pDoc->m_pConfig->m_strDATABASE_SERVER, 
	m_pDoc->m_pConfig->m_strDATABASE_USERID, 
	m_pDoc->m_pConfig->m_strDATABASE_USERPASSWORD);
#elif POSTGRESQL
	strConnet.Format(_T("Driver=%s;Server=%s;uid=%s;pwd=%s;Database=%s"), //Provider=MSDASQL; 
	m_pDoc->m_pConfig->m_strDATABASE_DRIVER,
	m_pDoc->m_pConfig->m_strDATABASE_SERVER, 
	m_pDoc->m_pConfig->m_strDATABASE_USERID, 
	m_pDoc->m_pConfig->m_strDATABASE_USERPASSWORD,
	m_pDoc->m_pConfig->m_strDATABASE_DATABASE);
#endif

	m_strErrMsg = "";

	// @.이미 열려 있는 접속을 덮어쓰면 그 접속은 주인을 잃는다. 먼저 닫는다.
	DisconnectDB();

	try
	{
		m_pWmsDb.CreateInstance(_uuidof(Connection));
		m_pWmsDb->ConnectionTimeout = 10;
		m_pWmsDb->CursorLocation = adUseClient;
		m_pWmsDb->Open(strConnet.AllocSysString(), "", "", NULL);
		m_pWmsDb->Errors->Clear();
		::InterlockedIncrement(&CResCount::s_nConnOpen);
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());
		::WritePrivateProfileString(_T("CONNECT"), _T("CONNECT"), strConnet , ECS_INI_FILE);
		m_strErrMsg.Format(_T("ConnectDB:%s\n%s\n%s"), (LPCTSTR)bstrSource,(LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg );
		AfxMessageBox(m_strErrMsg);
		return FALSE;
	}

	catch(...)
	{
		m_strErrMsg = "ConnectDB:DataBase Connect Failed!!!(Check ODBC)";
		AfxMessageBox(m_strErrMsg);
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}

void CAdoDB::CrateInputParameter(_CommandPtr pCmd, CObList *plistIn)
{
	for (POSITION pos = plistIn->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistIn->GetNext( pos );

		_bstr_t bstrName = pParam->m_strName;
		_bstr_t bstrValue = pParam->m_strData;

		pCmd->Parameters->Append( pCmd->CreateParameter(bstrName,
													    pParam->m_nType,
													    adParamInput,
													    1000,
													   _variant_t(bstrValue)) ); 
	}
}

void CAdoDB::CrateOutputParameter(_CommandPtr pCmd, CObList *plistOut)
{
	for (POSITION pos = plistOut->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistOut->GetNext( pos );

		_bstr_t bstrName = pParam->m_strName;
		pCmd->Parameters->Append(pCmd->CreateParameter(bstrName,
													   pParam->m_nType,
													   adParamOutput,
													   pParam->m_nSize) );
	}
}

void CAdoDB::ExtractOutputParameter(_CommandPtr pCmd, CObList *plistOut)
{
	for (POSITION pos = plistOut->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistOut->GetNext( pos );
		_bstr_t bstrName = pParam->m_strName;

		_variant_t vValue = pCmd->GetParameters()->GetItem(bstrName)->Value;
		pParam->SetData( (LPCTSTR)_bstr_t(vValue) );

		//pParam->JustShowData();
	}

}

BOOL CAdoDB::ExecuteStoredProc(CAdoDbIO *pAdoDbIO, int nScNum)
{
	m_pDoc->EnterBlcokingSection();

	_CommandPtr pCmd = NULL;
	CString strProcName = pAdoDbIO->m_strProcName;
	

//	POSITION pos = pAdoDbIO->m_listInput.GetHeadPosition();
//	CIOParam *pParam = (CIOParam *)pAdoDbIO->m_listInput.GetNext( pos );
//	CString strScNum =  pParam->m_strData;
///		_bstr_t bstrName = pParam->m_strName;
///		_bstr_t bstrValue = pParam->m_strData;
///		pParam->JustShowData();

	CString strScNum;
	strScNum.Format(_T("%d"),nScNum+1);
	//m_pDoc->strName = strScNum;
	CTime tStartTime = CTime::GetCurrentTime();
	//m_pDoc->strStartTime = CTime::GetCurrentTime().Format("%Y/%m/%d-%H:%M:%S");
	
	m_strErrMsg = "";

	try
	{
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = strProcName.AllocSysString();
		pCmd->CommandType = adCmdStoredProc;
		
		// Input
		CrateInputParameter(pCmd, &(pAdoDbIO->m_listInput));
		CrateOutputParameter(pCmd, &(pAdoDbIO->m_listOutput));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL, NULL, NULL);

		ExtractOutputParameter(pCmd, &(pAdoDbIO->m_listOutput));
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		m_strErrMsg.Format(_T("[프로시저명]=%s\n [SOURCE]=%s\n [DESCRIPTION]=%s\n [MSG]=%s"), 
							strProcName, (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);

		if ( err.Error() == E_FAIL )
			m_bConnected = FALSE;

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}
	catch(...)
	{
		m_strErrMsg.Format(_T("[프로시저명]=%s\n 처리 되지 않은 예외 발생.. 관리자에게 문의"),
							strProcName);

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}

	CTime tEndTime = CTime::GetCurrentTime();
	//m_pDoc->strEndTime = CTime::GetCurrentTime().Format(_T("%Y/%m/%d-%H:%M:%S"));
	CTimeSpan tDiffTime = tEndTime - tStartTime;
	//m_pDoc->lDiffTime = tDiffTime.GetTotalSeconds( );

	m_pDoc->LeaveBlcokingSection();
	return TRUE;
}

BOOL CAdoDB::ExecuteQueryString(CString strSql)
{
	m_pDoc->EnterBlcokingSection(); 
	try
	{
		m_pWmsDb->CommandTimeout=20;
		m_pWmsDb->Execute(strSql.AllocSysString(), NULL, adCmdText);
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		m_strErrMsg.Format(_T("[ExecuteQueryString] [SOURCE]=%s\n [DESCRIPTION]=%s\n [MSG]=%s"), 
					   (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);

		if ( err.Error() == E_FAIL )
			m_bConnected=FALSE;
		
		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}
	catch(...)
	{
		m_strErrMsg.Format(_T("[ExecuteQueryString]\n 처리 되지 않은 예외 발생.. 관리자에게 문의"));

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}

	m_pDoc->LeaveBlcokingSection();
	return TRUE;
}

void CAdoDB::UTF8toANSI(LPSTR src, CString &dst)
{
	//determine length of UTF-8 encoded string


//	dst = CString(lpszA);


	//delete[] lpszW;
	//delete[] lpszA;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// RecordSet


///////////////////////////////////////////////
// @.레코드셋을 조용히 닫는다.
//   열어 둔 레코드셋은 접속을 붙잡는다. 스마트 포인터에 NULL 을 넣어
//   놓아 주는 것만으로는 닫히지 않는다. 0건이거나 예외가 났을 때도
//   반드시 닫고 나가야 접속이 쌓이지 않는다.

static void CloseRs(_RecordsetPtr& rsPtr)
{
	if (rsPtr == NULL)
		return;

	try
	{
		if (rsPtr->GetState() != adStateClosed)
		{
			rsPtr->Close();
			::InterlockedIncrement(&CResCount::s_nRsClose);
		}
	}
	catch (_com_error&)
	{
	}
	catch (...)
	{
	}

	rsPtr = NULL;
}

_RecordsetPtr CAdoDB::SelectSqlForThread_RecordSet(CString strSql, int &nRowCnt, CString &strMsg)
{
	m_pWmsDb->CommandTimeout=60;

	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), 
			adOpenForwardOnly, adLockReadOnly, adCmdText);
		::InterlockedIncrement(&CResCount::s_nRsOpen);
			//adOpenDynamic, adLockReadOnly, adCmdText);

		if (rsPtr->adoEOF)
		{
			// @.0건이어도 레코드셋은 열려 있다. 닫고 나가야 접속이 풀린다.
			//   PLC 를 한 대만 붙여 둔 상태에서는 설비 조회 여덟 중 일곱이
			//   매 주기 0건이라, 여기서 새는 양이 가장 컸다.
			nRowCnt = 0;
			CloseRs(rsPtr);
			return NULL;
		}
		
		nRowCnt = rsPtr->RecordCount; 
		// @.Clone 으로 가벼운 사본을 만들고 원본을 닫는다.
		//   이 짝을 깨면(원본을 열어 둔 채 돌려주면) 조회 결과가 통째로
		//   남는다. 실제로 그렇게 바꿔 보니 4 GB 를 200초에 다 썼다.
		_RecordsetPtr rtrsPtr = rsPtr->Clone(adLockReadOnly);
		::InterlockedIncrement(&CResCount::s_nRsClone);
		rsPtr->Close();
		::InterlockedIncrement(&CResCount::s_nRsClose);
		return rtrsPtr;
	}
	// Error 발생 시 처리
	catch(_com_error &err){
 		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
			(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		CloseRs(rsPtr);

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	
			m_bConnected=FALSE;

		return NULL;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		CloseRs(rsPtr);	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	CloseRs(rsPtr);
	return NULL;
}

BOOL CAdoDB::SelectSqlForThread(CString strSql, CStringList &strTempList, int nRtRecord, int &nRowCnt, CString &strMsg)
{
	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), \
										adOpenDynamic, adLockReadOnly, adCmdText);

		if (rsPtr->adoEOF)
		{
			CloseRs(rsPtr);
			return TRUE;
		}
		
		nRowCnt = rsPtr->RecordCount; 

		for(int i=0; i < nRowCnt; i++) 
		{
			FieldsPtr fdsPtr = rsPtr->GetFields();
			CString strStream;
			
			
			for(int j=0; j<fdsPtr->Count; j++)
			{
				FieldPtr fdPtr = fdsPtr->GetItem(_variant_t((long)j));
				_bstr_t bstrFieldName  = fdPtr->Name;
				_bstr_t bstrFieldValue;
				
				if (fdPtr->Value.vt == VT_NULL)
				{
					bstrFieldValue = "";
				}
				else
				{
					bstrFieldValue = fdPtr->Value;
				}


			//	strTemp.Format(_T("%s;%s"), (LPCTSTR)bstrFieldName, (LPCTSTR)bstrFieldValue);
			//	strTempList.AddTail(strTemp);
			}

			if (i+1 ==nRtRecord)
			{
				rsPtr->Close();
				CloseRs(rsPtr);
				return TRUE;

			}
			//AfxMessageBox(strStream);
			rsPtr->MoveNext();

		}
		rsPtr->Close();

	}
	// Error 발생 시 처리
	catch(_com_error &err){
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
					(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		CloseRs(rsPtr);

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;
		
		return FALSE;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		CloseRs(rsPtr);	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	CloseRs(rsPtr);
	return TRUE;
}

BOOL CAdoDB::SelectSqlForThread(CString strSql, int &nRowCnt, CString &strMsg)
{
	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), \
			adOpenForwardOnly, adLockPessimistic, adCmdText);

		if (rsPtr->adoEOF)
		{
			CloseRs(rsPtr);
			return TRUE;
		}
		nRowCnt = rsPtr->RecordCount; 
		rsPtr->Close();
	}
	// Error 발생 시 처리
	catch(_com_error &err){
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
			(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		CloseRs(rsPtr);
		return FALSE;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		CloseRs(rsPtr);	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	CloseRs(rsPtr);
	return TRUE;
}


BOOL CAdoDB::ExecuteSqlForMainPGM(CString strSql)
{
	_bstr_t bstrValue;
	_variant_t vRecsAffected(0L);
	CString strTemp=_T("");
    
    bstrValue = strSql; 

	try
	{
		m_pWmsDb->CommandTimeout=20;
		m_pWmsDb->Execute(bstrValue, NULL, adCmdText);
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteSqlForMainPGM:%s\n\n%s\n\n%s"), 
					(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;
		
		return FALSE;
	}
	catch(...)
	{
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "ExecuteSqlForMainPGM:데이타 베이스 관리자에게 문의 하십시요");
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Procedure 실행
BOOL CAdoDB::ExecuteProcForInputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForInputOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForInputOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected = FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForInputOrder:데이타 베이스 관리자에게 문의 하십시요"));
		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

BOOL CAdoDB::ExecuteProcForOutputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg, CString &strJobNo)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poJobNo", adChar, adParamOutput, 5));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 
		vPoJobNo = pCmd->GetParameters()->GetItem("poJobNo")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		strJobNo.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoJobNo));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForOutputOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForOutputOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected = FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForOutputOrder:데이타 베이스 관리자에게 문의 하십시요"));
		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

// 이상시 처리
BOOL CAdoDB::ExecuteProcForManualOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForManualOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForManualOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForManualOrder:데이타 베이스 관리자에게 문의 하십시요"));
		

		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

