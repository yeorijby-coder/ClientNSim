// ManualJob.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualJob.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "afxwin.h"


// CManualJob 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualJob, CSkinDialog)

CManualJob::CManualJob(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualJob::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualJob::CManualJob(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualJob::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}
CManualJob::~CManualJob()
{
	m_pDoc->m_pManualJob = NULL;
	this->DestroyWindow();
}

void CManualJob::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRP_MANUAL_JOB_SEARCH,		    m_grpManualJobSearch);

	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_WH_TYP,         m_lblManualJobWhTyp);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_STARTPOS,       m_lblManualJobStartPos);
	DDX_Control(pDX, IDC_MANUAL_JOB_LBL_DESTPOS,        m_lblManualJobDestPos);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_JOBTYP,         m_lblManualJobJobTyp);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_REMARKS,        m_lblManualJobRemark);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_LOCATION,       m_lblManualJobLocation);

	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_WHTYPE,		m_cbxManualJobWhTyp);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_STARTPOS,     m_cbxManualJobStartPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_DESTPOS,      m_cbxManualJobDestPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_EDIT_JOB_TYP, m_cbxManualJobJobTyp);

	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_START_LOCATION,   m_edtManualJobStartLocation);
	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_DEST_LOCATION, m_edtManualJobDestLocation);
	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_REMARKS,		m_edtManualJobRemark);

	DDX_Control(pDX, IDC_MASK_EDIT_MANUAL_JOB_CELL_NO_FR,   m_maskedtManualJobLocationFr);
	DDX_Control(pDX, IDC_MASK_EDIT_MANUAL_JOB_CELL_NO_TO,	m_maskedtManualJobLocationTo);

	DDX_Control(pDX, ID_BTN_MANUL_JOB_INSERT,			m_btnManualJobAdd);
	DDX_Control(pDX, ID_BTN_MANUAL_JOB_CLEAR,			m_btnManualJobDataClear);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_LOCATION_TO,    m_lblManualJobLocation_To);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_CNT, m_cbxManualJobCnt);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_PRIORITY, m_cbxManualJobPriority);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_CNT, m_lblManualJobCnt);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_PRIORITY, m_lblManualJobPriority);
}

BEGIN_MESSAGE_MAP(CManualJob, CSkinDialog)
	ON_COMMAND_RANGE(ID_BTN_MANUL_JOB_INSERT, ID_BTN_MANUAL_JOB_STODATE_FRTO, &CManualJob::OnCommandRangeButtonEvent)
	ON_BN_CLICKED(IDCANCEL, &CManualJob::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BTN_MANUL_JOB_INSERT, &CManualJob::OnBnClickedBtnManulJobInsert)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_MANUAL_JOB_CLEAR, &CManualJob::OnBnClickedBtnManualJobClear)
	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_JOB_EDIT_JOB_TYP, &CManualJob::OnSelchangeComboManualJobEditJobTyp)
	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_JOB_WHTYPE, &CManualJob::OnCbnSelchangeComboManualJobWhtype)
END_MESSAGE_MAP()

void CManualJob::OnClose()
{
	m_pDoc->m_pManualJob = NULL;
	delete this;
}

// CManualJob 메시지 처리기입니다.
BOOL CManualJob::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어
	InitializeFontManager(this);
	SetFontNation((int)pEn, 0);

	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobStartPos, m_pDoc);
	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobDestPos, m_pDoc);

	CLib::BindCombo(m_cbxManualJobWhTyp, _T("WH_TYP"), m_pDoc,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxManualJobJobTyp, _T("MANUAL_JOB_TYP"), m_pDoc,(int)pEn, TRUE);
	CLib::BindCombo(m_cbxManualJobPriority, _T("JOB_PRIORITY"), m_pDoc,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxManualJobCnt, _T("JOB_CNT"), m_pDoc,(int)pEn, FALSE);

	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RedrawImage();
		RenameResource(pEn);

		m_bInitialized = TRUE;		
	}

	m_edtManualJobStartLocation.SetWindowText(_T("01-001-01"));
	m_edtManualJobDestLocation.SetWindowText(_T("01-002-01"));

	m_maskedtManualJobLocationFr.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationFr.SetWindowText(_T("01-001-01"));

	m_maskedtManualJobLocationTo.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationTo.SetWindowText(_T("01-002-01"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManualJob::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}

void CManualJob::OnCommandRangeButtonEvent(UINT nID)
{
	switch(nID)
	{
	case ID_BTN_MANUL_JOB_INSERT:
		{
			OnBnClickedBtnManulJobInsert();
			break;
		}
	case ID_BTN_MANUAL_JOB_CLEAR:
		{
			OnBnClickedBtnManualJobClear();
			break;
		}   
	case ID_BTN_MANUAL_JOB_LOCATION_FRTO:
		{
			break;
		}    
	case ID_BTN_MANUAL_JOB_STODATE_FRTO:
		{
			break;
		}   
	default:
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("정의되지 않은 버튼 이벤트")) + CConvert::ToString((int)nID));
		}
	}
}

void CManualJob::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (strValue.IsEmpty())
		strValue = _T("수동 작업");	// 리소스 ini 부재 시 기본 제목
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completereport"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUL_JOB_COMP, strValue);
	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_CELL_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualadd "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUL_JOB_INSERT, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualdelete "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("datereset "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_CLEAR, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("copy"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_COPY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_JOBTYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("strageloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectcnt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("racktime"), (int)m_enLang);
//	SetDlgItemText(IDC_LBL_MANUAL_JOB_DURATION_TIME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_STARTPOS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_MANUAL_JOB_LBL_DESTPOS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_REMARKS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstartdt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_STODATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("serach"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_UPDATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_MANUAL_JOB_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_MANUAL_JOB_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("location"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("locationto"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION_TO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobpriority"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_PRIORITY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobcnt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_CNT, strValue);

	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("sign"), (int)m_enLang);
	//SetDlgItemText(IDOK, strValue);
}

void CManualJob::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;


	m_btnManualJobAdd.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualJobAdd.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnManualJobAdd.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualJobAdd.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnManualJobDataClear.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualJobDataClear.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);
	m_btnManualJobDataClear.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualJobDataClear.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}

void CManualJob::OnBnClickedBtnManulJobInsert()
{
	CString strSql = _T("");
	CString strWhTyp = _T("");
	CString strStartPos = _T("");
	CString strStartLocation = _T("");
	CString strDestPos = _T("");
	CString strDestLocation = _T("");
	CString strJobTyp = _T("");
	CString strRemarks = _T("");
	CString strStartLocBankFork1 = _T("");
	CString strStartLocBayFork1 = _T("");
	CString strStartLocLevFork1 = _T("");
	CString strDestLocBankFork1 = _T("");
	CString strDestLocBayFork1 = _T("");
	CString strDestLocLevFork1 = _T("");
	CString strJobPriority = _T("");
	CString strJobCnt = _T("");
	CString strScNo = _T("");
	CString strCellUseYn = _T("");
	CString strMessage = _T("");
	CString strJobStatus = _T("99");
	int nCnt = 0;
	int nRowCnt = 0;

	CStringList strJobMstList;

	UpdateData(TRUE);
	strWhTyp = m_pDoc->m_WH_TYP;
	m_cbxManualJobStartPos.GetWindowText(strStartPos);
	m_cbxManualJobDestPos.GetWindowText(strDestPos);
	m_cbxManualJobJobTyp.GetWindowText(strJobTyp);

	m_edtManualJobRemark.GetWindowText(strRemarks);
	m_maskedtManualJobLocationFr.GetWindowText(strStartLocation);
	m_maskedtManualJobLocationTo.GetWindowText(strDestLocation);

	//0100101
	strStartLocBankFork1 = strStartLocation.Mid(0,2);
	strStartLocBayFork1 = strStartLocation.Mid(2,3);
	strStartLocLevFork1 = strStartLocation.Mid(5,2);
	strDestLocBankFork1 = strDestLocation.Mid(0,2);
	strDestLocBayFork1 = strDestLocation.Mid(2,3);
	strDestLocLevFork1 = strDestLocation.Mid(5,2);

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("입력한 작업을 생성하시겠습니까?")), MB_YESNO) == IDYES)
    {
		//창고구분
		strWhTyp = m_cbxManualJobWhTyp.GetItemKey(m_cbxManualJobWhTyp.GetCurSel());
		if (m_cbxManualJobWhTyp.GetCurSel() == CB_ERR || strWhTyp == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("창고를 선택해주세요.")));
			return ;
		}

		//작업구분
		strJobTyp = m_cbxManualJobJobTyp.GetItemKey(m_cbxManualJobJobTyp.GetCurSel());
		if (m_cbxManualJobJobTyp.GetCurSel() == CB_ERR || strJobTyp == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업구분을 선택해주세요.")));
			return ;
		}

		//출발위치
		strStartPos = m_cbxManualJobStartPos.GetItemKey(m_cbxManualJobStartPos.GetCurSel());
		
		//도착지
		strDestPos = m_cbxManualJobDestPos.GetItemKey(m_cbxManualJobDestPos.GetCurSel());

		//우선순위 체크
		strJobPriority = m_cbxManualJobPriority.GetItemKey(m_cbxManualJobPriority.GetCurSel());
		if (strJobPriority == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("우선순위를 선택해주세요.")));
			return ;
		}

		//작업개수 체크
		strJobCnt = m_cbxManualJobCnt.GetItemKey(m_cbxManualJobCnt.GetCurSel());
		if (strJobCnt == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업개수를 선택해주세요.")));
			return ;
		}
		#pragma region 작업구분에 따른 로케이션 체크
		//로케이션 체크
		if(strJobTyp == "11")
		{
			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			if (strStartPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지를 선택해주세요.")));
					return ;
			}

			//입고때 출발 로케이션 00-000-00
			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");

			//로케이션에 따른 SC 구하기.
			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}
			strDestPos = strScNo;
			strJobStatus = "10";
		}
		else if(strJobTyp == "12")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if (strDestPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택해주세요.")));
					return ;
			}

			//출고때 도착 로케이션 00-000-00
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");

			//로케이션에 따른 SC 구하기.
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}
			strStartPos = strScNo;
			strJobStatus = "20";
		}
		else if(strJobTyp == "13")
		{
			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");
		}
		else if(strJobTyp == "14")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			//출발, 도착 로케이션 확인
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}

			strStartPos = strScNo;

			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}

			strDestPos = strScNo;

			if(strStartPos != strDestPos)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션이 동일한 SC가 아닙니다.")));
				return ;
			}
		}
		else if(strJobTyp == "15")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			//출발, 도착 로케이션 확인
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}

			strStartPos = strScNo;

			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}

			strDestPos = strScNo;

			if (strStartPos == strDestPos)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션의 SC가 동일한 호기입니다.")));
				return ;
			}
		}
		else if(strJobTyp == "10")
		{
			if (strStartPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지를 선택해주세요.")));
					return ;
			}
			if (strDestPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택해주세요.")));
					return ;
			}

			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");
		}
		#pragma endregion

		if (strStartPos == _T("")){ strStartPos = _T("0");}
		if (strDestPos == _T("")){ strDestPos = _T("0");}
		if (strJobTyp == _T("")){ strJobTyp = _T("0");}

		strWhTyp.Trim();
		strStartPos.Trim();
		strStartLocation.Trim();
		strDestPos.Trim();
		strDestLocation.Trim();
		strJobTyp.Trim();
		strJobPriority.Trim();
		strJobCnt.Trim();

		int nJobCnt = CConvert::ToInt(strJobCnt);

		//작업대 이동일 때만 작업개수. 그 외에는 1건
		if(strJobTyp != "6")
		{
			nJobCnt = 1;
		}

		CString strNextVal = _T("");

		//해당 SC의 출고 HS 구하기
		strSql.Format(_T("  SELECT NEXTVAL('MANUAL_SEQ') "));

		_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

		if (nRowCnt <= 0)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("MANUAL_SEQ 쿼리 조회 실패")));
			return;
		}

		pRsw->MoveFirst();
		//seq에서 작업에 사용할 번호 가져오기
		strNextVal = pRsw->GetItem(_T("nextval"));

		int nTemp = _tstoi(strNextVal);
		if (nTemp < 0 || nTemp > 999)
			nTemp = 0;

		strNextVal.Format(_T("%d"), nTemp + 9000);

		//pRsw->MoveNext();
		delete pRsw;

		m_pDoc->BeginTrans_DLG();

		for(int i = 0; i < nJobCnt; i++)
		{
			strSql.Format(_T(" INSERT INTO JOB_MST     (  WH_TYP				\n")
			_T("					                , LUGG_NO					\n")
			_T("					                , START_POS					\n")
			_T("					                , START_LOCATION			\n")
			_T("                                    , DEST_POS					\n")
			_T("                                    , DEST_LOCATION				\n")
			_T("                                    , JOB_TYP					\n")
			_T("                                    , JOB_STATUS				\n")
			_T("                                    , JOB_PRIORITY				\n")
			_T("                                    , INS_DT					\n")
			_T("                                    , INS_USER_ID				\n")
			_T("                                    , REMARKS 					\n")
			_T("                                    , WC_STEP )					\n")
			_T("                            VALUES ('%s'						\n")
			_T("                                   , '%s'						\n") 
			_T("                                   , '%s'						\n")
			_T("                                   , '%02s-%03s-%02s'			\n")
			_T("                                   , '%s'						\n")
			_T("                                   , '%02s-%03s-%02s'			\n")
			_T("                                   , '%s'						\n")
			_T("								   , '%s'						\n")
			_T("								   , '%s'						\n")
			_T("                                   , ") + m_pDoc->SYSDATE + _T(" \n") 
			_T("                                   , '%s'						 \n")
			_T("                                   , '%s' 						 \n")
			_T("                                   , '0' )						 ")  , strWhTyp, strNextVal, strStartPos, strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strDestPos, strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strJobTyp, strJobStatus, strJobPriority, m_pDoc->m_strId, strRemarks);
			

			BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == FALSE || isSuccess < 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("반자동 작업 생성 실패")));
				m_pDoc->RollbackTrans_DLG();
				return;
			}
		}
		
		CString strStartLoc = strStartLocBankFork1 + _T("-") + strStartLocBayFork1 + _T("-") + strStartLocLevFork1;
		CString strDestLoc = strDestLocBankFork1 + _T("-") + strDestLocBayFork1 + _T("-") + strDestLocLevFork1;
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("수동 작업 지시 -> 출발지 : %s , 출발로케이션 : %s , 도착지 : %s, 도착로케이션 : %s, 작업구분 : %s, 작업개수 : %s"), strStartPos, strStartLoc, strDestPos, strDestLoc, strJobTyp, strJobCnt);
		
		if (!m_pDoc->GetQueryInsertClientLog(_T("CManualJob"), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("반자동 작업 생성 성공")));
		refresh_Chk();
		m_pDoc->CommitTrans_DLG();
	}
}

BEGIN_EVENTSINK_MAP(CManualJob, CSkinDialog)
END_EVENTSINK_MAP()

void CManualJob::refresh_Chk()
{
	m_cbxManualJobJobTyp.SetCurSel(0);
	m_cbxManualJobStartPos.SetCurSel(0);
	m_cbxManualJobDestPos.SetCurSel(0);
	m_cbxManualJobPriority.SetCurSel(0);
	m_cbxManualJobCnt.SetCurSel(0);

	m_edtManualJobStartLocation.SetWindowText(_T("01-001-01")); //출발 롴
	m_edtManualJobDestLocation.SetWindowText(_T("01-002-01")); //도착 롴
	m_maskedtManualJobLocationFr.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationFr.SetWindowText(_T("01-001-01"));

	m_maskedtManualJobLocationTo.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationTo.SetWindowText(_T("01-002-01"));

	m_edtManualJobRemark.SetWindowText(_T(""));
}

void CManualJob::OnBnClickedBtnManualJobClear()
{
	refresh_Chk();
	m_maskedtManualJobLocationFr.EnableWindow(FALSE);
	m_maskedtManualJobLocationTo.EnableWindow(FALSE);
	m_cbxManualJobStartPos.EnableWindow(FALSE);
	m_cbxManualJobDestPos.EnableWindow(FALSE);
}

void CManualJob::OnSelchangeComboManualJobEditJobTyp()
{
	//EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어
	//SetFontNation((int)pEn);

	CString strJobTyp =_T("");
	
	UpdateData(TRUE);

	strJobTyp = m_cbxManualJobJobTyp.GetItemKey(m_cbxManualJobJobTyp.GetCurSel());

	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobStartPos, m_pDoc);
	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobDestPos, m_pDoc);

	if (strJobTyp == _T("10"))
	{
		m_cbxManualJobStartPos.DeleteString(3);
		m_cbxManualJobStartPos.DeleteString(3);
		m_cbxManualJobDestPos.DeleteString(3);
		m_cbxManualJobDestPos.DeleteString(3);
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(TRUE);
		m_cbxManualJobDestPos.EnableWindow(TRUE);
	}
	else if (strJobTyp == _T("11"))
	{
		m_cbxManualJobStartPos.DeleteString(3);
		m_cbxManualJobStartPos.DeleteString(3);
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(TRUE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("12"))
	{
		m_cbxManualJobDestPos.DeleteString(3);
		m_cbxManualJobDestPos.DeleteString(3);
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(TRUE);
	}
	else if (strJobTyp == _T("13"))
	{
		m_cbxManualJobDestPos.DeleteString(1);
		m_cbxManualJobDestPos.DeleteString(1);
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(TRUE);
	}
	else if (strJobTyp == _T("14"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("15"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
}


void CManualJob::OnCbnSelchangeComboManualJobWhtype()
{
}

CString CManualJob::GetQrySelectCELLMST(CString strBANK, CString strBAY, CString strLEV, CString& strSC_NO, CString& strCELL_USE_YN, int& nCnt)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql += CRLF + _T("SELECT SC_NO, CELL_USE_YN");
	strSql += CRLF + _T("  FROM CELL_MST ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP);
	strSql += CRLF + _T("   AND BANK = ") + CLib::Quot(strBANK);
	strSql += CRLF + _T("   AND BAY = ") + CLib::Quot(strBAY);
	strSql += CRLF + _T("   AND LEV = ") + CLib::Quot(strLEV);


	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();


	strSC_NO = pRsw->GetItem(_T("SC_NO"));
	strCELL_USE_YN = pRsw->GetItem(_T("CELL_USE_YN"));
	nCnt = nRowCnt;

	delete pRsw;
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}