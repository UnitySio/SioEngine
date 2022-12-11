#pragma once
#include <coroutine>

class Coroutine
{
public:
    struct promise_type
    {
        Coroutine get_return_object()
        {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend()
        {
            return {};
        }

        std::suspend_always final_suspend()
        {
            return {};
        }

        void return_void()
        {
        }

        void unhandled_exception()
        {
        }
    };

    std::coroutine_handle<promise_type> handler;

    Coroutine(std::coroutine_handle<promise_type> handle);
    ~Coroutine();
};
