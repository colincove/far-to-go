#include <blResourceManager.h>

namespace BoulderLeaf::Graphics
{
	blResourceId blResourceManager::GetNewResourceId()
	{
		static size_t nextId = 0;
		blResourceId newId(nextId);
		++nextId;
		return newId;
	}

	bool blResourceManager::IsValidResource(blResourceId id)
	{
		return mReleasedResourceIds.find(id) == mReleasedResourceIds.end();
	}

	void blResourceManager::ReleaseResource(const blResourceId& id)
	{
		mReleasedResourceIds.insert(id);
	}

	blResourceManager& blResourceManager::Get()
	{
		static blResourceManager instance;
		return instance;
	}
}