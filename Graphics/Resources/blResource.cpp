#include <blResource.h>
#include <blResourceManager.h>

namespace BoulderLeaf::Graphics
{
	blResourceId blResourceId::Invalid = blResourceId(0, false);

	std::array<const wchar_t*, static_cast<int>(eResourceType::Count)> kResourceTypeNames =
	{
		L"Scene",
		L"Mesh",
		L"Shader",
		L"Material",
		L"DataBuffer",
	};
}