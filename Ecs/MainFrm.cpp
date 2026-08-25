// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Ecs.h"
#include "MainFrm.h"
#include "Splash.h"
#include "MFCRibbonPanel_Wrap.h"
#include "MinButton.h"
#include "RecordSetWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrame
//
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

const int iCategoryIndex_MAIN = 0;
const int iCategoryIndex_SETTING = 1;
const int iCategoryIndex_AA = 2;
const int iCategoryIndex_BB = 3;
const int iCategoryIndex_CC = 4;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()

	ON_MESSAGE(WM_USER_LANG_NOTIFY, &CMainFrame::OnLangUpdate)

	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_1, &CMainFrame::OnUpdateStatusCv1)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_2, &CMainFrame::OnUpdateStatusCv2)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_3, &CMainFrame::OnUpdateStatusCv3)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_4, &CMainFrame::OnUpdateStatusCv4)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_5, &CMainFrame::OnUpdateStatusCv5)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_6, &CMainFrame::OnUpdateStatusCv6)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_7, &CMainFrame::OnUpdateStatusCv7)
	ON_UPDATE_COMMAND_UI(ID_STATUS_CV_8, &CMainFrame::OnUpdateStatusCv8)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_1, &CMainFrame::OnUpdateStatusSc1)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_2, &CMainFrame::OnUpdateStatusSc2)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_3, &CMainFrame::OnUpdateStatusSc3)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_4, &CMainFrame::OnUpdateStatusSc4)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_5, &CMainFrame::OnUpdateStatusSc5)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_6, &CMainFrame::OnUpdateStatusSc6)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_7, &CMainFrame::OnUpdateStatusSc7)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_8, &CMainFrame::OnUpdateStatusSc8)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_9, &CMainFrame::OnUpdateStatusSc9)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_10, &CMainFrame::OnUpdateStatusSc10)
	ON_UPDATE_COMMAND_UI(ID_STATUS_SC_11, &CMainFrame::OnUpdateStatusSc11)
	//ON_UPDATE_COMMAND_UI(ID_STATUS_RTV_1, &CMainFrame::OnUpdateStatusRtv1)
	//ON_UPDATE_COMMAND_UI(ID_STATUS_BCR_1, &CMainFrame::OnUpdateStatusBcr1)
	//ON_UPDATE_COMMAND_UI(ID_STATUS_BCR_2, &CMainFrame::OnUpdateStatusBcr2)

	ON_CONTROL_RANGE(BN_CLICKED, ID_STATUS_CV_1, ID_STATUS_SC_11, &CMainFrame::OnButtonComm)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_bNotDockingJob = false;
	m_bNotDockingInfo = false;
	m_bShowStatusBar = false;
	m_bToolNMenuBar = false;
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), 0);

}
CMainFrame::~CMainFrame()
{
}
#define LIGHT_SKY	RGB(0x99, 0xFF, 0xFF)
#define NAVY		RGB(0x00, 0x00, 0x80)
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	DockPane(&m_wndToolBar);
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1){	return -1;	}
	ExcuteTheme();
	if (!m_wndRibbonBar.Create(this)){		return -1;		}

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
 	
	CRect rect;
	//this->GetClientRect(&rect);
	GetDesktopWindow()->GetWindowRect(&rect);
	int nWindowSizeX = rect.Width() - 4;
	int nWindowSizeY = rect.Height() - 80;

	m_wndStatusBar.MoveWindow(0, nWindowSizeY, nWindowSizeX, 40);

	InitializeRibbonMenu();
	AddStatusBarPane();
	CSplashWnd::ShowSplashScreen(this);

	m_hIcon = LoadIcon(::AfxGetInstanceHandle(), _T("WCS.exe"));
	SetIcon(Global.m_hIcon[IDX_ICON_MAX-1][2], TRUE);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.cx = 1024;
	cs.cy = 768;

	return TRUE;
}

// CMainFrame 진단
//
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
//
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::MakeSureFileExists(CString strFile)
{
	if ( strFile.IsEmpty() )		return FALSE;

	if ( strFile.Right(1) == _T("\\") )
		return FALSE;

	if ( GetFileAttributes(strFile) == 0xFFFFFFFF )
		return FALSE;

	return TRUE;
}
void CMainFrame::DisplayMessage(CString strMsg)
{
}
void CMainFrame::UpdateCommStatus(int nIndex, UINT unID, char Status)
{
}
int CMainFrame::GetIndex(char Status) 
{
	switch (Status)
	{
	case COMM_STA_SEND:			return 13;
	case COMM_STA_RECEIVE:		return 14;
	case COMM_STA_STOP:			return 15;
	default:					return 15;
	}
}

void CMainFrame :: OnStyleRibboninterface ()
{
	if ( nullptr == m_wndRibbonBar.m_hWnd)
	{
		m_wndRibbonBar.Create (this);
		m_wndRibbonBar.LoadFromResource (IDR_MAINFRAME_RIBBON);
	}

	// 리본을 표시하고 메뉴 / 툴바를 숨 깁니다.
	m_wndRibbonBar.ShowPane ( TRUE , TRUE , TRUE );

	// 크기와 위치를 변경하지 않고 프레임 윈도우를 강제로 다시 그립니다.
	CRect rectThis;
	GetWindowRect (& rectThis);
	SetWindowPos (
		& wndTop, 
		rectThis.left, 
		rectThis.top, 
		rectThis.Width (), 
		rectThis.Height (), 
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
}

void CMainFrame :: OnRbnBtnToolbarInterface ()
{
	// 메뉴 / 도구 모음을 표시하고 리본을 숨깁니다.
	m_wndRibbonBar.ShowPane ( FALSE , FALSE , FALSE );

	// 크기와 위치를 변경하지 않고 프레임 윈도우를 강제로 다시 그립니다.
	CRect rectThis;
	GetWindowRect (& rectThis);
	SetWindowPos (
		& wndTop, 
		rectThis.left, 
		rectThis.top, 
		rectThis.Width (), 
		rectThis.Height (), 
		SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOZORDER
		);
}

HICON CMainFrame::HICONFromPATH(CString pstrPath)
{
	CImage image;

	/*
	 * 그림 파일이 없으면 CImage::Load 가 실패하고 m_hBitmap 이 0 으로 남는다.
	 * 그 상태로 Detach() 를 부르면 atlimage.h 에서 어서션이 난다.
	 * 그림 하나 없다고 창이 죽을 이유는 없다. 없으면 아이콘 없이 연다.
	 */
	if (FAILED(image.Load(pstrPath)) || image.IsNull())
		return NULL;
	if(image == NULL)
	{
		return Global.GetIcon(Global.ICO_CV_ON);
	}
	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), 
		bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = {0};
	ii.fIcon    = TRUE;
	ii.hbmColor = bitmap;
	ii.hbmMask  = hbmMask;

	HICON hIcon = ::CreateIconIndirect(&ii);
	::DeleteObject(hbmMask);

	return hIcon;
}
CString CMainFrame::GetConcatPath(CString pstrAppPath, CString pstrFileName, CString pstrExtension)
{
	CString strReturn = _T("");
	strReturn.Format(_T("%s%s%s"), pstrAppPath, pstrFileName, pstrExtension);
	return strReturn;
}

CString CMainFrame::GetConcatPath_LANG(CString pstrAppPath, CString pstrFileName, CString pstrExtension, EN_LANG penLang)
{
	CString strReturn = _T("");
	if(penLang == EN_LANG::EN_KOR)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_K"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_ENG)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_E"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_CHIN)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_C"), pstrExtension);
		return strReturn;
	}
	if(penLang == EN_LANG::EN_HUN)
	{
		strReturn.Format(_T("%s%s%s%s"), pstrAppPath, pstrFileName, _T("_H"), pstrExtension);
		return strReturn;
	}
	strReturn.Format(_T("%s%s%s%s"), pstrAppPath, _T("BLIND"), _T("_E"), _T(".png"));
	return _T("");
}

void CMainFrame::InitializeRibbonMenu(EN_LANG penLang)
{
	m_MainButton.SetImage(IDB_LOGO_SKI);
	m_MainButton.SetToolTipText(_T("File"));
	m_MainButton.SetText(_T("\nf"));

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\");

	CString strCMainframeConfigFileName[4];
	CString strExtension = _T(".png");
	strCMainframeConfigFileName[0] = _T("CMAINFRAME_CONFIG");
	strCMainframeConfigFileName[1] = _T("");
	strCMainframeConfigFileName[2] = _T("");
	strCMainframeConfigFileName[3] = _T("");

	m_MainButton.SetImage(IDB_LOGO_SKI);
	m_MainButton.SetToolTipText(_T("File"));
	m_MainButton.SetText(_T("\nf"));
	AddCategoryWCS();
	AddCategoryMANUAL();
	AddCategoryLOG();
}

LRESULT CMainFrame::OnLangUpdate(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = (CEcsDoc*)lParam;
	if(pDoc == NULL)
		return 0;

	RenameRibbonText(pDoc->m_enLang);			// TEST

	m_wndRibbonBar.ForceRecalcLayout();			// TEST

	return 0;
}

void CMainFrame::AddCategoryWCS()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	CString strExtension = _T(".png");

	CString TEST_WH_TYP = strAppPath.Left(strAppPath.ReverseFind('\\'));
	TEST_WH_TYP = TEST_WH_TYP.Left(TEST_WH_TYP.ReverseFind('\\'));
	CString strWH_TYP = _T("");
	
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("WH"), _T("WH_TYP"), _T("10"), szTemp, _MAX_PATH, ECS_INI_FILE);
	strWH_TYP.Format(_T("%s"), szTemp); 

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("WCS"), IDB_LOGO_ECS, IDB_LOGO_ECS);

	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelConfig = pCategory->AddPanel(_T("CONFIG"));

	CMFCRibbonButton* pBtnEqpSuspend = new CMFCRibbonButton(ID_SYSTEM_EQP_SUSPEND, _T("EQP"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqpsuspend"), strExtension)), TRUE);
	pBtnEqpSuspend->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnEqpSuspend);

	CMFCRibbonButton* pBtnLogDelete = new CMFCRibbonButton(IDD_CONFIG_LOG_DELETE, _T("LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("configlogdelete"), strExtension)), TRUE);
	pBtnLogDelete->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnLogDelete);

	CMFCRibbonButton* pBtnConfigStatus = new CMFCRibbonButton(IDD_CONFIG_STATUS, _T("CONF"), HICONFromPATH(GetConcatPath(strAppPath, _T("configstatus"), strExtension)), TRUE);
	pBtnConfigStatus->SetAlwaysLargeImage();
	pPanelConfig->Add(pBtnConfigStatus);

	//
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_view\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelView = pCategory->AddPanel(_T("VIEW"));

	CMFCRibbonButton* pBtnJob = new CMFCRibbonButton(ID_VIEW_JOBLIST, _T("JOB-INFO"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
	pBtnJob->SetAlwaysLargeImage();
	pPanelView->Add(pBtnJob);

	CMFCRibbonButton* pBtnIfstatus = new CMFCRibbonButton(ID_VIEW_IFLIST, _T("IFSTATUS-INFO"), HICONFromPATH(GetConcatPath(strAppPath, _T("Ifstatus"), strExtension)), TRUE);
	pBtnIfstatus->SetAlwaysLargeImage();
	pPanelView->Add(pBtnIfstatus);

	CMFCRibbonButton* pBtnSearch = new CMFCRibbonButton(ID_VIEW_SEARCH, _T("SEARCH"), HICONFromPATH(GetConcatPath(strAppPath, _T("search"), strExtension)), TRUE);
	pBtnSearch->SetAlwaysLargeImage();
	pPanelView->Add(pBtnSearch);
	//

	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_monitor\\");
	strExtension = _T(".png");

	CMFCRibbonPanel* pPanelMonitor = pCategory->AddPanel(_T("WAREHOUSE MONITOR"));


	if(strWH_TYP == _T("10"))
	{
		CMFCRibbonButton* pBtnCv = new CMFCRibbonButton(ID_MONITORING_CV, _T("트랙번호 표시"), HICONFromPATH(GetConcatPath(strAppPath, _T("Cv"), strExtension)), TRUE);
		pBtnCv->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtnCv);

		CMFCRibbonButton* pBtnLg = new CMFCRibbonButton(ID_MONITORING_LG, _T("작업번호 표시"), HICONFromPATH(GetConcatPath(strAppPath, _T("Lg"), strExtension)), TRUE);
		pBtnLg->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtnLg);

		CMFCRibbonButton* pBtnPl = new CMFCRibbonButton(ID_MONITORING_PL, _T("팔렛트번호 표시"), HICONFromPATH(GetConcatPath(strAppPath, _T("Pl"), strExtension)), TRUE);
		pBtnPl->SetAlwaysLargeImage();
		pPanelMonitor->Add(pBtnPl);
	}

	AddPanelLAYOUT(pCategory);
}

//	층을 오가는 버튼.
//	EcsLayout1.xml = 1F, EcsLayout2.xml = 2F, EcsLayout3.xml = 3F 이고
//	Ecs.ini 의 [COMMON] TabCount 만큼 읽어 들인다.
//	누르면 CEcsDoc::OnCommandRangeMainFrameLAYOUT 이 받는다.
//	읽지 못한 층의 버튼은 OnUpdateMainFrameLAYOUT 이 회색으로 만든다.
void CMainFrame::AddPanelLAYOUT(CMFCRibbonCategory* pCategory)
{
	if (pCategory == NULL)
		return;

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);

	CString strAppPath;
	strAppPath.Format(_T("%s"), chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_layout\\");
	CString strExtension = _T(".png");

	CMFCRibbonPanel* pPanelLayout = pCategory->AddPanel(_T("레이아웃"));

	const UINT nIDs[3]      = { ID_LAYOUT_1F, ID_LAYOUT_2F, ID_LAYOUT_3F };
	LPCTSTR     szTexts[3]  = { _T("1F"), _T("2F"), _T("3F") };
	LPCTSTR     szImages[3] = { _T("1f"), _T("2f"), _T("3f") };

	for (int i = 0; i < 3; ++i)
	{
		CMFCRibbonButton* pBtn = new CMFCRibbonButton(
			nIDs[i], szTexts[i],
			HICONFromPATH(GetConcatPath(strAppPath, szImages[i], strExtension)), TRUE);
		pBtn->SetAlwaysLargeImage();
		pPanelLayout->Add(pBtn);
	}
}
	
void CMainFrame::AddCategoryMANUAL()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_manual\\");
	CString strExtension = _T(".png");

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("MANUAL"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	CMFCRibbonPanel* pPanelManual = pCategory->AddPanel(_T("MANUAL"));

	CMFCRibbonButton* pBtnManualJob = new CMFCRibbonButton(ID_MANUAL_JOB, _T("JOB"), HICONFromPATH(GetConcatPath(strAppPath, _T("job"), strExtension)), TRUE);
	pBtnManualJob->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualJob);

	CMFCRibbonButton* pBtnManualSc = new CMFCRibbonButton(ID_MANUAL_SC, _T("SC"), HICONFromPATH(GetConcatPath(strAppPath, _T("sc"), strExtension)), TRUE);
	pBtnManualSc->SetAlwaysLargeImage();
	pPanelManual->Add(pBtnManualSc);

	//CMFCRibbonButton* pBtnManualRtv = new CMFCRibbonButton(ID_MANUAL_RTV, _T("RTV"), HICONFromPATH(GetConcatPath(strAppPath, _T("rtv"), strExtension)), TRUE);
	//pBtnManualRtv->SetAlwaysLargeImage();
	//pPanelManual->Add(pBtnManualRtv);
}

void CMainFrame::AddCategoryLOG()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_log\\");
	CString strExtension = _T(".png");

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("LOG"), IDB_LOGO_ECS, IDB_LOGO_ECS);
	CMFCRibbonPanel* pPanelLog = pCategory->AddPanel(_T("LOG"));

	CMFCRibbonButton* pBtnIO_LOG = new CMFCRibbonButton(ID_LOG_IO, _T("IO_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("iolog"), strExtension)), TRUE);
	pBtnIO_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnIO_LOG);

	CMFCRibbonButton* pBtnMES_LOG = new CMFCRibbonButton(ID_LOG_MES, _T("MES_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("meslog"), strExtension)), TRUE);
	pBtnMES_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnMES_LOG);

	CMFCRibbonButton* pBtnEQP_HIS_LOG = new CMFCRibbonButton(ID_LOG_EQP_HIS, _T("EQP_HIS_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("eqphislog"), strExtension)), TRUE);
	pBtnEQP_HIS_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnEQP_HIS_LOG);

	TCHAR szTempBcrView[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("LOG"), _T("BCR_LOG_VIEW"), _T("1"), szTempBcrView, _MAX_PATH, ECS_INI_FILE);
	if (CString(szTempBcrView) != _T("0"))	// Ecs.ini [LOG] BCR_LOG_VIEW=0 �̸� BCR(WC)�α� ��ư ����
	{
		CMFCRibbonButton* pBtnBCR_LOG = new CMFCRibbonButton(ID_LOG_BCR, _T("BCR_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("bcrlog"), strExtension)), TRUE);
		pBtnBCR_LOG->SetAlwaysLargeImage();
		pPanelLog->Add(pBtnBCR_LOG);
	}

	CMFCRibbonButton* pBtnCLIENT_LOG = new CMFCRibbonButton(ID_LOG_CLIENT, _T("CLIENT_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("clientlog"), strExtension)), TRUE);
	pBtnCLIENT_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnCLIENT_LOG);

	CMFCRibbonButton* pBtnWCS_LOG = new CMFCRibbonButton(IDD_LOG_WCS_LOG_PGR, _T("WCS_LOG"), HICONFromPATH(GetConcatPath(strAppPath, _T("wcslog"), strExtension)), TRUE);
	pBtnWCS_LOG->SetAlwaysLargeImage();
	pPanelLog->Add(pBtnWCS_LOG);
}

void CMainFrame::RenameRibbonText(EN_LANG penLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");
	
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetCategory(1);
	BOOL bTemp1 = FALSE;
	BOOL bTemp2 = FALSE;
	CString strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_config\\"), _T("config"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Config = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(0);

	CMFCRibbonButton* pButton_sign = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(0);
	pButton_sign->SetText(CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)penLang));

	CMFCRibbonButton* pButton_log_delete = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(1);
	pButton_log_delete->SetText(CLib::GetIniStringFromPath(strFullPath, _T("configlogdelete"), (int)penLang));

	CMFCRibbonButton* pButton_config_status = (CMFCRibbonButton*)pPanel_Wrap_Config->GetElement(2);
	pButton_config_status->SetText(CLib::GetIniStringFromPath(strFullPath, _T("configstatus"), (int)penLang));

	pPanel_Wrap_Config->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));

	pPanel_Wrap_Config->SetCenterColumnVert();
	pPanel_Wrap_Config->SetJustifyColumns();
	
	 strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_view\\"), _T("view"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_View = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(1);
	pPanel_Wrap_View->SetCenterColumnVert();
	pPanel_Wrap_View->SetJustifyColumns();
	pPanel_Wrap_View->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pButton_Job = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(0);
	pButton_Job->SetText(CLib::GetIniStringFromPath(strFullPath, _T("job-info"), (int)penLang));
	//CMFCRibbonButton* pButton_emptyplt = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(1);
	//pButton_emptyplt->SetText(_T("공PLT작업"));
	CMFCRibbonButton* pButton_Ifstatus = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(1);
	pButton_Ifstatus->SetText(CLib::GetIniStringFromPath(strFullPath, _T("ifstatus-info"), (int)penLang));
	CMFCRibbonButton* pButton_Search = (CMFCRibbonButton*)pPanel_Wrap_View->GetElement(2);
	pButton_Search->SetText(CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)penLang));

	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_monitor\\"), _T("monitor"), strExtension);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Monitor = (CMFCRibbonPanel_Wrap*)pCategory->GetPanel(2);
	pPanel_Wrap_Monitor->SetCenterColumnVert();
	pPanel_Wrap_Monitor->SetJustifyColumns();
	pPanel_Wrap_Monitor->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));

	//MANUAL
	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_manual\\"), _T("manual"), strExtension);
	CMFCRibbonCategory* pCategory2 = m_wndRibbonBar.GetCategory(2);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Manual = (CMFCRibbonPanel_Wrap*)pCategory2->GetPanel(0);
	pPanel_Wrap_Manual->SetCenterColumnVert();
	pPanel_Wrap_Manual->SetJustifyColumns();
	pPanel_Wrap_Manual->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pBtnManualJob = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(0);
	pBtnManualJob->SetText(CLib::GetIniStringFromPath(strFullPath, _T("job"), (int)penLang));
	CMFCRibbonButton* pBtnManualSc = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(1);
	pBtnManualSc->SetText(CLib::GetIniStringFromPath(strFullPath, _T("sc"), (int)penLang));
	CMFCRibbonButton* pBtnManualRtv = (CMFCRibbonButton*)pPanel_Wrap_Manual->GetElement(2);
	//pBtnManualRtv->SetText(CLib::GetIniStringFromPath(strFullPath, _T("rtv"), (int)penLang));

	strFullPath = GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\mainframe_log\\"), _T("log"), strExtension);
	CMFCRibbonCategory* pCategory3 = m_wndRibbonBar.GetCategory(3);
	CMFCRibbonPanel_Wrap* pPanel_Wrap_Log = (CMFCRibbonPanel_Wrap*)pCategory3->GetPanel(0);
	pPanel_Wrap_Log->SetCenterColumnVert();
	pPanel_Wrap_Log->SetJustifyColumns();
	pPanel_Wrap_Log->SetName(CLib::GetIniStringFromPath(strFullPath, _T("categoryname"), (int)penLang));
	CMFCRibbonButton* pBtnLogIoLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(0);
	pBtnLogIoLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("io_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogMesLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(1);
	pBtnLogMesLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("mes_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogEqpHisLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(2);
	pBtnLogEqpHisLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("eqphislog"), (int)penLang));
	TCHAR szTempBcrView2[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("LOG"), _T("BCR_LOG_VIEW"), _T("1"), szTempBcrView2, _MAX_PATH, ECS_INI_FILE);
	int nLogBtnIdx = 3;
	if (CString(szTempBcrView2) != _T("0"))	// BCR(WC)�α� ��ư�� ���� ���� (���� �� �ε��� �и� ����)
	{
		CMFCRibbonButton* pBtnLogBcrLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(nLogBtnIdx++);
		if (pBtnLogBcrLog) pBtnLogBcrLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("bcrlog"), (int)penLang));
	}
	CMFCRibbonButton* pBtnLogClientLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(nLogBtnIdx++);
	if (pBtnLogClientLog) pBtnLogClientLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("client_log"), (int)penLang));
	CMFCRibbonButton* pBtnLogWcsLog = (CMFCRibbonButton*)pPanel_Wrap_Log->GetElement(nLogBtnIdx++);
	if (pBtnLogWcsLog) pBtnLogWcsLog->SetText(CLib::GetIniStringFromPath(strFullPath, _T("wcs_log"), (int)penLang));

	m_wndRibbonBar.ForceRecalcLayout();	

	bTemp1 = pPanel_Wrap_Config->IsJustifyColumns();
	bTemp2 = pPanel_Wrap_Config->IsCenterColumnVert();
}

void CMainFrame::ExcuteTheme()
{	
	CDockingManager::SetDockingMode (DT_SMART);
	CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D;
	CMFCVisualManagerOffice2007::SetStyle (CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));	
	CDockingManager::SetDockingMode (DT_SMART);
	CDockingManager* pDockManager = GetDockingManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustPaneFrames ();
	}
	CTabbedPane::ResetTabs ();
	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

void CMainFrame::AddStatusBarPane()
{
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(10);
	m_wndStatusBar.SetPaneWidth(0, 20);

	int iwitdh = 80;
	int i=0;
	++i;

	InsertButtonPainToStatusBar(_T("CV_1그룹"), ID_STATUS_CV_1, i + 1, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_2그룹"), ID_STATUS_CV_2, i + 2, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_3그룹"), ID_STATUS_CV_3, i + 3, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_4그룹"), ID_STATUS_CV_4, i + 4, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_5그룹"), ID_STATUS_CV_5, i + 5, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_6그룹"), ID_STATUS_CV_6, i + 6, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_7그룹"), ID_STATUS_CV_7, i + 7, iwitdh);
	InsertButtonPainToStatusBar(_T("CV_8그룹"), ID_STATUS_CV_8, i + 8, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_1호기"), ID_STATUS_SC_1, i + 9, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_2호기"), ID_STATUS_SC_2, i + 10, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_3호기"), ID_STATUS_SC_3, i + 11, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_4호기"), ID_STATUS_SC_4, i + 12, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_5호기"), ID_STATUS_SC_5, i + 13, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_6호기"), ID_STATUS_SC_6, i + 14, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_7호기"), ID_STATUS_SC_7, i + 15, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_8호기"), ID_STATUS_SC_8, i + 16, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_9호기"), ID_STATUS_SC_9, i + 17, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_10호기"), ID_STATUS_SC_10, i + 18, iwitdh);
	InsertButtonPainToStatusBar(_T("SC_11호기"), ID_STATUS_SC_11, i + 19, iwitdh);
	//InsertButtonPainToStatusBar(_T("RTV_1호기"), ID_STATUS_RTV_1, i + 4, 80);
	//InsertButtonPainToStatusBar(_T("BCR_1호기"), ID_STATUS_BCR_1, i + 5, 80);
	//InsertButtonPainToStatusBar(_T("BCR_2호기"), ID_STATUS_BCR_2, i + 6, 80);
		
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_2, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_3, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_4, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_5, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_6, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_7, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_8, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_5, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_6, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_7, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_8, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_9, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_10, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_11, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_RTV_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_1, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
	//m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_2, 12, _T("Arial Black"), BLACK, DARK_GRAY, WHITE, CMinButton::Gradient, 100, CMinButton::UPTODOWN);
}

void CMainFrame::InsertButtonPainToStatusBar(CString strCaption, int nID, int nPos, int nWidth)
{
	CExtStatusBarButton* pButton = new CExtStatusBarButton;
	if (!pButton->Create(strCaption, WS_CHILD|WS_VISIBLE|WS_TABSTOP|WM_DRAWITEM, CRect(0,0,0,0), &m_wndStatusBar, nID))
		return;

	pButton->SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT))	);
	if ( !m_wndStatusBar.AddPane(nID, nPos) )
		return;
	int nIndex = m_wndStatusBar.CommandToIndex(nID);
	if (nIndex == -1)
		return;
	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.AddPaneControl(pButton, nID, true);
}

void CMainFrame::OnButtonComm(UINT nID)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )	
		return;

	CString strEQP_TIME = _T("");
	CString strEQP_CONNECTED_YN = _T("");
	CString strTYPE = _T("");
	int nEQP_TIME = 0;
	CString strTemp1, strTemp2;
	int nEQP_NUM = 0;
	int nCheckTime = 0;				// Check하는 시간이 다름!

	CTrackInfo* pTrackInfo = NULL;
	CSC_DATA* pSC_DATA = NULL;
	CRTV_DATA* pRTV_DATA = NULL;
	CBCR_MST* pBCR_DATA = NULL;

    #pragma region Control 번호를 통해서 필요한 정보들을 설정하는 부분

	switch(nID)
	{
	case ID_STATUS_CV_1:
		#pragma region ID_STATUS_CV_1
		pTrackInfo = pDoc->GetTrackInfoNew(_T("101"));
	
		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

		#pragma endregion
	case ID_STATUS_CV_2:
#pragma region ID_STATUS_CV_2
		pTrackInfo = pDoc->GetTrackInfoNew(_T("102"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_3:
#pragma region ID_STATUS_CV_3
		pTrackInfo = pDoc->GetTrackInfoNew(_T("103"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_4:
#pragma region ID_STATUS_CV_4
		pTrackInfo = pDoc->GetTrackInfoNew(_T("104"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_5:
#pragma region ID_STATUS_CV_5
		pTrackInfo = pDoc->GetTrackInfoNew(_T("105"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_6:
#pragma region ID_STATUS_CV_6
		pTrackInfo = pDoc->GetTrackInfoNew(_T("106"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_7:
#pragma region ID_STATUS_CV_7
		pTrackInfo = pDoc->GetTrackInfoNew(_T("107"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
	case ID_STATUS_CV_8:
#pragma region ID_STATUS_CV_8
		pTrackInfo = pDoc->GetTrackInfoNew(_T("108"));

		if (pTrackInfo == NULL)
			return;

		//strTemp1.Format(_T("CV_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.236"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("CV");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_1:
		#pragma region ID_STATUS_SC_1
		pSC_DATA = pDoc->GetSC_DATA(_T("901"));
	
		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

		#pragma endregion
		break;
	case ID_STATUS_SC_2:
#pragma region ID_STATUS_SC_2
		pSC_DATA = pDoc->GetSC_DATA(_T("902"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_3:
#pragma region ID_STATUS_SC_3
		pSC_DATA = pDoc->GetSC_DATA(_T("903"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_4:
#pragma region ID_STATUS_SC_4
		pSC_DATA = pDoc->GetSC_DATA(_T("904"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_5:
#pragma region ID_STATUS_SC_5
		pSC_DATA = pDoc->GetSC_DATA(_T("905"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_6:
#pragma region ID_STATUS_SC_6
		pSC_DATA = pDoc->GetSC_DATA(_T("906"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_7:
#pragma region ID_STATUS_SC_7
		pSC_DATA = pDoc->GetSC_DATA(_T("907"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_8:
#pragma region ID_STATUS_SC_8
		pSC_DATA = pDoc->GetSC_DATA(_T("908"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_9:
#pragma region ID_STATUS_SC_9
		pSC_DATA = pDoc->GetSC_DATA(_T("909"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.242"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 1;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_10:
#pragma region ID_STATUS_SC_10
		pSC_DATA = pDoc->GetSC_DATA(_T("910"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;
	case ID_STATUS_SC_11:
#pragma region ID_STATUS_SC_11
		pSC_DATA = pDoc->GetSC_DATA(_T("911"));

		if (pSC_DATA == NULL)
			return;

		//strTemp1.Format(_T("SC_01 (IP:%s) (PORT:%04d)"),_T("10.99.43.243"),8192);		// switch문 밑에서 만들기

		strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
		strEQP_TIME = pSC_DATA->V_EQP_TIME;
		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
		strTYPE = _T("SC");
		nEQP_NUM = 2;
		nCheckTime = 5;

#pragma endregion
		break;


//	case ID_STATUS_RTV_1:
//#pragma region ID_STATUS_RTV_1
//		pRTV_DATA = pDoc->GetRTV_DATA(_T("801"));
//
//		if (pRTV_DATA == NULL)
//			return;
//
//		strEQP_CONNECTED_YN = pRTV_DATA->V_EQP_CONNECTED_YN;
//		strEQP_TIME = pRTV_DATA->V_EQP_TIME;
//		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//		strTYPE = _T("RTV");
//		nEQP_NUM = 1;
//		nCheckTime = 5;
//
//#pragma endregion
//		break;
//	case ID_STATUS_BCR_1:
//#pragma region ID_STATUS_BCR_1
//		pBCR_DATA = pDoc->GetBCR_MST(_T("701"));
//
//		if (pBCR_DATA == NULL)
//			return;
//		strEQP_CONNECTED_YN = pBCR_DATA->V_EQP_CONNECTED_YN;
//		strEQP_TIME = pBCR_DATA->V_EQP_TIME;
//		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//		strTYPE = _T("BCR");
//		nEQP_NUM = 1;
//		nCheckTime = 5;
//
//#pragma endregion
//		break;
//	case ID_STATUS_BCR_2:
//#pragma region ID_STATUS_BCR_2
//		pBCR_DATA = pDoc->GetBCR_MST(_T("702"));
//
//		if (pBCR_DATA == NULL)
//			return;
//		strEQP_CONNECTED_YN = pBCR_DATA->V_EQP_CONNECTED_YN;
//		strEQP_TIME = pBCR_DATA->V_EQP_TIME;
//		nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//		strTYPE = _T("BCR");
//		nEQP_NUM = 2;
//		nCheckTime = 5;
//
//#pragma endregion
//		break;
	default:
		return;
	}
	#pragma endregion

	if (pDoc->m_blConnectStatus == TRUE)
	{
		//HOST_IF_LOG에 들어온 메시지가 30초 이상 경과 되었을 떄 에러로 표시
		if (nEQP_TIME > nCheckTime)
		{
			// 쿼리문을 날려라
			// 가져온값으로 확인하라.
			CString strSql, strTemp, strMessage;
			strSql.Format(_T("SELECT * FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = '%s' AND PLC_NO = '%02d'"), pDoc->m_WH_TYP, strTYPE, nEQP_NUM);

			int nRowCount = 0;//m_pDoc->GetSelectQryCnt_DLG(strSql);
			_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr(strSql, nRowCount, strMessage);

 			if (nRowCount < 1)
 			{
				strTemp.Format(_T("설비가 존재하지 않습니다. [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
 				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
 				AfxMessageBox(strTemp);									// 번역안할때
 				return;
 			}

 			if (nRowCount > 1)
 			{
				strTemp.Format(_T("설비가 한개가 아니라 여러개 존재합니다. - 검색 조건 확인요망 => [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
 				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
 				AfxMessageBox(strTemp);									// 번역안할때
				
 				return;
 			}

			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
			pRsw->MoveFirst(); 

			//CString strIP, strPORT, strBAT, strEQP_TYP;
	
			//for(int i = 1; i <= nRowCount; i++)
			//{
			CString	strREMARKS = pRsw->GetItem(_T("REMARKS"));		
			CString	strIP = pRsw->GetItem(_T("PLC_IP"));		
			CString	strPORT = pRsw->GetItem(_T("PLC_PORT"));		
			//CString	strBAT = pRsw->GetItem(_T("BATCH"));
			//	pRsw->MoveNext();
			//}

			delete pRsw;

			strTemp1.Format(_T("%s (IP:%s) (PORT:%s)"), strREMARKS, strIP, strPORT);		

			strTemp2.Format(_T("PingTest를 하겠습니까? [접속정보 -> %s]"), strTemp1);
			
			if (AfxMessageBox(strTemp2, MB_YESNO) != IDYES)
			{
				AfxMessageBox(_T("통신 연결 되지 않았습니다!"));
				return;
			}

			AfxMessageBox(_T("PingTest 하겠습니다.\n\nPing 비정상시 네트워크 담당자에게 확인바랍니다!\nPing 정상시 PORT가 정상적으로 OPEN 되어있는지 확인하세요"));
			//::ShellExecute(NULL, _T("open"), _T(".\\PING_WC01.BAT"), NULL, NULL, SW_SHOW);
			::ShellExecute(NULL, _T("open"), _T("cmd"), _T("/C ping " + strIP + " -t"), NULL, SW_SHOW);		// 정상
		}
		else
		{
			CString strSql, strTemp, strMessage;
			strSql.Format(_T("SELECT * FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = '%s' AND PLC_NO = '%02d'"), pDoc->m_WH_TYP, strTYPE, nEQP_NUM);

			int nRowCount = 0;//m_pDoc->GetSelectQryCnt_DLG(strSql);
			_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr(strSql, nRowCount, strMessage);

			if (nRowCount < 1)
			{
				strTemp.Format(_T("설비가 존재하지 않습니다. [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
				AfxMessageBox(strTemp);									// 번역안할때
				return;
			}

			if (nRowCount > 1)
			{
				strTemp.Format(_T("설비가 한개가 아니라 여러개 존재합니다. - 검색 조건 확인요망 => [설비종류 : %s][설비번호 : %02d]"), strTYPE, nEQP_NUM);
				//AfxMessageBox(m_pDoc->GetMsgLangDef(strTemp));		// 번역할때 
				AfxMessageBox(strTemp);									// 번역안할때

				return;
			}

			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

			pRsw->MoveFirst();

			CString	strIP = pRsw->GetItem(_T("PLC_IP"));
			CString	strPORT = pRsw->GetItem(_T("PLC_PORT"));
			strTemp1.Format(_T("%s %02d의 연결상태가 양호합니다. (IP:%s) (PORT:%s)"), strTYPE, nEQP_NUM, strIP, strPORT);
			AfxMessageBox(strTemp1);
		}
	}
}

void CMainFrame::OnUpdateStatusCv1(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("101"));
	
	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_1, _T("CV_1G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv2(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("201"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_2, _T("CV_2G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_2, _T("CV_2G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv3(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("301"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_3, _T("CV_3G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_3, _T("CV_3G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv4(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("401"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_4, _T("CV_4G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_4, _T("CV_4G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv5(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("501"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_5, _T("CV_5G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_5, _T("CV_5G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv6(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("601"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_6, _T("CV_6G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_6, _T("CV_6G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv7(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("701"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_7, _T("CV_7G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_7, _T("CV_7G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusCv8(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(_T("801"));

	if (pTrackInfo == NULL)
		return;


	CString strEQP_TIME = pTrackInfo->m_pCV_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pTrackInfo->m_pCV_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pTrackInfo->m_pCV_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_8, _T("CV_8G"), RED, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_CV_8, _T("CV_8G"), GREEN, BLACK);
			pTrackInfo->m_pCV_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}


void CMainFrame::OnUpdateStatusSc1(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("901"));
	
	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_1, _T("SC_1호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc2(CCmdUI *pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL )
		return;
	//m_pDoc = pDoc;
	
	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("902"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
	
	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if(strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if(strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_2, _T("SC_2호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc3(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("903"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_3, _T("SC_3호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc4(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("904"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_4, _T("SC_4호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc5(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("905"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_5, _T("SC_5호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_5, _T("SC_5호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc6(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("906"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_6, _T("SC_6호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_6, _T("SC_6호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc7(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("907"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_7, _T("SC_7호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_7, _T("SC_7호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc8(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("908"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_8, _T("SC_8호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_8, _T("SC_8호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc9(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("909"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_9, _T("SC_9호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_9, _T("SC_9호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc10(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("910"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_10, _T("SC_10호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_10, _T("SC_10호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}

void CMainFrame::OnUpdateStatusSc11(CCmdUI* pCmdUI)
{
	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
	if (pDoc == NULL)
		return;
	//m_pDoc = pDoc;

	CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(_T("911"));

	if (pSC_DATA == NULL)
		return;

	CString strEQP_TIME = pSC_DATA->V_EQP_TIME;
	CString strEQP_COLOR = pSC_DATA->V_EQP_COLOR;
	CString strEQP_CONNECTED_YN = pSC_DATA->V_EQP_CONNECTED_YN;
	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);

	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
	{
		if (strEQP_COLOR != _T("RED"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_11, _T("SC_11호기"), RED, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("RED"));
		}
	}
	else
	{
		if (strEQP_COLOR != _T("GREEN"))
		{
			m_wndStatusBar.SetPaneInfo(ID_STATUS_SC_11, _T("SC_11호기"), GREEN, BLACK);
			pSC_DATA->SetEQP_COLOR(_T("GREEN"));
		}
	}
	return;
}


//void CMainFrame::OnUpdateStatusRtv1(CCmdUI* pCmdUI)
//{
//	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
//	if (pDoc == NULL)
//		return;
//	//m_pDoc = pDoc;
//
//	CRTV_DATA* pRTV_DATA = pDoc->GetRTV_DATA(_T("801"));
//
//	if (pRTV_DATA == NULL)
//		return;
//
//
//	CString strEQP_TIME = pRTV_DATA->V_EQP_TIME;
//	CString strEQP_COLOR = pRTV_DATA->V_EQP_COLOR;
//	CString strEQP_CONNECTED_YN = pRTV_DATA->V_EQP_CONNECTED_YN;
//	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//
//	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
//	{
//		if (strEQP_COLOR != _T("RED"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_RTV_1, _T("RTV_1호기"), RED, BLACK);
//			pRTV_DATA->SetEQP_COLOR(_T("RED"));
//		}
//	}
//	else
//	{
//		if (strEQP_COLOR != _T("GREEN"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_RTV_1, _T("RTV_1호기"), GREEN, BLACK);
//			pRTV_DATA->SetEQP_COLOR(_T("GREEN"));
//		}
//	}
//	return;
//}
//
//void CMainFrame::OnUpdateStatusBcr1(CCmdUI* pCmdUI)
//{
//	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
//	if (pDoc == NULL)
//		return;
//	//m_pDoc = pDoc;
//
//	CBCR_MST* pBCR_DATA = pDoc->GetBCR_MST(_T("701"));
//
//	if (pBCR_DATA == NULL)
//		return;
//
//	CString strEQP_TIME = pBCR_DATA->V_EQP_TIME;
//	CString strEQP_COLOR = pBCR_DATA->V_EQP_COLOR;
//	CString strEQP_CONNECTED_YN = pBCR_DATA->V_EQP_CONNECTED_YN;
//	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//
//	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
//	{
//		if (strEQP_COLOR != _T("RED"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_1, _T("BCR_1호기"), RED, BLACK);
//			pBCR_DATA->SetEQP_COLOR(_T("RED"));
//		}
//	}
//	else
//	{
//		if (strEQP_COLOR != _T("GREEN"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_1, _T("BCR_1호기"), GREEN, BLACK);
//			pBCR_DATA->SetEQP_COLOR(_T("GREEN"));
//		}
//	}
//	return;
//}
//
//void CMainFrame::OnUpdateStatusBcr2(CCmdUI* pCmdUI)
//{
//	CEcsDoc* pDoc = (CEcsDoc*)GetActiveDocument();
//	if (pDoc == NULL)
//		return;
//	//m_pDoc = pDoc;
//
//	CBCR_MST* pBCR_DATA = pDoc->GetBCR_MST(_T("702"));
//
//	if (pBCR_DATA == NULL)
//		return;
//
//	CString strEQP_TIME = pBCR_DATA->V_EQP_TIME;
//	CString strEQP_COLOR = pBCR_DATA->V_EQP_COLOR;
//	CString strEQP_CONNECTED_YN = pBCR_DATA->V_EQP_CONNECTED_YN;
//	int nEQP_TIME = CConvert::ToInt(strEQP_TIME);
//
//	if (nEQP_TIME > 5 || strEQP_CONNECTED_YN == _T("N"))
//	{
//		if (strEQP_COLOR != _T("RED"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_2, _T("BCR_2호기"), RED, BLACK);
//			pBCR_DATA->SetEQP_COLOR(_T("RED"));
//		}
//	}
//	else
//	{
//		if (strEQP_COLOR != _T("GREEN"))
//		{
//			m_wndStatusBar.SetPaneInfo(ID_STATUS_BCR_2, _T("BCR_2호기"), GREEN, BLACK);
//			pBCR_DATA->SetEQP_COLOR(_T("GREEN"));
//		}
//	}
//	return;
//}
