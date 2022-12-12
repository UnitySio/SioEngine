#include "pch.h"
#include "Audio.h"
#include "Audio/AudioManager.h"

Audio::Audio(const char* path, bool is_loop)
{
    if (is_loop)
    {
        FMOD_System_CreateSound(AUDIO_MANAGER->fmod_system_, path, FMOD_LOOP_NORMAL, 0, &sound_);
    }
    else
    {
        FMOD_System_CreateSound(AUDIO_MANAGER->fmod_system_, path, FMOD_DEFAULT, 0, &sound_);
    }
}

void Audio::Play()
{
    FMOD_System_PlaySound(AUDIO_MANAGER->fmod_system_, sound_, NULL, false, &channel_);
}

void Audio::Pause()
{
    FMOD_Channel_SetPaused(channel_, true);
}

void Audio::Resume()
{
    FMOD_Channel_SetPaused(channel_, false);
}

void Audio::Stop()
{
    FMOD_Channel_Stop(channel_);
}

void Audio::SetVolume(float volume)
{
    volume_ = std::clamp(volume_, 0.f, 1.f);
    FMOD_Channel_SetVolume(channel_, volume_);
}

bool Audio::IsPlaying()
{
    FMOD_Channel_IsPlaying(channel_, &is_playing_);
    
    return is_playing_;
}
