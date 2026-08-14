#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CRtvSkinDlg 대화 상자입니다.

class CRtvSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CRtvSkinDlg)

public:
	CRtvSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CRtvSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRtvSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_RTV_CTRL };

	enum { 
		EN_BtnRtvConfirm = 1,
		EN_BtnRtvEmergency = 2,
		EN_BtnRtvAuto = 3,
		EN_BtnRtvStop = 4,
		EN_BtnRtvErrReset = 5,
		EN_BtnRtvCanel = 9,
		EN_BtnRtvCallToHome = 10,
		EN_BtnRtvSave = 11
	};

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	
	EN_LANG m_nLang;

public:
	CRtv* m_Rtv;
	CRTV_DATA* m_pRTV_DATA;


public:
	void GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	//void InitializeResource(int pnTabIndex);
	void InitializeResource(EN_LANG pnLANG);


	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:	
	LRESULT OnMessagSwitch(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//Label
	CStaticTransparent m_lblLuggNo;
	CStaticTransparent m_lblStartPos;
	CStaticTransparent m_lblDestPos;
	CStaticTransparent m_lblJobTyp;
	CStaticTransparent m_lblRtvMode;
	CStaticTransparent m_lblComplete;
	CStaticTransparent m_lblRtvProdLoad;
	CStaticTransparent m_lblRtvIsError;
	CStaticTransparent m_lblRtvStep;
	CStaticTransparent m_lblHorizontalPos;
	CStaticTransparent m_lblJobLuggNo;
	CStaticTransparent m_lblJobStartPos;
	CStaticTransparent m_lblJobDestPos;
	CStaticTransparent m_lblJobJobTyp;
	CStaticTransparent m_lblJobProductId;
	CStaticTransparent m_lblJobBcrBottom;
	CStaticTransparent m_lblJobJobStatus;

	//Edit
	CEdit m_edtRtvNo;
	CEdit m_edtRtvMode;
	CEdit m_edtWaiting;
	CEdit m_edtRtvProdLoad;
	CEdit m_edtRtvIsError;
	CEdit m_edtRtvStep;
	CEdit m_edtHorizontalPos;

	CEdit m_edtJobJobNo;
	CEdit m_edtJobJobStatus;
	CEdit m_edtJobProdInfo;
	CEdit m_edtDiagnosis;
	CEdit m_edtRtvSuspend;

	CEdit m_edtJobNo;
	CEdit m_edtRtvStartPos;
	CEdit m_edtRtvDestPos;
	CEdit m_edtRtvJobTyp;

	CComboBoxWrapper m_edtRtvJobStartPos;
	CComboBoxWrapper m_edtRtvJobDestPos;
	CComboBoxWrapper m_edtRtvJobJobTyp;
	CComboBoxWrapper m_edtRtvJobProductId;
	CComboBoxWrapper m_edtRtvJobBcrBottom;

	CTGroupBox m_grpRtvStatus;
	CTGroupBox m_grpRtvStatusItem;
	CTGroupBox m_grpRtvStatusFk1;
	CTGroupBox m_grpRtvStatusFk2;
	CTGroupBox m_grpRtvStatusCommand;

	CTGroupBox m_grpRtvStatusItem3;
	CTGroupBox m_grpRtvStatusRtv;
	CTGroupBox m_grpRtvJobStatus;
	CTGroupBox m_grpRtvJobStatusItem;
	CTGroupBox m_grpRtvJobStatusFk1;
	CTGroupBox m_grpRtvCommandRightBottom;

	CTGroupBox m_grpRtvErrorInfomation;
	CTGroupBox m_grpRtvSuspend;

	//Button
	CSkinButton m_btnRtvComplete;
	CSkinButton m_btnRtvEstop;
	CSkinButton m_btnRtvActive;
	CSkinButton m_btnRtvStop;
	CSkinButton m_btnRtvResetError;
	CSkinButton m_btnRtvDelete;
	CSkinButton m_btnRtvCallToHome;
	CSkinButton m_btnRtvManual;
	CSkinButton m_btnRtvSuspend;
	//

public:
	void SetButtonScImage( CButton& pButton, CString pFieldName, int pValue );

	void InvalidateRtvData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
	void UpdateRtvData(int pBntJob);

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRtvComplete();
	afx_msg void OnBnClickedBtnRtvEstop();
	afx_msg void OnBnClickedBtnRtvActive();
	afx_msg void OnBnClickedBtnRtvStop();
	afx_msg void OnBnClickedBtnRtvResetError();
	afx_msg void OnBnClickedBtnRtvDelete();
	afx_msg void OnBnClickedBtnRtvCallToHome();
	afx_msg void OnBnClickedBtnRtvSuspend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString GetQrySelectJOB_MST( CRTV_DATA* pRTV_DATA );
	CString GetQrySelectRTV_STATUS_CCD( CRTV_DATA* pRTV_DATA );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
