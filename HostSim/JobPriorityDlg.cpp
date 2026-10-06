// JobPriorityDlg.cpp : 작업 우선순위 변경(P) 전문 보내기
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "JobPriorityDlg.h"
#include "EcsDoc.h"
#include "Host.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CJobPriorityDlg, CDialog)
END_MESSAGE_MAP()

CJobPriorityDlg::CJobPriorityDlg(CEcsDoc* pDoc, CWnd* pParent)
	: CDialog(CJobPriorityDlg::IDD, pParent)
{
	m_pDoc = pDoc;
}

void CJobPriorityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CJobPriorityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// @.마지막으로 낸 작업번호를 기본값으로 넣어 둔다. 대개 그것을 바꾼다.
	CString strTemp;
	strTemp.Format(_T("%d"), m_pDoc->m_nPrevLuggNum);
	SetDlgItemText(IDC_EDIT_PRIO_LUGGNO, strTemp);
	SetDlgItemText(IDC_EDIT_PRIO_VALUE, _T("100"));

	return TRUE;
}

void CJobPriorityDlg::OnOK()
{
	CString strLuggNo, strPriority;
	GetDlgItemText(IDC_EDIT_PRIO_LUGGNO, strLuggNo);
	GetDlgItemText(IDC_EDIT_PRIO_VALUE, strPriority);

	int nLuggNum  = _ttoi(strLuggNo);
	int nPriority = _ttoi(strPriority);

	if (nLuggNum <= 0)
	{
		AfxMessageBox(_T("작업번호를 넣으십시오."));
		return;
	}

	// @.전문의 우선순위 칸은 세 자리다.
	if (nPriority < 0 || nPriority > 999)
	{
		AfxMessageBox(_T("우선순위는 0 ~ 999 사이로 넣으십시오."));
		return;
	}

	if (m_pDoc->m_pHostCl == NULL)
	{
		AfxMessageBox(_T("상위와 연결되어 있지 않습니다."));
		return;
	}

	m_pDoc->m_pHostCl->JobPriority(nLuggNum, nPriority);

	CDialog::OnOK();
}
