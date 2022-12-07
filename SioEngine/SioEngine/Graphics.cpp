#include "pch.h"
#include "Graphics.h"
#include "Core.h"

using namespace D2D1;

void Graphics::Initiate()
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);

    RECT rc = {};
    GetClientRect(Core::GetInstance()->GetHWND(), &rc);

    factory_->CreateHwndRenderTarget(
        RenderTargetProperties(),
        HwndRenderTargetProperties(Core::GetInstance()->GetHWND(), SizeU(rc.right, rc.bottom)),
        &render_target_
    );

    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&write_factory_)
    );

    write_factory_->CreateTextFormat(
        L"Arial",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        12.f,
        L"ko-kr",
        &write_text_format_
    );

    //write_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    //write_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
    write_text_format_()
{
}

Graphics::~Graphics()
{
    factory_->Release();
    CoUninitialize();
}

void Graphics::Clear(float r, float g, float b)
{
    render_target_->Clear(ColorF(r, g, b));
}

void Graphics::DrawEllipse(float x, float y, float width, float height)
{
    D2D1_ELLIPSE ellipse = {};

    ellipse.point.x = x;
    ellipse.point.y = y;
    ellipse.radiusX = width;
    ellipse.radiusY = height;

    ID2D1SolidColorBrush* brush = NULL;

    render_target_->CreateSolidColorBrush(ColorF(255, 0, 0), &brush);
    render_target_->FillEllipse(ellipse, brush);

    brush->Release();
}

void Graphics::DrawTextW(float x, float y, float width, float height, LPCWSTR text)
{
    D2D1_RECT_F rect = RectF(x, y, width, height);

    ID2D1SolidColorBrush* brush = NULL;

    render_target_->CreateSolidColorBrush(ColorF(255, 0, 0), &brush);

    render_target_->DrawTextW(
        text,
        wcslen(text),
        write_text_format_,
        rect,
        brush
    );
}
