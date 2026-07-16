#include <gtest/gtest.h>
#include <Resources/blResources.h>
#include <Resources/blResourceHandleOfType.h>

namespace BoulderLeaf::Resources
{
    TEST(CoreApplication, ResourceContainer)
    {
        blResourceContainerPool pool;
        blResourceContainer* container = pool.GetContainer(
            1000000 * 256, // 256mb
            10000
        );

        struct Test
        {
            int a;
            int b;
            int c;
        };

        blResourceHandleOfType<Test> TestResource = container->CreateResourceOfType<Test>(L"Test");
        Test& data = *TestResource;
        data.a = 10;
        data.b = 30;
        data.c = 1;
        const std::wstring_view name = TestResource.GetName();

        blResourceHandleOfType<Test> TestResource2 = container->CreateResourceOfType<Test>(L"Test 2");
        Test& data2 = *TestResource2;
        data2.a = 14;
        data2.b = 32;
        data2.c = 12;
        const std::wstring_view name2 = TestResource2.GetName();


        EXPECT_EQ(10, TestResource->a);
        EXPECT_EQ(30, TestResource->b);
        EXPECT_EQ(1, TestResource->c);
        EXPECT_EQ(L"Test", name);

        EXPECT_EQ(14, TestResource2->a);
        EXPECT_EQ(32, TestResource2->b);
        EXPECT_EQ(12, TestResource2->c);
        EXPECT_EQ(L"Test 2", name2);
    }
}