#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EmptyScene.h"

SceneManager::SceneManager()
{
	std::shared_ptr<Scene> empty_scene = std::make_shared<EmptyScene>();
	current_scene_ = empty_scene;
	empty_scene->Enter();
}

void SceneManager::FixedUpdate()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->FixedUpdate();
	}
}

void SceneManager::Update()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Update();
	}
}

void SceneManager::LateUpdate()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->LateUpdate();
	}
}

void SceneManager::Render()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->Render();
	}
}

void SceneManager::OnGUI()
{
	if (current_scene_ != nullptr)
	{
		current_scene_->OnGUI();
	}
}

Scene* SceneManager::GetCurrentScene()
{
	return current_scene_.get();
}
