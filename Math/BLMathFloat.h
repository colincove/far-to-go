#pragma once

#include "Matrix4x4.inl"
#include "Rect.inl"
#include "Vector3.inl"
#include "Vector2.inl"
#include "Quad.inl"

namespace BoulderLeaf::Math
{
	using Matrix4x4 = Templates::Matrix4x4<float>;
	using Rect = Templates::Rect<float>;
	using Vector3 = Templates::Vector3<float>;
	using Vector2 = Templates::Vector2<float>;
	using Quad = Templates::Quad<float>;
}