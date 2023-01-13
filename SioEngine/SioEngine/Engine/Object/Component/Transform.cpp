#include "pch.h"
#include "Transform.h"

Transform::Transform() :
	z_rotation()
{
}

Transform::Transform(const Transform& kOrigin) :
	z_rotation(kOrigin.z_rotation)
{
}

Vector2 Transform::Right()
{
	const float kTheta = z_rotation * (PI / 180.f);
	const float kX = cos(kTheta);
	const float kY = sin(kTheta);

	return { kX, kY };
}

Vector2 Transform::Left()
{
	const float kTheta = (z_rotation * 270.f) * (PI / 180.f);
	const float kX = cos(kTheta);
	const float kY = sin(kTheta);

	return { kX, kY };
}

void Transform::Translate(Vector2 translation)
{
	position += translation;
}
