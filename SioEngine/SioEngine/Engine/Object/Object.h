#pragma once

class Object :
    public std::enable_shared_from_this<Object>
{
    Vector2 position_;
    Vector2 scale_;
    float z_rotation_;
public:
    Object();
    Object(const Object& kOrigin);
    virtual ~Object() = default;

    virtual void Awake();
    virtual void Start();
    virtual void FixedUpdate();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();
};
