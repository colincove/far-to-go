#include "../Math/blMath.inl"
#include <blDX12.h>
#include <blDX12Math.inl>
#include <blDx12VertexBuffer.h>
#include <blDx12VertexData.h>
#include <blDx12UploadBuffer.h>
#include <blDX12ReadFile.h>
#include "BoxExample.h"
#include <DirectXColors.h>
#include <memory>
#include <string>

namespace BoulderLeaf::Graphics::DX12
{
	BoxExample::BoxExample(std::shared_ptr<DX12> dx12)
		:AbstractExample(dx12)
	{
		//When should I do this? it was not in the example
		//You cannot call Reset unless it is in a closed state
		m_dx12->mCommandList->Close();

		// Reset the command list to prep for initialization commands.
		m_dx12->mCommandList->Reset(m_dx12->mDirectCommandListAllocator.Get(), nullptr);
		BuildConstantBuffers();
		BuildRootSignature();
		BuildShadersAndInputLayout();
		BuildBoxGeometry();
		BuildPSO();

		// Execute the initialization commands.
		//m_dx12->mCommandList->Close(); //I am assuming that I don't need to do this, beause I am doing it above. 
		ID3D12CommandList* cmdsLists[] = { m_dx12->mCommandList.Get() };


		//again. Why do do Need to do this? I got an error.
		// //Command lists must be closed before execution.
		m_dx12->mCommandList->Close();

		m_dx12->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);


		// Wait until initialization is complete.
		m_dx12->FlushCommandQueue();
	};

	BoxExample::~BoxExample()
	{
		m_dx12->FlushCommandQueue();
	}

	void BoxExample::Update(const Metrics::blTime& gameTime)
	{
		mTheta += 0.05f;
		mPhi += 0.05f;
		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);
		// Build the view matrix.
		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&mView, view);
		XMMATRIX world = XMLoadFloat4x4(&mWorld);
		XMMATRIX proj = XMLoadFloat4x4(&m_dx12->mProj);
		XMMATRIX worldViewProj = world * view * proj;
		// Update the constant buffer with the latest worldViewProj matrix.
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
		mObjectCB->CopyData(0, objConstants);
	};

	void BoxExample::Draw() 
	{
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished
		// execution on the GPU.
		m_dx12->mDirectCommandListAllocator->Reset();
		// A command list can be reset after it has been added to the
		// command queue via ExecuteCommandList. Reusing the command
		// list reuses memory.
		//https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx12/main.cpp
		m_dx12->mCommandList->Reset(m_dx12->mDirectCommandListAllocator.Get(), mPSO.Get());
		m_dx12->mCommandList->RSSetViewports(1, &m_dx12->mViewPort);
		m_dx12->mCommandList->RSSetScissorRects(1, &m_dx12->mScissorRect);
		// Indicate a state transition on the resource usage.
		ID3D12Resource* currentBackBuffer = m_dx12->CurrentBackBuffer();
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_dx12->mCommandList->ResourceBarrier(1,
			&resourceBarrier);
		
		// Clear the back buffer and depth buffer.
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_dx12->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = m_dx12->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = mBoxGeo->VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = mBoxGeo->IndexBufferView();
		m_dx12->mCommandList->ClearRenderTargetView(m_dx12->CurrentBackBufferView(),
			Colors::LightSteelBlue, 0, nullptr);
		m_dx12->mCommandList->ClearDepthStencilView(m_dx12->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);

		//TESTING try to get the depth buffer into a different state
		//CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_dx12->mDepthBufferView.Get(),
		//	D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		//m_dx12->mCommandList->ResourceBarrier(1,
		//&resourceBarrier);
		//END TESTING
		
		// Specify the buffers we are going to render to.
		m_dx12->mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_dx12->mCbvHeap.Get() };
		m_dx12->mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		m_dx12->mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
		m_dx12->mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		m_dx12->mCommandList->IASetIndexBuffer(&indexBufferView);
		m_dx12->mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_dx12->mCommandList->SetGraphicsRootDescriptorTable(0, m_dx12->mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		m_dx12->mCommandList->DrawIndexedInstanced(
			mBoxGeo->DrawArgs["box"].IndexCount,
			1, 0, 0, 0);
		// Indicate a state transition on the resource usage.
		currentBackBuffer = m_dx12->CurrentBackBuffer();
		CD3DX12_RESOURCE_BARRIER resourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_dx12->mCommandList->ResourceBarrier(1,
			&resourceBarrier2);
		// Done recording commands.
		m_dx12->mCommandList->Close();
		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { m_dx12->mCommandList.Get() };
		m_dx12->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
		// swap the back and front buffers
		m_dx12->mSwapChain->Present(0, 0);
		m_dx12->mCurrBackBuffer = (m_dx12->mCurrBackBuffer + 1) % SwapChainBufferCount;
		// Wait until frame commands are complete. This waiting is
		// inefficient and is done for simplicity. Later we will show how to
		// organize our rendering code so we do not have to wait per frame.
		m_dx12->FlushCommandQueue();
	}
	void BoxExample::OnMouseDown(WPARAM btnState, int x, int y) {}
	void BoxExample::OnMouseUp(WPARAM btnState, int x, int y) {}
	void BoxExample::OnMouseMove(WPARAM btnState, int x, int y) {}

	void BoxExample::BuildConstantBuffers() 
	{
		mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_dx12->mDevice.Get(), 1, true);

		UINT objCBByteSize = Math::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource() -> GetGPUVirtualAddress();
		// Offset to the ith object constant buffer in the buffer.
		// Here our i = 0.
		int boxCBufIndex = 0;
		cbAddress += boxCBufIndex * objCBByteSize;
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = Math::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		m_dx12->mDevice->CreateConstantBufferView(
			&cbvDesc,
			m_dx12->mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	}
	void BoxExample::BuildRootSignature() 
	{
		// Shader programs typically require resources as input (constant
		// buffers, textures, samplers). The root signature defines the
		// resources the shader programs expect. If we think of the shader
		// programs as a function, and the input resources as function
		// parameters, then the root signature can be thought of as defining
		// the function signature.
		// Root parameter can be a table, root descriptor or root constants.
		CD3DX12_ROOT_PARAMETER slotRootParameter[1];
		// Create a single descriptor table of CBVs.
		CD3DX12_DESCRIPTOR_RANGE cbvTable;
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
		// A root signature is an array of root parameters.
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		// create a root signature with a single slot which points to a
		// descriptor range consisting of a single constant buffer
		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
		
		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}

		m_dx12->mDevice->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature));
	}
	void BoxExample::BuildShadersAndInputLayout() 
	{
		HRESULT hr = S_OK;

		mvsByteCode = LoadBinary(std::wstring(L"../../shaders/bin/v_box_example.cso"));
		mpsByteCode = LoadBinary(std::wstring(L"../../shaders/bin/p_box_example.cso"));
		
		mInputLayout = std::vector<D3D12_INPUT_ELEMENT_DESC>(VertexDesc);
	}
	void BoxExample::BuildBoxGeometry() 
	{
		std::array<Vertex, 8> vertices =
		{
			Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::White), XMFLOAT2() }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Black), XMFLOAT2()}),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Red), XMFLOAT2() }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Green), XMFLOAT2() }),
			Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Blue), XMFLOAT2() }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Yellow), XMFLOAT2() }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Cyan), XMFLOAT2() }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT3(), XMFLOAT4(DirectX::Colors::Magenta), XMFLOAT2()
		})
		};

		std::array<std::uint16_t, 36> indices =
		{
			// front face

			0, 1, 2,
			0, 2, 3,
			// back face
			4, 6, 5,
			4, 7, 6,
			// left face
			4, 5, 1,
			4, 1, 0,
			// right face
			3, 2, 6,
			3, 6, 7,
			// top face
			1, 5, 6,
			1, 6, 2,
			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);
		mBoxGeo = std::make_unique<MeshGeometry>();
		mBoxGeo->Name = "boxGeo";
		D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU);
		CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
		D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU);
		CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		mBoxGeo->VertexBufferGPU = CreateDefaultBuffer(
			m_dx12->mDevice.Get(),
			m_dx12->mCommandList.Get(),
			vertices.data(),
			vbByteSize,
			mBoxGeo->VertexBufferUploader);

		mBoxGeo->IndexBufferGPU = CreateDefaultBuffer(
			m_dx12->mDevice.Get(),
			m_dx12->mCommandList.Get(),
			indices.data(),
			ibByteSize,
			mBoxGeo->IndexBufferUploader);

		mBoxGeo->VertexByteStride = sizeof(Vertex);
		mBoxGeo->VertexBufferByteSize = vbByteSize;
		mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
		mBoxGeo->IndexBufferByteSize = ibByteSize;
		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)indices.size();

		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;
		mBoxGeo->DrawArgs["box"] = submesh;
	}
	void BoxExample::BuildPSO() 
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
		psoDesc.pRootSignature = mRootSignature.Get();
		psoDesc.VS =
		{
			reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
			mvsByteCode->GetBufferSize()
		};
		psoDesc.PS =
		{
			reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
			mpsByteCode->GetBufferSize()
		};
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = sBackbufferFormat;
		//The render target sample desc in slot 0 does not match that specified by the current pipeline state.
		//psoDesc.SampleDesc.Count = m_dx12->m4xMsaaState ? 4 : 1;
		//psoDesc.SampleDesc.Quality = m_dx12->m4xMsaaState ? (m_dx12->m4xMsaaQuality - 1) : 0;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.DSVFormat = sDepthStencilFormat;
		m_dx12->mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
	}
}