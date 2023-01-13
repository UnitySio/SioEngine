#include "pch.h"
#include "Scene.h"
#include "../Object/Object.h"

void Scene::FixedUpdate()
{
	for (size_t i = 0; i < (size_t)Layer::kEnd; i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->FixedUpdate();
		}
	}
}

void Scene::Update()
{
	for (size_t i = 0; i < (size_t)Layer::kEnd; i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->Update();
		}
	}
}

void Scene::LateUpdate()
{
	for (size_t i = 0; i < (size_t)Layer::kEnd; i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->LateUpdate();
		}
	}
}

void Scene::Render()
{
	for (size_t i = 0; i < (size_t)Layer::kEnd; i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->Render();
		}
	}
}

void Scene::OnGUI()
{
	for (size_t i = 0; i < (size_t)Layer::kEnd; i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->OnGUI();
		}
	}
}
