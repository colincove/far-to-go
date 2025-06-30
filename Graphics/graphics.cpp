#include <blHeadless.h>

using namespace BoulderLeaf::Graphics;

int TestInitialize(API* api)
{
    HWND mhMainWnd = ::CreateWindowA("STATIC", "Test", WS_VISIBLE, 0, 0, 1024, 768, NULL, NULL, NULL, NULL);
    api->Initialize(mhMainWnd);
    return 1;
}

int main()
{
    Headless* graphics = new Headless();
    return TestInitialize(graphics);
}