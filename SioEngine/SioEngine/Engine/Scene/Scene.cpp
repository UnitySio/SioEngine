#include "pch.h"
#include "Scene.h"

Scene::Scene() :
    name_()
{
}

void Scene::FixedUpdate()
{
}

void Scene::Update()
{
}

void Scene::LateUpdate()
{
}

void Scene::Render()
{
}

void Scene::OnGUI()
{
}

void Scene::SetName(std::wstring name)
{
    name_ = name;
}

std::wstring Scene::GetName()
{
    return name_;
}
