// DialogScDelFork.cpp: 구현 파일
//

#//include "pch.h"
#include "stdafx.h"
#include "Ecs.h"
#include "DialogScDelFork.h"
#include "afxdialogex.h"


// CDialogScDelFork 대화 상자

//IMPLEMENT_DYNAMIC(CDialogScDelFork, CDialog)

CDialogScDelFork::CDialogScDelFork(CWnd* pParent /*=nullptr*/)
	: CDialog(IDC_DIALOG_SC_DEL_FORK, pParent)
{
	m_nDelFork = -1;
}

CDialogScDelFork::~CDialogScDelFork()
{
}

void CDialogScDelFork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_DEL_FORK1, m_nDelFork);

}


BEGIN_MESSAGE_MAP(CDialogScDelFork, CDialog)
END_MESSAGE_MAP()


// CDialogScDelFork 메시지 처리기
