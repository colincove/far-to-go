#pragma once

#include <memory>
#include <blGlobalRenderData.h>
#include <blWindow.h>
#include <blRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12Imgui : public blRenderComponentBase
	{
	public:
		blDX12Imgui(blGlobalRenderData* globalRenderData,
			Core::blWindow* window);

		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void Initialize() override;
		virtual void StartFrame() override;
		virtual void EndFrame() override;
	private:
		Core::blWindow* mWindow;
		std::unique_ptr<blConstantBufferDescriptorHeap> mConstantBufferDescriptorHeap;
	};
}