#pragma once
#include "Singleton.h"

class Core :
    public Singleton<Core>
{
private:
    LPCWSTR kClassName;
    LPCWSTR kWindowName;

    HWND hWnd;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
};
