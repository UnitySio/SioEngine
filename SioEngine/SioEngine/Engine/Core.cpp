#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "GamepadManager.h"
#include "Scene/SceneManager.h"
#include "AudioManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

LRESULT Core::WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return 0;
    }

    if (message == WM_GETMINMAXINFO)
    {
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = window_area_.bottom - window_area_.top;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.x = window_area_.right - window_area_.left;
        reinterpret_cast<MINMAXINFO*>(lParam)->ptMaxTrackSize.y = window_area_.bottom - window_area_.top;

        return 0;
    }

    if (message == WM_SETFOCUS ||
        message == WM_KILLFOCUS)
    {
        focus_ = GetFocus();

        return 0;
    }

    if (message == WM_DESTROY)
    {
        is_logic_loop_ = false;

        WaitForSingleObject(logic_handle_, INFINITE);

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        AUDIO_MANAGER->Release();
        SCENE_MANAGER->Release();
        GAMEPAD_MANAGER->Release();
        INPUT_MANAGER->Release();
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
    INPUT_MANAGER->Update();
    GAMEPAD_MANAGER->Update();
    SCENE_MANAGER->Update();

    if (INPUT_MANAGER->GetKey(MK_LBUTTON))
    {
        Vector2 mouse_position = INPUT_MANAGER->GetMousePosition();

        if (mouse_position.x > position_.x - scale_[0] &&
            mouse_position.x < position_.x + (scale_[0] * 2) &&
            mouse_position.y > position_.y - scale_[1] &&
            mouse_position.y < position_.y + (scale_[1] * 2))
        {
            position_ += INPUT_MANAGER->GetMouseDelta();
        }
    }

    if (INPUT_MANAGER->GetKey(MK_RBUTTON))
    {
        Vector2 mouse_position = INPUT_MANAGER->GetMousePosition();

        if (mouse_position.x > position_.x - scale_[0] &&
            mouse_position.x < position_.x + (scale_[0] * 2) &&
            mouse_position.y > position_.y - scale_[1] &&
            mouse_position.y < position_.y + (scale_[1] * 2))
        {
            z_rotation_++;

            if (z_rotation_ > 360.f)
            {
                z_rotation_ = 0.f;
            }
        }
    }
}

void Core::LateUpdate()
{
    SCENE_MANAGER->LateUpdate();
}

void Core::Render()
{
    SCENE_MANAGER->Render();

    GRAPHICS->FillRectangle(
        {position_.x, position_.y, scale_[0], scale_[1]},
        {255, 255, 255},
        z_rotation_
    );
}

void Core::OnGUI()
{
    SCENE_MANAGER->OnGUI();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                PostMessage(hWnd, WM_DESTROY, 0, 0);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    float* position[2] = {
        &position_.x,
        &position_.y
    };

    if (ImGui::Begin("Inspector"))
    {
        const Vector2 mouse_position = INPUT_MANAGER->GetMousePosition();

        ImGui::Text("FPS : %d", TIME_MANAGER->GetFPS());
        ImGui::Text("X: %.f, Y: %.f", mouse_position.x, mouse_position.y);
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::InputFloat2("Position", *position);
            ImGui::InputFloat("Z Rotation", &z_rotation_);
            ImGui::InputFloat2("Scale", scale_);
        }

        ImGui::Separator();
        ImGui::End();
    }

    if (ImGui::Begin("Hierarchy"))
    {
        ImGui::End();
    }

    if (ImGui::Begin("Console"))
    {
        if (ImGui::Button("Clear"))
        {
            logs_.clear();
        }

        ImGui::Separator();

        if (ImGui::BeginChild("Console"))
        {
            ImGuiListClipper clipper;
            clipper.Begin(logs_.size());

            while (clipper.Step())
            {
                for (size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    ImGui::Text(logs_[i].c_str());
                }
            }

            clipper.End();

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.f);
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
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
    timer_(),
    scale_{},
    position_{640.f, 360.f}
{
}

ATOM Core::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = GetInstance()->StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = kClassName;
    wcex.hIconSm = nullptr;

    return RegisterClassEx(&wcex);
}

BOOL Core::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    resolution_ = {1366, 768};
    window_area_ = {0, 0, resolution_.x, resolution_.y};
    AdjustWindowRect(&window_area_, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindowExW(
        0,
        kClassName,
        kWindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
        (screen_width - (window_area_.right - window_area_.left)) / 2,
        (screen_height - (window_area_.bottom - window_area_.top)) / 2,
        window_area_.right - window_area_.left,
        window_area_.bottom - window_area_.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hWnd == nullptr)
    {
        return false;
    }

    ShowWindow(hWnd, nCmdShow);

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (!GRAPHICS->Initaite())
    {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(".\\Font\\NanumBarunGothic.ttf", 14.f, nullptr, io.Fonts->GetGlyphRangesKorean());
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(GRAPHICS->d3d_device_, GRAPHICS->d3d_device_context_);

    TIME_MANAGER->Initaite();

    if (!AUDIO_MANAGER->Initiate())
    {
        return false;
    }

    logic_handle_ = CreateThread(nullptr, 0, LogicThread, nullptr, 0, nullptr);

    return true;
}

bool Core::InitiateWindow(HINSTANCE hInstance, int nCmdShow)
{
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return false;
    }

    return true;
}

bool Core::UpdateMessage()
{
    MSG msg = {};

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message != WM_QUIT;
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

    GRAPHICS->BeginRenderD3D();
    GRAPHICS->BeginRenderD2D();

    Render();

    GRAPHICS->EndRenderD2D();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    OnGUI();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    GRAPHICS->EndRenderD3D();

    AUDIO_MANAGER->Update();
}

void Core::SubLogic()
{
}

void Core::Log(std::wstring format, ...)
{
    va_list args;
    va_start(args, format);

    WCHAR buffer[1024];
    vswprintf_s(buffer, format.c_str(), args);

    OutputDebugStringW(buffer);

    va_end(args);
}
