#include "pch.h"
#include "Object.h"
#include "Component/Transform.h"

Object::Object()
{
    AddTransform();
}

Object::Object(const Object& kOrigin) :
    name(kOrigin.name)
{
    if (kOrigin.transform_ != nullptr)
    {
        transform_ = std::make_shared<Transform>(*reinterpret_cast<Transform*>(kOrigin.transform_.get()));
        transform_->SetOwner(this);
    }
}

void Object::FixedUpdate()
{
}

void Object::Update()
{
}

void Object::LateUpdate()
{
}

void Object::Render()
{
}

void Object::OnGUI()
{
}

void Object::AddTransform()
{
    transform_ = std::make_shared<Transform>();
    transform_->SetOwner(this);
}

std::shared_ptr<Transform> Object::GetTransform()
{
    return transform_;
}
