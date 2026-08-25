#pragma once


// CDialogScDelFork 대화 상자

class CDialogScDelFork : public CDialog
{
//	DECLARE_DYNAMIC(CDialogScDelFork)

public:
	CDialogScDelFork(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogScDelFork();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_DIALOG_SC_DEL_FORK };

#endif

public:
	int		m_nDelFork;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
