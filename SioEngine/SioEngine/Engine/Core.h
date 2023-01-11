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
    float z_rotation_;
    float opacity_;
    float font_size_;

    Vector2 position_;
    Vector2 scale_;
    Vector2 temp_;

    std::vector<std::string> logs_;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static DWORD WINAPI LogicThread(LPVOID lpParam);

    void FixedUpdate();
    void Update();
    void LateUpdate();
    void Render();
    void OnGUI();

public:
    Core();
    ~Core() final = default;

    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

    bool InitiateWindow(HINSTANCE hInstance, int nCmdShow);

    HWND GetHWND();
    HWND GetHWNDFocus();

    POINT GetResolution();

    void MainLogic();

    /**
     * \brief 디버거 로그 메시지를 출력합니다.
     * \param format 문자열
     * \param ... 인수
     */
    void Log(std::wstring format, ...);
};
