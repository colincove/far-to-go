#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"
#include "imgui.h"
#include <blWindow.h>
#include <memory>
#include <blDX12.h>

using namespace BoulderLeaf;
using namespace BoulderLeaf::Graphics;

int main()
{	
	Metrics::LoadPIX();
	std::shared_ptr<Core::blWindow> window(std::make_shared<Core::blWindow>("Graphics Demo"));
	std::shared_ptr<Graphics::API> api(std::make_shared<DX12::blDX12>(window));
	return 1;
}