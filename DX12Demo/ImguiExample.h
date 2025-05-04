#pragma once

#include "AbstractExample.h"
#include <memory.h>

namespace BoulderLeaf::Graphics::DX12
{
	class ImguiExample : public AbstractExample
	{
	public:
		ImguiExample(std::shared_ptr<DX12> dx12);
		~ImguiExample();
	public:
		void Update(const Metrics::blTime& gameTime) override;
		void Draw() override;
		virtual LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		bool mSwapChainOccluded;
		bool showDemoWindow;
	};
}