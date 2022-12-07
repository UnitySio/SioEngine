#include "pch.h"
#include "Time.h"

void Time::Initaite()
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&previous_count_);
}

void Time::Update()
{
    QueryPerformanceCounter(&current_count_);

    delta_time_ = (current_count_.QuadPart - previous_count_.QuadPart) / (float)frequency_.QuadPart;

    previous_count_ = current_count_;

    frame_counter_++;

    timer_ += delta_time_;

    if (timer_ >= 1.f)
    {
        fps_ = frame_counter_;
        frame_counter_ = 0;
        timer_ = 0.f;
    }
}

Time::Time() :
    frequency_{},
    previous_count_{},
    current_count_{},
    delta_time_(),
    time_scale_(1.f),
    timer_(),
    fps_(),
    frame_counter_()
{
}

UINT Time::GetFPS()
{
    return fps_;
}
