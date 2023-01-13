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
     * \brief ���带 �߰��մϴ�.
     * \param name �̸�
     * \param path ���
     * \param is_loop �ݺ� ����
     */
    void AddSound(std::string name, std::string path, bool is_loop);

    /**
     * \brief ���带 ����մϴ�.
     * \param name �̸�
     */
    void Play(std::string name);

    /**
     * \brief ��� ���� ���带 �Ͻ� �����մϴ�.
     */
    void Pause();

    /**
     * \brief �Ͻ� ������ ���带 �簳�մϴ�.
     */
    void Resume();

    /**
     * \brief ��� ���� ���带 �����մϴ�.
     */
    void Stop();

    /**
     * \brief ��ü ������ ������ �����մϴ�.
     * \param volume ���� ũ��
     */
    void SetVolume(int volume);
};
