#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "Time.h"

LRESULT Core::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (message == WM_GETMINMAXINFO)
    {
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = window_area_.right - window_area_.left;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = window_area_.bottom - window_area_.top;
        ((MINMAXINFO*)lParam)->ptMaxTrackSize.x = window_area_.right - window_area_.left;
        ((MINMAXINFO*)lParam)->ptMaxTrackSize.y = window_area_.bottom - window_area_.top;
        return 0;
    }

    if (message == WM_DESTROY)
    {
        Graphics::GetInstance()->Release();
        GetInstance()->Release();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Core::StaticWndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    return GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

void Core::Update()
{
    Time::GetInstance()->Update();
}

void Core::LateUpdate()
{
}

void Core::Render()
{
    Graphics::GetInstance()->BeginDraw();
    Graphics::GetInstance()->Clear(255.f, 255.f, 255.f);

    WCHAR word[1024];
    wsprintf(word, L"FPS: %d", Time::GetInstance()->GetFPS());

    Graphics::GetInstance()->DrawTextW(10.f, 10.f, 300.f, 30.f, word);

    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = 0; j < 100; j++)
        {
        Graphics::GetInstance()->DrawEllipse(100.f + (j * 10.f), 100.f + (i * 10.f), 32.f, 32.f);
        }
    }
    
    Graphics::GetInstance()->EndDraw();
}

Core::Core() :
    kClassName(L"SIOENGINE"),
    kWindowName(L"Sio Engine"),
    hWnd(),
    resolution_{},
    window_area_{}
{
}

ATOM Core::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wc = {};

    wc.lpfnWndProc = GetInstance()->StaticWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = kClassName;

    return RegisterClass(&wc);
}

BOOL Core::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    resolution_ = {640, 480};

    window_area_ = {0, 0, resolution_.x, resolution_.y};

    AdjustWindowRect(&window_area_, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(
        0,
        kClassName,
        kWindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
        (screen_width - (window_area_.right - window_area_.left)) / 2,
        (screen_height - (window_area_.bottom - window_area_.top)) / 2,
        window_area_.right - window_area_.left,
        window_area_.bottom - window_area_.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);

    Graphics::GetInstance()->Initiate();
    Time::GetInstance()->Initaite();

    return TRUE;
}

HWND Core::GetHWND()
{
    return hWnd;
}

void Core::Logic()
{
    Update();
    LateUpdate();
    Render();
}
