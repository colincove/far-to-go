#pragma once

#include <blResource.h>
#include <blShader.h>
#include <blMesh.h>
#include <blGraphicsAPI.h>
#include <blRenderScene.h>
#include <blMeshLibrary.h>
#include <blMaterial.h>
#include <blBufferTypes.h>
#include <blVirtualCamera.h>
#include <blWindow.h>
#include <blTime.h>
#include <DemoScene.h>
#include <memory>

namespace BoulderLeaf::Graphics
{
	class blMeshDemoSceneBase : public blDemoScene
	{
	protected:
		blMeshBaseResourcePtr mMeshResource;
		blResourceHandleOfType<blInlineMesh> mInlineMeshResourceHandle;
		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		RenderMeshDataInstanced mDrawData;
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();

		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		VirtualCamera mCamera;

	public:
		blMeshDemoSceneBase(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
			: blDemoScene(graphicsAPI, window, resourceContainer),
			mTheta(1.5f * PIf),
			mPhi(PIfDIV4),
			mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
		{
			mShaderResource = blResourceManager::Get().CreateResourceWithName<blShaderResource>(
				L"graphics_dx12_demoscene01",
				blShader(
					"graphics_dx12_demoscene01",
					std::vector<blShader::Parameter>
					{
						{1, 0, 0, blShader::Parameter::ConstantBuffer}
					},
					StandardVertexDefinition::Description));

			mMaterialResource = blResourceManager::Get().CreateResource<blMaterialResource>(mShaderResource, false);
			mSceneResource = blResourceManager::Get().CreateResource<blSceneResource>();
			mObjectConstantBufferResource = blResourceManager::Get().CreateResourceWithName<blStandardObjectConstantsBufferResource>(
				L"blDemoScene01 Standard Object Constants Buffer");

			mObjectConstantBufferResource->GetDataMutable().reserve(1000);
			mObjectConstantBufferResource->GetDataMutable().push_back({Math::Matrix4x4::Identity()});
			mObjectConstantBufferResource->GetDataMutable().push_back({ Math::Matrix4x4::Identity() });

			mDrawData.constantBuffer = std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource);
			mDrawData.group = blRenderGroups::Default;
			mDrawData.material = mMaterialResource;
		}

		virtual void Draw() override
		{
			mGraphicsAPI->DrawMeshInstanced(mDrawData, mSceneResource);
		}

		virtual void Update(const Metrics::blTime& gameTime) override
		{
			using namespace Math;
			static float rotationSpeed = 0.5f; // radians per second
			float deltaSeconds = gameTime.DeltaSeconds();
			mTheta += gameTime.DeltaSeconds() * rotationSpeed;
			mPhi += gameTime.DeltaSeconds() * rotationSpeed;
			// Convert Spherical to Cartesian coordinates.
			float x = mRadius * sinf(mPhi) * cosf(mTheta);
			float z = mRadius * sinf(mPhi) * sinf(mTheta);
			float y = mRadius * cosf(mPhi);

			Vector4 pos = Vector4(x, y, z, 1.0f);

			Vector4 target = Vector4::Zero();
			Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

			const Matrix4x4 translate = Matrix4x4::TranslationMatrix(-2, 0, 3);
			const Matrix4x4 translate2 = Matrix4x4::TranslationMatrix(0, 0, 5);
			const Matrix4x4 view = Matrix4x4::ViewMatrix(pos, target, up) * translate;
			const Matrix4x4 view2 = Matrix4x4::ViewMatrix(pos, target, up) * translate2;
			const Matrix4x4 world = Matrix4x4::TranslationMatrix(Vector3());
			const Matrix4x4 proj = mCamera.GetProjectionMatrix();
			const Matrix4x4 worldViewProj = world * view * proj;
			const Matrix4x4 worldViewProj2 = world * view2 * proj;

			mObjectConstantBufferResource->GetDataMutable()[0].WorldViewProj = worldViewProj.Transpose();
			mObjectConstantBufferResource->GetDataMutable()[1].WorldViewProj = worldViewProj2.Transpose();

			mGraphicsAPI->MarkResourceDirty(mObjectConstantBufferResource->GetId());
		}

	protected:
		void SetMeshResource(const blMeshBaseResourcePtr& mesh)
		{
			mMeshResource = mesh;
			mDrawData.mesh = mMeshResource;
		}

		void SetMeshResourceExprimentalFromInlineMesh(
			std::wstring name, 
			const std::unique_ptr<byte[]>& mesh,
			const BufferDescription& bufferDescription)
		{
			blInlineMesh& inlineMeshSource = reinterpret_cast<blInlineMesh&>(*mesh.get());

			blResourceHandleOfType<blInlineMesh> resource = blResourceHandleOfType<blInlineMesh>(
				mResourceContainer->CreateResource(name, inlineMeshSource.GetTotalSize()));
			
			memcpy(resource.GetMutable(),
				mesh.get(),
				inlineMeshSource.GetTotalSize());
			/*
			resource->mHeader.description = InlineBufferDescription::CreateResourceFromBufferDescription(name, bufferDescription, mResourceContainer);

			mInlineMeshResourceHandle = resource;
			mDrawData.meshResource = resource;
			mDrawData.meshResourceBufferDescription = bufferDescription;*/
		}

		void SetMeshResourceExprimental(const blResourceHandleOfType<blInlineMesh> mesh)
		{
			mInlineMeshResourceHandle = mesh;
			mDrawData.meshResource = mesh;
		}
	};
}
