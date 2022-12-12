#pragma once
#include "Singleton.h"
#include "fmod.hpp"

class AudioManager :
    public Singleton<AudioManager>
{
    friend class Core;
    friend class Audio;

    FMOD_SYSTEM* fmod_system_;

    void Release() final;

    void Update();

    bool Initiate();

public:
    AudioManager();
    ~AudioManager() final = default;
};
