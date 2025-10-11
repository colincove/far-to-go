#include <blResource.h>
#include <blResourceManager.h>

namespace BoulderLeaf::Graphics
{
	blResourceCollar::blResourceCollar() : mId(), mOwner(std::shared_ptr<blResourceManager>())
	{

	}

	blResourceCollar::blResourceCollar(blResourceId id, std::shared_ptr<blResourceManager> owner)
		:mId(id), mOwner(owner)
	{

	}

	blResourceCollar::~blResourceCollar()
	{
		if (auto owner = mOwner.get())
		{
			owner->ReleaseResource(*this);
		}
	}
}