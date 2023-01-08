#include "pch.h"
#include "Graphics.h"
#include "Core.h"

bool Graphics::Initaite()
{
    return CreateDeviceD3D() && CreateRenderTargetD3D() && CreateFactoryD2D() && CreateRenderTargetD2D();
}

bool Graphics::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc.Width = CORE->GetResolution().x;
    swap_chain_desc.BufferDesc.Height = CORE->GetResolution().y;
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
    swap_chain_desc.Windowed = true;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    const D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        &feature_levels,
        1,
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &dxgi_swap_chain_,
        &d3d_device_,
        nullptr,
        &d3d_device_context_
    );

    return SUCCEEDED(result);
}

bool Graphics::CreateRenderTargetD3D()
{
    ID3D11Texture2D* back_buffer;
    HRESULT result = dxgi_swap_chain_->GetBuffer(
        0,
        IID_PPV_ARGS(&back_buffer)
    );

    if (FAILED(result))
    {
        return false;
    }

    result = d3d_device_->CreateRenderTargetView(
        back_buffer,
        nullptr,
        &d3d_render_target_view_
    );

    back_buffer->Release();

    // 보류 (확인 후 함수로 분리할 예정)
    d3d_viewport_.TopLeftX = 0.f;
    d3d_viewport_.TopLeftY = 0.f;
    d3d_viewport_.Width = static_cast<float>(CORE->GetResolution().x);
    d3d_viewport_.Height = static_cast<float>(CORE->GetResolution().y);
    d3d_viewport_.MinDepth = 0.f;
    d3d_viewport_.MaxDepth = 1.f;

    return SUCCEEDED(result);
}

bool Graphics::CreateFactoryD2D()
{
    HRESULT result = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &d2d_factory_
    );

    return SUCCEEDED(result);
}

bool Graphics::CreateRenderTargetD2D()
{
    const float dpi = GetDpiForWindow(CORE->GetHWND());

    const D2D1_RENDER_TARGET_PROPERTIES render_target_properties = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(
            DXGI_FORMAT_UNKNOWN,
            D2D1_ALPHA_MODE_PREMULTIPLIED
        ),
        dpi,
        dpi
    );

    IDXGISurface* back_buffer;
    HRESULT result = dxgi_swap_chain_->GetBuffer(
        0,
        IID_PPV_ARGS(&back_buffer)
    );

    if (FAILED(result))
    {
        return false;
    }

    result = d2d_factory_->CreateDxgiSurfaceRenderTarget(
        back_buffer,
        &render_target_properties,
        &d2d_render_target_
    );

    back_buffer->Release();

    return SUCCEEDED(result);
}

void Graphics::BeginRenderD3D()
{
    const float color_rgba[4] = {
        0.f,
        0.f,
        0.f,
        1.f
    };

    d3d_device_context_->OMSetRenderTargets(1, &d3d_render_target_view_, nullptr);
    d3d_device_context_->RSSetViewports(1, &d3d_viewport_);
    d3d_device_context_->ClearRenderTargetView(d3d_render_target_view_, color_rgba);
}

void Graphics::EndRenderD3D()
{
    dxgi_swap_chain_->Present(0, 0);
}

void Graphics::BeginRenderD2D()
{
    d2d_render_target_->BeginDraw();
}

void Graphics::EndRenderD2D()
{
    d2d_render_target_->EndDraw();
}

Graphics::~Graphics()
{
    d2d_render_target_->Release();
    d2d_factory_->Release();
    d3d_render_target_view_->Release();
    d3d_device_context_->Release();
    d3d_device_->Release();
}

void Graphics::FillRectangle(Rect position, Color color, float z_rotation)
{
    z_rotation = std::clamp(z_rotation, 0.f, 360.f);
    
    const D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    D2D1_POINT_2F center = D2D1::Point2F(
        position.x + position.width / 2.f,
        position.y + position.height / 2.f
    );
    
    d2d_render_target_->SetTransform(D2D1::Matrix3x2F::Rotation(z_rotation, center));

    d2d_render_target_->FillRectangle(&rect, brush);

    brush->Release();
}

void Graphics::DrawRectangle(Rect position, Color color, float stroke)
{
    const D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->DrawRectangle(&rect, brush, stroke);

    brush->Release();
}

void Graphics::FillRoundedRectangle(Rect position, Color color, float radius)
{
    const D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    const D2D1_ROUNDED_RECT rounded = D2D1::RoundedRect(rect, radius, radius);

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->FillRoundedRectangle(&rounded, brush);

    brush->Release();
}

void Graphics::DrawRoundedRectangle(Rect position, Color color, float radius, float stroke)
{
    const D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    const D2D1_ROUNDED_RECT rounded = D2D1::RoundedRect(rect, radius, radius);

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->DrawRoundedRectangle(&rounded, brush, stroke);

    brush->Release();
}

void Graphics::FillEllipse(Rect position, Color color)
{
    const D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(position.x, position.y),
        position.width, position.height
    );

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->FillEllipse(&ellipse, brush);

    brush->Release();
}

void Graphics::DrawEllipse(Rect position, Color color, float stroke)
{
    const D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(position.x, position.y),
        position.width, position.height
    );

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->DrawEllipse(&ellipse, brush, stroke);

    brush->Release();
}

void Graphics::DrawLine(Vector2 a, Vector2 b, Color color, float stroke)
{
    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->DrawLine(
        D2D1::Point2F(a.x, a.y),
        D2D1::Point2F(b.x, b.y),
        brush,
        stroke
    );

    brush->Release();
}

void Graphics::DrawTextW(Rect position, Color color, std::wstring text, float font_size, DWRITE_TEXT_ALIGNMENT h_align,
                         DWRITE_PARAGRAPH_ALIGNMENT v_align)
{
    const D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    IDWriteFactory* write_factory;
    HRESULT result = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&write_factory)
    );

    if (FAILED(result))
    {
        return;
    }

    IDWriteTextFormat* write_text_format;
    result = write_factory->CreateTextFormat(
        L"Arial",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        font_size,
        L"en-us",
        &write_text_format
    );

    if (FAILED(result))
    {
        return;
    }

    write_text_format->SetTextAlignment(h_align);
    write_text_format->SetParagraphAlignment(v_align);

    const auto red = static_cast<float>(color.r) / 255.f;
    const auto green = static_cast<float>(color.g) / 255.f;
    const auto blue = static_cast<float>(color.b) / 255.f;
    const auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    d2d_render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    d2d_render_target_->DrawTextW(
        text.c_str(),
        static_cast<UINT32>(text.size()),
        write_text_format,
        rect,
        brush
    );

    brush->Release();
    write_text_format->Release();
    write_factory->Release();
}
