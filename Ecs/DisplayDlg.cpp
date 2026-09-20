// DisplayDlg.cpp: 구현 파일
//

//#include "pch.h"
#include "StdAfx.h"
#include "Ecs.h"
#include "DisplayDlg.h"
#include "afxdialogex.h"
#include "JobCollection.h"
#include "RecordSetWrap.h"


// CDisplayDlg 대화 상자

IMPLEMENT_DYNAMIC(CDisplayDlg, CDialogEx)

CDisplayDlg::CDisplayDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayDlg::IDD, pParent)
{
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CDisplayDlg::CDisplayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SKIN_DISPLAY_CTRL, pParent)
{
	m_bInitialized = FALSE;
}

CDisplayDlg::~CDisplayDlg()
{
	m_pDoc->m_pDisplayDlg = NULL;
	//CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_DISPLAY_CLOSE, m_btnDisplayClose);
	DDX_Control(pDX, IDC_BTN_DISPLAY_SNDDATA, m_btnDisplaySndData);
	DDX_Control(pDX, IDC_BTN_DISPLAY_INIT, m_btnDisplayInit);

	DDX_Control(pDX, IDC_GRP_DISPLAY_DISP_TEST, m_grpDisplayDispTest);
	DDX_Control(pDX, IDC_GRP_DISPLAY_TEST_DATA, m_grpDisplayTestData);

	DDX_Control(pDX, IDC_LBL_DISPLAY_CONTROLLER, m_lblDisplayController);
	DDX_Control(pDX, IDC_LBL_DISPLAY_DISP_NO, m_lblDisplayDispNo);
	DDX_Control(pDX, IDC_LBL_DISPLAY_COLOR, m_lblDisplayColor);
	DDX_Control(pDX, IDC_LBL_DISPLAY_DATA, m_lblDisplayData);

	DDX_Control(pDX, IDC_CBX_DISPLAY_CONTROLLER, m_cbxDisplayController);
	DDX_Control(pDX, IDC_CBX_DISPLAY_DISP_NO, m_cbxDisplayDispNo);
	DDX_Control(pDX, IDC_CBX_DISPLAY_COLOR, m_cbxDisplayColor);
	DDX_Control(pDX, IDC_EDT_DISPLAY_DATA, m_edtDisplayData);




}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DISPLAY_DISCONNECT, &CDisplayDlg::OnBnClickedBtnDisplayDisconnect)
	ON_BN_CLICKED(IDC_BTN_DISPLAY_SNDDATA, &CDisplayDlg::OnBnClickedBtnDisplaySnddata)
	ON_BN_CLICKED(IDC_BTN_DISPLAY_INIT, &CDisplayDlg::OnBnClickedBtnDisplayInit)
END_MESSAGE_MAP()


// CDisplayDlg 메시지 처리기

BOOL CDisplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어

	int iPlcNo;
	CString strPlcNo;

	SetBindCombo_DISPLAY_CTRL(m_cbxDisplayController, _T("10"));

	iPlcNo = m_cbxDisplayController.GetItemData(m_cbxDisplayController.GetCurSel());
	strPlcNo.Format(_T("%02d"), iPlcNo);

	SetBindCombo_DISPLAY_NO(m_cbxDisplayDispNo, _T("10"), strPlcNo);
	SetBindCombo_COMMON_CODE(m_cbxDisplayColor, _T("COLOR"), NULL);
	
	RenameResource(pEn);

	return TRUE;
}

void CDisplayDlg::OnClose()
{
	m_pDoc->m_pDisplayDlg = NULL;
	CDialogEx::OnClose();
}

void CDisplayDlg::RenameResource(EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("display"), (int)m_enLang);
	if (strValue.IsEmpty())
		strValue = _T("전광판");	// 리소스 ini 부재 시 기본 제목
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("controller"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_DISPLAY_CONTROLLER, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dispno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_DISPLAY_DISP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("displaytest"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_DISPLAY_DISP_TEST, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("testdata"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_DISPLAY_TEST_DATA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("color"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_DISPLAY_COLOR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("data"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_DISPLAY_DATA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("snddata"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DISPLAY_SNDDATA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("init"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DISPLAY_INIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_display\\"), _T("dlg_display"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("disconnect"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DISPLAY_DISCONNECT, strValue);
}

void CDisplayDlg::OnBnClickedBtnDisplayInit()
{
	m_edtDisplayData.SetWindowTextW(_T(""));

}

void CDisplayDlg::OnBnClickedBtnDisplaySnddata()
{
	CString strWH_TYP;
	CString strPLC_NO;
	CString strDISP_NO;
	CString strDISP_DATA;
	CString strLOG_MSG = _T("");

	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	CString strTemp;
	int iCOLOR;

	if (m_edtDisplayData.GetWindowTextLengthW() < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("TEST DATA를 입력해주세요")));
		return;
	}

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("TEST DATA를 전송하시겠습니까?")), MB_OKCANCEL) == IDCANCEL)
		return;

	strWH_TYP = _T("10");
	strPLC_NO.Format(_T("%02d"), m_cbxDisplayController.GetItemData(m_cbxDisplayController.GetCurSel()));
	strDISP_NO.Format(_T("%d"), m_cbxDisplayDispNo.GetItemData(m_cbxDisplayDispNo.GetCurSel()));
	iCOLOR = m_cbxDisplayColor.GetItemData(m_cbxDisplayColor.GetCurSel());

	m_edtDisplayData.GetWindowText(strDISP_DATA);

	m_pDoc->BeginTrans_DLG();

	strSql += CRLF + _T("  UPDATE DISPLAY_DATA		");
	strSql += CRLF + _T("	  SET CMD_RQ_YN	= 'Y'	");
	strSql += CRLF + _T("		, DISP_DATA	= '") + strDISP_DATA + _T("'	");
	if (iCOLOR != -1)
	{
		strTemp.Format(_T("	    , CMD_COLOR	=  %d"), iCOLOR);
		strSql += CRLF + strTemp;
	}
	strSql += CRLF + _T("	WHERE WH_TYP	= '") + strWH_TYP + _T("'	");
	strSql += CRLF + _T("	  AND PLC_NO	= '") + strPLC_NO + _T("'	");
	strSql += CRLF + _T("	  AND DISP_NO	= '") + strDISP_NO + _T("'	");

	//strSql.Format(_T("  UPDATE DISPLAY_DATA		") iCOLOR;
	//	_T("	           SET CMD_RQ_YN	= 'Y'	")
	//	_T("	             , DISP_DATA	= '%s'	")
	//	_T("	             , CMD_COLOR	= '%d'	")
	//	_T("	 WHERE WH_TYP	= '%s'		")
	//	_T("	   AND PLC_NO	= '%s'		")
	//	_T("	   AND DISP_NO	= '%s'		"), strDISP_DATA, iCOLOR, strWH_TYP, strPLC_NO, strDISP_NO);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE || isSuccess < 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("전광판 TEST DATA 쓰기 실패")));
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strLOG_MSG.Format(_T("전광판 TEST DATA 쓰기-> 컨트롤러 번호 : %s, 전광판 번호 : %s, DATA : %s "), strPLC_NO, strDISP_NO, strDISP_DATA);

	if (!m_pDoc->GetQueryInsertClientLog(_T("DisplayDlg"), _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	m_pDoc->CommitTrans_DLG();

	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("전광판 TEST DATA 쓰기 성공")));
}


void CDisplayDlg::OnBnClickedBtnDisplayDisconnect()
{
	CString strSql;
	CString strWH_TYP;
	CString strPLC_NO;
	CString strLOG_MSG = _T("");

	strWH_TYP = _T("10");
	strPLC_NO.Format(_T("%02d"), m_cbxDisplayController.GetItemData(m_cbxDisplayController.GetCurSel()));

	m_pDoc->BeginTrans_DLG();

	strSql.Format(_T("  UPDATE DISPLAY_CTRL		")
				  _T("	   SET DISCONNECT_YN	= 'Y'	")
				  _T("	 WHERE WH_TYP	= '%s'		")
				  _T("	   AND PLC_NO	= '%s'		"), strWH_TYP, strPLC_NO);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE || isSuccess < 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("전광판 연결 종료 실패")));
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strLOG_MSG.Format(_T("전광판 연결 종료-> 컨트롤러 번호 : %s"), strPLC_NO);

	if (!m_pDoc->GetQueryInsertClientLog(_T("DisplayDlg"), _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	m_pDoc->CommitTrans_DLG();

	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("전광판 연결 종료 성공")));
}

void CDisplayDlg::SetBindCombo_DISPLAY_CTRL(CComboBoxWrapper& cbx, CString strWh_Typ)
{
	if (m_pDoc == NULL) { return; };

	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어

	CStringList strList;
	CString strSql;
	CString strPLC_NO, strPLC_NM;

	int nRowCnt = 0, j = 0;

	CString strMessage;

	cbx.ResetContent();

	strSql.Format(_T("  SELECT  PLC_NO				")
				_T("	     , '전광판 CONTROL#' || PLC_NO AS PLC_NM		")
				_T("	  FROM DISPLAY_CTRL			")
				_T("	 WHERE WH_TYP	= '%s'		"), strWh_Typ);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();

	for (int i = 0; i < nRowCnt; i++)
	{
		strPLC_NO = pRsw->GetItem(_T("PLC_NO"));
		strPLC_NM = pRsw->GetItem(_T("PLC_NM"));
		cbx.AddString(strPLC_NM);
		cbx.SetItemData(j, CConvert::ToInt(strPLC_NO));

		cbx.SetCurSel(j);

		pRsw->MoveNext();
		j++;
	}
	int cc = cbx.GetCurSel();


	delete pRsw;
}

void CDisplayDlg::SetBindCombo_DISPLAY_NO(CComboBoxWrapper& cbx, CString strWh_Typ, CString strPlc_No)
{
	if (m_pDoc == NULL) { return; };

	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어

	CStringList strList;
	CString strSql;
	CString strDISP_NO, strDISP_NM;

	int nRowCnt = 0, j = 0;

	CString strMessage;

	cbx.ResetContent();

	strSql.Format(_T("  SELECT  DISP_NO					")
		_T("	     , DISP_NO || '호기' AS DISP_NM		")
		_T("	  FROM DISPLAY_DATA			")
		_T("	 WHERE WH_TYP	= '%s'		")
		_T("	   AND PLC_NO	= '%s'		")
		_T("	 ORDER BY PLC_NO, DISP_NO"), strWh_Typ, strPlc_No);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();

	for (int i = 0; i < nRowCnt; i++)
	{
		strDISP_NO = pRsw->GetItem(_T("DISP_NO")); 
		strDISP_NM = pRsw->GetItem(_T("DISP_NM"));

		cbx.AddString(strDISP_NM);
		cbx.SetItemData(j, CConvert::ToInt(strDISP_NO));

		//cbx.SetCurSel(j);

		pRsw->MoveNext();
		j++;
	}

	if (cbx.GetCount() > 0)
	{
		cbx.SetCurSel(0);
	}


	delete pRsw;
}

void CDisplayDlg::SetBindCombo_COMMON_CODE(CComboBox& cbx, CString strCDX_CD, CString strCCD_NM)
{
	CStringList strList;
	CString strSql;
	CString strCCD_CD, strCCD_NM_KOR;
	int nRowCnt = 0, j = 0;
	cbx.ResetContent();
	CString strMessage;

	strSql.Format(_T(" SELECT CCD_CD, CCD_NM_KOR	")
		_T("	 FROM COMMON_CODE					")
		_T("	WHERE CCD_CD_YN = 'Y'				")
		_T("    AND CDX_CD = '%s'					")
		_T("	ORDER BY CCD_CD"), strCDX_CD);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();

	for (int i = 0; i < nRowCnt; i++)
	{
		strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
		strCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));
		cbx.AddString(strCCD_NM_KOR);
		cbx.SetItemData(j, CConvert::ToInt(strCCD_CD));
		if (strCCD_CD == strCCD_NM)
		{
			cbx.SetCurSel(j);
		}
		pRsw->MoveNext();
		j++;
	}
	int cc = cbx.GetCurSel();
	if (cc == -1)
	{
		cbx.SetWindowText(strCCD_NM);
	}

	delete pRsw;
}







