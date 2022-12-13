#include "pch.h"
#include "AudioManager.h"

void AudioManager::Release()
{
    auto iter = sounds_.begin();
    for (; iter != sounds_.end(); ++iter)
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

    result = FMOD_System_Init(fmod_system_, CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        return false;
    }

    AddSound("BGM", "GhostOfMyPast.mp3", true);

    return true;
}

AudioManager::AudioManager() :
    fmod_system_(),
    channels_{}
{
}

void AudioManager::AddSound(std::string name, const char* path, bool is_loop)
{
    FMOD_SOUND* sound = NULL;
    FMOD_RESULT result;

    if (is_loop)
    {
        result = FMOD_System_CreateSound(fmod_system_, path, FMOD_LOOP_NORMAL, 0, &sound);
    }
    else
    {
        result = FMOD_System_CreateSound(fmod_system_, path, FMOD_DEFAULT, 0, &sound);
    }

    if (result != FMOD_OK)
    {
        return;
    }

    sounds_.insert({name, sound});

    Play("BGM");
}

void AudioManager::Play(std::string name)
{
    static int idx = 0;
    
    FMOD_BOOL is_playing = false;
    FMOD_Channel_IsPlaying(channels_[idx], &is_playing);

    if (is_playing)
    {
        idx = (idx + 1) % CHANNEL_COUNT;
    }

    FMOD_System_PlaySound(fmod_system_, sounds_[name], NULL, false, &channels_[idx]);
}
