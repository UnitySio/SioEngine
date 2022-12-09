#include "pch.h"
#include "TimeManager.h"

void TimeManager::Initaite()
{
	QueryPerformanceFrequency(&frequency_);
	QueryPerformanceCounter(&previous_count_);
}

void TimeManager::Update()
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

TimeManager::TimeManager() :
	frequency_{},
	previous_count_{},
	current_count_{},
	delta_time_(),
	fixed_time_step_(.02f),
	time_scale_(1.f),
	timer_(),
	fps_(),
	frame_counter_()
{
}

void TimeManager::SetFixedTimeStep(float fixed_time_step)
{
	fixed_time_step_ = fixed_time_step;
}

void TimeManager::SetTimeScale(float time_scale)
{
	time_scale_ = time_scale;
}

float TimeManager::GetDeltaTime()
{
	return delta_time_ * time_scale_;
}

float TimeManager::GetFixedDeltaTime()
{
	return fixed_time_step_;
}

UINT TimeManager::GetFPS()
{
	return fps_;
}
