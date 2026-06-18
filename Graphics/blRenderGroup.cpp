#include <blRenderGroup.h>

namespace BoulderLeaf::Graphics
{
	unsigned char blRenderGroups::sNextId = 0;
	blRenderGroupData blRenderGroups::Data[MaxValue];

	size_t blRenderGroups::GetCount() { return sNextId + 1; }
	blRenderGroupId blRenderGroups::RegisterExternal(std::string name)
	{
		unsigned char id = sNextId + 2;
		assert(id <= MaxValue);
		sNextId++;

		blRenderGroupId groupId = blRenderGroupId(id);

		blRenderGroupData& data = Data[groupId];
		data.Name = std::string(name);
		data.NameWide = std::wstring(name.begin(), name.end());

		return groupId;
	}

	blRenderGroupData& blRenderGroups::GetRenderGroupData(blRenderGroupId groupId)
	{
		if (groupId == blRenderGroups::Default)
		{
			static blRenderGroupData DefaultRenderGroupData =
			{
				"Default",
				L"Default"
			};

			return DefaultRenderGroupData;
		}

		assert(groupId.value > 0 && groupId.value <= MaxValue);
		return Data[groupId];
	}
}