#pragma once
#include "Scene.h"

class EmptyScene :
    public Scene
{
public:
    EmptyScene();
    ~EmptyScene() final = default;

    void Enter() final;
    void Exit() final;
};
