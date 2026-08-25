// LogIoSkinDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "LogMesSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogMesSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogMesSkinDlg, CSkinDialog)

	CLogMesSkinDlg::CLogMesSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogMesSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogMesSkinDlg::CLogMesSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogMesSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogMesSkinDlg::~CLogMesSkinDlg()
{
	m_pDoc->m_pLogMesSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogMesSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogMesSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBL_MES_MST_CNT,				 m_lblSpdMainCnt);
	DDX_Control(pDX, IDC_BTN_LOG_IO_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);
	DDX_Control(pDX, IDC_LBL_MES_TO_DATE,				 m_lblToDate);
	DDX_Control(pDX, IDC_LBL_MES_FROM_DATE,				 m_lblFromDate);
	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);
	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_LBL_MES_MST_RESULT,			 m_lblSpdMainResult);
	DDX_Control(pDX, IDC_LBL_MES_LUGGNO,                 m_lblMESLuggno);
	DDX_Control(pDX, IDC_EDIT_MES_LOG,					 m_edtMESDetailLog);
	DDX_Control(pDX, IDC_GRP_MES_SEARCH,				 m_grpMesLogSearch);
	DDX_Control(pDX, IDC_GRP_MES_SEARCH2,				 m_grpMesLogSearch2);
	DDX_Control(pDX, IDC_STATIC_MES_LOG,				 m_grpMesLogMessage);
	DDX_Control(pDX, IDC_LBL_MES_LOG_WH_TYP,			 m_lblMESLogWhTyp);
	DDX_Control(pDX, IDC_CBX_MES_LOG_WH_TYP,			 m_cbxMESLogWhTyp);
	DDX_Control(pDX, IDC_CBX_ROW_CNT,					 m_cbxRowCnt);
	DDX_Control(pDX, IDC_BTN_PRE,						 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT,						 m_btnNext);
	DDX_Control(pDX, IDC_STATIC_MES_LOG2,				 m_grpMesLogMessage2);
	DDX_Control(pDX, IDC_EDIT_MES_LOG2,					 m_edtMESDetailLog2);
	DDX_Control(pDX, IDC_CBX_MES_LOG_JOB_KIND,			 m_cbxMesLogJobkind);
	DDX_Control(pDX, IDC_LBL_MES_LOG_JOB_KIND,			 m_lblMesLogJobkind);
	DDX_Control(pDX, IDC_LBL_MES_LOG_FROM_LOCATION,		 m_lblMesLogFrLoc);
	DDX_Control(pDX, IDC_LBL_MES_LOG_TO_LOCATION,		 m_lblMesLogToLoc);
	
		
	DDX_Control(pDX, IDC_LBL_MES_LOG_BARCODE,			 m_lblMesLogBarcode);
	DDX_Control(pDX, IDC_EDT_MES_LUGGNO,				 m_edtMESLuggno);
	DDX_Control(pDX, IDC_EDT_MES_BARCODE,				 m_edtMESBarcode);
	DDX_Control(pDX, IDC_EDT_MES_FROM_LOCATION,			 m_edtMESFromloc);
	DDX_Control(pDX, IDC_EDT_MES_TO_LOCATION,			 m_edtMESToloc);
}


BEGIN_MESSAGE_MAP(CLogMesSkinDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnBnClickedSpread)
	ON_BN_CLICKED(IDC_BTN_LOG_IO_SEARCH, &CLogMesSkinDlg::OnBnClickedBtnLogMesSearch)
	ON_BN_CLICKED(IDC_BTN_PRE, &CLogMesSkinDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CLogMesSkinDlg::OnBnClickedNextRow)
	ON_COMMAND(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnSpdLogIo)
	ON_UPDATE_COMMAND_UI(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnUpdateSpdLogIo)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_CBX_ROW_CNT, &CLogMesSkinDlg::OnSetfocusCmbRowCnt)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CLogMesSkinDlg, CSkinDialog)
	ON_EVENT(CLogMesSkinDlg, IDC_SPD_LOG_MES, 5, CLogMesSkinDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
// CLogMesSkinDlg 메시지 처리기입니다.

void CLogMesSkinDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}

void CLogMesSkinDlg::OnClose()
{
	m_pDoc->m_pLogMesSkinDlg = NULL;
	delete this;
}

HCURSOR CLogMesSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogMesSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogMesSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogMesSkinDlg::RelocationControls()
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

	m_btnSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CLogMesSkinDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxMESLogWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxMesLogJobkind, _T("JOB_KIND"),m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);
	
	InitializeResource(pEn);
	m_pPreRowCnt = 0;

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

	m_dtpFrom.SetTime(tStartTime);
	m_dtpFromTime.SetTime(tStartTime);

	m_dtpTo.SetTime(tstrToTime);
	m_dtpToTime.SetTime(tstrToTime);

	m_dtpFromTime.SetFormat(_T("HH:mm:ss"));
	m_dtpToTime.SetFormat(_T("HH:mm:ss"));

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
	m_SpreadSheet.AddColHead(_T("추가시간"), 21);
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

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	return TRUE;
}

void CLogMesSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogMesSkinDlg::RenameResource(EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (strValue.IsEmpty())
		strValue = _T("MES 로그");	// 리소스 ini 부재 시 기본 제목
	SetWindowText(strValue + _T(" [IF_LUGG_STA_HIS]"));

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_BTN_LOG_IO_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	SetDlgItemText(IDOK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	SetDlgItemText(IDCANCEL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LOG_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobkind"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LOG_JOB_KIND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("barcode"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LOG_BARCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LUGGNO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_TO_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_FROM_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	SetDlgItemText(IDC_GRP_MES_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	SetDlgItemText(IDC_GRP_MES_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("detail"), (int)enLang);
	SetDlgItemText(IDC_STATIC_MES_LOG, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("mrcdmsg"), (int)enLang);
	SetDlgItemText(IDC_STATIC_MES_LOG2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromloc"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LOG_FROM_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("toloc"), (int)enLang);
	SetDlgItemText(IDC_LBL_MES_LOG_TO_LOCATION, strValue);
}

void CLogMesSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}

void CLogMesSkinDlg::OnBnClickedBtnLogMesSearch()
{
	UpdateData(TRUE);

	if(m_pDoc == NULL) { return; };
	InitializeSpread(0, TRUE);
}
void CLogMesSkinDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
}
void CLogMesSkinDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
}
void CLogMesSkinDlg::OnSpdLogIo()
{
}
void CLogMesSkinDlg::OnUpdateSpdLogIo(CCmdUI *pCmdUI)
{
}
void CLogMesSkinDlg::OnBnClickedSpread()
{
}
void CLogMesSkinDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
}




////////SPREAD 값채우기
void CLogMesSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CLogMesSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogMesSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogMesSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CLogMesSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");
	strColValue = _T(" ") + strColValue;

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogMesSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

void CLogMesSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CLogMesSkinDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CTime tToDate, tToTime, tFromDate, tFromTime;
	CString strdtFrom, strdtTo, strtimeFrom, strtimeTo;
	CString strWH_TYP = m_cbxMESLogWhTyp.GetItemKey(m_cbxMESLogWhTyp.GetCurSel());
	CString strJobKind = m_cbxMesLogJobkind.GetItemKey(m_cbxMesLogJobkind.GetCurSel());
	CString strLuggno, strBarcode, strFromloc, strToloc;
	m_edtMESLuggno.GetWindowText(strLuggno);
	m_edtMESBarcode.GetWindowText(strBarcode);
	m_edtMESFromloc.GetWindowText(strFromloc);
	m_edtMESToloc.GetWindowText(strToloc);

	m_dtpTo.GetTime(tToDate);
	m_dtpToTime.GetTime(tToTime);
	m_dtpFrom.GetTime(tFromDate);
	m_dtpFromTime.GetTime(tFromTime);

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

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tFromDate, tFromTime);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tToDate, tToTime);

	strtimeFrom = strdtFrom.Mid(8, 6);
	strdtFrom = strdtFrom.Mid(0, 8);
	strtimeTo = strdtTo.Mid(8, 6);
	strdtTo = strdtTo.Mid(0, 8);


	strSql += CRLF + _T("SELECT ILSH.UPD_DT AS UPD_DT															");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, ILSH.WH_TYP) AS WH_TYP			");
	strSql += CRLF + _T("      ,ILSH.LUGGNO AS LUGGNO															");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_JOB_KIND.CCD_NM_KOR, ILSH.JOB_KIND) AS JOB_KIND		");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_WORK_STA.CCD_NM_KOR, ILSH.WORK_STA) AS WORK_STA		");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_IF_STATUS.CCD_NM_KOR, ILSH.IF_STATUS) AS IF_STATUS	");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_ERRCODE.CCD_NM_KOR, ILSH.ERRCODE) AS ERRCODE		");
	strSql += CRLF + _T("      ,ILSH.FROM_AREA AS FROM_AREA														");
	strSql += CRLF + _T("      ,ILSH.TO_AREA AS TO_AREA															");
	strSql += CRLF + _T("      ,ILSH.FROM_CV_NO AS FROM_CV_NO													");
	strSql += CRLF + _T("      ,ILSH.TO_CV_NO AS TO_CV_NO														");
	strSql += CRLF + _T("      ,ILSH.FROM_SC_NO AS FROM_SC_NO													");
	strSql += CRLF + _T("      ,ILSH.TO_SC_NO AS TO_SC_NO														");
	strSql += CRLF + _T("      ,ILSH.PRIORITY AS PRIORITY														");
	strSql += CRLF + _T("      ,ILSH.PRDCT_NM AS PRDCT_NM														");
	strSql += CRLF + _T("      ,ILSH.LD_CTN_NO AS LD_CTN_NO														");
	strSql += CRLF + _T("  FROM IF_LUGG_STA_HIS ILSH															");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP											");
	strSql += CRLF + _T("                    ON ILSH.WH_TYP = CCD_WH_TYP.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_WH_TYP.CDX_CD = 'WH_TYP'									");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_JOB_KIND										");
	strSql += CRLF + _T("                    ON ILSH.JOB_KIND = CCD_JOB_KIND.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_JOB_KIND.CDX_CD = 'JOB_KIND'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WORK_STA										");
	strSql += CRLF + _T("                    ON ILSH.WORK_STA = CCD_WORK_STA.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_WORK_STA.CDX_CD = 'WORK_STA'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_IF_STATUS										");
	strSql += CRLF + _T("                    ON ILSH.IF_STATUS = CCD_IF_STATUS.CCD_CD							");
	strSql += CRLF + _T("                   AND CCD_IF_STATUS.CDX_CD = 'IF_STATUS'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_ERRCODE											");
	strSql += CRLF + _T("                    ON ILSH.ERRCODE = CCD_ERRCODE.CCD_CD							");
	strSql += CRLF + _T("                   AND CCD_ERRCODE.CDX_CD = 'ERRCODE'								");
	strSql += CRLF + _T("				  WHERE ILSH.CRT_DATE BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");
	strSql += CRLF + _T("				    AND ILSH.CRT_TIME BETWEEN '") + strtimeFrom + _T("' AND '") + strtimeTo + _T("'");
	if(strWH_TYP != _T("") && strWH_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND ILSH.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	}
	if (strJobKind != _T("") && strJobKind != _T("ALL"))
	{
		strSql += CRLF + _T("    AND ILSH.JOB_KIND =  ") + CLib::Quot(strJobKind);
	}
	if (strLuggno != _T(""))
	{
		strSql += CRLF + _T("    AND ILSH.LUGGNO LIKE '%") + strLuggno + _T("%'");
	}
	if (strBarcode != _T(""))
	{
		strSql += CRLF + _T("    AND ILSH.LD_CTN_NO LIKE '%") + strBarcode + _T("%'");
	}
	if (strFromloc != _T(""))
	{
		strSql += CRLF + _T("    AND ILSH.FROM_AREA LIKE '%") + strFromloc + _T("%'");
	}
	if (strToloc != _T(""))
	{
		strSql += CRLF + _T("    AND ILSH.TO_AREA LIKE '%") + strToloc + _T("%'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY ILSH.UPD_DT DESC");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#endif
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}


void CLogMesSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CLogMesSkinDlg::OnBnClickedNextRow() 
{
	InitializeSpread(1, TRUE);
}

void CLogMesSkinDlg::ClickSpread(long Col, long Row) //셀클릭
{
}
