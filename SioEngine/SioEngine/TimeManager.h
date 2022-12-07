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
    float time_scale_;
    float timer_;

    UINT fps_;
    UINT frame_counter_;
    
    void Initaite();
    void Update();
public:
    TimeManager();
    ~TimeManager() final = default;

    void SetTimeScale(float time_scale);

    float GetDeltaTime();

    UINT GetFPS();
};
