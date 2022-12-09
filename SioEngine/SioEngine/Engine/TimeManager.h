#pragma once
#include "Singleton.h"

class TimeManager :
    public Singleton<TimeManager>
{
    friend class Core;
    
    LARGE_INTEGER frequency_;
    LARGE_INTEGER previous_count_;
    LARGE_INTEGER current_count_;

    float delta_time_;
    float fixed_time_step_;
    float time_scale_;
    float timer_;

    UINT fps_;
    UINT frame_counter_;
    
    void Initaite();
    void Update();
public:
    TimeManager();
    ~TimeManager() final = default;

    void SetFixedTimeStep(float fixed_time_step);
    void SetTimeScale(float time_scale);

    float GetDeltaTime();
    float GetFixedDeltaTime();

    UINT GetFPS();
};
