#pragma once
#pragma comment(lib, "fmod_vc")

#include "Singleton.h"
#include "fmod.hpp"

#define CHANNEL_COUNT 32

class AudioManager :
    public Singleton<AudioManager>
{
    friend class Core;

    FMOD_SYSTEM* fmod_system_;

    FMOD_CHANNEL* channels_[CHANNEL_COUNT];

    std::map<std::string, FMOD_SOUND*> sounds_;

    void Release() final;

    void Update();

    bool Initiate();

public:
    AudioManager();
    ~AudioManager() final = default;

    /**
     * \brief 사운드를 추가합니다.
     * \param name 이름
     * \param path 경로
     * \param is_loop 반복 여부
     */
    void AddSound(std::string name, std::string path, bool is_loop);

    /**
     * \brief 사운드를 재생합니다.
     * \param name 이름
     */
    void Play(std::string name);

    /**
     * \brief 재생 중인 사운드를 일시 정지합니다.
     */
    void Pause();

    /**
     * \brief 일시 정지된 사운드를 재개합니다.
     */
    void Resume();

    /**
     * \brief 재생 중인 사운드를 정지합니다.
     */
    void Stop();

    /**
     * \brief 전체 사운드의 볼륨을 설정합니다.
     * \param volume 볼륨 크기
     */
    void SetVolume(int volume);
};
