#pragma once
#include "Singleton.h"

class SceneManager :
    public Singleton<SceneManager>
{
public:
    SceneManager();
    ~SceneManager() final = default;
    
    void FixedUpdate();
    void Update();
    void LateUpdate();
    void Render();
};
