#include "pch.h"
#include "GamepadManager.h"
#include "Core.h"

DWORD GamepadManager::UpdateGamepadState(int user)
{
    ZeroMemory(&state_, sizeof(XINPUT_STATE));

    DWORD result = XInputGetState(user, &state_);

    return result;
}

void GamepadManager::Update()
{
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        DWORD result = UpdateGamepadState(i);

        if (result == ERROR_SUCCESS)
        {
            gamepads_[i].is_connected = true;

            UpdateStickAxis(i);
            UpdateButtonState(i);
            UpdateTriggerState(i);
        }
        else
        {
            gamepads_[i].is_connected = false;
        }
    }
}

void GamepadManager::UpdateStickAxis(int user)
{
    const auto left_stick_x = static_cast<float>(state_.Gamepad.sThumbLX);
    const auto left_stick_y = static_cast<float>(state_.Gamepad.sThumbLY);

    const auto right_stick_x = static_cast<float>(state_.Gamepad.sThumbRX);
    const auto right_stick_y = static_cast<float>(state_.Gamepad.sThumbRY);

    Vector2 left_stick = {left_stick_x, -left_stick_y};
    Vector2 right_stick = {right_stick_x, -right_stick_y};

    float left_stick_value = left_stick.Magnitude();
    float right_stick_value = right_stick.Magnitude();

    if (left_stick_value > gamepads_[user].left_stick_deadzone)
    {
        if (left_stick_value > 32767.0f)
        {
            left_stick_value = 32767.0f;
        }

        left_stick_value -= gamepads_[user].left_stick_deadzone;

        gamepads_[user].left_stick_value = left_stick_value / (32767.0f - gamepads_[user].left_stick_deadzone);
    }
    else
    {
        gamepads_[user].left_stick_value = 0.f;
    }

    if (right_stick_value > gamepads_[user].right_stick_deadzone)
    {
        if (right_stick_value > 32767.0f)
        {
            right_stick_value = 32767.0f;
        }

        right_stick_value -= gamepads_[user].right_stick_deadzone;

        gamepads_[user].right_stick_value = right_stick_value / (32767.0f - gamepads_[user].right_stick_deadzone);
    }
    else
    {
        gamepads_[user].right_stick_value = 0.f;
    }
    
    gamepads_[user].left_stick_axis = left_stick.Normalized();
    gamepads_[user].right_stick_axis = right_stick.Normalized();
}

void GamepadManager::UpdateButtonState(int user)
{
    auto iter = gamepads_[user].buttons.begin();
    for (; iter != gamepads_[user].buttons.end(); ++iter)
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

void GamepadManager::UpdateTriggerState(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    auto left_trigger = static_cast<float>(state_.Gamepad.bLeftTrigger);
    auto right_trigger = static_cast<float>(state_.Gamepad.bRightTrigger);

    if (left_trigger > gamepads_[user].trigger_threshold)
    {
        if (left_trigger > 255.f)
        {
            left_trigger = 255.f;
        }

        left_trigger -= gamepads_[user].trigger_threshold;

        gamepads_[user].left_trigger = left_trigger / (255.f - gamepads_[user].trigger_threshold);
    }
    else
    {
        gamepads_[user].left_trigger = 0.f;
    }

    if (right_trigger > gamepads_[user].trigger_threshold)
    {
        if (right_trigger > 255.f)
        {
            right_trigger = 255.f;
        }

        right_trigger -= gamepads_[user].trigger_threshold;

        gamepads_[user].right_trigger = right_trigger / (255.f - gamepads_[user].trigger_threshold);
    }
    else
    {
        gamepads_[user].right_trigger = 0.f;
    }
}

bool GamepadManager::IsConnected(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].is_connected;
}

bool GamepadManager::GetButtonDown(int user, int button_code)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].buttons[button_code].state == ButtonState::kDown;
}

bool GamepadManager::GetButton(int user, int button_code)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].buttons[button_code].state == ButtonState::kHeld;
}

bool GamepadManager::GetButtonUp(int user, int button_code)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].buttons[button_code].state == ButtonState::kUp;
}

float GamepadManager::GetBatteryLevel(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    XINPUT_BATTERY_INFORMATION battery_info;
    ZeroMemory(&battery_info, sizeof(XINPUT_BATTERY_INFORMATION));

    XInputGetBatteryInformation(user, BATTERY_DEVTYPE_GAMEPAD, &battery_info);

    if (battery_info.BatteryType == BATTERY_TYPE_WIRED)
    {
        return 1.f;
    }

    if (battery_info.BatteryLevel == BATTERY_LEVEL_EMPTY)
    {
        return 0.f;
    }
    else if (battery_info.BatteryLevel == BATTERY_LEVEL_LOW)
    {
        return .25f;
    }
    else if (battery_info.BatteryLevel == BATTERY_LEVEL_MEDIUM)
    {
        return .5f;
    }
    else if (battery_info.BatteryLevel == BATTERY_LEVEL_FULL)
    {
        return 1.f;
    }

    return 0.f;
}

float GamepadManager::GetLeftTrigger(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].left_trigger;
}

float GamepadManager::GetRightTrigger(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].right_trigger;
}

float GamepadManager::GetLeftStickValue(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].left_stick_value;
}

float GamepadManager::GetRightStickValue(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].right_stick_value;
}

Vector2 GamepadManager::GetLeftStickAxis(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].left_stick_axis;
}

Vector2 GamepadManager::GetRightStickAxis(int user)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);

    return gamepads_[user].right_stick_axis;
}

void GamepadManager::SetVibrate(int user, float left_moter_speed, float right_moter_speed)
{
    user = std::clamp(user, 0, XUSER_MAX_COUNT - 1);
    left_moter_speed = std::clamp(left_moter_speed, 0.f, 1.f);
    right_moter_speed = std::clamp(right_moter_speed, 0.f, 1.f);

    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = static_cast<WORD>(left_moter_speed * 65535.f);
    vibration.wRightMotorSpeed = static_cast<WORD>(right_moter_speed * 65535.f);

    XInputSetState(user, &vibration);
}
