#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class Headless : public API
	{
	public:
		void Initialize(HWND hMainWnd);
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	};
}