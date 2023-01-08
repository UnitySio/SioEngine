#pragma once

class Scene :
    public std::enable_shared_from_this<Scene>
{
    friend class SceneManager;

    std::wstring name_;
    
    void FixedUpdate();
    void Update();
    void LateUpdate();
    void Render();
    void OnGUI();

protected:
    virtual void Enter() = 0;
    virtual void Exit() = 0;

public:
    Scene();
    virtual ~Scene() = default;

    /**
     * \brief 씬의 이름을 설정합니다.
     * \param name 이름
     */
    void SetName(std::wstring name);

    /**
     * \brief 씬의 이름을 반환합니다.
     * \return wstring
     */
    std::wstring GetName();
};
