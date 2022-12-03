#pragma once
#include "Singleton.h"

#define MAX_LOADSTRING 100

class Core :
    public Singleton<Core>
{
private:
    // WinAPI
    HINSTANCE hInst; // 현재 인스턴스
    HWND hWnd;
    
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hinstance);
    BOOL InitInstance(HINSTANCE, int);
};
