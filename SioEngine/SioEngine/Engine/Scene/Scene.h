#pragma once

class Scene :
    public std::enable_shared_from_this<Scene>
{
public:
    Scene();
    virtual ~Scene() = default;
};
