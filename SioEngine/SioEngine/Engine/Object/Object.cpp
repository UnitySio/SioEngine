#include "pch.h"
#include "Object.h"
#include "Component/Transform.h"

Object::Object() :
	name()
{
}

Object::Object(const Object& kOrigin) :
	name(kOrigin.name)
{
	if (kOrigin.transform_ != nullptr)
	{
		transform_ = std::make_shared<Transform>(*(Transform*)kOrigin.transform_.get());
		transform_->owner_ = this;
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
	transform_->owner_ = this;
}

std::shared_ptr<Transform> Object::GetTransform()
{
	return transform_;
}
