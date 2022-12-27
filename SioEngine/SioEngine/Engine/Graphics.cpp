#include "pch.h"
#include "Graphics.h"
#include "Core.h"

bool Graphics::Initiate()
{
    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);

    if (FAILED(result))
    {
        return false;
    }

    RECT rect = {};
    GetClientRect(Core::GetInstance()->GetHWND(), &rect);

    result = factory_->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            CORE->GetHWND(),
            D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
        &render_target_
    );

    if (FAILED(result))
    {
        return false;
    }

    render_target_->CreateCompatibleRenderTarget(
        D2D1::SizeF(0.f, 0.f),
        D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top),
        &bitmap_render_target_
    );
    
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void Graphics::Resize(int width, int height)
{
    if (render_target_ != NULL)
    {
        render_target_->Resize(D2D1::SizeU(width, height));
    }
}

void Graphics::BeginDraw()
{
    render_target_->BeginDraw();
}

void Graphics::EndDraw()
{
    render_target_->EndDraw();
}

Graphics::Graphics() :
    factory_(),
    render_target_()
{
}

Graphics::~Graphics()
{
    render_target_->Release();
    factory_->Release();
}

void Graphics::ClearScreen(Color color)
{
    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    render_target_->Clear(D2D1::ColorF(
        red,
        green,
        blue,
        alpha
    ));
}

void Graphics::FillRectangle(Rect position, Color color)
{
    D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->FillRectangle(&rect, brush);

    brush->Release();
}

void Graphics::DrawRectangle(Rect position, Color color, float stroke)
{
    D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->DrawRectangle(&rect, brush, stroke);

    brush->Release();
}

void Graphics::FillRoundedRectangle(Rect position, Color color, float radius)
{
    D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    D2D1_ROUNDED_RECT rounded = D2D1::RoundedRect(rect, radius, radius);

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->FillRoundedRectangle(&rounded, brush);

    brush->Release();
}

void Graphics::DrawRoundedRectangle(Rect position, Color color, float radius, float stroke)
{
    D2D1_RECT_F rect = D2D1::RectF(
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    );

    D2D1_ROUNDED_RECT rounded = D2D1::RoundedRect(rect, radius, radius);

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->DrawRoundedRectangle(&rounded, brush, stroke);

    brush->Release();
}

void Graphics::FillEllipse(Rect position, Color color)
{
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(position.x, position.y),
        position.width, position.height
    );

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->FillEllipse(&ellipse, brush);

    brush->Release();
}

void Graphics::DrawEllipse(Rect position, Color color, float stroke)
{
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(position.x, position.y),
        position.width, position.height
    );

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->DrawEllipse(&ellipse, brush, stroke);

    brush->Release();
}

void Graphics::DrawLine(Vector2 a, Vector2 b, Color color, float stroke)
{
    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->DrawLine(
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
    D2D1_RECT_F rect = D2D1::RectF(
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
        NULL,
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

    auto red = static_cast<float>(color.r) / 255.f;
    auto green = static_cast<float>(color.g) / 255.f;
    auto blue = static_cast<float>(color.b) / 255.f;
    auto alpha = static_cast<float>(color.a) / 255.f;

    ID2D1SolidColorBrush* brush;
    render_target_->CreateSolidColorBrush(
        D2D1::ColorF(
            red,
            green,
            blue,
            alpha
        ),
        &brush
    );

    render_target_->DrawTextW(
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
