#pragma once

class Object;

class Transform
{
	friend class Object;

	Object* owner_;

public:
	Vector2 position;
	Vector2 scale;

	float z_rotation;

	Transform();
	Transform(const Transform& kOrigin);

	Transform& operator=(const Transform&) = delete;

	~Transform() = default;

	Vector2 Right();
	Vector2 Left();

	void Translate(Vector2 translation);
};

