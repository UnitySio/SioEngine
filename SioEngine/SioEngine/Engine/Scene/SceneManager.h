#pragma once
#include "../Singleton.h"

class Scene;

class SceneManager :
	public Singleton<SceneManager>
{
	std::shared_ptr<Scene> current_scene_;

public:
	SceneManager() = default;
	~SceneManager() = default;

	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Render();
	void OnGUI();
};

