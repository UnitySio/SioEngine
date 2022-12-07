#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "TimeManager.h"

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
        is_logic_loop_ = false;

        WaitForSingleObject(logic_handle_, INFINITE);

        Graphics::GetInstance()->Release();
        TimeManager::GetInstance()->Release();
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

DWORD WINAPI Core::LogicThread(LPVOID lpParam)
{
    while (GetInstance()->is_logic_loop_)
    {
        GetInstance()->Logic();
    }

    return 0;
}

void Core::FixedUpdate()
{
    OutputDebugString(L"Fixed Update\n");
    x2_ += 100.f * FIXED_TIME_STEP;
    x4_ += 100.f;
}

void Core::Update()
{
    OutputDebugString(L"Update\n");
    x1_ += 100.f * DELTA_TIME;
    x3_ += 100.f;
}

void Core::LateUpdate()
{
}

void Core::Render()
{
    Graphics::GetInstance()->BeginDraw();
    Graphics::GetInstance()->Clear(49, 77, 121);

    Graphics::GetInstance()->FillEllipse(x1_, y1_, 32.f, 32.f);
    Graphics::GetInstance()->FillEllipse(x2_, y2_, 32.f, 32.f);
    Graphics::GetInstance()->FillEllipse(x3_, y3_, 32.f, 32.f);
    Graphics::GetInstance()->FillEllipse(x4_, y4_, 32.f, 32.f);

    Graphics::GetInstance()->EndDraw();
}

Core::Core() :
    kClassName(L"SIOENGINE"),
    kWindowName(L"Sio Engine"),
    hWnd(),
    resolution_{},
    window_area_{},
    logic_handle_(),
    is_logic_loop_(true),
    timer_(),
    x1_(32.f),
    y1_(32.f),
    x2_(32.f),
    y2_(96.f),
    x3_(32.f),
    y3_(160.f),
    x4_(32.f),
    y4_(224.f)
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

    resolution_ = {1280, 720};

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
    TimeManager::GetInstance()->Initaite();

    logic_handle_ = CreateThread(NULL, 0, LogicThread, NULL, 0, NULL);

    return TRUE;
}

HWND Core::GetHWND()
{
    return hWnd;
}

void Core::Logic()
{
    TimeManager::GetInstance()->Update();
    timer_ += DELTA_TIME;

    while (timer_ >= FIXED_TIME_STEP)
    {
        FixedUpdate();
        timer_ -= FIXED_TIME_STEP;
    }

    Update();
    LateUpdate();
    Render();
}
