#pragma once
#include "Singleton.h"

class Core :
    public Singleton<Core>
{
    LPCWSTR kClassName;
    LPCWSTR kWindowName;

    HWND hWnd;
    HWND focus_;

    POINT resolution_;

    RECT window_area_;

    HANDLE logic_handle_;

    bool is_logic_loop_;

    float timer_;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static DWORD WINAPI LogicThread(LPVOID lpParam);

    void FixedUpdate();
    void Update();
    void LateUpdate();
    void Render();

public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    HWND GetHWND();
    HWND GetHWNDFocus();

    POINT GetResolution();
    
    void MainLogic();
};
