#include "pch.h"
#include "SioEngine.h"
#include "Engine/Core.h"

int APIENTRY wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    if (!CORE->InitiateWindow(hInstance, nCmdShow))
    {
        return 0;
    }

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}
