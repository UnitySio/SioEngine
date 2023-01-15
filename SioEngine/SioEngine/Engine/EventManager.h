#pragma once
#include "Singleton.h"

class EventManager :
    public Singleton<EventManager>
{
public:
    EventManager();
    ~EventManager() final = default;

    void Update();
};
