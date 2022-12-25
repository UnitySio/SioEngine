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

            UpdateButtonState(i);
            UpdateTriggerState(i);
        }
        else
        {
            gamepads_[i].is_connected = false;
        }
    }
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

    const auto left_trigger = static_cast<float>(state_.Gamepad.bLeftTrigger);
    const auto right_trigger = static_cast<float>(state_.Gamepad.bRightTrigger);

    if (left_trigger > gamepads_[user].trigger_threshold)
    {
        gamepads_[user].left_trigger = left_trigger / TRIGGER_MAX_THRESHOLD;
    }
    else
    {
        gamepads_[user].left_trigger = 0.f;
    }

    if (right_trigger > gamepads_[user].trigger_threshold)
    {
        gamepads_[user].right_trigger = right_trigger / TRIGGER_MAX_THRESHOLD;
    }
    else
    {
        gamepads_[user].right_trigger = 0.f;
    }
}

GamepadManager::GamepadManager()
{
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
