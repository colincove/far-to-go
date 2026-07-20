#pragma once

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
#include <blDX12Math.inl>

namespace BoulderLeaf::Graphics
{
	class blMeshDemoSceneBase : public blDemoScene
	{
	protected:
		blResourceHandleOfType<blIndexedMeshResource> mMeshResourceHandle;
		blResourceHandleOfType<blSceneResource> mSceneResource;
		blResourceHandleOfType<blMaterialResource> mMaterialResource;
		blResourceHandleOfType<blArrayBufferResource> mObjectConstantBufferResource;
		blResourceHandleOfType<blListResource> mObjectConstantBufferListResource;
		blResourceHandleOfType<blArrayBufferResource> mInstanceDataResource;

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
			mSceneResource = resourceContainer->CreateResourceOfType<blSceneResource>(L"Scene");
			
			blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
				resourceContainer, { BufferFormat::BoulderLeaf, blStandardObjectConstantsDefinition::Description, 256 });

			mObjectConstantBufferResource = CreateArrayBufferResource(
				resourceContainer,
				L"ObjectConstantBuffer",
				desc.GetRef(),
				sizeof(blStandardObjectConstants),
				2
			);

			mObjectConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType(mObjectConstantBufferResource->mBufferResourceRef);

			mDrawData.constantBuffer = mObjectConstantBufferResource;
			mDrawData.group = blRenderGroups::Default;
			mDrawData.material = mMaterialResource;
		}

		void SetMeshAndShaderData(blResourceHandleOfType<blIndexedMeshResource> mesh)
		{
			mMeshResourceHandle = mesh;
			mDrawData.mesh = mMeshResourceHandle;
			blResourceContainer* container = mesh.GetContainer();

			const blArrayBufferResource& vertexArrayBuffer = *container->CreateHandleFromRefOfType<blArrayBufferResource>(mesh->mArrayBufferResourceRef);

			// prepare shader parameters
			std::vector<blShader::Parameter> params =
			{
				{1, 0, 0, blShader::Parameter::ConstantBuffer}
			};

			const std::string shaderName = "graphics_dx12_demoscene01";

			// create shader resource
			blResourceHandleOfType<blShaderResource> handle =
				container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
					L"ShaderResource",
					shaderName,
					params.data(),
					static_cast<uint32_t>(params.size()),
					vertexArrayBuffer.mDescriptionResourceRef
				);

			mMaterialResource = container->CreateResourceOfType<blMaterialResource>(
				L"MaterialResource",
				handle.GetRef(),
				true /*instanced*/
			);

			mDrawData.material = mMaterialResource;
		}

		virtual void Draw() override
		{
			mGraphicsAPI->DrawMeshInstanced(mDrawData);
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

			mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(0).WorldViewProj = worldViewProj;
			mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(1).WorldViewProj = worldViewProj2;
			mObjectConstantBufferListResource.MarkDirty();
			mObjectConstantBufferResource.MarkDirty();
		}
	};
}
