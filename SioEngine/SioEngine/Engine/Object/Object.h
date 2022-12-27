#pragma once

class Object :
    public std::enable_shared_from_this<Object>
{
    std::wstring name_;
    
    Vector2 position_;
    Vector2 scale_;
    
    float z_rotation_;

    int depth_;
public:
    Object();
    Object(const Object& kOrigin);
    virtual ~Object() = default;

    virtual void Awake();
    virtual void Start();
    virtual void FixedUpdate();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

    /**
     * \brief 오브젝트의 이름을 설정합니다.
     * \param name 이름
     */
    void SetName(std::wstring name);

    /**
     * \brief 오브젝트의 위치를 설정합니다.
     * \param position 위치
     */
    void SetPosition(Vector2 position);

    /**
     * \brief 오브젝트의 크기를 설정합니다.
     * \param scale 크기
     */
    void SetScale(Vector2 scale);

    /**
     * \brief 오브젝트를 회전합니다.
     * \param z_rotation 각도
     */
    void SetZRotation(float z_rotation);

    /**
     * \brief 오브젝트의 깊이를 설정합니다.
     * \param depth 깊이
     */
    void SetDepth(int depth);

    /**
     * \brief 오브젝트의 이름을 반환합니다.
     * \return wstring
     */
    std::wstring GetName();

    /**
     * \brief 오브젝트의 위치를 반환합니다.
     * \return Vector2
     */
    Vector2 GetPosition();

    /**
     * \brief 오브젝트의 크기를 반환합니다.
     * \return Vector2
     */
    Vector2 GetScale();

    /**
     * \brief 오브젝트의 각도를 반환합니다.
     * \return float
     */
    float GetZRotation();

    /**
     * \brief 오브젝트의 깊이를 반환합니다.
     * \return int
     */
    int GetDepth();
};
