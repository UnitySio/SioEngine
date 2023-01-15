#pragma once

class Object;

class Component
{
    Object* owner_;
    
protected:
    Component();
    virtual ~Component() = default;

public:
    Component& operator=(const Component&) = delete;

    void SetOwner(Object* owner);

    Object* GetOwner();
};
