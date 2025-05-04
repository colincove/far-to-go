#pragma once

#include <blDX12.h>
#include <blTime.h>
#include <memory>
#include <vector>

namespace BoulderLeaf::Graphics::DX12
{
	class AbstractExample
	{
	public:
		AbstractExample(std::shared_ptr<DX12> dx12);
	public:
		virtual void Update(const Metrics::blTime& gameTime) = 0;
		virtual void Draw() = 0;
		virtual LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		std::shared_ptr<DX12> m_dx12;
	};
}