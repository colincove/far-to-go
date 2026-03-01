#include <blMeshRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	blMeshRenderComponent::blMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr)
	{
	}

	void blMeshRenderComponent::Render(const RenderMeshData& renderData, const  blSceneResourcePtr scene)
	{
		if (renderData.material.expired())
		{
			return;
		}

		blMaterialResourcePtr materialResourcePtr = renderData.material.lock();
		const blMaterial& material = materialResourcePtr->GetData();
		const blShaderResource& shaderResource = *material.shader;
		const blShader& shader = shaderResource.GetData();
		const blShaderCacheData& shaderCacheData = mGlobalRenderDataPtr->shaderCache->Get(shaderResource);

		if (renderData.mesh.expired())
		{
			return;
		}

		blMeshBaseResourcePtr meshBaseResourcePtr = renderData.mesh.lock();
		const blMeshBaseResource& meshResource = *meshBaseResourcePtr.get();
		const blMeshBase& mesh = meshBaseResourcePtr->GetData();
		const blDX12MeshStorageCacheData& meshCacheData = mGlobalRenderDataPtr->meshStorageCache->Get(meshResource);

		blRenderGroupData& renderGroupData = mGlobalRenderDataPtr->renderGroupData[renderData.group];
		const blDX12MeshDataDeviceCacheData& deviceCache = renderGroupData.meshDataDeviceCache->Get(meshResource);
		const blPSOCacheData& psoCacheData = mGlobalRenderDataPtr->mPSOCache->Get(shaderResource);
		const blPSO& pso = *psoCacheData.PSO;

		ComPtr<ID3D12GraphicsCommandList> commandList = renderGroupData.commandList->GetCommandListPtr();

		commandList->SetPipelineState(pso.GetDX12PSO().Get());

		// This is not right. Just set everything each time we draw. 
		// What might be more important is to minimize PSO switches.
		// Which is determined by the order in which we issue draw calls. 
		// (I think.....)
		if (mGlobalRenderDataPtr->mCurrentPSO.get() != nullptr
			&& *mGlobalRenderDataPtr->mCurrentPSO != pso)
		{
			//mGlobalRenderDataPtr->commandList->SetPipelineState(pso);
			//mGlobalRenderDataPtr->mCurrentPSO = psoCacheData.PSO;
		}
;	}
}

/*
mCommandAllocator->Reset();
mCommandList->Reset(mCommandAllocator.Get(), mPSO.Get());

mCommandList->RSSetViewports(1, &viewport);
mCommandList->RSSetScissorRects(1, &scissorRect);
mCommandList->OMSetRenderTargets(1, &rtvHandle, TRUE, &dsvHandle);

mCommandList->SetGraphicsRootSignature(rootSignature.Get());
mCommandList->SetPipelineState(pso.Get());

// Draw first object
mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView1);
mCommandList->IASetIndexBuffer(&indexBufferView1);
mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
mCommandList->DrawIndexedInstanced(indexCount1, 1, startIndex1, baseVertex1, 0);

// Draw second object
mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView2);
mCommandList->IASetIndexBuffer(&indexBufferView2);
mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
mCommandList->DrawIndexedInstanced(indexCount2, 1, startIndex2, baseVertex2, 0);

// ...repeat for more objects...

mCommandList->Close();
mCommandQueue->ExecuteCommandLists(1, &mCommandList);*/