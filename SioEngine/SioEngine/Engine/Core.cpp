#include "pch.h"
#include "Core.h"
#include "Graphics.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "GamepadManager.h"
#include "AudioManager.h"
#include "Scene/SceneManager.h"
#include "EventManager.h"
#include "Scene/Scene.h"
#include "Object/Object.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Object/Component/Transform.h"

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

        EVENT_MANAGER->Release();
        SCENE_MANAGER->Release();
        AUDIO_MANAGER->Release();
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
#ifdef NDEBUG
    SCENE_MANAGER->FixedUpdate();
#endif
}

void Core::Update()
{
    INPUT_MANAGER->Update();
    GAMEPAD_MANAGER->Update();

#ifdef NDEBUG
    SCENE_MANAGER->Update();
#endif
}

void Core::LateUpdate()
{
#ifdef NDEBUG
    SCENE_MANAGER->LateUpdate();
#endif
}

void Core::Render()
{
    SCENE_MANAGER->Render();

    auto* current_scene = SCENE_MANAGER->GetCurrentScene();

    const auto& objects = current_scene->GetObjects(static_cast<Layer>(0));
    
    for (size_t j = 0; j < objects.size(); j++)
    {
        const auto transform = objects[j]->GetTransform();

        GRAPHICS->FillRectangle(
            {transform->position.x, transform->position.y, transform->scale.x, transform->scale.y},
            {255, 255, 255},
            transform->z_rotation
        );
    }
}

void Core::OnGUI()
{
#ifdef _DEBUG
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

        if (ImGui::BeginMenu("GameObject"))
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                SCENE_MANAGER->GetCurrentScene()->AddEmptyObject();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Inspector
    ImGui::Begin("Inspector");

    if (object_ != nullptr)
    {
        ImGui::InputText("Name", object_->name.data(), 2048);

        if (ImGui::CollapsingHeader("Transform"))
        {
            const auto transform = object_->GetTransform();

            float* position[2] = {
                &transform->position.x,
                &transform->position.y
            };

            float* scale[2] = {
                &transform->scale.x,
                &transform->scale.y
            };
            
            ImGui::InputFloat2("Position", *position);
            ImGui::InputFloat("Z Rotation", &transform->z_rotation);
            ImGui::InputFloat2("Scale", *scale);
        }
    }

    ImGui::End();

    // Hierarchy
    ImGui::Begin("Hierarchy");

    auto* current_scene = SCENE_MANAGER->GetCurrentScene();

    const auto& objects = current_scene->GetObjects(static_cast<Layer>(0));
    
    ImGuiListClipper hierarchy_clipper;
    hierarchy_clipper.Begin(objects.size());

    while (hierarchy_clipper.Step())
    {
        for (size_t j = hierarchy_clipper.DisplayStart; j < hierarchy_clipper.DisplayEnd; j++)
        {
            ImGui::TreeNodeEx(
                reinterpret_cast<void*>(static_cast<intptr_t>(j)),
                ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen,
                "%s",
                objects[j]->name.c_str()
            );

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                object_ = objects[j].get();
            }
        }
    }

    hierarchy_clipper.End();
    
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.f);
    }

    ImGui::End();

    // Console
    ImGui::Begin("Console");

    if (ImGui::Button("Clear"))
    {
        logs_.clear();
    }

    ImGui::Separator();
    ImGui::BeginChild("Console");

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
    ImGui::End();

    ImGui::Begin("Debug");

    const Vector2 mouse_position = INPUT_MANAGER->GetMousePosition();

    ImGui::Text("FPS : %d", TIME_MANAGER->GetFPS());
    ImGui::Text("X: %.f, Y: %.f", mouse_position.x, mouse_position.y);

    ImGui::End();
#else
    SCENE_MANAGER->OnGUI();

#endif
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

    HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (FAILED(result))
    {
        return false;
    }

    if (!GRAPHICS->Initaite())
    {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    static_cast<void>(io);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF(".\\Engine/Fonts\\NanumBarunGothic.ttf", 14.f, nullptr,
                                 io.Fonts->GetGlyphRangesKorean());
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

    EVENT_MANAGER->Update();
    AUDIO_MANAGER->Update();
}

void Core::Log(std::wstring format, ...)
{
    va_list args;
    va_start(args, format);

    WCHAR buffer[1024];
    vswprintf_s(buffer, format.c_str(), args);
    wcscat_s(buffer, L"\n");

    std::string str;
    str.assign(buffer, buffer + wcslen(buffer));

    OutputDebugStringW(buffer);
    logs_.push_back(str);

    va_end(args);
}
