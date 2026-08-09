// DciControl.cpp: implementation of the CDciControl class.
//

#include "stdafx.h"
#include "DciControl.h"


IMPLEMENT_SERIAL(CDciControl, CObject, DCI_SIRIALIZE_SCHEMA)

// CDciControl
CDciControl::CDciControl()
{
}

CDciControl::~CDciControl()
{
}

// CDciControl 멤버 함수
void CDciControl::InitControl(CDciMaster* pDCI)
{
	m_pDCI = pDCI;
	ASSERT(m_pDCI != NULL);

	m_strCID.Empty();
	m_strText.Empty();
	m_rcControlL.SetRectEmpty();

	m_nBgMode = OPAQUE;
	m_nFgMode = TRANSPARENT;

	m_clrBgColor = RGB(255,255,255);
	m_clrFgColor = RGB(0,0,0);

	m_bEnable = TRUE;
	m_bVisible = TRUE;
	
	m_bClick = FALSE;
	m_nFontSize = 10;
}

int CDciControl::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = 0;

	properties.SetSize(enControlPropSize);
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("id"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("l"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("b"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("r"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("t"));
	properties[i++].SetProperty(CDciProperty::PT_STRING, _T("text"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("bgmode"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fgmode"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("bgcolor"));
	properties[i++].SetProperty(CDciProperty::PT_COLOR, _T("fgcolor"));
	properties[i++].SetProperty(CDciProperty::PT_BOOL, _T("enable"));
	properties[i++].SetProperty(CDciProperty::PT_BOOL, _T("visible"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("fontsize"));
//	m_nFontSize
	ASSERT(properties.GetSize() == i);

	return i;
}

int CDciControl::UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject /* = TRUE */)
{
	int i = 0;
	
	if (bSaveObject)
	{
		m_strCID			= properties[i++].m_strValue;
		m_rcControlL.left	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.bottom = (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.right	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_rcControlL.top	= (LONG)CConvert::ToInt(properties[i++].m_strValue);
		m_strText			= properties[i++].m_strValue;
		m_nBgMode			= CConvert::ToInt(properties[i++].m_strValue);
		m_nFgMode			= CConvert::ToInt(properties[i++].m_strValue);
		m_clrBgColor		= CConvert::ToColor(properties[i++].m_strValue);
		m_clrFgColor		= CConvert::ToColor(properties[i++].m_strValue);
		m_bEnable			= (BOOL)CConvert::ToInt(properties[i++].m_strValue);
		m_bVisible			= (BOOL)CConvert::ToInt(properties[i++].m_strValue);
		m_nFontSize			= (LONG)CConvert::ToInt(properties[i++].m_strValue);
	}
	else	
	{
		properties[i++].m_strValue.Format(_T("%s"), m_strCID);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.left);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.bottom);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.right);
		properties[i++].m_strValue.Format(_T("%d"), m_rcControlL.top);
		properties[i++].m_strValue.Format(_T("%s"), m_strText);
		properties[i++].m_strValue.Format(_T("%d"), m_nBgMode);
		properties[i++].m_strValue.Format(_T("%d"), m_nFgMode);
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrBgColor));
		properties[i++].m_strValue.Format(_T("%s"), CConvert::ToString(m_clrFgColor));
		properties[i++].m_strValue.Format(_T("%d"), m_bEnable);
		properties[i++].m_strValue.Format(_T("%d"), m_bVisible);
		properties[i++].m_strValue.Format(_T("%d"), m_nFontSize);
	}

	return i;
}

void CDciControl::UpdateControl(CDC* pDC)
{
	if (!m_bVisible)
		return;

	// prepare locals so catch block can restore state
	CBrush* pOldBrush = NULL;
	CFont* pOldFont = NULL;
	int nOldMode = OPAQUE;
	COLORREF nOldFgColor = RGB(0,0,0);

	try
	{
		CBrush bgBrush(m_clrBgColor);

		// save previous modes (callers may pass invalid pDC but we assume pDC valid here)
		nOldMode = pDC->GetBkMode();
		nOldFgColor = pDC->GetTextColor();

		nOldMode = pDC->SetBkMode(m_nFgMode);
		nOldFgColor = pDC->SetTextColor(m_clrFgColor);

		if (m_nBgMode == TRANSPARENT)
			pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		else
			pOldBrush = pDC->SelectObject(&bgBrush);

		CRect rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
		rcControlS.InflateRect(0, 0, 1, 1);
		pDC->Rectangle(rcControlS);

		//==================================================================================================
		//==================================================================================================
		LOGFONT m_logfont;
		CFont m_font;

		int nSize = 0;
		int nHeight = rcControlS.Height();
		int nWidth = rcControlS.Width();
		nSize = nHeight;
		if (nHeight > nWidth)
			nSize = nWidth;

		int nStrCnt = 0;
		if (m_strText.GetLength() < 5)
			nStrCnt = 1;
		else
			nStrCnt = (m_strText.GetLength() / 5) + 1;

		if (m_strText.GetLength() > 20)
			nSize = nSize / 2;
		else
			nSize = nSize / nStrCnt;

		/*
		 * 폰트는 글자 크기에만 좌우된다. 예전에는 글자를 그릴 때마다
		 * CreateFontIndirect 로 새로 만들었는데, 컨트롤 수백 개를 초당 몇 번씩
		 * 그리는 화면에서는 그 값이 만만치 않다. (CPU 를 떠 보면 여기가 가장 위였다)
		 * 크기별로 한 번만 만들어 두고 다시 쓴다. 그리기는 화면 스레드에서만 일어난다.
		 */
		static CMap<int, int, CFont*, CFont*> mapFontCache;

		CFont* pCacheFont = NULL;
		if (mapFontCache.Lookup(m_nFontSize, pCacheFont) == FALSE)
		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfQuality = PROOF_QUALITY;
			lf.lfHeight  = m_nFontSize;
			lf.lfWeight  = FW_BOLD;
			lstrcpy(lf.lfFaceName, _T("Arial"));

			pCacheFont = new CFont;
			pCacheFont->CreateFontIndirect(&lf);
			mapFontCache.SetAt(m_nFontSize, pCacheFont);
		}

		pOldFont = (CFont*)pDC->SelectObject(pCacheFont);

		m_pDCI->DrawText(pDC, m_rcControlL, m_strText, m_clrFgColor);

		// restore DC state
		if (pOldBrush != NULL)
			pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(nOldFgColor);
		pDC->SetBkMode(nOldMode);

		if (pOldFont != NULL)
			pDC->SelectObject(pOldFont);

	}
	catch (CException* e)
	{
		// Attempt to restore DC state to avoid leaving DC with deleted GDI objects
		if (pOldBrush != NULL && pDC != NULL)
			pDC->SelectObject(pOldBrush);
		if (pOldFont != NULL && pDC != NULL)
			pDC->SelectObject(pOldFont);
		if (pDC != NULL)
		{
			pDC->SetTextColor(nOldFgColor);
			pDC->SetBkMode(nOldMode);
		}

		TCHAR szMessage[_MAX_PATH] = {0};
		e->GetErrorMessage(szMessage, _MAX_PATH);
		TRACE(_T("\n CDciControl::UpdateControl... CException! [%s] \n"), szMessage);
		e->Delete();
	}
}

void CDciControl::DrawFontText(CDC* pDC, CString strText, CRect* pRect, int nOldBkMode, int nOldFgColor, CBrush* pOldBrush, CPen* pOldPen)
{
//	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
//	int nOldFgColor = pDC->SetTextColor(m_clrFgColor);
//	CBrush* pOldBrush = NULL;
//	CPen* pOldPen = NULL;




	CRect rcControlS = NULL;
		
	if (pRect == NULL)
		rcControlS = m_pDCI->ConvertRectS(m_rcControlL);
	else
		rcControlS = *pRect;

	CString strRealText = _T("");

	if (strText == _T(""))
		strRealText = m_strText;
	else
		strRealText = strText;


	LOGFONT m_logfont;
	CFont m_font;
	CFont* pOldFont;

	int nSize = 0;
	int nHeight = rcControlS.Height();
	int nWidth = rcControlS.Width();
	nSize = nHeight;
	if (nHeight > nWidth)
		nSize = nWidth;

	int nStrCnt = 0;
	//if (m_strText.GetLength() < 5)
	if (strRealText.GetLength() < 5)
		nStrCnt = 1;
	else
		nStrCnt = (strRealText.GetLength() / 5) + 1;
	//	nStrCnt = (m_strText.GetLength() / 5) + 1;

	//if (m_strText.GetLength() > 20)
	if (strRealText.GetLength() > 20)
		nSize = nSize / 2;
	else
		nSize = nSize / nStrCnt;

	/*
	 * 폰트는 글자 크기에만 좌우된다. 예전에는 글자를 그릴 때마다
	 * CreateFontIndirect 로 새로 만들었는데, 컨트롤 수백 개를 초당 몇 번씩
	 * 그리는 화면에서는 그 값이 만만치 않다. (CPU 를 떠 보면 여기가 가장 위였다)
	 * 크기별로 한 번만 만들어 두고 다시 쓴다. 그리기는 화면 스레드에서만 일어난다.
	 */
	static CMap<int, int, CFont*, CFont*> mapFontCache;

	CFont* pCacheFont = NULL;
	if (mapFontCache.Lookup(m_nFontSize, pCacheFont) == FALSE)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfQuality = PROOF_QUALITY;
		lf.lfHeight  = m_nFontSize;
		lf.lfWeight  = FW_BOLD;
		lstrcpy(lf.lfFaceName, _T("Arial"));

		pCacheFont = new CFont;
		pCacheFont->CreateFontIndirect(&lf);
		mapFontCache.SetAt(m_nFontSize, pCacheFont);
	}

	pOldFont = pDC->SelectObject(pCacheFont);
	
	//pDC->DrawText(m_strText, rcControlS, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->DrawText(strRealText, rcControlS, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (pOldFont != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldFont != NULL)
		pDC->SelectObject(pOldBrush);

	pDC->SetTextColor(nOldFgColor);
	pDC->SetBkMode(nOldBkMode);

	// restore previous font to avoid leaving the DC with a pointer to a temporary font
	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}


void CDciControl::InvalidateControl(CWnd* pWnd, BOOL bErase)
{
	if ((pWnd == NULL) || (pWnd->GetSafeHwnd() == NULL))
		return;

	pWnd->InvalidateRect(m_pDCI->ConvertRectS(m_rcControlL), bErase);
}

void CDciControl::InvalidateControl(HWND hWnd, BOOL bErase)
{
	if (hWnd == NULL)
		return;

	::InvalidateRect(hWnd, m_pDCI->ConvertRectS(m_rcControlL), bErase);	
}

BOOL CDciControl::SetClick(CWnd* pWnd, const CPoint& ptClickS)
{
	if (m_bClick = m_pDCI->ConvertRectS(m_rcControlL).PtInRect(ptClickS))
		InvalidateControl(pWnd, FALSE);

	return m_bClick;
}

// CDciControlArray 멤버 함수
CDciControlArray::CDciControlArray()
{

}

CDciControlArray::~CDciControlArray()
{
	Clear();
}

void CDciControlArray::Clear()
{
	for (int i=0; i<m_nSize; ++i)
		delete m_pData[i];
	RemoveAll();
}

// CDciControlList 멤버 함수
CDciControlList::CDciControlList()
{

}

CDciControlList::~CDciControlList()
{
	Clear();
}

void CDciControlList::Clear()
{
	for (POSITION pos=GetHeadPosition(); pos!=NULL; )
		delete GetNext(pos);
	RemoveAll();
}
