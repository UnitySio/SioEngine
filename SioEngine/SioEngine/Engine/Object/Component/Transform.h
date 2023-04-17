#pragma once
#include "Component.h"

class Transform :
	public Component
{
public:
	Vector2 position;
	Vector2 scale;

	float z_rotation;

	Transform();
	Transform(const Transform& kOrigin);

	~Transform() final = default;

	Vector2 Up();
	Vector2 Right();

	void Translate(Vector2 translation);
};

