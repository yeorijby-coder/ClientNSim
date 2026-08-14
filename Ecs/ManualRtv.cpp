// ManualRtv.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualRtv.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CManualRtv 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualRtv, CSkinDialog)
CManualRtv::CManualRtv(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualRtv::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualRtv::CManualRtv(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualRtv::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}
CManualRtv::~CManualRtv()
{
	m_pDoc->m_pManualRtv = NULL;
	this->DestroyWindow();
}

void CManualRtv::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_RTV_MANUAL_RTV_NO, m_lblRtvManaulRtvNo);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_TYP, m_lblRtvJobTyp);
	DDX_Control(pDX, IDC_LBL_RTV_DEP_FORK, m_lblRtvManaulDepFork);
	DDX_Control(pDX, IDC_LBL_RTV_ARR_FORK, m_lblRtvManaulArrFork);

	DDX_Control(pDX, IDC_BTN_RTV_MANUAL_SAVE, m_btnRtvManualSave);
	DDX_Control(pDX, IDC_BTN_RTV_MANUAL_CLEAR, m_btnRtvManualClear);

	DDX_Control(pDX, IDC_CMB_RTV_MANUAL_RTV_NO, m_cbxRtvNo);
	DDX_Control(pDX, IDC_CMB_RTV_JOB_TYP, m_cbxJobTyp);
	DDX_Control(pDX, IDC_CMB_RTV_ARR_FORK, m_cbxArrFork);
	DDX_Control(pDX, IDC_CMB_RTV_DEP_FORK, m_cbxDepFork);

	DDX_Control(pDX, IDC_GRP_RTV_MANUAL_INFO, m_grpRtvInfo);
}


BEGIN_MESSAGE_MAP(CManualRtv, CSkinDialog)
	//	ON_WM_CLOSE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_RTV_MANUAL_SAVE, &CManualRtv::OnBnClickedBtnRtvManualSave)
	ON_BN_CLICKED(IDC_BTN_RTV_MANUAL_CLEAR, &CManualRtv::OnBnClickedBtnRtvManualClear)
END_MESSAGE_MAP()

BOOL CManualRtv::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if (!m_bInitialized)
	{
		RelocationControls();
		m_bInitialized = TRUE;
	}
	CLib::BindCombo_RTV_NO(m_cbxRtvNo, m_pDoc, (int)pEn);
	m_cbxRtvNo.SetCurSel(0);
	CLib::BindCombo_RTV_JOB_TYP(m_cbxJobTyp, m_pDoc, (int)pEn);
	m_cbxJobTyp.SetCurSel(0);
	CLib::BindCombo(m_cbxArrFork, _T("RTVArrHS"), m_pDoc, (int)pEn);
	CLib::BindCombo(m_cbxDepFork, _T("RTVDepHS"), m_pDoc, (int)pEn);


	InitializeResource(pEn);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManualRtv::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x = 0, y = 0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnRtvManualSave.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvManualSave.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvManualClear.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvManualClear.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}

void CManualRtv::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath2.Format(_T("%s"), chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnRtvManualSave.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvManualSave.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnRtvManualSave.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnRtvManualSave.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnRtvManualClear.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvManualClear.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);
	m_btnRtvManualClear.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnRtvManualClear.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}

void CManualRtv::InitializeResource(int pnLang)
{	//
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\16x16\\");
	CString strExtension = _T(".png");

	CImageList imageList;
	imageList.Create(16, 16, TRUE/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cv"), strExtension)));
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("sc"), strExtension)));

	RenameResource((EN_LANG)pnLang);
	RedrawImage();
}

void CManualRtv::RenameResource(EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_MANUAL_RTV_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_DEP_FORK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("order"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_MANUAL_SAVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("clear"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_MANUAL_CLEAR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("info"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_MANUAL_INFO, strValue);
}

void CManualRtv::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_pDoc->m_pRtvSkinManualDlg = NULL;
	m_pDoc->m_pManualRtv = NULL;
	delete this;
}

void CManualRtv::OnBnClickedBtnRtvManualSave()
{
	CString strSql = _T("");
	CString strRtvNo = _T("");
	CString strRtvJobTyp = _T("");
	CString strDepFork = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strPassCv = _T("0");

	UpdateData(TRUE);

	m_cbxRtvNo.GetWindowText(strRtvNo);
	m_cbxJobTyp.GetWindowText(strRtvJobTyp);
//	m_cbxArrFork.GetWindowText(strArrFork);
	m_cbxDepFork.GetWindowText(strDepFork);

	strRtvNo = m_cbxRtvNo.GetItemKey(m_cbxRtvNo.GetCurSel());

	if (strRtvJobTyp == _T("로딩")) strRtvJobTyp = "1";
	else if (strRtvJobTyp == _T("언로딩")) strRtvJobTyp = "2";
	else
	{
		strRtvJobTyp = "6";
		strPassCv = "1";
	}

	strRtvNo.Trim();

	if (strRtvNo == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV 호기를 선택하세요"))); return; }

	m_pDoc->BeginTrans_DLG();

	if (strDepFork == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업 위치를 선택하세요"))); return; }
//	if (strArrFork == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지 포크1을 선택하세요"))); return; }

	strSql.Format(_T(" UPDATE RTV_DATA				\n")
		_T("    SET LUGG_OD = '9999'				\n")
		_T("	  , JOB_TYP_OD = '%s'				\n")
		_T("	  , RTV_DEST_OD = '%s'				\n")
		_T("	  , CMD_RQ_ID = 'MOD'				\n")
		_T("	  , CMD_RQ_YN = 'Y'					\n")
		_T("	  , RTV_PASSCV_OD = '%s'			\n")
		_T("  WHERE WH_TYP = '%s'					\n")
		_T("    AND RTV_NO = '%s'			"), strRtvJobTyp, strDepFork, strPassCv, strWhTyp, strRtvNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = _T("9998");
		CString strLOG_MSG = _T("");

		strLOG_MSG.Format(_T("MANUAL RTV JOB -> RTV_NO : %s , JOB_TYP : %s , LOCATION : %s"), strRtvNo, strRtvJobTyp, strDepFork);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CManualRtv"), strLOG_LUGG_NO, _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV 수동 지시 성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV 수동 지시 실패")));
	return;
}


void CManualRtv::OnBnClickedBtnRtvManualClear()
{
	DataClear();
}

void CManualRtv::DataClear()
{
	m_cbxRtvNo.SetCurSel(0);
	m_cbxJobTyp.SetCurSel(0);
	m_cbxArrFork.SetCurSel(0);
	m_cbxDepFork.SetCurSel(0);
}
