#include "pch.h"
#include "Scene.h"
#include "../Object/Object.h"
#include "../Object/EmptyObject.h"

void Scene::FixedUpdate()
{
	for (size_t i = 0; i < static_cast<size_t>(Layer::kEnd); i++)
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
	for (size_t i = 0; i < static_cast<size_t>(Layer::kEnd); i++)
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
	for (size_t i = 0; i < static_cast<size_t>(Layer::kEnd); i++)
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
	for (size_t i = 0; i < static_cast<size_t>(Layer::kEnd); i++)
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
	for (size_t i = 0; i < static_cast<size_t>(Layer::kEnd); i++)
	{
		auto iter = objects_[i].begin();
		for (; iter != objects_[i].end(); ++iter)
		{
			(*iter)->OnGUI();
		}
	}
}

void Scene::AddObject(std::shared_ptr<Object> object, Layer layer)
{
	objects_[static_cast<size_t>(layer)].push_back(object);
}

void Scene::AddEmptyObject()
{
	std::shared_ptr<Object> empty_object = std::make_shared<EmptyObject>();
	empty_object->name = "GameObject";
	AddObject(empty_object, Layer::kDefault);
}

std::vector<std::shared_ptr<Object>>& Scene::GetObjects(Layer layer)
{
	return objects_[static_cast<size_t>(layer)];
}
