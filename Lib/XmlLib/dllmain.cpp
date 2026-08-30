// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE XmlLIbDLL = { NULL, NULL };

// DllMain 은 DLL 진입점이라 static 이 무시된다(C4008). 그녑 외부 연결로 둔다.
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("XmlLib.DLL 초기화\n");
        if (!AfxInitExtensionModule(XmlLIbDLL, hInstance))
            return 0;
        new CDynLinkLibrary(XmlLIbDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("XmlLib.DLL 종료\n");
        AfxTermExtensionModule(XmlLIbDLL);
    }
    return 1;
}
