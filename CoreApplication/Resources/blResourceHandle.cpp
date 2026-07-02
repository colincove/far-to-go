#include "blResourceHandle.h"

namespace BoulderLeaf
{
	// blResourceRef implementations

	blResourceRef::blResourceRef() : blResourceRef(blResourceHandle::Invalid)
	{
	}

	blResourceRef::blResourceRef(blResourceId id)
		: mId(id)
	{
	}

	blResourceRef::blResourceRef(const blResourceHandle& resource)
		: blResourceRef(resource.mId)
	{
	}

	blResourceId blResourceRef::GetId() const
	{
		return mId;
	}

	bool blResourceRef::operator==(const blResourceRef& other) const
	{
		return mId.mValue == other.mId.mValue && mId.mContainerId == other.mId.mContainerId;
	}

	bool blResourceRef::operator!=(const blResourceRef& other) const
	{
		return !(*this == other);
	}

	bool blResourceRef::IsValid() const
	{
		return mId.IsValid();
	}
}
