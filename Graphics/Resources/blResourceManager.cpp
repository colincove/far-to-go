#include <blResourceManager.h>

namespace BoulderLeaf::Graphics
{
	void blResourceManager::UpdateResource(blResourceCollar& collar, std::shared_ptr<blResourceManager> manager)
	{
		if (collar.mId.mId == 0)
		{
			static size_t nextId = 0;
			collar = blResourceCollar(nextId, manager);
			++nextId;
		}
	}

	bool blResourceManager::IsValidResource(blResourceId id)
	{
		return mReleasedResourceIds.find(id) == mReleasedResourceIds.end();
	}

	void blResourceManager::ReleaseResource(const blResourceCollar& collar)
	{
		mReleasedResourceIds.insert(collar.mId);
	}
}