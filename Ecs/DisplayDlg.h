#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CDisplayDlg 대화 상자

class CDisplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDisplayDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDisplayDlg();

	enum { IDD = IDD_SKIN_DISPLAY_CTRL};

public:
	CEcsDoc* m_pDoc;
	EN_LANG m_nLang;

public:
	LRESULT OnMessageSwitch(WPARAM wParam, LPARAM lParam);

	CSkinButton m_btnDisplayClose;
	CSkinButton m_btnDisplaySndData;
	CSkinButton m_btnDisplayInit;

	CTGroupBox m_grpDisplayDispTest;
	CTGroupBox m_grpDisplayTestData;

	CStaticTransparent m_lblDisplayController;
	CStaticTransparent m_lblDisplayDispNo;
	CStaticTransparent m_lblDisplayColor;
	CStaticTransparent m_lblDisplayData;

	CComboBoxWrapper m_cbxDisplayController;
	CComboBoxWrapper m_cbxDisplayDispNo;
	CComboBoxWrapper m_cbxDisplayColor;
	
	CEdit m_edtDisplayData;


public:
	void RenameResource(EN_LANG m_enLang = EN_ENG);

	void SetBindCombo_DISPLAY_CTRL(CComboBoxWrapper& cbx, CString strWh_Typ);
	void SetBindCombo_DISPLAY_NO(CComboBoxWrapper& cbx, CString strWh_Typ, CString strPlc_No);
	void SetBindCombo_COMMON_CODE(CComboBox& cbx, CString strCDX_CD, CString strCCD_NM);


protected:
	BOOL m_bInitialized;

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME

#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDisplayDisconnect();
	afx_msg void OnBnClickedBtnDisplaySnddata();
	afx_msg void OnBnClickedBtnDisplayInit();
};
