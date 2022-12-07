#pragma once
#include "Singleton.h"

class Time :
    public Singleton<Time>
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
    Time();
    ~Time() final = default;

    UINT GetFPS();
};
