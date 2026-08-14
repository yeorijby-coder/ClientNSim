// DisplayDlg.cpp: 구현 파일
//

//#include "pch.h"
#include "StdAfx.h"
#include "Ecs.h"
#include "DisplayDlg.h"
#include "afxdialogex.h"


// CDisplayDlg 대화 상자

IMPLEMENT_DYNAMIC(CDisplayDlg, CDialogEx)

CDisplayDlg::CDisplayDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayDlg::IDD, pParent)
{

}

CDisplayDlg::CDisplayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISPLAY_CTRL, pParent)
{

}

CDisplayDlg::~CDisplayDlg()
{
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialogEx)
END_MESSAGE_MAP()


// CDisplayDlg 메시지 처리기
