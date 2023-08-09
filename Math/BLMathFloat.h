#pragma once

#include <blMatrix4x4.inl>
#include <blRect.inl>
#include <blVector3.inl>
#include <blVector2.inl>
#include <blQuad.inl>

namespace BoulderLeaf::Math
{
	using Matrix4x4 = Templates::Matrix4x4<float>;
	using Rect = Templates::Rect<float>;
	using Vector3 = Templates::Vector3<float>;
	using Vector2 = Templates::Vector2<float>;
	using Quad = Templates::Quad<float>;
}