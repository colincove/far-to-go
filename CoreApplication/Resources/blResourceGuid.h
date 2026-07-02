#pragma once

#include <combaseapi.h>

namespace BoulderLeaf
{
	struct blResourceGuid
	{
		GUID mValue;
		static blResourceGuid New();
		bool IsValid() const;
		void Clear();

		bool operator==(const blResourceGuid & other) const;
	};
}