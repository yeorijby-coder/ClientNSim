

// ScSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "ScSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CScSkinDlg 대화 상자입니다.0

IMPLEMENT_DYNAMIC(CScSkinDlg, CSkinDialog)

CScSkinDlg::CScSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pSC_DATA = NULL;
	m_pDB = NULL;
	m_pScManualRet = NULL;
}
CScSkinDlg::CScSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pSC_DATA = NULL;
	m_pDB = NULL;
	m_pScManualRet = NULL;
}

CScSkinDlg::~CScSkinDlg()
{
	m_pDoc->m_pScSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CScSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//BASE BUTTON
	DDX_Control(pDX, ID_BTN_SC_CANCEL, m_btnScCancel);
	DDX_Control(pDX, ID_BTN_SC_OK, m_btnScOk);

	//SC작업정보
	DDX_Control(pDX, IDC_LBL_SC_JOB_NO_FORK1, m_lblScJobNoFork1);
	DDX_Control(pDX, IDC_LBL_SC_JOB_TYP_FORK1, m_lblScJobTypFork1);
	DDX_Control(pDX, IDC_LBL_SC_START_POS_FORK1, m_lblScStartPosFork1);
	DDX_Control(pDX, IDC_LBL_SC_DEST_POS_FORK1, m_lblScDestPosFork1);
	DDX_Control(pDX, IDC_LBL_SC_JOB_NO_FORK2, m_lblScJobNoFork2);
	DDX_Control(pDX, IDC_LBL_SC_JOB_TYP_FORK2, m_lblScJobTypFork2);
	DDX_Control(pDX, IDC_LBL_SC_START_POS_FORK2, m_lblScStartPosFork2);
	DDX_Control(pDX, IDC_LBL_SC_DEST_POS_FORK2, m_lblScDestPosFork2);

	DDX_Control(pDX, IDC_EDT_SC_JOB_NO_FORK1, m_edtScJobNoFork1);
	DDX_Control(pDX, IDC_EDT_SC_JOB_TYP_FORK1, m_edtScJobTypFork1);
	DDX_Control(pDX, IDC_EDT_SC_START_LOC_FORK1, m_edtScStartLocFork1);
	DDX_Control(pDX, IDC_EDT_SC_DEST_LOC_FORK1, m_edtScDestLocFork1);
	DDX_Control(pDX, IDC_EDT_SC_JOB_NO_FORK2, m_edtScJobNoFork2);
	DDX_Control(pDX, IDC_EDT_SC_JOB_TYP_FORK2, m_edtScJobTypFork2);
	DDX_Control(pDX, IDC_EDT_SC_START_LOC_FORK2, m_edtScStartLocFork2);
	DDX_Control(pDX, IDC_EDT_SC_DEST_LOC_FORK2, m_edtScDestLocFork2);


	//SC상태정보
	DDX_Control(pDX, IDC_LBL_SC_ONLINE_MODE_RD, m_lblScOnlineModeRd);
	DDX_Control(pDX, IDC_LBL_SC_AUTO_MODE_RD, m_lblScAutoModeRd);
	DDX_Control(pDX, IDC_LBL_SC_SC_STATUS_RD, m_lblScScStatusRd);
	DDX_Control(pDX, IDC_LBL_SC_COMPLETE_RD, m_lblScCompleteRd);
	DDX_Control(pDX, IDC_LBL_SC_ACTIVE_MODE_RD, m_lblScActiveModeRd);
	DDX_Control(pDX, IDC_LBL_SC_POS_V_RD, m_lblScPosVRd);
	DDX_Control(pDX, IDC_LBL_SC_POS_H_RD, m_lblScPosHRd);
	DDX_Control(pDX, IDC_LBL_SC_SENSOR_FK_RD, m_lblScSensorFkRd);

	DDX_Control(pDX, IDC_EDT_SC_ONLINE_MODE_RD, m_edtScOnlineModeRd);
	DDX_Control(pDX, IDC_EDT_SC_AUTO_MODE_RD, m_edtScAutoModeRd);
	DDX_Control(pDX, IDC_EDT_SC_SC_STATUS_RD, m_edtScScStatusRd);
	DDX_Control(pDX, IDC_EDT_SC_COMPLETE_RD, m_edtScCompleteRd);
	DDX_Control(pDX, IDC_EDT_SC_ACTIVE_MODE_RD, m_edtScActiveModeRd);
	DDX_Control(pDX, IDC_EDT_SC_POS_V_RD, m_edtScPosVRd);
	DDX_Control(pDX, IDC_EDT_SC_POS_H_RD, m_edtScPosHRd);
	DDX_Control(pDX, IDC_EDT_SC_SENSOR_FK_RD, m_edtScSensorFkRd);


	//에러정보
	DDX_Control(pDX, IDC_EDT_SC_JOB_SC_ERR_CODE, m_edtScErrCode);


	//포크상태
	DDX_Control(pDX, IDC_LBL_SC_ERR_STA_FK1_RD, m_lblScErrStaFk1Rd);
	DDX_Control(pDX, IDC_LBL_SC_ERR_STA_FK2_RD, m_lblScErrStaFk2Rd);

	DDX_Control(pDX, IDC_EDT_SC_ERR_STA_FK1_RD, m_edtScErrStaFk1Rd);
	DDX_Control(pDX, IDC_EDT_SC_ERR_STA_FK2_RD, m_edtScErrStaFk2Rd);

	//작업정보
	DDX_Control(pDX, IDC_LBL_SC_ITN_LUGG_FK1, m_lblScItnLuggFk1);
	DDX_Control(pDX, IDC_LBL_SC_ITN_LUGG_FK2, m_lblScItnLuggFk2);
	DDX_Control(pDX, IDC_LBL_SC_BCR_BOTTOM_FK1, m_lblScBcrBottomFk1);
	DDX_Control(pDX, IDC_LBL_SC_BCR_BOTTOM_FK2, m_lblScBcrBottomFk2);
	DDX_Control(pDX, IDC_LBL_SC_JOB_TYP, m_lblScJobTyp);
	DDX_Control(pDX, IDC_LBL_SC_STO_HS, m_lblScStoHs);
	DDX_Control(pDX, IDC_LBL_SC_RET_HS, m_lblScRetHs);
	DDX_Control(pDX, IDC_LBL_SC_JOB_PROGRESS, m_lblScJobProgress);
	DDX_Control(pDX, IDC_LBL_SC_PRODUCT_ID, m_lblScProductId);

	DDX_Control(pDX, IDC_EDT_SC_ITN_LUGG_FK1, m_edtScItnLuggFk1);
	DDX_Control(pDX, IDC_EDT_SC_ITN_LUGG_FK2, m_edtScItnLuggFk2);
	DDX_Control(pDX, IDC_EDT_SC_BCR_BOTTOM_FK1, m_edtScBcrBottomFk1);
	DDX_Control(pDX, IDC_EDT_SC_BCR_BOTTOM_FK2, m_edtScBcrBottomFk2);
	DDX_Control(pDX, IDC_EDT_SC_JOB_TYP, m_edtScJobTyp);
	DDX_Control(pDX, IDC_EDT_SC_STO_HS, m_edtScStoHs);
	DDX_Control(pDX, IDC_EDT_SC_RET_HS, m_edtScRetHs);
	DDX_Control(pDX, IDC_EDT_SC_JOB_PROGRESS, m_edtScJobProgress);
	DDX_Control(pDX, IDC_EDT_SC_PRODUCT_ID,  m_edtScProductId);


	//그룹박스
	DDX_Control(pDX, IDC_GRP_SC_SC_JOB_INFORMATION, m_grpScScJobInformation);
	DDX_Control(pDX, IDC_GRP_SC_SC_STA_INFO, m_grpScScJobInformation);
	DDX_Control(pDX, IDC_GRP_SC_FORK_STA, m_grpScScJobInformation);
	DDX_Control(pDX, IDC_GRP_SC_INFORMATION, m_grpScInformation);
	DDX_Control(pDX, IDC_GRP_SC_ERROR_INFORMATION, m_grpScErrorInformation);
	DDX_Control(pDX, IDC_GRP_SC_SC_JOB_INFORMATION_FORK1, m_grpScScJobInformationFork1);
	DDX_Control(pDX, IDC_GRP_SC_SC_JOB_INFORMATION_FORK2, m_grpScScJobInformationFork2);

	//체크박스
	DDX_Control(pDX, IDC_SC_CHECK_STORE, m_chkScCheckStore);
	DDX_Control(pDX, IDC_SC_CHECK_RETIEVE, m_chkScCheckRetive);


	//CSTATIC
	DDX_Control(pDX, IDC_EDT_SC_NO, m_edtScNo);


	DDX_Control(pDX, IDC_LBL_SC_JOB_SC_SUSPEND, m_lblScSuspend);

	//COMMAND버튼
	DDX_Control(pDX, IDC_BTN_SC_COMPLETE, m_btnScConfirm);
	DDX_Control(pDX, IDC_BTN_SC_DELETE, m_btnScDelete);
	DDX_Control(pDX, IDC_BTN_SC_ESTOP, m_btnScEmergency);
	DDX_Control(pDX, IDC_BTN_SC_STOP, m_btnScStop);
	DDX_Control(pDX, IDC_BTN_SC_ACTIVE, m_btnScActive);
	DDX_Control(pDX, IDC_BTN_SC_ERROR_RESET, m_btnScErrorReset);
	DDX_Control(pDX, IDC_BTN_SC_CALL_TO_HOME, m_btnScCallToHome);
	DDX_Control(pDX, IDC_BTN_SC_DUAL_STORE, m_btnDuplSto);
	DDX_Control(pDX, IDC_BTN_SC_CUT_JOBINFO, m_btnCutJobInfo);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL, m_btnScManual);



	DDX_Control(pDX, IDC_BTN_SC_INIT, m_btnScInit);
	DDX_Control(pDX, IDC_BTN_SC_STO_SUSPEND, m_btnScStoSuspend);
	DDX_Control(pDX, IDC_BTN_SC_RET_SUSPEND, m_btnScRetSuspend);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL_RET, m_btnScManualRet);




	DDX_Control(pDX, IDC_CBX_SC_JOB_TYP, m_cbxScJobTyp);
	DDX_Control(pDX, IDC_CBX_SC_START_POS, m_cbxScStartPos);
	DDX_Control(pDX, IDC_CBX_SC_DEST_POS, m_cbxScDestPos);

	//--JOBSTATUS

	//SCSTATUS
	//DDX_Control(pDX, IDC_EDT_SC_WARMINGUP, m_edtScWarmingUp);
	DDX_Control(pDX, IDC_EDT_SC_JOB_SC_SUSPEND, m_edtScSuspend);





	//DDX_Control(pDX, IDC_BTN_SC_ALL_SUSPEND,				m_btnScAllSuspend);
	//--COMMAND


}

BEGIN_MESSAGE_MAP(CScSkinDlg, CSkinDialog)
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CScSkinDlg::OnMessagSwitch)

	//버튼 이벤트
	ON_BN_CLICKED(IDC_BTN_SC_COMPLETE, &CScSkinDlg::OnBnClickedBtnScConfirm)
	ON_BN_CLICKED(IDC_BTN_SC_DELETE, &CScSkinDlg::OnBnClickedBtnScDelte)
	ON_BN_CLICKED(IDC_BTN_SC_ESTOP, &CScSkinDlg::OnBnClickedBtnScEmergency)
	ON_BN_CLICKED(IDC_BTN_SC_STOP, &CScSkinDlg::OnBnClickedBtnScStop)
	ON_BN_CLICKED(IDC_BTN_SC_ACTIVE, &CScSkinDlg::OnBnClickedBtnScActive)
	ON_BN_CLICKED(IDC_BTN_SC_ERROR_RESET, &CScSkinDlg::OnBnClickedBtnScErrorReset)
	ON_BN_CLICKED(IDC_BTN_SC_CALL_TO_HOME, &CScSkinDlg::OnBnClickedBtnScCallToHome)
	ON_BN_CLICKED(IDC_BTN_DUPL_STO, &CScSkinDlg::OnBnClickedScDuplicationSto)
	ON_BN_CLICKED(IDC_BTN_SC_CUT_JOBINFO, &CScSkinDlg::OnBnClickedBtnScCutJobInfo)
	ON_BN_CLICKED(IDC_BTN_SC_MANUAL, &CScSkinDlg::OnBnClickedBtnScManual)

	ON_BN_CLICKED(IDC_SC_CHECK_STORE, &CScSkinDlg::OnCheckStore)
	ON_BN_CLICKED(IDC_SC_CHECK_RETIEVE, &CScSkinDlg::OnCheckRetrieve)


	//ON_BN_CLICKED(ID_BTN_SC_OK, &CScSkinDlg::OnBnClickedBtnScOk)
	ON_BN_CLICKED(ID_BTN_SC_CANCEL, &CScSkinDlg::OnBnClickedBtnScCancel)
	ON_WM_CLOSE()

	//ON_BN_CLICKED(IDC_BTN_SC_STO_SUSPEND, &CScSkinDlg::OnBnClickedBtnScStoSuspend)
	//ON_BN_CLICKED(IDC_BTN_SC_RET_SUSPEND, &CScSkinDlg::OnBnClickedBtnScRetSuspend)
	//ON_BN_CLICKED(IDC_BTN_SC_ALL_SUSPEND, &CScSkinDlg::OnBnClickedBtnScAllSuspend)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK1, &CScSkinDlg::OnBnClickedChkScFork1)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK2, &CScSkinDlg::OnBnClickedChkScFork2)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK12, &CScSkinDlg::OnBnClickedScRtvFork12)

	ON_BN_CLICKED(IDC_BTN_SC_MANUAL_RET, &CScSkinDlg::OnBnClickedBtnScManualRet)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//	ON_BN_CLICKED(IDC_GRP_SC_JOB_STATUS_COMMAND, &CScSkinDlg::OnBnClickedGrpScJobStatusCommand)
	//ON_BN_CLICKED(IDC_BTN_SC_INIT, &CScSkinDlg::OnBnClickedBtnScInit)

	//ON_STN_CLICKED(IDC_LBL_SC_JOB_START_LOC, &CScSkinDlg::OnStnClickedLblScJobStartLoc)
END_MESSAGE_MAP()

BOOL CScSkinDlg::OnInitDialog()
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

	m_strScFork = _T("0");

	SetTimer(1, 1500, NULL);

	InitializeResource(pEn);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScSkinDlg::RenameResource(EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	//버튼
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forcecompletion"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_COMPLETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_DELETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("emergencystop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ESTOP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_STOP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorreset"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ERROR_RESET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("calltohome"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_CALL_TO_HOME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dualstore"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_DUAL_STORE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cutjobinfo"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_CUT_JOBINFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manual"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_MANUAL, strValue);


	//sc작업정보
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scjobinformation"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_JOB_INFORMATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_JOB_INFORMATION_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_NO_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_TYP_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_START_POS_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_DEST_POS_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_JOB_INFORMATION_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_NO_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_TYP_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_START_POS_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_DEST_POS_FORK2, strValue);


	//SC상태정보
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scstainformation"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STA_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("onlinemode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_ONLINE_MODE_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("autoemode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_AUTO_MODE_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_SC_STATUS_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sccomplete"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_COMPLETE_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("activemode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_ACTIVE_MODE_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("posv"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_POS_V_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("posh"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_POS_H_RD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sensorfk"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_SENSOR_FK_RD, strValue);


	//포크상태
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forksta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_FORK_STA, strValue);


	//작업정보
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("information"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_INFORMATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("itnluggfk1"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_ITN_LUGG_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_ITN_LUGG_FK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrbottomfk1"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_BCR_BOTTOM_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_BCR_BOTTOM_FK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stohs"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_STO_HS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("reths"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_RET_HS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobprogress"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_PROGRESS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productid"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_PRODUCT_ID, strValue);


	//에러정보
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorinformation"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_ERROR_INFORMATION, strValue);


	//일시정지
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scstatuscommand"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("checkstore"), (int)m_enLang);
	SetDlgItemText(IDC_SC_CHECK_STORE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("checkretieve"), (int)m_enLang);
	SetDlgItemText(IDC_SC_CHECK_RETIEVE, strValue);
	//	예전에는 Ecs.rc 캡션 그대로라 언어를 바꿔도 안 바뀌던 것들
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SUSPEND, strValue);

	}

void CScSkinDlg::RedrawSize()
	{
		CRect rect;

		if (m_pSC_DATA->V_SC_TYP == "SINGLE")
		{
			m_grpScScJobInformationFork2.ShowWindow(SW_HIDE);

			m_lblScJobNoFork2.ShowWindow(SW_HIDE);
			m_lblScJobTypFork2.ShowWindow(SW_HIDE);
			m_lblScStartPosFork2.ShowWindow(SW_HIDE);
			m_lblScDestPosFork2.ShowWindow(SW_HIDE);

			m_edtScJobNoFork2.ShowWindow(SW_HIDE);
			m_edtScJobTypFork2.ShowWindow(SW_HIDE);
			m_edtScStartLocFork2.ShowWindow(SW_HIDE);
			m_edtScDestLocFork2.ShowWindow(SW_HIDE);

			m_lblScErrStaFk2Rd.ShowWindow(SW_HIDE);

			m_edtScErrStaFk2Rd.ShowWindow(SW_HIDE);

			m_grpScScJobInformationFork1.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_grpScScJobInformationFork1.MoveWindow(rect.left, rect.top, 485, rect.Height(), true);

			m_edtScJobNoFork1.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_edtScJobNoFork1.MoveWindow(rect.left, rect.top, 380, rect.Height(), true);

			m_edtScJobTypFork1.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_edtScJobTypFork1.MoveWindow(rect.left, rect.top, 380, rect.Height(), true);

			m_edtScStartLocFork1.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_edtScStartLocFork1.MoveWindow(rect.left, rect.top, 380, rect.Height(), true);

			m_edtScDestLocFork1.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_edtScDestLocFork1.MoveWindow(rect.left, rect.top, 380, rect.Height(), true);

			m_edtScErrStaFk1Rd.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_edtScErrStaFk1Rd.MoveWindow(rect.left, rect.top, 380, rect.Height(), true);

		}
	}

void CScSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath2.Format(_T("%s"), chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");

	m_btnScConfirm.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScConfirm.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("forcecompletion"), strExtension)), NULL, 5, 5);

	m_btnScDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnScEmergency.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScEmergency.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("emergencystop"), strExtension)), NULL, 5, 5);

	m_btnScStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("stop"), strExtension)), NULL, 5, 5);

	m_btnScActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("active"), strExtension)), NULL, 5, 5);

	m_btnScErrorReset.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScErrorReset.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("errorreset"), strExtension)), NULL, 5, 5);

	m_btnScCallToHome.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScCallToHome.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("calltohome"), strExtension)), NULL, 5, 5);

	m_btnDuplSto.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnDuplSto.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("dualstore"), strExtension)), NULL, 5, 5);

	m_btnCutJobInfo.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCutJobInfo.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cutjobinfo"), strExtension)), NULL, 5, 5);


	m_btnScManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

}


void CScSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_bInitialized)
		RelocationControls();
}
HCURSOR CScSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CScSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x = 0, y = 0;


	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnScConfirm.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScConfirm.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScEmergency.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScEmergency.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScStop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScStop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScErrorReset.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScErrorReset.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScCallToHome.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScCallToHome.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnDuplSto.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnDuplSto.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCutJobInfo.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCutJobInfo.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScManual.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScManual.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}



BOOL CScSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CScSkinDlg::InvalidateScData(EN_LANG pLang)
{
	if (m_pDoc == NULL) return;
	if (m_pSC_DATA == NULL) return;

	if (!UpdateData())
		return;

	CString strScNo = m_pSC_DATA->K_SC_NO;//SC 번호
	CString strStartLocation;
	CString strDestLocation;

	CString strSuspend;
	

	m_edtScNo.SetWindowText(m_pSC_DATA->V_MC_NO + " (" + m_pSC_DATA->V_MC_NO_NM + ")");//SC 명
	//m_edtScJobNoFork1.SetWindowText(m_pSC_DATA->V_LUGG_NO);//작업번호1

	CString strSql = GetQrySelectSC_STATUS_CCD(m_pSC_DATA);
	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
	/*	CString strSTART_HSPOS = pRsw->GetItem(_T("START_HSPOS"));
		CString strDEST_HSPOS = pRsw->GetItem(_T("DEST_HSPOS"));
		CString strStartBankTemp = m_pSC_DATA->V_START_BANK;
		CString strDestBankTemp = m_pSC_DATA->V_DEST_BANK;

		if (m_pSC_DATA->V_START_BANK == "05") strStartBankTemp = "01";
		else if (m_pSC_DATA->V_START_BANK == "06") strStartBankTemp = "02";
		else if (m_pSC_DATA->V_START_BANK == "07") strStartBankTemp = "03";
		else if (m_pSC_DATA->V_START_BANK == "08") strStartBankTemp = "04";

		if (m_pSC_DATA->V_DEST_BANK == "05") strDestBankTemp = "01";
		else if (m_pSC_DATA->V_DEST_BANK == "06") strDestBankTemp = "02";
		else if (m_pSC_DATA->V_DEST_BANK == "07") strDestBankTemp = "03";
		else if (m_pSC_DATA->V_DEST_BANK == "08") strDestBankTemp = "04";

		strStartLocation.Format(_T("%02s-%03s-%02s"), strStartBankTemp, m_pSC_DATA->V_START_BAY, m_pSC_DATA->V_START_LEVEL);
		strDestLocation.Format(_T("%02s-%03s-%02s"), strDestBankTemp, m_pSC_DATA->V_DEST_BAY, m_pSC_DATA->V_DEST_LEVEL);

		if (strStartLocation == "01-000-00") strStartLocation = strSTART_HSPOS + _T("[입고 H/S]");
		if (strDestLocation == "01-000-00") strDestLocation = strDEST_HSPOS + _T("[출고 H/S]");*/

		m_edtScJobNoFork1.SetWindowText(pRsw->GetItem(_T("LUGG_NO_FK1")));
		m_edtScJobTypFork1.SetWindowText(pRsw->GetItem(_T("JOB_TYP_FK1")));
		m_edtScStartLocFork1.SetWindowText(pRsw->GetItem(_T("START_LOC_FK1")));
		m_edtScDestLocFork1.SetWindowText(pRsw->GetItem(_T("DEST_LOC_FK1")));
		m_edtScJobNoFork2.SetWindowText(pRsw->GetItem(_T("LUGG_NO_FK2")));
		m_edtScJobTypFork2.SetWindowText(pRsw->GetItem(_T("JOB_TYP_FK2")));
		m_edtScStartLocFork2.SetWindowText(pRsw->GetItem(_T("START_LOC_FK2")));
		m_edtScDestLocFork2.SetWindowText(pRsw->GetItem(_T("DEST_LOC_FK2")));

		m_edtScOnlineModeRd.SetWindowText(pRsw->GetItem(_T("ONLINE_MODE_RD")));
		m_edtScAutoModeRd.SetWindowText(pRsw->GetItem(_T("AUTO_MODE_RD")));
		m_edtScScStatusRd.SetWindowText(pRsw->GetItem(_T("UCSTATUS_RD")));
		m_edtScCompleteRd.SetWindowText(pRsw->GetItem(_T("COMPLETE_RD")));
		m_edtScActiveModeRd.SetWindowText(pRsw->GetItem(_T("ACTIVE_MODE_RD")));
		m_edtScPosVRd.SetWindowText(pRsw->GetItem(_T("POS_H_RD")));
		m_edtScPosHRd.SetWindowText(pRsw->GetItem(_T("POS_V_RD")));
		m_edtScSensorFkRd.SetWindowText(pRsw->GetItem(_T("SENSOR_FK_RD")));

		m_edtScErrStaFk1Rd.SetWindowText(pRsw->GetItem(_T("ERR_STA_FK1_RD")));
		m_edtScErrStaFk2Rd.SetWindowText(pRsw->GetItem(_T("ERR_STA_FK2_RD")));

		m_edtScItnLuggFk1.SetWindowText(pRsw->GetItem(_T("ITN_LUGG_FK1")));
		m_edtScItnLuggFk2.SetWindowText(pRsw->GetItem(_T("ITN_LUGG_FK2")));
		m_edtScBcrBottomFk1.SetWindowText(pRsw->GetItem(_T("BCR_BOTTOM_FK1")));
		m_edtScBcrBottomFk2.SetWindowText(pRsw->GetItem(_T("BCR_BOTTOM_FK2")));
		m_edtScJobTyp.SetWindowText(pRsw->GetItem(_T("JOB_TYP")));
		m_edtScStoHs.SetWindowText(pRsw->GetItem(_T("START_POS_NM")));
		m_edtScRetHs.SetWindowText(pRsw->GetItem(_T("DEST_POS_NM")));
		m_edtScJobProgress.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
		m_edtScProductId.SetWindowText(pRsw->GetItem(_T("PRODUCT_ID")));

		m_edtScErrCode.SetWindowText(pRsw->GetItem(_T("ERR_CODE_RD")));


		strSuspend = pRsw->GetItem(_T("SUSPEND"));

		if (strSuspend == "0") {
			m_chkScCheckStore.SetCheck(0);
			m_chkScCheckRetive.SetCheck(0);
		}
		else if (strSuspend == "1") {
			m_chkScCheckStore.SetCheck(1);
			m_chkScCheckRetive.SetCheck(0);
		}
		else if (strSuspend == "2") {
			m_chkScCheckStore.SetCheck(0);
			m_chkScCheckRetive.SetCheck(1);
		}
		else if (strSuspend == "3") {
			m_chkScCheckStore.SetCheck(1);
			m_chkScCheckRetive.SetCheck(1);
		}

		//m_edtScStatus.SetWindowText(pRsw->GetItem(_T("CRANE_REQUEST_RD")));//동작상태
		//m_edtScComplete.SetWindowText(pRsw->GetItem(_T("ORDER_CHECK_RD")));//완료상태
		//m_edtScProdLoad.SetWindowText(pRsw->GetItem(_T("PROD_CHECK_RD")));//화물유무
		//m_edtScWarmingUp.SetWindowText(pRsw->GetItem(_T("WARMING_UP_RD")));//예열상태
		//m_edtScErrorStatus.SetWindowText(pRsw->GetItem(_T("IS_ERROR_RD")));//에러상태
		//m_edtScHP.SetWindowText(pRsw->GetItem(_T("CRANE_AT_HP_RD")));//홈포지션상태
		//m_edtScSuspend.SetWindowText(pRsw->GetItem(_T("SUSPEND")));//SUSPEND
		//m_edtScForkingPos.SetWindowText(pRsw->GetItem(_T("START_DEEP_CELL")) + "/" + pRsw->GetItem(_T("DEST_DEEP_CELL")));//포킹위치
	}
	delete pRsw;



	//if (m_pSC_DATA->V_LUGG_NO != _T("0"))
	//{
	//	strSql = GetQrySelectJOB_MST_FK1(m_pSC_DATA);
	//	nRowCnt = -1;
	//	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	//	if (nRowCnt <= 0) { return; }
	//	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	//	pRsw->MoveFirst();
	//	for (int i = 0; i < nRowCnt; i++)
	//	{
	//		m_edtScJobJobNo.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));

	//		CString strJOB_TYP_TEMP = pRsw->GetItem(_T("JOB_TYP"));
	//		//m_edtScJobJobTyp.SetWindowText(strJOB_TYP_TEMP);//작업구분

	//		//m_edtScJobJobStatus.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
	//		//m_edtScJobProductId.SetWindowText(pRsw->GetItem(_T("PRODUCT_ID")));
	//		
	//		strStartLocation.Format(_T("%09s"), pRsw->GetItem(_T("START_LOCATION")));
	//		strDestLocation.Format(_T("%09s"), pRsw->GetItem(_T("DEST_LOCATION")));

	//		if (strJOB_TYP_TEMP == "입고") strStartLocation = "1";
	//		if (strJOB_TYP_TEMP == "출고") strDestLocation = "2";

	//		//m_edtScJobStartLoc.SetWindowText(strStartLocation);
	//		//m_edtScJobDestLoc.SetWindowText(strDestLocation);
	//		//m_edtScJobBcrBottom.SetWindowText(pRsw->GetItem(_T("BCR_BOTTOM")));

	//		pRsw->MoveNext();
	//	}
	//	delete pRsw;
	//}
	InvalidateLangControl(pLang);
}

void CScSkinDlg::InvalidateScDataSuspend(EN_LANG pLang)
{

	CString strSuspend;


	CString strSql = GetQrySelectSC_DATA(m_pSC_DATA);
	CString strMessage;

	int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
	

		strSuspend = pRsw->GetItem(_T("SUSPEND"));

		if (strSuspend == "0") {
			m_chkScCheckStore.SetCheck(0);
			m_chkScCheckRetive.SetCheck(0);
		}
		else if (strSuspend == "1") {
			m_chkScCheckStore.SetCheck(1);
			m_chkScCheckRetive.SetCheck(0);
		}
		else if (strSuspend == "2") {
			m_chkScCheckStore.SetCheck(0);
			m_chkScCheckRetive.SetCheck(1);
		}
		else if (strSuspend == "3") {
			m_chkScCheckStore.SetCheck(1);
			m_chkScCheckRetive.SetCheck(1);
		}
	
	}
	delete pRsw;

}

void CScSkinDlg::InvalidateLangControl(EN_LANG enLangTemp)
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if (m_pDoc == NULL)
		return;
}


LRESULT CScSkinDlg::OnMessagSwitch(WPARAM wParam, LPARAM lParam)
{
	CSC_DATA* pSC_DATA = (CSC_DATA*)wParam;
	if (pSC_DATA == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC를 찾을수 없습니다")));
		return 0;
	}

	m_pSC_DATA = pSC_DATA;

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if (m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	RedrawSize();

	InvalidateScData(enLangTemp);
	//InvalidateScDataSuspend(enLangTemp);

	return 0;
}

void CScSkinDlg::SetButtonScImage(CButton& pButton, CString pFieldName, int pValue)
{
	//HICON hIcon = Global.GetIcon(Global.GetCvIconIndex(pFieldName, pValue));
	//pButton.SetIcon(hIcon);
}

//강제완료
void CScSkinDlg::OnBnClickedBtnScConfirm()
{
	UpdateScData(EN_BtnScConfirm);
}

//삭제
void CScSkinDlg::OnBnClickedBtnScDelte()
{
	int BtnJobDelete;

	CDialogScDelFork dlgDelFork;

	if (m_pSC_DATA ->V_SC_TYP == "TWIN")
	{
		dlgDelFork.m_nDelFork = 2;
		if (dlgDelFork.DoModal() != IDOK)
			return;

	}
	else
		dlgDelFork.m_nDelFork = 0;

	switch (dlgDelFork.m_nDelFork)
	{
	case 0:		BtnJobDelete = EN_BtnScFk1Delete;	break;
	case 1:		BtnJobDelete = EN_BtnScFk2Delete;	break;
	case 2:		BtnJobDelete = EN_BtnScFk1Fk2Delete;	break;
	}

	UpdateScData(BtnJobDelete);
}

//비상정지
void CScSkinDlg::OnBnClickedBtnScEmergency()
{
	UpdateScData(EN_BtnScEmergency);
}

//정지
void CScSkinDlg::OnBnClickedBtnScStop()
{
	UpdateScData(EN_BtnScStop);
}

//ACTIVE
void CScSkinDlg::OnBnClickedBtnScActive()
{
	UpdateScData(EN_BtnScActive);
}

//에러리셋
void CScSkinDlg::OnBnClickedBtnScErrorReset()
{
	UpdateScData(EN_BtnScErrReset);
}

//홈복귀
void CScSkinDlg::OnBnClickedBtnScCallToHome()
{
	UpdateScData(EN_BtnScCallToHome);
}

//이중입고
void CScSkinDlg::OnBnClickedScDuplicationSto()
{
	UpdateData(TRUE);

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	if (m_pSC_DATA == NULL) return;

	CString strScNo = m_pSC_DATA->K_SC_NO;//SC 번호
	CString strLuggNo = m_pSC_DATA->V_LUGG_NO;//작업 번호

	strSql.Format(_T(" SELECT *					 ")
		_T("		     FROM SC_DATA                     ")
		_T("		    WHERE WH_TYP      = '%s'          ")
		_T("		      AND EQP_ERR_CD   = 'E054'       ")
		_T("		      AND SC_NO   = '%s'       "), m_pDoc->m_WH_TYP, strScNo);


	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);

	if (nRowCnt > 0)
	{
		strSql.Format(_T(" UPDATE JOB_MST					 ")
			_T("		      SET JOB_STA = '44'                     ")
			_T("		    WHERE WH_TYP  = '%s'          ")
			_T("		      AND LUGG_NO = '%s'       "), m_pDoc->m_WH_TYP, strLuggNo);

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == TRUE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 사용하지 않음")));
	}
	else
	{

	}
}

//잘라내기
void CScSkinDlg::OnBnClickedBtnScCutJobInfo()
{

}

//수동지시
void CScSkinDlg::OnBnClickedBtnScManual()
{
	UpdateScData(EN_BtnScManual);
}





void CScSkinDlg::OnBnClickedBtnScCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->m_pScSkinDlg = NULL;
	CSkinDialog::OnClose();
	OnClose();
}

void CScSkinDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDoc->m_pScSkinDlg = NULL;
	delete this;
}

void CScSkinDlg::UpdateScData(int nBtnJob)
{
	CString strSql = _T("");
	CString strWhTyp = _T("");
	CString strScNo = _T("");
	CString strPlcNo = _T("");
	CString strScGrpNo = _T("");
	CString strCmdId = _T("");
	CString strMsg = _T("");
	CString strFK = _T("");
	UpdateData(TRUE);

	strWhTyp = m_pDoc->m_WH_TYP;
	strScNo = m_pSC_DATA->K_SC_NO;
	strPlcNo = m_pSC_DATA->K_PLC_NO;



	//CMD_RQ_ID 공통코드 참조
	if (nBtnJob == EN_BtnScConfirm) { strCmdId = _T("FCMP"); strFK = _T("0");  strMsg = _T("강제완료를 진행하시겠습니까?"); }					//강제완료
	else if (nBtnJob == EN_BtnScFk1Delete) { strCmdId = _T("DELFK1"); strFK = _T("1");   strMsg = _T("Fork1 삭제를 진행하시겠습니까?"); }		//Fork1삭제
	else if (nBtnJob == EN_BtnScFk2Delete) { strCmdId = _T("DELFK2"); strFK = _T("2");   strMsg = _T("Fork2 삭제를 진행하시겠습니까?"); }		//Fork2삭제
	else if (nBtnJob == EN_BtnScFk1Fk2Delete) { strCmdId = _T("DELFK12"); strFK = _T("0");  strMsg = _T("Fork1,2 삭제를 진행하시겠습니까?"); }	//Fork1, Fork2삭제
	else if (nBtnJob == EN_BtnScStop) { strCmdId = _T("PAUSE"); strFK = _T("0"); strMsg = _T("STOP을 진행하시겠습니까?"); }						//정지
	else if (nBtnJob == EN_BtnScEmergency) { strCmdId = _T("EMERGENCY"); strFK = _T("0"); strMsg = _T("비상정지를 진행하시겠습니까?"); }		//비상정지
	else if (nBtnJob == EN_BtnScActive) { strCmdId = _T("ACTIVE"); strFK = _T("0"); strMsg = _T("ACTIVE를 진행하시겠습니까?"); }			//ACTIVE
	else if (nBtnJob == EN_BtnScErrReset) { strCmdId = _T("RESET"); strFK = _T("0"); strMsg = _T("ERROR RESET을 진행하시겠습니까?"); }		//ERROR RESET
	else if (nBtnJob == EN_BtnScCallToHome) { strCmdId = _T("CTH"); strFK = _T("0");  strMsg = _T("홈복귀를 진행하시겠습니까?"); }			//홈복귀
	else if (nBtnJob == EN_BtnScDualStore) { strCmdId = _T("DUSTO"); strFK = _T("0");  strMsg = _T("이중입고를 진행하시겠습니까?"); }		//이중입고
	//잘라내기
	else if (nBtnJob == EN_BtnScManual) { strCmdId = _T("MOD"); strFK = _T("0");  strMsg = _T("수동지시를 진행하시겠습니까?"); }			//수동지시

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strMsg), MB_YESNO) != IDYES)
		return;

	m_pDoc->BeginTrans_DLG();

	//공출고 일 때 삭제처리
	if (strCmdId == _T("RESET"))
	{
		if (m_pSC_DATA->V_ERR_CODE_RD == _T("0060") || m_pSC_DATA->V_ERR_CODE_RD == _T("0061") ||
			m_pSC_DATA->V_ERR_CODE_RD == _T("0062") || m_pSC_DATA->V_ERR_CODE_RD == _T("0063"))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 발생된 에러는 이중입고 에러입니다.\n에러 리셋을 할 경우 재지정처리가 어렵습니다.\n에러리셋을 명령하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}
	}
	strSql.Format(_T(" UPDATE SC_DATA						  \n")
		_T("    SET CMD_RQ_ID = '%s'						  \n")
		_T("	  , OD_RQ_YN = 'Y'							  \n")
		_T("	  , READ_UPD_DT = ") + m_pDoc->SYSDATE + _T(" \n")
		_T("  WHERE WH_TYP = '%s'							  \n")
		_T("	AND PLC_NO = '%s'							  \n")
		_T("    AND MC_NO = '%s'							   "), strCmdId, strWhTyp, strPlcNo, strScNo);

	BOOL IsSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (IsSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = m_pSC_DATA->V_ITN_LUGG;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
		CString strLOG_BARCODE = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC ACTION BUTTON -> SC NO : %s , ACTION : %s"), strScNo, strCmdId);

		if (strFK == _T("1"))
		{
			if (strLOG_LUGG_NO != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		else if (strFK == _T("2"))
		{
			if (strLOG_LUGG_NO != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		else
		{
			if (strLOG_LUGG_NO == _T("0"))
			{
				if (strCmdId != _T("DELFK12"))
				{
					if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
					{
						m_pDoc->RollbackTrans_DLG();
						return;
					}
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 성공")));
		SetScStatus(strCmdId, _T(""));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 실패")));
	return;
}


void CScSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
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

	RenameResource(nEN_LANG);
	RedrawImage();

}



void CScSkinDlg::SetScStatus(CString pCMD, CString pSC_SUSPEND)
{
	CString strCcdNmKor = _T("");
	CString strGetErrorCode = _T("");

	if (pCMD == _T("SUSPEND"))
	{
		SelCommonCode(pSC_SUSPEND, _T("SC_SUSPEND"), strCcdNmKor);
		m_edtScSuspend.SetWindowText(strCcdNmKor);
	}
}

void CScSkinDlg::SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR)
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

void CScSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	//조회
	InvalidateScData(EN_KOR);
}


void CScSkinDlg::OnCheckStore()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strScNo = _T("");
	CString strPlcNo = m_pSC_DATA->K_PLC_NO;
	CString strMsg = _T("");
	CString strSUSPEND = _T("");
	int nSUSPEND = 0;

	UpdateData(TRUE);

	strScNo = m_pSC_DATA->K_SC_NO;

	strSql = GetQrySelectSC_DATA(m_pSC_DATA);

	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		nSUSPEND = _ttoi(pRsw->GetItem(_T("SUSPEND")));
	}
	delete pRsw;


	//nSUSPEND = CConvert::ToInt(m_pSC_DATA->V_SUSPEND);

	//현재 SUSPEND 상황에 따른 버튼처리
	switch (nSUSPEND)
	{
	case 0:
		strSUSPEND = _T("1"); //정상 -> 입고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 하시겠습니까?")), MB_YESNO) != IDYES) 	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 1:
		strSUSPEND = _T("0"); //입고정지 -> 정상
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 해제 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 2:
		strSUSPEND = _T("3"); //출고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 3:
		strSUSPEND = _T("2"); //입출고정지 -> 출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 해제 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	}

	strSql = GetUpdateQry(strWhTyp, strPlcNo, strScNo, strSUSPEND);

	m_pDoc->BeginTrans_DLG();

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = m_pSC_DATA->V_ITN_LUGG;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
		CString strLOG_BARCODE = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC SUSPEND STO BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), strScNo, nSUSPEND, strSUSPEND);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 성공")));
		SetScStatus(_T("SUSPEND"), strSUSPEND);
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 실패")));
	return;
}

void CScSkinDlg::OnCheckRetrieve()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strScNo = _T("");
	CString strPlcNo = m_pSC_DATA->K_PLC_NO;
	CString strMsg = _T("");
	CString strSUSPEND = _T("");
	int nSUSPEND = 0;

	UpdateData(TRUE);

	strScNo = m_pSC_DATA->K_SC_NO;

	strSql = GetQrySelectSC_DATA(m_pSC_DATA);

	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		nSUSPEND = _ttoi(pRsw->GetItem(_T("SUSPEND")));
	}
	delete pRsw;


	//nSUSPEND = CConvert::ToInt(m_pSC_DATA->V_SUSPEND);

	//현재 SUSPEND 상황에 따른 버튼처리
	switch (nSUSPEND)
	{
	case 0:
		strSUSPEND = _T("2"); //정상 -> 출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 1:
		strSUSPEND = _T("3"); //입고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 2:
		strSUSPEND = _T("0"); //출고정지 -> 정상
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 해제 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	case 3:
		strSUSPEND = _T("1"); //입출고정지 -> 입고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 해제 하시겠습니까?")), MB_YESNO) != IDYES)	InvalidateScDataSuspend(EN_KOR);	return;
		break;
	}

	strSql = GetUpdateQry(strWhTyp, strPlcNo, strScNo, strSUSPEND);

	m_pDoc->BeginTrans_DLG();

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = m_pSC_DATA->V_ITN_LUGG;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
		CString strLOG_BARCODE = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC SUSPEND RET BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), strScNo, nSUSPEND, strSUSPEND);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 성공")));
		SetScStatus(_T("SUSPEND"), strSUSPEND);
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 명령 지시 실패")));
	return;
}

void CScSkinDlg::OnBnClickedChkScFork1()
{

	UpdateData(TRUE);

	if (m_chkScCheckStore.GetCheck() == 1)
	{
		m_strScFork = _T("1");
		//Enable 처리
		m_chkScCheckStore.EnableWindow(TRUE);
		m_chkScCheckRetive.EnableWindow(FALSE);
		m_chkScFork1Fork2.EnableWindow(FALSE);
	}
	else {
		m_strScFork = _T("0");
		m_chkScCheckStore.EnableWindow(TRUE);
		m_chkScCheckRetive.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CScSkinDlg::OnBnClickedChkScFork2()
{
	UpdateData(TRUE);

	if (m_chkScCheckRetive.GetCheck() == 1)
	{
		m_strScFork = _T("2");
		//Enable 처리
		m_chkScCheckStore.EnableWindow(FALSE);
		m_chkScCheckRetive.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(FALSE);
	}
	else {
		m_strScFork = _T("0");
		m_chkScCheckStore.EnableWindow(TRUE);
		m_chkScCheckRetive.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}



void CScSkinDlg::OnBnClickedScRtvFork12()
{
	UpdateData(TRUE);

	if (m_chkScFork1Fork2.GetCheck() == 1)
	{
		m_strScFork = _T("3");
		//Enable 처리
		m_chkScCheckStore.EnableWindow(FALSE);
		m_chkScCheckRetive.EnableWindow(FALSE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	else {
		m_strScFork = _T("0");
		m_chkScCheckStore.EnableWindow(TRUE);
		m_chkScCheckRetive.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}

void CScSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	if (m_pDoc == NULL) { return; };
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j = 0;
	CString strMessage;


	strSql.Format(_T(" SELECT MSG_KOR, \n")
		_T(" ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, \n")
		_T(" ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, \n")
		_T(" ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN  \n ")
		_T("    FROM EQP_ECD_MST                    \n ")
		_T("   WHERE EQP_TYP      = '%s'             \n  ")
		_T("    AND EQP_ERR_CD   = '%02s'            \n   "), strEqpTyp, strErrorCode);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();

	strMSG_KOR = pRsw->GetItem(_T("MSG_KOR"));
	strMSG_CHIN = pRsw->GetItem(_T("MSG_CHIN"));
	strMSG_ENG = pRsw->GetItem(_T("MSG_ENG"));
	strMSG_HUN = pRsw->GetItem(_T("MSG_HUN"));

	if (nEN_LANG == 0)
	{
		if (strMSG_KOR == "")
			strMSG_KOR = "Unknown Error";

		strGetErrorCode.Format(_T("[%s]%s"), strErrorCode, strMSG_KOR);
	}
	else if (nEN_LANG == 1)
	{
		strGetErrorCode.Format(_T("[%s]%s"), strErrorCode, strMSG_ENG);
	}
	else if (nEN_LANG == 2)
	{
		strGetErrorCode.Format(_T("[%s]%s"), strErrorCode, strMSG_HUN);
	}
	else if (nEN_LANG == 3)
	{
		strGetErrorCode.Format(_T("[%s]%s"), strErrorCode, strMSG_CHIN);
	}

	delete pRsw;
}

void CScSkinDlg::GetCommonCode(CString pWH_TYP, CString pSC_NO)
{
	CString strSql = GetSelectQry(pWH_TYP, pSC_NO);
	int nRowCnt = 0;
	CString strMessage;

	_RecordsetPtr pRsptr = NULL; //m_pDoc->m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst();

	for (int i = 0; i < nRowCnt; i++)
	{

	}

	delete pRsw;
}

CString CScSkinDlg::GetUpdateQry(CString pWH_TYP, CString pPLC_NO, CString pSC_NO, CString pSuspend)
{
	CString strSql;

	strSql.Format(_T(" UPDATE SC_DATA		\n")
		_T("    SET SUSPEND = '%s' \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	  AND PLC_NO = '%02s'			\n")
		_T("    AND SC_NO = '%s'			"), pSuspend, pWH_TYP, pPLC_NO, pSC_NO);

	return strSql;
}

CString CScSkinDlg::GetSelectQry(CString pWH_TYP, CString pSC_NO)
{
	CString strSql;

	strSql.Format(_T("                                                                           ")
		_T("      SELECT SC.*																		 ")
		_T("           , ") + m_pDoc->NVL + _T("(CM1.CCD_NM_KOR,ONLINE_MODE_RD) AS ONLINE_MODE_RD_KOR      ")
		_T("           , ") + m_pDoc->NVL + _T("(CM2.CCD_NM_KOR,AUTO_MODE_RD) AS AUTO_MODE_RD_KOR          ")
		_T("           , ") + m_pDoc->NVL + _T("(CM3.CCD_NM_KOR,ACTIVE_MODE_RD) AS ACTIVE_MODE_RD_KOR      ")
		_T("           , ") + m_pDoc->NVL + _T("(CM4.CCD_NM_KOR,UCSTATUS_RD) AS UCSTATUS_RD_KOR            ")
		_T("           , ") + m_pDoc->NVL + _T("(CM5.CCD_NM_KOR,ERR_STA_FK1_RD) AS ERR_STA_RD_KOR          ")
		_T("           , ") + m_pDoc->NVL + _T("(CM6.CCD_NM_KOR,ERR_STA_FK2_RD) AS ERR_STA_RD_KOR          ")
		_T("           , ") + m_pDoc->NVL + _T("(CM7.CCD_NM_KOR,COMPLETE_RD) AS COMPLETE_RD_KOR            ")
		_T("           , ") + m_pDoc->NVL + _T("(CM8.CCD_NM_KOR,SENSOR_FK_RD) AS SENSOR_FK_RD_KOR          ")
		_T("        FROM SC_DATA SC                                                    ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'ONLINE_MODE_RD'                            ")
		_T("            ) CM1                                                          ")
		_T("            ON SC.ONLINE_MODE_RD = CM1.CCD_CD                              ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'AUTO_MODE_RD'                              ")
		_T("            ) CM2                                                          ")
		_T("            ON SC.AUTO_MODE_RD = CM2.CCD_CD                                ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'ACTIVE_MODE_RD'                            ")
		_T("            ) CM3                                                          ")
		_T("            ON SC.ACTIVE_MODE_RD = CM3.CCD_CD                              ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'UCSTATUS_RD'                               ")
		_T("            ) CM4                                                          ")
		_T("            ON SC.UCSTATUS_RD = CM4.CCD_CD                                 ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'ERR_STA_RD'                                ")
		_T("            ) CM5                                                          ")
		_T("            ON SC.ERR_STA_FK1_RD = CM5.CCD_CD                              ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'ERR_STA_RD'                                ")
		_T("            ) CM6                                                          ")
		_T("            ON SC.ERR_STA_FK2_RD = CM6.CCD_CD                              ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'COMPLETE_RD'                               ")
		_T("            ) CM7                                                          ")
		_T("            ON SC.COMPLETE_RD = CM7.CCD_CD                                 ")
		_T("    LEFT JOIN (                                                            ")
		_T("                SELECT *                                                   ")
		_T("                FROM COMMON_CODE                                           ")
		_T("                WHERE CDX_CD = 'SENSOR_FK_RD'                              ")
		_T("            ) CM8                                                          ")
		_T("            ON SC.SENSOR_FK_RD = CM8.CCD_CD                                ")
		_T("        WHERE SC.WH_TYP = '%s'                                             ")
		_T("        AND SC.SC_NO = '%s'                                               "), pWH_TYP, pSC_NO);

	return strSql;
}

CString CScSkinDlg::GetQrySelectJOB_MST_FK1(CSC_DATA* pSC_DATA)
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
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_TOP,'N') AS BCR_TOP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM,'N') AS BCR_BOTTOM ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR,'N') AS JOB_STATUS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID,'N') AS PRODUCT_ID ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.MES_ERROR_CD,'0') AS MES_ERROR_CD ");
	strSql += CRLF + _T("  FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS ");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE JM.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pSC_DATA->V_ITN_LUGG);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CScSkinDlg::GetQrySelectSC_STATUS_CCD(CSC_DATA* pSC_DATA)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(SD.LUGG_NO_FK1_RD) AS LUGG_NO_FK1	");								//작업번호FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, SD.JOB_TYP_RD) AS JOB_TYP_FK1 ");						//작업구분FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.START_BANK_FK1_RD || '-' || SD.START_BAY_FK1_RD || '-' || SD.START_LEVEL_FK1_RD) AS START_LOC_FK1 ");		//출발위치FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.DEST_BANK_FK1_RD || '-' || SD.DEST_BAY_FK1_RD || '-' || SD.DEST_LEVEL_FK1_RD) AS DEST_LOC_FK1 ");			//도착위치FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.LUGG_NO_FK1_RD) AS LUGG_NO_FK2	");								//작업번호FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, SD.JOB_TYP_RD) AS JOB_TYP_FK2 ");						//작업구분FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.START_BANK_FK2_RD || '-' || SD.START_BAY_FK2_RD || '-' || SD.START_LEVEL_FK2_RD) AS START_LOC_FK2 ");		//출발위치FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.DEST_BANK_FK2_RD || '-' || SD.DEST_BAY_FK2_RD || '-' || SD.DEST_LEVEL_FK2_RD) AS DEST_LOC_FK2 ");			//도착위치FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ONLINE_MODE_RD.CCD_NM_KOR, SD.ONLINE_MODE_RD) AS ONLINE_MODE_RD ");			//지상반
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_AUTO_MODE_RD.CCD_NM_KOR, SD.AUTO_MODE_RD) AS AUTO_MODE_RD ");			//기상반
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_UCSTATUS_RD.CCD_NM_KOR, SD.UCSTATUS_RD) AS UCSTATUS_RD ");					//SC상태
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_COMPLETE_RD.CCD_NM_KOR, SD.COMPLETE_RD) AS COMPLETE_RD ");					//완료상태
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ACTIVE_MODE_RD.CCD_NM_KOR, SD.ACTIVE_MODE_RD) AS ACTIVE_MODE_RD ");			//ACTIVE
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.POS_H_RD) AS POS_H_RD");											//수평주행
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.POS_V_RD) AS POS_V_RD");											//수직주행
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SENSOR_FK_RD.CCD_NM_KOR, SD.SENSOR_FK_RD) AS SENSOR_FK_RD ");				//화물유무
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ERR_STA_FK1_RD.CCD_NM_KOR, SD.ERR_STA_FK1_RD) AS ERR_STA_FK1_RD");	//포크상태#1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ERR_STA_FK2_RD.CCD_NM_KOR, SD.ERR_STA_FK2_RD) AS ERR_STA_FK2_RD");	//포크상태#2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.ITN_LUGG_FK1) AS ITN_LUGG_FK1	");								//내부번호FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.ITN_LUGG_FK2) AS ITN_LUGG_FK2	");								//내부번호FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM_FK1.BCR_BOTTOM)AS BCR_BOTTOM_FK1	");								//적재용기FK1
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM_FK2.BCR_BOTTOM) AS BCR_BOTTOM_FK2	");							//적재용기FK2
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, SD.JOB_TYP_RD) AS JOB_TYP ");				//작업구분
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(MC_MST_START.MC_NO_NM) AS START_POS_NM	");							//입고대상태
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(MC_MST_DEST.MC_NO_NM) AS DEST_POS_NM	");							//출고대상태
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR, JM_FK1.JOB_STATUS) AS JOB_STATUS ");		//진행상태
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM_FK1.PRODUCT_ID) AS PRODUCT_ID	");								//제품정보
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ERRCODE.CCD_NM_KOR, SD.ERR_CODE_RD) AS ERR_CODE_RD");			//에러코드
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(SD.SUSPEND) AS SUSPEND");											//입출고금지상태
	strSql += CRLF + _T("  FROM SC_DATA SD LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = SD.JOB_TYP_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ONLINE_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_ONLINE_MODE_RD.CDX_CD = 'SC_ONLINE_MODE'	");
	strSql += CRLF + _T("                         AND CCD_ONLINE_MODE_RD.CCD_CD = SD.ONLINE_MODE_RD	");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_AUTO_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_AUTO_MODE_RD.CDX_CD = 'SC_AUTO_MODE' ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.CCD_CD = SD.AUTO_MODE_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_UCSTATUS_RD");
	strSql += CRLF + _T("                          ON CCD_UCSTATUS_RD.CDX_CD = 'UCSTATUS_RD'	");
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.CCD_CD = SD.UCSTATUS_RD	");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_COMPLETE_RD	");
	strSql += CRLF + _T("                          ON CCD_COMPLETE_RD.CDX_CD = 'COMPLETE_RD' ");
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.CCD_CD = SD.COMPLETE_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ACTIVE_MODE_RD	");
	strSql += CRLF + _T("                          ON CCD_ACTIVE_MODE_RD.CDX_CD = 'ACTIVE_MODE_RD' ");
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.CCD_CD = SD.ACTIVE_MODE_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SENSOR_FK_RD	");
	strSql += CRLF + _T("                          ON CCD_SENSOR_FK_RD.CDX_CD = 'SENSOR_FK_RD' ");
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.CCD_CD = SD.SENSOR_FK_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ERR_STA_FK1_RD");
	strSql += CRLF + _T("                          ON CCD_ERR_STA_FK1_RD.CDX_CD = 'ERR_STA_FK1_RD' ");
	strSql += CRLF + _T("                         AND CCD_ERR_STA_FK1_RD.CCD_CD = SD.ERR_STA_FK1_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ERR_STA_FK2_RD");
	strSql += CRLF + _T("                          ON CCD_ERR_STA_FK2_RD.CDX_CD = 'ERR_STA_FK2_RD' ");
	strSql += CRLF + _T("                         AND CCD_ERR_STA_FK2_RD.CCD_CD = SD.ERR_STA_FK2_RD ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN JOB_MST JM_FK1 ");
	strSql += CRLF + _T("                          ON JM_FK1.LUGG_NO = SD.ITN_LUGG_FK1 ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN JOB_MST JM_FK2 ");
	strSql += CRLF + _T("                          ON JM_FK2.LUGG_NO = SD.ITN_LUGG_FK2 ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN (SELECT MC_NO, MC_NO_NM	");
	strSql += CRLF + _T("                                     FROM CV_DATA		");
	strSql += CRLF + _T("                                    UNION				");
	strSql += CRLF + _T("								    SELECT MC_NO, MC_NO_NM	");
	strSql += CRLF + _T("                                     FROM SC_DATA) MC_MST_START ");
	strSql += CRLF + _T("                          ON MC_MST_START.MC_NO = JM_FK1.START_POS ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN (SELECT MC_NO, MC_NO_NM	");
	strSql += CRLF + _T("                                     FROM CV_DATA		");
	strSql += CRLF + _T("                                    UNION				");
	strSql += CRLF + _T("								    SELECT MC_NO, MC_NO_NM	");
	strSql += CRLF + _T("                                     FROM SC_DATA) MC_MST_DEST	");
	strSql += CRLF + _T("                          ON MC_MST_DEST.MC_NO = JM_FK1.DEST_POS ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS		");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM_FK1.JOB_STATUS ");

	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ERRCODE		");
	strSql += CRLF + _T("                          ON CCD_ERRCODE.CDX_CD = 'ERRCODE' ");
	strSql += CRLF + _T("                         AND CCD_ERRCODE.CCD_CD = SD.ERR_CODE_RD ");

	strSql += CRLF + _T(" WHERE SD.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND SD.PLC_NO = ") + CLib::Quot(pSC_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND SD.MC_NO = ") + CLib::Quot(pSC_DATA->K_SC_NO);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CScSkinDlg::GetQrySelectSC_DATA(CSC_DATA* pSC_DATA)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(SD.SUSPEND) AS SUSPEND");											//입출고금지상태
	strSql += CRLF + _T("  FROM SC_DATA SD");
	strSql += CRLF + _T(" WHERE SD.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND SD.PLC_NO = ") + CLib::Quot(pSC_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND SD.MC_NO = ") + CLib::Quot(pSC_DATA->K_SC_NO);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CScSkinDlg::OnBnClickedBtnScManualRet()
{
	//CString strSC_NO = m_pSC_DATA->K_SC_NO;
	//m_pDoc->OnCreateScManualRet(strSC_NO);

	//return;

	//CString strLuggNo = _T("");
	//CString strHsMcNo = _T("");
	//CString strSensor0DataRd = _T("");
	//CString strItnLuggNo = m_pSC_DATA->V_ITN_LUGG; //진행중인 작업번호 미리 담기.
	//CString strProductSize = _T("");
	//CString strLOG_MSG = _T("");

	//if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출은 비상 시 사용합니다. 하시겠습니까? ")), MB_YESNO) != IDYES)
	//	return;

	//if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS에 화물이 있습니까? ")), MB_YESNO) != IDYES)
	//	return;


	////화물감지유무 확인.
	//if (RetHsDataSelect(strHsMcNo, strSensor0DataRd) == FALSE)
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS 화물유무감지 조회중 실패")));
	//	return;
	//}

	//if (strSensor0DataRd == _T("0"))
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS에 화물이 없습니다. 센서를 확인해주세요.")));
	//	return;
	//}
	////
	//////SC 화물 없어야됨.
	////if (strSensorFkRd == _T("1") || strSensorFkRd == _T("3"))
	////{
	////	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC에 화물이 있어서 강제배출을 할 수 없습니다.")));
	////	return;
	////}
	////
	////Sc 포크 센터여야 함.
	////if (strForkPosRd != _T("0"))
	////{
	////	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 포크가 센터가 아니어서 강제배출을 할 수 없습니다.")));
	////	return;
	////}

	//m_pDoc->BeginTrans_DLG();

	////SC DATA 삭제
	//if (ScDataDelete() == FALSE)
	//{
	//	m_pDoc->RollbackTrans_DLG();
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 데이터 삭제중 실패")));
	//	return;
	//}

	////해당 작업정보 조회 및 삭제(없으면 안함)
	//if (JobMstDelete(strItnLuggNo) == FALSE)
	//{
	//	m_pDoc->RollbackTrans_DLG();
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 삭제중 실패")));
	//	return;
	//}

	////반자동작업 생성
	//if (JobMstInsert(strHsMcNo, strLuggNo) == FALSE)
	//{
	//	m_pDoc->RollbackTrans_DLG();
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("반자동 작업 생성중 실패")));
	//	return;
	//}

	////출고HS DATA 쓰기
	//if (CvDataUpdate(strHsMcNo, strLuggNo) == FALSE)
	//{
	//	m_pDoc->RollbackTrans_DLG();
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS DATA 쓰기중 실패")));
	//	return;
	//}

	//strLOG_MSG.Format(_T("SC 강제 배출 BUTTON -> SC NO : %s , 삭제 작업번호 : %s, 생성 작업번호 : %s"), m_pSC_DATA->K_SC_NO, strItnLuggNo, strLuggNo);
	//if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), _T(""), _T(""), strLOG_MSG))
	//{
	//	m_pDoc->RollbackTrans_DLG();
	//	return;
	//}

	//m_pDoc->CommitTrans_DLG();
	//AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출 성공")));
	//return;
}

BOOL CScSkinDlg::RetHsDataSelect(CString& strHS_MC_NO, CString& strSENSOR0_DATA_RD)
{
	//CString strHS_NO = _T("");
	//strHS_MC_NO = _T("");
	//strSENSOR0_DATA_RD = _T("");

	//CString strSql = _T("");
	//int nRowCnt = 0;
	//CString strMessage = _T("");

	////해당 SC의 출고 HS 구하기
	//strSql.Format(_T("  SELECT SHD.HS_MC_NO, CD.SENSOR0_DATA_RD	")
	//	_T("	  FROM SC_HS_DEF SHD					")
	//	_T("   INNER JOIN CV_DATA CD				    ")
	//	_T("           ON SHD.HS_MC_NO = CD.MC_NO 	")
	//	_T("   WHERE SHD.WH_TYP = '%s' 				")
	//	_T("     AND SHD.HS_NO = '02' 				")
	//	_T("     AND SHD.SC_NO = '%s'				    "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_SC_NO);

	//_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	//CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	//if (nRowCnt <= 0)
	//	return FALSE;

	//pRsw->MoveFirst();
	////출고 HS 번호, 화물감지 유무
	//strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));
	//strSENSOR0_DATA_RD = pRsw->GetItem(_T("SENSOR0_DATA_RD"));

	////pRsw->MoveNext();
	//delete pRsw;

	//if (nRowCnt <= 0)
	//	return FALSE;

	return TRUE;

}

BOOL CScSkinDlg::ScDataDelete()
{
	CString strSql = _T("");

	//해당 SC DATA 삭제
	strSql.Format(_T("UPDATE SC_DATA_MURATA 				\n")
		_T("	 SET CMD_RQ_ID = 'DELFK1'   \n")
		_T("	   , CMD_RQ_YN = 'Y'		 \n")
		_T("    WHERE WH_TYP = '%s'		\n")
		_T("      AND PLC_NO = '%s'		\n")
		_T("      AND SC_NO = '%s'		  "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;

	//strSql.Format(_T("UPDATE SC_DATA 					\n")
	//              _T("   SET JOB_TYP_OD = '0'			 \n")
	//              _T("      ,LUGG_NO_FK1_OD = '0000'	 \n")
	//              _T("      ,ITN_LUGG_FK1 = '0'			 \n")
	//              _T("      ,START_BANK_FK1_OD = '00'	 \n")
	//              _T("      ,START_BAY_FK1_OD = '000'	 \n")
	//              _T("      ,START_LEVEL_FK1_OD = '00'	 \n")
	//              _T("      ,START_HSPOS_FK1_OD = '0'	 \n")
	//              _T("      ,DEST_BANK_FK1_OD = '00'	 \n")
	//              _T("      ,DEST_BAY_FK1_OD = '000'	 \n")
	//              _T("      ,DEST_LEVEL_FK1_OD = '00'	 \n")
	//              _T("      ,DEST_HSPOS_FK1_OD = '0'	 \n")
	//              _T("      ,OD_RQ_YN = 'Y'				 \n")
	//			  _T("    WHERE WH_TYP = '%s'			 \n")
	//	          _T("      AND PLC_NO = '%s'			 \n")
	//	          _T("      AND SC_NO = '%s'		       "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);
	//
	//int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	//
	//if(isSuccess == TRUE)
	//{
	//	return TRUE;
	//}
	//return FALSE;
}

BOOL CScSkinDlg::JobMstDelete(CString pLuggNo)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//SC의 작업 작업번호가 작업정보에 있는지 확인
	strSql.Format(_T("  SELECT *						")
		_T("	  FROM JOB_MST					")
		_T("   WHERE LUGG_NO = '%s'		    "), pLuggNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt == 1)
	{
		pRsw->MoveFirst();

		delete pRsw;

		//해당 작업정보 삭제
		strSql.Format(_T("DELETE FROM JOB_MST 	 \n")
			_T(" WHERE LUGG_NO = '%s'	 \n"), pLuggNo);

		int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == TRUE)
		{
			//자동 작업인 경우 메시지 보여주기
			if (pLuggNo.Left(1) != _T("9"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업을 WMS에서도 삭제해주세요.")));
			}
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CScSkinDlg::JobMstInsert(CString pHsMcNo, CString& strNextVal)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");
	strNextVal = _T("");

	//UpdateData(TRUE);
	//CString strDestPos = _T("");
	//m_cbxDestPos.GetWindowText(strDestPos);

	//해당 SC의 출고 HS 구하기
	strSql.Format(_T("  SELECT NEXTVAL('MANUAL_SEQ') "));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//seq에서 작업에 사용할 번호 가져오기
	strNextVal = pRsw->GetItem(_T("nextval"));

	//pRsw->MoveNext();
	delete pRsw;

	//작업대이동 반자동작업 생성
	strSql.Format(_T("INSERT INTO JOB_MST (WH_TYP					\n")
		_T("					  ,LUGG_NO					\n")
		_T("					  ,START_POS 				\n")
		_T("					  ,START_LOCATION 			\n")
		_T("					  ,DEST_POS 				\n")
		_T("					  ,DEST_LOCATION 			\n")
		_T("					  ,PRODUCT_SIZE 			\n")
		_T("					  ,JOB_TYP 					\n")
		_T("					  ,JOB_STATUS 				\n")
		_T("					  ,JOB_PRIORITY 			\n")
		_T("					  ,HS_TRACK_NO 				\n")
		_T("					  ,SC_NO		 			\n")
		_T("					  ,INS_DT 					\n")
		_T("					  ,INS_USER_ID)				\n")
		_T("			   VALUES ('%s'						\n")
		_T("			          ,'%s'						\n") // 작업번호
		_T("			          ,'%s'						\n") // 출발지 해당 SC
		_T("			          ,'00-000-00'				\n") // 출발 로케이션 '00-000-00'
		_T("			          ,'148'					\n") // 도착지 '148'
		_T("			          ,'00-000-00'				\n") // 도착 로케이션 '00-000-00'
		_T("			          ,'0'						\n") // 펄프 단수 '0'
		_T("			          ,'6'						\n") // 작업구분 '6'(작업대이동)
		_T("			          ,'11'				    	\n") // 작업상태 '11' (CV구동중)
		_T("			          ,'100'					\n") // 우선순위 '100'
		_T("			          ,'%s'						\n") // 출고 HS
		_T("			          ,'%s'						\n") // SC 번호
		_T("			          ,") + m_pDoc->SYSDATE + _T("\n")
		_T("			          ,'%s')					 \n"), m_pSC_DATA->K_WH_TYP, strNextVal, m_pSC_DATA->K_SC_NO, pHsMcNo, m_pSC_DATA->K_SC_NO, m_pDoc->m_strId);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScSkinDlg::CvDataUpdate(CString strMcNo, CString strLuggNo)
{
	CString strSql = _T("");

	//입고 HS 트랙 데이터 삭제
	strSql.Format(_T("  UPDATE CV_DATA									 \n")
		_T("			   SET LUGG_NO_OD = '%s'						 \n")
		_T("			      ,DEST_POS_OD = '148'						 \n")
		_T("			      ,JOB_TYP_OD = '6'							 \n")
		_T("			      ,OD_RQ_YN = 'Y'							 \n")
		_T("			 WHERE WH_TYP = '%s'							 \n")
		_T("			   AND MC_NO = '%s'								   "), strLuggNo, m_pSC_DATA->K_WH_TYP, strMcNo);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}





HBRUSH CScSkinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_EDT_SC_JOB_SC_SUSPEND)
	{
		if (m_pSC_DATA->V_SUSPEND == _T("0"))
		{
			hbr = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW);
			pDC->SetTextColor(BLACK);  // 텍스트 색
			pDC->AssertValid();

		}
		else
		{
			hbr = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW);
			pDC->SetTextColor(RED);  // 텍스트 색
			pDC->AssertValid();
		}
	}

	return hbr;
}

void CScSkinDlg::OnStnClickedLblScJobStartLoc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

