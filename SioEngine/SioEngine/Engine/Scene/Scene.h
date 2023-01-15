#pragma once

class Object;

class Scene :
	public std::enable_shared_from_this<Scene>
{
	std::vector<std::shared_ptr<Object>> objects_[static_cast<size_t>(Layer::kEnd)];
public:
	std::wstring name;

	Scene() = default;
	virtual ~Scene() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void OnGUI();

	void AddObject(std::shared_ptr<Object> object, Layer layer);
	void AddEmptyObject();

	std::vector<std::shared_ptr<Object>>& GetObjects(Layer layer);
};
