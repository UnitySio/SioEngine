#pragma once
#include "Object.h"

class EmptyObject :
    public Object
{
public:
    EmptyObject();
    ~EmptyObject() final = default;
};
