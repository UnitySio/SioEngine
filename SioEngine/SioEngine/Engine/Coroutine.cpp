#include "pch.h"
#include "Coroutine.h"

Coroutine::Coroutine(std::coroutine_handle<promise_type> handle) :
    handler(handle)
{
}

Coroutine::~Coroutine()
{
    handler.destroy();
}
