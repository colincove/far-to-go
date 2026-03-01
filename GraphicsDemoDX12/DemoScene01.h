#pragma once

#include <blResource.h>
#include <blShader.h>
#include <blMesh.h>
#include <blGraphicsAPI.h>
#include <blRenderScene.h>
#include <blMeshLibrary.h>
#include <blMaterial.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	class blDemoScene01
	{
	private:
		blMeshBaseResourcePtr mBoxMeshResource;
		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		std::shared_ptr<API> mGraphicsAPI;
		RenderItemBuffer mRenderItems;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		RenderMeshDataInstanced mDrawData;
	public:
		blDemoScene01(std::shared_ptr<API> graphicsAPI);
		void Draw();
		void Update();
	};
}