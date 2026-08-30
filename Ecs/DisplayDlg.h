#pragma once


// CDisplayDlg 대화 상자

class CDisplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDisplayDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDisplayDlg();

	enum { IDD = IDD_DISPLAY_CTRL };

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME

#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
