#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "TimeManager.h"
#include "Scene/SceneManager.h"
#include "InputManager.h"
#include "Audio/AudioManager.h"
#include "Audio/Audio.h"

LRESULT Core::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (message == WM_SIZE)
    {
        GRAPHICS->Resize();
    }
    else if (message == WM_GETMINMAXINFO)
    {
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = window_area_.bottom - window_area_.top;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.y = window_area_.bottom - window_area_.top;

        return 0;
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
    AUDIO_MANAGER->Update();

    if (INPUT_MANAGER->GetKey(VK_RIGHT))
    {
        x_ += 100.f * DELTA_TIME;
    }

    static Audio* audio = new Audio("GhostOfMyPast.mp3", true);
    if (!audio->IsPlaying())
    {
        audio->Play();
    }
    
}

void Core::LateUpdate()
{
    SCENE_MANAGER->LateUpdate();
}

void Core::Render()
{
    GRAPHICS->BeginDraw();
    GRAPHICS->ClearScreen({49, 77, 121});

    SCENE_MANAGER->Render();

    GRAPHICS->FillEllipse(
        {x_, y_, 32.f, 32.f},
        {}
    );

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
    timer_(),
    x_(32.f),
    y_(32.f)
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
    TIME_MANAGER->Update();
    timer_ += DELTA_TIME;

    while (timer_ >= TIME_MANAGER->fixed_time_step_)
    {
        FixedUpdate();
        timer_ -= TIME_MANAGER->fixed_time_step_;
    }

    Update();
    LateUpdate();
    Render();

    ReleaseSemaphore(semaphore_, 1, NULL);
}

void Core::SubLogic()
{
    WaitForSingleObject(semaphore_, INFINITE);
    INPUT_MANAGER->Update();
}
