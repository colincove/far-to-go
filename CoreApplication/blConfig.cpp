#include <blConfig.h>

#include <filesystem>

namespace
{
	const std::string kConfigDirectory = "config/";
}

namespace BoulderLeaf::Core
{
	std::string GetConfigFilePath(const std::string& fileName)
	{
		std::filesystem::path cwd = std::filesystem::current_path();
		return cwd.generic_string() + "/" + kConfigDirectory + fileName;
	}
}