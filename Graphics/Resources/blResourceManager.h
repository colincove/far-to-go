#pragma once

#include <memory>
#include <Resources/blResource.h>
#include <vector>
#include <set>

namespace BoulderLeaf::Graphics
{
	class blResourceManager;

	class blResourceManager
	{
	private:
		// TODO: Implement a method of resource id recycling
		std::set<blResourceId> mReleasedResourceIds;
	public:
		static void UpdateResource(blResourceCollar& collar, std::shared_ptr<blResourceManager> manager);
		void ReleaseResource(const blResourceCollar& collar);
		bool IsValidResource(blResourceId id);
	};
}
