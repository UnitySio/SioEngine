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

    Vector2 operator+(float val);
    Vector2 operator-(float val);
    Vector2 operator*(float val);
    Vector2 operator/(float val);

    /**
     * \brief Vector2(0.f, 0.f)
     * \return Vector2
     */
    Vector2 Zero();

    /**
     * \brief Vector2(1.f, 1.f)
     * \return Vector2
     */
    Vector2 One();

    /**
     * \brief Vector2(-1.f, 0.f)
     * \return Vector2
     */
    Vector2 Left();

    /**
     * \brief Vector2(0.f, -1.f)
     * \return Vector2
     */
    Vector2 Up();

    /**
     * \brief Vector2(1.f, 0.f)
     * \return Vector2
     */
    Vector2 Right();

    /**
     * \brief Vector2(0.f, 1.f)
     * \return Vector2
     */
    Vector2 Down();

    /**
     * \brief 백터를 정규화된 값으로 반환합니다.
     * \return Vector2
     */
    Vector2 Normalized();

    /**
     * \brief 두 백터를 선형 보간합니다.
     * \param a 첫번째 백터
     * \param b 두번째 백터
     * \param t 비율
     * \return Vector2
     */
    Vector2 Lerp(Vector2 a, Vector2 b, float t);

    ~Vector2() = default;

    bool operator==(const Vector2& kVector2) const;
    bool operator!=(const Vector2& kVector2) const;
    bool operator==(float val) const;
    bool operator!=(float val) const;

    /**
     * \brief 백터의 길이를 반환합니다.
     * \return float
     */
    float Magnitude();

    /**
     * \brief 두 백터의 거리를 반환합니다.
     * \param a 첫번째 백터
     * \param b 두번째 백터
     * \return float
     */
    float Distance(Vector2 a, Vector2 b);

    /**
     * \brief 두 백터의 내적을 반환합니다.
     * \param a 첫번째 백터
     * \param b 두번째 백터
     * \return float
     */
    float Dot(Vector2 a, Vector2 b);

    /**
     * \brief 백터를 정규화를 합니다.
     */
    void Normalize();
};
