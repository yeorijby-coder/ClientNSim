// Ecs.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ecs.h"

#include "MainFrm.h"
#include "EcsDoc.h"
#include "EcsView.h"
//#include "Splash.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsApp

BEGIN_MESSAGE_MAP(CEcsApp, CWinApp)
	//{{AFX_MSG_MAP(CEcsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsApp construction

CEcsApp::CEcsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEcsApp object

CEcsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEcsApp initialization


// @.예전의 GetBuildStamp 는 아래 MakeWcsTitle 이 대신한다.
//   빌드 시각에 더해 DB 접속정보와 실행파일 위치까지 한 줄로 만든다.

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

	// @.이 프로그램은 DB 를 쓰지 않는다. DB 칸은 적지 않는다.

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
	CString strPrcessName;

	TCHAR szPath[_MAX_PATH] = { 0 };
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	TCHAR szTemp[_MAX_PATH] = { 0 };
	::GetPrivateProfileString(_T("COMMON"), _T("ProgramInfo"), _T("ProcessName"), szTemp, _MAX_PATH, ECS_INI_FILE);
	strPrcessName.Format(_T("%s"), szTemp);
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, strPrcessName);
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox(_T("이미") + strPrcessName + _T("프로그램이 실행중입니다."));
		::CloseHandle(hMutex);
		return FALSE;
	}

	::CoInitialize(NULL);

	// CG: The following block was added by the Splash Screen component.
//	CCommandLineInfo cmdInfo;
//	ParseCommandLine(cmdInfo);
//	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	/*
	CLogin dlg;
	
	if (dlg.DoModal() != IDOK)	return FALSE;
	m_strCurID = dlg.m_strUserID;
	m_strCurPwd = dlg.m_strPassword;
	*/
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Enable3dControls 는 더 이상 필요 없다 (MFC 7 이후 기본 동작)

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("SFA"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEcsDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEcsView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// The one and only window has been initialized, so show and update it.
	// @.제목 : WCS [빌드][DB][실행파일 위치]. 길면 흘러간다.
	SetWcsTitle(m_pMainWnd);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CEcsApp::ExitInstance() 
{
	::CoUninitialize();

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEcsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
BOOL CEcsApp::PreTranslateMessage(MSG* pMsg)
{

	
	return CWinApp::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////
// CEcsApp message handlers