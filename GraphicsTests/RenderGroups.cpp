#include <gtest/gtest.h>
#include <blRenderGroup.h>
#include <set>

namespace
{
	using namespace BoulderLeaf::Graphics;
	using namespace BoulderLeaf;

	BL_STATIC_RENDER_GROUP(Objects, "Objects");
	BL_STATIC_RENDER_GROUP(UI, "UI");
	BL_STATIC_RENDER_GROUP(Effects, "Effects");
	BL_STATIC_RENDER_GROUP(Landscape, "Landscape");

	constexpr size_t ExpectedCount = 5; // Default + 4 registered groups
	static std::set<blRenderGroupId> SetAllGroups = { blRenderGroups::Default, Objects, UI, Effects, Landscape };

	TEST(RenderGroup, Count)
	{
		EXPECT_EQ(blRenderGroups::GetCount(), ExpectedCount);
	}

	TEST(RenderGroup, Unique)
	{
		EXPECT_EQ(SetAllGroups.size(), ExpectedCount);
	}

	TEST(RenderGroup, Iterate)
	{
		int arrayIndex[ExpectedCount];
		std::set<blRenderGroupId> Set;
		size_t count = 0;
		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			EXPECT_TRUE(SetAllGroups.find(group) != SetAllGroups.end());
			EXPECT_TRUE(Set.find(group) == Set.end());
			Set.insert(group);
			arrayIndex[group] = count;
			count++;
		}

		EXPECT_EQ(count, ExpectedCount);

		int expectedArrayIndexValue = 0;
		for (int value : arrayIndex)
		{
			EXPECT_EQ(value, expectedArrayIndexValue);
			expectedArrayIndexValue++;
		}
	}
}