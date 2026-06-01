#include <gtest/gtest.h>
#include <blMatrix4x4.inl>
#include <DirectXMath.h>
#include <blDX12Core.h>
#include <blMath.inl>
#include "blDX12Buffer.h"

namespace BoulderLeaf::Graphics::DX12::Test
{
	using namespace BoulderLeaf;
	using namespace DirectX;

	TEST(DX12, ViewMatrixDX12MatchLH)
	{
		const Math::Vector4 arbitraryUp = Math::Vector4::Vector3D(0, 1, 0);
		const Math::Vector4 from = Math::Vector4::Point3D(1, 1, 1);
		const Math::Vector4 to = Math::Vector4::Point3D(3, 3, 2);

		const Math::Matrix4x4 viewMatrix = Math::Matrix4x4::ViewMatrix(from, to, arbitraryUp);

		XMVECTOR dxArbitraryUp = XMVectorSet(arbitraryUp.x, arbitraryUp.y, arbitraryUp.z, 0);
		XMVECTOR dxFrom = XMVectorSet(from.x, from.y, from.z, 0);
		XMVECTOR dxTo = XMVectorSet(to.x, to.y, to.z, 0);

		XMMATRIX dxView = XMMatrixLookAtLH(dxFrom, dxTo, dxArbitraryUp);
		
		XMFLOAT4X4 storedView;
		XMStoreFloat4x4(&storedView, dxView);

		XMFLOAT4X4 marshalledStoredView;
		DX12BufferAdapter::Get().MarshalMatrix4x4(viewMatrix, reinterpret_cast<byte*>(&marshalledStoredView));

		for (int row = 0; row < 4; ++row)	
		{
			for(int column = 0; column < 4; ++column)
			{
				EXPECT_TRUE(Math::IsNearEqual(storedView.m[row][column], marshalledStoredView.m[row][column]));
				EXPECT_TRUE(Math::IsNearEqual(storedView.m[row][column], viewMatrix.elements[4 * row + column]));
			}
		}
	}

	TEST(DX12, LookTo)
	{


		//EXPECT_EQ(dx12Mesh.GetVertex(1), v1);
	}
}