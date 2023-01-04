#include "pch.h"
#include "Graphics.h"
#include "Core.h"

bool Graphics::Initaite()
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc.Width = 0;
    swap_chain_desc.BufferDesc.Height = 0;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = CORE->GetHWND();
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        &feature_levels,
        1,
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &swap_chain_,
        &device_,
        NULL,
        &device_context_
    );

    if (FAILED(result))
    {
        return false;
    }

    result = swap_chain_->ResizeBuffers(
        0,
        CORE->GetResolution().x,
        CORE->GetResolution().y,
        DXGI_FORMAT_UNKNOWN,
        0
    );

    if (FAILED(result))
    {
        return false;
    }

    ID3D11Texture2D* back_buffer;
    result = swap_chain_->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&back_buffer)
    );

    if (FAILED(result))
    {
        return false;
    }

    result = device_->CreateRenderTargetView(
        back_buffer,
        NULL,
        &render_target_view_
    );

    if (FAILED(result))
    {
        return false;
    }

    viewport_.TopLeftX = 0.f;
    viewport_.TopLeftY = 0.f;
    viewport_.Width = static_cast<float>(CORE->GetResolution().x);
    viewport_.Height = static_cast<float>(CORE->GetResolution().y);
    viewport_.MinDepth = 0.f;
    viewport_.MaxDepth = 1.f;

    back_buffer->Release();

    return true;
}

void Graphics::BeginRender()
{
    device_context_->OMSetRenderTargets(1, &render_target_view_, NULL);
    device_context_->RSSetViewports(1, &viewport_);
    device_context_->ClearRenderTargetView(render_target_view_, &clear_color_[0]);
}

void Graphics::EndRender()
{
    swap_chain_->Present(0, 0);
}

Graphics::~Graphics()
{
    render_target_view_->Release();
    swap_chain_->Release();
    device_->Release();
    device_context_->Release();
}
