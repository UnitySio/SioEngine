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
    factory_->Release();
    CoUninitialize();
}

void Graphics::Clear(int r, int g, int b)
{
    render_target_->Clear({r / 255.f, g / 255.f, b / 255.f});
}

void Graphics::FillEllipse(float x, float y, float width, float height)
{
    D2D1_ELLIPSE ellipse = {};

    ellipse.point.x = x;
    ellipse.point.y = y;
    ellipse.radiusX = width;
    ellipse.radiusY = height;

    ID2D1SolidColorBrush* brush = NULL;

    render_target_->CreateSolidColorBrush(ColorF(1.f, 1.f, 0.f), &brush);
    render_target_->FillEllipse(ellipse, brush);

    brush->Release();
}
