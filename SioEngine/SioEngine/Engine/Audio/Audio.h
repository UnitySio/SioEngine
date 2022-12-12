#pragma once
#include "fmod.hpp"

class Audio
{
    FMOD_SOUND* sound_;
    FMOD_CHANNEL* channel_;
    FMOD_BOOL is_playing_;

    float volume_;
    
public:
    Audio(const char* path, bool is_loop);
    ~Audio() = default;

    /**
     * \brief 사운드를 재생합니다.
     */
    void Play();

    /**
     * \brief 사운드 재생을 일시정지합니다.
     */
    void Pause();

    /**
     * \brief 사운드 재생을 재개합니다.
     */
    void Resume();

    /**
     * \brief 사운드를 멈춤니다.
     */
    void Stop();

    /**
     * \brief 사운드의 볼륨을 설정합니다.
     */
    void SetVolume(float volume = 1.f);

    /**
     * \brief 현재 사운드가 재생 중인지 확인합니다.
     * \return bool
     */
    bool IsPlaying();
};
