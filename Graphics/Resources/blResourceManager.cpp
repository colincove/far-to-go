#include <blResourceManager.h>

#include <blConfig.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidxml.hpp>
#include <assert.h>

namespace BoulderLeaf::Graphics
{
	bool blResourceManager::Initialize()
	{
		if (mInitialized)
		{
			return false;
		}

		ReadConfig();

		for (unsigned int budget : mResourceTypeBudgets)
		{
			mFullResourceBudget += budget;
		}

		mResourceMetaData.resize(mFullResourceBudget);

		mInitialized = true;

		return true;
	}

	bool blResourceManager::ReadConfig()
	{
		using namespace rapidxml;

		std::vector<char> xmlBuffer;

		{
			std::string filepath = Core::GetConfigFilePath("resourceManager.xml");
			std::ifstream file(filepath.c_str(), std::ios::binary);
			if (!file.is_open())
			{
				return false;
			}

			// Initialize budgets to 0
			mResourceTypeBudgets.fill(0u);

			// Read whole file into a std::string (uses heap, not large stack)
			file.seekg(0, std::ios::end);
			std::streamoff size = file.tellg();

			if (size <= 0)
			{
				return false;
			}
				
			file.seekg(0, std::ios::beg);

		
			xmlBuffer.resize(static_cast<size_t>(size));
			file.read(&xmlBuffer[0], size);
			xmlBuffer.push_back('\0');
		}

		xml_document<> doc;

		try
		{
			doc.parse<0>(xmlBuffer.data());
		}
		catch (...)
		{
			// parse failed
			return false;
		}

		auto stringToType = [](const std::string& s) -> std::optional<eResourceType> {
			if (s == "Scene") return eResourceType::Scene;
			if (s == "Mesh") return eResourceType::Mesh;
			if (s == "Shader") return eResourceType::Shader;
			if (s == "Material") return eResourceType::Material;
			if (s == "DataBuffer") return eResourceType::DataBuffer;
			return std::nullopt;
		};

		if (xml_node<>* resourceManager = doc.first_node("resource_manager"))
		{
			if (xml_node<>* budgets = resourceManager->first_node("budgets"))
			{
				for (xml_node<>* budget = budgets->first_node("budget");
					budget != nullptr;
					budget = budget->next_sibling("budget"))
				{
					std::string typeStr;
					std::string valueStr;

					if (xml_attribute<>* attr = budget->first_attribute("type"))
					{
						typeStr = attr->value();
					}
					else
					{
						continue;
					}

					// value could be either node value or attribute "value"
					if (budget->value() && budget->value()[0] != '\0')
					{
						valueStr = budget->value();
					}
					else if (xml_attribute<>* valAttr = budget->first_attribute("value"))
					{
						valueStr = valAttr->value();
					}

					if (typeStr.empty() || valueStr.empty())
					{
						continue;
					}

					std::optional<eResourceType> typeOpt = stringToType(typeStr);
					if (!typeOpt.has_value())
					{
						continue;
					}
					
					try
					{
						unsigned int val = static_cast<unsigned int>(std::stoul(valueStr));
						mResourceTypeBudgets[static_cast<int>(typeOpt.value())] = val;
					}
					catch (...)
					{
						// ignore malformed values
						continue;
					}
				}
				return true;
			}
		}

		return false;
	}

	blResourceId blResourceManager::GetNewResourceId()
	{
		assert(mInitialized);

		static size_t nextId = 0;
		blResourceId newId(nextId);
		++nextId;
		return newId;
	}

	blResourceId blResourceManager::GetNewResourceIdForType(eResourceType type)
	{
		static std::array<size_t, static_cast<size_t>(eResourceType::Count)> nextIds = { 0 };
		size_t& nextId = nextIds[static_cast<size_t>(type)];

		if (nextId < mResourceTypeBudgets[static_cast<size_t>(type)])
		{
			blResourceId newId(nextId);
			++nextId;
			return newId;
		}
		else
		{
			// Budget exceeded, return an invalid ID
			return blResourceId::Invalid;
		}
	}

	bool blResourceManager::IsValidResource(blResourceId id)
	{
		return mReleasedResourceIds.find(id) == mReleasedResourceIds.end();
	}

	void blResourceManager::ReleaseResource(const blResourceId& id)
	{
		mResourceMap.erase(id);
		mReleasedResourceIds.insert(id);
	}

	blResourceManager& blResourceManager::Get()
	{
		static blResourceManager instance;
		return instance;
	}
}