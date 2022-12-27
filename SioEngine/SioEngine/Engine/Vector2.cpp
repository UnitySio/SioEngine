#include "pch.h"
#include "Vector2.h"

Vector2::Vector2() :
    x(),
    y()
{
}

Vector2::Vector2(float x, float y) :
    x(x),
    y(y)
{
}

Vector2& Vector2::operator+=(const Vector2& kVector2)
{
    x += kVector2.x;
    y += kVector2.y;

    return *this;
}

Vector2& Vector2::operator-=(const Vector2& kVector2)
{
    x -= kVector2.x;
    y -= kVector2.y;

    return *this;
}

Vector2& Vector2::operator*=(const Vector2& kVector2)
{
    x *= kVector2.x;
    y *= kVector2.y;

    return *this;
}

Vector2& Vector2::operator/=(const Vector2& kVector2)
{
    x /= kVector2.x;
    y /= kVector2.y;

    return *this;
}

Vector2& Vector2::operator=(const Vector2& kVector2)
{
    x = kVector2.x;
    y = kVector2.y;

    return *this;
}

Vector2 Vector2::operator+(const Vector2& kVector2)
{
    return {x + kVector2.x, y + kVector2.y};
}

Vector2 Vector2::operator-(const Vector2& kVector2)
{
    return {x - kVector2.x, y - kVector2.y};
}

Vector2 Vector2::operator*(const Vector2& kVector2)
{
    return {x * kVector2.x, y * kVector2.y};
}

Vector2 Vector2::operator/(const Vector2& kVector2)
{
    return {x / kVector2.x, y / kVector2.y};
}

Vector2 Vector2::operator+(float val)
{
    return {x + val, y + val};
}

Vector2 Vector2::operator-(float val)
{
    return {x - val, y - val};
}

Vector2 Vector2::operator*(float val)
{
    return {x * val, y * val};
}

Vector2 Vector2::operator/(float val)
{
    return {x / val, y / val};
}

Vector2 Vector2::Zero()
{
    return {0.f, 0.f};
}

Vector2 Vector2::One()
{
    return {1.f, 1.f};
}

Vector2 Vector2::Left()
{
    return {-1.f, 0.f};
}

Vector2 Vector2::Up()
{
    return {0.f, -1.f};
}

Vector2 Vector2::Right()
{
    return {1.f, 0.f};
}

Vector2 Vector2::Down()
{
    return {0.f, 1.f};
}

Vector2 Vector2::Normalized()
{
    float m = Magnitude();

    if (m > 0)
    {
        return {x / m, y / m};
    }

    return Vector2().Zero();
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
    t = std::clamp(t, 0.f, 1.f);
    return a + (b - a) * t;
}

bool Vector2::operator==(const Vector2& kVector2) const
{
    if (fabsf(x - kVector2.x) <= FLT_EPSILON &&
        fabsf(y - kVector2.y) <= FLT_EPSILON)
    {
        return true;
    }
    
    return false;
}

bool Vector2::operator!=(const Vector2& kVector2) const
{
    if (fabsf(x - kVector2.x) > FLT_EPSILON ||
        fabsf(y - kVector2.y) > FLT_EPSILON)
    {
        return true;
    }
    
    return false;
}

bool Vector2::operator==(float val) const
{
    if (fabsf(x - val) <= FLT_EPSILON &&
        fabsf(y - val) <= FLT_EPSILON)
    {
        return true;
    }
    
    return false;
}

bool Vector2::operator!=(float val) const
{
    if (fabsf(x - val) > FLT_EPSILON ||
        fabsf(y - val) > FLT_EPSILON)
    {
        return true;
    }
    
    return false;
}

float Vector2::Magnitude()
{
    auto temp_x = static_cast<double>(x);
    auto temp_y = static_cast<double>(y);
    
    return static_cast<float>(sqrt(pow(temp_x, 2) + pow(temp_y, 2)));
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
    Vector2 diff = a - b;
    return diff.Magnitude();
}

void Vector2::Normalize()
{
    float m = Magnitude();

    if (m > 0)
    {
        x /= m;
        y /= m;
    }
    else
    {
        x = 0.f;
        y = 0.f;
    }
}
