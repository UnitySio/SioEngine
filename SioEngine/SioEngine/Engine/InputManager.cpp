#include "pch.h"
#include "InputManager.h"
#include "Core.h"

void InputManager::Update()
{
    for (auto iter = keys_.begin(); iter != keys_.end(); ++iter)
    {
        if (CORE->GetHWNDFocus())
        {
            if (GetAsyncKeyState(iter->first) & 0x8000)
            {
                if (iter->second.is_down)
                {
                    iter->second.state = KeyState::kHeld;
                }
                else
                {
                    iter->second.state = KeyState::kDown;
                }

                iter->second.is_down = true;
            }
            else
            {
                if (iter->second.is_down)
                {
                    iter->second.state = KeyState::kUp;
                }
                else
                {
                    iter->second.state = KeyState::kNone;
                }

                iter->second.is_down = false;
            }
        }
        else
        {
            if (iter->second.state == KeyState::kDown ||
                iter->second.state == KeyState::kHeld)
            {
                iter->second.state = KeyState::kUp;
            }
            else
            {
                iter->second.state = KeyState::kNone;
            }

            iter->second.is_down = false;
        }
    }

    if (CORE->GetHWNDFocus())
    {
        POINT mouse_pos = {};
        GetCursorPos(&mouse_pos);
        ScreenToClient(CORE->GetHWND(), &mouse_pos);

        const auto mouse_x = static_cast<float>(mouse_pos.x);
        const auto mouse_y = static_cast<float>(mouse_pos.y);

        mouse_previous_position_ = mouse_position_;
        mouse_position_ = { mouse_x, mouse_y };
        mouse_delta_ = mouse_position_ - mouse_previous_position_;
    }
}

bool InputManager::GetKeyDown(int key_code)
{
    return keys_[key_code].state == KeyState::kDown;
}

bool InputManager::GetKey(int key_code)
{
    return keys_[key_code].state == KeyState::kHeld;
}

bool InputManager::GetKeyUp(int key_code)
{
    return keys_[key_code].state == KeyState::kUp;
}

bool InputManager::GetMouseMove()
{
    return mouse_delta_ != 0;
}

Vector2 InputManager::GetMousePosition()
{
    return mouse_position_;
}

Vector2 InputManager::GetMouseDelta()
{
    return mouse_delta_;
}
