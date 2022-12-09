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

Vector2 Vector2::operator+(float kValue)
{
    return {x + kValue, y + kValue};
}

Vector2 Vector2::operator-(float kValue)
{
    return {x - kValue, y - kValue};
}

Vector2 Vector2::operator*(float kValue)
{
    return {x * kValue, y * kValue};
}

Vector2 Vector2::operator/(float kValue)
{
    return {x / kValue, y / kValue};
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

bool Vector2::operator==(const Vector2& kVector2) const
{
    return x == kVector2.x && y == kVector2.y;
}

bool Vector2::operator!=(const Vector2& kVector2) const
{
    return x != kVector2.x || y != kVector2.y;
}
