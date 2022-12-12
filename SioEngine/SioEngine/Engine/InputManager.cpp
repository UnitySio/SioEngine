#include "pch.h"
#include "InputManager.h"

void InputManager::Update()
{
    HWND focus = GetFocus();
    
    std::map<int, KeyState>::iterator iter = keys_.begin();
    
    for (; iter != keys_.end(); ++iter)
    {
        if (focus)
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
