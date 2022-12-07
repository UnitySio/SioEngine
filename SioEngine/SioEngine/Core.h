#pragma once
#include "Singleton.h"

class Core :
    public Singleton<Core>
{
    LPCWSTR kClassName;
    LPCWSTR kWindowName;

    HWND hWnd;

    POINT resolution_;

    RECT window_area_;

    HANDLE handles_[2];

    bool is_logic_loop_;
    bool is_fixed_loop_;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    static DWORD WINAPI LogicThread(LPVOID lpParam);
    static DWORD WINAPI FixedThread(LPVOID lpParam);

    void Update();
    void LateUpdate();
    void FixedUpdate();
    void Render();
public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    HWND GetHWND();

    void Logic();
};
