
// CvSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "CvSkinDlg.h"
#include "afxdialogex.h"
#include "JOB_MST.h"
#include "JobCollection.h"
#include "RecordSetWrap.h"


// CCvSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCvSkinDlg, CSkinDialog)

CCvSkinDlg::CCvSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CCvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pTrackInfo = NULL;
	m_blAutoSel = false;
}

CCvSkinDlg::CCvSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CCvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = NULL;
	m_nLang = EN_ENG;
	m_pTrackInfo = NULL;
	m_blAutoSel = false;

}

CCvSkinDlg::~CCvSkinDlg()
{

	m_pDoc->m_pCvSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CCvSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);	
	//BASE BUTTON
	DDX_Control(pDX, ID_BTN_CV_CANCEL, m_btnCancel);
	DDX_Control(pDX, ID_BTN_CV_OK, m_btnOk);
	//------------

	//DLG DATA ( LBL )

	DDX_Control(pDX, IDC_LBL_CV_JOB_NO,                m_lblCvJobNo);
	DDX_Control(pDX, IDC_LBL_CV_DEST_POS,		       m_lblCvDestPos);
	DDX_Control(pDX, IDC_LBL_CV_JOB_TYP,		       m_lblCvJobTyp);
	DDX_Control(pDX, IDC_LBL_CV_BARCODE,			   m_lblCvBarcode);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_NO,		       m_lblCvJobJobNo);
	DDX_Control(pDX, IDC_LBL_JOB_START_POS,		       m_lblCvJobStartPos);
	DDX_Control(pDX, IDC_LBL_JOB_DEST_POS,		       m_lblCvJobDestPos);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_TYP,		       m_lblCvJobJobTyp);
	DDX_Control(pDX, IDC_LBL_CV_WAIT_TIME,			   m_lblCvWaiteTIme);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_STATUS,		   m_lblCvJobStatus);

	//GRP
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS,			   m_grpCvCvStatus);
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_COMMAND,     m_grpCvCvStatusCommand);	// 명령 그룹박스도 다른 그룹박스와 동일 스타일로
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_ITEM,        m_grpCvCvStatusItem);
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_VALUE,       m_grpCvCvStatusValue);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_VALUE5,	   m_grpCvJobStatusValue5);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_ITEM3,	   m_grpCvJobStatusItem3);

	DDX_Control(pDX, IDC_GRP_CV_SENSOR_INFO, m_grpCvSensorInfo);
	DDX_Control(pDX, IDC_GRP_CV_JOB_READY_STA_INFO, m_grpCvJobReadyStaInfo);
	
	//CVSTATUS

	//JOBSTATUS
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS,			   m_grpCvJobStatus);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_ITEM,	   m_grpCvJobStatusItem);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_VALUE,	   m_grpCvJobStatusValue);
	//--JOBSTATUS

	//STATUS
	//	DDX_Control(pDX, IDC_GRP_CV_STATUS_SENSOR,     	   m_grpCvStatusSensor);
	DDX_Control(pDX, IDC_GRP_CV_STATUS_STATUS,	       m_grpCvStatusStatus);
	//--STATUS
	//GRP


	//TRACKNO
	DDX_Control(pDX, IDC_EDT_TRACK_NO,                 m_edtCvTrackNo);
	//--TRACKNO

	//CVSTATUS					
	DDX_Control(pDX, IDC_EDT_CV_JOB_NO,			       m_edtCvJobNo);
	DDX_Control(pDX, IDC_EDT_CV_ERR_CD,                m_edtCvErrorCode);

	DDX_Control(pDX, IDC_CBX_CV_DEST_POS,              m_cbxCvDestPos); 
	DDX_Control(pDX, IDC_CBX_CV_JOB_TYP,               m_cbxCvJobTyp);

	//--CVSTATUS									   

	//JOBSTATUS										   
	DDX_Control(pDX, IDC_EDT_JOB_JOB_NO,               m_edtCvJobJobNo);
	DDX_Control(pDX, IDC_EDT_JOB_JOB_TYP,			   m_edtCvJobJobTyp);
	DDX_Control(pDX, IDC_EDT_JOB_JOB_STATUS,		   m_edtCvJobJobStatus);
	DDX_Control(pDX, IDC_EDT_JOB_START_POS,            m_edtCvJobStartPos);
	DDX_Control(pDX, IDC_EDT_JOB_DEST_POS,             m_edtCvJobDestPos);

	DDX_Control(pDX, IDC_BTN_CV_WAIT_TIME_UP,		   m_btnCvWaitTimeUp);
	DDX_Control(pDX, IDC_BTN_CV_WAIT_TIME_DOWN,		   m_btnCvWaitTimeDown);
	DDX_Control(pDX, IDC_BTN_CV_ACTIVE,				   m_btnCvActive);
	//--JOBSTATUS

	//SENSOR

	//DDX_Control(pDX, IDC_BTN_RTV_INTERLOCK,			   m_btnCvRtvLockSign);

	//--SENSOR

	//STATUS
	DDX_Control(pDX, IDC_BTN_STATUS_AUTO_MODE,				m_btnCvAutoMode);
	DDX_Control(pDX, IDC_BTN_SENSOR_PROD0,					m_btnCvSenserProd0);
	DDX_Control(pDX, IDC_BTN_TR_PAUSE,						m_btnCvTrPause);
	DDX_Control(pDX, IDC_BTN_STATUS_STO_READY,				m_btnCvStoReady);
	DDX_Control(pDX, IDC_BTN_STATUS_RET_READY,				m_btnCvRetReady);
	DDX_Control(pDX, IDC_BTN_STATUS_STOHS_READY,			m_btnCvStoHsReady);
	DDX_Control(pDX, IDC_BTN_STATUS_RETHS_READY,			m_btnCvRetHsReady);
	DDX_Control(pDX, IDC_BTN_STATUS_DEVERTER_HS_DOWN,		m_btnCvDiverterHsDown);
	DDX_Control(pDX, IDC_BTN_STATUS_DEVERTER_HS_UP,			m_btnCvDiverterHsUp);
	DDX_Control(pDX, IDC_BTN_DOOR_OPEN,						m_btnCvDoorOpen);
	DDX_Control(pDX, IDC_BTN_RGV_STATUS_LOAD,				m_btnRgvStatusLoad);
	DDX_Control(pDX, IDC_BTN_SC_PA_PK_POSSIBLE,				m_btnScPaPkPossible);
	DDX_Control(pDX, IDC_BTN_DOOR_OPEN_REQ,					m_btnCvDoorOpenReq);
	DDX_Control(pDX, IDC_BTN_DOOR_CLOSE_REQ,				m_btnCvDoorCloseReq);
	DDX_Control(pDX, IDC_BTN_MOTER_1_RUN,					m_btnCvMoter1Run);
	DDX_Control(pDX, IDC_BTN_MOTER_2_RUN,					m_btnCvMoter2Run);
	DDX_Control(pDX, IDC_BTN_SIZE_CHECKER_LOW,				m_btnCvSizeCheckerLow);
	DDX_Control(pDX, IDC_BTN_SIZE_CHECKER_HIGH,				m_btnCvSizeCheckerHigh);
	DDX_Control(pDX, IDC_BTN_STATUS_RET_READY_STA,			m_btnCvRetReadySta);
	DDX_Control(pDX, IDC_BTN_PICKING_4_BYPASS,				m_btnCvPicking4ByPass);
	//DDX_Control(pDX, IDC_BTN_STATUS_RTV_DEPART_HS_READY,	m_btnCvRtvDepartHsReady);
//DDX_Control(pDX, IDC_BTN_STATUS_RTV_ARRIVE_HS_READY,	m_btnCvRtvArriveHsReady);
	//--STATUS

	//COMMAND
	DDX_Control(pDX, IDC_BTN_CV_WRITE,	               m_btnCvWrite);
	DDX_Control(pDX, IDC_BTN_CV_DELETE,                m_btnCvDelete);
	DDX_Control(pDX, IDC_BTN_CV_COPY,	               m_btnCvCopy);
	DDX_Control(pDX, IDC_BTN_CV_CUT,	               m_btnCvCut);
	DDX_Control(pDX, IDC_BTN_CV_PASTE,	               m_btnCvPaste);
	DDX_Control(pDX, IDC_BTN_CV_DISCHARGE,			   m_btnCvDischarge);
	DDX_Control(pDX, IDC_BTN_CV_SUSPEND,			   m_btnCvSuspend);
	DDX_Control(pDX, IDC_BTN_CV_ACTIVE,				   m_btnCvActive);
	DDX_Control(pDX, IDC_BTN_CV_WAITE_TIME,			   m_btnCvWaitTime);
	DDX_Control(pDX, IDC_BTN_CV_ERR_RESET,			   m_btnCvErr_Reset);
	DDX_Control(pDX, IDC_BTN_DP_INSERT_JOB,			   m_btnDpInsertJob);

	DDX_Control(pDX, IDC_LBL_JOB_PALLET_NUMBER, m_lblCvJobPalletNumber);
	DDX_Control(pDX, IDC_EDT_JOB_PALLET_NUMBER, m_edtCvJobPalletNumber);
	DDX_Control(pDX, IDC_LBL_JOB_PRODUCT_ID,	m_lblCvJobProductId);
	DDX_Control(pDX, IDC_EDT_JOB_PRODUCT_ID,	m_edtCvJobProductId);

	DDX_Control(pDX, IDC_EDT_CV_BARCDOE, m_edtCvBarCode);
	DDX_Control(pDX, IDC_EDT_JOB_DEST_LOC, m_edtJobDestLoc);
	DDX_Control(pDX, IDC_EDT_JOB_START_LOC, m_edtJobStartLoc);
	DDX_Control(pDX, IDC_EDT_CV_SUSPEND, m_edtCvSuspend);
	DDX_Control(pDX, IDC_LBL_JOB_DEST_LOC, m_lblJobDestLoc);
	DDX_Control(pDX, IDC_LBL_JOB_START_LOC, m_lblJobStartLoc);
	DDX_Control(pDX, IDC_LBL_CV_SUSPEND, m_lblCvSuspend);

	DDX_Control(pDX, IDC_EDT_TRACK, m_edtTrack);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL, m_chkAutoSel);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL2, m_chkDebugMode);
	DDX_Control(pDX, IDC_CHK_TR_PAUSE, m_chkTrPause);
	
	DDX_Control(pDX, IDC_EDT_MG_STATUS, m_edtMgStatus);
	DDX_Control(pDX, IDC_GRP_MGDP_STATUS, m_grpMgdpStatus);
	DDX_Control(pDX, IDC_LBL_MG_STATUS, m_lblMgStatus);
	DDX_Control(pDX, IDC_BTN_MZ_MOVE, m_btnMzMove);
}

BEGIN_MESSAGE_MAP(CCvSkinDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CCvSkinDlg::OnMessageSwitch)
	ON_BN_CLICKED(IDC_BTN_CV_WRITE, &CCvSkinDlg::OnBnClickedBtnCvWrite)
	ON_BN_CLICKED(IDC_BTN_CV_DELETE, &CCvSkinDlg::OnBnClickedBtnCvDelete)
	ON_BN_CLICKED(IDC_BTN_CV_COPY, &CCvSkinDlg::OnBnClickedBtnCvCopy)
	ON_BN_CLICKED(IDC_BTN_CV_CUT, &CCvSkinDlg::OnBnClickedBtnCvCut)
	ON_BN_CLICKED(IDC_BTN_CV_PASTE, &CCvSkinDlg::OnBnClickedBtnCvPaste)
	ON_BN_CLICKED(IDC_BTN_CV_WAIT_TIME_UP, &CCvSkinDlg::OnBnClickedBtnCvWaitTimeUp)
	ON_BN_CLICKED(IDC_BTN_CV_WAIT_TIME_DOWN, &CCvSkinDlg::OnBnClickedBtnCvWaitTimeDown)
	ON_BN_CLICKED(IDC_BTN_CV_SUSPEND, &CCvSkinDlg::OnBnClickedBtnCvSuspend)
	ON_BN_CLICKED(IDC_BTN_CV_ACTIVE, &CCvSkinDlg::OnBnClickedBtnCvActive)
	ON_BN_CLICKED(IDC_BTN_CV_ERR_RESET, &CCvSkinDlg::OnBnClickedBtnCvErrReset)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL, &CCvSkinDlg::OnBnClickedChkAutoSel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MZ_MOVE, &CCvSkinDlg::OnBnClickedBtnMzMove)
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL2, &CCvSkinDlg::OnBnClickedChkAutoSel2)
	ON_BN_CLICKED(IDC_CHK_TR_PAUSE, &CCvSkinDlg::OnBnClickedChkTrPause)
	ON_BN_CLICKED(IDC_BTN_STATUS_RETHS_READY, &CCvSkinDlg::OnBnClickedBtnStatusRethsReady)
	ON_BN_CLICKED(IDC_BTN_CV_DISCHARGE, &CCvSkinDlg::OnBnClickedBtnCvDischarge)

END_MESSAGE_MAP()

// CCvSkinDlg 메시지 처리기입니다.

LRESULT CCvSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
{ 
	CTrackInfo* pTrackInfo = (CTrackInfo*)wParam;
	CTrackInfo* pTrackInfoParm = m_pTrackInfo;
	if(pTrackInfo == NULL){ pTrackInfo = pTrackInfoParm;}
	if(pTrackInfo == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("NOT FIND TRACK")));
		return 0;
	}
	
	m_pTrackInfo = pTrackInfo;
	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
 	}

	InvalidateTrackData(enLangTemp);

	return 0;
}

BOOL CCvSkinDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		RelocationControls();
		m_bInitialized = TRUE;	
	}
	InitializeResource(pEn);
	CLib::BindCombo(m_cbxCvJobTyp, _T("JOB_TYP"), m_pDoc, (int)pEn);

	int nTemp = (int)m_pDoc->m_bDebugMode;

	m_chkDebugMode.SetCheck(nTemp);

	m_btnCvSuspend.EnableWindow(false);

	m_chkAutoSel.SetCheck(1);
	m_blAutoSel = true;
	SetTimer(1, 1500, NULL);

	UpdateData(FALSE);

	return TRUE;
}

//	rc_resource\dlg_cv\dlg_cv.ini 에서 한 줄 읽는다.
//	상태에 따라 글자가 바뀌는 자리(자동/수동 모드)도 언어를 타야 해서 둔다.
CString CCvSkinDlg::GetResString(LPCTSTR lpszKey)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	strAppPath.Format(_T("%s"), chrFileName);
	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), _T(".ini"));

	EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
	return CLib::GetIniStringFromPath(strFullPath, lpszKey, (int)enLang);
}

void CCvSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (strValue.IsEmpty())
		strValue = _T("CV 상태정보");	// 리소스 ini 부재 시 기본 제목
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("write"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_WRITE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("reset"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_ERR_RESET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_DELETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("copy"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_COPY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("discharge"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_DISCHARGE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cut"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_CUT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("paste"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_PASTE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waittime"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_WAITE_TIME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waittime"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_WAIT_TIME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("palletno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_PALLET_NUMBER, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productid"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_PRODUCT_ID, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dpinsjob"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DP_INSERT_JOB, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvactive"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("barcode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_BARCODE, strValue);
	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_COMMAND, strValue);

	//JOBSTATUS
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS, strValue);

	//JOBSTATUS
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_MGDP_STATUS, strValue);
	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item2"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_ITEM3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value2"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_VALUE5, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_STATUS_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_START_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_DEST_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sensorinfo"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_SENSOR_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobreadystainfo"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_READY_STA_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_SUSPEND, strValue);
	//	예전에는 Ecs.rc 캡션 그대로라 언어를 바꿔도 안 바뀌던 것들
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("autosel"), (int)m_enLang);
	SetDlgItemText(IDC_CHK_AUTO_SEL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stoready"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_STO_READY, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retready"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_RET_READY, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stohsready"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_STOHS_READY, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rethsready"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_RETHS_READY, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("automode"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_AUTO_MODE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("prodsensor"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SENSOR_PROD0, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("divhsup"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_DEVERTER_HS_UP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("divhsdown"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_DEVERTER_HS_DOWN, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dooropen"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DOOR_OPEN, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retreadysta"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STATUS_RET_READY_STA, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("picking4bypass"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_PICKING_4_BYPASS, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rgvload"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RGV_STATUS_LOAD, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scpapk"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_PA_PK_POSSIBLE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dooropenreq"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DOOR_OPEN_REQ, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("doorclosereq"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DOOR_CLOSE_REQ, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("moter1run"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_MOTER_1_RUN, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("moter2run"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_MOTER_2_RUN, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sizecheckerlow"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SIZE_CHECKER_LOW, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sizecheckerhigh"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SIZE_CHECKER_HIGH, strValue);
}

void CCvSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");
	m_btnCvWrite.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWrite.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvErr_Reset.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvErr_Reset.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("reset"), strExtension)), NULL, 5, 5);

	m_btnDpInsertJob.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnDpInsertJob.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("mgdp"), strExtension)), NULL, 5, 5);

	m_btnCvDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnCvCopy.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvCopy.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);

	m_btnCvDischarge.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvDischarge.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("discharge"), strExtension)), NULL, 5, 5);

	m_btnCvCut.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvCut.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);

	m_btnCvPaste.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvPaste.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("paste"), strExtension)), NULL, 5, 5);

	m_btnCvWaitTime.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTime.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
	
	m_btnCvWaitTimeUp.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTimeUp.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-up"), strExtension)), NULL, 5, 5);

	m_btnCvWaitTimeDown.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTimeDown.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-down"), strExtension)), NULL, 5, 5);

	m_btnCvSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("stop"), strExtension)), NULL, 5, 5);

	m_btnCvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnMzMove.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnMzMove.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);


	m_btnCvAutoMode.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvSenserProd0.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvTrPause.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvStoReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRetReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvStoHsReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRetHsReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvDiverterHsDown.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvDiverterHsUp.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvDoorOpen.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnRgvStatusLoad.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnScPaPkPossible.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvDoorOpenReq.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvDoorCloseReq.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvMoter1Run.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvMoter2Run.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvSizeCheckerLow.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvSizeCheckerHigh.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRetReadySta.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvPicking4ByPass.SetIcon(Global.GetIcon(Global.ICO_CV_ON));

}

void CCvSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}


HCURSOR CCvSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCvSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CCvSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnCvWrite.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWrite.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvErr_Reset.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvErr_Reset.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnDpInsertJob.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnDpInsertJob.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvCopy.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvCopy.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvDischarge.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvDischarge.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvCut.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvCut.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvPaste.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvPaste.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvWaitTime.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWaitTime.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx,  sizeLarge.cy);
}

BOOL CCvSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CCvSkinDlg::InvalidateTrackData(EN_LANG pLang)
{	
	if(m_pDoc == NULL) return;
	if(m_pTrackInfo == NULL) return;
	if(m_pTrackInfo->m_pCV_DATA == NULL) return;

	CString strGetErrorCode = _T("");
	CString strSUSPEND = _T("");
	CString strSTOCK_MODE = _T("");
	CString strROLL_MODE = _T("");
	CString strREMOTE_CONTROL = _T("");
	CString strKIND = _T("");
	CString strSTATUS = _T("");
	CString strOD_RQ_ID = _T("");
	int nLUGG_NO = 0;

	if (!UpdateData())
		return;
	
	//붙여넣기 활성화 여부 판단 COPY_YN(복사버튼 눌렀는지 유무)
	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();

	if (pCopyJob->COPY_YN == false)
		GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);

	if (!m_pTrackInfo->m_pCV_DATA->IsRejectTrack())
		GetDlgItem(IDC_BTN_CV_DISCHARGE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BTN_CV_DISCHARGE)->EnableWindow(FALSE);

	if (!m_pTrackInfo->m_pCV_DATA->IsNotInvokeTrack())
		GetDlgItem(IDC_BTN_CV_SUSPEND)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BTN_CV_SUSPEND)->EnableWindow(FALSE);

	CString strJobTyp, strDestPos;

	m_cbxCvJobTyp.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD);
	//CV상태의 도착위치 콤보박스
	SetBindCombo_DEST_POS_DEF(m_cbxCvDestPos, m_pTrackInfo->m_pCV_DATA->K_PLC_NO,m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD);

	//상단 명칭 표시
	m_edtTrack.SetWindowText(_T("Track No"));

	//CV 명칭 표시
	if(m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM == "") // 명칭이 없으면 그냥 3자리 NO 
	{
		m_edtCvTrackNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_MC_NO);
	}
	else
	{
		m_edtCvTrackNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_MC_NO + " (" + m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM + ")");
	}

	m_cbxCvJobTyp.SetCurSelEx(m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD);
	m_edtCvJobNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD);

	GetErrorCode(_T("CV"), m_pTrackInfo->m_pCV_DATA->V_ERROR_CODE, (int)pLang, strGetErrorCode);
	m_edtCvErrorCode.SetWindowText(strGetErrorCode);

	CString strCCD_NM_KOR;
	if (m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD == "1")
	{
		m_chkTrPause.SetCheck(1);
		m_edtCvSuspend.SetWindowText(_T("작업중지"));
	}
	else
	{
		m_chkTrPause.SetCheck(0);
		m_edtCvSuspend.SetWindowText(_T("작업중"));
	}
	
	m_edtCvBarCode.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_BARCODE);

	CString strMgMsg = _T("");

	CString strMessage;
	int nRowCnt = -1;
	
	//버튼추가되야함
	m_btnCvSenserProd0.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));

	m_btnCvAutoMode.SetIcon((m_pTrackInfo->m_pCV_DATA->V_AUTO_MODE_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	if (m_pTrackInfo->m_pCV_DATA->V_AUTO_MODE_RD == _T("0"))
	{
		m_btnCvAutoMode.SetWindowText(GetResString(_T("modemanual")));
	}
	else
	{
		m_btnCvAutoMode.SetWindowText(GetResString(_T("modeauto")));
	}
	m_btnCvStoReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_STO_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRetReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RET_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvStoHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_STOHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRetHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RETHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvTrPause.SetIcon((m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	
	m_btnCvDiverterHsDown.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DEVERTER_HS_DOWN_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvDiverterHsUp.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DEVERTER_HS_UP_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvDoorOpen.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DOOR_STATUS_RD== _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnRgvStatusLoad.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RGV_STA_LOAD_RD== _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnScPaPkPossible.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SC_PA_PK_POSSIBLE_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvDoorOpenReq.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DOOR_OPEN_REQ_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvDoorCloseReq.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DOOR_CLOSE_REQ_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvMoter1Run.SetIcon((m_pTrackInfo->m_pCV_DATA->V_MTR1_RUN_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvMoter2Run.SetIcon((m_pTrackInfo->m_pCV_DATA->V_MTR2_RUN_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvSizeCheckerLow.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SZ_CHK_LOW_RD== _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvSizeCheckerHigh.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SZ_CHK_HIGH_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRetReadySta.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SRT_READY_STA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvPicking4ByPass.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PICK4_PASS_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));


	//m_btnCvRtvDepartHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RTV_DEPARTHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
//m_btnCvRtvArriveHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RTV_ARRIVEHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
//m_btnCvRtvLockSign.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RTV_LOCK_SENSOR == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));

	CString strSql;
	strSql = GetQrySelectJOB_MST(m_pTrackInfo->m_pCV_DATA);
	nRowCnt = -1;
	_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0)
	{
		m_edtCvJobJobNo.SetWindowText(_T("N"));
		m_edtCvJobStartPos.SetWindowText(_T("N"));
		m_edtCvJobDestPos.SetWindowText(_T("N"));
		m_edtCvJobJobTyp.SetWindowText(_T("N"));
		m_edtCvJobJobStatus.SetWindowText(_T("N"));
		m_edtCvJobPalletNumber.SetWindowText(_T("N"));
		m_edtCvJobProductId.SetWindowText(_T("N"));
		m_edtJobStartLoc.SetWindowText(_T("N"));
		m_edtJobDestLoc.SetWindowText(_T("N"));

		return;	  
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);

	pRsw->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		m_edtCvJobJobNo.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));
		CString strStartPos = pRsw->GetItem(_T("START_POS"));
		CString strDestPos = pRsw->GetItem(_T("DEST_POS"));
		if(strStartPos == _T("00000")) {strStartPos = pRsw->GetItem(_T("START_LOCATION")); };
		if(strDestPos == _T("00000")) {strDestPos = pRsw->GetItem(_T("DEST_LOCATION")); };
		m_edtCvJobStartPos.SetWindowText(strStartPos);
		m_edtCvJobDestPos.SetWindowText(strDestPos);
		m_edtJobStartLoc.SetWindowText(pRsw->GetItem(_T("START_LOCATION")));
		m_edtJobDestLoc.SetWindowText(pRsw->GetItem(_T("DEST_LOCATION")));
		m_edtCvJobJobTyp.SetWindowText(pRsw->GetItem(_T("JOB_TYP")));
		m_edtCvJobJobStatus.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
		m_edtCvJobPalletNumber.SetWindowText(pRsw->GetItem(_T("BCR_BOTTOM")));
		m_edtCvJobProductId.SetWindowText(pRsw->GetItem(_T("PRODUCT_ID")));
	
		pRsw->MoveNext();
	}

	delete pRsw;

	//InvalidateLangControl(pLang);

}

void CCvSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;
}

void CCvSkinDlg::InvalidateComboBoxData(EN_LANG enLangTemp, CComboBox* pComboBox, CString pTemp)
{
	pComboBox->InsertString(0,pTemp);
}

void CCvSkinDlg::OnClose()
{
	m_pDoc->m_pCvSkinDlg = NULL;
	delete this;
}

//CV쓰기
void CCvSkinDlg::OnBnClickedBtnCvWrite()
{
	UpdateTrackData(EN_BtnWrite);
}
	
//CV삭제
void CCvSkinDlg::OnBnClickedBtnCvDelete()
{
	UpdateTrackData(EN_BtnDelete);	
}

//CV복사
void CCvSkinDlg::OnBnClickedBtnCvCopy()
{
	UpdateTrackData(EN_BtnCopy);	
}

//CV잘라내기
void CCvSkinDlg::OnBnClickedBtnCvCut()
{
	UpdateTrackData(EN_BtnCut);	
}

//CV붙여넣기
void CCvSkinDlg::OnBnClickedBtnCvPaste()
{
	UpdateTrackData(EN_BtnPaste);
}

void CCvSkinDlg::SetButtonCvImage( CButton& pButton, CString pFieldName, int pValue )
{
	HICON hIcon = Global.GetIcon(CGlobal::ICO_CV_ON);
	pButton.SetIcon(hIcon);
}

//Wait Time 10초 Up
void CCvSkinDlg::OnBnClickedBtnCvWaitTimeUp()
{
	UpdateData(TRUE);

	CString strCvWaitTimeRd = _T("");
	CString strCvWaitTimeOd = _T("");

	m_edtCvWaitTimeOd.GetWindowText(strCvWaitTimeOd);

	int nCvWaitTimeOd = CConvert::ToInt(strCvWaitTimeOd) + 10;

	m_edtCvWaitTimeOd.SetWindowText(CConvert::ToString(nCvWaitTimeOd));

	UpdateData(FALSE);

	return;
}

//Wait Time 10초 Up
void CCvSkinDlg::OnBnClickedBtnCvWaitTimeDown()
{
	UpdateData(TRUE);

	CString strCvWaitTimeRd = _T("");
	CString strCvWaitTimeOd = _T("");

	m_edtCvWaitTimeOd.GetWindowText(strCvWaitTimeOd);

	int nCvWaitTimeOd = CConvert::ToInt(strCvWaitTimeOd) - 10;
	
	if (nCvWaitTimeOd < 0) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("0 보다 작을 수 없습니다.")));
		return;
	}

	m_edtCvWaitTimeOd.SetWindowText(CConvert::ToString(nCvWaitTimeOd));

	UpdateData(FALSE);

	return;
}

//CV_DATA 바코드 상단버튼 클릭
void CCvSkinDlg::OnBnClickedBtnCvBcrTopTransfer()
{
	CString strWhTyp = _T("");
	CString strPLC_NO = _T("");
	CString strBcrTop = _T("");
	CString strSql= _T("");
	CString strTrackNo;
	CString strPlcNo;
	CStringList strCvDataList;
	CString strUserId;
	CString strMessage;

	UpdateData(TRUE);

	m_edtCvTrackNo.GetWindowText(strTrackNo);

	strWhTyp = m_pDoc->m_WH_TYP;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	if ((strBcrTop == "")) 
	{
		AfxMessageBox(m_pDoc->m_pLang->GetLangValue(_T("바코드 형식을 확인해주세요."), m_pDoc->m_enLang));
		return;
	}
	
	strSql=_T("");

	strSql.Format(_T(" UPDATE CV_DATA									\n")
	_T("				  SET BCR_TOP = '%s'						    \n")
	_T("				     ,WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
	_T("                WHERE WH_TYP = '%s'							    \n")
	_T("	              AND PLC_NO = '%02s'							\n")
	_T("                  AND TRACK_NO = '%05s'							  "), strBcrTop, strWhTyp, strPlcNo, strTrackNo);
	

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
	return;
}

//CV_DATA 바코드 하단버튼 클릭
void CCvSkinDlg::OnBnClickedBtnCvBcrBottomTransfer()
{
 	CString strWhTyp = _T("");
	CString strPLC_NO = _T("");
	CString strBcrBottom = _T("");
	CString strSql= _T("");
	CString strTrackNo;
	CString strPlcNo;
	CStringList strCvDataList;
	CString strUserId;
	int nCntSel = 0;
	CString strMessage = _T("");

	UpdateData(TRUE);

	m_edtCvBcrBottomUser.GetWindowText(strBcrBottom);
	m_edtCvTrackNo.GetWindowText(strTrackNo);

	strWhTyp = m_pDoc->m_WH_TYP;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	if ((strBcrBottom == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql=_T("");

	strSql.Format(_T(" UPDATE CV_DATA									\n")
	_T("			      SET BCR_BOTTOM = '%s'						    \n")
	_T("			        , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
	_T("                WHERE WH_TYP = '%s'						        \n")
	_T("                      AND PLC_NO = '%02s'						\n")
	_T("                  AND TRACK_NO = '%03s'							  "), strBcrBottom, strWhTyp, strPlcNo, strTrackNo);
	

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
	return;
}

//JOB_MST 바코드 상단 버튼클릭
void CCvSkinDlg::OnBnClickedBtnJobBcrTopTransfer()
{
	CString strBcrTop = _T("");
	CString strSql= _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	m_edtCvTrackNo.GetWindowText(strTrackNo);
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;

	m_edtCvJobBcrTopUser.GetWindowText(strBcrTop);

	if ((strBcrTop == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql.Format(_T("SELECT * FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%04s'"), strWhTyp, strJobNo);

	nRowCount = m_pDoc->GetSelectQryCnt_DLG(strSql);

 	if (nRowCount < 1)
 	{
 		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호가 존재하지 않습니다. 작업번호를 확인해주세요.")));
 		return;
 	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE JOB_MST				\n")
	_T("    SET BCR_TOP = '%s'					    \n")
	_T("      , UPD_USER_ID = '%s'				    \n")
	_T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("		\n")
	_T("	WHERE WH_TYP = '%s'						\n")
	_T("    AND LUGG_NO = '%04s'					  "), strBcrTop, strUserId, strWhTyp, strJobNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
	return;


}
//JOB_MST 바코드 하단 버튼클릭
void CCvSkinDlg::OnBnClickedBtnJobBcrBottomTransfer()
{
	CString strBcrBottom = _T("");
	CString strSql= _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	m_edtCvTrackNo.GetWindowText(strTrackNo);
	strPlcNo = strTrackNo.Mid(0,2);
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;

	m_edtCvJobBcrBottomUser.GetWindowText(strBcrBottom);

	if ((strBcrBottom == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql.Format(_T("SELECT * FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%04s'"), strWhTyp, strJobNo);

	nRowCount = m_pDoc->GetSelectQryCnt_DLG(strSql);

	if (nRowCount < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호가 존재하지 않습니다. 작업번호를 확인해주세요.")));
		return;
	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE JOB_MST							\n")
				  _T("    SET BCR_BOTTOM = '%s'					\n")
				  _T("      , UPD_USER_ID = '%s'				\n")
				  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("	\n")
				  _T("	WHERE WH_TYP = '%s'						\n")
				  _T("    AND LUGG_NO = '%04s'					  "), strBcrBottom, strUserId, strWhTyp, strJobNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
	return;
}

void CCvSkinDlg::UpdateTrackData(int pBtnJob)
{
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	CString strPLC_NO = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strTemp = _T("");
	CString strJobNo = _T("");
	CString strDestPos = _T("");
	CString strJobTyp = _T("");
	CString strFmsRptOd = _T("");
	CString strBarcode = _T("");
	CString strErrorCode = _T("");
	CString strSql=_T("");
	CString CRLF = _T("\r\n");
	CString strLOG_LUGG_NO_RD = m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD;
	CString strLOG_JOB_TYP_RD = m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD;
	CString strLOG_DEST_POS_RD = m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD;

	CStringList strManualSeqList;
	CStringList strJobMstList;
	
	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();

	int nRowCount = 0;

	UpdateData(TRUE);

	m_edtCvJobNo.GetWindowText(strJobNo);
	//CLib::GetComBoBoxData(m_cbxCvDestPos, strDestPos, 5);
	strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

	//CLib::GetComBoBoxData(m_cbxCvJobTyp, strJobTyp, 1);
	m_cbxCvDestPos.GetWindowText(strDestPos);
	CString strDestPosSimple = strDestPos.Mid(0, 3);
	//m_cbxCvJobTyp.GetWindowText(strJobTyp);
	//m_cbxCvFmsRpt.GetWindowText(strFmsRptOd);
	//m_cbxCvTrayTyp.GetWindowText(strTrayTyp);
	//MES I/F STA - 확인필요
	m_edtCvErrorCode.GetWindowText(strErrorCode);

	//Edit Data 자릿수 Check
 	if (IsValidataEditData(strJobNo, strDestPosSimple,strJobTyp) == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Checking Data")));
		return;
	}

	m_pDoc->BeginTrans_DLG();

	switch (pBtnJob)
	{
	case EN_BtnWrite :
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업을 적으시겠습니까?")), MB_YESNO) != IDYES)
				break;

			if (strJobNo == _T("")){ strJobNo = _T("0");}
			if (strDestPos == _T("")){ strDestPos = _T("0"); }
			if (strJobTyp == _T("")){ strJobTyp=_T("0"); }
			if (strBarcode == _T("")){ strBarcode =_T("0");}
			if (strErrorCode == _T("") ){strErrorCode=_T("0");}

			strSql.Format(_T(" UPDATE CV_DATA									\n")
			_T("				  SET LUGG_NO_OD = '%s'							\n")
			_T("					 ,DEST_POS_OD = '%s'						\n")
			_T("					 ,JOB_TYP_OD = '%s'							\n")
			_T("					 ,WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
			_T("					 ,OD_RQ_YN = 'Y'							\n")
			_T("				WHERE WH_TYP = '%s'								\n")
			_T("				  AND PLC_NO = '%02s'							\n")
			_T("				  AND MC_NO = '%s'								\n")
			_T("				  AND OD_RQ_YN = 'N'							\n"), strJobNo, strDestPosSimple, strJobTyp, strWhTyp, strPLC_NO, strTrackNo);

 			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				CString strLOG_LUGG_NO = strJobNo;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BARCODE = strBarcode;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 쓰기 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, strJobNo, strLOG_JOB_TYP_RD, strJobTyp, strLOG_DEST_POS_RD, strDestPos);
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}
				
				CString strTemp = _T("");
				SetCvStatus(strJobNo, strJobTyp, strDestPosSimple, _T(""), _T("WRITE"));
				
				int nRowCnt = 0;
				strSql = _T("");
				CString strMessage = _T("");

				strSql.Format(_T("  SELECT *						")
						  _T("	  FROM JOB_MST						")
						  _T("	 WHERE WH_TYP = '%s'				")
						  _T("	   AND LUGG_NO = '%s'				"), strWhTyp, strJobNo);

				_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

				pRsw->MoveFirst(); 

				delete pRsw;

				//조회건수가 있다 -> 이때 작업정보 변경
				if(nRowCnt > 0)
				{
					//// 작업정보도 CV 구동중으로 변경할것!
					strSql.Format(_T(" UPDATE JOB_MST								\n")
								  _T("    SET JOB_STATUS = '11'						\n")
								  _T("	    , UPD_USER_ID = '%s'	    			\n")
								  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
								  _T("  WHERE WH_TYP = '%s'							\n")
								  _T("	  AND LUGG_NO = '%s'						\n"), m_pDoc->m_strId, strWhTyp, strJobNo);
					
					BOOL isSuccess2 =  m_pDoc->ExcuteQueryString_DLG(strSql);

					strTemp = isSuccess2 ? _T("!!") : _T("!");
					
					if (isSuccess2 == FALSE)
					{
						m_pDoc->RollbackTrans_DLG();
						AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 변경 실패")));
						return;
					}
					
					m_pDoc->CommitTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 쓰기 및 작업정보 변경 성공")) + strTemp);
					return;
				}
				else
				{
					m_pDoc->CommitTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 쓰기 성공")) + strTemp);
					return;
				}
			}

			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 쓰기 실패")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;

	case EN_BtnDelete :
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙의 작업을 삭제하시겠습니까?")), MB_YESNO) != IDYES)
				break;

			strSql=_T("");

			strSql.Format(_T(" UPDATE CV_DATA									\n")
					_T("		  SET LUGG_NO_OD = '0'							\n")
					_T("			 ,DEST_POS_OD = '0'							\n")
					_T("			 ,JOB_TYP_OD = '0'							\n")
					_T("			 ,WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
					_T("			 ,OD_RQ_YN = 'Y'							\n")
					_T("		WHERE WH_TYP = '%s'								\n")
					_T("		  AND PLC_NO = '%02s'							\n")
					_T("		  AND MC_NO = '%s'								\n")
					_T("		  AND OD_RQ_YN = 'N'							\n"), strWhTyp, strPLC_NO, strTrackNo);
			
			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				//CTrackInfo* pTrackInfo = m_pDoc->pCV-m_pInfo->m_MapTrackInfo[strTrackNo];
				//CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;

				CString strLOG_LUGG_NO = m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BARCODE = m_pTrackInfo->m_pCV_DATA->V_BARCODE;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 삭제 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, _T("0"), strLOG_JOB_TYP_RD, _T("0"), strLOG_DEST_POS_RD, _T("0"));
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}

				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 삭제 성공")));
				SetCvStatus(_T("0000"), _T("0"), _T("000"), _T(""), _T("WRITE"));
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 삭제 실패")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;

	case EN_BtnCopy :
		{
			m_strCopyJobNo=_T("");
			m_strCopyDestPos=_T("");
			m_strCopyJobTyp=_T("");
			m_strCopyBcrTop=_T("");
			m_strCopyBcrBottom=_T("");
			m_strCopyPulpSensor=_T("");
			
			m_edtCvJobNo.GetWindowText(strJobNo);
			strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

			pCopyJob->LUGG_NO=strJobNo;
			pCopyJob->DEST_POS=strDestPos;
			pCopyJob->JOB_TYP=strJobTyp;
			pCopyJob->BCR_TOP = strBarcode;
			pCopyJob->BCR_BOTTOM = strBarcode;

			pCopyJob->COPY_YN = true;
			GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);

		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 복사 성공")));
		break;

	case EN_BtnCut :
		{
			m_strCopyJobNo=_T("");
			m_strCopyDestPos=_T("");
			m_strCopyJobTyp=_T("");
			m_strCopyBcrTop=_T("");
			m_strCopyBcrBottom=_T("");

			m_edtCvJobNo.GetWindowText(strJobNo);
			strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

			pCopyJob->LUGG_NO=strJobNo;
			pCopyJob->DEST_POS=strDestPos;
			pCopyJob->JOB_TYP=strJobTyp;
			pCopyJob->BCR_TOP = strBarcode;
			pCopyJob->BCR_BOTTOM = strBarcode;

			strSql=_T("");

			strSql.Format(_T(" UPDATE CV_DATA									\n")
						_T("      SET LUGG_NO_OD = '0'							\n")
						_T("  	     ,DEST_POS_OD = '0'							\n")
						_T("  	     ,JOB_TYP_OD = '0'							\n")
						_T("         ,WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
						_T("         ,OD_RQ_YN = 'Y'							\n")
						_T("    WHERE WH_TYP = '%s'								\n")
						_T("   	  AND PLC_NO = '%02s'							\n")
						_T("      AND MC_NO = '%s'								\n")
						_T("      AND OD_RQ_YN = 'N'							\n"), strWhTyp, strPLC_NO, strTrackNo);


			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 잘라내기 성공")));
				SetCvStatus(_T("0000"), _T("0"), _T("000"),_T(""), _T("WRITE"));
				pCopyJob->COPY_YN = true;
				GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 잘라내기 실패")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;
	case EN_BtnErrReset:
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 에러를 리셋하시겠습니까?")), MB_YESNO) != IDYES)
				break;

			CString strCMD_RQ_ID = _T("RESET");

			m_pDoc->BeginTrans_DLG();

			strSql=_T("");
			strSql.Format(_T(" UPDATE CV_DATA									\n")
				          _T("    SET CMD_RQ_ID = '%s'							\n")
				          _T("       ,CMD_RQ_YN = 'Y'							\n")
				          _T("  WHERE WH_TYP = '%s'								\n")
				          _T("	  AND PLC_NO = '%02s'							\n")
				          _T("    AND MC_NO = '%s'								\n")
				          _T("    AND OD_RQ_YN = 'N'							\n"),  strCMD_RQ_ID, strWhTyp, strPLC_NO, strTrackNo);

 			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 에러 리셋 성공")));
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 에러 리셋 실패")));
			return;
		}
		break;
	case EN_BtnPaste :
		{
			strJobNo = _T("");
			strDestPos = _T("");
			strJobTyp = _T("");
			
			if (pCopyJob->LUGG_NO == _T("") ){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호 없습니다"))); return; }

			strSql.Format(_T(" UPDATE CV_DATA		\n")
				_T("		SET LUGG_NO_OD = '%s'	\n")
				_T("	       ,DEST_POS_OD = '%s'	\n")
				_T("	       ,JOB_TYP_OD = '%s'	\n")
				_T("	       ,BARCODE = '%s'		\n")
				_T("           ,OD_RQ_YN = 'Y'		\n")
				_T("	       ,TR_PAUSE_OD = '0'	\n")
				_T("	       ,ERR_RQ_OD = '0'		\n")
				_T("      WHERE WH_TYP = '%s'		\n")
				_T("  	    AND PLC_NO = '%02s'		\n")
				_T("        AND MC_NO = '%s'		\n")
				_T("        AND OD_RQ_YN = 'N'		  "), pCopyJob->LUGG_NO, pCopyJob->DEST_POS, pCopyJob->JOB_TYP,
														pCopyJob->BCR_BOTTOM, strWhTyp, strPLC_NO, strTrackNo);

			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{

				CString strLOG_LUGG_NO = strJobNo;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BARCODE = strBarcode;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 붙여넣기 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, pCopyJob->LUGG_NO, strLOG_JOB_TYP_RD, pCopyJob->JOB_TYP, strLOG_DEST_POS_RD, pCopyJob->DEST_POS);
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}
				
				CString strTemp = _T("");
				SetCvStatus(pCopyJob->LUGG_NO, pCopyJob->JOB_TYP, pCopyJob->DEST_POS, _T(""), _T("WRITE"));
				pCopyJob->COPY_YN = false;
				GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(FALSE);
				//UpdateData()

				int nRowCnt = 0;
				strSql = _T("");
				CString strMessage = _T("");

				strSql.Format(_T("  SELECT *						")
						  _T("	  FROM JOB_MST						")
						  _T("	 WHERE WH_TYP = '%s'				")
						  _T("	   AND LUGG_NO = '%s'				"), strWhTyp, pCopyJob->LUGG_NO);

				_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

				pRsw->MoveFirst(); 

				delete pRsw;

				//조회건수가 있다 -> 이때 작업정보 변경
				if(nRowCnt > 0)
				{
					//// 작업정보도 CV 구동중으로 변경할것!
					strSql.Format(_T(" UPDATE JOB_MST								\n")
								  _T("    SET JOB_STATUS = '11'						\n")
								  _T("	    , UPD_USER_ID = '%s'	    			\n")
								  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
								  _T("  WHERE WH_TYP = '%s'							\n")
								  _T("	  AND LUGG_NO = '%s'						\n"), m_pDoc->m_strId, strWhTyp, pCopyJob->LUGG_NO);
					
					BOOL isSuccess2 =  m_pDoc->ExcuteQueryString_DLG(strSql);

					strTemp = isSuccess2 ? _T("!!") : _T("!");
					
					if (isSuccess2 == FALSE)
					{
						m_pDoc->RollbackTrans_DLG();
						AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 변경 실패")));
						return;
					}

					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 붙여넣기 및 작업정보 변경 성공")) + strTemp);

					m_pDoc->CommitTrans_DLG();
					return;
				}
				else
				{
					m_pDoc->CommitTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 붙여넣기 성공")) + strTemp);
					return;
				}
			}

			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CV 데이터 붙여넣기 실패")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;
	}
}

void CCvSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

BOOL CCvSkinDlg::IsValidataEditData(CString strJobNo, CString strDestPos, CString strJobTyp)
{
	int a = strJobNo.GetLength();

	if (strJobNo.GetLength() > 5){return FALSE;}
	if (strDestPos.GetLength() > 6){return FALSE;}

	return TRUE;
}

void CCvSkinDlg::OnBnClickedBtnCvSuspend()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = _T("");
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strLOG_MSG = _T("");
	CString strTrPauseRd = _T("");

	UpdateData(TRUE);

	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	if (m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD == "1")
		strTrPauseRd = "0";
	else strTrPauseRd = "1";

	strSql.Format(_T(" UPDATE CV_DATA					                \n")
		_T("		      SET TR_PAUSE_OD = '%s'		                \n")
		_T("				 ,CMD_RQ_ID = 'SUSPEND'			                \n")
		_T("				 ,CMD_RQ_YN = 'Y'			                \n")
		_T("		       WHERE WH_TYP = '%s'						\n")
		_T("			  AND PLC_NO = '%s'								\n")
		_T("			  AND MC_NO = '%s'								  "), strTrPauseRd, strWhTyp, strPlcNo, strTrackNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("트랙 일시정지 설정 -> TRACK NO : %s , 일시정지 : %s"), strTrackNo, strTrPauseRd);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), strLOG_MSG))
		{
			return;
		};
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙 일시정지 실패")));
	return;	
}

void CCvSkinDlg::OnBnClickedBtnCvActive()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = _T("");
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	UpdateData(TRUE);

	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙을 활성화시키겠습니까?")), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE CV_DATA							 \n")
		_T("       SET SUSPEND = '0'							 \n")
		_T("     WHERE WH_TYP = '%s'							 \n")
		_T("	   AND PLC_NO = '%02s'							 \n")
		_T("       AND MC_NO = '%s'								   "), strWhTyp, strPlcNo, strTrackNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙 활성화 지시 성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙 활성화 지시 실패")));
	return;	
}

void CCvSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;

	strSql.Format(_T(" SELECT MSG_KOR, ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN   ")
	_T("    FROM EQP_ECD_MST						 ")
	_T("   WHERE EQP_TYP      = '%s'                 ")
	_T("    AND EQP_ERR_CD   = '%04s'                "), strEqpTyp, strErrorCode);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	strMSG_KOR = pRsw->GetItem(_T("MSG_KOR"));
	strMSG_CHIN = pRsw->GetItem(_T("MSG_CHIN"));
	strMSG_ENG = pRsw->GetItem(_T("MSG_ENG"));
	strMSG_HUN = pRsw->GetItem(_T("MSG_HUN"));
	if(nEN_LANG == 0)
	{
		strGetErrorCode.Format(_T("[%04s]%s"), strErrorCode, strMSG_KOR);
	}
	else if(nEN_LANG == 1)
	{
		strGetErrorCode.Format(_T("[%04s]%s"), strErrorCode, strMSG_ENG);
	}
	else if(nEN_LANG == 2)
	{
		strGetErrorCode.Format(_T("[%04s]%s"), strErrorCode, strMSG_HUN);
	}
	else if(nEN_LANG == 3)
	{
		strGetErrorCode.Format(_T("[%04s]%s"), strErrorCode, strMSG_CHIN);
	}

	delete pRsw;
}

int CCvSkinDlg::GetDP_YN(CString pWH_TYP, CString pTrack)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;
	strSql.Format(_T(" SELECT *   ")
		_T("    FROM CV_DATA                     ")
		_T("   WHERE WH_TYP      = '%s'               ")
		_T("    AND MC_NO = '%s'               ")
		_T("    AND DP_HS_YN   = 'Y'               "), pWH_TYP, pTrack);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	delete pRsw;

	return nRowCnt;
}

void CCvSkinDlg::SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx, CString strGroup_No, CString strDEST_POS_RD)
{
	if(m_pDoc == NULL){return;};

	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어

	CStringList strList;
	CString strSql;
	CString strTRACK_NO, strREMARKS;
	int nRowCnt = 0, j=0;
	CString strDEST_POS;
	CString strMessage;
	cbx.ResetContent();
	strSql.Format(_T("  SELECT TRACK_NO						")
				  _T("       , PRIORITY						") 
				  _T("       , REMARKS						") 
				  _T("       , GROUP_NO						")
				  _T("       , MC_NO						")
				  _T("	  FROM DEST_POS_DEF					")
				  _T("ORDER BY GROUP_NO, TRACK_NO, PRIORITY	"));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	/*
	cbx.SetItemDataEx(0, _T("000"));
	cbx.AddString(_T("000"));*/

	pRsw->MoveFirst(); 
	
	for(int i = 1; i <= nRowCnt; i++)
	{
		strTRACK_NO = pRsw->GetItem(_T("MC_NO"));		
		strREMARKS = pRsw->GetItem(_T("REMARKS"));		
		strDEST_POS = strTRACK_NO +_T(" - ") + strREMARKS;
		cbx.AddString(strDEST_POS);
		cbx.SetItemData(i, CConvert::ToInt(strTRACK_NO));

		if (strTRACK_NO == strDEST_POS_RD)
		{
			cbx.SetCurSel(i);
		}

		pRsw->MoveNext();
	}

	if (cbx.GetCurSel() == -1)
	{
		cbx.SetWindowText(strDEST_POS_RD);
	}

	delete pRsw;
}

void CCvSkinDlg::SetBindCombo_COMMON_CODE(CComboBox& cbx, CString strCDX_CD, CString strCCD_NM)
{
	CStringList strList;
	CString strSql;
	CString strCCD_CD, strCCD_NM_KOR;
	int nRowCnt = 0, j=0;
	cbx.ResetContent();
	CString strMessage;
	strSql.Format(_T(" SELECT CCD_CD, CCD_NM_KOR	")
		_T("	 FROM COMMON_CODE					")
		_T("	WHERE CCD_CD_YN = 'Y'				")
		_T("    AND CDX_CD = '%s'					")
		_T("	ORDER BY ") + m_pDoc->TO_NUMBER(_T("CCD_CD")) + _T(	")", strCDX_CD));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	for(int i = 0; i < nRowCnt; i++)
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
	int cc =cbx.GetCurSel();
	if (cc == -1)
	{
		cbx.SetWindowText(strCCD_NM);
	}

 	delete pRsw;
}

void CCvSkinDlg::GetComBoBoxData(CComboBox& cbx, CString& strVal, int nlength = 0)
{
	int nCurSel = cbx.GetCurSel();
	if (nCurSel == -1)
	{
		cbx.GetWindowText(strVal);
		return;
	}
	
	DWORD bItemDate = cbx.GetItemData(nCurSel);
	TCHAR szBuf[10]= {0};
	switch(nlength)
	{
	case 1:
		wsprintf(szBuf,_T("%ld"), bItemDate);
	case 5:
		wsprintf(szBuf,_T("%5d"), bItemDate);
	}
	strVal = (LPCTSTR)szBuf;
	strVal = strVal.Trim();
	/*
	cbx.GetWindowText(strVal);

	strVal = strVal.Left(nlength);*/
}

void CCvSkinDlg::OnBnClickedBtnCvErrReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateTrackData(EN_BtnErrReset);

}

CString CCvSkinDlg::GetQrySelectJOB_MST( CCV_DATA* pCV_DATA )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.WH_TYP,'10') AS WH_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.START_POS,'00000') AS START_POS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.START_LOCATION,'00-000-00') AS START_LOCATION ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.DEST_POS,'00000') AS DEST_POS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.DEST_LOCATION,'00-000-00') AS DEST_LOCATION ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'N') AS JOB_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM,'N') AS BCR_BOTTOM ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR,'N') AS JOB_STATUS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID,'N') AS PRODUCT_ID ");
	strSql += CRLF + _T("  FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS ");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE JM.WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pCV_DATA->V_LUGG_NO_RD);
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CCvSkinDlg::OnEnterSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSkinDialog::OnEnterSizeMove(0,0);
}

void CCvSkinDlg::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSkinDialog::OnExitSizeMove(0,0);
	Invalidate(TRUE);
}

void CCvSkinDlg::SetCvStatus(CString pLUGG_NO, CString pJOB_TYP, CString pDEST_POS, CString pTRACK_PAUSE, CString pCMD)
{
	CString strCcdNmKor = _T("");
	if (pCMD == _T("WRITE"))
	{
		m_cbxCvJobTyp.SetCurSelEx(pJOB_TYP);
		m_edtCvJobNo.SetWindowText(pLUGG_NO);
		SetBindCombo_DEST_POS_DEF(m_cbxCvDestPos, m_pTrackInfo->m_pCV_DATA->K_PLC_NO, pDEST_POS);
	}
}

void CCvSkinDlg::SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T("  SELECT CCD_NM_KOR					")
				  _T("	  FROM COMMON_CODE					")
				  _T("	 WHERE CDX_CD = '%s'				")
				  _T("	   AND CCD_CD = '%s'				"), pCMD, pCCD_CD);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	pCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));	

	delete pRsw;
}

void CCvSkinDlg::OnBnClickedChkAutoSel()
{
	int nChk = m_chkAutoSel.GetCheck();

	if(nChk == 1)
	{
		if (m_blAutoSel == false)
		{
			SetTimer(1,1500,NULL);
			m_blAutoSel = true;
		}
		else
		{
			return;
		}
	}
	else
	{
		KillTimer(1);
		m_blAutoSel = false;
	}
	return;
}

void CCvSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == TRUE)
	{
		//조회
		InvalidateTrackData(EN_KOR);
	}
}

void CCvSkinDlg::OnBnClickedBtnMzMove()
{
	CString strSql = _T("");
	CString strWH_TYP = m_pTrackInfo->m_pCV_DATA->K_WH_TYP;
	CString strPLC_NO = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strMC_NO = m_pTrackInfo->m_pCV_DATA->V_MC_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("매거진(108번)으로 이동시키겠습니까?")), MB_YESNO) != IDYES)
			return;

	m_pDoc->BeginTrans_DLG();

	strSql.Format(_T(" UPDATE CV_DATA					  \n")
	_T("    SET LUGG_NO_OD = '9999'						  \n")
	_T("	  , DEST_POS_OD = '108'						  \n")
	_T("	  , JOB_TYP_OD = '6'						  \n")
	_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("\n")
	_T("      , OD_RQ_YN = 'Y'							  \n")
	_T("  WHERE WH_TYP = '%s'							  \n")
	_T("	AND PLC_NO = '%02s'							  \n")
	_T("    AND MC_NO = '%s'							  \n")
	_T("    AND OD_RQ_YN = 'N'							  \n"),  strWH_TYP, strPLC_NO, strMC_NO);

 	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = _T("9999");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("MZ 이동 지시 -> 트랙번호 : %s , 작업구분 : %s , 도착지 : %s"), strMC_NO, _T("6"), _T("108"));
		if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO, _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			InvalidateTrackData(m_nLang);
			return;
		}
		
		CString strTemp = _T("");
		SetCvStatus(_T("9999"), _T("6"), _T("108"),_T(""), _T("WRITE"));

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("매거진(108번)으로 이동 성공")));
		return;
	}

	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("매거진(108번)으로 이동 실패")));
	InvalidateTrackData(m_nLang);
	return;
}

void CCvSkinDlg::OnBnClickedChkAutoSel2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nChk = m_chkDebugMode.GetCheck();

	if(nChk == 1)
	{
		m_pDoc->m_bDebugMode = TRUE;
	}
	else
	{
		m_pDoc->m_bDebugMode = FALSE;
	}
	return;
}

void CCvSkinDlg::OnBnClickedChkTrPause()
{
	int nChk = m_chkTrPause.GetCheck();

	CString strSql = _T("");
	CString strTrackPause = _T("");
	CString strWhTyp = _T("");
	CString strTrackNo = _T("");
	CString strJobNo = _T("");
	CString strPlcNo = _T("");
	CString strUserId = _T("");
	CString strTrPauseRd = _T("");
	CString strLOG_MSG = _T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	//m_edtCvTrackNo.GetWindowText(strTrackNo);
	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;
	strTrackPause = m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD;

	if (nChk == 1)
		strTrPauseRd = _T("1");
	else strTrPauseRd = _T("0");

	strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT COUNT(1) AS CNT    		 \n")
		_T("   FROM CV_DATA					 \n")
		_T("	WHERE WH_TYP = '%s'				 \n")
		_T("	  AND PLC_NO = '%s'				 \n")
		_T("	  AND MC_NO = '%s'				 \n")
		_T("	  AND STN_KIND IS NOT NULL		   "), strWhTyp, strPlcNo, strTrackNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	CString strCNT = pRsw->GetItem(_T("CNT"));
	delete pRsw;

	if (strCNT == "1")
	{
		strSql = _T("");

		strSql.Format(_T(" UPDATE CV_DATA					                \n")
			_T("		      SET TR_PAUSE_OD = '%s'		                \n")
			_T("				 ,CMD_RQ_ID = 'SUSPEND'			                \n")
			_T("				 ,CMD_RQ_YN = 'Y'			                \n")
			_T("				  WHERE WH_TYP = '%s'						\n")
			_T("			  AND PLC_NO = '%s'								\n")
			_T("			  AND MC_NO = '%s'								  "), strTrPauseRd, strWhTyp, strPlcNo, strTrackNo);


		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == TRUE)
		{
			strLOG_MSG.Format(_T("트랙 일시정지 설정 -> TRACK NO : %s , 일시정지 : %s"), strTrackNo, strTrPauseRd);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), strLOG_MSG))
			{
				//m_pDoc->RollbackTrans_DLG();
				return;
			};
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙 일시정지 설정 실패")));
		return;
	}
	else
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙은 일시정지를 할 수 없습니다.")));
		return;
	}
}

void CCvSkinDlg::OnBnClickedBtnStatusRethsReady()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CCvSkinDlg::OnBnClickedBtnCvDischarge()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = _T("");
	CString strDestTrackNo = m_pTrackInfo->m_pCV_DATA->V_COMMING_DEST_TR;
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	UpdateData(TRUE);

	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("팔렛트를 배출하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE CV_DATA									\n")
		_T("		      SET LUGG_NO_OD = '9999'						\n")
		_T("				 ,DEST_POS_OD = '%03s'						\n")
		_T("            WHERE WH_TYP = '%s'								\n")
		_T("	          AND PLC_NO = '%02s'							\n")
		_T("              AND MC_NO = '%s'								  "), strDestTrackNo, strWhTyp, strPlcNo, strTrackNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("팔렛트 배출 지시 성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("팔렛트 배출 지시 실패")));
	return;
}


