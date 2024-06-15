#pragma once
#include "blMath3D.inl"
#include "blQuaternion.inl"

namespace BoulderLeaf::Math
{
	struct Rotation
	{
	private:
		Quaternion m_quaternion;
	public:

		Rotation(Quaternion quaternion) :
			m_quaternion(quaternion)
		{

		}

		Rotation():Rotation(Quaternion())
		{

		}

		Quaternion GetQuaternian() const
		{
			return m_quaternion;
		}

		void SetQuaternian(Quaternion quaternion)
		{
			m_quaternion = quaternion;
		}

		Vector3 Forward() const
		{
			return m_quaternion.vector;
		}

		Vector3 Up() const
		{
			return Matrix3x3::Transform(Quaternion::RotationMatrix3x3(m_quaternion), Vector3::Up());
		}

		Vector3 Left() const
		{
			return Matrix3x3::Transform(Quaternion::RotationMatrix3x3(m_quaternion), Vector3::Left());
		}

		Vector3 Backward() const
		{
			return Forward() * -1;
		}

		Vector3 Down() const
		{
			return Up() * -1;
		}

		Vector3 Right() const
		{
			return Left() * -1;
		}

		CartesianCoordinates GetFrame() const
		{
			return CartesianCoordinates(Forward(), Up(), Left());
		}
	};
}