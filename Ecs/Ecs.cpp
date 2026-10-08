// Ecs.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "AfxWinAppEx.h"
#include "AfxDialogEx.h"
#include "Ecs.h"
#include "MainFrm.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEcsApp
//
BEGIN_MESSAGE_MAP(CEcsApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEcsApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()



// CEcsApp 생성
//
CEcsApp::CEcsApp()
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DLS.WCS.WCSServer.1001"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CEcsApp 개체입니다.
//
CEcsApp theApp;
CGlobal Global;


// CEcsApp 초기화
//
///////////////////////////////////////////////
// @.프로세스가 쓰는 모든 힙에 저단편화 힙(LFH)을 켠다.
//
//   이 프로그램은 1초마다 설비 전체를 다시 읽는다. 그 과정에서 ADO 가
//   행·칸마다 작은 덩어리를 수없이 잡았다 놓는다. 힙이 그만큼 벌집이 되고,
//   쓰지 못하는 구멍이 주소공간을 채운다. 실제로 멈춘 프로세스를 떠 보니
//   할당덩어리는 1,216개뿐인데 그 안의 조각이 442,215개였다.
//
//   LFH 는 작은 덩어리를 크기별 칸에 모아 주어 이 구멍을 크게 줄인다.
//   메인 힙은 요즘 윈도우에서 기본으로 켜져 있지만, ADO 의 커서 엔진처럼
//   제 힙을 따로 만들어 쓰는 쪽은 꺼진 채로 남는다. 그래서 전부 훑어 켠다.

static void EnableLowFragmentationHeap()
{
	HANDLE hHeaps[256] = { 0 };
	DWORD  nCount = ::GetProcessHeaps(256, hHeaps);
	if (nCount > 256)
		nCount = 256;

	for (DWORD ii = 0; ii < nCount; ii++)
	{
		ULONG ulMode = 2;	// 2 = LFH
		::HeapSetInformation(hHeaps[ii], HeapCompatibilityInformation, &ulMode, sizeof(ulMode));
	}
}


///////////////////////////////////////////////////////////////////////////////
// @.창 제목
//
//     WCS [빌드 2026-10-08 09:12][DB KET_WCS/postgres@localhost][D:\\...] 
//
//   빌드 시각은 돌고 있는 exe 의 파일 시각이다. __DATE__/__TIME__ 은 그 파일을
//   다시 컴파일할 때만 갱신되어, 다른 파일만 고친 빌드에서는 옛 시각이 남는다.
//   실행파일 위치를 적는 것은 같은 프로그램을 Bin\\Debug 와 Deploy\\Debug 양쪽에
//   두고 쓰기 때문이다. 어느 것이 돌고 있는지 제목만 보고 알 수 있다.
//   DB 접속정보는 쓰는 프로그램만 적는다. 없으면 그 칸을 뺀다.
//
//   제목 표시줄은 넘치는 글자를 잘라 버리므로, 길면 한 글자씩 흘려 보낸다.
//   메시지맵을 건드리지 않으려고 콜백형 타이머를 쓴다.

#define DEF_TITLE_TIMER		9701	// 제목 흘리기 타이머
#define DEF_TITLE_VIEW			110		// 한 번에 보여 줄 글자 수
#define DEF_TITLE_TICK			250		// 한 글자 넘기는 간격(ms)

static CString g_strWcsTitle;
static int     g_nWcsTitleOffset = 0;

static CString MakeWcsTitle()
{
	CString strTitle = _T("WCS");

	TCHAR szPath[_MAX_PATH] = { 0 };
	if (::GetModuleFileName(NULL, szPath, _MAX_PATH) == 0)
		return strTitle;

	// @.빌드 시각 = 돌고 있는 실행파일의 파일 시각
	WIN32_FILE_ATTRIBUTE_DATA fad;
	::ZeroMemory(&fad, sizeof(fad));
	if (::GetFileAttributesEx(szPath, GetFileExInfoStandard, &fad))
	{
		SYSTEMTIME stUtc, stLocal;
		::ZeroMemory(&stUtc, sizeof(stUtc));
		::ZeroMemory(&stLocal, sizeof(stLocal));

		if (::FileTimeToSystemTime(&fad.ftLastWriteTime, &stUtc))
		{
			if (!::SystemTimeToTzSpecificLocalTime(NULL, &stUtc, &stLocal))
				stLocal = stUtc;

			CString strBuild;
			strBuild.Format(_T(" [빌드 %04d-%02d-%02d %02d:%02d]"),
				stLocal.wYear, stLocal.wMonth, stLocal.wDay,
				stLocal.wHour, stLocal.wMinute);
			strTitle += strBuild;
		}
	}

	// @.DB 접속정보. Ecs.ini 에서 CConfig 와 같은 자리를 읽는다.
	//   설정이 비어 있으면(= DB 를 쓰지 않으면) 이 칸을 통째로 뺀다.
	{
		TCHAR szSrv[_MAX_PATH] = { 0 };
		TCHAR szDb[_MAX_PATH]  = { 0 };
		TCHAR szUid[_MAX_PATH] = { 0 };

		::GetPrivateProfileString(_T("DB_2"), _T("SERVER"),   _T(""), szSrv, _MAX_PATH, ECS_INI_FILE);
		::GetPrivateProfileString(_T("DB_2"), _T("DATABASE"), _T(""), szDb,  _MAX_PATH, ECS_INI_FILE);
		::GetPrivateProfileString(_T("DB_2"), _T("USERID"),   _T(""), szUid, _MAX_PATH, ECS_INI_FILE);

		CString strSrv(szSrv);
		if (strSrv.IsEmpty() == FALSE)
		{
			CString strDbInfo;
			strDbInfo.Format(_T(" [DB %s/%s@%s]"), szDb, szUid, szSrv);
			strTitle += strDbInfo;
		}
	}

	// @.실행파일이 있는 폴더
	CString strFolder(szPath);
	int nSlash = strFolder.ReverseFind(_T('\\'));
	if (nSlash > 0)
		strFolder = strFolder.Left(nSlash);

	strTitle += _T(" [") + strFolder + _T("]");

	return strTitle;
}

// @.한 글자씩 민다. 끝과 처음 사이에 빈 칸을 두어 경계를 알아보게 한다.
static VOID CALLBACK WcsTitleTimerProc(HWND hWnd, UINT, UINT_PTR, DWORD)
{
	if (::IsWindow(hWnd) == FALSE)
		return;

	CString strLoop = g_strWcsTitle + _T("      ");
	int nLen = strLoop.GetLength();
	if (nLen <= 0)
		return;

	if (g_nWcsTitleOffset >= nLen)
		g_nWcsTitleOffset = 0;

	CString strShow = strLoop.Mid(g_nWcsTitleOffset) + strLoop;
	::SetWindowText(hWnd, strShow.Left(DEF_TITLE_VIEW));

	g_nWcsTitleOffset++;
}

// @.제목을 걸고, 길면 흘리기를 시작한다.
static void SetWcsTitle(CWnd* pWnd)
{
	if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL)
		return;

	g_strWcsTitle = MakeWcsTitle();
	g_nWcsTitleOffset = 0;

	if (g_strWcsTitle.GetLength() <= DEF_TITLE_VIEW)
	{
		// @.다 보이면 흘릴 까닭이 없다.
		pWnd->SetWindowText(g_strWcsTitle);
		return;
	}

	pWnd->SetWindowText(g_strWcsTitle.Left(DEF_TITLE_VIEW));
	::SetTimer(pWnd->GetSafeHwnd(), DEF_TITLE_TIMER, DEF_TITLE_TICK, WcsTitleTimerProc);
}

BOOL CEcsApp::InitInstance()
{
	//dll등 다른 용도 사용
	//_CrtSetBreakAlloc(77997); 
	//AfxSetAllocStop();
	_CrtDumpMemoryLeaks();

	// @.가장 먼저 켜 둔다. 이미 만들어진 힙에도 적용된다.
	EnableLowFragmentationHeap();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_ulGdiplusToken, &gdiplusStartupInput, NULL);

	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

//HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("SKI6_7_ECS_SERVER"));
//if (::GetLastError() == ERROR_ALREADY_EXISTS)
//{
//	AfxMessageBox(_T("이미 ECS SERVER 프로그램이 실행중입니다."));
//	::CloseHandle(hMutex);
//	return FALSE;
//}

	::CoInitialize(NULL);
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
//	CSplashWnd::EnableSplashScreen(FALSE);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("WCS"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEcsDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CEcsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// DDE Execute 열기를 활성화합니다.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	// @.제목 : WCS [빌드][DB][실행파일 위치]. 길면 흘러간다.
	SetWcsTitle(m_pMainWnd);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();


//	MiniDumper mDump( _T("EcsDump") );

	return TRUE;
}

int CEcsApp::ExitInstance()
{
	::CoUninitialize();
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEcsApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
//
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CEcsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEcsApp 사용자 지정 로드/저장 메서드
//
void CEcsApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CEcsApp::LoadCustomState()
{
}

void CEcsApp::SaveCustomState()
{
}

// CEcsApp 메시지 처리기
//
BOOL CEcsApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}


