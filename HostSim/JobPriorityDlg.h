// JobPriorityDlg.h : 작업 우선순위 변경(P) 전문 보내기
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"

class CEcsDoc;

// @.상위(WMS)가 이미 내린 작업의 우선순위를 바꾸라고 보내는 전문이다.
//   HOST 타스크의 ParseP() 가 받아 JOB_MST.JOB_PRIORITY 를 고친다.

class CJobPriorityDlg : public CDialog
{
public:
	CJobPriorityDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CJobPriorityDlg() {}

	enum { IDD = IDD_JOB_PRIORITY };

public:
	CEcsDoc* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
