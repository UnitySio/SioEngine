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

    /**
     * \brief 고정 프레임 업데이트가 수행되는 간격(초)을 설정합니다.
     * \param fixed_time_step 간격(초)
     */
    void SetFixedTimeStep(float fixed_time_step);

    /**
     * \brief 시간이 흐르는 크기를 설정합니다.
     * \param time_scale 시간의 크기
     */
    void SetTimeScale(float time_scale);

    /**
     * \brief 마지막 프레임에서 현재 프레임까지의 간격(초)을 반환합니다.
     * \return float
     */
    float GetDeltaTime();

    /**
     * \brief 고정 프레임 업데이트가 수행되는 간격(초)을 반환합니다.
     * \return float
     */
    float GetFixedDeltaTime();

    /**
     * \brief 현재 FPS를 반환합니다.
     * \return UINT
     */
    UINT GetFPS();
};
