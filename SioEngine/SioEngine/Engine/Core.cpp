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

        GRAPHICS->Release();
        TIME->Release();
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
        GetInstance()->MainLogic();
    }

    return 0;
}

void Core::FixedUpdate()
{
    OutputDebugString(L"F\n");
}

void Core::Update()
{
    OutputDebugString(L"U\n");
}

void Core::LateUpdate()
{
}

void Core::Render()
{
    GRAPHICS->BeginDraw();
    GRAPHICS->Clear(49, 77, 121);

    WCHAR word[1024];
    wsprintf(word, L"FPS: %d", TIME->GetFPS());
    
    GRAPHICS->Label(FRect(10.f, 10.f, 100.f, 20.f), word);

    GRAPHICS->EndDraw();
}

Core::Core() :
    kClassName(L"SIOENGINE"),
    kWindowName(L"Sio Engine"),
    hWnd(),
    resolution_{},
    window_area_{},
    logic_handle_(),
    semaphore_(),
    is_logic_loop_(true),
    timer_()
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

    GRAPHICS->Initiate();
    TIME->Initaite();

    logic_handle_ = CreateThread(NULL, 0, LogicThread, NULL, 0, NULL);

    semaphore_ = CreateSemaphore(NULL, 0, 1, NULL);

    return TRUE;
}

HWND Core::GetHWND()
{
    return hWnd;
}

void Core::MainLogic()
{
    TIME->Update();
    timer_ += DELTA_TIME;

    while (timer_ >= TIME->fixed_time_step_)
    {
        FixedUpdate();
        timer_ -= TIME->fixed_time_step_;
    }

    Update();
    LateUpdate();
    Render();

    ReleaseSemaphore(semaphore_, 1, NULL);
}

void Core::SubLogic()
{
    WaitForSingleObject(semaphore_, INFINITE);
}
