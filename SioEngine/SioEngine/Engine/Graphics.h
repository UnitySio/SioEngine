#pragma once
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "Singleton.h"

#include <d2d1.h>
#include <dwrite.h>

class Graphics :
    public Singleton<Graphics>
{
    friend class Core;

    ID2D1Factory* factory_;
    ID2D1HwndRenderTarget* render_target_;
    ID2D1BitmapRenderTarget* bitmap_render_target_;

    /**
     * \brief Direct2D를 사용하기 위한 객체들을 초기화하고 성공 여부를 반환합니다.
     * \return bool
     */
    bool Initiate();

    /**
     * \brief Render Target의 크기를 재조정합니다.
     * \param width 가로 크기
     * \param height 세로 크기
     */
    void Resize(int width, int height);

    /**
     * \brief 화면에 그리기를 시작합니다.
     */
    void BeginDraw();

    /**
     * \brief 화면에 그리기를 끝냅니다.
     */
    void EndDraw();

public:
    Graphics();
    ~Graphics() final;

    /**
     * \brief 화면을 지정한 색상으로 초기화합니다.
     */
    void ClearScreen(Color color);

    /**
     * \brief 화면에 꽉찬 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     */
    void FillRectangle(Rect position, Color color);

    /**
     * \brief 화면에 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param stroke 두께
     */
    void DrawRectangle(Rect position, Color color, float stroke = 1.f);

    /**
     * \brief 화면에 가장자리가 둥근 꽉찬 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param radius 곡률
     */
    void FillRoundedRectangle(Rect position, Color color, float radius = 0.f);

    /**
     * \brief 화면에 가장자리가 둥근 사각형을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param radius 곡률
     * \param stroke 두께
     */
    void DrawRoundedRectangle(Rect position, Color color, float radius = 0.f, float stroke = 1.f);

    /**
     * \brief 화면에 꽉찬 원을 그립니다.
     * \param position 위치
     * \param color 색상
     */
    void FillEllipse(Rect position, Color color);

    /**
     * \brief 화면에 원을 그립니다.
     * \param position 위치
     * \param color 색상
     * \param stroke 두께
     */
    void DrawEllipse(Rect position, Color color, float stroke = 1.f);

    /**
     * \brief 화면에 a에서 b까지 선을 그립니다.
     * \param a 시작 지점
     * \param b 끝 지점
     * \param color 색상
     * \param stroke 두께
     */
    void DrawLine(Vector2 a, Vector2 b, Color color, float stroke = 1.f);

    /**
     * \brief 화면에 텍스트를 표시합니다.
     * \param position 위치
     * \param color 색상
     * \param text 텍스트
     * \param font_size 폰트 크기
     * \param h_align 가로 정렬
     * \param v_align 세로 정렬
     */
    void DrawTextW(Rect position, Color color, std::wstring text, float font_size = 12.f,
                   DWRITE_TEXT_ALIGNMENT h_align = DTA_LEFT, DWRITE_PARAGRAPH_ALIGNMENT v_align = DTA_TOP);
};
