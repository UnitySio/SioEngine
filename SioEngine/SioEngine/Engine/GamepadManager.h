#pragma once
#pragma comment(lib, "Xinput")

#include "Singleton.h"

#include <Xinput.h>

class GamepadManager :
    public Singleton<GamepadManager>
{
    friend class Core;

    enum class ButtonState : int
    {
        kNone = 0,
        kDown,
        kHeld,
        kUp
    };

    struct Button
    {
        ButtonState state;
        bool is_down;
    };

    struct Gamepad
    {
        std::map<int, Button> buttons;

        bool is_connected;

        float left_stick_deadzone;
        float right_stick_deadzone;
        float trigger_threshold;

        float left_trigger;
        float right_trigger;

        float left_stick_value;
        float right_stick_value;

        Vector2 left_stick_axis;
        Vector2 right_stick_axis;

        Gamepad() :
            left_stick_deadzone(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE),
            right_stick_deadzone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE),
            trigger_threshold(XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
        {
        }
    };

    XINPUT_STATE state_;

    Gamepad gamepads_[XUSER_MAX_COUNT];

    DWORD UpdateGamepadState(int user);

    void Update();
    void UpdateStickAxis(int user);
    void UpdateButtonState(int user);
    void UpdateTriggerState(int user);

public:
    GamepadManager() = default;
    ~GamepadManager() final = default;

    /**
     * \brief 게임패드가 연결이 되었는지 확인합니다.
     * \param user 유저 번호
     * \return bool
     */
    bool IsConnected(int user);

    /**
     * \brief 게임패드의 버튼이 눌렸는지 확인합니다.
     * \param user 유저 번호
     * \param button_code 버튼 코드
     * \return bool
     */
    bool GetButtonDown(int user, int button_code);

    /**
     * \brief 게임패드의 버튼이 눌려져 있는지 확인합니다.
     * \param user 유저 번호
     * \param button_code 버튼 코드
     * \return bool
     */
    bool GetButton(int user, int button_code);

    /**
     * \brief 게임패드의 버튼이 떼어졌는지 확인합니다.
     * \param user 
     * \param button_code 버튼 코드
     * \return bool
     */
    bool GetButtonUp(int user, int button_code);

    /**
     * \brief 게임패드의 배터리 잔량을 반환합니다.
     * \param user 유저 번호
     * \return float
     */
    float GetBatteryLevel(int user);

    /**
     * \brief 게임패드 왼쪽 트리거의 값을 반환합니다.
     * \param user 
     * \return float
     */
    float GetLeftTrigger(int user);

    /**
     * \brief 게임패드 오른쪽 트리거의 값을 반환합니다.
     * \param user 유저 번호
     * \return float
     */
    float GetRightTrigger(int user);

    /**
     * \brief 게임패드 왼쪽 스틱의 값을 반환합니다.
     * \param user 유저 번호
     * \return float
     */
    float GetLeftStickValue(int user);

    /**
     * \brief 게임패드 오른쪽 스틱의 값을 반환합니다.
     * \param user 유저 번호
     * \return flaot
     */
    float GetRightStickValue(int user);

    /**
     * \brief 게임패드 왼쪽 스틱의 축을 반환합니다.
     * \param user 유저 번호
     * \return Vector2
     */
    Vector2 GetLeftStickAxis(int user);

    /**
     * \brief 게임패드 오른쪽 스틱의 축을 반환합니다.
     * \param user 유저 번호
     * \return Vector2
     */
    Vector2 GetRightStickAxis(int user);
};
