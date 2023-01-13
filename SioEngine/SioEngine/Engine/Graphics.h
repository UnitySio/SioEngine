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
     * \brief ȭ�鿡 ���� �簢���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param z_rotation ����
     */
    void FillRectangle(Rect position, Color color, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 �簢���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param stroke �β�
     * \param z_rotation ����
     */
    void DrawRectangle(Rect position, Color color, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 �����ڸ��� �ձ� ���� �簢���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param radius ���
     * \param z_rotation ����
     */
    void FillRoundedRectangle(Rect position, Color color, float radius = 0.f, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 �����ڸ��� �ձ� �簢���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param radius ���
     * \param stroke �β�
     * \param z_rotation ����
     */
    void DrawRoundedRectangle(Rect position, Color color, float radius = 0.f, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 ���� ���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param z_rotation ����
     */
    void FillEllipse(Rect position, Color color, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 ���� �׸��ϴ�.
     * \param position ��ġ
     * \param color ����
     * \param stroke �β�
     * \param z_rotation ����
     */
    void DrawEllipse(Rect position, Color color, float stroke = 1.f, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 a���� b���� ���� �׸��ϴ�.
     * \param a ���� ����
     * \param b �� ����
     * \param color ����
     * \param stroke �β�
     */
    void DrawLine(Vector2 a, Vector2 b, Color color, float stroke = 1.f);

    /**
     * \brief ȭ�鿡 �ؽ�Ʈ�� ǥ���մϴ�.
     * \param position ��ġ
     * \param color ����
     * \param text �ؽ�Ʈ
     * \param font_size ��Ʈ ũ��
     * \param h_align ���� ����
     * \param v_align ���� ����
     * \param z_rotation ����
     */
    void DrawTextW(Rect position, Color color, std::wstring text, float font_size = 12.f,
        DWRITE_TEXT_ALIGNMENT h_align = DTA_LEFT, DWRITE_PARAGRAPH_ALIGNMENT v_align = DTA_TOP, float z_rotation = 0.f);

    /**
     * \brief ȭ�鿡 ��Ʈ���� �׸��ϴ�.
     * \param bitmap ��Ʈ��
     * \param position ��ġ
     * \param opacity ����
     * \param z_rotation ����
     */
    void DrawBitmap(ID2D1Bitmap* bitmap, Rect position, float opacity = 1.f, float z_rotation = 0.f);
};
