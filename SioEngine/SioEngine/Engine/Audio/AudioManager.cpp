#include "pch.h"
#include "AudioManager.h"

void AudioManager::Release()
{
    FMOD_System_Close(fmod_system_);
    FMOD_System_Release(fmod_system_);

    Singleton<AudioManager>::Release();
}

void AudioManager::Update()
{
    FMOD_System_Update(fmod_system_);
}

bool AudioManager::Initiate()
{
    FMOD_RESULT result = FMOD_System_Create(&fmod_system_, FMOD_VERSION);
    if (result != FMOD_OK)
    {
        return false;
    }

    result = FMOD_System_Init(fmod_system_, 32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        return false;
    }

    return true;
}

AudioManager::AudioManager() :
    fmod_system_()
{
}
