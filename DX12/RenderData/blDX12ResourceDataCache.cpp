#include "blDX12ResourceDataCache.h"

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ResourceCacheBase::blDX12ResourceCacheBase()
		:mCacheMetaData()
	{
	}

	bool blDX12ResourceCacheBase::IsCacheInitialized(const blResourceId id) const
	{
		if (id.GetRaw() >= mCacheMetaData.size())
		{
			return false;
		}

		return mCacheMetaData[id.GetRaw()].Initialized;
	}

	void blDX12ResourceCacheBase::SetCacheInitialized(const blResourceId id, bool value)
	{
		if (id.GetRaw() >= mCacheMetaData.size())
		{
			mCacheMetaData.resize(id.GetRaw() + 1);
		}

		mCacheMetaData[id.GetRaw()].Initialized = value;
	}
}