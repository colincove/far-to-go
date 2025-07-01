#include <blDX12.h>
#include "AbstractExample.h"
#include <memory>
#include "blDX12Core.h"
#include "blDX12UploadBuffer.h"
#include <blDX12MeshGeometry.h>
#include <blDx12Math.inl>
#include <blTIme.h>

namespace BoulderLeaf::Graphics::DX12
{
	class ShapesExample : public AbstractExample
	{
	public:
		using ShapesMeshGeometry = MeshGeometry<StandardMesh>;
		static constexpr int s_numFrameResources = 3;

		struct PassConstants
		{
			XMFLOAT4X4 View;
			XMFLOAT4X4 InvView;
			XMFLOAT4X4 Proj;
			XMFLOAT4X4 InvProj;
			XMFLOAT4X4 ViewProj;
			XMFLOAT4X4 InvViewProj;

			XMFLOAT2 RenderTargetSize;
			XMFLOAT2 InvRenderTargetSize;
			float NearZ;
			float FarZ;
			float TotalTime;
			float DeltaTime;
		};

		struct ObjectConstants
		{

		};

		struct FrameResource
		{
		public:
			FrameResource(std::shared_ptr<DX12> dx12, UINT passCount, UINT objectCount);
			FrameResource(const FrameResource& rhs) = delete;

			std::shared_ptr<DX12> mDx12;
			UINT mPassCount;
			UINT mObjectCount;

			// We cannot reset the allocator until the GPU is done processing the
			// commands. So each frame needs their own allocator.
			ComPtr<ID3D12CommandAllocator> mCommandListAllocator;

			// We cannot update a cbuffer until the GPU is done processing the
			// commands that reference it. So each frame needs their own cbuffers.
			std::unique_ptr<UploadBuffer<PassConstants>> mPassConstants = nullptr;
			std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectConstants = nullptr;

			UINT64 mFence;
		};

		// Lightweight structure stores parameters to draw a shape. This will
		// vary from app-to-app.
		struct RenderItem
		{
			RenderItem() = default;
			// World matrix of the shape that describes the object’s local space
			// relative to the world space, which defines the position,
			// orientation, and scale of the object in the world.
			XMFLOAT4X4 World = Math::Identity4x4();
			// Dirty flag indicating the object data has changed and we need
			// to update the constant buffer. Because we have an object
			// cbuffer for each FrameResource, we have to apply the
			// update to each FrameResource. Thus, when we modify obect data we
			// should set
			// NumFramesDirty = gNumFrameResources so that each frame resource
			// gets the update.
			int NumFramesDirty = s_numFrameResources;
			// Index into GPU constant buffer corresponding to the ObjectCB
			// for this render item.
			UINT ObjCBIndex = -1;
			// Geometry associated with this render-item. Note that multiple
			// render-items can share the same geometry.
			ShapesMeshGeometry* Geo = nullptr;
			// Primitive topology.
			D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			// DrawIndexedInstanced parameters.
			UINT IndexCount = 0;
			UINT StartIndexLocation = 0;
			int BaseVertexLocation = 0;
		};

	public:
		ShapesExample(std::shared_ptr<DX12> dx12);
		~ShapesExample();
	public:
		void Update(const Metrics::blTime& gameTime) override;
		void Draw() override;
	private:
		std::vector<std::unique_ptr<FrameResource>> mFrameResources;
		FrameResource* mCurrFrameResource;
		int mCurrFrameResourceIndex;

		PassConstants mMainPassCB;

		// List of all the render items.
		std::vector<std::unique_ptr<RenderItem>> mAllRitems;
		// Render items divided by PSO.
		std::vector<RenderItem*> mOpaqueRitems;
		std::vector<RenderItem*> mTransparentRitems;

		XMFLOAT4X4 mWorld = Math::Identity4x4();
		XMFLOAT4X4 mView;
		XMFLOAT4X4 mProj;

		void BuildFrameResources();
		void UpdateObjectCBs();
		void UpdateMainPassCB(const Metrics::blTime& gameTime);
	};
}