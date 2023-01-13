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

    DWORD UpdateGamepadState(UserIndex user);

    void Update();
    void UpdateStickAxis(UserIndex user);
    void UpdateButtonState(UserIndex user);
    void UpdateTriggerState(UserIndex user);

public:
    GamepadManager() = default;
    ~GamepadManager() final;

    /**
     * \brief �����е尡 ������ �Ǿ����� Ȯ���մϴ�.
     * \param user ���� ��ȣ
     * \return bool
     */
    bool IsConnected(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е��� ��ư�� ���ȴ��� Ȯ���մϴ�.
     * \param user ���� ��ȣ
     * \param button_code ��ư �ڵ�
     * \return bool
     */
    bool GetButtonDown(UserIndex user, int button_code);

    /**
     * \brief �����е��� ��ư�� ������ �ִ��� Ȯ���մϴ�.
     * \param user ���� ��ȣ
     * \param button_code ��ư �ڵ�
     * \return bool
     */
    bool GetButton(UserIndex user, int button_code);

    /**
     * \brief �����е��� ��ư�� ���������� Ȯ���մϴ�.
     * \param user
     * \param button_code ��ư �ڵ�
     * \return bool
     */
    bool GetButtonUp(UserIndex user, int button_code);

    /**
     * \brief �����е��� ���͸� �ܷ��� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return float
     */
    float GetBatteryLevel(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ���� Ʈ������ ���� ��ȯ�մϴ�.
     * \param user
     * \return float
     */
    float GetLeftTrigger(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ������ Ʈ������ ���� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return float
     */
    float GetRightTrigger(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ���� ��ƽ�� ���� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return float
     */
    float GetLeftStickValue(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ������ ��ƽ�� ���� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return flaot
     */
    float GetRightStickValue(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ���� ��ƽ�� ���� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return Vector2
     */
    Vector2 GetLeftStickAxis(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е� ������ ��ƽ�� ���� ��ȯ�մϴ�.
     * \param user ���� ��ȣ
     * \return Vector2
     */
    Vector2 GetRightStickAxis(UserIndex user = UserIndex::kOne);

    /**
     * \brief �����е忡 ���� �ӵ��� �����մϴ�.
     * \param user ���� ��ȣ
     * \param left_moter_speed ���� ���� �ӵ�
     * \param right_moter_speed ������ ���� �ӵ�
     */
    void SetVibrate(UserIndex user = UserIndex::kOne, float left_moter_speed = 0.f, float right_moter_speed = 0.f);
};
