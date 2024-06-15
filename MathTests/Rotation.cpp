#include <gtest/gtest.h>
#include "blRotation.inl"
#include "blQuaternion.inl"
#include <array>

namespace BoulderLeaf::Math::Test
{
	TEST(Rotation, CartesianCoordinates)
	{
		Rotation r (Quaternion(3.0f, 2.0f, 4.0f, 1.0f));
		CartesianCoordinates c(r.GetFrame());
		EXPECT_TRUE(Vector3::IsOrthogonalized(std::begin(c.data), std::end(c.data)));
		EXPECT_TRUE(c.x.IsNormalized());
		EXPECT_TRUE(c.y.IsNormalized());
		EXPECT_TRUE(c.z.IsNormalized());
	}
}