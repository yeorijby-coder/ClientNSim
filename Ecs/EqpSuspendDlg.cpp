// ScSuspendDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "EqpSuspendDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CScSuspendDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEqpSuspendDlg, CSkinDialog)

CEqpSuspendDlg::CEqpSuspendDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CEqpSuspendDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_nActiveRow = -1;
}

CEqpSuspendDlg::CEqpSuspendDlg(CEcsDoc* pDoc, CWnd* pParent )
	: CSkinDialog(CEqpSuspendDlg::IDD, pParent)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}


CEqpSuspendDlg::~CEqpSuspendDlg()
{
	m_pDoc->m_pEqpSuspendDlg = NULL;
	this->DestroyWindow();
}

void CEqpSuspendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EQP_SUSPEND,					m_pSpEQPSUSPEND);

	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_WH_TYP,		m_lblEqpSuspendWhTyp);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_EQP_TYP,		m_lblEqpSuspendEqpTyp);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_USE_YN,		m_lblEqpSuspendUseYn);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_CONNECTED_YN,  m_lblEqpSuspnedConnectedYn);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_1,				m_lblEqpSuspend);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_RESULT,		m_lblEqpSuspendResult);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_CNT,			m_lblEqpSuspendCnt);

	DDX_Control(pDX, IDC_LBL_PLC_IP_2,					m_lblEqpPLC_IP);
	DDX_Control(pDX, IDC_LBL_PLC_PORT_FROM_2,			m_lblEqpPLC_PORT_FROM);
	DDX_Control(pDX, IDC_LBL_PLC_TO_2,					m_lblEqpPLC_PORT_TO);
	DDX_Control(pDX, IDC_LBL_USE_YN_2,					m_lblEqpUSE_YN);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND2_3,			m_lblEqpSUSPEND);


	DDX_Control(pDX, IDC_EDIT_PLC_IP_2,					m_edtPLC_IP);
	
	DDX_Control(pDX, IDC_EDIT_PLC_PORT_FROM_2,			m_edtPLC_PORT_FROM);
	DDX_Control(pDX, IDC_EDIT_PLC_PORT_TO_2,			m_edtPLC_PORT_TO);

	DDX_Control(pDX, IDC_CBX_USE_YN_2,					m_cbxUSE_YN);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND2_2,			m_cbxEQP_SUSPEND);
	
	//
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_WH_TYP,		m_cbxEqpSuspendWhTyp);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_USE_YN,		m_cbxEqpSuspendUseYn);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_EQP_TYP,		m_cbxEqpSuspendEqpTyp);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_CONNECTED,		m_cbxEqpSusepndConnectedYn);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_EQP_SUSPEND,   m_cbxEqpSuspendEqpSuspend);
	//
	DDX_Control(pDX, IDC_GRP_EQP_EQP_STATE,				m_grpEqpEqpState);
	DDX_Control(pDX, IDC_GRP_EQP_SUSPEND,				m_grpEqpSuspend);
	DDX_Control(pDX, IDC_GRP_EQP_SUSPEND2,				m_grpEqpSuspend2); 
	//
	DDX_Control(pDX, ID_BTN_EQP_SUSPEND_SUSPEND,		m_btnEqpSuspend);
	DDX_Control(pDX, ID_BTN_EQP_SUSPEND_SEARCH,			m_btnEqpSuspendSearch);

}


BEGIN_MESSAGE_MAP(CEqpSuspendDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_EQP_SUSPEND_SEARCH, &CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSearch)
	ON_BN_CLICKED(ID_BTN_EQP_SUSPEND_SUSPEND, &CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSuspend)
	ON_BN_CLICKED(IDC_EQP_SUSPEND_CHECK_ALL, &CEqpSuspendDlg::OnBnClickedEqpSuspendCheckAll)
	ON_BN_CLICKED(IDC_CHK_SC_ACTIVE, &CEqpSuspendDlg::OnBnClickedChkScActive)
	ON_BN_CLICKED(IDC_CHK_SC_STO_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkScStoSuspend)
	ON_BN_CLICKED(IDC_CHK_SC_RET_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkRetSuspend)
	ON_BN_CLICKED(IDC_CHK_SC_ALL_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkScAllSuspend)
	ON_BN_CLICKED(IDC_CHK_OTHER_EQP_ACTIVE, &CEqpSuspendDlg::OnBnClickedChkOtherEqpActive)
	ON_BN_CLICKED(IDC_CHK_OTHER_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkOtherSuspend)
	ON_CBN_SELCHANGE(IDC_CBX_EQP_SUSPEND_EQP_TYP, &CEqpSuspendDlg::OnCbnSelchangeCbxEqpSuspendEqpTyp)
END_MESSAGE_MAP()


// CScSuspendDlg 메시지 처리기입니다.

BOOL CEqpSuspendDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxEqpSuspendWhTyp, _T("WH_TYP"), m_pDoc, (int)pEn);
	CLib::BindCombo(m_cbxEqpSuspendEqpTyp, _T("EQP_TYP"), m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxEqpSusepndConnectedYn, _T("CONNECTED_YN"), m_pDoc, (int)pEn, TRUE);
	
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
	m_SpreadSheet.AddColHead(_T("창고타입"), 9);
	m_SpreadSheet.AddColHead(_T("설비종류"), 9);
	m_SpreadSheet.AddColHead(_T("프로세스"), 13);
	m_SpreadSheet.AddColHead(_T("설비명"), 14);
	m_SpreadSheet.AddColHead(_T("설비 IP"), 13);
	m_SpreadSheet.AddColHead(_T("사용 포트"), 10);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	OnBnClickedBtnEqpSuspendSearch();
	Invalidate(TRUE);
	return TRUE;
}

void CEqpSuspendDlg::InitializeControlLanguage()
{
	//m_lblScWhTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("창고구분"), m_pDoc->m_enLang));
	//m_lblScSuspend.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("S/C상태"), m_pDoc->m_enLang));
	//
	//m_btnSearch.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	//
	//m_btnOk.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("확인"), m_pDoc->m_enLang));
	//m_btnCancel.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("취소"), m_pDoc->m_enLang));
}


void CEqpSuspendDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}

HCURSOR CEqpSuspendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEqpSuspendDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CEqpSuspendDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;
	m_btnEqpSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnEqpSuspend.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	m_btnEqpSuspendSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnEqpSuspendSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}

BOOL CEqpSuspendDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				break;
			}
		case VK_ESCAPE:
			{
				break;
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CEqpSuspendDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	CString bbb = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), bbb);
	//m_pSpEQPSUSPEND.SetText( nColIdx + 1, 0, variant_t(aaa));
}

void CEqpSuspendDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	//m_pSpEQPSUSPEND.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}
void CEqpSuspendDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_EQP_SUSPEND2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqptyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_EQP_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_EQP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpgrp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcip"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_IP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportfr"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_PORT_FROM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportto"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_PORT_TO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("connected"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_CONNECTED_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retryyn"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_RETRY_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("useyn"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_USE_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("useyn"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_USE_YN_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sceqp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_SC_EQP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("othereqp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_OTHER_EQP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpchangesta"), (int)m_enLang);
	SetDlgItemText(ID_BTN_EQP_SUSPEND_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcip"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_PLC_IP_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportfr"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_PLC_PORT_FROM_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportto"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_PLC_TO_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EQP_SUSPEND2_3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_EQP_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_EQP_SUSPEND2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_EQP_EQP_STATE, strValue);
	//	예전에는 Ecs.rc 캡션 그대로라 언어를 바꿔도 안 바뀌던 것들
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	SetDlgItemText(ID_BTN_EQP_SUSPEND_SEARCH, strValue);


}

void CEqpSuspendDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\128x32\\");
	CString strExtension = _T(".png");
	
	m_btnEqpSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 5);
	m_btnEqpSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 0);
	
	m_btnEqpSuspendSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 5);
	m_btnEqpSuspendSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
}

void CEqpSuspendDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CEqpSuspendDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pDoc->m_pEqpSuspendDlg =  NULL;
	CSkinDialog::OnClose();
}

void CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSearch()
{
	m_SpreadSheet.PrepareLoadSpread();

	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
//	int nColCnt;

	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main();

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

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((strColValue.GetLength()*2 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*2;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨
				//m_SpreadSheet.SetData(nIdxCol, nIdxRow, strColValue);		// 번역안됨

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

		//for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
		//{
		//	int nSize =CConvert::ToInt(arrColSize[nIdxCol]);

		//	//SetColWidth(nIdxCol, nSize);
		//	m_SpreadSheet.SetColWidth(nIdxCol + 1, nSize);
		//}
	}

	m_lblEqpSuspendCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();
	return;

}

void CEqpSuspendDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpEQPSUSPEND.SetMaxRows(pRowCnt);
}

void CEqpSuspendDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpEQPSUSPEND.SetMaxCols(pMaxCol);
}

void CEqpSuspendDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpEQPSUSPEND.put_ColWidth(pColCnt+1,pColSize);
}

void CEqpSuspendDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");
	strColValue = _T(" ") + strColValue;

	//m_pSpEQPSUSPEND.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CEqpSuspendDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

	return 0;
}

CString CEqpSuspendDlg::GetQrySelect_Main()
{
 	CString strSql = _T("");
	CString strTemp = _T("");															
	CString strEQP_TYP = _T("");
	CString strCONNECTED_YN = _T("");

	strEQP_TYP = m_cbxEqpSuspendEqpTyp.GetItemKey(m_cbxEqpSuspendEqpTyp.GetCurSel());
	strCONNECTED_YN = m_cbxEqpSusepndConnectedYn.GetItemKey(m_cbxEqpSusepndConnectedYn.GetCurSel());

	strEQP_TYP.Trim();
	strCONNECTED_YN.Trim();
	
	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, EM.WH_TYP) AS WH_TYP 																\n")	
			  _T("		     ,EM.EQP_TYP AS EQP_TYP																												\n")
			  _T("		     ,EM.PROCESS AS PROCESS																												\n")
			  _T("		     ,EM.REMARKS AS EQP_NM																												\n")
			  _T("		     ,EM.PLC_IP AS PLC_IP																												\n")
			  _T("		     ,EM.PLC_PORT AS PLC_PORT																											\n")
			  _T("       FROM EQP_MST EM																														\n")	
			  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP																						\n")
			  _T("								  ON CCD_WH_TYP.WH_TYP LIKE '%%%s%%'																			\n")	
			  _T("								 AND CCD_WH_TYP.CDX_CD = 'WH_TYP'																				\n")	
			  _T("								 AND EM.WH_TYP = CCD_WH_TYP.CCD_CD																				\n")	
			  _T("  WHERE EM.WH_TYP = '%s'											\n"), m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);				    
	
	
	if ((strEQP_TYP != "") && (strEQP_TYP != _T("ALL")))
	{	
		strSql += _T("AND EM.EQP_TYP ='") + strEQP_TYP + _T("' \n");	
	}
	
	if ((strCONNECTED_YN != "") && (strCONNECTED_YN != _T("ALL")))
	{		
		strSql += _T("AND EM.CONNECTED_YN ='") + strCONNECTED_YN + _T("' \n");	
	}
	
   	strSql += _T("   ORDER BY EM.PRIORITY \n");


   return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CEqpSuspendDlg::GetQryUpdate( CString strWH_TYP, CString strEQP_TYP, CString strPLC_NO, CString strEQP_NO)
{
	CString strTABLE_NM;
	CString strEQP_KEY = strEQP_TYP;
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	if (strEQP_TYP == _T("CV"))
	{
		strSql += CRLF + _T(" UPDATE CV_DATA");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND TRACK_NO = '") + strEQP_NO+ _T("'");
	}
	else if (strEQP_TYP == _T("RTV"))
	{
		strSql += CRLF + _T(" UPDATE RTV_DATA");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND RTV_NO = '") + strEQP_NO + _T("'");
	}
	else if (strEQP_TYP == _T("BCR"))
	{
		strSql += CRLF + _T(" UPDATE BCR_MST");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND BCR_NO = '0") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND BCR_MC_NO = '") +strEQP_NO + _T("'");
	
	}
	else if (strEQP_TYP == _T("SC"))
	{
		strSql += CRLF + _T(" UPDATE SC_DATA_MURATA");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strScEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND SC_NO = '") + strEQP_NO + _T("'");
	}

	return strSql;
}

CString CEqpSuspendDlg::GetQrySelect(int nLANG,CString strWH_TYP, CString strEQP_TYP, CString strEQP_SUSPEND, 
									 CString strEQP_NO, CString strEQP_GRP_NO, CString strPLC_IP,
									 CString strPLC_PORT_FR, CString strPLC_PORT_TO, 
									 CString strCONNECTED_YN, CString strRETRY_YN, CString strUSE_YN)
{
		return _T("");
}



void CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSuspend()
{
}

void CEqpSuspendDlg::OnBnClickedEqpSuspendCheckAll()
{
	
}

void CEqpSuspendDlg::OnBnClickedChkScActive()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkScStoSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkRetSuspend()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkScAllSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkOtherEqpActive()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkOtherSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnCbnSelchangeCbxEqpSuspendEqpTyp()
{
	CString strEqpTyp = _T("");

	UpdateData(TRUE);
	strEqpTyp = m_cbxEqpSuspendEqpTyp.GetItemKey(m_cbxEqpSuspendEqpTyp.GetCurSel());
	m_cbxEqpSuspendEqpSuspend.ResetContent();
	strEqpTyp += _T("_SUSPEND");
	CLib::BindCombo(m_cbxEqpSuspendEqpSuspend, strEqpTyp,  m_pDoc ,m_pDoc->m_enLang);
	UpdateData(FALSE);

}
BEGIN_EVENTSINK_MAP(CEqpSuspendDlg, CSkinDialog)
	ON_EVENT(CEqpSuspendDlg, IDC_EQP_SUSPEND, 5, CEqpSuspendDlg::ClickEqpSuspend, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CEqpSuspendDlg::ClickEqpSuspend(long Col, long Row)
{
	/*
	m_nActiveRow = Row;

	variant_t val;
	m_pSpEQPSUSPEND.GetText(2, Row, &val);
	CString strEQP_TYP = (LPCTSTR)(_bstr_t)val;
	strEQP_TYP.Trim();

	m_pSpEQPSUSPEND.GetText(3, Row, &val);
	CString strSUSPEND = (LPCTSTR)(_bstr_t)val;
	strSUSPEND.Trim();
	CString strBindName = strEQP_TYP + _T("_SUSPEND");
	CLib::BindCombo(m_cbxEQP_SUSPEND, strBindName, m_pDoc ,(int)m_pDoc->m_enLang);
	m_cbxEQP_SUSPEND.SetCurSelTextEx(strSUSPEND);
	//qq55

	m_pSpEQPSUSPEND.GetText(5, Row, &val);
	CString strPLC_IP = (LPCTSTR)(_bstr_t)val;
	strPLC_IP.Trim();
	m_edtPLC_IP.SetWindowText(strPLC_IP);

	m_pSpEQPSUSPEND.GetText(6, Row, &val);
	CString strPLC_PORT_FROM = (LPCTSTR)(_bstr_t)val;
	strPLC_PORT_FROM.Trim();
	m_edtPLC_PORT_FROM.SetWindowText(strPLC_PORT_FROM);

	m_pSpEQPSUSPEND.GetText(7, Row, &val);
	CString strPLC_PORT_TO = (LPCTSTR)(_bstr_t)val;
	strPLC_PORT_TO.Trim();
	m_edtPLC_PORT_TO.SetWindowText(strPLC_PORT_TO);

	m_pSpEQPSUSPEND.GetText(8, Row, &val);
	CString strUSE_YN = (LPCTSTR)(_bstr_t)val;
	strUSE_YN.Trim();
	CLib::BindCombo(m_cbxUSE_YN, _T("USE_YN"), m_pDoc ,(int)m_pDoc->m_enLang);
	m_cbxUSE_YN.SetCurSelTextEx(strUSE_YN);
	*/
}
