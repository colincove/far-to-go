#pragma once

#include <memory>
#include <blGlobalRenderData.h>
#include <blWindow.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12Imgui
	{
	public:
		blDX12Imgui(std::shared_ptr<blGlobalRenderData> globalRenderData,
			std::shared_ptr<Core::blWindow> window);

		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void Initialize();
		void StartFrame();
		void EndFrame();
	private:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderData;	
		std::shared_ptr<Core::blWindow> mWindow;
		std::shared_ptr<blConstantBufferDescriptorHeap> mConstantBufferDescriptorHeap;
	};
}