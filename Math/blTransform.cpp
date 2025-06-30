#pragma once
#include <blMatrix4x4.inl>
#include <blTransform.h>

namespace BoulderLeaf::Math
{
	Matrix4x4 Transform::GetTransitionMatrix() const
	{
		return TransitionMatrix(rotation.GetFrame(), position);
	}

	void Transform::LookAt(const Vector4 point)
	{
		const Vector4 delta = (point - position).Normalize();
		rotation.SetQuaternian(Quaternion(
			rotation.GetQuaternian().real,
			Vector3(delta.x, delta.y, delta.z)
		));
	}
	
	Transform FromTransitionMatrix(const Matrix4x4 matrix)
	{
		return Transform();
	}
}