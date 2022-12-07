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

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    void Update();
    void LateUpdate();
    void Render();
public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    HWND GetHWND();

    void Logic();
};
