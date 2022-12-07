#pragma once
#include "Singleton.h"

class Graphics :
    public Singleton<Graphics>
{
    friend class Core;

    ID2D1Factory* factory_;
    ID2D1HwndRenderTarget* render_target_;

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
     * \brief 원을 그립니다.
     * \param x 원의 X 좌표입니다.
     * \param y 원의 Y 좌표입니다.
     * \param width 원의 가로 크기입니다.
     * \param height 원의 세로 크기입니다.
     */
    void FillEllipse(float x, float y, float width, float height);
};
