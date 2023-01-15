#pragma once

class Transform;

class Object :
	public std::enable_shared_from_this<Object>
{
	// Component
	std::shared_ptr<Transform> transform_;
public:
	std::string name;

	Object();
	Object(const Object& kOrigin);
	virtual ~Object() = default;

	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void OnGUI();

	void AddTransform();

	std::shared_ptr<Transform> GetTransform();
};

