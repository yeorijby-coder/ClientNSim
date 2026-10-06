// TrackingErrorDlg.h : 트랙킹 오류 사유 보기
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"

class CEcsDoc;
class CTrackInfo;

// @.화물이 다음 트랙으로 넘어가지 않을 때 그 까닭을 한 줄씩 보여 준다.
//   이동 판정이 보는 것과 같은 순서로 따져 보고, 처음 걸리는 데서 멈춘다.

class CTrackingErrorDlg : public CDialog
{
public:
	CTrackingErrorDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CTrackingErrorDlg() {}

	enum { IDD = IDD_TRACKING_ERROR };

public:
	CEcsDoc* m_pDoc;

protected:
	CListCtrl	m_lstReason;
	BOOL		m_bOnlyBlocked;

protected:
	void UpdateList();

	// 한 트랙을 따져 본다. 막혔으면 TRUE 를 주고 strReason 에 까닭을 담는다.
	BOOL CheckTrack(CTrackInfo* pTrack, CString& strReason, int& nLuggNo, int& nDestNo);

	// 목적지에 걸린 다음트랙을 찾는다. 없으면 0.
	int  FindNextTrack(CTrackInfo* pTrack, int nDestNo, int& nNextPlcNum);

	// 트랙이 비어 있는가 (작업번호도 화물감지도 없는가)
	BOOL IsTrackEmpty(CTrackInfo* pTrack);

	int  GetDevNum(CTrackInfo* pTrack);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBtnRefresh();
	afx_msg void OnCheckOnlyBlocked();

	DECLARE_MESSAGE_MAP()
};
