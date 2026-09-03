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


/*
 * GetBuildStamp :: 돌고 있는 실행파일의 최종 수정 시각
 *
 *   제목표시줄에 붙여, 어느 빌드가 도는지 창만 보고 알 수 있게 한다.
 *   원격지에 옛 실행파일이 남아 고친 것이 반영되지 않았는데도 코드를
 *   다시 뒤진 적이 있다. (실행 중인 exe 는 덮어쓰기가 막히는데 복사
 *   도구가 조용히 넘어가는 경우가 많다)
 *
 *   __DATE__ / __TIME__ 은 그 소스가 다시 컴파일될 때만 갱신되어
 *   다른 파일만 고친 빌드에서는 옛 시각이 남는다. 그래서 파일 시각을 쓴다.
 *   탐색기에서 보이는 값과 같으므로 원격지 파일과 눈으로 대조하기도 쉽다.
 */
static CString GetBuildStamp()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	if (::GetModuleFileName(NULL, szPath, _MAX_PATH) == 0)
		return _T("");

	WIN32_FILE_ATTRIBUTE_DATA fad;
	::ZeroMemory(&fad, sizeof(fad));
	if (!::GetFileAttributesEx(szPath, GetFileExInfoStandard, &fad))
		return _T("");

	SYSTEMTIME stUtc, stLocal;
	::ZeroMemory(&stUtc, sizeof(stUtc));
	::ZeroMemory(&stLocal, sizeof(stLocal));
	if (!::FileTimeToSystemTime(&fad.ftLastWriteTime, &stUtc))
		return _T("");
	if (!::SystemTimeToTzSpecificLocalTime(NULL, &stUtc, &stLocal))
		stLocal = stUtc;

	CString strStamp;
	strStamp.Format(_T("   [빌드 %04d-%02d-%02d %02d:%02d]"),
					stLocal.wYear, stLocal.wMonth, stLocal.wDay,
					stLocal.wHour, stLocal.wMinute);
	return strStamp;
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
	m_pMainWnd->SetWindowText(_T("Equipment Control System - ") + strPrcessName + GetBuildStamp());
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