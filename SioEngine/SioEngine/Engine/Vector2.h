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
     * \brief ���͸� ����ȭ�� ������ ��ȯ�մϴ�.
     * \return Vector2
     */
    Vector2 Normalized();

    /**
     * \brief �� ���͸� ���� �����մϴ�.
     * \param a ù��° ����
     * \param b �ι�° ����
     * \param t ����
     * \return Vector2
     */
    Vector2 Lerp(Vector2 a, Vector2 b, float t);

    ~Vector2() = default;

    bool operator==(const Vector2& kVector2) const;
    bool operator!=(const Vector2& kVector2) const;
    bool operator==(float val) const;
    bool operator!=(float val) const;

    /**
     * \brief ������ ���̸� ��ȯ�մϴ�.
     * \return float
     */
    float Magnitude();

    /**
     * \brief �� ������ �Ÿ��� ��ȯ�մϴ�.
     * \param a ù��° ����
     * \param b �ι�° ����
     * \return float
     */
    float Distance(Vector2 a, Vector2 b);

    /**
     * \brief �� ������ ������ ��ȯ�մϴ�.
     * \param a ù��° ����
     * \param b �ι�° ����
     * \return float
     */
    float Dot(Vector2 a, Vector2 b);

    /**
     * \brief ���͸� ����ȭ�� �մϴ�.
     */
    void Normalize();
};
