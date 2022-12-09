#pragma once
#include "Singleton.h"

class Graphics :
    public Singleton<Graphics>
{
    friend class Core;

    ID2D1Factory* factory_;
    ID2D1HwndRenderTarget* render_target_;

    IDWriteFactory* write_factory_;
    IDWriteTextFormat* write_text_format_;

    ID2D1SolidColorBrush* text_brush_;

    D2D1::ColorF text_color_;

    /**
     * \brief Direct2D를 사용하기 위한 초기화를 합니다.
     */
    void Initiate();

    /**
     * \brief 그리기를 시작합니다.
     */
    void BeginDraw();

    /**
     * \brief 그리기를 끝냅니다.
     */
    void EndDraw();
public:
    Graphics();
    ~Graphics() final;

    /**
     * \brief 화면을 지정한 색상으로 초기화합니다.
     */
    void Clear(int r, int g, int b);

    /**
     * \brief 화면에 레이블을 표시합니다.
     * \param position 레이블을 표시할 위치입니다.
     * \param kText 표시할 텍스트입니다.
     */
    void Label(FRect position, LPCWSTR kText);

    /**
     * \brief 텍스트를 가로 정렬합니다.
     * \param alignment 가로 정렬 방향
     */
    void SetTextAlign(DWRITE_TEXT_ALIGNMENT alignment);

    /**
     * \brief 텍스트를 세로 정렬합니다.
     * \param alignment 세로 정렬 방향
     */
    void SetParAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment);

    /**
     * \brief 텍스트의 색상을 변경합니다.
     * \param r 현재 색상의 빨간색입니다.
     * \param g 현재 색상의 초록색입니다.
     * \param b 현재 색상의 파란색입니다.
     * \param a 현재 색상의 알파 채널 값입니다.
     */
    void SetTextColor(int r, int g, int b, int a = 255);
};
