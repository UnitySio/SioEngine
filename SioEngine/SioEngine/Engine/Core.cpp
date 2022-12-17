#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "TimeManager.h"
#include "Scene/SceneManager.h"
#include "InputManager.h"
#include "Audio/AudioManager.h"

LRESULT Core::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (message == WM_SIZE)
    {
        RECT rect = {};
        GetClientRect(Core::GetInstance()->GetHWND(), &rect);
        
        GRAPHICS->Resize(rect.right - rect.left, rect.bottom - rect.top);
    }
    else if (message == WM_GETMINMAXINFO)
    {
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = window_area_.bottom - window_area_.top;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.y = window_area_.bottom - window_area_.top;

        return 0;
    }
    else if (message == WM_SETFOCUS ||
        message == WM_KILLFOCUS)
    {
        focus_ = GetFocus();
    }
    else if (message == WM_DESTROY)
    {
        is_logic_loop_ = false;

        WaitForSingleObject(logic_handle_, INFINITE);

        AUDIO_MANAGER->Release();
        INPUT_MANAGER->Release();
        SCENE_MANAGER->Release();
        TIME_MANAGER->Release();
        GRAPHICS->Release();
        GetInstance()->Release();

        CoUninitialize();

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
    SCENE_MANAGER->FixedUpdate();
}

void Core::Update()
{
    SCENE_MANAGER->Update();
    INPUT_MANAGER->Update();
}

void Core::LateUpdate()
{
    SCENE_MANAGER->LateUpdate();
}

void Core::Render()
{
    SCENE_MANAGER->Render();

    GRAPHICS->DrawTextW(
        {10.f, 10.f, 100.f, 20.f},
        {},
        L"FPS: " + std::to_wstring(TIME_MANAGER->GetFPS())
    );
}

Core::Core() :
    kClassName(L"SIOENGINE"),
    kWindowName(L"Sio Engine"),
    hWnd(),
    focus_(),
    resolution_{},
    window_area_{},
    logic_handle_(),
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

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if (!GRAPHICS->Initiate())
    {
        return FALSE;
    }

    TIME_MANAGER->Initaite();

    if (!AUDIO_MANAGER->Initiate())
    {
        return FALSE;
    }

    AUDIO_MANAGER->AddSound(L"Ghost Of My Past", L"GhostOfMyPast.mp3", true);
    AUDIO_MANAGER->Play(L"Ghost Of My Past");

    logic_handle_ = CreateThread(NULL, 0, LogicThread, NULL, 0, NULL);

    return TRUE;
}

HWND Core::GetHWND()
{
    return hWnd;
}

HWND Core::GetHWNDFocus()
{
    return focus_;
}

POINT Core::GetResolution()
{
    return resolution_;
}

void Core::MainLogic()
{
    TIME_MANAGER->Update();
    timer_ += DELTA_TIME;

    while (timer_ >= TIME_MANAGER->fixed_time_step_)
    {
        FixedUpdate();
        timer_ -= TIME_MANAGER->fixed_time_step_;
    }

    Update();
    LateUpdate();

    GRAPHICS->BeginDraw();
    GRAPHICS->ClearScreen({49, 77, 121});

    Render();

    GRAPHICS->EndDraw();

    AUDIO_MANAGER->Update();
}
