#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

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
