// BCRSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "BCRSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CBCRSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBCRSkinDlg, CSkinDialog)

CBCRSkinDlg::CBCRSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CBCRSkinDlg::IDD, pParent)
{
	m_pBCR_MST = NULL;
	m_pCV_DATA = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CBCRSkinDlg::CBCRSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CBCRSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pBCR_MST = NULL;
	m_pCV_DATA = NULL;
}

CBCRSkinDlg::~CBCRSkinDlg()
{
	this->DestroyWindow();
}

void CBCRSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBL_BARCODE, m_lblBarcode);
	DDX_Control(pDX, IDC_LBL_BCR_STA, m_lblBcrSta);
	DDX_Control(pDX, IDC_LBL_BCR_TRACK_NO, m_lblBcrTrackNo);
	DDX_Control(pDX, IDC_LBL_BCR_SUSPEND, m_lblBcrSuspend);
	DDX_Control(pDX, IDC_LBL_BCR_UPD_DT, m_lblBcrUpdDt);
	DDX_Control(pDX, IDC_LBL_CV_STO_READY, m_lblCvStoReady);

	DDX_Control(pDX, IDC_EDT_BCR_NO, m_edtBcrNo);
	DDX_Control(pDX, IDC_EDIT_BARCODE, m_edtBarcode);
	DDX_Control(pDX, IDC_EDIT_BCR_STA, m_edtBcrSta);
	DDX_Control(pDX, IDC_EDIT_BCR_TRACK_NO, m_edtBcrTrackNo);
	DDX_Control(pDX, IDC_EDIT_BCR_SUSPEND, m_edtBcrSuspend);
	DDX_Control(pDX, IDC_EDIT_BCR_UPD_DT, m_edtBcrUpdDt);
	DDX_Control(pDX, IDC_EDIT_CV_STO_READY, m_edtCvStoReady);

	DDX_Control(pDX, IDC_BTN_BCR_READ_BARCODE, m_btnBcrReadBarcode);
	DDX_Control(pDX, IDC_BTN_BCR_WRITE_MANUAL, m_btnBcrWriteManual);
	DDX_Control(pDX, IDC_BTN_BCR_SUSPEND, m_btnBcrSuspend);
	DDX_Control(pDX, IDC_BTN_BCR_MES_REPORT, m_btnBcrMesReport);
	DDX_Control(pDX, IDC_BTN_ACTIVE, m_btnBcrActive);

	DDX_Control(pDX, IDC_GRP_BCR_COMMAND, m_grpBcrCommand);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT, m_grpBcrReport);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT_ITEM, m_grpBcrReportItem);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT_VALUE, m_grpBcrReportValue);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS, m_grpBcrTrackStatus);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS_ITEM, m_grpBcrTrackStatusItem);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS_VALUE, m_grpBcrTrackStatusValue);
	DDX_Control(pDX, IDC_GRP_BCR_SUSPEND, m_grpBcrSuspend);

	DDX_Control(pDX, IDC_LIST_BCR_RCV_VAL, m_listBoxBcrListRcvVal);
	DDX_Control(pDX, IDC_LIST_BCR_UPD_DT, m_listBoxBcrListUpdDt);

	DDX_Control(pDX, IDC_CHK_AUTO_SEL, m_chkAutoSel);
}

BOOL CBCRSkinDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if (!m_bInitialized)
	{
		RelocationControls();
		m_bInitialized = TRUE;
	}

	//listFont.CreatePointFont(150, _T("Arial"));
	//m_listBoxBcrListRcvVal.SetFont(&listFont);
	//m_listBoxBcrListUpdDt.SetFont(&listFont);

	InitializeResource(pEn);

	CRect rect;							// 윈도우 기준 절대 좌표 
	GetDlgItem(IDC_STATIC_SPREAD)->GetWindowRect(&rect);

	CRect rectTemp = rect;				// 다이얼 로그 영역안의 컨트롤(rect)의 상대좌표 
	ScreenToClient(&rectTemp);

	CRect rtTemp;
	::GetWindowRect(this->m_hWnd, &rtTemp);

	CRect rectMargin = CRect(rectTemp.left, rectTemp.top, rtTemp.right - rect.right, rtTemp.bottom - rect.bottom);

	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_colorBaseBack = WHITE;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL | SS_GRID_VERTICAL | SS_GRID_SOLID;

	m_SpreadSheet.m_rectMargin = rectMargin;

	HFONT hFontH = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	HFONT hFontB = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	m_SpreadSheet.AddColHead(_T("BCR Vale"), 20);
	m_SpreadSheet.AddColHead(_T("읽은시간"), 34);

	m_chkAutoSel.SetCheck(0);
	m_blAutoSel = false;

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBCRSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CBCRSkinDlg::RenameResource(EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manual_write"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_BCR_WRITE_MANUAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manual_read"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_BCR_READ_BARCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("mes_report"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_BCR_MES_REPORT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcractive"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_BCR_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("barcode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BARCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsta"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BCR_STA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("trackinfo"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BCR_TRACK_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsuspendstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BCR_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrupdatetime"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BCR_UPD_DT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvstoready"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_STO_READY, strValue);

	//grp
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrrep "), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_REPORT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item "), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_REPORT_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value "), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_REPORT_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("tracksta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_BCR_SUSPEND, strValue);
}

void CBCRSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath2.Format(_T("%s"), chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");

	m_btnBcrReadBarcode.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrReadBarcode.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);

	m_btnBcrWriteManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrWriteManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnBcrSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);

	m_btnBcrMesReport.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrMesReport.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);

	m_btnBcrActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
}

BEGIN_MESSAGE_MAP(CBCRSkinDlg, CSkinDialog)
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CBCRSkinDlg::OnMessageSwitch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_BCR_READ_BARCODE, &CBCRSkinDlg::OnBnClickedBtnBcrReadBarcode)
	ON_BN_CLICKED(IDC_BTN_BCR_WRITE_MANUAL, &CBCRSkinDlg::OnBnClickedBtnBcrWriteManual)
	ON_BN_CLICKED(IDC_BTN_BCR_SUSPEND, &CBCRSkinDlg::OnBnClickedBtnBcrSuspend)
	ON_BN_CLICKED(IDC_BTN_BCR_MES_REPORT, &CBCRSkinDlg::OnBnClickedBtnBcrMesReport)
	ON_BN_CLICKED(IDC_BTN_ACTIVE, &CBCRSkinDlg::OnBnClickedBtnActive)
	ON_EN_CHANGE(IDC_EDIT_BARCODE, &CBCRSkinDlg::OnEnChangeEditBarcode)
	ON_MESSAGE(SSM_CLICK, &CBCRSkinDlg::OnSpreadLClick)
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL, &CBCRSkinDlg::OnBnClickedChkAutoSel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

LRESULT CBCRSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
{
	CBCR_MST* pBCR_MST = (CBCR_MST*)wParam;
	if (pBCR_MST == NULL) { return 0; };
	m_pBCR_MST = pBCR_MST;
	if (m_pCV_DATA == NULL)
	{
		m_pCV_DATA = (CCV_DATA*)(m_pDoc->GetTrackInfoNew(m_pBCR_MST->m_strBCR_MC_NO)->m_pCV_DATA);
	}
	if (m_pCV_DATA == NULL) { return 0; };

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if (m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	InvalidateBcrData(enLangTemp);
	InitializeSpread(0, TRUE);
	return 0;
}

// CRtvSkinDlg 메시지 처리기입니다.
void CBCRSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x = 0, y = 0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnBcrReadBarcode.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrReadBarcode.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrWriteManual.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrWriteManual.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrMesReport.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrMesReport.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}

void CBCRSkinDlg::InvalidateBcrData(EN_LANG pLang)
{
	if (m_pDoc == NULL) return;
	if (m_pBCR_MST == NULL) return;
	if (m_pCV_DATA == NULL) return;

	if (!UpdateData())
		return;
	CString strView;
	strView.Format(_T("7%02s (BCR %s >> TRACK %s)"), m_pBCR_MST->K_BCR_NO, m_pBCR_MST->K_BCR_NO, m_pBCR_MST->m_strBCR_MC_NO);
	m_edtBcrNo.SetWindowText(strView);
	m_edtBcrTrackNo.SetWindowText(m_pBCR_MST->m_strBCR_MC_NO);//BCR_MC_NO

	CString strSql = GetQrySelectBCR_STATUS_CCD(m_pBCR_MST);
	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		m_edtBarcode.SetWindowText(pRsw->GetItem(_T("BCR_RCV_VAL")));
		m_edtBcrSta.SetWindowText(pRsw->GetItem(_T("BCR_STA")));
		m_edtBcrSuspend.SetWindowText(pRsw->GetItem(_T("SUSPEND")));
		m_edtBcrUpdDt.SetWindowText(pRsw->GetItem(_T("UPD_DT")));
		m_edtCvStoReady.SetWindowText(pRsw->GetItem(_T("CV_STO_READY")));
	}

	/*strSql = GetQrySelectCV_STATUS_CCD();
	strMessage = "";
	nRowCnt = -1;
	ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if (nRowCnt <= 0) { return; }
	pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst();
	for (int i = 0; i < nRowCnt; i++)
	{
		m_edtCvStoReady.SetWindowText(pRsw->GetItem(_T("CV_STO_READY")));
	}*/

	UpdateData(FALSE);
	InvalidateLangControl(pLang);
}

void CBCRSkinDlg::InvalidateLangControl(EN_LANG enLangTemp)
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if (m_pDoc == NULL)
		return;

	//언어추가할것
	//m_lblScJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
}

void CBCRSkinDlg::OnClose()
{
	m_pDoc->m_pBcrSkinDlg = NULL;
	delete this;
}

void CBCRSkinDlg::OnBnClickedBtnBcrReadBarcode()
{
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 수동읽기를 지시하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;

	strSql.Format(_T(" UPDATE BCR_DATA		\n")
		_T("    SET BCR_STA = '1'			\n")
		_T("       ,CMD_RQ_YN = 'Y'         \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND BCR_NO= '%03s'			  "), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 수동읽기 지시 성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 수동읽기 지시 실패")));
	return;
}

void CBCRSkinDlg::OnBnClickedBtnBcrWriteManual()
{
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 HOST전송을 지시하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;
	CString strBcrMcNo = m_pBCR_MST->V_BCR_MC_NO;
	CString strBarcode = _T("");

	UpdateData(TRUE);
	m_edtBarcode.GetWindowText(strBarcode);

	if (strBarcode == _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 데이터 확인")));
		return;
	}

	if (strBarcode == _T(""))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 데이터 확인")));
		return;
	}

	if (IsValidataEditData(strBarcode) == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 데이터 확인")));
		return;
	}

	strSql.Format(_T(" UPDATE BCR_DATA		\n")
		_T("              SET BCR_STA = '2'         \n")
		_T("                 ,BCR_RCV_VAL = '%s'         \n")
		_T("            WHERE WH_TYP = '%s'			\n")
		_T("	          AND BCR_NO= '%03s'			  "), strBarcode, strWhTyp, strBcrNo);
	
	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		strSql.Format(_T(" UPDATE CV_DATA		\n")
			_T("              SET BARCODE = '%s'         \n")
			_T("            WHERE WH_TYP = '%s'			\n")
			_T("	          AND MC_NO= '%03s'			  "), strBarcode, strWhTyp, strBcrMcNo);

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == TRUE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 HOST전송 지시 성공")));
			return;
		}
		else
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 HOST전송 지시 성공 , CV DATA BARCODE 입력 실패")));
			return;
		}

	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 HOST전송 지시 실패")));
	return;
}

BOOL CBCRSkinDlg::IsValidataEditData(CString strBarcode)
{
	if (strBarcode.GetLength() > 7) { return FALSE; }
	return TRUE;
}

void CBCRSkinDlg::OnBnClickedBtnBcrSuspend()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;

	if (m_pBCR_MST->V_SUSPEND == _T("Y"))
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 중지를 해제하시겠습니까?")), MB_YESNO) != IDYES)
			return;

	if (m_pBCR_MST->V_SUSPEND == _T("N"))
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 사용을 중지하시겠습니까?")), MB_YESNO) != IDYES)
			return;

	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = _T("0");
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
	CString strLOG_BARCODE = _T("");
	CString strLOG_MSG = _T("BCR SUSPEND. BCR NO : ") + strBcrNo;
	CString strSUSPEND;

	if (m_pBCR_MST->V_SUSPEND == _T("Y"))
		strSUSPEND = "N";
	if (m_pBCR_MST->V_SUSPEND == _T("N"))
		strSUSPEND = "Y";

	if (!m_pDoc->GetQueryInsertClientLog(_T("CBcrSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strSql.Format(_T(" UPDATE BCR_DATA				\n")
		_T("    SET SUSPEND = '%s'       \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND BCR_NO= '%03s'			  "), strSUSPEND, strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 상태 변경 성공")));
		if (strSUSPEND == "N")
		{
			m_edtBcrSuspend.SetWindowTextW(_T("정상"));
			m_btnBcrSuspend.SetWindowTextW(_T("BCR정지"));
		}
		else
		{
			m_edtBcrSuspend.SetWindowTextW(_T("정지"));
			m_btnBcrSuspend.SetWindowTextW(_T("정지해제"));
		}

		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 상태 변경 실패")));
	return;
}

void CBCRSkinDlg::OnBnClickedBtnBcrMesReport()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;

	strSql.Format(_T(" UPDATE BCR_DATA				\n")
		_T("    SET BCR_STA = '2'			\n")
		_T("	  , CMD_RQ_YN = 'Y'       \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	  AND BCR_NO= '%03s'			  "), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 상위 보고 성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 상위 보고 실패")));
	return;
}

void CBCRSkinDlg::OnBnClickedBtnActive()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;

	if (m_pBCR_MST->V_SUSPEND == _T("0"))
		return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("BCR을 사용하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = _T("");
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
	CString strLOG_BARCODE = _T("");
	CString strLOG_MSG = _T("BCR ACTIVE. BCR NO : ") + strBcrNo;

	if (!m_pDoc->GetQueryInsertClientLog(_T("CBcrSkinDlg"), strLOG_LUGG_NO, strLOG_BARCODE, strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strSql.Format(_T(" UPDATE BCR_DATA				\n")
		_T("    SET SUSPEND = '0'			   \n")
		_T("  WHERE WH_TYP = '%s'			  \n")
		_T("	AND BCR_NO= '%03s'			  \n")
		_T("	AND SUSPEND = '1'			"), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 ACTIVE 지시 성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 ACTIVE 지시 실패")));
	return;
}

CString CBCRSkinDlg::GetQrySelectBCR_STATUS_CCD(CBCR_MST* pBCR_MST)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(BD.BCR_NO,'000') AS BCR_NO ");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(BD.BCR_MC_NO,'000') AS BCR_MC_NO ");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_BCR_STA.CCD_NM_KOR,'0') AS BCR_STA ");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(BD.BCR_RCV_VAL,'0') AS BCR_RCV_VAL ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, '0') AS SUSPEND ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(BD.CHK_BYPASS_YN, '0') AS CHK_BYPASS_YN ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_CV_STA.CCD_NM_KOR,'0') AS CV_STO_READY ");
	strSql += CRLF + _T("	   ,BD.UPD_DT AS UPD_DT ");
	strSql += CRLF + _T("				   FROM BCR_DATA BD ");
	strSql += CRLF + _T("				  INNER JOIN CV_DATA CD                             ");
	strSql += CRLF + _T("					 ON BD.BCR_MC_NO = CD.MC_NO                              ");
	strSql += CRLF + _T("				   LEFT OUTER JOIN COMMON_CODE CCD_BCR_STA");
	strSql += CRLF + _T("                          ON CCD_BCR_STA.CDX_CD = 'BCR_STA' ");
	strSql += CRLF + _T("                         AND CCD_BCR_STA.CCD_CD = BD.BCR_STA ");
	strSql += CRLF + _T("                         AND CCD_BCR_STA.WH_TYP LIKE ") + CLib::QuotLikeLR(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND ");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'BCR_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = BD.SUSPEND ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.WH_TYP LIKE ") + CLib::QuotLikeLR(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_CV_STA ");
	strSql += CRLF + _T("                          ON CCD_CV_STA.CDX_CD = 'CV_STO_READY' ");
	strSql += CRLF + _T("                         AND CCD_CV_STA.CCD_CD = CD.STO_READY_RD ");
	strSql += CRLF + _T("                         AND CCD_CV_STA.WH_TYP LIKE ") + CLib::QuotLikeLR(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T(" WHERE BD.WH_TYP = ") + CLib::Quot(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("   AND BD.BCR_NO = '") + _T("0") + pBCR_MST->K_BCR_NO + _T("'");

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CBCRSkinDlg::GetQrySelectCV_STATUS_CCD()
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_CV_STA.CCD_NM_KOR,'0') AS CV_STO_READY ");
	strSql += CRLF + _T("  FROM CV_DATA CD LEFT OUTER JOIN COMMON_CODE CCD_CV_STA");
	strSql += CRLF + _T("                          ON CCD_CV_STA.CDX_CD = 'CV_STO_READY' ");
	strSql += CRLF + _T("                         AND CCD_CV_STA.CCD_CD = CD.STO_READY_RD ");

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CBCRSkinDlg::OnEnChangeEditBarcode()
{
}

void CBCRSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_SpreadSheet.FinishLoadSpread();
}

int CBCRSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
{
	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main(nRowCheck, bSearch);

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);

	if (nRowCnt < 0)
	{
		if (pRsw != NULL)
		{
			delete pRsw;
		}
	}
	else
	{
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		pRsw->MoveFirst();

		for (int nIdxRow = 1; nIdxRow <= 5; nIdxRow++)
		{
			m_SpreadSheet.SetCurrentRow();
			for (int nIdxCol = nIdxStart; nIdxCol < 2; nIdxCol++)
			{
				CString strColValue = pRsw->GetItem(arrColName[(nIdxRow - 1) * 2 + nIdxCol]);
				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((strColValue.GetLength() * 1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength() * 1.5;
					arrColSize[nIdxCol] = CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨
				//m_SpreadSheet.SetData(nIdxCol, nIdxRow, strColValue);		// 번역안됨

				// 짝수마다 색깔을 다르게 칠할것!
				if (nIdxRow % 2 == 0)
				{
					m_SpreadSheet.SetColor(nIdxCol + 1, nIdxRow, m_pDoc->m_pConfig->m_clrSPREAD_COLOR, BLACK);
				}
				else
				{
					m_SpreadSheet.SetColor(nIdxCol + 1, nIdxRow, SPREAD_COLOR_NONE, BLACK);
				}

			}
			//pRsw->MoveNext();
		}
		if (pRsw != NULL)
		{
			delete pRsw;
		}
	}
	return nRowCnt;
}

int CBCRSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	nColIdx = pStrArrColName.GetCount();
	int nStartCol = 0;
	if (pStrArrColName[0].Right(5) == _T("_HIDE"))
	{
		nStartCol = -1;
	}

	//SetMaxCols(nColIdx + nStartCol);

	CString strTemp = _T("");
	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);
	//int nNewColIdx = 0;
	for (int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		int nSize = arrColRename[nIdxCol].GetLength();

		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
		{
			nSize = 0;
			if (nIdxCol == 0)
				nSize = 3;

			continue;
		}
		//++nNewColIdx;
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	//nStartCol = 1;
	for (int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];

		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
			continue;

		SetColumnText(nIdxCol + nStartCol, 0, arrColRename[nIdxCol]);
	}

	return 0;
}

void CBCRSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T("");
	strColValue = _T(" ") + strColValue;

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

CString CBCRSkinDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	if (m_pBCR_MST == NULL) 
		return _T("");

	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strRowCnt;

	strSql += CRLF + _T("		SELECT BCR_RCV_VAL1  AS BCR_RCV_VAL1");
	strSql += CRLF + _T("		      ,UPD_DT1		 AS UPD_DT1		");
	strSql += CRLF + _T("		      ,BCR_RCV_VAL2  AS BCR_RCV_VAL2");
	strSql += CRLF + _T("		      ,UPD_DT2		 AS UPD_DT2		");
	strSql += CRLF + _T("		      ,BCR_RCV_VAL3  AS BCR_RCV_VAL3");
	strSql += CRLF + _T("		      ,UPD_DT3		 AS UPD_DT3		");
	strSql += CRLF + _T("		      ,BCR_RCV_VAL4  AS BCR_RCV_VAL4");
	strSql += CRLF + _T("		      ,UPD_DT4		 AS UPD_DT4		");
	strSql += CRLF + _T("		      ,BCR_RCV_VAL5  AS BCR_RCV_VAL5");
	strSql += CRLF + _T("		      ,UPD_DT5		 AS UPD_DT5		");
	strSql += CRLF + _T("		  FROM BCR_DATA			");
	strSql += CRLF + _T("		 WHERE WH_TYP = ") + CLib::Quot(m_pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("		   AND BCR_NO = '") + _T("0") + m_pBCR_MST->K_BCR_NO + _T("'");

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

LRESULT CBCRSkinDlg::OnSpreadLClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if (pCell->Col == 0 || pCell->Row == 0)
		return 0;

	m_nActiveRow = pCell->Row;

	CString strBcrRcvVal;
	CString strBcrRecTime;

	strBcrRcvVal = m_SpreadSheet.GetValueTXT(1, m_nActiveRow);
	strBcrRcvVal.Trim();
	m_edtBarcode.SetWindowText(strBcrRcvVal);

	strBcrRecTime = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);
	strBcrRecTime.Trim();
	m_edtBcrUpdDt.SetWindowText(strBcrRecTime);

	m_chkAutoSel.SetCheck(0);
	m_blAutoSel = false;
	KillTimer(1);

	return 0;
}

void CBCRSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == TRUE)
	{
		InvalidateBcrData(EN_KOR);
		InitializeSpread(0, TRUE);
	}
}


void CBCRSkinDlg::OnBnClickedChkAutoSel()
{
	int nChk = m_chkAutoSel.GetCheck();

	if (nChk == 1)
	{
		if (m_blAutoSel == false)
		{
			SetTimer(1, 1500, NULL);
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
