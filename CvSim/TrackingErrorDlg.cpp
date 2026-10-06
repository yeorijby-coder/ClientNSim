// TrackingErrorDlg.cpp : 트랙킹 오류 사유 보기
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "TrackingErrorDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TRACKING_ERROR_TIMER		1

BEGIN_MESSAGE_MAP(CTrackingErrorDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TRACKING_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_CHECK_ONLY_BLOCKED, OnCheckOnlyBlocked)
END_MESSAGE_MAP()

CTrackingErrorDlg::CTrackingErrorDlg(CEcsDoc* pDoc, CWnd* pParent)
	: CDialog(CTrackingErrorDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bOnlyBlocked = TRUE;
}

void CTrackingErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRACKING_ERROR, m_lstReason);
	DDX_Check(pDX, IDC_CHECK_ONLY_BLOCKED, m_bOnlyBlocked);
}

BOOL CTrackingErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	const int nSize = 6;
	LPTSTR	pszText[nSize] = { _T("PLC"), _T("트랙"), _T("작업번호"), _T("목적지"), _T("다음트랙"), _T("사유") };
	int		cx[nSize]      = { 34, 50, 64, 50, 60, 420 };

	for (int i = 0; i < nSize; ++i)
		m_lstReason.InsertColumn(i, pszText[i], LVCFMT_LEFT, cx[i]);

	m_lstReason.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);
	UpdateList();

	SetTimer(ID_TRACKING_ERROR_TIMER, 1000, NULL);

	return TRUE;
}

void CTrackingErrorDlg::PostNcDestroy()
{
	m_pDoc->m_pTrackingErrorDlg = NULL;
	delete this;

	CDialog::PostNcDestroy();
}

void CTrackingErrorDlg::OnOK()
{
	KillTimer(ID_TRACKING_ERROR_TIMER);
	DestroyWindow();
}

void CTrackingErrorDlg::OnCancel()
{
	OnOK();
}

// @.목록을 보는 중에 엔터로 창이 닫히면 성가시다. Logic Validation 과 같이 막는다.
BOOL CTrackingErrorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateList();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTrackingErrorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_TRACKING_ERROR_TIMER)
		UpdateList();

	CDialog::OnTimer(nIDEvent);
}

void CTrackingErrorDlg::OnBtnRefresh()
{
	UpdateList();
}

void CTrackingErrorDlg::OnCheckOnlyBlocked()
{
	UpdateData(TRUE);
	UpdateList();
}

////////////////////////////////////////////////////////////////////////////////////////////////

int CTrackingErrorDlg::GetDevNum(CTrackInfo* pTrack)
{
	int nPlcIdx = pTrack->m_nCvPlcNum - 1;
	if (nPlcIdx < 0 || nPlcIdx >= CV_PLC_CNT)
		return -1;

	return (pTrack->m_nNumber - m_pDoc->m_nStTrNum[nPlcIdx] + 1) * m_pDoc->m_nWordCnt;
}

BOOL CTrackingErrorDlg::IsTrackEmpty(CTrackInfo* pTrack)
{
	int nPlcIdx = pTrack->m_nCvPlcNum - 1;
	int nDevNum = GetDevNum(pTrack);
	if (nDevNum < 0)
		return TRUE;

	int nLuggNo  = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("LuggNum"));
	int nSensing = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("ProductSensor"));

	return (nLuggNo == 0 && nSensing == 0);
}

// 이동 판정이 목적지로 다음트랙을 찾는 방법과 같다.
int CTrackingErrorDlg::FindNextTrack(CTrackInfo* pTrack, int nDestNo, int& nNextPlcNum)
{
	nNextPlcNum = 0;

	int nLen = pTrack->m_nStationArray.GetSize();
	for (int i = 0; i < nLen; ++i)
	{
		int nStation = pTrack->m_nStationArray[i];
		CStationInfo* pStationInfo = (i < m_pDoc->m_pStationInfos.GetSize()) ? m_pDoc->m_pStationInfos[i] : NULL;
		int nDestCode = (pStationInfo != NULL && pStationInfo->m_pTrack != NULL) ? pStationInfo->m_pTrack->m_nDestCode : 0;

		if ((nStation != 0 && nStation == nDestNo) ||
			((pStationInfo != NULL) && (_ttoi(pStationInfo->m_strID) == nDestNo)) ||
			(nDestCode == nDestNo))
		{
			nNextPlcNum = pTrack->m_nNextPlcArray[i];
			return pTrack->m_nNextTrArray[i];
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// @.이동 판정이 보는 순서 그대로 따져 본다. 처음 걸리는 데서 멈추고 그 까닭을 돌려준다.

BOOL CTrackingErrorDlg::CheckTrack(CTrackInfo* pTrack, CString& strReason, int& nLuggNo, int& nDestNo)
{
	strReason.Empty();
	nLuggNo = 0;
	nDestNo = 0;

	int nPlcIdx = pTrack->m_nCvPlcNum - 1;
	int nDevNum = GetDevNum(pTrack);
	if (nDevNum < 0)
		return FALSE;

	nLuggNo      = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("LuggNum"));
	nDestNo      = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("DestPos"));
	int nSensing = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("ProductSensor"));
	int nAuto    = m_pDoc->GetAddrByName(nPlcIdx, nDevNum, _T("Auto"));

	// 실려 있는 게 없으면 따질 것도 없다.
	if (nLuggNo == 0 && nSensing == 0)
		return FALSE;

	if (nAuto == 0)
	{
		strReason = _T("이 트랙이 수동입니다");
		return TRUE;
	}

	if (nSensing == 0)
	{
		strReason.Format(_T("작업번호(%d)는 있는데 화물감지가 꺼져 있습니다"), nLuggNo);
		return TRUE;
	}

	if (nLuggNo == 0)
	{
		strReason = _T("화물은 있는데 작업번호가 없습니다 - 상위가 작업을 내려야 움직입니다");
		return TRUE;
	}

	if (nDestNo == 0)
	{
		strReason = _T("목적지가 없습니다");
		return TRUE;
	}

	// 여기가 종착지면 안 움직이는 게 맞다.
	if (nDestNo == pTrack->m_nNumber ||
		(pTrack->m_nDestCode != 0 && nDestNo == pTrack->m_nDestCode))
	{
		strReason = _T("여기가 종착지입니다 - 상위가 가져가야 합니다");
		return TRUE;
	}

	int nNextPlcNum = 0;
	int nNextTrNum = FindNextTrack(pTrack, nDestNo, nNextPlcNum);

	if (nNextTrNum == 0)
	{
		strReason.Format(_T("목적지 %d 에 다음트랙이 등록되어 있지 않습니다 - 작업정보 창에서 등록하십시오"), nDestNo);
		return TRUE;
	}

	// 출발조건 - 적어 둔 트랙이 다 비어야 떠난다.
	CString strWait = pTrack->GetWaitTracks(nDestNo);
	if (strWait.IsEmpty() == FALSE)
	{
		int nPos = 0;
		CString strOne = strWait.Tokenize(_T(","), nPos);

		while (strOne.IsEmpty() == FALSE)
		{
			strOne.Trim();
			int nWaitTr = _ttoi(strOne);

			if (nWaitTr > 0 && nWaitTr != pTrack->m_nNumber)
			{
				CTrackInfo* pWait = m_pDoc->GetTrackInfo(nWaitTr);
				if (pWait != NULL && IsTrackEmpty(pWait) == FALSE)
				{
					strReason.Format(_T("출발조건에 걸렸습니다 - 트랙 %d 가 아직 차 있습니다 [조건 %s]"),
									 nWaitTr, (LPCTSTR)strWait);
					return TRUE;
				}
			}

			strOne = strWait.Tokenize(_T(","), nPos);
		}
	}

	CTrackInfo* pNext = m_pDoc->GetTrackInfo(nNextTrNum, nNextPlcNum);
	if (pNext == NULL)
	{
		strReason.Format(_T("다음트랙 %d 을 찾을 수 없습니다 - EcsDefine.xml 을 확인하십시오"), nNextTrNum);
		return TRUE;
	}

	int nNextIdx = pNext->m_nCvPlcNum - 1;
	int nNextDev = GetDevNum(pNext);
	if (nNextDev < 0)
	{
		strReason.Format(_T("다음트랙 %d 의 자리를 계산할 수 없습니다"), nNextTrNum);
		return TRUE;
	}

	int nNextAuto    = m_pDoc->GetAddrByName(nNextIdx, nNextDev, _T("Auto"));
	int nNextLuggNo  = m_pDoc->GetAddrByName(nNextIdx, nNextDev, _T("LuggNum"));
	int nNextSensing = m_pDoc->GetAddrByName(nNextIdx, nNextDev, _T("ProductSensor"));

	if (nNextAuto == 0)
	{
		strReason.Format(_T("다음트랙 %d 가 수동입니다"), nNextTrNum);
		return TRUE;
	}

	if (nNextLuggNo != 0 || nNextSensing != 0)
	{
		strReason.Format(_T("다음트랙 %d 가 차 있습니다 (작업번호 %d%s)"),
						 nNextTrNum, nNextLuggNo, nNextSensing ? _T(", 화물감지 ON") : _T(""));
		return TRUE;
	}

	strReason.Format(_T("조건은 다 맞습니다 - 다음트랙 %d 로 곧 넘어갑니다"), nNextTrNum);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CTrackingErrorDlg::UpdateList()
{
	if (m_pDoc == NULL || m_lstReason.GetSafeHwnd() == NULL)
		return;

	// 보고 있던 줄을 지키려고 맨 위 줄을 기억해 둔다. 1 초마다 다시 그리기 때문이다.
	int nTop = m_lstReason.GetTopIndex();

	m_lstReason.SetRedraw(FALSE);
	m_lstReason.DeleteAllItems();

	CString strReason, strTemp;
	int nRow = 0;

	int nLen = m_pDoc->m_pTrackInfos.GetSize();
	for (int i = 0; i < nLen; ++i)
	{
		CTrackInfo* pTrack = m_pDoc->m_pTrackInfos[i];
		if (pTrack == NULL)
			continue;

		int nLuggNo = 0, nDestNo = 0;
		BOOL bBlocked = CheckTrack(pTrack, strReason, nLuggNo, nDestNo);

		if (strReason.IsEmpty())
			continue;							// 실려 있는 게 없는 트랙

		if (m_bOnlyBlocked && bBlocked == FALSE)
			continue;

		strTemp.Format(_T("%d"), pTrack->m_nCvPlcNum);
		m_lstReason.InsertItem(nRow, strTemp);

		strTemp.Format(_T("%d"), pTrack->m_nNumber);
		m_lstReason.SetItemText(nRow, 1, strTemp);

		strTemp.Format(_T("%d"), nLuggNo);
		m_lstReason.SetItemText(nRow, 2, strTemp);

		strTemp.Format(_T("%d"), nDestNo);
		m_lstReason.SetItemText(nRow, 3, strTemp);

		int nNextPlcNum = 0;
		int nNextTrNum = (nDestNo > 0) ? FindNextTrack(pTrack, nDestNo, nNextPlcNum) : 0;
		strTemp.Format(_T("%d"), nNextTrNum);
		m_lstReason.SetItemText(nRow, 4, strTemp);

		m_lstReason.SetItemText(nRow, 5, strReason);

		++nRow;
	}

	if (nTop > 0 && nTop < m_lstReason.GetItemCount())
		m_lstReason.EnsureVisible(nTop, FALSE);

	m_lstReason.SetRedraw(TRUE);
}
