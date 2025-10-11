#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class Headless : public API
	{
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	};
}