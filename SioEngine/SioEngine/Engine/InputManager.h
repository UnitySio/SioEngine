#pragma once
#include "Singleton.h"

class InputManager :
    public Singleton<InputManager>
{
    friend class Core;

    enum class KeyState : int
    {
        kNone = 0,
        kDown,
        kHeld,
        kUp
    };

    struct Key
    {
        KeyState state;
        bool is_down;
    };

    std::map<int, Key> keys_;

    Vector2 mouse_position_;
    Vector2 mouse_previous_position_;
    Vector2 mouse_delta_;

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
     * \brief 키가 눌려있는지 확인합니다.
     * \param key_code 키 코드
     * \return bool
     */
    bool GetKey(int key_code);

    /**
     * \brief 키가 떼어졌는지 확인합니다.
     * \param key_code 키 코드
     * \return bool
     */
    bool GetKeyUp(int key_code);

    /**
     * \brief 마우스가 움직이는지 확인합니다.
     * \return bool
     */
    bool GetMouseMove();

    /**
     * \brief 마우스의 위치를 반환합니다.
     * \return Vector2
     */
    Vector2 GetMousePosition();

    Vector2 GetMouseDelta();
};
