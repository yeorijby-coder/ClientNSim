#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "ScManualRet.h"
#include "DialogScDelFork.h"


// CScSkinDlg 대화 상자입니다.

class CScSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CScSkinDlg)

public:
	CScSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CScSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_SC_CTRL };

	enum { 
		EN_BtnScConfirm = 1,
		EN_BtnScEmergency = 2,
		EN_BtnScActive = 3,
		EN_BtnScStop = 4,
		EN_BtnScErrReset = 5,
		EN_BtnScFk1Delete = 6,
		EN_BtnScFk2Delete = 7,
		EN_BtnScFk1Fk2Delete = 8,
		EN_BtnScCanel = 9,
		EN_BtnScCallToHome = 10,
		EN_BtnScSave = 11,
		EN_BtnScInit = 12,
		EN_BtnScDualStore = 13,
		EN_BtnScCut = 14,
		EN_BtnScManual = 15
	};


public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	CSc* m_Sc;
	CSC_DATA * m_pSC_DATA;
	CScManualRet* m_pScManualRet;
	CDialogScDelFork* m_pDialogScDelFor;

	CString m_strScFork;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void RedrawSize();
	//void InitializeResource(int pnTabIndex);
	void InitializeResource(EN_LANG nEN_LANG);
	void GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode);

public:
	CSkinButton m_btnScOk;
	CSkinButton m_btnScCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	EN_LANG m_nLang;

public:	
	LRESULT OnMessagSwitch(WPARAM wParam, LPARAM lParam);

	//SC지시정보
	CStaticTransparent m_lblScJobNoFork1;
	CStaticTransparent m_lblScJobTypFork1;
	CStaticTransparent m_lblScStartPosFork1;
	CStaticTransparent m_lblScDestPosFork1;
	CStaticTransparent m_lblScJobNoFork2;
	CStaticTransparent m_lblScJobTypFork2;
	CStaticTransparent m_lblScStartPosFork2;
	CStaticTransparent m_lblScDestPosFork2;

	CEdit m_edtScJobNoFork1;
	CEdit m_edtScJobTypFork1;
	CEdit m_edtScStartLocFork1;
	CEdit m_edtScDestLocFork1;
	CEdit m_edtScJobNoFork2;
	CEdit m_edtScJobTypFork2;
	CEdit m_edtScStartLocFork2;
	CEdit m_edtScDestLocFork2;

	//SC상태정보
	CStaticTransparent m_lblScOnlineModeRd;
	CStaticTransparent m_lblScAutoModeRd;
	CStaticTransparent m_lblScScStatusRd;
	CStaticTransparent m_lblScCompleteRd;
	CStaticTransparent m_lblScActiveModeRd;
	CStaticTransparent m_lblScPosVRd;
	CStaticTransparent m_lblScPosHRd;
	CStaticTransparent m_lblScSensorFkRd;

	CEdit m_edtScOnlineModeRd;
	CEdit m_edtScAutoModeRd;
	CEdit m_edtScScStatusRd;
	CEdit m_edtScCompleteRd;
	CEdit m_edtScActiveModeRd;
	CEdit m_edtScPosVRd;
	CEdit m_edtScPosHRd;
	CEdit m_edtScSensorFkRd;

	//포크상태
	CStaticTransparent m_lblScErrStaFk1Rd;
	CStaticTransparent m_lblScErrStaFk2Rd;

	CEdit m_edtScErrStaFk1Rd;
	CEdit m_edtScErrStaFk2Rd;

	//작업정보
	CStaticTransparent m_lblScItnLuggFk1;
	CStaticTransparent m_lblScItnLuggFk2;
	CStaticTransparent m_lblScBcrBottomFk1;
	CStaticTransparent m_lblScBcrBottomFk2;
	CStaticTransparent m_lblScJobTyp;
	CStaticTransparent m_lblScStoHs;
	CStaticTransparent m_lblScRetHs;
	CStaticTransparent m_lblScJobProgress;
	CStaticTransparent m_lblScProductId;

	CEdit m_edtScItnLuggFk1;
	CEdit m_edtScItnLuggFk2;
	CEdit m_edtScBcrBottomFk1;
	CEdit m_edtScBcrBottomFk2;
	CEdit m_edtScJobTyp;
	CEdit m_edtScStoHs;
	CEdit m_edtScRetHs;
	CEdit m_edtScJobProgress;
	CEdit m_edtScProductId;

	//에러정보
	CEdit m_edtScErrCode;


	//COMMAND버튼
	CSkinButton m_btnScConfirm;
	CSkinButton m_btnScDelete;
	CSkinButton m_btnScEmergency;
	CSkinButton m_btnScStop;
	CSkinButton m_btnScActive;
	CSkinButton m_btnScErrorReset;
	CSkinButton m_btnScCallToHome;
	CSkinButton m_btnDuplSto;
	CSkinButton m_btnCutJobInfo;
	CSkinButton m_btnScManual;


	//체크박스
	CButton m_chkScCheckStore;
	CButton m_chkScCheckRetive;
	CButton m_chkScFork1Fork2;



	CStaticTransparent m_lblScOnlineMode;
	CStaticTransparent m_lblScProdLoad;
	CStaticTransparent m_lblScHP;
	CStaticTransparent m_lblScStatus;
	CStaticTransparent m_lblScComplete;
	CStaticTransparent m_lblScWarmingup;
	CStaticTransparent m_lblScJobStatus;
	CStaticTransparent m_lblScProductID;
		
	CStaticTransparent m_lblScErrorStatus;
	CStaticTransparent m_lblScScStatus;
	CStaticTransparent m_lblScHorizontalPos;
	CStaticTransparent m_lblScVerticalPos;
	CStaticTransparent m_lblScForkingPos;

	CStaticTransparent m_lblScJobJobNo;
	CStaticTransparent m_lblScJobBcrTop;
	CStaticTransparent m_lblScJobBcrBottom;

	CStaticTransparent m_lblScSuspend;

	//GRP
	//SCSTATUS
	CTGroupBox m_grpScScJobInformation;
	CTGroupBox m_grpScScScStaInfo;
	CTGroupBox m_grpScScForkSta;
	CTGroupBox m_grpScScJobInformationFork1;
	CTGroupBox m_grpScScJobInformationFork2;


	//CTGroupBox m_grpScScStautsItem1;
	//CTGroupBox m_grpScScStatusValue1;
	//CTGroupBox m_grpScScStautsItem2;
	//CTGroupBox m_grpScScStatusValue2;
	//CTGroupBox m_grpScScStautsItem3;
	//CTGroupBox m_grpScScStatusValue3;
	//CTGroupBox m_grpScScStautsItem4;
	//CTGroupBox m_grpScScStautsItem5;
	//CTGroupBox m_grpScScStatusValue4;
	//CTGroupBox m_grpScScStatusValue5;

	CTGroupBox m_grpScScStautsCommand;
	//--SCSTATUS

	//JOBSTATUS
	CTGroupBox m_grpScInformation;
	CTGroupBox m_grpScErrorInformation;


	CTGroupBox m_grpScJobStatusItem1;
	CTGroupBox m_grpScJobStatusValue1;
	CTGroupBox m_grpScJobStatusValue2;
	CTGroupBox m_grpScJobStatusItem2;
	CTGroupBox m_grpScJobStatusCommand;
	CTGroupBox m_grpBottom;
	
	CTGroupBox m_grpScResponse;
	CTGroupBox m_grpScJobStatus2;
	CTGroupBox m_grpScSuspend;
	//--JOBSTATUS
	//--GRP

	//--CSTATIC


	//SC_NM
	CEdit m_edtScNo;
	//--SC_NM

	//SC_NO
	CEdit m_edtScNo2;
	//--SC_NO



	//JOBSTATUS


	CComboBoxWrapper m_cbxScJobTyp;
	CComboBoxWrapper m_cbxScStartPos;
	CComboBoxWrapper m_cbxScDestPos;
	//--JOBSTATUS

	//SCSTATUS
	CEdit m_edtScOnlineMode;
	CEdit m_edtScProdLoad;
	CEdit m_edtScStatus;
	CEdit m_edtScComplete;
	CEdit m_edtScWarmingUp;
	CEdit m_edtScErrorStatus;

	CEdit m_edtScJobJobTyp;
	CEdit m_edtScForkingPos;

	CEdit m_edtScJobStatus;
	CEdit m_edtScScStatus;
	CEdit m_edtScHorizontalPos;
	CEdit m_edtScVerticalPos;
	//--SCSTATUS

	//JOBSTATUS
	CEdit m_edtScJobJobNo;
	CEdit m_edtScJobJobStatus;
	CEdit m_edtScJobProductId;
	CEdit m_edtScJobBcrTop;
	CEdit m_edtScJobBcrBottom;
	CEdit m_edtScSuspend;
	CEdit m_edtRespCode;
	CEdit m_edtScHP;
	CEdit m_edtScJobDestLoc;
	CEdit m_edtScJobStartLoc;
	CStaticTransparent m_lblScJobDestLoc;
	CStaticTransparent m_lblScJobStartLoc;

	//--JOBSTATUS




	CSkinButton m_btnScInit;
	CSkinButton m_btnScStoSuspend;
	CSkinButton m_btnScRetSuspend;
	//CSkinButton m_btnScAllSuspend;




	BOOL    m_ChkSuspend;
	CSkinButton m_btnScManualRet;
	//--COMMAND

public:
	CString m_strScSuspend;

public:
	void SetButtonScImage( CButton& pButton, CString pFieldName, int pValue );

	void InvalidateScData(EN_LANG pLang);
	void InvalidateScDataSuspend(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
	void UpdateScData(int pBntJob);
	void GetCommonCode(CString pWH_TYP, CString pSC_NO);
	CString GetSelectQry(CString pWH_TYP, CString pSC_NO);
	CString GetUpdateQry(CString pWH_TYP, CString pPLC_NO, CString pSC_NO, CString pSuspned);

	//버튼 이벤트
	afx_msg void OnBnClickedBtnScConfirm();
	afx_msg void OnBnClickedBtnScDelte();
	afx_msg void OnBnClickedBtnScEmergency();
	afx_msg void OnBnClickedBtnScStop();
	afx_msg void OnBnClickedBtnScActive();
	afx_msg void OnBnClickedBtnScErrorReset();
	afx_msg void OnBnClickedBtnScCallToHome();
	afx_msg void OnBnClickedBtnScCutJobInfo();
	afx_msg void OnBnClickedBtnScManual();

	afx_msg void OnCheckStore();
	afx_msg void OnCheckRetrieve();

	//afx_msg void OnBnClickedBtnScOk();
	afx_msg void OnBnClickedBtnScCancel();
	afx_msg void OnClose();

	//afx_msg void OnBnClickedBtnScStoSuspend();
	//afx_msg void OnBnClickedBtnScRetSuspend();
	afx_msg void OnBnClickedChkScFork1();
	afx_msg void OnBnClickedChkScFork2();
	afx_msg void OnBnClickedScRtvFork12();
	afx_msg void OnBnClickedScDuplicationSto();

	CString GetQrySelectJOB_MST_FK1( CSC_DATA * pSC_DATA );
	CString GetQrySelectSC_STATUS_CCD( CSC_DATA* pSC_DATA );

	CString GetQrySelectSC_DATA(CSC_DATA* pSC_DATA);


	afx_msg void OnBnClickedBtnScManualRet();

	BOOL RetHsDataSelect(CString& strHS_MC_NO, CString& strSensor0DataRd);
	BOOL JobMstInsert(CString pHsMcNo, CString& pLuggNo);
	BOOL ScDataDelete();
	BOOL JobMstDelete(CString pLuggNo);
	BOOL CvDataUpdate(CString pMcNo, CString pLuggNo);
	void SetScStatus(CString pCMD, CString pSC_SUSPEND);
	void SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedGrpScJobStatusCommand();
	afx_msg void OnStnClickedLblScJobStartLoc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedBtnScInit();
};
