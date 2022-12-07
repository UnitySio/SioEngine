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
    void Clear(float r, float g, float b);

    /**
     * \brief 화면에 원을 그립니다.
     * \param x 원이 그려질 X 좌표입니다.
     * \param y 원이 그려질 Y 좌표입니다.
     * \param width 원이 가로 크기입니다.
     * \param height 원의 세로 크기입니다.
     */
    void DrawEllipse(float x, float y, float width, float height);

    /**
     * \brief 화면에 텍스트를 그립니다.
     * \param x 텍스트가 그려질 X 좌표입니다.
     * \param y 텍스트가 그려질 Y 좌표입니다.
     * \param width 텍스트가 그려질 영역의 가로 크기입니다.
     * \param height 텍스트가 그려질 영역의 세로 크기입니다.
     * \param text 그려질 텍스트입니다.
     */
    void DrawText(float x, float y, float width, float height, LPCWSTR text);
};
