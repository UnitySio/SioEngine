#pragma once

class Vector2
{
public:
    float x;
    float y;

    Vector2();
    Vector2(float x, float y);
    Vector2& operator+=(const Vector2& kVector2);
    Vector2& operator-=(const Vector2& kVector2);
    Vector2& operator*=(const Vector2& kVector2);
    Vector2& operator/=(const Vector2& kVector2);
    Vector2& operator=(const Vector2& kVector2);

    Vector2 operator+(const Vector2& kVector2);
    Vector2 operator-(const Vector2& kVector2);
    Vector2 operator*(const Vector2& kVector2);
    Vector2 operator/(const Vector2& kVector2);

    Vector2 operator+(float kValue);
    Vector2 operator-(float kValue);
    Vector2 operator*(float kValue);
    Vector2 operator/(float kValue);

    /**
     * \brief Vector2(0.f, 0.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 Zero();

    /**
     * \brief Vector2(1.f, 1.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 One();

    /**
     * \brief Vector2(-1.f, 0.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 Left();

    /**
     * \brief Vector2(0.f, -1.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 Up();

    /**
     * \brief Vector2(1.f, 0.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 Right();

    /**
     * \brief Vector2(0.f, 1.f) 작성에 대한 속기
     * \return Vector2
     */
    Vector2 Down();

    ~Vector2() = default;

    bool operator==(const Vector2& kVector2) const;
    bool operator!=(const Vector2& kVector2) const;
};
