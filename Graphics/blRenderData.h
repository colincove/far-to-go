#pragma once
#include <blTypes.h>
#include <blRenderScene.h>
#include <blRenderDataTypes.h>
#include <blMesh.h>
#include <blMatrix4x4.inl>
#include <blFlags.h>

namespace BoulderLeaf::Graphics
{
	namespace
	{

		template<typename TData, RenderDataType type>
		class blRenderDataInterface
		{
		public:
			static void Write(blRenderScene& scene, const TData& data)
			{
				scene.WriteRenderData<TData, type>(data);
			}
		};
	}

	struct StandardMeshRenderData
	{
	public:
		using Interface = blRenderDataInterface<StandardMeshRenderData, RenderDataType::Mesh>;

		enum class OptionBits : char
		{
			None = 0,
			Instance = 1 << 0,
		};

		using OptionFlags = blFlagsOf<OptionBits>;

	public:
		std::weak_ptr<StandardMesh> mesh;
		Math::Matrix4x4 transform;
		OptionFlags flags;
	};
}