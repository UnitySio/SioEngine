#pragma once
#include "Singleton.h"

class InputManager :
    public Singleton<InputManager>
{
    friend class Core;

    enum class KeyType : int
    {
        kNone = 0,
        kDown,
        kHold,
        kUp
    };

    struct KeyState
    {
        KeyType type;
        bool is_down;
    };

    std::map<int, KeyState> keys_;

    void Update();

public:
    InputManager() = default;
    ~InputManager() final = default;

    /**
     * \brief 키가 눌렸는지 확인합니다.
     * \param key_code 키 코드
     * \return bool
     */
    bool GetKeyDown(int key_code);

    /**
     * \brief 키를 누루고 있는 중인지 확인합니다.
     * \param key_code 키 코드
     * \return bool
     */
    bool GetKey(int key_code);

    /**
     * \brief 키를 뗏는지 확인합니다.
     * \param key_code 키 코드
     * \return bool
     */
    bool GetKeyUp(int key_code);
};
