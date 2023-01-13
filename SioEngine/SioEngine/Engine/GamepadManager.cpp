#include "pch.h"
#include "GamepadManager.h"
#include "Core.h"

DWORD GamepadManager::UpdateGamepadState(UserIndex user)
{
    ZeroMemory(&state_, sizeof(XINPUT_STATE));

    const DWORD result = XInputGetState(static_cast<int>(user), &state_);

    return result;
}

void GamepadManager::Update()
{
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        const auto user_idx = static_cast<UserIndex>(i);

        if (const DWORD result = UpdateGamepadState(user_idx); result == ERROR_SUCCESS)
        {
            gamepads_[i].is_connected = true;

            UpdateStickAxis(user_idx);
            UpdateButtonState(user_idx);
            UpdateTriggerState(user_idx);
        }
        else
        {
            gamepads_[i].is_connected = false;
        }
    }
}

void GamepadManager::UpdateStickAxis(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    const auto left_stick_x = static_cast<float>(state_.Gamepad.sThumbLX);
    const auto left_stick_y = static_cast<float>(state_.Gamepad.sThumbLY);

    const auto right_stick_x = static_cast<float>(state_.Gamepad.sThumbRX);
    const auto right_stick_y = static_cast<float>(state_.Gamepad.sThumbRY);

    Vector2 left_stick = { left_stick_x, -left_stick_y };
    Vector2 right_stick = { right_stick_x, -right_stick_y };

    float left_stick_value = left_stick.Magnitude();
    float right_stick_value = right_stick.Magnitude();

    if (left_stick_value > gamepads_[user_idx].left_stick_deadzone)
    {
        if (left_stick_value > 32767.0f)
        {
            left_stick_value = 32767.0f;
        }

        left_stick_value -= gamepads_[user_idx].left_stick_deadzone;

        gamepads_[user_idx].left_stick_value = left_stick_value / (32767.0f - gamepads_[user_idx].left_stick_deadzone);
    }
    else
    {
        gamepads_[user_idx].left_stick_value = 0.f;
    }

    if (right_stick_value > gamepads_[user_idx].right_stick_deadzone)
    {
        if (right_stick_value > 32767.0f)
        {
            right_stick_value = 32767.0f;
        }

        right_stick_value -= gamepads_[user_idx].right_stick_deadzone;

        gamepads_[user_idx].right_stick_value = right_stick_value / (32767.0f - gamepads_[user_idx].
            right_stick_deadzone);
    }
    else
    {
        gamepads_[user_idx].right_stick_value = 0.f;
    }

    gamepads_[user_idx].left_stick_axis = left_stick.Normalized();
    gamepads_[user_idx].right_stick_axis = right_stick.Normalized();
}

void GamepadManager::UpdateButtonState(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    for (auto iter = gamepads_[user_idx].buttons.begin(); iter != gamepads_[user_idx].buttons.end(); ++iter)
    {
        if (CORE->GetHWNDFocus())
        {
            if (state_.Gamepad.wButtons & iter->first)
            {
                if (iter->second.is_down)
                {
                    iter->second.state = ButtonState::kHeld;
                }
                else
                {
                    iter->second.state = ButtonState::kDown;
                }

                iter->second.is_down = true;
            }
            else
            {
                if (iter->second.is_down)
                {
                    iter->second.state = ButtonState::kUp;
                }
                else
                {
                    iter->second.state = ButtonState::kNone;
                }

                iter->second.is_down = false;
            }
        }
        else
        {
            if (iter->second.state == ButtonState::kDown ||
                iter->second.state == ButtonState::kHeld)
            {
                iter->second.state = ButtonState::kUp;
            }
            else
            {
                iter->second.state = ButtonState::kNone;
            }

            iter->second.is_down = false;
        }
    }
}

void GamepadManager::UpdateTriggerState(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    auto left_trigger = static_cast<float>(state_.Gamepad.bLeftTrigger);
    auto right_trigger = static_cast<float>(state_.Gamepad.bRightTrigger);

    if (left_trigger > gamepads_[user_idx].trigger_threshold)
    {
        if (left_trigger > 255.f)
        {
            left_trigger = 255.f;
        }

        left_trigger -= gamepads_[user_idx].trigger_threshold;

        gamepads_[user_idx].left_trigger = left_trigger / (255.f - gamepads_[user_idx].trigger_threshold);
    }
    else
    {
        gamepads_[user_idx].left_trigger = 0.f;
    }

    if (right_trigger > gamepads_[user_idx].trigger_threshold)
    {
        if (right_trigger > 255.f)
        {
            right_trigger = 255.f;
        }

        right_trigger -= gamepads_[user_idx].trigger_threshold;

        gamepads_[user_idx].right_trigger = right_trigger / (255.f - gamepads_[user_idx].trigger_threshold);
    }
    else
    {
        gamepads_[user_idx].right_trigger = 0.f;
    }
}

GamepadManager::~GamepadManager()
{
}

bool GamepadManager::IsConnected(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].is_connected;
}

bool GamepadManager::GetButtonDown(UserIndex user, int button_code)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].buttons[button_code].state == ButtonState::kDown;
}

bool GamepadManager::GetButton(UserIndex user, int button_code)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].buttons[button_code].state == ButtonState::kHeld;
}

bool GamepadManager::GetButtonUp(UserIndex user, int button_code)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].buttons[button_code].state == ButtonState::kUp;
}

float GamepadManager::GetBatteryLevel(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    XINPUT_BATTERY_INFORMATION battery_info;
    ZeroMemory(&battery_info, sizeof(XINPUT_BATTERY_INFORMATION));

    XInputGetBatteryInformation(user_idx, BATTERY_DEVTYPE_GAMEPAD, &battery_info);

    if (battery_info.BatteryType == BATTERY_TYPE_WIRED)
    {
        return 1.f;
    }

    if (battery_info.BatteryLevel == BATTERY_LEVEL_EMPTY)
    {
        return 0.f;
    }

    if (battery_info.BatteryLevel == BATTERY_LEVEL_LOW)
    {
        return .25f;
    }

    if (battery_info.BatteryLevel == BATTERY_LEVEL_MEDIUM)
    {
        return .5f;
    }

    if (battery_info.BatteryLevel == BATTERY_LEVEL_FULL)
    {
        return 1.f;
    }

    return 0.f;
}

float GamepadManager::GetLeftTrigger(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].left_trigger;
}

float GamepadManager::GetRightTrigger(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].right_trigger;
}

float GamepadManager::GetLeftStickValue(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].left_stick_value;
}

float GamepadManager::GetRightStickValue(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].right_stick_value;
}

Vector2 GamepadManager::GetLeftStickAxis(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].left_stick_axis;
}

Vector2 GamepadManager::GetRightStickAxis(UserIndex user)
{
    const auto user_idx = static_cast<int>(user);

    return gamepads_[user_idx].right_stick_axis;
}

void GamepadManager::SetVibrate(UserIndex user, float left_moter_speed, float right_moter_speed)
{
    const auto user_idx = static_cast<int>(user);

    left_moter_speed = std::clamp(left_moter_speed, 0.f, 1.f);
    right_moter_speed = std::clamp(right_moter_speed, 0.f, 1.f);

    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = static_cast<WORD>(left_moter_speed * 65535.f);
    vibration.wRightMotorSpeed = static_cast<WORD>(right_moter_speed * 65535.f);

    XInputSetState(user_idx, &vibration);
}
