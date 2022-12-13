#pragma once
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

    void AddSound(std::string, const char* path, bool is_loop);
    void Play(std::string name);
};
