#pragma once

class Transform
{
public:
    Vector2 position;
    Vector2 scale;

    float z_rotation;
    
    Transform();
    Transform(const Transform& kOrigin);

    Transform& operator=(const Transform& kOrigin) = delete;
    
    ~Transform() = default;

    Vector2 Right();
    Vector2 Up();

    void Translate(Vector2 translation);
};
