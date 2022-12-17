#include "pch.h"
#include "InputManager.h"
#include "Core.h"

void InputManager::Update()
{
    std::map<int, KeyState>::iterator iter = keys_.begin();
    for (; iter != keys_.end(); ++iter)
    {
        if (CORE->GetHWNDFocus())
        {
            if (GetAsyncKeyState(iter->first) & 0x8000)
            {
                if (iter->second.is_down)
                {
                    iter->second.type = KeyType::kHold;
                }
                else
                {
                    iter->second.type = KeyType::kDown;
                }
        
                iter->second.is_down = true;
            }
            else
            {
                if (iter->second.is_down)
                {
                    iter->second.type = KeyType::kUp;
                }
                else
                {
                    iter->second.type = KeyType::kNone;
                }
        
                iter->second.is_down = false;
            }
        }
        else
        {
            if (iter->second.type == KeyType::kDown ||
                iter->second.type == KeyType::kHold)
            {
                iter->second.type = KeyType::kUp;
            }
            else
            {
                iter->second.type = KeyType::kNone;
            }

            iter->second.is_down = false;
        }
    }

    POINT mouse_pos = {};
    GetCursorPos(&mouse_pos);
    ScreenToClient(CORE->GetHWND(), &mouse_pos);

    float mouse_x = static_cast<float>(mouse_pos.x);
    float mouse_y = static_cast<float>(mouse_pos.y);
    
    mouse_prev_pos_ = mouse_pos_;
    mouse_pos_ = {mouse_x, mouse_y};
    mouse_delta_ = mouse_pos_ - mouse_prev_pos_;
}

bool InputManager::GetKeyDown(int key_code)
{
    return keys_[key_code].type == KeyType::kDown;
}

bool InputManager::GetKey(int key_code)
{
    return keys_[key_code].type == KeyType::kHold;
}

bool InputManager::GetKeyUp(int key_code)
{
    return keys_[key_code].type == KeyType::kUp;
}

bool InputManager::GetMouseMove()
{
    return mouse_delta_ != 0;
}

Vector2 InputManager::GetMousePos()
{
    return mouse_pos_;
}

