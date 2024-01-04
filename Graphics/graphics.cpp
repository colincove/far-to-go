#include "blHeadless.h"

using namespace BoulderLeaf::Graphics;

int TestInitialize(API* api)
{
    api->Initialize();
    return 1;
}

int main()
{
    Headless* graphics = new Headless();
    return TestInitialize(graphics);
}