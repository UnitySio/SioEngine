#pragma once
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "Singleton.h"

#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

class Graphics :
    public Singleton<Graphics>
{
    friend class Core;

    ID3D11Device* d3d_device_;
    ID3D11DeviceContext* d3d_device_context_;
    ID3D11RenderTargetView* d3d_render_target_view_;

    D3D11_VIEWPORT d3d_viewport_;

    IDXGISwapChain* dxgi_swap_chain_;

    ID2D1Factory* d2d_factory_;
    ID2D1RenderTarget* d2d_render_target_;

    bool Initaite();
    bool CreateDeviceD3D();
    bool CreateRenderTargetD3D();
    bool CreateFactoryD2D();
    bool CreateRenderTargetD2D();

    void BeginRenderD3D();
    void EndRenderD3D();
    void BeginRenderD2D();
    void EndRenderD2D();

public:
    Graphics() = default;
    ~Graphics() final;

    ID2D1Bitmap* LoadImageW(std::wstring file_name);

    /**
     * \brief 화면에 꽉찬 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param z_rotation 각도
     */
    void FillRectangle(Rect position, Color color, float z_rotation = 0.f);

    /**
     * \brief 화면에 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param stroke 두께
     * \param z_rotation 각도
     */
    void DrawRectangle(Rect position, Color color, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief 화면에 가장자리가 둥근 꽉찬 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param radius 곡률
     * \param z_rotation 각도
     */
    void FillRoundedRectangle(Rect position, Color color, float radius = 0.f, float z_rotation = 0.f);

    /**
     * \brief 화면에 가장자리가 둥근 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param radius 곡률
     * \param stroke 두께
     * \param z_rotation 각도
     */
    void DrawRoundedRectangle(Rect position, Color color, float radius = 0.f, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief 화면에 꽉찬 원을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param z_rotation 각도
     */
    void FillEllipse(Rect position, Color color, float z_rotation = 0.f);

    /**
     * \brief 화면에 원을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param stroke 두께
     * \param z_rotation 각도
     */
    void DrawEllipse(Rect position, Color color, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief 화면에 a에서 b까지 선을 그립니다.
     * \param a 시작 지점
     * \param b 끝 지점
     * \param color 색상
     * \param stroke 두께
     */
    void DrawLine(Vector2 a, Vector2 b, Color color, float stroke = 1.f);

    /**
     * \brief 화면에 텍스트를 표시합니다.
     * \param position 위치
     * \param color 색상
     * \param text 텍스트
     * \param font_size 폰트 크기
     * \param h_align 가로 정렬
     * \param v_align 세로 정렬
     * \param z_rotation 각도
     */
    void DrawTextW(Rect position, Color color, std::wstring text, float font_size = 12.f,
        DWRITE_TEXT_ALIGNMENT h_align = DTA_LEFT, DWRITE_PARAGRAPH_ALIGNMENT v_align = DTA_TOP, float z_rotation = 0.f);

    /**
     * \brief 화면에 비트맵을 그립니다.
     * \param bitmap 비트맵
     * \param position 위치
     * \param opacity 투명도
     * \param z_rotation 각도
     */
    void DrawBitmap(ID2D1Bitmap* bitmap, Rect position, float opacity = 1.f, float z_rotation = 0.f);
};
