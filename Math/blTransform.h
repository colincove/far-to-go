#pragma once
#include "blMatrix4x4.inl"
#include "blVector4.inl"
#include "blRotation.inl"

namespace BoulderLeaf::Math
{
	struct Transform
	{
		Vector4 position;
		Rotation rotation;

		Transform() :
			position(),
			rotation()
		{

		}

		Matrix4x4 GetTransitionMatrix() const;
		void LookAt(const Vector4 point);
	};

	Transform FromTransitionMatrix(const Matrix4x4 matrix);
}