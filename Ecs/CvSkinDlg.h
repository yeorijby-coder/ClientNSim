#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CCvSkinDlg 대화 상자입니다.
class CCvSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CCvSkinDlg)

public:
	CCvSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CCvSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCvSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_CV_CTRL };

	enum {	EN_BtnWrite = 1, 
		    EN_BtnDelete = 2, 
			EN_BtnCopy = 3, 
			EN_BtnCut = 4,
			EN_BtnPaste = 5,
			EN_BtnErrReset = 6,
		  };

public:
	CEcsDoc* m_pDoc;

public:
	CString	m_strCopyJobNo;
	CString m_strCopyStartPos;
	CString m_strCopyDestPos;
	CString m_strCopyJobTyp;
	CString m_strCopyFmsRpt;
	CString m_strCopyTrayLev;
	CString m_strCopyTrayTyp;
	CString m_strCopyBcrTop;
	CString m_strCopyBcrBottom;
	CString m_strCopyPulpSensor;
	CString m_strTrackUseYn;

public:
	CTrackInfo* m_pTrackInfo;

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
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	EN_LANG m_nLang;

public:
	LRESULT OnMessageSwitch(WPARAM wParam, LPARAM lParam);
	//CSTATIC
	CStaticTransparent m_lblCvJobNo;	
	CStaticTransparent m_lblCvDestPos;
	CStaticTransparent m_lblCvJobTyp;
	CStaticTransparent m_lblCvBarcode;
	CStaticTransparent m_lblCvFmsRpt;
	CStaticTransparent m_lblCvTrayLev;
	CStaticTransparent m_lblCvTrayTyp;
	CStaticTransparent m_lblCvBcrTop;
	CStaticTransparent m_lblCvBcrBottom;
	CStaticTransparent m_lblCvJobJobNo;
	CStaticTransparent m_lblCvJobStartPos;
	CStaticTransparent m_lblCvJobDestPos;
	CStaticTransparent m_lblCvJobJobTyp;
	CStaticTransparent m_lblCvJobBcrTop;
	CStaticTransparent m_lblCvJobBcrBottom;
	CStaticTransparent m_lblCvJobMrcd;
	CStaticTransparent m_lblCvWaiteTIme;
	CStaticTransparent m_lblCvJobStatus;
	CStaticTransparent m_lblCvSuspend;

	//GROP
	//CVSTATUS
	CTGroupBox  m_grpCvCvStatus;
	CTGroupBox m_grpCvCvStatusItem;
	CTGroupBox m_grpCvCvStatusValue;
	CTGroupBox m_grpCvCvStatusCommand;
	CTGroupBox m_grpCvCvFunction;
	//CVSTATUS

	//JOBSTATUS
	CTGroupBox m_grpCvJobStatus;
	CTGroupBox m_grpCvJobStatusItem;
	CTGroupBox m_grpCvJobStatusValue;
	CTGroupBox m_grpCvTrackUseYn;
	//--JOBSTATUS

	//STATUS
	CTGroupBox m_grpCvStatus;
	CTGroupBox m_grpCvStatusSensor;
	CTGroupBox m_grpCvStatusStatus;
	CTGroupBox m_grpCvSuspend;	// 일시정지 그룹박스
	//--STATUS

	//TRACK_NO
	CEdit m_edtCvTrackNo;
	//--TRACK_NO

	//CVSTATUS
	CEdit m_edtCvLuggNo;
	CEdit m_edtCvJobNo;
	CEdit m_edtCvBcrTop;
	CEdit m_edtCvBcrTopUser;
	CEdit m_edtCvBcrBottom;
	CEdit m_edtCvBcrBottomUser;
	CEdit m_edtCvErrorCode;
	CEdit m_edtCvSuspend;
	CEdit m_edtCvWaitTimeOd;

	CComboBoxWrapper m_cbxCvStartPos;
	CComboBoxWrapper m_cbxCvDestPos; 
	CComboBoxWrapper m_cbxCvJobTyp;
	CComboBoxWrapper m_cbxCvFmsRpt;
	CComboBoxWrapper m_cbxCvTrayLev;
	CComboBoxWrapper m_cbxCvTrayTyp;
	CComboBoxWrapper m_cbxCvMesIFSta;
	CComboBoxWrapper m_cbxCvJobJobStatus;

	CSkinButton m_btnCvBcrTopTransfer;
	CSkinButton m_btnCvBcrBottomTransfer;
	//--CVSTATUS

	//JOBSTATUS
	CEdit m_edtCvJobJobNo;
	CEdit m_edtCvJobJobTyp;
	CEdit m_edtCvJobJobStatus;
	CEdit m_edtCvJobStartPos;
	CEdit m_edtCvJobDestPos;
	CEdit m_edtJobDestLoc;
	CEdit m_edtJobStartLoc;
	CEdit m_edtCvJobBcrTop;
	CEdit m_edtCvJobBcrTopUser;
	CEdit m_edtCvJobBcrBottom;
	CEdit m_edtCvJobBcrBottomUser;


	CSkinButton m_btnCvJobBcrToTransfer;
	CSkinButton m_btnCvJobBcrBottomTransfer;
	CSkinButton m_btnCvWaitTimeUp;
	CSkinButton m_btnCvWaitTimeDown;
	CSkinButton m_btnCvSuspend;
	CSkinButton m_btnCvActive;
	CSkinButton m_btnDpInsertJob;

	CStaticTransparent m_lblCvJobPalletNumber;
	CStaticTransparent m_lblCvJobProductId;
	CEdit m_edtCvJobPalletNumber;
	CEdit m_edtCvJobProductId;

	//--JOBSTATUS

	//SENSOR
	//CSkinButton m_btnCvRtvInSign;
	//CSkinButton m_btnCvRtvOutSign;
	//CSkinButton m_btnCvRtvLockSign;

	//--SENSOR

	//STATUS
	CSkinButton m_btnCvAutoMode;
	CSkinButton m_btnCvSenserProd0;
	CSkinButton m_btnCvTrPause;
	CSkinButton m_btnCvStoReady;
	CSkinButton m_btnCvRetReady;
	CSkinButton m_btnCvPickComp;
	CSkinButton m_btnCvStoHsReady;
	CSkinButton m_btnCvRetHsReady;
	CSkinButton m_btnCvDiverterHsDown;
	CSkinButton m_btnCvDiverterHsUp;
	CSkinButton m_btnCvDoorOpen;
	CSkinButton m_btnRgvStatusLoad;
	CSkinButton m_btnScPaPkPossible;
	CSkinButton m_btnCvDoorOpenReq;
	CSkinButton m_btnCvDoorCloseReq;
	CSkinButton m_btnCvMoter1Run;
	CSkinButton m_btnCvMoter2Run;
	CSkinButton m_btnCvSizeCheckerLow;
	CSkinButton m_btnCvSizeCheckerHigh;
	CSkinButton m_btnCvRetReadySta;
	CSkinButton m_btnCvPicking4ByPass;
	//CSkinButton m_btnCvRtvDepartHsReady;
	//CSkinButton m_btnCvRtvArriveHsReady;
	//--STATUS

	//COMMAND
	CSkinButton m_btnCvWrite;
	CSkinButton m_btnCvDelete;
	CSkinButton m_btnCvCopy;
	CSkinButton m_btnCvCut;
	CSkinButton m_btnCvPaste;
	CSkinButton m_btnCvDischarge;
	CSkinButton m_btnCvWaitTime;
	CSkinButton m_btnCvErr_Reset;
	//--COMMAND

	CSkinButton m_btnCvTrackPause;
	CEdit m_edtCvBarCode;
	CStaticTransparent m_lblCvTrackPause;

	CStaticTransparent m_lblJobDestLoc;
	CStaticTransparent m_lblJobStartLoc;
	
	CEdit m_edtMgStatus;
	CTGroupBox m_grpMgdpStatus;
	CStaticTransparent m_lblMgStatus;
	CSkinButton m_btnMzMove;
	CTGroupBox m_grpCvJobStatusValue5;
	CTGroupBox m_grpCvJobStatusItem3;
	CTGroupBox m_grpCvSensorInfo;
	CTGroupBox m_grpCvJobReadyStaInfo;

public:
	CString GetResString(LPCTSTR lpszKey);	// dlg_cv.ini 한 줄 읽기
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	void GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode);

public:
	void SetButtonCvImage( CButton& pButton, CString pFieldName, int pValue );

public:
	void InvalidateTrackData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
	void InvalidateComboBoxData(EN_LANG enLangTemp, CComboBox* pComboBox, CString pTemp);
	void UpdateTrackData(int pBtnJob);
	void SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx, CString strGroup_No, CString strDEST_POS_RD);
	void SetBindCombo_COMMON_CODE(CComboBox& cbx, CString strCDX_CD, CString strCCD_NM);
	void GetComBoBoxData(CComboBox& cbx, CString& strVal, int nlength);
	int GetDP_YN(CString pWH_TYP, CString pTrack);

public:
	BOOL IsValidataEditData(CString pJobNo, CString pDestPos, CString pJobTyp);

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnCvWrite();
	afx_msg void OnBnClickedBtnCvDelete();
	afx_msg void OnBnClickedBtnCvCopy();
	afx_msg void OnBnClickedBtnCvCut();
	afx_msg void OnBnClickedBtnCvPaste();
	afx_msg void OnBnClickedBtnCvBcrTopTransfer();
	afx_msg void OnBnClickedBtnCvBcrBottomTransfer();
	afx_msg void OnBnClickedBtnJobBcrTopTransfer();
	afx_msg void OnBnClickedBtnJobBcrBottomTransfer();

	afx_msg void OnBnClickedBtnCvWaitTimeUp();
	afx_msg void OnBnClickedBtnCvWaitTimeDown();
	afx_msg void OnBnClickedBtnCvSuspend();
	afx_msg void OnBnClickedBtnCvActive();
	afx_msg void OnBnClickedBtnCvErrReset();
	CString GetQrySelectJOB_MST( CCV_DATA* pCV_DATA );
	afx_msg void OnEnterSizeMove();
	afx_msg void OnExitSizeMove();

	void SetCvStatus(CString pLUGG_NO, CString pJOB_TYP, CString pDEST_POS, CString pTRACK_PAUSE, CString pCMD);
	void SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR);
	CEdit m_edtTrack;
	CButton m_chkAutoSel;
	CButton m_chkDebugMode;
	CButton m_chkTrPause;
	afx_msg void OnBnClickedChkAutoSel();
	BOOL m_blAutoSel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnMzMove();
	afx_msg void OnBnClickedChkAutoSel2();
	afx_msg void OnBnClickedChkTrPause();
	afx_msg void OnBnClickedBtnStatusRethsReady();
	afx_msg void OnBnClickedBtnCvDischarge();


};