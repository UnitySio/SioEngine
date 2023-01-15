#include "pch.h"
#include "Component.h"

Component::Component() :
    owner_()
{
}

void Component::SetOwner(Object* owner)
{
    owner_ = owner;
}

Object* Component::GetOwner()
{
    return owner_;
}
