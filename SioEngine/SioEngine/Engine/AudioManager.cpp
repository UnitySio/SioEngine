#include "pch.h"
#include "AudioManager.h"

void AudioManager::Release()
{
    for (auto iter = sounds_.begin(); iter != sounds_.end(); ++iter)
    {
        FMOD_Sound_Release(iter->second);
    }

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

    result = FMOD_System_Init(fmod_system_, CHANNEL_COUNT, FMOD_INIT_NORMAL, nullptr);

    return result == FMOD_OK;
}

AudioManager::AudioManager() :
    fmod_system_(),
    channels_{}
{
}

void AudioManager::AddSound(std::string name, std::string path, bool is_loop)
{
    FMOD_SOUND* sound = nullptr;
    FMOD_RESULT result;

    if (is_loop)
    {
        result = FMOD_System_CreateSound(fmod_system_, path.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
    }
    else
    {
        result = FMOD_System_CreateSound(fmod_system_, path.c_str(), FMOD_DEFAULT, 0, &sound);
    }

    if (result != FMOD_OK)
    {
        return;
    }

    sounds_.insert({ name, sound });
}

void AudioManager::Play(std::string name)
{
    int idx = 0;

    for (FMOD_CHANNEL* i : channels_)
    {
        FMOD_BOOL is_play = false;
        FMOD_Channel_IsPlaying(i, &is_play);

        if (!is_play)
        {
            FMOD_System_PlaySound(fmod_system_, sounds_[name], nullptr, false, &channels_[idx]);
            break;
        }

        idx = (idx + 1) % CHANNEL_COUNT;
    }
}

void AudioManager::Pause()
{
    for (FMOD_CHANNEL* i : channels_)
    {
        if (i == nullptr)
        {
            break;
        }

        FMOD_Channel_SetPaused(i, true);
    }
}

void AudioManager::Resume()
{
    for (FMOD_CHANNEL* i : channels_)
    {
        if (i == nullptr)
        {
            break;
        }

        FMOD_Channel_SetPaused(i, false);
    }
}

void AudioManager::Stop()
{
    for (FMOD_CHANNEL* i : channels_)
    {
        if (i == nullptr)
        {
            break;
        }

        FMOD_Channel_Stop(i);
    }
}

void AudioManager::SetVolume(int volume)
{
    volume = std::clamp(volume, 0, 100);

    const auto f = static_cast<float>(volume) / 100.f;

    for (FMOD_CHANNEL* i : channels_)
    {
        if (i == nullptr)
        {
            break;
        }

        FMOD_Channel_SetVolume(i, f);
    }
}
