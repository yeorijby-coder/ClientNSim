// LogIoSkinDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "ViewMesListDlg.h"


// CViewMesListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewMesListDlg, CSkinDialog)

	CViewMesListDlg::CViewMesListDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewMesListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewMesListDlg::CViewMesListDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewMesListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CViewMesListDlg::~CViewMesListDlg()
{
	m_pDoc->m_pViewMesListDlg = NULL;
	this->DestroyWindow();
}

BOOL CViewMesListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CViewMesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBL_MES_VIEW_MST_CNT,			 m_lblMESViewSpdMainCnt);
	DDX_Control(pDX, IDC_BTN_MES_VIEW_SEARCH,            m_btnMESViewSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_MST_RESULT,		 m_lblMESViewSpdMainResult);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_LUGGNO,            m_lblMESViewLuggno);
	DDX_Control(pDX, IDC_EDIT_MES_LOG,					 m_edtMESDetailLog);
	DDX_Control(pDX, IDC_GRP_MES_SEARCH,				 m_grpMesLogSearch);
	DDX_Control(pDX, IDC_GRP_UPDATE,					 m_grpMesLogUpdate);
	DDX_Control(pDX, IDC_GRP_MES_SEARCH2,				 m_grpMesLogSearch2);
	DDX_Control(pDX, IDC_STATIC_MES_LOG,				 m_grpMesLogMessage);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_WH_TYP,			 m_lblMESViewWhTyp);
	DDX_Control(pDX, IDC_CBX_MES_VIEW_WH_TYP,			 m_cbxMESViewWhTyp);
	DDX_Control(pDX, IDC_CBX_ROW_CNT,					 m_cbxRowCnt);
	DDX_Control(pDX, IDC_BTN_PRE,						 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT,						 m_btnNext);
	DDX_Control(pDX, IDC_STATIC_MES_LOG2,				 m_grpMesLogMessage2);
	DDX_Control(pDX, IDC_EDIT_MES_LOG2,					 m_edtMESDetailLog2);
	DDX_Control(pDX, IDC_CBX_MES_VIEW_JOB_KIND,			 m_cbxMesViewJobkind);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_JOB_KIND,			 m_lblMesViewJobkind);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_FROM_LOCATION,	 m_lblMesViewFrLoc);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_TO_LOCATION,	     m_lblMesViewToLoc);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_WORKSTATUS,        m_lblMesViewWorkSta);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_ERRORCODE,	     m_lblMesViewErrorCode);
	DDX_Control(pDX, IDC_LBL_MES_VIEW_BARCODE,			 m_lblMesViewBarcode);
	DDX_Control(pDX, IDC_EDT_MES_VIEW_LUGGNO,		     m_edtMESViewLuggno);
	DDX_Control(pDX, IDC_EDT_MES_VIEW_BARCODE,			 m_edtMESViewBarcode);
	DDX_Control(pDX, IDC_EDT_MES_VIEW_FROM_LOCATION,	 m_edtMESViewFromloc);
	DDX_Control(pDX, IDC_EDT_MES_VIEW_TO_LOCATION,		 m_edtMESViewToloc);
	DDX_Control(pDX, IDC_CBX_MES_VIEW_WORK_STATUS,		 m_cbxWorkStatus);
	DDX_Control(pDX, IDC_CBX_MES_VIEW_ERRCODE,			 m_cbxErrCode);
	DDX_Control(pDX, IDC_VIEW_MES_VIEW_EDIT_ERRORCODE,   m_edtMESViewErrorCode);
	DDX_Control(pDX, ID_MES_UPDATE,						 m_btnMESUpdate);
	DDX_Control(pDX, IDC_CHK_MES_VIEW_AUTO_SEL,			 m_chkAutoSel);
}


BEGIN_MESSAGE_MAP(CViewMesListDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SPD_LOG_IO, &CViewMesListDlg::OnBnClickedSpread)
	ON_BN_CLICKED(IDC_BTN_MES_VIEW_SEARCH, &CViewMesListDlg::OnBnClickedBtnViewMesSearch)
	ON_BN_CLICKED(IDC_BTN_PRE, &CViewMesListDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CViewMesListDlg::OnBnClickedNextRow)
	ON_MESSAGE(SSM_CLICK, &CViewMesListDlg::OnSpreadLClick)
	ON_COMMAND(IDC_SPD_LOG_IO, &CViewMesListDlg::OnSpdLogIo)
	ON_UPDATE_COMMAND_UI(IDC_SPD_LOG_IO, &CViewMesListDlg::OnUpdateSpdLogIo)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_CBX_ROW_CNT, &CViewMesListDlg::OnSetfocusCmbRowCnt)
	ON_BN_CLICKED(ID_MES_UPDATE, &CViewMesListDlg::OnBnClickedMesUpdate)
	ON_BN_CLICKED(IDC_CHK_MES_VIEW_AUTO_SEL, &CViewMesListDlg::OnBnClickedChkMesViewAutoSel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CViewMesListDlg, CSkinDialog)
	ON_EVENT(CViewMesListDlg, IDC_SPD_LOG_MES, 5, CViewMesListDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
// CViewMesListDlg 메시지 처리기입니다.

void CViewMesListDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}

void CViewMesListDlg::OnClose()
{
	m_pDoc->m_pViewMesListDlg = NULL;
	delete this;
}

HCURSOR CViewMesListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewMesListDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CViewMesListDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CViewMesListDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;
	m_btnOk.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnOk.MoveWindow(rc2.left - 30, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCancel.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCancel.MoveWindow(rc2.left - 15, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnMESViewSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnMESViewSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CViewMesListDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;	//	기본은 한국어
	CTime tStartD, tStartT, tEndD, tEndT;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		RelocationControls();
		m_bInitialized = TRUE;	
	}

	CLib::BindCombo(m_cbxMESViewWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxMesViewJobkind, _T("JOB_KIND"),m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxWorkStatus, _T("WORK_STA"), m_pDoc, int(pEn), FALSE);
	CLib::BindCombo(m_cbxErrCode, _T("ERRCODE"), m_pDoc, int(pEn), FALSE);
	
	InitializeResource(pEn);
	m_pPreRowCnt = 0;
	m_chkAutoSel.SetCheck(0);
	m_blAutoSel = false;

	CTime tToDate = CTime::GetTickCount();
	CTimeSpan span(1,0,0,0); // (d,h,m,s)
	//tToDate -= span;
	int nYear = tToDate.GetYear();
	int nMonth = tToDate.GetMonth();
	int nDay = tToDate.GetDay();
	int nHour = tToDate.GetHour();
	int nMin = tToDate.GetMinute();
	int nSec = tToDate.GetSecond();

	CString strStartTime;
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nYear, nMonth, nDay, 00, 00, 00);
	COleDateTime tStartTime;

	tStartTime = CLib::ConvertCStringToCOleDateTime(strStartTime);

	CString strToTime;
	strToTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nYear, nMonth, nDay, 23, 59, 59);
	COleDateTime tstrToTime;

	tstrToTime = CLib::ConvertCStringToCOleDateTime(strToTime);

	#pragma region 스프레드 초기화 
	//IDC_CUSTOM1 라는 컨트롤에서 Rect 값을 가져와서 스프레드에 적용 
	// 상대 좌표 구하기
	CRect rect;							// 윈도우 기준 절대 좌표 
	GetDlgItem(IDC_STATIC_SPREAD)->GetWindowRect(&rect);

	CRect rectTemp = rect;				// 다이얼 로그 영역안의 컨트롤(rect)의 상대좌표 
	ScreenToClient(&rectTemp);
	 
	CRect rtTemp;
	::GetWindowRect(this->m_hWnd, &rtTemp);
	
	CRect rectMargin = CRect(rectTemp.left, rectTemp.top, rtTemp.right-rect.right, rtTemp.bottom - rect.bottom);

//	m_SpreadSheet.m_rectWnd = rectTemp;//CRect(0, 0, 0, 0);
	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_colorBaseBack = WHITE;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL | SS_GRID_VERTICAL | SS_GRID_SOLID;

	m_SpreadSheet.m_rectMargin = rectMargin;

	HFONT hFontH = CreateFont(15,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));
	HFONT hFontB = CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,_T("System"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	//m_SpreadSheet.AddColHead(_T(" "), 10);
	m_SpreadSheet.AddColHead(_T("창고타입"), 9);
	m_SpreadSheet.AddColHead(_T("작업번호"), 9);
	m_SpreadSheet.AddColHead(_T("작업구분"), 13);
	m_SpreadSheet.AddColHead(_T("상태정보"), 34);
	m_SpreadSheet.AddColHead(_T("처리상태"), 9);
	m_SpreadSheet.AddColHead(_T("에러메시지"), 25);
	m_SpreadSheet.AddColHead(_T("출발로케이션"), 12);
	m_SpreadSheet.AddColHead(_T("도착로케이션"), 12);
	m_SpreadSheet.AddColHead(_T("출발CV"), 9);
	m_SpreadSheet.AddColHead(_T("도착CV"), 9);
	m_SpreadSheet.AddColHead(_T("출발SC"), 9);
	m_SpreadSheet.AddColHead(_T("도착SC"), 9);
	m_SpreadSheet.AddColHead(_T("우선순위"), 9);
	m_SpreadSheet.AddColHead(_T("제품이름"), 15);
	m_SpreadSheet.AddColHead(_T("팔렛트바코드"), 15);
	m_SpreadSheet.AddColHead(_T("추가시간"), 21);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	return TRUE;
}

void CViewMesListDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnMESViewSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnMESUpdate.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnMESViewSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnMESUpdate.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
}

void CViewMesListDlg::RenameResource(EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_BTN_MES_VIEW_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	SetDlgItemText(IDOK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	SetDlgItemText(IDCANCEL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobkind"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_JOB_KIND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("barcode"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_BARCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_LUGGNO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	SetDlgItemText(IDC_GRP_MES_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	SetDlgItemText(IDC_GRP_MES_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("detail"), (int)enLang);
	SetDlgItemText(IDC_STATIC_MES_LOG, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("mrcdmsg"), (int)enLang);
	SetDlgItemText(IDC_STATIC_MES_LOG2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromloc"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_FROM_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("toloc"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_TO_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("workstatus"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_WORKSTATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorcode"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_VIEW_ERRORCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)enLang);
	SetDlgItemText(IDC_GRP_UPDATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)enLang);
	SetDlgItemText(ID_MES_UPDATE, strValue);
	//	예전에는 Ecs.rc 캡션 그대로라 언어를 바꿔도 안 바뀌던 것들
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_ifinfo\\"), _T("dlg_ifinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("autosel"), (int)enLang);
	SetDlgItemText(IDC_CHK_MES_VIEW_AUTO_SEL, strValue);
	
}

void CViewMesListDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}

void CViewMesListDlg::OnBnClickedBtnViewMesSearch()
{
	UpdateData(TRUE);

	if(m_pDoc == NULL) { return; };
	InitializeSpread(0, TRUE);
}

void CViewMesListDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
}
void CViewMesListDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
}
void CViewMesListDlg::OnSpdLogIo()
{
}
void CViewMesListDlg::OnUpdateSpdLogIo(CCmdUI *pCmdUI)
{
}
void CViewMesListDlg::OnBnClickedSpread()
{
}
void CViewMesListDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
}

////////SPREAD 값채우기
void CViewMesListDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblMESViewSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CViewMesListDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CViewMesListDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CViewMesListDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	nColIdx = pStrArrColName.GetCount();
	int nStartCol = 0;
	if (pStrArrColName[0].Right(5) == _T("_HIDE"))
	{
		nStartCol = - 1;
	}

	//SetMaxCols(nColIdx + nStartCol);

	CString strTemp = _T("");
	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);
	//int nNewColIdx = 0;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
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
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];
		
		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
			continue;

		SetColumnText(nIdxCol + nStartCol, 0, arrColRename[nIdxCol]);
		
	}
	//*/

	//nColIdx = nNewColIdx;

	return 0;
}

void CViewMesListDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");
	strColValue = _T(" ") + strColValue;

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CViewMesListDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

	if(nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
		}
	}
	else
	{	
		// arrColName과 arrColSize의 갯수가 다름!! Hide되어있는 Column을 가져오느라 어쩔수 없음~
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		//SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			m_SpreadSheet.SetCurrentRow();
			for (int nIdxCol = nIdxStart; nIdxCol < nColSize; nIdxCol++)		
			{
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);

				if (arrColName[nIdxCol] == "UPD_DT")
				{
					CString sTemp = _T("");
					int nTemp = _tstoi(strColValue.Mid(11, 2));
					if (nTemp > 12)
					{
						nTemp = nTemp - 12;
						sTemp = _T(" 오후 ");
					}
					else sTemp = _T(" 오전 ");

					strColValue = strColValue.Mid(0, 10) + sTemp + CConvert::ToString(nTemp) + strColValue.Mid(13, 6);
				}

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((strColValue.GetLength()*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*1.5;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨

				// 짝수마다 색깔을 다르게 칠할것!
				if (nIdxRow % 2 == 0)
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, m_pDoc->m_pConfig->m_clrSPREAD_COLOR, BLACK);
				}
				else
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, SPREAD_COLOR_NONE, BLACK);					
				}

			}
			pRsw->MoveNext();
		}

		if (pRsw != NULL)
		{
			delete pRsw;
		}
	}
	return nRowCnt;
}

void CViewMesListDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CViewMesListDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strWH_TYP = m_cbxMESViewWhTyp.GetItemKey(m_cbxMESViewWhTyp.GetCurSel());
	CString strJobKind = m_cbxMesViewJobkind.GetItemKey(m_cbxMesViewJobkind.GetCurSel());
	CString strLuggno, strBarcode, strFromloc, strToloc;
	m_edtMESViewLuggno.GetWindowText(strLuggno);
	m_edtMESViewBarcode.GetWindowText(strBarcode);
	m_edtMESViewFromloc.GetWindowText(strFromloc);
	m_edtMESViewToloc.GetWindowText(strToloc);

	CString strRowCnt;
	int nRowCnt;	
	int nRowSum;
	strRowCnt = m_cbxRowCnt.GetItemKey(m_cbxRowCnt.GetCurSel()); 
	nRowCnt = CConvert::ToInt(strRowCnt);
	if (nRowCheck == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }
	if (nRowCheck < 0) { nRowSum = m_pPreRowCnt - nRowCnt; }
	if (nRowCheck > 0) { nRowSum = m_pPreRowCnt + nRowCnt; }
	if (nRowSum == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }

	CString strPreRowCnt = CConvert::ToString(nRowSum - nRowCnt + 1);
	CString strNextRowCnt = CConvert::ToString(nRowSum);
	m_pPreRowCnt = nRowSum;

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, ILS.WH_TYP) AS WH_TYP			");
	strSql += CRLF + _T("      ,ILS.LUGGNO AS LUGGNO															");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_JOB_KIND.CCD_NM_KOR, ILS.JOB_KIND) AS JOB_KIND		");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_WORK_STA.CCD_NM_KOR, ILS.WORK_STA) AS WORK_STA		");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_IF_STATUS.CCD_NM_KOR, ILS.IF_STATUS) AS IF_STATUS	");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_ERRCODE.CCD_NM_KOR, ILS.ERRCODE) AS ERRCODE		");
	strSql += CRLF + _T("      ,ILS.FROM_AREA AS FROM_AREA														");
	strSql += CRLF + _T("      ,ILS.TO_AREA AS TO_AREA															");
	strSql += CRLF + _T("      ,ILS.FROM_CV_NO AS FROM_CV_NO													");
	strSql += CRLF + _T("      ,ILS.TO_CV_NO AS TO_CV_NO														");
	strSql += CRLF + _T("      ,ILS.FROM_SC_NO AS FROM_SC_NO													");
	strSql += CRLF + _T("      ,ILS.TO_SC_NO AS TO_SC_NO														");
	strSql += CRLF + _T("      ,ILS.PRIORITY AS PRIORITY														");
	strSql += CRLF + _T("      ,ILS.PRDCT_NM AS PRDCT_NM														");
	strSql += CRLF + _T("      ,ILS.LD_CTN_NO AS LD_CTN_NO														");
	strSql += CRLF + _T("      ,ILS.UPD_DT AS UPD_DT															");
	strSql += CRLF + _T("  FROM IF_LUGG_STA ILS															");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP											");
	strSql += CRLF + _T("                    ON ILS.WH_TYP = CCD_WH_TYP.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_WH_TYP.CDX_CD = 'WH_TYP'									");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_JOB_KIND										");
	strSql += CRLF + _T("                    ON ILS.JOB_KIND = CCD_JOB_KIND.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_JOB_KIND.CDX_CD = 'JOB_KIND'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WORK_STA										");
	strSql += CRLF + _T("                    ON ILS.WORK_STA = CCD_WORK_STA.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_WORK_STA.CDX_CD = 'WORK_STA'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_IF_STATUS										");
	strSql += CRLF + _T("                    ON ILS.IF_STATUS = CCD_IF_STATUS.CCD_CD							");
	strSql += CRLF + _T("                   AND CCD_IF_STATUS.CDX_CD = 'IF_STATUS'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_ERRCODE											");
	strSql += CRLF + _T("                    ON ILS.ERRCODE = CCD_ERRCODE.CCD_CD							    ");
	strSql += CRLF + _T("                   AND CCD_ERRCODE.CDX_CD = 'ERRCODE'									");
	strSql += CRLF + _T("				  WHERE ILS.WH_TYP =    												") + CLib::Quot(strWH_TYP);
	if (strJobKind != _T("") && strJobKind != _T("ALL"))
	{
		strSql += CRLF + _T("    AND ILS.JOB_KIND =  ") + CLib::Quot(strJobKind);
	}
	if (strLuggno != _T(""))
	{
		strSql += CRLF + _T("    AND ILS.LUGGNO LIKE '%") + strLuggno + _T("%'");
	}
	if (strBarcode != _T(""))
	{
		strSql += CRLF + _T("    AND ILS.LD_CTN_NO LIKE '%") + strBarcode + _T("%'");
	}
	if (strFromloc != _T(""))
	{
		strSql += CRLF + _T("    AND ILS.FROM_AREA LIKE '%") + strFromloc + _T("%'");
	}
	if (strToloc != _T(""))
	{
		strSql += CRLF + _T("    AND ILS.TO_AREA LIKE '%") + strToloc + _T("%'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY ILS.UPD_DT DESC");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#endif
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}


void CViewMesListDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CViewMesListDlg::OnBnClickedNextRow() 
{
	InitializeSpread(1, TRUE);
}

void CViewMesListDlg::ClickSpread(long Col, long Row) //셀클릭
{
}


void CViewMesListDlg::OnBnClickedMesUpdate()
{
	variant_t val;
	CString strSql;
	CString strWH_TYP, strLUGG_NO, strWorkStatus, strErrCode;

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오")));
		return;
	}

	strWH_TYP = m_SpreadSheet.GetValueTXT(1, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
	strWorkStatus = m_cbxWorkStatus.GetItemKey(m_cbxWorkStatus.GetCurSel());
	strErrCode = m_cbxErrCode.GetItemKey(m_cbxErrCode.GetCurSel());

	//m_edtMESViewErrorCode.GetWindowText(strErrCode);

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	//strErrCode.Trim();

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("상태정보 및 에러코드를 수정 하시겠습니까?  [ 작업번호 : ") + strLUGG_NO + _T(" ]")), MB_YESNO) != IDYES)
		return;

	UpdateData(TRUE);

	long bTrans = m_pDoc->BeginTrans_DLG();

	if (bTrans < 1)
		return;

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
	CString strLOG_MSG = _T("IF_LUGG_STA UPDATE : IF_LUGG_STA -> ") + strWorkStatus;
	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewMesListDlg"), strLOG_LUGG_NO, _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strWH_TYP = m_cbxMESViewWhTyp.GetItemCCD(strWH_TYP);
	strSql = _T("");
	strSql.Format(_T("UPDATE IF_LUGG_STA    ")
		_T("	  SET WORK_STA   = '%s'		")
		_T("	    , ERRCODE    = '%s'		")
		_T("	    , IF_STATUS  = 'N'  ")
		_T("    WHERE WH_TYP     = '%s'		")
		_T("      AND LUGGNO     = '%s'		"), strWorkStatus, strErrCode, strWH_TYP, strLUGG_NO);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("상태정보 및 에러코드 변경 실패")));
		return;
	}
	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("상태정보 및 에러코드 변경 성공")));
	InitializeSpread(TRUE);
}

LRESULT CViewMesListDlg::OnSpreadLClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if (pCell->Col == 0 || pCell->Row == 0)
		return 0;

	if (m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX))
	{
		BOOL bCheck = !m_SpreadSheet.GetValueINT(pCell->Col, pCell->Row);
		m_SpreadSheet.SetValueINT(pCell->Col, pCell->Row, bCheck);

		if (m_SpreadSheet.IsCellType(pCell->Col, 0, SS_TYPE_CHECKBOX) &&
			m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX))
		{
			if (!bCheck)  // Uncheck 시
				m_SpreadSheet.SetHeaderCellCheck(pCell->Col, false);
		}
		return 0;
	}

	//variant_t val;
	if (pCell->Row > 0)
	{
		m_nActiveRow = pCell->Row;

		//작업정보
		CString strWorkSta = m_SpreadSheet.GetValueTXT(4, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strWorkSta.Trim();
		m_cbxWorkStatus.SetCurSelTextEx(strWorkSta);

		//작업정보
		CString strErrCode = m_SpreadSheet.GetValueTXT(6, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strErrCode.Trim();
		m_cbxErrCode.SetCurSelTextEx(strErrCode);

		//CString strErrorCode = m_SpreadSheet.GetValueTXT(6, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		//strErrorCode.Trim();
		//if (strErrorCode.Mid(1, 1) == _T("(")) strErrorCode = strErrorCode.Mid(0, 1);
		//else if (strErrorCode.Mid(2, 1) == _T("(")) strErrorCode = strErrorCode.Mid(0, 2);
		//else if (strErrorCode.Mid(3, 1) == _T("(")) strErrorCode = strErrorCode.Mid(0, 3);
		//else if (strErrorCode.Mid(4, 1) == _T("(")) strErrorCode = strErrorCode.Mid(0, 4);
		//m_edtMESViewErrorCode.SetWindowText(strErrorCode);

		CLib::BindSpreadCommonCode(_T("WORK_STA"), strErrCode, m_strWORK_STA, m_pDoc);
		CLib::BindSpreadCommonCode(_T("ERRCODE"), strWorkSta, m_strERRCODE, m_pDoc);
	}

	return 0;
}

void CViewMesListDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == TRUE)
	{
		InitializeSpread(TRUE); (EN_KOR);
	}
}

void CViewMesListDlg::OnBnClickedChkMesViewAutoSel()
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
