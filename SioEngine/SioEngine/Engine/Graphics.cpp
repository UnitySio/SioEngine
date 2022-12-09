#include "pch.h"
#include "Graphics.h"
#include "Core.h"

using namespace D2D1;

void Graphics::Initiate()
{
    HRESULT hr;

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);

    RECT rc = {};
    GetClientRect(Core::GetInstance()->GetHWND(), &rc);

    if (SUCCEEDED(hr))
    {
        hr = factory_->CreateHwndRenderTarget(
            RenderTargetProperties(),
            HwndRenderTargetProperties(Core::GetInstance()->GetHWND(), SizeU(rc.right, rc.bottom)),
            &render_target_
        );
    }

    if (SUCCEEDED(hr))
    {
        render_target_->CreateSolidColorBrush(text_color_, &text_brush_);

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&write_factory_)
        );
    }

    if (SUCCEEDED(hr))
    {
        write_factory_->CreateTextFormat(
            L"Arial",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            12.f,
            L"en-us",
            &write_text_format_
        );
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
    render_target_(),
    write_factory_(),
    write_text_format_(),
    text_brush_(),
    text_color_(1.f, 1.f, 1.f)
{
}

Graphics::~Graphics()
{
    write_text_format_->Release();
    write_factory_->Release();
    text_brush_->Release();
    render_target_->Release();
    factory_->Release();
    
    CoUninitialize();
}

void Graphics::Clear(int r, int g, int b)
{
    render_target_->Clear(ColorF(r / 255.f, g / 255.f, b / 255.f));
}

void Graphics::Label(FRect position, LPCWSTR kText)
{
    D2D1_RECT_F rect = {};

    rect.left = position.left;
    rect.top = position.top;
    rect.right = position.right;
    rect.bottom = position.bottom;

    render_target_->DrawText(
        kText,
        wcslen(kText),
        write_text_format_,
        rect,
        text_brush_
    );
}

void Graphics::SetTextAlign(DWRITE_TEXT_ALIGNMENT alignment)
{
    write_text_format_->SetTextAlignment(alignment);
}

void Graphics::SetParAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
    write_text_format_->SetParagraphAlignment(alignment);
}

void Graphics::SetTextColor(int r, int g, int b, int a)
{
    text_color_ = ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

    text_brush_->Release();
    render_target_->CreateSolidColorBrush(text_color_, &text_brush_);
}
