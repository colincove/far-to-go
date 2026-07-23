#pragma once

#include <blBuffer.h>

#include <blMatrix4x4.inl>

namespace BoulderLeaf
{
	namespace Graphics
	{
		class VirtualCamera;
	}

	namespace Metrics
	{
		class blTime;
	}
}

namespace BoulderLeaf::Graphics
{
	struct blStandardObjectConstants
	{
		Math::Matrix4x4 WorldViewProj = Math::Matrix4x4::Identity();
	};

	struct blStandardObjectConstantsDefinition : public BufferDefinitionTemplate<blStandardObjectConstants, BufferFormat::BoulderLeaf>
	{

	};

	struct blTerrainObjectConstants
	{
		Math::Matrix4x4 World = Math::Matrix4x4::Identity();
		float Scale;
		uint32_t Size;
		float MinHeight;
		float MaxHeight;
		Math::Matrix4x4 WorldViewProj = Math::Matrix4x4::Identity();
		Math::Matrix4x4 WorldViewProjInverseTranspose = Math::Matrix4x4::Identity();
	};

	static const std::vector<BufferElementDescription> blTerrainObjectConstantsElementDescriptions =
	{
		{
				"World",
				BufferElementType::Matrix4x4,
				BufferElementSemantic::None
		},
		{
				"Scale",
				BufferElementType::Float,
				BufferElementSemantic::None
		},
		{
				"Size",
				BufferElementType::UInt,
				BufferElementSemantic::None
		},
		{
				"MinHeight",
				BufferElementType::Float,
				BufferElementSemantic::None
		},
		{
				"MaxHeight",
				BufferElementType::Float,
				BufferElementSemantic::None
		},
		{
				"WorldViewProj",
				BufferElementType::Matrix4x4,
				BufferElementSemantic::None
		},
		{
				"WorldViewProjInverseTranspose",
				BufferElementType::Matrix4x4,
				BufferElementSemantic::None
		},
	};

	struct blTerrainVertex
	{
		float Height;
		Math::Vector4 Normal;
	};

	void CalculateHeightBoundsForTerrain(const blTerrainVertex* data, uint32_t count, float& minOut, float& maxOut);

	static const std::vector<BufferElementDescription> blTerrainVertexElementDescriptions =
	{
		{
				"Height",
				BufferElementType::Float,
				BufferElementSemantic::Height
		},
		{
				"Normal",
				BufferElementType::Float4,
				BufferElementSemantic::Normal
		},
	};

	struct cbPass
	{
		BoulderLeaf::Math::Matrix4x4 view;
		BoulderLeaf::Math::Matrix4x4 invView;
		BoulderLeaf::Math::Matrix4x4 Proj;
		BoulderLeaf::Math::Matrix4x4 invProj;
		BoulderLeaf::Math::Matrix4x4 viewProj;
		BoulderLeaf::Math::Matrix4x4 invViewProj;
		BoulderLeaf::Math::Vector3 eyePosW;
		float cbPerObjectPad1;
		BoulderLeaf::Math::Vector2 renderTargetSize;
		BoulderLeaf::Math::Vector2 invRenderTargetSize;
		float nearZ;
		float farZ;
		float totalTime;
		float deltaTime;
	};

	cbPass CalculatePassData(const VirtualCamera& camera, const Metrics::blTime& gameTime);

	static const std::vector<BufferElementDescription> cbPassDescription =
	{
		{
				"View",
				BufferElementType::Matrix4x4
		},
		{
				"InvView",
				BufferElementType::Matrix4x4
		},
		{
				"Projection",
				BufferElementType::Matrix4x4
		},
		{
				"invProj",
				BufferElementType::Matrix4x4
		},
		{
				"viewProj",
				BufferElementType::Matrix4x4
		},
		{
				"invViewProj",
				BufferElementType::Matrix4x4
		},
		{
				"eyePosW",
				BufferElementType::Float3
		},
		{
				"cbPerObjectPad1",
				BufferElementType::Float
		},
		{
				"renderTargetSize",
				BufferElementType::Float2
		},
		{
				"invRenderTargetSize",
				BufferElementType::Float2
		},
		{
				"nearZ",
				BufferElementType::Float
		},
		{
				"farZ",
				BufferElementType::Float
		},
		{
				"totalTime",
				BufferElementType::Float
		},
		{
				"deltaTime",
				BufferElementType::Float
		},
	};
}