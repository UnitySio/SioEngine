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
     * \brief Ű�� ���ȴ��� Ȯ���մϴ�.
     * \param key_code Ű �ڵ�
     * \return bool
     */
    bool GetKeyDown(int key_code);

    /**
     * \brief Ű�� �����ִ��� Ȯ���մϴ�.
     * \param key_code Ű �ڵ�
     * \return bool
     */
    bool GetKey(int key_code);

    /**
     * \brief Ű�� ���������� Ȯ���մϴ�.
     * \param key_code Ű �ڵ�
     * \return bool
     */
    bool GetKeyUp(int key_code);

    /**
     * \brief ���콺�� �����̴��� Ȯ���մϴ�.
     * \return bool
     */
    bool GetMouseMove();

    /**
     * \brief ���콺�� ��ġ�� ��ȯ�մϴ�.
     * \return Vector2
     */
    Vector2 GetMousePosition();

    Vector2 GetMouseDelta();
};
