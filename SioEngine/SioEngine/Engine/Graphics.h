#pragma once
#pragma comment(lib, "d3d11")

#include "Singleton.h"

#include <d3d11.h>

class Graphics :
    public Singleton<Graphics>
{
    friend class Core;

    ID3D11Device* device_;
    ID3D11DeviceContext* device_context_;
    ID3D11RenderTargetView* render_target_view_;
    
    IDXGISwapChain* swap_chain_;

    D3D11_VIEWPORT viewport_;

    float clear_color_[4] = { 1.f, 1.f, 1.f, 1.f };

    bool Initaite();
    
    void BeginRender();
    void EndRender();

public:
    Graphics() = default;
    ~Graphics() final;
};
