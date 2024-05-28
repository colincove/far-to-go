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

		Rotation():
			m_quaternion()
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
			return Vector3();
		}

		Vector3 Up() const
		{
			return Vector3();
		}

		Vector3 Left() const
		{
			return Vector3();
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
			//determine from quaternian
			return CartesianCoordinates(Forward(), Up(), Left());
		}
	};
}