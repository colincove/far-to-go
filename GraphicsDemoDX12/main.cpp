#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"
#include "imgui.h"
#include <blWindow.h>

using namespace BoulderLeaf;
using namespace BoulderLeaf::Graphics;

int main()
{	
	Metrics::LoadPIX();
	blWindow window("Graphics Demo");
	return 1;
}